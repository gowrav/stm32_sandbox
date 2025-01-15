#======================================================================#
#Output files
BUILD_DIR=build
EXECUTABLE=$(BUILD_DIR)/firmware.elf
BIN_IMAGE=$(BUILD_DIR)/firmware.bin
#Output_Directory
#======================================================================#
#Toolchain
TC_ROOT=/Users/gowrav/Work/gcc-arm-none-eabi-10-2020-q4-major
CC=$(TC_ROOT)/bin/arm-none-eabi-gcc
OBJCOPY=$(TC_ROOT)/bin/arm-none-eabi-objcopy
GDB=$(TC_ROOT)/bin/arm-none-eabi-gdb

#======================================================================#

#Flags

CFLAGS=-g -mlittle-endian -mthumb \
	-mcpu=cortex-m4 \
	-mfpu=fpv4-sp-d16 -mfloat-abi=hard \
	--specs=nano.specs \
	--specs=nosys.specs
CFLAGS+=-D USE_STDPERIPH_DRIVER
CFLAGS+=-D STM32F4xx
#CFLAGS+=-D __FPU_PRESENT=1
# CFLAGS+=-D ARM_MATH_CM4
#CFLAGS+=-D __FPU_USED=1

#Include all Headers in root
CFLAGS+=-I./Inc/


#stm32-flash
CFLAGS+=-Wl,-T,stm32_flash.ld

#======================================================================#

#Source code
CFLAGS+=-I./lib/STM32Cube_FW_F4_V1.28.0/Drivers/CMSIS/Core/Include
CFLAGS+=-I./lib/STM32Cube_FW_F4_V1.28.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include
SRC=./Src/system_stm32f4xx.c
SRC+=./Src/stm32f4xx_it.c

#StdPeriph
ST_LIB=./lib/STM32Cube_FW_F4_V1.28.0/Drivers/STM32F4xx_HAL_Driver
CFLAGS+=-I$(ST_LIB)/Inc
SRC+=$(ST_LIB)/Src/stm32f4xx_hal.c \
	$(ST_LIB)/Src/stm32f4xx_hal_rcc.c \
	$(ST_LIB)/Src/stm32f4xx_hal_gpio.c \
	$(ST_LIB)/Src/stm32f4xx_hal_cortex.c\

#======================================================================#
#STM32 startup file
STARTUP=startup_stm32f405xx.s
STARTUP_OBJ=$(BUILD_DIR)/startup_stm32f405xx.o

#Major programs
SRC+=./Src/main.c

#======================================================================#

#Make rules

#Compile
all:$(BIN_IMAGE)

# Ensure the build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(STARTUP_OBJ): $(STARTUP) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(EXECUTABLE):$(SRC) $(STARTUP_OBJ) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_IMAGE):$(EXECUTABLE) | $(BUILD_DIR)
	$(OBJCOPY) -O binary $^ $@

#Make clean
clean:
	rm -rf $(BUILD_DIR)

#Flash	st-flash write $(BIN_IMAGE) 0x8000000
flash: $(BIN_IMAGE) |  $(EXECUTABLE)
	openocd -f board/stm32f4discovery.cfg -c "reset_config trst_only combined" -c "program $(EXECUTABLE) verify reset exit"

#OpenOCD GDB server
openocd:
	openocd -f ./openocd.cfg

#GNU Debugger
gdbauto:
	cgdb -d $(GDB) -x ./openocd_gdb.gdb

#======================================================================
.PHONY:all clean flash
