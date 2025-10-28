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

// with 0xFF 24 leds consumes 602mA
// with 192, the same 24 leds should consume 450-500mA
#define MAX_WHITE_INTENSITY_WITH_BLUE_DIV_2 0xC0

void loop() {
  static uint8_t last_intensity = 0;
  uint8_t colors[] = { 0, 0, 0 };
  uint32_t ms = millis();
  uint8_t intensity = (ms >> 7) & 0xFF;
  uint8_t value = (ms >> 10) % 4;

  if (ms < 2000) {
    colors[0] = colors[1] = colors[2] = reverse_bits(0x10);  // 0 = 65mA
  } else if (ms < 4000) {
    colors[0] = colors[1] = colors[2] = reverse_bits(0x30);  // 1 = 175mA
  } else if (ms < 6000) {
    colors[0] = colors[1] = colors[2] = reverse_bits(0x50);  // 2 = 272mA
  } else if (ms < 8000) {
    colors[0] = colors[1] = colors[2] = reverse_bits(0x70);  // 3 = 353mA
  } else if (ms < 10000) {
    colors[0] = colors[1] = colors[2] = reverse_bits(0x90);  // 4 = 421mA
  } else if (ms < 12000) {
    colors[0] = colors[1] = colors[2] = reverse_bits(0xB0);  // 5 = 475mA
  } else if (ms < 14000) {
    colors[0] = colors[1] = colors[2] = reverse_bits(0xD0);  // 6 = 525mA
  } else if (ms < 16000) {
    colors[0] = colors[1] = colors[2] = reverse_bits(0xFF);  // 7 = 585mA
  } else {
    if (value == 0) {
      colors[0] = colors[1] = reverse_bits(MAX_WHITE_INTENSITY_WITH_BLUE_DIV_2);
      colors[2] = colors[0] >> 1;
    } else {
      colors[value - 1] = reverse_bits(intensity);
    }
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
