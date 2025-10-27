# WS2812B-arduino-led

Power consumption **WARNING** when powerinf the LEDS from the MCU board voltage regulator

- Each LED in the strip consumes up to 50mA when fully white and at full brightness
- If you power the strip/ring by the arduino, beware of total power consumption !
- For an arduino uno, the available current from the onboard 5V regulator is 400-500mA max
