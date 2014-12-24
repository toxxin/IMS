#!/bin/bash

# sudo ./st-flash write /home/anton/Source/IMS/Debug/IMS.hex 0x08000000

# lsusb
# ID 0483:3748 SGS Thomson Microelectronics ST-LINK/V2

STLINK_PATH=/home/anton/Distrib/stm32/stm32-tools/stlink/st-flash
BIN=/home/anton/Source/IMS/Debug/IMS.bin

FLASH_ADDR=0x08000000

$STLINK_PATH write $BIN $FLASH_ADDR > /dev/null


