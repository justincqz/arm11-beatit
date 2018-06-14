ldr r0,=0x20200000 
mov r1,#1
lsl r1,#18
str r1,[r0,#4]

mov r1,r1,lsr #2 

loop:

str r1,[r0,#40]
mov r2,#0xF0000

offloop:
sub r2,r2,#1
cmp r2,#0
bne offloop

str r1,[r0,#28]
mov r2,#0xF0000

onloop:
sub r2,r2,#1
cmp r2,#0
bne onloop

b loop
