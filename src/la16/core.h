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

enum LA16_OPCODE
{
    LA16_OPCODE_HLT             = 0b00000000,

    LA16_OPCODE_MOV             = 0b00000001,
    LA16_OPCODE_MOVZ            = 0b00000010,
    LA16_OPCODE_CPY             = 0b00000011,
    LA16_OPCODE_MLD             = 0b00000100,
    LA16_OPCODE_MST             = 0b00000101,
    LA16_OPCODE_IN              = 0b00000110,
    LA16_OPCODE_OUT             = 0b00000111,
    LA16_OPCODE_PUSH            = 0b00001000,
    LA16_OPCODE_POP             = 0b00001001,

    LA16_OPCODE_ADD             = 0b00001010,
    LA16_OPCODE_SUB             = 0b00001011,
    LA16_OPCODE_MUL             = 0b00001100,
    LA16_OPCODE_DIV             = 0b00001101,
    LA16_OPCODE_IDIV            = 0b00001110,
    LA16_OPCODE_INC             = 0b00001111,
    LA16_OPCODE_DEC             = 0b00010000,
    LA16_OPCODE_NOT             = 0b00010001,
    LA16_OPCODE_AND             = 0b00010010,
    LA16_OPCODE_OR              = 0b00010011,
    LA16_OPCODE_XOR             = 0b00010100,
    LA16_OPCODE_SHR             = 0b00010101,
    LA16_OPCODE_SHL             = 0b00010110,
    LA16_OPCODE_ROR             = 0b00010111,
    LA16_OPCODE_ROL             = 0b00011000,

    LA16_OPCODE_JMP             = 0b00011001,
    LA16_OPCODE_CMP             = 0b00011010,
    LA16_OPCODE_JE              = 0b00011011,
    LA16_OPCODE_JNE             = 0b00011100,
    LA16_OPCODE_JLT             = 0b00011101,
    LA16_OPCODE_JGT             = 0b00011110,
    LA16_OPCODE_BL              = 0b00011111,
    LA16_OPCODE_BLE             = 0b00100000,
    LA16_OPCODE_BLNE            = 0b00100001,
    LA16_OPCODE_BLLT            = 0b00100010,
    LA16_OPCODE_BLGT            = 0b00100011,
    LA16_OPCODE_RET             = 0b00100100,

    LA16_OPCODE_INT             = 0b00100101,
    LA16_OPCODE_INTSET          = 0b00100110,
    LA16_OPCODE_INTCLEAR        = 0b00100111,
    LA16_OPCODE_INTRET          = 0b00101000,

    LA16_OPCODE_MPAGEMAP        = 0b00101001,
    LA16_OPCODE_MPAGEUNMAP      = 0b00101010,
    LA16_OPCODE_MPAGEUNMAPALL   = 0b00101011,
    LA16_OPCODE_MPAGEPROT       = 0b00101100,
    LA16_OPCODE_MPAGECLEAR      = 0b00101101,
    LA16_OPCODE_MPAGEAVAL       = 0b00101110,
    LA16_OPCODE_MPAGECOUNT      = 0b00101111,
    LA16_OPCODE_MADDR           = 0b00110000,

    LA16_OPCODE_CRRESUME        = 0b00110001,
    LA16_OPCODE_CRSTOP          = 0b00110010,
    LA16_OPCODE_CRDUMP          = 0b00110011,
    LA16_OPCODE_CRFLASH         = 0b00110100,
    LA16_OPCODE_CRTIMESET       = 0b00110101,
    LA16_OPCODE_CRCTXHNDLSET    = 0b00110110,
    LA16_OPCODE_CREXCHNDLSET    = 0b00110111,

    LA16_OPCODE_KTRRSET         = 0b00111000,

    LA16_OPCODE_NOP             = 0b00111001,

    LA16_OPCODE_MAX             = 0b00111010
};

/*
 * Illegal parameter type combination will always result in the CPU CORE executing LA16_OPCODE_HLT
 *
 */
enum LA16_PTYPE
{
    LA16_PTYPE_NONE = 0b00,
    LA16_PTYPE_REG  = 0b01,
    LA16_PTYPE_IMM  = 0b10,
    LA16_PTYPE_IMM8 = 0b11,
};

