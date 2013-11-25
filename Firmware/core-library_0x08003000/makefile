COMPILE_OPTS = -Os -g -mcpu=cortex-m3 -mthumb -march=armv7-m -nostdlib -ffunction-sections -fdata-sections -Wl,--gc-sections -DBOARD_CM904 -DMCU_STM32F103CB -DSTM32_MEDIUM_DENSITY -DVECT_TAB_FLASH -DCM9_DEBUG
COMPILEAS_OPTS = -mcpu=cortex-m3 -march=armv7-m -mthumb -DBOARD_CM904 -DMCU_STM32F103CB -DSTM32_MEDIUM_DENSITY -DVECT_TAB_FLASH -DCM9_DEBUG
COMPILECPP_OPTS = -Os -g -mcpu=cortex-m3 -mthumb -march=armv7-m -nostdlib -ffunction-sections -fdata-sections -Wl,--gc-sections -fno-rtti -fno-exceptions -Wall -DBOARD_CM904 -DMCU_STM32F103CB -DSTM32_MEDIUM_DENSITY -DVECT_TAB_FLASH -DCM9_DEBUG 
INCLUDE_DIRS = -I . -I lib/inc
LIBRARY_DIRS = -L

TCHAIN_PREFIX=arm-none-eabi-

#c compiler setting
CC = $(TCHAIN_PREFIX)gcc
CFLAGS = $(COMPILE_OPTS) $(INCLUDE_DIRS)
#c++ compiler setting
CXX = $(TCHAIN_PREFIX)g++
CXXFLAGS = $(COMPILECPP_OPTS) $(INCLUDE_DIRS)
#Assembler compiler setting
AS = $(TCHAIN_PREFIX)gcc
ASFLAGS = $(COMPILEAS_OPTS)

#LD = $(TCHAIN_PREFIX)gcc

LD = $(TCHAIN_PREFIX)g++
LDFLAGS = -T flash.ld -L . -mcpu=cortex-m3 -mthumb -Xlinker --gc-sections -Wall

OBJCP = $(TCHAIN_PREFIX)objcopy
OBJCPFLAGS = -v -Obinary

OBJDUMP = $(TCHAIN_PREFIX)objdump
OBJDUMPFLAGS = -h -S -C -D
#Archive maintaining program -> ar archiver
AR = $(TCHAIN_PREFIX)ar
ARFLAGS = cr

MAIN_OUT = cm9-core-library
MAIN_OUT_ELF = $(MAIN_OUT).elf
MAIN_OUT_BIN = $(MAIN_OUT).bin
MAIN_OUT_LSS = $(MAIN_OUT).lss

# all
#여기는 최종 target 파일이 3개가 있다. 각각 3개의 target이 자동으로 생성된다. 
all: $(MAIN_OUT_ELF) $(MAIN_OUT_BIN) 
#$(MAIN_OUT_LSS)

# main
MAIN_OBJS = exc.o\
 lib/src/descriptors.o \
 lib/src/adc.o \
 lib/src/exti.o \
 lib/src/flash.o \
 lib/src/gpio.o \
 lib/src/i2c.o \
 lib/src/iwdg.o \
 lib/src/nvic.o \
 lib/src/pwr.o \
 lib/src/rcc.o \
 lib/src/spi.o \
 lib/src/syscalls.o \
 lib/src/systick.o \
 lib/src/timer.o \
 lib/src/usart.o \
 lib/src/usb.o \
 lib/src/usb_callbacks.o \
 lib/src/usb_core.o \
 lib/src/usb_hardware.o \
 lib/src/usb_init.o \
 lib/src/usb_int.o \
 lib/src/usb_mem.o \
 lib/src/usb_regs.o \
 lib/src/boards.o \
 lib/src/cxxabi-compat.o \
 lib/src/ext_interrupts.o \
 lib/src/HardwareSerial.o \
 lib/src/HardwareSPI.o \
 lib/src/HardwareTimer.o \
 lib/src/Print.o \
 lib/src/pwm.o \
 lib/src/usb_serial.o \
 template.o \
 main.o \
 OLLO.o	\
 bkp_core.o	\
 BKP.o	\
 eeprom_core.o	\
 EEPROM.o	\
 RC100.o	\
 rc100_core.o	\
 dxl_pro.o	\
 DynamixelPro.o	\
 lib/src/CM900.o \
 lib/src/CM904.o \
 lib/src/Dynamixel.o \
 lib/src/dxl.o \
 lib/src/Arduino-compatibles.o
 
 
 
 
$(MAIN_OUT_ELF): $(MAIN_OBJS) 
	$(LD) $(LDFLAGS) $(MAIN_OBJS) --output $@

$(MAIN_OUT_BIN): $(MAIN_OUT_ELF)
	$(OBJCP) $(OBJCPFLAGS) $< $@

#$(MAIN_OUT_LSS): $(MAIN_OUT_ELF)
#	$(OBJDUMP) $(OBJDUMPFLAGS) $< > $@

clean :
	-cs-rm lib/src/*.o *.o $(MAIN_OUT_ELF) $(MAIN_OUT_BIN)
