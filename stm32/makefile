include makefiles/drivers.mk
include makefiles/core.mk

OBJ_PROJ = $(PROJ)/Obj
SRC_PROJ = $(PROJ)/src
INC_PROJ = $(PROJ)/inc
TARGET = $(OBJ_PROJ)/program
PROJ_SRC_FILES = $(wildcard $(SRC_PROJ)/*.c)
PROJ_OBJ_FILES = $(patsubst $(SRC_PROJ)/%.c,$(OBJ_PROJ)/%.o,$(PROJ_SRC_FILES))
PROJ_LIB = $(TARGET).a

C_INCLUDES = $(foreach d, $(INC) $(INC_PROJ), -I$d)
CFLAGS = $(MCU_FLAGS) $(C_DEFS) $(C_INCLUDES) $(OPT) $(F_FLAGS) $(EXT_FLAGS)

STARTUP = $(SRC_DEV)/gcc/startup_$(MCU_CHIP_L)xx.s
LD_SCRIPT = drivers/$(MCU_CHIP)VGTx_FLASH.ld
EXTRA = -Wl,--gc-sections -Wl,-Map=$(TARGET).map
LDFLAGS = $(MCU_FLAGS) $(LDSPECS) $(EXTRA) -T$(LD_SCRIPT) $(STARTUP)

.PHONY: all, run, debug, proj
proj: $(PROJ_LIB)
all: $(TARGET).bin program.elf
serial: 
	cu -l /dev/ttyACM* -s 115200
run: $(TARGET).bin program.elf
	st-flash write $< 0x8000000
	st-flash reset
	cu -l /dev/ttyACM* -s 115200

program.elf: $(TARGET).elf
	cp $(TARGET).elf program.elf

debug: $(TARGET).elf 
	export LANG=en_AU.UTF-8;
	tmux \
		new-session "st-util & arm-none-eabi-gdb -ex 'target extended-remote:4242' -ex 'load' $^" \; \
		split-window -h "cu -l /dev/ttyACM* -s 115200"

$(TARGET).bin: $(TARGET).elf
	@echo "[OBJCOPY] $(TARGET).bin"
	$(CP) -O binary $< $@

$(TARGET).elf: $(PROJ_LIB) $(EX_LIBS) $(LD_SCRIPT) $(STARTUP)
	@echo "[LD]      $(TARGET).elf"
	$(CC) $(CFLAGS) $(LDFLAGS) $(PROJ_LIB) $(EX_LIBS) -o $@
	@echo "[OBJDUMP] $(TARGET).lst"
	$(OBJDUMP) -St $(TARGET).elf >$(TARGET).lst
	@echo "[SIZE]    $(TARGET).elf"
	$(SZ) $(TARGET).elf
	

$(PROJ_LIB): $(PROJ_OBJ_FILES)
$(PROJ_LIB) $(ALL_LIB): | $(DIR_LIB)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU GCC Archiver'
	rm -f $@
	$(AR) -r $@ $^
	@echo 'Finished building target: $@'
	@echo ' '

$(OBJ_PROJ) $(ALL_OBJ) $(DIR_LIB):;mkdir -p $@
$(OBJ_PROJ)/%.o: $(SRC_PROJ)/%.c | $(OBJ_PROJ); $(CC) -c $(CFLAGS) -o $@ $<

clean:
	find . -name Obj | xargs rm -rf
	rm -rf $(DIR_LIB)

clean-proj:; rm -rf $(OBJ_PROJ)
