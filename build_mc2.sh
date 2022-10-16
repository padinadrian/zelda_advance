#!/usr/bin/env bash

/opt/devkitpro/devkitARM/bin/arm-none-eabi-gcc -o mc2.elf -specs=gba.specs minish_cap2.s
/opt/devkitpro/devkitARM/bin/arm-none-eabi-objcopy -O binary mc2.elf mc2.gba
gbafix mc2.gba


