#!/usr/bin/env bash

/opt/devkitpro/devkitARM/bin/arm-none-eabi-gcc -o zelda.elf -specs=gba.specs minish_cap.annotated.s
/opt/devkitpro/devkitARM/bin/arm-none-eabi-objcopy -O binary zelda.elf zelda.gba
gbafix zelda.gba


