#include <digitalWriteFast.h>

#define WS2812B_DATA_PIN 5
#define TRIGGER_PIN 6
#define AUX_PIN 7

void setup() {
  pinModeFast(WS2812B_DATA_PIN, OUTPUT);
  digitalWriteFast(WS2812B_DATA_PIN, LOW);
  pinModeFast(TRIGGER_PIN, OUTPUT);
  digitalWriteFast(TRIGGER_PIN, LOW);
  pinModeFast(AUX_PIN, OUTPUT);
  digitalWriteFast(AUX_PIN, LOW);
}

void loop() {
  digitalWriteFast(TRIGGER_PIN, HIGH);
  digitalWriteFast(TRIGGER_PIN, LOW);
  for (int i = 0; i < 16; i++) {
    delay(1);
    digitalWriteFast(WS2812B_DATA_PIN, HIGH);
    delay(1);
    digitalWriteFast(WS2812B_DATA_PIN, LOW);
  }
  digitalWriteFast(AUX_PIN, HIGH);
  digitalWriteFast(AUX_PIN, LOW);
}
