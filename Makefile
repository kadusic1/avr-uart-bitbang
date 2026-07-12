MCU      = atmega328p
PORT     = /dev/ttyACM0
BAUD     = 115200
AVRA_INC = /usr/share/avra

SRC = src/main.S
HEX = build/main.hex
LST = build/main.lst

.PHONY: all build flash clean monitor

all: build

build: $(HEX)

$(HEX): $(SRC) src/pins.inc src/delay.inc src/uart.inc
	mkdir -p build
	avra -I $(AVRA_INC) -I src -fI $(SRC) -o $(HEX) -l $(LST)

flash: build
	avrdude -c arduino -p $(MCU) -P $(PORT) -b $(BAUD) -U flash:w:$(HEX):i

monitor:
	tio -b 9600 $(PORT)

clean:
	rm -rf build
	rm -f src/*.obj src/*.eep.hex src/*.cof
