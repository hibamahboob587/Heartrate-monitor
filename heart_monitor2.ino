#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MAX30105.h>
#include "heartRate.h" 

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

MAX30105 particleSensor;

#define SMOOTHING_WINDOW 5  // Number of samples for moving average
int bpmBuffer[SMOOTHING_WINDOW];
int bufferIndex = 0;
int beatAvg = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Initializing...");
  display.display();

  if (!particleSensor.begin(Wire, I2C_SPEED_STANDARD)) {
    Serial.println("MAX30102 not found. Please check connections.");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("MAX30102 not found!");
    display.display();
    while (1);
  }

  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x0A); // Red LED low
  particleSensor.setPulseAmplitudeGreen(0);  // Green LED off

  // Initialize buffer
  for (int i = 0; i < SMOOTHING_WINDOW; i++) bpmBuffer[i] = 0;
}

void loop() {
  long irValue = particleSensor.getIR();

  if (irValue < 50000) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("No finger detected");
    display.display();
    delay(500);
    return;
  }

  if (checkForBeat(irValue)) {
    static uint32_t lastBeat = 0;
    uint32_t now = millis();
    uint32_t delta = now - lastBeat;
    lastBeat = now;

    float bpm = 60.0 / (delta / 1000.0);
    if (bpm > 20 && bpm < 255) {
      // Add to buffer
      bpmBuffer[bufferIndex] = (int)bpm;
      bufferIndex = (bufferIndex + 1) % SMOOTHING_WINDOW;

      // Compute moving average
      int sum = 0;
      for (int i = 0; i < SMOOTHING_WINDOW; i++) {
        sum += bpmBuffer[i];
      }
      beatAvg = sum / SMOOTHING_WINDOW;

      // Display
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("Heart Rate:");
      display.setTextSize(2);
      display.setCursor(0, 20);
      display.print(beatAvg);
      display.println(" BPM");
      display.display();

      Serial.print("Smoothed BPM: ");
      Serial.println(beatAvg);
    }
  }
}
