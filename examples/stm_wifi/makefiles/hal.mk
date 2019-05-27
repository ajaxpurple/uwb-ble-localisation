DIR_HAL = drivers/$(MCU_FAMILY)_HAL_Driver
SRC_HAL = $(DIR_HAL)/Src
INC_HAL = $(DIR_HAL)/Inc
OBJ_HAL = $(DIR_HAL)/Obj
HAL_OBJ_FILES := $(call o_files,$(SRC_HAL),$(OBJ_HAL),*)
HAL_LIB = $(DIR_LIB)/lib$(MCU_FAMILY)_HAL.a
hal: $(HAL_LIB)
$(HAL_LIB): $(HAL_OBJ_FILES) | $(DIR_LIB)
$(eval $(call compile,$(SRC_HAL),$(OBJ_HAL)))
clean-hal:; rm -rf $(OBJ_HAL)