enum LA16_PTCRYPT_COMBO
{
    LA16_PTCRYPT_COMBO_NONE_NONE = 0b0000,
    LA16_PTCRYPT_COMBO_NONE_REG  = 0b0001,
    LA16_PTCRYPT_COMBO_NONE_IMM  = 0b0010,
    LA16_PTCRYPT_COMBO_NONE_IMM8 = 0b0011,  /* new ABI */
    LA16_PTCRYPT_COMBO_REG_NONE  = 0b0100,
    LA16_PTCRYPT_COMBO_REG_REG   = 0b0101,
    LA16_PTCRYPT_COMBO_REG_IMM   = 0b0110,
    LA16_PTCRYPT_COMBO_REG_IMM8  = 0b0111,  /* new ABI */
    LA16_PTCRYPT_COMBO_IMM_NONE  = 0b1000,
    LA16_PTCRYPT_COMBO_IMM_REG   = 0b1001, 
    LA16_PTCRYPT_COMBO_IMM_IMM   = 0b1010,  /* ILLEGAL */
    LA16_PTCRYPT_COMBO_IMM_IMM8  = 0b1011,  /* new ABI */
    LA16_PTCRYPT_COMBO_IMM8_NONE = 0b1100,  /* new ABI */
    LA16_PTCRYPT_COMBO_IMM8_REG  = 0b1101,  /* new ABI */
    LA16_PTCRYPT_COMBO_IMM8_IMM  = 0b1110,  /* new ABI */
    LA16_PTCRYPT_COMBO_IMM8_IMM8 = 0b1111   /* new ABI */
};

enum LA16_PTRES_COMBO
{
    LA16_PTRES_COMBO_4B      = 0b000,
    LA16_PTRES_COMBO_16B     = 0b001,
    LA16_PTRES_COMBO_4B_4B   = 0b010,
    LA16_PTRES_COMBO_4B_16B  = 0b011
};

enum LA16_REGISTER
{
    LA16_REGISTER_PC = 0b0000,
    LA16_REGISTER_SP = 0b0001,
    LA16_REGISTER_FP = 0b0010,
    LA16_REGISTER_CF = 0b0011,
    LA16_REGISTER_R0 = 0b0100,
    LA16_REGISTER_R1 = 0b0101,
    LA16_REGISTER_R2 = 0b0110,
    LA16_REGISTER_R3 = 0b0111,
    LA16_REGISTER_R4 = 0b1000,
    LA16_REGISTER_R5 = 0b1001,
    LA16_REGISTER_R6 = 0b1010,
    LA16_REGISTER_R7 = 0b1011,
    LA16_REGISTER_R8 = 0b1100,
    LA16_REGISTER_RR = 0b1101,

    LA16_REGISTER_EL0_MAX = 0b1110,

    LA16_REGISTER_EL = 0b1110,
    LA16_REGISTER_ELB = 0b1111,

    LA16_REGISTER_EL1_MAX = 0b10000
};

enum LA16_CMP
{
    LA16_CMP_EQ = 0b0001,
    LA16_CMP_IL = 0b0010,
    LA16_CMP_IG = 0b0011
};

enum LA16_CORE_MODE
{
    LA16_CORE_MODE_EL0 = 0b00000000, /* Userspace */
    LA16_CORE_MODE_EL1 = 0b00000001, /* Kernel level */
};

enum LA16_TERM_FLAG
{
    LA16_TERM_FLAG_NONE         = 0b00000000,
    LA16_TERM_FLAG_HALT         = 0b00000001,
    LA16_TERM_FLAG_BAD_ACCESS   = 0b00000010,
    LA16_TERM_FLAG_PERMISSION   = 0b00000011
};

struct la16_decoder_resources
{
    unsigned char a[2];
    unsigned short b;
};

typedef struct la16_decoder_resources la16_decoder_resources_t;
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
    unsigned short imm;
    unsigned char op;
    unsigned short *pa;
    unsigned short *pb;

    /* Exec flags */
    unsigned char runs;
    unsigned char term;

    /* Machine related things */
    la16_machine_t *machine;
    unsigned short page[300];
    unsigned char pageu[300];
};

typedef struct la16_core* la16_core_t;
typedef void (*la16_opfunc_t)(la16_core_t core);

extern la16_opfunc_t opfunc_table[];

la16_core_t la16_core_alloc();
void la16_core_dealloc(la16_core_t core);
void la16_core_execute(la16_core_t core);
void la16_core_terminate(la16_core_t core);

#endif /* LA16_CORE_H */
