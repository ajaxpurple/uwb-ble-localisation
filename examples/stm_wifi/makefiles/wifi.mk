include makefiles/core.mk
DIR_WIFI = drivers/WiFi
SRC_WIFI = $(DIR_WIFI)/Src
INC_WIFI = $(DIR_WIFI)/Inc
OBJ_WIFI = $(DIR_WIFI)/Obj
WIFI_OBJ_FILES := $(call o_files,$(SRC_WIFI),$(OBJ_WIFI),*)
WIFI_LIB = $(DIR_LIB)/libWiFi.a

.PHONY: wifi
wifi: $(WIFI_LIB)
$(WIFI_LIB): $(WIFI_OBJ_FILES) | $(DIR_LIB)
$(eval $(call compile,$(SRC_WIFI),$(OBJ_WIFI)))
clean-wifi:; rm -rf $(OBJ_WIFI)