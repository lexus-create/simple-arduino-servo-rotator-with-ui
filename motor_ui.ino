#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "activity_64_64_28f.h"
#include <Servo.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

Servo myServo;
const int selectbut = 3;
const int activebut = 4;

bool hasstarted = false;
bool hasleftselected = false;
bool hasrightselected = false;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

static const unsigned char PROGMEM image_ButtonLeft_bits[] = {0x10,0x30,0x70,0xf0,0x70,0x30,0x10};
static const unsigned char PROGMEM image_ButtonRight_bits[] = {0x80,0xc0,0xe0,0xf0,0xe0,0xc0,0x80};
static const unsigned char PROGMEM image_FaceCharging_bits[] = {0x02,0x00,0x00,0x40,0x06,0x00,0x00,0xc0,0x0c,0x00,0x01,0x80,0x18,0x00,0x03,0x00,0x3f,0x00,0x07,0xe0,0x3f,0x00,0x07,0xe0,0x06,0x00,0x00,0xc0,0x0c,0x00,0x01,0x80,0x18,0x00,0x03,0x00,0x10,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x00,0x00,0x10,0x40,0x00,0x00,0x0f,0x80,0x00};
static const unsigned char PROGMEM image_ButtonLeft_copy_1_bits[] = {0x10,0x30,0x70,0xf0,0x70,0x30,0x10};
static const unsigned char PROGMEM image_ButtonRight_copy_1_bits[] = {0x80,0xc0,0xe0,0xf0,0xe0,0xc0,0x80};
static const unsigned char PROGMEM image_FaceCharging_copy_1_bits[] = {0x02,0x00,0x00,0x40,0x06,0x00,0x00,0xc0,0x0c,0x00,0x01,0x80,0x18,0x00,0x03,0x00,0x3f,0x00,0x07,0xe0,0x3f,0x00,0x07,0xe0,0x06,0x00,0x00,0xc0,0x0c,0x00,0x01,0x80,0x18,0x00,0x03,0x00,0x10,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x00,0x00,0x10,0x40,0x00,0x00,0x0f,0x80,0x00};

unsigned long lastSelectPress = 0;
unsigned long lastActivePress = 0;
const unsigned long debounceDelay = 125; //Adjust to your liking

void drawScreen_2(void) {
    display.clearDisplay();
    display.setTextColor(1);
    display.setTextWrap(false);
    display.setCursor(87, 30);
    display.print("Left");
    display.setCursor(86, 42);
    display.print("Right");
    display.setCursor(38, 5);
    display.print("Motor UI");
    display.drawBitmap(3, 1, image_FaceCharging_bits, 29, 14, 1);
    display.drawBitmap(80, 42, image_ButtonLeft_bits, 4, 7, 1);
    display.drawBitmap(117, 42, image_ButtonRight_bits, 4, 7, 1);
    display.display();
}

void drawScreen_3(void) {
    display.clearDisplay();
    display.setTextColor(1);
    display.setTextWrap(false);
    display.setCursor(87, 30);
    display.print("Left");
    display.setCursor(86, 42);
    display.print("Right");
    display.setCursor(38, 5);
    display.print("Motor UI");
    display.drawBitmap(3, 1, image_FaceCharging_copy_1_bits, 29, 14, 1);
    display.drawBitmap(82, 30, image_ButtonLeft_copy_1_bits, 4, 7, 1);
    display.drawBitmap(111, 30, image_ButtonRight_copy_1_bits, 4, 7, 1);
    display.display();
}

void drawScreen_1(void) {
    display.clearDisplay();
    display.display();
}

void updateAnimations() {
    activity_64_64_28f_frame = millis() / 42 % 28;
}

void drawAnimation_activity_64_64_28f(void) {
    display.fillRect(32, 0, 64, 64, 0);
    display.drawBitmap(32, 0, activity_64_64_28f_frames[activity_64_64_28f_frame], 64, 64, 1);
    display.display();
}

void setup() {
    Wire.begin();
    display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
    display.clearDisplay();

    myServo.attach(9);
    pinMode(selectbut, INPUT_PULLUP);
    pinMode(activebut, INPUT_PULLUP);

    // startup animation, once
    for (int i = 0; i < 28; i++) {
        activity_64_64_28f_frame = i;
        drawAnimation_activity_64_64_28f();
        delay(42);
    }

    drawScreen_1();
}

void loop() {
  int selectvalue = digitalRead(selectbut);
  int activevalue = digitalRead(activebut);
  unsigned long now = millis();

  if (hasstarted == false) {
    drawScreen_3();
    hasleftselected = true;
    hasstarted = true;
  }

  if (selectvalue == LOW && (now - lastSelectPress > debounceDelay)) {
    lastSelectPress = now;

    if (hasleftselected == true) {
      drawScreen_2();
      hasleftselected = false;
      hasrightselected = true;
    } else if (hasrightselected == true) {
      drawScreen_3();
      hasleftselected = true;
      hasrightselected = false;
    }
  }

  if (activevalue == LOW && (now - lastActivePress > debounceDelay)) {
    lastActivePress = now;

    if (hasleftselected == true) {
      int currentAngle = myServo.read();
      if (currentAngle > 0) {
        myServo.write(currentAngle - 3);
      }
    } else if (hasrightselected == true) {
      int currentAngle = myServo.read();
      if (currentAngle < 180) {
        myServo.write(currentAngle + 3);
      }
    }
  }
}