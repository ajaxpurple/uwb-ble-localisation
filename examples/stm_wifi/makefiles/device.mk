DIR_CMSIS = drivers/CMSIS
DIR_DEV = $(DIR_CMSIS)/Device/ST/$(MCU_FAMILY)
SRC_DEV = $(DIR_DEV)/Source/Templates
INC_CMSIS = $(DIR_CMSIS)/Include
INC_DEV = $(DIR_DEV)/Include
OBJ_DEV = $(DIR_DEV)/Obj
DEV_OBJ_FILE = $(OBJ_DEV)/system_$(MCU_FAMILY_L).o
DEV_OBJ_FILES := $(call o_files,$(SRC_DEV),$(OBJ_DEV),*)
dev: $(DEV_OBJ_FILES)
$(eval $(call compile,$(SRC_DEV),$(OBJ_DEV)))
clean-dev:; rm -rf $(OBJ_DEV)