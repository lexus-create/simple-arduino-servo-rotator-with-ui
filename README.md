# simple-arduino-servo-rotator-with-ui
A  program that uses Arduino Nano to rotate a Micro Servo. It has a simple OLED UI. Hardware wiring can be found in readme.

OLED SDA → A4
OLED SCL → A5
OLED VCC → 5V
OLED GND → GND
Servo signal → D9
Servo VCC → 5V
Servo GND → GND
Select button → D3, other leg to GND
Active button → D4, other leg to GND

Select toggles which way the servo rotates. The active button is held down to make the servo actually rotate.

