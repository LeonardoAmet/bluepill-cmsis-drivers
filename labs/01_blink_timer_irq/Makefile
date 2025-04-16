# Proyecto bare-metal STM32F103C8T6 con CMSIS externo
PROJECT = main
TARGET = $(PROJECT).elf
BUILD_DIR = bin

# Rutas base
COMMON        = ../../common
CMSIS_DEVICE  = /home/lamet/dev/cmsis-device-f1
CMSIS_CORE    = /home/lamet/dev/CMSIS_5/CMSIS/Core/Include

# Includes
INCLUDES = -I$(COMMON)
INCLUDES += -I$(COMMON)/drivers
INCLUDES += -I$(CMSIS_DEVICE)/Include
INCLUDES += -I$(CMSIS_CORE)

# Definición del micro
CDEFS = -DSTM32F103xB

# Fuentes
C_SOURCES = main.c \
            $(COMMON)/system_stm32f1xx.c \
            $(wildcard $(COMMON)/drivers/*.c)

ASM_SOURCES = $(COMMON)/startup.s

# Compilador y flags
CC      = arm-none-eabi-gcc
CFLAGS  = -mcpu=cortex-m3 -mthumb -Wall -O0 -g $(INCLUDES) $(CDEFS) -nostdlib
LDSCRIPT = $(COMMON)/linker.ld
LDLIBS  = -lc -lgcc -lnosys

# Compilación principal
all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(C_SOURCES) $(ASM_SOURCES)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -T$(LDSCRIPT) $^ -o $@ $(LDLIBS)

# Limpieza
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/*

# Flasheo con OpenOCD
.PHONY: flash
flash: $(BUILD_DIR)/$(TARGET)
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c "program $^ verify reset exit"

# Abrir OpenOCD
.PHONY: openocd
openocd:
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg

# Conexión con GDB
.PHONY: gdb
gdb: $(BUILD_DIR)/$(TARGET)
	gdb-multiarch $^ -ex "target remote localhost:3333"

# Tamaño de la imagen
.PHONY: size
size: $(BUILD_DIR)/$(TARGET)
	arm-none-eabi-size $^

# Ayuda
.PHONY: help
help:
	@echo "Comandos disponibles:"
	@echo "  make           → Compila y genera .elf"
	@echo "  make flash     → Programa el microcontrolador"
	@echo "  make gdb       → Lanza GDB"
	@echo "  make openocd   → Inicia OpenOCD"
	@echo "  make clean     → Borra archivos generados"
	@echo "  make size      → Muestra uso de memoria"
