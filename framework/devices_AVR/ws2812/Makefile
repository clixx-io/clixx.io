# Makefile found at http://www.instructables.com/id/Honey-I-Shrunk-the-Arduino-Moving-from-Arduino-t/step2/Our-first-AVR-C-project-Hello-world-LED/
#
# The following modifications have been done:
#
# target fuse_flash will both flash chip and set fuses instead of sequential invocation of avrdude which causes problems on my system because the dragon
# reenumerates on the USB bus, second invocation takes place too fast. Using only one invocation of avrdude fixes this.
#
# Added .PHONY for targets that doesn't produce files
# Mikael Lövqvist 2014-09-19

#
# Makefile template for ATtiny85
# Derived from AVR Crosspack template
#

DEVICE     = attiny85           # See avr-help for all possible devices
CLOCK      = 8000000            # 8Mhz
PROGRAMMER = -c usbasp -P usb
OBJECTS    = main.o ws2812_driver.o
HEADERS    = ws2812_config.h ws2812_driver.h
FUSES      = -U lfuse:w:0xE2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m  # settings as taken from http://www.engbedded.com/fusecalc/

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)


# symbolic targets:
all:	main.hex

$(OBJECTS): $(HEADERS)

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@


flash:	all
	$(AVRDUDE) -U flash:w:main.hex:i

fuse_flash: all
	$(AVRDUDE) -U flash:w:main.hex:i $(FUSES)

install: fuse_flash

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID main.hex

clean:
	rm -f main.hex main.elf $(OBJECTS)

# file targets:
main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size --format=avr --mcu=$(DEVICE) main.elf
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	main.elf
	avr-objdump -d main.elf

cpp:
	$(COMPILE) -E main.c

.PHONY: flash fuse_flash install