include makefiles/core.mk
DIR_ARGTABLE = drivers/argtable2/src
SRC_ARGTABLE = $(DIR_ARGTABLE)
INC_ARGTABLE = $(DIR_ARGTABLE)
OBJ_ARGTABLE = $(DIR_ARGTABLE)/Obj
ARGTABLE_OBJ_FILES := $(call o_files,$(SRC_ARGTABLE),$(OBJ_ARGTABLE),*)
ARGTABLE_LIB = $(DIR_LIB)/libARGTABLE.a

.PHONY: argtable
argtable: $(ARGTABLE_LIB)
$(ARGTABLE_LIB): $(ARGTABLE_OBJ_FILES) | $(DIR_LIB)
$(eval $(call compile,$(SRC_ARGTABLE),$(OBJ_ARGTABLE)))
clean-argtable:; rm -rf $(OBJ_ARGTABLE)