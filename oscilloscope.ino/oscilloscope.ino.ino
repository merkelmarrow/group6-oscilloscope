#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <CircularBuffer.hpp>
 

constexpr int analogPin = A0;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_MOSI   3
#define OLED_CLK   2
#define OLED_DC    6
#define OLED_CS    7
#define OLED_RESET 8

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_DC, OLED_RESET, OLED_CS);

// Circular buffer to store last 128 values
// May be useful for triggers/scaling later
// CircularBuffer<int, 128> buffer;
int 

// X coordinate to plot on display
int x = 0;

void setup() {
  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_RESET)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Loop forever
  }

  // Display adafruit logo for 2 seconds to show if display is working
  display.display();
  delay(2000);
  display.clearDisplay();
}

void loop() {
  float val = analogRead(analogPin);
  float voltage = 5.0 * (val / 1023.0);

  // Centre voltage around 2.5V and scale to display height
  // We have to condition the signal to be centred at approximately 2.5V
  float pre_y = voltage - 2.5;

  // If max signal is 2.5V from centre (5V in total)
  // then 5V should be the highest y (0.8 * height)
  //    calcluation: (2.5 / 2.5) * (height * 0.3) + (height / 2)
  //    = 0.3 * height + 0.5 * height
  //    = 0.8 * height
  // and 0V should be the lowest y (0.2 height)
  //    calculation: (-2.5 / 2.5) * (height * 0.3) + (height / 2)
  //    = -0.3 * height + 0.5 * height
  //    = 0.2 * height
  int y = (int) ((pre_y / 2.5) * (SCREEN_HEIGHT * 0.3) + (SCREENHEIGHT / 2));

  // shouldn't need this if we condition the signal properly
  y = constrain(y, 0, SCREEN_HEIGHT - 1);

  // When we reach the end of the display, pause the display for a second
  // (to give time to look at the signal)
  if (x == SCREEN_WIDTH - 1) {
    display.display();
    delay(1000);
    display.clearDisplay();
  } else {
    display.drawPixel(x, y, SSD1306_WHITE);
    display.display(); // Refresh screen with new pixel
  }

  // Increment x and reset if reaches screen width
  x = (x + 1) % SCREEN_WIDTH;
}
