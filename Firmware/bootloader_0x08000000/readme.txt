CodeSourcery/STM32 (Lanchon 1-Feb-2008)
minor modification by Martin Thomas 10-Feb-2008 - see below

This is an STM32 build environment for CodeSourcery arm-2007q3-53-arm-none-eabi.

The GCC version of the STM32F10x Firmware Library 1.0 (10/08/2007) is included
in full and unchanged. Startup code is taken form this library. Linker scripts
are taken from the RAISONANCE RKitARM for RIDE7 1.03.0004. The scripts have notes
permitting use: "Copyright RAISONANCE 2007. You can use, modify and distribute
this file freely, but without any warranty." We should all be thankful to them.
Please consider RAISONANCE's RIDE7 if you need an advanced IDE.

Included is a demo project that flashes a led connected to PC12 (ready to run on
an Olimex STM32-H103 board). To build it change to the project directory and use
the make tool that came with CodeSourcery:

    cs-make           to build the firmware library and the main application
    cs-make -B        to force a complete rebuild
    cs-make clean     to delete all previously built object files
    cs-make flash     to flash an STM32 after building via JTAG using OpenOCD

Configuration is done through these files:

    makefile          build options (optimization, etc)
    stm32.ld          target memory size and type
    stm32f10x_conf.h  firmware library configuration
    stm32f10x_it.c    interrupt handlers
    jtag/flash.cfg    OpenOCD configuration (JTAG dongle, etc)

To flash you need OpenOCD installed. See openocd.berlios.de (or www.yagarto.de
for a win32 port).

Cheers!

-------------------------------------------------------------------------------

Remarks and Modifications by Martin Thomas
www.siwawi.arubi.uni-kl.de/avr_projects/arm_projects

Version history mthomas
10. Feb. 2006 - initial

I do not remember where I found Lanchon's example but by far most of the
work has been done by him, STmicro and Raisonance. I have just changed
main.c and the makefile a little bit to get started with the STM32 family.

Since I currently do not have a working JTAG or SWD connection to the STM32
I used the STM32's preinstalled bootloader and the Flash-Loader software
from STmicro to transfer to user-application to the STM32's flash-memory.

Used software/hardware:
- Host-PC running Win2000
- CodeSourcery arm-2007q3-53-arm-none-eabi. It's still the only "ready made" 
  package with GNU tools that support Cortex-M3 (Feb/2008)
- Manley EK-STM32F evaluation-board with STM32F103VBH6 Rev Z 
  (www.manley.com.cn)
- STM32 Flash Loader Demonstrator V1.0 (avail. from st.com file: um0462.zip)
- PC serial-port connected to the EK, Power-Supply though USB-connector

Modifications:
- changed LED connection from PC12 to PC4 in main.c
- extended makefile to create a binary-file using arm-none-eabi-objcopy

Step-by-Step:
- Install CodeSourcery's arm-eabi package.
- Install the STM32 Flash Loader Demonstrator
- type cs-make in the projects's root directory (see above for further
  options). This creates the files main.elf and main.bin:
   arm-none-eabi-gcc -Wl,--gc-sections,-Map=main.elf.map,-cref,-u,Reset_Handler 
    -I . -I lib/inc -L lib -T stm32.ld main.o stm32f10x_it.o lib/libstm32.a 
    --output main.elf
   arm-none-eabi-objcopy -O binary main.elf main.bin
- connect a PC serial-port (or USB-UART bridge) to the connector
  labeled UART-0 of the board (the label is misleading the EK "UART-0"
  is connected to the STM32's UART1 which is the one used by the
  bootloader).
- Select boot-mode "System Memory". This is pins Boot0:1 and Boot1:0 (see
  STmicro application-note AN2606 for further information)
- Power-up the board or reset the STM32
- Start the the flash loader software, select the COM-Port keep all
  other settings. I changed the parity to even as mentioned in AN2606 
  but parity none worked too (none is also the setting shown in STmicro's 
  document UM0462).
- Select [Next] and the software will establish the communication with 
  to the STM32. If not: double check the connection, verify the BOOT
  settings, Reset the STM32, hit [Back] followed by [Next].
- Select [Next], select (*) Download, hit [...], select the file main.bin,
  keep "@" as 08000000. "Verify" and "Jump to user program" are optional but
  enabling both is a good idea for the first tests.
- hit [Next] and the machine-code from main.bin will be transfered to the 
  STM32's flash. If "Jump to user program" has been enabled the LED should
  start blinking now. OR:
- Set Boot0:0 Boot1:0 (boot-mode "User Flash Memory") and reset/power-toogle 
  the STM32 to start the new firmware.

TODOs:
- add at least one volatile in delay for -Os
- test with external JTAG-interfaces and OpenOCD too
- Use a makefile based on the "WinARM template"


Remarks on the EK-STM32F evaluation-board from Manley:

I have gotten the board from a friendly person in China (not Manley) 
Thanks again for sending it.

It's an interesting board with a small LCD-"glas", buttons, LEDs, SD-card
connector, RTC-battery, potentiometer to test the ADC and a small "joystick".
The only documenation I found so far is written in Chinise. So the following
is based on information from the person who sent the board, the few words in 
the documentation written in English and the board-schematic.

The board features an integrated JTAG-interface based on a STR751FR2. The JTAG-
hardware/firmware seems to be IAR C-SPY-compatible but it did not work for
me using IAR EWARM 5.11 with the drivers from the Manley web-site (2/2008). 
EWARM 4.11 should work but I did not test this here.

The STM32 can be disconnected from the on-board JTAG-hardware by removing some
jumpers. Although there is a standard 20-pin ARM-JTAG-connector on the board 
it seems that it can not be used to connect the STM32 with an external JTAG-
interface (i.e. Wiggler or FT2232-based) since there is no option to disconnect
the 20-pin connector from the on-board JTAG-hardware (level-shifters connected
to STR751). Anyway: I will try to solder a small interface to connect a Amontec 
JTAGkey or Wiggler later and to test with OpenOCD. So far I have used the 
preinstalled bootloader for the STM32 to upload the firmware to controller and
this worked as expected.

