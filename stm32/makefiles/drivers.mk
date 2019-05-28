include makefiles/core.mk
include makefiles/rtos.mk
include makefiles/bsp.mk
include makefiles/argtable.mk
include makefiles/bluenrg.mk
include makefiles/wifi.mk
include makefiles/hal.mk
include makefiles/mylib.mk
include makefiles/device.mk


DIR_CONFIG = drivers/Config
DIR_ROOT = /usr/arm-none-eabi/include

ALL_LIB =  $(MYLIB) $(BSP_LIB) $(RTOS_LIB) $(BLUENRG_LIB) $(WIFI_LIB) $(HAL_LIB) $(ARGTABLE_LIB)
EX_LIBS = $(ALL_LIB) $(DEV_OBJ_FILES)
INC = $(INC_BSP) $(INC_HAL) $(INC_DEV) $(INC_CMSIS) $(INC_RTOS) $(INC_BLUENRG) $(INC_WIFI) $(INC_MYLIB) $(INC_ARGTABLE) \
 		 $(DIR_CONFIG) $(DIR_ROOT) \
		 
ALL_OBJ = $(OBJ_DEV) $(OBJ_BSP) $(OBJ_MYLIB) $(OBJ_HAL) $(OBJ_RTOS) $(OBJ_ARGTABLE) $(OBJ_BLUENRG) $(OBJ_WIFI) 

.PHONY: lib

lib: $(EX_LIBS)