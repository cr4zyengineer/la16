/*
 * MIT License
 *
 * Copyright (c) 2024 cr4zyengineer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LA16_CORE_H
#define LA16_CORE_H

#include <la16/register.h>

enum LA16_OPCODE {
    /* core operations */
    LA16_OPCODE_HLT             = 0b00000000,
    LA16_OPCODE_NOP             = 0b00000001,

    /* io operations */
    LA16_OPCODE_IN              = 0b00000010,
    LA16_OPCODE_OUT             = 0b00000011,

    /* memory operations */
    LA16_OPCODE_LDB             = 0b00000100,
    LA16_OPCODE_STB             = 0b00000101,
    LA16_OPCODE_LDW             = 0b00000110,
    LA16_OPCODE_STW             = 0b00000111,
    LA16_OPCODE_CASB            = 0b00001000,
    LA16_OPCODE_CASW            = 0b00001001,
    LA16_OPCODE_FAAB            = 0b00001010,
    LA16_OPCODE_FAAW            = 0b00001011,
    LA16_OPCODE_FENCE           = 0b00001100,

    /* data operations */
    LA16_OPCODE_MOV             = 0b00001101,
    LA16_OPCODE_SWP             = 0b00001110,
    LA16_OPCODE_SWPZ            = 0b00001111,
    LA16_OPCODE_PUSH            = 0b00010000,
    LA16_OPCODE_POP             = 0b00010001,

    /* arithmetic operations */
    LA16_OPCODE_ADD             = 0b00010010,
    LA16_OPCODE_SUB             = 0b00010011,
    LA16_OPCODE_MUL             = 0b00010100,
    LA16_OPCODE_DIV             = 0b00010101,
    LA16_OPCODE_IDIV            = 0b00010110,
    LA16_OPCODE_INC             = 0b00010111,
    LA16_OPCODE_DEC             = 0b00011000,
    LA16_OPCODE_NOT             = 0b00011001,
    LA16_OPCODE_AND             = 0b00011010,
    LA16_OPCODE_OR              = 0b00011011,
    LA16_OPCODE_XOR             = 0b00011100,
    LA16_OPCODE_SHR             = 0b00011101,
    LA16_OPCODE_SHL             = 0b00011110,
    LA16_OPCODE_ROR             = 0b00011111,
    LA16_OPCODE_ROL             = 0b00100000,

    /* control flow operations */
    LA16_OPCODE_JMP             = 0b00100001,
    LA16_OPCODE_CMP             = 0b00100010,
    LA16_OPCODE_JE              = 0b00100011,
    LA16_OPCODE_JNE             = 0b00100100,
    LA16_OPCODE_JLT             = 0b00100101,
    LA16_OPCODE_JGT             = 0b00100110,
    LA16_OPCODE_JLE             = 0b00100111,
    LA16_OPCODE_JGE             = 0b00101000,
    LA16_OPCODE_BL              = 0b00101001,
    LA16_OPCODE_RET             = 0b00101010,

    /* interupt controller operations */
    LA16_OPCODE_INT             = 0b00101011,
    LA16_OPCODE_INTSET          = 0b00101100,
    LA16_OPCODE_INTRET          = 0b00101101,

    /* memory protection operations */
    LA16_OPCODE_PPCNT           = 0b00101110,
    LA16_OPCODE_PPKTRRSET       = 0b00101111,
    LA16_OPCODE_VPSET           = 0b00110000,
    LA16_OPCODE_VPGET           = 0b00110001,
    LA16_OPCODE_VPFLGSET        = 0b00110010,
    LA16_OPCODE_VPFLGGET        = 0b00110011,
    LA16_OPCODE_VPADDR          = 0b00110100,

    /* core concurrency operations (unlikely to be implemented) */
    LA16_OPCODE_CRRESUME        = 0b00110101,
    LA16_OPCODE_CRSTOP          = 0b00110110,
    LA16_OPCODE_CRDUMP          = 0b00110111,
    LA16_OPCODE_CRFLASH         = 0b00111000,
    LA16_OPCODE_CRTIMESET       = 0b00111001,
    LA16_OPCODE_CRCTXHNDLSET    = 0b00111010,
    LA16_OPCODE_CREXCHNDLSET    = 0b00111011,

    LA16_OPCODE_MAX             = LA16_OPCODE_CREXCHNDLSET
};

enum LA16_PARAMETER_CODING_COMBINATION {
    LA16_PARAMETER_CODING_COMBINATION_NONE          = 0b000,
    LA16_PARAMETER_CODING_COMBINATION_REG           = 0b001,
    LA16_PARAMETER_CODING_COMBINATION_REG_REG       = 0b010,
    LA16_PARAMETER_CODING_COMBINATION_IMM16         = 0b011,
    LA16_PARAMETER_CODING_COMBINATION_IMM16_REG     = 0b100,
    LA16_PARAMETER_CODING_COMBINATION_REG_IMM16     = 0b101,
    LA16_PARAMETER_CODING_COMBINATION_IMM8_IMM8     = 0b110     /* in ISA v2.3 it will be IMM10_IMM10 */
};

