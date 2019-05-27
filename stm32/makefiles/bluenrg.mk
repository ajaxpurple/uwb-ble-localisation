include makefiles/core.mk
DIR_BLUENRG = drivers/BlueNRG-MS

SRC_BLUENRG_HCI_LE = $(DIR_BLUENRG)/hci
SRC_BLUENRG = $(DIR_BLUENRG)/hci/controller
SRC_BLUENRG_HCI_TL = $(DIR_BLUENRG)/hci/hci_tl_patterns/Basic
SRC_BLUENRG_UTILS = $(DIR_BLUENRG)/utils

INC_BLUENRG = $(DIR_BLUENRG)/includes $(DIR_BLUENRG)/hci/hci_tl_patterns/Basic $(DIR_BLUENRG)/utils

OBJ_BLUENRG = $(DIR_BLUENRG)/Obj

BLUENRG_OBJ_FILES := $(call o_files,$(SRC_BLUENRG),$(OBJ_BLUENRG),*)
BLUENRG_OBJ_FILES += $(OBJ_BLUENRG)/hci_tl.o $(OBJ_BLUENRG)/hci_tl_interface.o $(OBJ_BLUENRG)/b_l475e_iot01a_bus.o
BLUENRG_OBJ_FILES += $(OBJ_BLUENRG)/hci_le.o
BLUENRG_OBJ_FILES += $(OBJ_BLUENRG)/ble_list.o

BLUENRG_LIB = $(DIR_LIB)/libBLUENRG.a

.PHONY: bluenrg
bluenrg: $(BLUENRG_LIB)
$(BLUENRG_LIB): $(BLUENRG_OBJ_FILES) | $(DIR_LIB)
$(eval $(call compile,$(SRC_BLUENRG),$(OBJ_BLUENRG)))
$(eval $(call compile,$(SRC_BLUENRG_HCI_TL),$(OBJ_BLUENRG)))
$(eval $(call compile,$(SRC_BLUENRG_HCI_LE),$(OBJ_BLUENRG)))
$(eval $(call compile,$(SRC_BLUENRG_UTILS),$(OBJ_BLUENRG)))

clean-bluenrg:; rm -rf $(OBJ_BLUENRG)