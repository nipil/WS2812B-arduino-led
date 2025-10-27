#define THROW_ERROR_IF_NOT_FAST  // If activated, an error is thrown if pin is not a compile time constant

#include <digitalWriteFast.h>

#define WS2812B_DATA_PIN 5
#define TRIGGER_PIN 6
#define AUX_PIN 7

#define LED_COUNT 24

#define WS2812B_DATA_PIN 5
#define TRIGGER_PIN 6
#define AUX_PIN 7

#define LED_COUNT 24

uint8_t reverse_bits(uint8_t value) {
  // see https://stackoverflow.com/a/2602885/5973357
  value = (value & 0xF0) >> 4 | (value & 0x0F) << 4;
  value = (value & 0xCC) >> 2 | (value & 0x33) << 2;
  value = (value & 0xAA) >> 1 | (value & 0x55) << 1;
  return value;
}

inline void WS2812_write_bit(const uint8_t bit) __attribute__((always_inline));

void WS2812_write_bit(const uint8_t bit) {
  digitalWriteFast(WS2812B_DATA_PIN, HIGH);
  digitalWriteFast(WS2812B_DATA_PIN, bit);
  digitalWriteFast(WS2812B_DATA_PIN, bit);
  digitalWriteFast(WS2812B_DATA_PIN, bit);
  digitalWriteFast(WS2812B_DATA_PIN, bit);
  digitalWriteFast(WS2812B_DATA_PIN, bit);
  digitalWriteFast(WS2812B_DATA_PIN, bit);
  digitalWriteFast(WS2812B_DATA_PIN, bit);
  digitalWriteFast(WS2812B_DATA_PIN, bit);
  digitalWriteFast(WS2812B_DATA_PIN, LOW);
}

inline void WS2812_write_byte(const uint8_t value) __attribute__((always_inline));

void WS2812_write_byte(uint8_t value) {
  // write from LSB, as byte bits must have been reversed beforehand for constant speed
  for (int i = 0; i < 8; i++) {
    WS2812_write_bit(value & 0x01);
    value >>= 1;
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
  static uint8_t last_intensity = 0;
  uint8_t colors[] = { 0, 0, 0 };
  uint32_t ms = millis();
  uint8_t intensity = (ms >> 6) & 0xFF;
  uint8_t value = (ms >> 10) % 3;

  if (ms < 2000) {
    colors[0] = colors[1] = colors[2] = 0xFF;
  } else {
    colors[value] = reverse_bits(intensity);
  }

  digitalWriteFast(TRIGGER_PIN, HIGH);
  digitalWriteFast(TRIGGER_PIN, LOW);

  noInterrupts();

  for (int i = 0; i < LED_COUNT; i++) {
    WS2812_write_byte(colors[0]);
    WS2812_write_byte(colors[1]);
    WS2812_write_byte(colors[2]);
  }

  interrupts();

  delay(1);

  if (intensity != last_intensity) {
    last_intensity = intensity;
    Serial.println(intensity);
  }
}
