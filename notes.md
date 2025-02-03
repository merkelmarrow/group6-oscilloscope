# Project Notes

## Minimum viable solution

For a minimum viable solution, the oscilloscope needs to be able to draw a very slow waveform (ie. 1 Hz) on the OLED display at a known offset voltage of 2.5 V and a max pk-pk voltage of 4 V.

## Goal solution

A good goal to try to reach for is an oscilloscope that can condition a wide range of signals with different DC offsets and voltages to map them to within the Arduino ADC's 0-5 V input range and automatically scale the captured waveform with a vertical and horizontal scaling factor so that it fits onto the display.