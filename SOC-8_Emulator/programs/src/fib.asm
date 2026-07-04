; basic fib program coded by me :D (ARs135)
LDI r1 0
LDI r2 1

.loop
ADD r3 r1 r2
BRH C .halt
ADD r1 r2 r0
ADD r2 r3 r0
JMP .loop

.halt
HLT