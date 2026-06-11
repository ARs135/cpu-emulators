# SOC-8
It isn't a sock.

---

Stands for **S**imple **O**perator of **C**omputing **8**-bit. \
Hence for this stupendous name that is similar to an item you wear around your foot.

This is one of my own CPUs that I've designed by myself
## Specifications:
- Harvard Architecture (IROM and RAM are distinct, they're not unified unlike the Von Neumann)
- RISC Architecture (It has a fix instruction width)
- Register File: 8 8-bit General Purpose Registers (r0-r7, you can't really use r0 to store any value cuz it's always 0 :P also obviously it's 8-bits. Also enjoy the limited amount of register programmers :D)
- Instruction Memory/IROM: 16-bit fixed instruction width, 1024 addresses
- Random Access Memory/RAM: 8-bit word size (duh it's an 8-bit CPU), 256 Addresses
- Call Stack: 16-deep Hardware Call Stack
- Program Counter: 10-bits (well obviously, if you can not do math, 2^10 = 1024)
- Flags: Zero and Carry Flags (also Not Zero and Not Carry)

## ISA
placeholder (I have no idea how do I style the ISA here)