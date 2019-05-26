include makefiles/core.mk
DIR_FATFS = drivers/FatFs
SRC_FATFS = $(DIR_FATFS)/src
SRC_FATFS_OPTION = $(SRC_FATFS)/option

INC_FATFS = $(DIR_FATFS)/src
OBJ_FATFS = $(DIR_FATFS)/Obj

FATFS_OBJ_FILES := $(call o_files,$(SRC_FATFS),$(OBJ_FATFS),*)
FATFS_OBJ_FILES += $(call o_files,$(SRC_FATFS_OPTION),$(OBJ_FATFS),*)

FATFS_LIB = $(DIR_LIB)/libFatFs.a

.PHONY: fatfs
fatfs: $(FATFS_LIB)
$(FATFS_LIB): $(FATFS_OBJ_FILES) | $(DIR_LIB)
$(eval $(call compile,$(SRC_FATFS),$(OBJ_FATFS)))
$(eval $(call compile,$(SRC_FATFS_OPTION),$(OBJ_FATFS)))
clean-fatfs:; rm -rf $(OBJ_FATFS)