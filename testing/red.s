.arm
.text
.global main
main:
    @ r0 = 0x0400 0000 (I/0 registers)
    mov r0, #0x4000000
    @ r1 = 0x400
    mov r1, #0x400
    @ r1 = 0x403
    add r1, r1, #3
    @ [0x04000000] = 403
    str r1, [r0]

    @ r0 = 0x0600 0000 (Display memory)
    mov r0, #0x6000000
    @ Column counter
    mov r3, #0xA0

loopAll:
    @ Set color to blue
    mov r1, #0x9900
    add r1, r1, #0x99
    @ Row counter
    mov r2, #0x50

loopR:
    @ strh - store register halfword - calculates an address from a base register value and an immediate offset, and stores a halfword from a register to memory
    @ [0x06000000] = 0xFF and r0 += 2
    strh r1, [r0], #2
    @ counter -= 1
    subs r2, r2, #1
    bne loopR

    @ Set color to white
    mov r1, #0x9900
    add r1, r1, #0x99
    @ Row counter
    mov r2, #0x50

loopG:
    @ strh - store register halfword - calculates an address from a base register value and an immediate offset, and stores a halfword from a register to memory
    @ [0x06000000] = 0xFF and r0 += 2
    strh r1, [r0], #2
    @ counter -= 1
    subs r2, r2, #1
    bne loopG

    @ Set color to red
    mov r1, #0x9900
    add r1, r1, #0x99
    @ Row counter
    mov r2, #0x50

loopB:
    @ strh - store register halfword - calculates an address from a base register value and an immediate offset, and stores a halfword from a register to memory
    @ [0x06000000] = 0xFF and r0 += 2
    strh r1, [r0], #2
    @ counter -= 1
    subs r2, r2, #1
    bne loopB

    @ Column counter -= 1
    subs r3, r3, #1
    bne loopAll

infin:
    b infin
