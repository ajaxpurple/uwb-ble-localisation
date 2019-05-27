include makefiles/core.mk
DIR_USB = drivers/ST/STM32_USB_Device_Library
SRC_USB_CORE = $(DIR_USB)/Core/Src
SRC_USB_HID = $(DIR_USB)/Class/HID/Src

INC_USB_CORE = $(DIR_USB)/Core/Inc
INC_USB_HID = $(DIR_USB)/Class/HID/Inc
INC_USB = $(INC_USB_CORE) $(INC_USB_HID)

OBJ_USB = $(DIR_USB)/Obj

USB_OBJ_FILES := $(call o_files,$(SRC_USB_CORE),$(OBJ_USB),*)
USB_OBJ_FILES += $(call o_files,$(SRC_USB_HID),$(OBJ_USB),*)

USB_LIB = $(DIR_LIB)/libUSB.a

usb: $(USB_LIB)
$(USB_LIB): $(USB_OBJ_FILES) | $(DIR_LIB)
$(eval $(call compile,$(SRC_USB_CORE),$(OBJ_USB)))
$(eval $(call compile,$(SRC_USB_HID),$(OBJ_USB)))

clean-usb:; rm -rf $(OBJ_USB)