#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN 6
#define BTN_LEFT_PIN 3
#define BTN_RIGHT_PIN 4
#define MAX_PLATFORM_VELOCITY 1
int SideNum = 8;
int NUMPIXELS = SideNum * SideNum;
int lenPlatform = 3;
float xOfPlatform = (SideNum - lenPlatform) / 2;
float platformVelocity = 0.0;
bool isGameOver = false;

float BallX = SideNum / 2;
float BallY = SideNum / 2;
float BallXVelocity = 0.5;
float BallYVelocity = 0.5;

int fps = 7;


Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 200 // Time (in milliseconds) to pause between pixels

void setup() {
  pinMode(BTN_LEFT_PIN, INPUT_PULLUP);
  pinMode(BTN_RIGHT_PIN, INPUT_PULLUP);
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.
  pixels.begin();
}

void XYpixelZigZakSet(float x, float y, int red, int green, int blue) {
  int i = int(x) + (int(y) * SideNum);
  if (int(y) % 2 != 0) {
    i = (SideNum - int(x) - 1) + (int(y) * SideNum);
  }
  pixels.setPixelColor(i, pixels.Color(red, green, blue));
}

void drawPlatform() {
  for (int i = 0; i < lenPlatform; i++) {
    XYpixelZigZakSet(xOfPlatform + i, 0, 0, 255, 0);
  }
}

void drawBall() {
  float newX = BallX + BallXVelocity;
  float newY = BallY + BallYVelocity;
  if (newX >= SideNum || newX < 0.0) {
    BallXVelocity = -BallXVelocity;
    newX = BallX + BallXVelocity;
  }
  if (newY >= SideNum) {
    BallYVelocity = -BallYVelocity;
    newY = BallY + BallYVelocity;
  }
  if (int(newY) == 0 && int(xOfPlatform) < int(newX) && int(newX) < int(xOfPlatform + lenPlatform)) {
    BallXVelocity = -BallXVelocity;
    newX = BallX + BallXVelocity;
    BallYVelocity = -BallYVelocity;
    newY = BallY + BallYVelocity;
  }
  if (newY < 0) {
    isGameOver = true;
  }
  BallX = newX;
  BallY = newY;
  XYpixelZigZakSet(BallX, BallY, 255, 0, 0);
}

void controllers() {
  bool btnleft = digitalRead(BTN_LEFT_PIN);
  bool btnright = digitalRead(BTN_RIGHT_PIN);
  if (btnleft == LOW) {
    platformVelocity = -MAX_PLATFORM_VELOCITY;
  } else if (btnright == LOW) {
    platformVelocity = MAX_PLATFORM_VELOCITY;
  } else {
    platformVelocity = 0;
  }
  xOfPlatform += platformVelocity;
}

void autoPlatform() {
  xOfPlatform += 0.1;
  if (xOfPlatform > 8 - 3) {
    xOfPlatform = 0;
  }
}


void loop() {
  pixels.clear();
  if (isGameOver) {
    for (int x = 0; x < SideNum; x++) {
      for (int y = 0; y < SideNum; y++) {
        XYpixelZigZakSet(x, y, 255, 0, 0);
      }
    }
    pixels.show();
    delay(3000);
    isGameOver = false;
    BallX = SideNum / 2;
    BallY = SideNum / 2;
    BallXVelocity = 0.1;
    BallYVelocity = 1;
  }
  else {
    controllers();
    drawPlatform();
    autoPlatform();
    drawBall();
    pixels.show();
    int sleeptime = 1000 / fps;
    delay(sleeptime);
  }
}
