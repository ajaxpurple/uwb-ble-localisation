include makefiles/core.mk
RTOS_HEAP = heap_4
DIR_RTOS = drivers/FreeRTOS/FreeRTOS/Source
DIR_RTOS_PORT = $(DIR_RTOS)/portable/GCC/ARM_CM4F

SRC_RTOS = $(DIR_RTOS)
SRC_RTOS_PORT = $(DIR_RTOS_PORT)
SRC_RTOS_MEM = $(DIR_RTOS)/portable/MemMang

INC_RTOS = $(DIR_RTOS)/include $(DIR_RTOS_PORT)
OBJ_RTOS = $(DIR_RTOS)/Obj
RTOS_OBJ_FILES := $(call o_files,$(SRC_RTOS),$(OBJ_RTOS),*)
RTOS_OBJ_FILES += $(OBJ_RTOS)/port.o $(OBJ_RTOS)/$(RTOS_HEAP).o
RTOS_LIB = $(DIR_LIB)/libRTOS.a

.PHONY: rtos
rtos: $(RTOS_LIB)
$(RTOS_LIB): $(RTOS_OBJ_FILES) | $(DIR_LIB)
$(eval $(call compile,$(SRC_RTOS),$(OBJ_RTOS)))
$(eval $(call compile,$(SRC_RTOS_PORT),$(OBJ_RTOS)))
$(eval $(call compile,$(SRC_RTOS_MEM),$(OBJ_RTOS)))

clean-rtos:; rm -rf $(OBJ_RTOS)