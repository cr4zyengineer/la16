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

#pragma mark - opcodes

/* core */
#define LA16_OPCODE_HLT         0b00000000
#define LA16_OPCODE_NOP         0b00000001

/* io */
#define LA16_OPCODE_IN          0b00000010
#define LA16_OPCODE_OUT         0b00000011

/* memory */
#define LA16_OPCODE_LDB         0b00000100
#define LA16_OPCODE_STB         0b00000101
#define LA16_OPCODE_LDW         0b00000110
#define LA16_OPCODE_STW         0b00000111
#define LA16_OPCODE_CASB        0b00001000
#define LA16_OPCODE_CASW        0b00001001
#define LA16_OPCODE_FAAB        0b00001010
#define LA16_OPCODE_FAAW        0b00001011
#define LA16_OPCODE_FENCE       0b00001100

/* data */
#define LA16_OPCODE_MOV         0b00001101
#define LA16_OPCODE_SWP         0b00001110
#define LA16_OPCODE_SWPZ        0b00001111
#define LA16_OPCODE_PUSH        0b00010000
#define LA16_OPCODE_POP         0b00010001

/* arithmetic */
#define LA16_OPCODE_ADD         0b00010010
#define LA16_OPCODE_SUB         0b00010011
#define LA16_OPCODE_MUL         0b00010100
#define LA16_OPCODE_DIV         0b00010101
#define LA16_OPCODE_IDIV        0b00010110
#define LA16_OPCODE_INC         0b00010111
#define LA16_OPCODE_DEC         0b00011000
#define LA16_OPCODE_NOT         0b00011001
#define LA16_OPCODE_NEG         0b00011010
#define LA16_OPCODE_AND         0b00011011
#define LA16_OPCODE_OR          0b00011100
#define LA16_OPCODE_XOR         0b00011101
#define LA16_OPCODE_SHR         0b00011110
#define LA16_OPCODE_SHL         0b00011111
#define LA16_OPCODE_ROL         0b00100000
#define LA16_OPCODE_ROR         0b00100001

/* control flow */
#define LA16_OPCODE_JMP         0b00100010
#define LA16_OPCODE_CMP         0b00100011
#define LA16_OPCODE_JE          0b00100100
#define LA16_OPCODE_JNE         0b00100101
#define LA16_OPCODE_JLT         0b00100110
#define LA16_OPCODE_JGT         0b00100111
#define LA16_OPCODE_BL          0b00101000
#define LA16_OPCODE_RET         0b00101001

/* interrupt controller */
#define LA16_OPCODE_INT         0b00101010
#define LA16_OPCODE_INTSET      0b00101011
#define LA16_OPCODE_INTRET      0b00101100

/* physical memory page protection */
#define LA16_OPCODE_PPCNT       0b00101101
#define LA16_OPCODE_PPKTRRSET   0b00101110

/* virtual memory page protection */
#define LA16_OPCODE_VPSET       0b00101111
#define LA16_OPCODE_VPGET       0b00110000
#define LA16_OPCODE_VPFLGSET    0b00110001
#define LA16_OPCODE_VPFLGGET    0b00110010
#define LA16_OPCODE_VPADDR      0b00110011

#define LA16_OPCODE_MAX         0b00110100

#pragma mark - coder

/* coding combinations */
#define LA16_CODING_COMBINATION_NONE        0b000
#define LA16_CODING_COMBINATION_IMM6        0b001
#define LA16_CODING_COMBINATION_REG         0b010
#define LA16_CODING_COMBINATION_REG_REG     0b011
#define LA16_CODING_COMBINATION_IMM16       0b100
#define LA16_CODING_COMBINATION_IMM16_REG   0b101
#define LA16_CODING_COMBINATION_REG_IMM16   0b110
#define LA16_CODING_COMBINATION_IMM11_IMM11 0b111

typedef struct la16_decoder_resources {
    unsigned char imm6[2];
    unsigned short imm16;
} la16_decoder_resources_t;

typedef struct la16_coder_instruction_layout {
    unsigned char opcode;
    unsigned char mode;
    la16_decoder_resources_t res;
} la16_coder_instruction_layout_t;

#pragma mark - register

/* special purpose register */
#define LA16_REGISTER_PC        0b000000
#define LA16_REGISTER_SP        0b000001
#define LA16_REGISTER_FP        0b000010
#define LA16_REGISTER_CF        0b000011

