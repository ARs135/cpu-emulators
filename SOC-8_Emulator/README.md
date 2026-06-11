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

| Mnemonic | Description | Opcode | Instruction Format |
|----------|-------------|--------|------------------|
| NOP | No Operation | 0000 | No Operands |
| HLT | Halt | 0001 | No Operands |
| ADD | Add | 0010 | RRR |
| SUB | Subtract | 0011 | RRR |
| NAND | Bitwise NAND | 0100 | RRR |
| OR | Bitwise OR | 0101 | RRR |
| XOR | Bitwise XOR | 0110 | RRR |
| RSH | Right Shift | 0111 | RR |
| LDI | Load Immediate | 1000 | RI |
| ADI | Add Immediate | 1001 | RI |
| JMP | Jump | 1010 | Addr10 |
| BRH | Branch | 1011 | CAddr10 |
| CALL | Call | 1100 | Addr10 |
| RET | Return | 1101 | No Operands |
| LOD | Load | 1110 | RdAddr |
| STR | Store | 1111 | RsAddr |

### Instruction Formats

O - Nothing/Unused \
D - Destination Register \
X - Source 1 Register \
Y - Source 2 Register \
I - Immediate \
A - RAM Address \
C - Cond \
R - IROM Address

| Format | Bit Layout |
|--------|------------|
| No Operands | OOOO OOOO OOOO |
| RRR | DDDX XXYY YOOO |
| RR | DDDX XXOO OOOO |
| RI | DDDI IIII IIIO |
| Addr10 | OORR RRRR RRRR |
| CAddr10 | CCRR RRRR RRRR |
| RdAddr | DDDA AAAA AAAO |
| RsAddr | XXXA AAAA AAAO |

---

Oh welp, I didn't put a joke in this README in a bit. Time to end it with a joke \
*ahem* \
What did the computer do with the snack? \
It took *byte* out of it.