# Arduino code to control the rover

Only some pins support analog write, which can be seen on the card and in the [documentation](https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite/).

Make sure the ENA and ENB pins are on analog pins.

## Develop on the pi

### Setup

`arduino-cli` can be set up on the raspberry pi in order to make the development cycle quicker.

Arduino CLI doesn't come pre-installed with the AVR boards, in order to install them run:

`arduino-cli core install arduino:avr`

### R&D

In order to compile the sketch run:

`arduino-cli compile -b arduino:avr:uno rover`

and in order to upload to the arduino run:

`arduino-cli upload -p /dev/ttyUSB0 -b arduino:avr:uno rover`