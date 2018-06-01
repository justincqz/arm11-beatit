ldr r0,=0x20200000
ldr r2,[r0,#4]

ldr r1,=0xffffffff
mov r5,#1000

onloop:
str r1,[r0,#28]
b onloop
