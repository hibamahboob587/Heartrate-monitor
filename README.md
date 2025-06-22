# Heart Rate Monitor

An Arduino-based heart rate monitor using the MAX30102 sensor and SSD1306 OLED display.

##  Hardware Components
- Arduino Nano
- MAX30102 Pulse Sensor
- SSD1306 OLED Display (I2C, 128x64)
- 4.7kΩ Pull-up Resistors (for SDA and SCL lines)
- Breadboard and jumper wires

##  Description
This project reads heart rate using the MAX30102 sensor and displays the average BPM on the OLED screen.

##  Connections
- MAX30102 → SDA, SCL → Arduino A4, A5 (with 4.7kΩ pull-ups)
- OLED → SDA, SCL → Shared with MAX30102
- Power → 3.3V / GND
