BOARD = B-L475E-IOT01
MCU_FAMILY = STM32L4xx
MCU_FAMILY_L = `echo $(MCU_FAMILY) | tr A-Z a-z`
MCU_CHIP = STM32L475
MCU_CHIP_L = stm32l475
DIR_LIB = libs

TOOL = arm-none-eabi
CC = $(TOOL)-gcc
AS = $(CC)-x assembler-with-cpp
CP = $(TOOL)-objcopy
OBJDUMP = $(TOOL)-objdump
AR = $(TOOL)-ar
SZ = $(TOOL)-size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

OPT = -Os
MCU_FLAGS = -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -march=armv7e-m
F_FLAGS = -fdata-sections -ffunction-sections -fmessage-length=0 -fno-aggressive-loop-optimizations
C_DEFS = -D$(MCU_CHIP)xx -DUSE_HAL_DRIVER -DUSE_DBPRINTF -DUSE_RTOS_SYSTICK -Dfprintf=debug_fprintf -Dfputc=debug_fputc -DHAVE_CONFIG_H -DNDEBUG
EXT_FLAGS = -Wall -g -std=gnu99 -Wpedantic -Wfatal-errors
LDSPECS = -specs=nano.specs -specs=nosys.specs
o_files = $(patsubst $(1)/%.c, $(2)/%.o, $(wildcard $(1)/$(3).c))
compile = $(2)/%.o: $(1)/%.c | $(2); @$$(CC) -c $$(CFLAGS) -o $$@ $$<; echo $$(notdir $$<) ">" $$(notdir $$@);
C_INCLUDES = $(foreach d, $(INC), -I$d)
CFLAGS = $(MCU_FLAGS) $(C_DEFS) $(C_INCLUDES) $(OPT) $(F_FLAGS) $(EXT_FLAGS)
