PROJECT = main
BUILD_DIR = bin
COMMON = ../../common
CMSIS_BASE = /home/leo/dev/cmsis/Drivers/CMSIS

INCLUDES = -I$(COMMON) -I$(COMMON)/drivers
INCLUDES += -I$(CMSIS_BASE)/Include
INCLUDES += -I$(CMSIS_BASE)/Device/ST/STM32F1xx/Include

C_SOURCES = main.c $(COMMON)/startup.s $(COMMON)/system_stm32f1xx.c
C_SOURCES += $(wildcard $(COMMON)/drivers/*.c)

CC = arm-none-eabi-gcc
CFLAGS = -mcpu=cortex-m3 -mthumb -Wall -O0 -g $(INCLUDES)

LDSCRIPT = $(COMMON)/linker.ld

all: $(BUILD_DIR)/$(PROJECT).elf

$(BUILD_DIR)/$(PROJECT).elf: $(C_SOURCES)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -T$(LDSCRIPT) $^ -o $@

clean:
	rm -rf $(BUILD_DIR)
