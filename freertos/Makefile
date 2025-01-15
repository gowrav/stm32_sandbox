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
# CFLAGS+=-D __FPU_PRESENT=1
CFLAGS+=-D ARM_MATH_CM4
# CFLAGS+=-D __FPU_USED=1
CFLAGS+=-D STM32F405xx

#Include all Headers in root
CFLAGS+=-I./Inc/


#stm32-flash
CFLAGS+=-Wl,-T,stm32_flash.ld

#======================================================================#

#Source code
CFLAGS+=-I./lib/STM32Cube_FW_F4_V1.28.0/Drivers/CMSIS/Core/Include
CFLAGS+=-I./lib/STM32Cube_FW_F4_V1.28.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include
SRC=./Src/system_stm32f4xx.c

#StdPeriph
ST_LIB=./lib/STM32Cube_FW_F4_V1.28.0/Drivers/STM32F4xx_HAL_Driver
CFLAGS+=-I$(ST_LIB)/Inc
SRC+=$(ST_LIB)/Src/stm32f4xx_hal.c \
	$(ST_LIB)/Src/stm32f4xx_hal_rcc.c \
	$(ST_LIB)/Src/stm32f4xx_hal_gpio.c \
	$(ST_LIB)/Src/stm32f4xx_hal_cortex.c\

#FreeRTOS
ARCH=CM4F
FREERTOS=./lib/FreeRTOSV10.4.3LTSP3

CFLAGS+=-I$(FREERTOS)/include \
	-I$(FREERTOS)/portable/GCC/ARM_$(ARCH)
SRC+=$(FREERTOS)/croutine.c \
	$(FREERTOS)/list.c \
	$(FREERTOS)/queue.c \
	$(FREERTOS)/tasks.c \
	$(FREERTOS)/timers.c \
	$(FREERTOS)/portable/MemMang/heap_4.c \
	$(FREERTOS)/portable/GCC/ARM_$(ARCH)/port.c

#======================================================================#
#STM32 startup file
STARTUP=startup_stm32f405xx.s
STARTUP_OBJ=$(BUILD_DIR)/startup_stm32f405xx.o

#Major programs
SRC+=./Src/main.c

#======================================================================#
#Pre Requisites
# FREERTOS https://codeload.github.com/FreeRTOS/FreeRTOS-Kernel/zip/refs/tags/V10.4.3-LTS-Patch-3
# ST CMSIS HAL DRIVERS https://codeload.github.com/STMicroelectronics/STM32CubeF4/zip/refs/tags/v1.28.0
# pre_req : 
# 	# wget -O lib/FreeRTOSV10.4.3LTSP3.zip https://codeload.github.com/FreeRTOS/FreeRTOS-Kernel/zip/refs/tags/V10.4.3-LTS-Patch-3
# 	# unzip lib/FreeRTOSV10.4.3LTSP3.zip -d lib/.
# 	# mv lib/FreeRTOS-Kernel-10.4.3-LTS-Patch-3 lib/FreeRTOSV10.4.3LTSP3
# 	# rm lib/FreeRTOSV10.4.3LTSP3.zip
# 	# wget -O lib/STM32Cube_FW_F4_V1.28.0.zip https://codeload.github.com/STMicroelectronics/STM32CubeF4/zip/refs/tags/v1.28.0
# 	# unzip lib/STM32Cube_FW_F4_V1.28.0.zip -d lib/.
# 	# mv lib/STM32CubeF4-1.28.0 lib/STM32Cube_FW_F4_V1.28.0
# 	# rm lib/STM32Cube_FW_F4_V1.28.0.zip

#======================================================================#
#Make rules
clean_lib: 
	rm -r -f -d .git/
	rm -r -f -d ./lib/*.*
setup_lib: clean_lib
	git init
	git submodule init
	git submodule add https://github.com/FreeRTOS/FreeRTOS-Kernel lib/FreeRTOSV10.4.3LTSP3
	git submodule update --init --recursive
	cd lib/FreeRTOSV10.4.3LTSP3 && git checkout -f tags/V10.4.3-LTS-Patch-3
	git submodule add https://github.com/STMicroelectronics/STM32CubeF4 lib/STM32Cube_FW_F4_V1.28.0
	git submodule update --init --recursive
	cd lib/STM32Cube_FW_F4_V1.28.0 && git checkout -f tags/v1.28.0
	cd lib/STM32Cube_FW_F4_V1.28.0/Drivers/STM32F4xx_HAL_Driver && git checkout -f tags/v1.8.2
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
