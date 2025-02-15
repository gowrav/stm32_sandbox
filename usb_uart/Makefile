#======================================================================#
#Output files
BUILD_DIR=build
EXECUTABLE=$(BUILD_DIR)/stm32_executable.elf
BIN_IMAGE=$(BUILD_DIR)/stm32_bin_image.bin
#Output_Directory
#======================================================================#
#Cross Compiler
CC=/Users/gowrav/Work/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc
OBJCOPY=/Users/gowrav/Work/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-objcopy

#======================================================================#
#Flags
CFLAGS=-g -mlittle-endian -mthumb \
	-mcpu=cortex-m4 \
	-mfpu=fpv4-sp-d16 -mfloat-abi=hard \
	--specs=nano.specs \
	--specs=nosys.specs
CFLAGS+=-D USE_STDPERIPH_DRIVER
CFLAGS+=-D STM32F4xx
CFLAGS+=-D __FPU_PRESENT=1 \
        -D ARM_MATH_CM4 \
	-D __FPU_USED=1 \
	-D USE_USB_OTG_FS=1


#stm32-flash
CFLAGS+=-Wl,-T,stm32_flash.ld

#======================================================================#
#lib

#Stm32 libraries
ST_LIB=./lib/STM32F4xx_StdPeriph_Driver
CFLAGS+=-I./lib/CMSIS/ST/STM32F4xx/Include

#CMSIS libraries
CFLAGS+=-I./lib/CMSIS/Include

#StdPeriph includes
CFLAGS+=-I$(ST_LIB)/inc

#stm32 usb device library
CFLAGS+=-I./lib/STM32_USB_Device_Library/Class/cdc/inc
CFLAGS+=-I./lib/STM32_USB_Device_Library/Core/inc

#stm32 usb otg library
CFLAGS+=-I./lib/STM32_USB_OTG_Driver/inc

# #stm32f4-discovery utilities
CFLAGS+=-I./lib/STM32F4-Discovery

CFLAGS+=-I./
CFLAGS+=-I./usb/

#======================================================================#
#Source code
SRC=./lib/CMSIS/system_stm32f4xx.c

#StdPeriph
SRC+=$(ST_LIB)/src/misc.c \
	$(ST_LIB)/src/stm32f4xx_rcc.c \
	$(ST_LIB)/src/stm32f4xx_dma.c \
	$(ST_LIB)/src/stm32f4xx_flash.c \
	$(ST_LIB)/src/stm32f4xx_gpio.c \
	$(ST_LIB)/src/stm32f4xx_usart.c \
	$(ST_LIB)/src/stm32f4xx_tim.c\
	$(ST_LIB)/src/stm32f4xx_spi.c\
	$(ST_LIB)/src/stm32f4xx_i2c.c \
	$(ST_LIB)/src/stm32f4xx_exti.c \
	$(ST_LIB)/src/stm32f4xx_syscfg.c

SRC+=./lib/STM32F4-Discovery/stm32f4_discovery.c \
	./lib/STM32_USB_OTG_Driver/src/usb_dcd_int.c \
	./lib/STM32_USB_OTG_Driver/src/usb_dcd.c \
	./lib/STM32_USB_Device_Library/Core/src/usbd_core.c \
	./lib/STM32_USB_Device_Library/Core/src/usbd_req.c \
	./lib/STM32_USB_Device_Library/Core/src/usbd_ioreq.c \
	./lib/STM32_USB_Device_Library/Class/cdc/src/usbd_cdc_core.c

#Major programs
SRC+=./main.c \
	./interrupt.c \
	./usb/usb_bsp.c \
	./usb/usbd_desc.c \
	./usb/usbd_usr.c \
	./usb/usb_core.c \
	./usb/usbd_cdc_vcp.c \


#======================================================================#
#STM32 startup file
STARTUP=startup_stm32f4xx.s
STARTUP_OBJ=$(BUILD_DIR)/startup_stm32f4xx.o
#======================================================================#
#Make rules

#Make all
all: $(BIN_IMAGE)

# Ensure the build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile startup file
$(STARTUP_OBJ): $(STARTUP) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Build executable
$(EXECUTABLE): $(SRC) $(STARTUP_OBJ) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# Create binary from executable
$(BIN_IMAGE): $(EXECUTABLE) | $(BUILD_DIR)
	$(OBJCOPY) -O binary $^ $@



#Make clean
clean:
	rm -rf $(BUILD_DIR)
#Make flash
flash: $(BIN_IMAGE)
	st-flash write $(BIN_IMAGE) 0x8000000

#======================================================================
.PHONY:all clean flash


