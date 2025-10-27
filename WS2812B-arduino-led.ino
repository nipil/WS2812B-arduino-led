#include <digitalWriteFast.h>

#define WS2812B_DATA_PIN 5
#define TRIGGER_PIN 6
#define AUX_PIN 7

#define LED_COUNT 24

typedef struct {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} LED;

typedef LED LEDS[LED_COUNT];

LEDS global_leds;

uint8_t reverse_bits(uint8_t value) {
  // see https://stackoverflow.com/a/2602885/5973357
  value = (value & 0xF0) >> 4 | (value & 0x0F) << 4;
  value = (value & 0xCC) >> 2 | (value & 0x33) << 2;
  value = (value & 0xAA) >> 1 | (value & 0x55) << 1;
  return value;
}

void set_color(LEDS& leds, const uint8_t red, const uint8_t green, const uint8_t blue) {
  uint8_t count = sizeof(LEDS) / sizeof(LED);
  for (uint8_t i = 0; i < count; i++) {
    leds[i].red = reverse_bits(red);
    leds[i].green = reverse_bits(green);
    leds[i].blue = reverse_bits(blue);
  }
}

inline void noop() __attribute__((always_inline));

void noop_tick() {
  uint8_t dummy = PORTB;
}

inline void WS2812_write_bit(const uint8_t bit) __attribute__((always_inline));

void WS2812_write_bit(const uint8_t bit) {
  digitalWriteFast(WS2812B_DATA_PIN, HIGH);
  noop_tick();
  noop_tick();
  noop_tick();
  noop_tick();
  digitalWriteFast(WS2812B_DATA_PIN, bit);
  noop_tick();
  noop_tick();
  noop_tick();
  noop_tick();
  noop_tick();
  noop_tick();
  digitalWriteFast(WS2812B_DATA_PIN, LOW);
  noop_tick();
  noop_tick();
  noop_tick();
  noop_tick();
}

inline void WS2812_write_byte(const uint8_t value) __attribute__((always_inline));

void WS2812_write_byte(uint8_t value) {
  // write from LSB, as byte bits must have been reversed beforehand
  WS2812_write_bit(value & 0x01);
  value >>= 1;
  WS2812_write_bit(value & 0x01);
  value >>= 1;
  WS2812_write_bit(value & 0x01);
  value >>= 1;
  WS2812_write_bit(value & 0x01);
  value >>= 1;
  WS2812_write_bit(value & 0x01);
  value >>= 1;
  WS2812_write_bit(value & 0x01);
  value >>= 1;
  WS2812_write_bit(value & 0x01);
  value >>= 1;
  WS2812_write_bit(value & 0x01);
  value >>= 1;
}

inline void WS2812_write_led(const LED& led) __attribute__((always_inline));

void WS2812_write_led(const LED& led) {
  WS2812_write_byte(led.green);
  WS2812_write_byte(led.red);
  WS2812_write_byte(led.blue);
}

inline void WS2812_write_leds(const LEDS& leds) __attribute__((always_inline));

void WS2812_write_leds(const LEDS& leds) {
  uint8_t count = sizeof(LEDS) / sizeof(LED);
  for (uint8_t i = 0; i < count; i++) {
    WS2812_write_led(leds[i]);
  }
}

void setup() {
  Serial.begin(9600);
  pinModeFast(WS2812B_DATA_PIN, OUTPUT);
  digitalWriteFast(WS2812B_DATA_PIN, LOW);
  pinModeFast(TRIGGER_PIN, OUTPUT);
  digitalWriteFast(TRIGGER_PIN, LOW);
  pinModeFast(AUX_PIN, OUTPUT);
  digitalWriteFast(AUX_PIN, LOW);
}

void loop() {

  uint8_t green = 0, red = 0, blue = 0;
  uint32_t value = (millis() / 1000) % 3;
  switch (value) {
    case 0:
      red = 0xFF;
      break;
    case 1:
      green = 0xFF;
      break;
    case 2:
      blue = 0XFF;
      break;
  }

  // Serial.print("value=");
  // Serial.print(value);
  // Serial.print(" red=");
  // Serial.print(red);
  // Serial.print(" green=");
  // Serial.print(red);
  // Serial.print(" blue=");
  // Serial.println(blue);

  noInterrupts();

  digitalWriteFast(TRIGGER_PIN, HIGH);
  digitalWriteFast(TRIGGER_PIN, LOW);

  // for (uint8_t i = 0; i < LED_COUNT; i++) {
  WS2812_write_byte(green);
  WS2812_write_byte(red);
  WS2812_write_byte(blue);

  WS2812_write_byte(0);
  WS2812_write_byte(0xFF);
  WS2812_write_byte(0);
  // }

  // delayMicroseconds(10);
  delayMicroseconds(100);
  // delay(1);
  // delay(10);
  // delay(100);

  digitalWriteFast(AUX_PIN, HIGH);
  digitalWriteFast(AUX_PIN, LOW);

  interrupts();
}
