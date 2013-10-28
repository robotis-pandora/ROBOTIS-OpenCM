# setup
# modified by Martin Thomas for WinARM 3/2008 testing

COMPILE_OPTS = -mcpu=cortex-m3 -mthumb -Wall -g -Os -ffunction-sections
INCLUDE_DIRS = -I . -I lib/inc
LIBRARY_DIRS = -L lib

TCHAIN_PREFIX=arm-none-eabi-


CC = $(TCHAIN_PREFIX)gcc
CFLAGS = $(COMPILE_OPTS) $(INCLUDE_DIRS)

CXX = $(TCHAIN_PREFIX)g++
CXXFLAGS = $(COMPILE_OPTS) $(INCLUDE_DIRS)

AS = $(TCHAIN_PREFIX)gcc
ASFLAGS = $(COMPILE_OPTS) -c

LD = $(TCHAIN_PREFIX)gcc
LDFLAGS = -Wl,--gc-sections,-Map=$@.map,-cref,-u,Reset_Handler $(INCLUDE_DIRS) $(LIBRARY_DIRS) -T stm32.ld

OBJCP = $(TCHAIN_PREFIX)objcopy
OBJCPFLAGS = -O binary

OBJDUMP = $(TCHAIN_PREFIX)objdump
OBJDUMPFLAGS = -h -S -C -D


AR = $(TCHAIN_PREFIX)ar
ARFLAGS = cr

MAIN_OUT = STM32F103C8_bootloader
MAIN_OUT_ELF = $(MAIN_OUT).elf
MAIN_OUT_BIN = $(MAIN_OUT).bin
MAIN_OUT_LSS = $(MAIN_OUT).lss

# all

all: $(MAIN_OUT_ELF) $(MAIN_OUT_BIN) $(MAIN_OUT_LSS)

# main

$(MAIN_OUT_ELF): \
 main.o \
 stm32f10x_it.o \
 usb_desc.o \
 usb_endp.o \
 usb_istr.o \
 usb_prop.o \
 USBInit.o \
 lib/libstm32.a
	$(LD) $(LDFLAGS) \
 main.o \
 stm32f10x_it.o \
 usb_desc.o \
 usb_endp.o \
 usb_istr.o \
 usb_prop.o \
 USBInit.o \
 lib/libstm32.a --output $@

$(MAIN_OUT_BIN): $(MAIN_OUT_ELF)
	$(OBJCP) $(OBJCPFLAGS) $< $@

$(MAIN_OUT_LSS): $(MAIN_OUT_ELF)
	$(OBJDUMP) $(OBJDUMPFLAGS) $< > $@



# libstm32.a

LIBSTM32_OUT = lib/libstm32.a

LIBSTM32_OBJS = \
 lib/src/stm32f10x_flash.o \
 lib/src/stm32f10x_iwdg.o \
 lib/src/stm32f10x_gpio.o \
 lib/src/stm32f10x_lib.o \
 lib/src/stm32f10x_nvic.o \
 lib/src/stm32f10x_rcc.o \
 lib/src/stm32f10x_tim.o \
 lib/src/stm32f10x_systick.o \
 lib/src/stm32f10x_usart.o \
 lib/src/cortexm3_macro.o \
 lib/src/stm32f10x_vector.o \
 lib/USB_lib/src/usb_init.o \
 lib/USB_lib/src/usb_core.o \
 lib/USB_lib/src/usb_mem.o \
 lib/USB_lib/src/usb_regs.o \
 lib/USB_lib/src/usb_sil.o \
 lib/USB_lib/src/usb_int.o 


$(LIBSTM32_OUT): $(LIBSTM32_OBJS)
	$(AR) $(ARFLAGS) $@ $(LIBSTM32_OBJS)

$(LIBSTM32_OBJS): stm32f10x_conf.h


clean:
	-cs-rm *.o lib/src/*.o USBCDC/*.o lib/USB_lib/src/*.o  $(LIBSTM32_OUT) $(MAIN_OUT_ELF) $(MAIN_OUT_BIN) $(MAIN_OUT_LSS) $(MAIN_OUT_ELF).map
