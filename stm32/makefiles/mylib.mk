DIR_MYLIB = mylib

DIR_MYLIB_OS = $(DIR_MYLIB)/os
SRC_MYLIB_OS = $(DIR_MYLIB_OS)/src
INC_MYLIB_OS = $(DIR_MYLIB_OS)/inc

DIR_MYLIB_CLI = $(DIR_MYLIB)/cli
SRC_MYLIB_CLI = $(DIR_MYLIB_CLI)/src
INC_MYLIB_CLI = $(DIR_MYLIB_CLI)/inc

DIR_MYLIB_HAL = $(DIR_MYLIB)/hal
SRC_MYLIB_HAL = $(DIR_MYLIB_HAL)/src
INC_MYLIB_HAL = $(DIR_MYLIB_HAL)/inc

OBJ_MYLIB = $(DIR_MYLIB)/Obj

INC_MYLIB = $(INC_MYLIB_HAL) $(INC_MYLIB_CLI) $(INC_MYLIB_OS) 

MYLIB_OBJ_FILES := $(call o_files,$(SRC_MYLIB_HAL),$(OBJ_MYLIB),*)
MYLIB_OBJ_FILES += $(call o_files,$(SRC_MYLIB_CLI),$(OBJ_MYLIB),*)
MYLIB_OBJ_FILES += $(call o_files,$(SRC_MYLIB_OS),$(OBJ_MYLIB),*)

MYLIB = $(DIR_LIB)/libMYLIB.a

mylib: $(MYLIB)

$(MYLIB): $(MYLIB_OBJ_FILES) | $(DIR_LIB)

$(eval $(call compile,$(SRC_MYLIB_HAL),$(OBJ_MYLIB)))
$(eval $(call compile,$(SRC_MYLIB_OS),$(OBJ_MYLIB)))
$(eval $(call compile,$(SRC_MYLIB_CLI),$(OBJ_MYLIB)))
clean-mylib:; rm -rf $(OBJ_MYLIB)