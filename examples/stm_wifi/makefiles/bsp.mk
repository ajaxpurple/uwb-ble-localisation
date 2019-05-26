include makefiles/core.mk
DIR_BSP = drivers/BSP
SRC_BSP = $(DIR_BSP)/$(BOARD)
SRC_BSP_ACCEL = $(DIR_BSP)/Components/lsm6dsl
SRC_BSP_MAGNETO = $(DIR_BSP)/Components/lis3mdl

INC_BSP = $(DIR_BSP)/$(BOARD) $(DIR_BSP)/Components/lsm6dsl $(DIR_BSP)/Components/lis3mdl

OBJ_BSP = $(DIR_BSP)/Obj

BSP_OBJ_FILES := $(call o_files,$(SRC_BSP),$(OBJ_BSP),*)
BSP_OBJ_FILES += $(OBJ_BSP)/lsm6dsl.o
BSP_OBJ_FILES += $(OBJ_BSP)/lis3mdl.o

BSP_LIB = $(DIR_LIB)/lib$(MCU_FAMILY)_BSP.a
bsp: $(BSP_LIB)
$(BSP_LIB): $(BSP_OBJ_FILES) | $(DIR_LIB)
$(eval $(call compile,$(SRC_BSP),$(OBJ_BSP)))
$(eval $(call compile,$(SRC_BSP_ACCEL),$(OBJ_BSP)))
$(eval $(call compile,$(SRC_BSP_MAGNETO),$(OBJ_BSP)))


clean-bsp:; rm -rf $(OBJ_BSP)