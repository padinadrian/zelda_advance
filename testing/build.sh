#!/usr/bin/env bash

/opt/devkitpro/devkitARM/bin/arm-none-eabi-gcc -o red.elf -specs=gba.specs red.s
/opt/devkitpro/devkitARM/bin/arm-none-eabi-objcopy -O binary red.elf red.gba
gbafix red.gba


