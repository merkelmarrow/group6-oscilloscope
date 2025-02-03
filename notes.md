# Project Notes

- Minimum viable solution (MVS)

For an MVS, the oscilloscope needs to be able to draw a very slow waveform (ie. 1 Hz) on the OLED display at a known offset voltage of 2.5 V and a max pk-pk voltage of 4 V.

-  Goal solution

A good goal to try to reach for is an oscilloscope that can condition a wide range of signals with different DC offsets and voltages to map them to within the Arduino ADC's 0-5 V input range and automatically scale the captured waveform with a vertical and horizontal scaling factor so that it fits onto the display.

## Minimum solution implementation notes

- Since the input signal is already between 0 and 5V, it can be fed directly to the Arduino, but it's still a good idea to use a series resistor to limit current and protect the ADC.

A series resistor can introduce a voltage divider effect, but the input impedance of the Arduino's ADC is high enough that it can be ignored. 

A more sophisticated solution can use an op-amp buffer circuit which provides very low output impedance while protecting the circuit (not necessary for MVS).

- No need for a low pass filter at ~1 Hz. Since the signal is very slow, we don't need high-speed sampling. Simple polling (ie. analogRead()) should work fine.

- MDOB128064BV-WS (calling it disp from now) module takes 5V power and uses SPI. 

Here are what the different pins do:

1. VCC: power supply (5V).
2. GND: ground.
3. MOSI (aka D1): data from microcontroller to disp.
4. SCK (aka CLK, D0): serial clock, signal for synchronising data.
5. CS (aka SS): selects the active SPI device (not relevant to us because we're only using one SPI "slave" device, ie the disp).
6. DC: used to differentiate between command and data bytes.
7. RST: resets the display (active LOW).

On the Arduino, MOSI is pin 11 and SCK is 13, the rest are configurable.

- Disp is 128 pixels wide by 64 pixels tall.

- For a 1 Hz waveform, one full cycle takes 1000 ms. Say we want to display two cycles on the screen. If we have 128 pixels across, and two cycles take two seconds (because signal is 1 Hz), we should sample every ~16 ms (2000 ms / 128). So we can use delay(16) between samples.

- With the Arduino ADC, readings range from 0 to 1023 and are mapped to 0 V and 5 V respectively.

- For testing at home without an oscilloscope, we have to generate the waves on the Arduino, but the Arduino Uno R3 doesn't have a DAC. We could try to hack together a PWM + low pass filter to try create a sine wave but we can just generate a square wave from the digital pins instead (much easier). If we want to read our generated signal on the Arduino at the same time, it's safe to connect the digital pin directly to the analog pin because of the high input impedance (100 M$\Omega$'s ensures no overcurrent).