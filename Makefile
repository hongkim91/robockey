# --------------------------------------------------------
# Custom M2 Makefile
# written by: Jonathan Bohren & Jonathan Fiene
# updated: Feb 2, 2013
# --------------------------------------------------------

# --------------------------------------------------------
# if you write separate C files to include in main,
# add their .o targets to the CHILDREN line below
# (e.g. "CHILDREN = myfile.o")
#
# to include code supplied by maevarm, add a .o target
# tag to the parents line (e.g. "PARENTS = "m_bus.o")
# --------------------------------------------------------
MAIN       = main.o
CHILDREN   = timer.o localization.o comm.o debug.o camera.o motor.o adc.o control.o puck.o constants.o goalie.o
PARENTS    = m_bus.o m_usb.o m_rf.o m_wii.o

# --------------------------------------------------------
# if you want to use one of our pre-compiled libraries,
# add it to the line below (e.g. "LIBRARIES = libsaast.a")
# --------------------------------------------------------

# --------------------------------------------------------
# you shouldn't change anything below here,
# unless you really know what you're doing
# --------------------------------------------------------
DEVICE     = atmega32u4
CLOCK      = 16000000

COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) \
          -Wl,-u,vfprintf -lprintf_flt -lm

# symbolic targets:
all:	main.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

install: flash

flash: all
	dfu-programmer atmega32u4 erase
	dfu-programmer atmega32u4 flash main.hex

clean:
	rm -f main.hex main.elf $(MAIN) $(CHILDREN)

# file targets:
main.elf: $(MAIN) $(CHILDREN) $(PARENTS)
	$(COMPILE) -o main.elf $(MAIN) $(CHILDREN) $(PARENTS) $(LIBRARIES) -lm

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex

receiver.elf: receiver.o $(CHILDREN) $(PARENTS)
	$(COMPILE) -o receiver.elf receiver.o $(CHILDREN) $(PARENTS) $(LIBRARIES) -lm

receiver.hex: receiver.elf
	rm -f receiver.hex
	avr-objcopy -j .text -j .data -O ihex receiver.elf receiver.hex
recv: receiver.hex
	dfu-programmer atmega32u4 erase
	dfu-programmer atmega32u4 flash receiver.hex

live_plot.elf: live_plot.o $(CHILDREN) $(PARENTS)
	$(COMPILE) -o live_plot.elf live_plot.o $(CHILDREN) $(PARENTS) $(LIBRARIES) -lm

live_plot.hex: live_plot.elf
	rm -f live_plot.hex
	avr-objcopy -j .text -j .data -O ihex live_plot.elf live_plot.hex
live_plot: live_plot.hex
	dfu-programmer atmega32u4 erase
	dfu-programmer atmega32u4 flash live_plot.hex

# Targets for code debugging and analysis:
disasm:	main.elf
	avr-objdump -d main.elf

cpp:
	$(COMPILE) -E main.c
