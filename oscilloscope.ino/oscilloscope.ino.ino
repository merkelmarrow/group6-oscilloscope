#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

constexpr int SIG_READ_PIN = A0;
constexpr int SIG_GEN_PIN = 4;

#define OLED_DC 5
#define OLED_CS 6
#define OLED_RESET 7

/* OTHER PINS
MOSI -> 11
SLK/CLK -> 13
*/

// Using hardware SPI
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_DC, OLED_RESET, OLED_CS);



unsigned long prev_millis_out = 0;
unsigned long prev_millis_in = 0;
bool sig_state = true;

#define OUT_INTERVAL 200
#define IN_INTERVAL 16

int16_t x = 0;

int16_t prev_x = 10;
int16_t prev_y = 10;

void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  // Display adafruit logo for 2 seconds to show if display is working
  display.display();
  delay(2000);
  display.clearDisplay();

  pinMode(SIG_GEN_PIN, OUTPUT);
  digitalWrite(SIG_GEN_PIN, sig_state);
  pinMode(SIG_READ_PIN, INPUT);
}

void loop() {
  unsigned long current_millis = millis();

  if (current_millis - prev_millis_out >= OUT_INTERVAL) {
    prev_millis_out = current_millis;
    sig_state = !sig_state;
    digitalWrite(SIG_GEN_PIN, sig_state);
  }

  if (current_millis - prev_millis_in >= IN_INTERVAL) {
    prev_millis_in = current_millis;
    int analog_val = analogRead(SIG_READ_PIN);
    int16_t pixel_y = map(analog_val, 0, 1023, 10, 54);
    display.drawLine(prev_x, prev_y, x, pixel_y, SSD1306_WHITE);
    display.display();
    if (x >= 127) {
      display.clearDisplay();
      x = 0;
      prev_x = 0;
    } else {
      prev_x = x++;
    }
    prev_y = pixel_y;
  }
}