/* general purpose register */
#define LA16_REGISTER_R0        0b000100
#define LA16_REGISTER_R1        0b000101
#define LA16_REGISTER_R2        0b000110
#define LA16_REGISTER_R3        0b000111
#define LA16_REGISTER_R4        0b001000
#define LA16_REGISTER_R5        0b001001
#define LA16_REGISTER_R6        0b001010
#define LA16_REGISTER_R7        0b001011
#define LA16_REGISTER_R8        0b001100
#define LA16_REGISTER_R9        0b001101
#define LA16_REGISTER_R10       0b001110
#define LA16_REGISTER_R11       0b001111
#define LA16_REGISTER_R12       0b010000
#define LA16_REGISTER_R13       0b010001
#define LA16_REGISTER_R14       0b010010
#define LA16_REGISTER_R15       0b010011
#define LA16_REGISTER_R16       0b010100
#define LA16_REGISTER_R17       0b010101
#define LA16_REGISTER_R18       0b010110
#define LA16_REGISTER_R19       0b010111
#define LA16_REGISTER_R20       0b011000
#define LA16_REGISTER_R21       0b011001
#define LA16_REGISTER_R22       0b011010
#define LA16_REGISTER_R23       0b011011
#define LA16_REGISTER_R24       0b011100
#define LA16_REGISTER_R25       0b011101
#define LA16_REGISTER_R26       0b011110
#define LA16_REGISTER_R27       0b011111
#define LA16_REGISTER_R28       0b100000
#define LA16_REGISTER_R29       0b100001
#define LA16_REGISTER_R30       0b100010
#define LA16_REGISTER_R31       0b100011
#define LA16_REGISTER_R32       0b100100
#define LA16_REGISTER_R33       0b100101
#define LA16_REGISTER_R34       0b100110
#define LA16_REGISTER_R35       0b100111
#define LA16_REGISTER_R36       0b101000
#define LA16_REGISTER_R37       0b101001
#define LA16_REGISTER_R38       0b101010
#define LA16_REGISTER_R39       0b101011
#define LA16_REGISTER_R40       0b101100
#define LA16_REGISTER_R41       0b101101
#define LA16_REGISTER_R42       0b101110
#define LA16_REGISTER_R43       0b101111
#define LA16_REGISTER_R44       0b110000
#define LA16_REGISTER_R45       0b110001
#define LA16_REGISTER_R46       0b110010
#define LA16_REGISTER_R47       0b110011
#define LA16_REGISTER_RR0       0b110100
#define LA16_REGISTER_RR1       0b110101
#define LA16_REGISTER_RR2       0b110110
#define LA16_REGISTER_RR3       0b110111
#define LA16_REGISTER_RR4       0b111000
#define LA16_REGISTER_RR5       0b111001
#define LA16_REGISTER_RR6       0b111010

#define LA16_REGISTER_EL0_MAX   LA16_REGISTER_RR6

/* control register */
#define LA16_REGISTER_EL        0b111011
#define LA16_REGISTER_ELB       0b111100
#define LA16_REGISTER_TR        0b111101
#define LA16_REGISTER_TLBP      0b111110
#define LA16_REGISTER_VTLBP     0b111111

#define LA16_REGISTER_EL1_MAX   LA16_REGISTER_VTLBP

#pragma mark - compare flag

#define LA16_CMP_Z 0x0001
#define LA16_CMP_L 0x0002
#define LA16_CMP_G 0x0004

#pragma mark - elevation modes

#define LA16_CORE_MODE_EL0 0b0
#define LA16_CORE_MODE_EL1 0b1

#define la16_core_is_user_level(core) ((*core->rl[LA16_REGISTER_EL]) == LA16_CORE_MODE_EL0)
#define la16_core_is_kernel_level(core) ((*core->rl[LA16_REGISTER_EL]) == LA16_CORE_MODE_EL1)

#pragma mark - termination flags

#define LA16_TERM_FLAG_NONE         0b00
#define LA16_TERM_FLAG_HALT         0b01
#define LA16_TERM_FLAG_BAD_ACCESS   0b10
#define LA16_TERM_FLAG_PERMISSION   0b11

#pragma mark - virtual page entry flags

#define LA16_PAGEU_FLAG_NONE    0b00000
#define LA16_PAGEU_FLAG_MAPPED  0b00001
#define LA16_PAGEU_FLAG_READ    0b00010
#define LA16_PAGEU_FLAG_WRITE   0b00100
#define LA16_PAGEU_FLAG_EXEC    0b01000
#define LA16_PAGEU_FLAG_COW     0b10000

#pragma mark - main implementation

typedef struct la16_machine la16_machine_t;

struct la16_core
{
    /* Special Purpose Register */
    la16_register_t pc;     /* program counter */
    la16_register_t sp;     /* stack pointer */
    la16_register_t fp;     /* frame pointer */
    la16_register_t cf;     /* compare flag*/

    /* Kernel level registers */
    la16_register_t el;     /* elevation level */
    la16_register_t elb;    /* elevation level backup (safer than loading it from virtual address space stack memory :skull: )*/

    la16_register_t rl[LA16_REGISTER_EL1_MAX];

    /* Opertion registers */
    unsigned short imm16;
    unsigned short imm6[2];
    unsigned char op;
    unsigned short *pa;
    unsigned short *pb;
    unsigned char pc_inc;

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
