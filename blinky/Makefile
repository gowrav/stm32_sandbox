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
	-D __FPU_USED=1
CFLAGS+=-I./


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
	$(ST_LIB)/src/stm32f4xx_i2c.c

#Major programs
SRC+=./main.c

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

#Flash	st-flash write $(BIN_IMAGE) 0x8000000
flash: $(BIN_IMAGE) |  $(EXECUTABLE)
	openocd -f board/stm32f4discovery.cfg -c "reset_config trst_only combined" -c "program $(EXECUTABLE) verify reset exit"

#======================================================================
.PHONY:all clean flash


