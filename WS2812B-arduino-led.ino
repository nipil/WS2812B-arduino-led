#include <digitalWriteFast.h>

#define WS2812B_DATA_PIN 5
#define TRIGGER_PIN 6
#define AUX_PIN 7

#define LED_COUNT 24

typedef struct {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} PIXEL;

typedef PIXEL LED_STRIP[LED_COUNT];

LED_STRIP global_leds;

void set_color(LED_STRIP& leds, uint8_t red, uint8_t green, uint8_t blue) {
  uint8_t count = sizeof(LED_STRIP) / sizeof(PIXEL);
  for (uint8_t i = 0; i < count; i++) {
    leds[i].red = red;
    leds[i].green = green;
    leds[i].blue = blue;
  }
}

inline void WS2812_write_bit(uint8_t value) __attribute__((always_inline));

void WS2812_write_bit(uint8_t value) {
  digitalWriteFast(WS2812B_DATA_PIN, HIGH);
  digitalWriteFast(WS2812B_DATA_PIN, HIGH);
  digitalWriteFast(WS2812B_DATA_PIN, HIGH);
  digitalWriteFast(WS2812B_DATA_PIN, value);
  digitalWriteFast(WS2812B_DATA_PIN, value);
  digitalWriteFast(WS2812B_DATA_PIN, value);
  digitalWriteFast(WS2812B_DATA_PIN, value);
  digitalWriteFast(WS2812B_DATA_PIN, LOW);
  digitalWriteFast(WS2812B_DATA_PIN, LOW);
  digitalWriteFast(WS2812B_DATA_PIN, LOW);
}

inline void WS2812_write_one() __attribute__((always_inline));

void WS2812_write_one() {
  digitalWriteFast(WS2812B_DATA_PIN, HIGH);
  digitalWriteFast(WS2812B_DATA_PIN, HIGH);
  digitalWriteFast(WS2812B_DATA_PIN, HIGH);
  digitalWriteFast(WS2812B_DATA_PIN, HIGH);
  digitalWriteFast(WS2812B_DATA_PIN, HIGH);
  digitalWriteFast(WS2812B_DATA_PIN, HIGH);
  digitalWriteFast(WS2812B_DATA_PIN, HIGH);
  digitalWriteFast(WS2812B_DATA_PIN, LOW);
  digitalWriteFast(WS2812B_DATA_PIN, LOW);
  digitalWriteFast(WS2812B_DATA_PIN, LOW);
}

inline void WS2812_write_zero() __attribute__((always_inline));

void WS2812_write_zero() {
  digitalWriteFast(WS2812B_DATA_PIN, HIGH);
  digitalWriteFast(WS2812B_DATA_PIN, HIGH);
  digitalWriteFast(WS2812B_DATA_PIN, HIGH);
  digitalWriteFast(WS2812B_DATA_PIN, LOW);
  digitalWriteFast(WS2812B_DATA_PIN, LOW);
  digitalWriteFast(WS2812B_DATA_PIN, LOW);
  digitalWriteFast(WS2812B_DATA_PIN, LOW);
  digitalWriteFast(WS2812B_DATA_PIN, LOW);
  digitalWriteFast(WS2812B_DATA_PIN, LOW);
  digitalWriteFast(WS2812B_DATA_PIN, LOW);
}

void setup() {
  pinModeFast(WS2812B_DATA_PIN, OUTPUT);
  digitalWriteFast(WS2812B_DATA_PIN, LOW);
  pinModeFast(TRIGGER_PIN, OUTPUT);
  digitalWriteFast(TRIGGER_PIN, LOW);
  pinModeFast(AUX_PIN, OUTPUT);
  digitalWriteFast(AUX_PIN, LOW);
  set_color(global_leds, 64, 128, 255);
}

void loop() {
  digitalWriteFast(TRIGGER_PIN, HIGH);
  digitalWriteFast(TRIGGER_PIN, LOW);

  WS2812_write_one();
  digitalWriteFast(AUX_PIN, HIGH);
  digitalWriteFast(AUX_PIN, LOW);
  WS2812_write_zero();
  digitalWriteFast(AUX_PIN, HIGH);
  digitalWriteFast(AUX_PIN, LOW);
  WS2812_write_bit(HIGH);
  digitalWriteFast(AUX_PIN, HIGH);
  digitalWriteFast(AUX_PIN, LOW);
  WS2812_write_bit(LOW);
  digitalWriteFast(AUX_PIN, HIGH);
  digitalWriteFast(AUX_PIN, LOW);

  for (int i = 0; i < 16; i++) {
    delay(1);
    digitalWriteFast(WS2812B_DATA_PIN, HIGH);
    delay(1);
    digitalWriteFast(WS2812B_DATA_PIN, LOW);
  }
  digitalWriteFast(AUX_PIN, HIGH);
  digitalWriteFast(AUX_PIN, LOW);
}
