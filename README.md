# WS2812B-arduino-led

## WARNING: led strip power consumption

Do not power the  LEDS from the MCU board voltage regulator without prior current measurement !

- Each LED in the strip consumes up to 50mA when fully white and at full brightness
- If you power the strip/ring by the arduino, beware of total power consumption !
- For an arduino uno, the available current from the onboard 5V regulator is 400-500mA max
- Real world example : [this](https://aliexpress.com/item/1005007587825691.html) 24 led ring consumes +650mA when full white

## How does the protocol work

- Very interesting read about timing tolerance of WS2812 chips
  - <https://cpldcpu.com/2014/01/14/light_ws2812-library-v2-0-part-i-understanding-the-ws2812>
- For an UNO with a 16MHz crystal, using and oscilloscope :
  - ATMEGA328P digitalWriteFast duration is 125ns
  - `PORTB=PORTB;` is 125ns too
  - `uint8_t dummy=PORTB` is 188 ns (125ns + 62.5ns)
- for WS2812B
  - one cycle
    - data bus works at 800 KHz
    - so a bit frame is 1.25 μs ± 150ns = 1250ns ± 150ns
    - so it is 10 DWF ± 1 DWF
  - to send a 0
    - T0H = 0.35us ±150ns and T0L 0.9us ±150ns
    - so 3 HIGH DWF (375ns) then 7 LOW DWF (875ns)
  - to send a 1
    - T1H = 0.9us ±150ns and T1L 0.35us ±150ns
    - so 7 HIGH DWF (875ns) + 3 LOW DWF (375ns)
  - Transmission order
    - G7 G6 G5 G4 G3 G2 G1 G0 R7 R6 R5 R4 R3 R2 R1 R0 B7 B6 B5 B4 B3 B2 B1 B0
    - Follow the order of GRB to sent data and the high bit sent at first
    - Any LOW for +50us resets the LED chip
    - All consecutive bits after the first 24 are forwarded to the next device
    - The internal buffer is written to the PWM controller during the next reset
  - Reset
    - an idle (low) period of more than a few microseconds trigger a reset
    - on reset, the data is latched and used to set led color and brighness
    - after reset each device reads the first 24 bit (GRB 8:8:8) of data into an internal buffer
    - and once the first 24 bits are read, the next ones are re-emitted to the next led

## Current consumption (raw measurement)

| White value | SMPS mA@5V | mA/led | uA/led/unit |
|:-----------:|:----------:|:------:|:-----------:|
|     0x10    |     65     |   2.7  |     169     |
|     0x30    |     175    |   7.3  |     152     |
|     0x50    |     272    |  11.3  |     142     |
|     0x70    |     353    |  14.7  |     131     |
|     0x90    |     421    |  17.5  |     122     |
|     0xB0    |     475    |  19.8  |     112     |
|     0xD0    |     525    |  21.9  |     105     |
|     0xFF    |     585    |  24.4  |      96     |

## Too-blue compensation

I noticed that at full white, the color is actually blue-ish

I divided the BLUE value by about 2 compared to RED/GREEN to get a better balance
