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

#include <compiler/opcode.h>
#include <stdlib.h>
#include <string.h>

opcode_entry_t opcode_table[LA16_OPCODE_MAX] = {
    /* core operations */
    { .name = "hlt", .opcode = LA16_OPCODE_HLT },
    { .name = "nop", .opcode = LA16_OPCODE_NOP },

    /* io operations */
    { .name = "in", .opcode = LA16_OPCODE_IN },
    { .name = "out", .opcode = LA16_OPCODE_OUT },

    /* memory operations */
    { .name = "ldb", .opcode = LA16_OPCODE_LDB },
    { .name = "stb", .opcode = LA16_OPCODE_STB },
    { .name = "ldw", .opcode = LA16_OPCODE_LDW },
    { .name = "stw", .opcode = LA16_OPCODE_STW },
    { .name = "casb", .opcode = LA16_OPCODE_CASB },
    { .name = "casw", .opcode = LA16_OPCODE_CASW },
    { .name = "faab", .opcode = LA16_OPCODE_FAAB },
    { .name = "faaw", .opcode = LA16_OPCODE_FAAW },
    { .name = "fence", .opcode = LA16_OPCODE_FENCE },

    /* data operations */
    { .name = "mov", .opcode = LA16_OPCODE_MOV },
    { .name = "swp", .opcode = LA16_OPCODE_SWP },
    { .name = "swpz", .opcode = LA16_OPCODE_SWPZ },
    { .name = "push", .opcode = LA16_OPCODE_PUSH },
    { .name = "pop", .opcode = LA16_OPCODE_POP },

    /* arithmetic operations */
    { .name = "add", .opcode = LA16_OPCODE_ADD },
    { .name = "sub", .opcode = LA16_OPCODE_SUB },
    { .name = "mul", .opcode = LA16_OPCODE_MUL },
    { .name = "div", .opcode = LA16_OPCODE_DIV },
    { .name = "idiv", .opcode = LA16_OPCODE_IDIV },
    { .name = "inc", .opcode = LA16_OPCODE_INC },
    { .name = "dec", .opcode = LA16_OPCODE_DEC },
    { .name = "not", .opcode = LA16_OPCODE_NOT },
    { .name = "and", .opcode = LA16_OPCODE_AND },
    { .name = "or", .opcode = LA16_OPCODE_OR },
    { .name = "xor", .opcode = LA16_OPCODE_XOR },
    { .name = "shr", .opcode = LA16_OPCODE_SHR },
    { .name = "shl", .opcode = LA16_OPCODE_SHL },
    { .name = "ror", .opcode = LA16_OPCODE_ROR },
    { .name = "rol", .opcode = LA16_OPCODE_ROL },

    /* control flow operations */
    { .name = "jmp", .opcode = LA16_OPCODE_JMP },
    { .name = "cmp", .opcode = LA16_OPCODE_CMP },
    { .name = "je", .opcode = LA16_OPCODE_JE },
    { .name = "jne", .opcode = LA16_OPCODE_JNE },
    { .name = "jlt", .opcode = LA16_OPCODE_JLT },
    { .name = "jgt", .opcode = LA16_OPCODE_JGT },
    { .name = "jle", .opcode = LA16_OPCODE_JLE },
    { .name = "jge", .opcode = LA16_OPCODE_JGE },
    { .name = "bl", .opcode = LA16_OPCODE_BL },
    { .name = "ret", .opcode = LA16_OPCODE_RET },

    /* interupt controller operations */
    { .name = "int", .opcode = LA16_OPCODE_INT },
    { .name = "intset", .opcode = LA16_OPCODE_INTSET },
    { .name = "intret", .opcode = LA16_OPCODE_INTRET },

    /* memory page protection operations */
    { .name = "ppcnt", .opcode = LA16_OPCODE_PPCNT },
    { .name = "ppktrrset", .opcode = LA16_OPCODE_PPKTRRSET },
    { .name = "vpset", .opcode = LA16_OPCODE_VPSET },
    { .name = "vpget", .opcode = LA16_OPCODE_VPGET },
    { .name = "vpflgset", .opcode = LA16_OPCODE_VPFLGSET },
    { .name = "vpflgget", .opcode = LA16_OPCODE_VPFLGGET },
    { .name = "vpaddr", .opcode = LA16_OPCODE_VPADDR },

    /* core concurrency operations */
    { .name = "crresume", .opcode = LA16_OPCODE_CRRESUME },
    { .name = "crstop", .opcode = LA16_OPCODE_CRSTOP },
    { .name = "crdump", .opcode = LA16_OPCODE_CRDUMP },
    { .name = "crflash", .opcode = LA16_OPCODE_CRFLASH },
    { .name = "crtimeset", .opcode = LA16_OPCODE_CRTIMESET },
    { .name = "crctxhndlset", .opcode = LA16_OPCODE_CRCTXHNDLSET },
    { .name = "crexchndlset", .opcode = LA16_OPCODE_CREXCHNDLSET },
};

unsigned char opcode_from_string(const char *name)
{
    /* null pointer check */
    if(name == NULL)
    {
        return 0xFF;
    }

    /* iterating through table */
    for(unsigned char opcode = 0x00; opcode < (LA16_OPCODE_MAX - 1); opcode++)
    {
        /* check if opcode name matches */
        if(strcmp(opcode_table[opcode].name, name) == 0)
        {
            return opcode_table[opcode].opcode;
        }
    }

    /* shouldnt happen if code is correct */
    return 0xFF;
}