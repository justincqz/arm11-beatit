ldr r0,=0x20200000
ldr r2,[r0,#4]

mov r1,#0x00010000
mov r5,#1000

loop:
mov r4,r5
onloop:
str r1,[r0,#28]
sub r4,r4,#1
cmp r4,#0
bne onloop

mov r4,r5

offloop:
ldr r1,[r0,#40]
sub r4,r4,#1
cmp r4,#0
bne offloop

b loop