enum LA16_REGISTER {
    LA16_REGISTER_PC =      0b00000,
    LA16_REGISTER_SP =      0b00001,
    LA16_REGISTER_FP =      0b00010,
    LA16_REGISTER_CF =      0b00011,

    LA16_REGISTER_R0 =      0b00100,
    LA16_REGISTER_R1 =      0b00101,
    LA16_REGISTER_R2 =      0b00110,
    LA16_REGISTER_R3 =      0b00111,
    LA16_REGISTER_R4 =      0b01000,
    LA16_REGISTER_R5 =      0b01001,
    LA16_REGISTER_R6 =      0b01010,
    LA16_REGISTER_R7 =      0b01011,
    LA16_REGISTER_R8 =      0b01100,
    LA16_REGISTER_R9 =      0b01101,
    LA16_REGISTER_R10 =     0b01110,
    LA16_REGISTER_R11 =     0b01111,
    LA16_REGISTER_R12 =     0b10000,
    LA16_REGISTER_R13 =     0b10001,
    LA16_REGISTER_R14 =     0b10010,
    LA16_REGISTER_R15 =     0b10011,
    LA16_REGISTER_R16 =     0b10100,
    LA16_REGISTER_R17 =     0b10101,
    LA16_REGISTER_R18 =     0b10110,
    LA16_REGISTER_R19 =     0b10111,
    LA16_REGISTER_R20 =     0b11000,
    LA16_REGISTER_R21 =     0b11001,
    LA16_REGISTER_R22 =     0b11010,
    LA16_REGISTER_R23 =     0b11011,
    LA16_REGISTER_R24 =     0b11100,
    LA16_REGISTER_RR =      0b11101,

    LA16_REGISTER_EL0_MAX = LA16_REGISTER_RR,

    LA16_REGISTER_EL =      0b11110,
    LA16_REGISTER_ELB =     0b11111,

    LA16_REGISTER_EL1_MAX = LA16_REGISTER_ELB
};

enum LA16_CMP {
    LA16_CMP_Z = 0x0001,
    LA16_CMP_L = 0x0002,
    LA16_CMP_G = 0x0004
};

enum LA16_CORE_MODE {
    LA16_CORE_MODE_EL0 = 0b00000000, /* Userspace */
    LA16_CORE_MODE_EL1 = 0b00000001, /* Kernel level */
};

enum LA16_TERM_FLAG {
    LA16_TERM_FLAG_NONE         = 0b00000000,
    LA16_TERM_FLAG_HALT         = 0b00000001,
    LA16_TERM_FLAG_BAD_ACCESS   = 0b00000010,
    LA16_TERM_FLAG_PERMISSION   = 0b00000011
};

enum LA16_PAGEU_FLAG {
    LA16_PAGEU_FLAG_NONE =      0b00000000,
    LA16_PAGEU_FLAG_MAPPED =    0b00000001,
    LA16_PAGEU_FLAG_READ =      0b00000010,
    LA16_PAGEU_FLAG_WRITE =     0b00000100,
    LA16_PAGEU_FLAG_EXEC =      0b00001000
};

typedef struct la16_machine la16_machine_t;

typedef struct {
    unsigned char op;
    unsigned char reg[2];
    unsigned short imm;
    unsigned short imm8[2];
    unsigned short *pa;
    unsigned short *pb;
} la16_operation_t;

struct la16_core {
    /* Special Purpose Register */
    la16_register_t pc;     /* program counter */
    la16_register_t sp;     /* stack pointer */
    la16_register_t fp;     /* frame pointer */
    la16_register_t cf;     /* compare flag*/

    /* Kernel level registers */
    la16_register_t el;     /* elevation level */
    la16_register_t elb;    /* elevation level backup (safer than loading it from virtual address space stack memory :skull: )*/

    la16_register_t rl[LA16_REGISTER_EL1_MAX + 1];

    /* Opertion registers */
    la16_operation_t op;

    /* Exec flags */
    unsigned char runs;
    unsigned char term;

    /* Machine related things */
    la16_machine_t *machine;
    unsigned short page[257];
    unsigned char pageu[257];
};

typedef struct la16_core* la16_core_t;
typedef void (*la16_opfunc_t)(la16_core_t core);

extern la16_opfunc_t opfunc_table[];

la16_core_t la16_core_alloc();
void la16_core_dealloc(la16_core_t core);
void la16_core_execute(la16_core_t core);
void la16_core_terminate(la16_core_t core);

#endif /* LA16_CORE_H */
