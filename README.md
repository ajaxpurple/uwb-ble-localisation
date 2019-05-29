This repository contains all the code required for localisation:
 - Python GUI with Particle filtering
 - STM32 firmware codebase for WiFi and BLE
 - DWM1001 firmware codebase for UWB and BLE

Python GUI:

STM32:
Use makefile to run code:
Command: (make run PROJ=<project name>)
 - WiFi mobile node (mobile_node)
 - BLE static node (static_node)

DWM1001:
- Install Segger Embedded Studio
- BLE/UWB static node (static)
- BLE/UWB mobile node (mobile)
