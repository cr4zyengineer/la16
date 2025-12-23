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

#include <stdlib.h>
#include <la16/core.h>
#include <la16/register.h>
#include <la16/memory.h>
#include <la16/machine.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>

#include <la16/instruction/core.h>
#include <la16/instruction/data.h>
#include <la16/instruction/mpp.h>
#include <la16/instruction/arithmetic.h>
#include <la16/instruction/execution.h>
#include <la16/instruction/ic.h>

void op_hlt(la16_core_t core)
{
    core->term = 0b00000001;
    return;
}

la16_opfunc_t opfunc_table[LA16_OPCODE_MAX] = {
    la16_op_hlt,    /* 0b0000000 */
    la16_op_nop,    /* 0b0000001 */

    la16_op_in,     /* 0b0000010 */
    la16_op_out,    /* 0b0000011 */

    NULL,           /* 0b0000100 */
    NULL,           /* 0b0000101 */
    la16_op_ldw,    /* 0b0000110 */
    la16_op_stw,    /* 0b0000111 */
    NULL,           /* 0b0001000 */
    NULL,           /* 0b0001001 */
    NULL,           /* 0b0001010 */
    NULL,           /* 0b0001011 */
    NULL,           /* 0b0001100 */

    la16_op_mov,    /* 0b0001101 */
    la16_op_swp,    /* 0b0001110 */
    la16_op_swpz,   /* 0b0001111 */
    la16_op_push,   /* 0b0010000 */
    la16_op_pop,    /* 0b0010001 */

    la16_op_add,    /* 0b0010010 */
    la16_op_sub,
    la16_op_mul,
    la16_op_div,
    la16_op_idiv,
    la16_op_inc,
    la16_op_dec,
    la16_op_not,
    NULL,
    la16_op_and,
    la16_op_or,
    la16_op_xor,
    la16_op_shr,
    la16_op_shl,
    la16_op_ror,
    la16_op_rol,

    la16_op_jmp,
    la16_op_cmp,
    la16_op_je,
    la16_op_jne,
    la16_op_jlt,
    la16_op_jgt,
    la16_op_bl,
    la16_op_ret,

    la16_op_int,
    la16_op_intset,
    la16_op_intret,

    NULL,
    NULL,

    NULL,
    NULL,
    NULL,
    NULL
};

la16_core_t la16_core_alloc()
{
    // Allocate new core
    la16_core_t core = calloc(1, sizeof(struct la16_core));

    // Allocate all 8 registers
    for(unsigned char i = 0b0000; i < LA16_REGISTER_EL1_MAX; i++)
    {
        core->rl[i] = la16_register_alloc();
    }

    // The first 3 of em are special purpose register
    core->pc = core->rl[LA16_REGISTER_PC];
    core->sp = core->rl[LA16_REGISTER_SP];
    core->fp = core->rl[LA16_REGISTER_FP];
    core->cf = core->rl[LA16_REGISTER_CF];
    core->el = core->rl[LA16_REGISTER_EL];
    core->elb = core->rl[LA16_REGISTER_ELB];

    // A core always starts in EL1
    *(core->el) = LA16_CORE_MODE_EL1;

    return core;
}

void la16_core_dealloc(la16_core_t core)
{
    // Free
    for(unsigned char i = 0b00000000; i < 0b00001100; i++)
    {
        la16_register_dealloc(core->rl[i]);
    }

    free(core);
}

/*static void la16_core_decode_helper_get_resources(unsigned char *opptr,
                                                  unsigned char resflag,
                                                  la16_decoder_resources_t *res)
{
    *((unsigned short*)res->imm4) = 0;
    *((unsigned short*)res->imm8) = 0;
    res->imm16 = 0;

    switch (resflag)
    {
        case LA16_PTRES_COMBO_4B:
        {
            // register in low nibble of operand byte
            res->imm4[0] = opptr[0] & 0x0F;
            break;
        }
        case LA16_PTRES_COMBO_16B:
        {
            // full 16-bit address/immediate starting at operand[1]
            res->imm16 = ((unsigned short)opptr[2] << 8) | opptr[1];
            break;
        }
        case LA16_PTRES_COMBO_4B_4B:
        {
            res->imm4[0] = opptr[0] & 0x0F;
            res->imm4[1] = opptr[1] & 0x0F;
            break;
        }
        case LA16_PTRES_COMBO_4B_16B:
        {
            res->imm4[0] = opptr[0] & 0x0F;
            res->imm16 = ((unsigned short)opptr[2] << 8) | opptr[1];
            break;
        }
        case LA16_PTRES_COMBO_4B_8B:
        {
            res->imm4[0] = opptr[0] & 0x0F;
            unsigned short raw_c = ((unsigned short)opptr[2] << 8) | opptr[1];
            res->imm8[0] = (unsigned char)(raw_c & 0xFF);
            break;
        }
        case LA16_PTRES_COMBO_8B_8B:
        {
            res->imm8[0] = opptr[1];
            res->imm8[1] = opptr[2];
            break;
        }
        case LA16_PTRES_COMBO_8B:
        {
            res->imm8[0] = opptr[1];
            break;
        }
        default:
            break;
    }
}*/

extern unsigned char la16_compiler_pc_inc_for_mode(unsigned char mode);

static void la16_core_decode_instruction_at_pc(la16_core_t core)
{
    /* preparing 4 byte buffer for the entire instruction */
    unsigned char instruction[4] = {};

    /* preparing real address for memory */
    unsigned short pc_real_addr = *(core->pc);

    /* using la16 memory page protection access */
    if(!la16_mpp_access(core, &pc_real_addr, LA16_PAGEU_FLAG_EXEC))
    {
        /* setting operation to halt */
        core->op = LA16_OPCODE_HLT;

        /* setting termination flag to bad access */
        core->term = LA16_TERM_FLAG_BAD_ACCESS;
        return;
    }

    /* copying instruction from memory into instruction copy */
    memcpy(instruction, &core->machine->memory->memory[pc_real_addr], 4);

    /* setting opcode according to instruction */
    //core->op = instruction[0];

    /* preparing layout */
    la16_coder_instruction_layout_t instruction_layout;

    /* decoding opcode */
    instruction_layout.opcode = (instruction[0] >> 1) & 0b11111110;
    core->op = instruction_layout.opcode;

    /* decoding mode */
    instruction_layout.mode |= (instruction[0] & 0b00000001) << 2;
    instruction_layout.mode |= (instruction[1] & 0b11000000) >> 6;

    /* getting size of layout */
    core->pc_inc = la16_compiler_pc_inc_for_mode(instruction_layout.mode);

    /* resetting intermediate */
    core->imm16 = 0;

    /* setting parameter to intermediate */
    core->pa = &(core->imm16);
    core->pb = &(core->imm16);

    /* creating new resources on stack memory */
    la16_decoder_resources_t res = {};

    /* handling parameter mode */
    switch(instruction_layout.mode)
    {
        case LA16_CODING_COMBINATION_REG:
        {
            core->pa = core->rl[(instruction[1] & 0b00111111)];
            goto out_res_a_check;
        }
        case LA16_CODING_COMBINATION_IMM6:
        {
            core->imm6[0] = (instruction[1] & 0b00111111);
            core->pa = &core->imm6[0];
            break;
        }
        case LA16_CODING_COMBINATION_REG_REG:
        {
            core->pa = core->rl[(instruction[1] & 0b00111111)];
            core->pb = core->rl[(instruction[2] & 0b11111100 << 1)];
            goto out_res_a_check;
        }
        case LA16_CODING_COMBINATION_REG_IMM16:
        {
            core->pa = core->rl[(instruction[1] & 0b00111111)];
            core->imm16 = *((unsigned short*)&instruction[2]);
            goto out_res_a_check;
        }
        case LA16_CODING_COMBINATION_IMM16_REG:
        {
            core->pb = core->rl[(instruction[1] & 0b00111111)];
            core->imm16 = *((unsigned short*)&instruction[2]);
            goto out_res_a_check;
        }
        case LA16_CODING_COMBINATION_IMM16:
        {
            core->pa = core->rl[(instruction[1] & 0b00111111)];
            break;
        }
        case LA16_CODING_COMBINATION_IMM11_IMM11:
        {
            /* fuck my actual life*/
            break;
        }
        default:
            break;
    }

    return;

out_res_a_check:
    // Find out what res contains
    if(res.imm6[0] >= LA16_REGISTER_EL0_MAX ||
       res.imm6[1] >= LA16_REGISTER_EL0_MAX)
    {
        if(*(core->el) == LA16_CORE_MODE_EL0)
        {
            core->op = LA16_OPCODE_HLT;
            core->term = LA16_TERM_FLAG_PERMISSION;
        }
    }
}

static void *la16_core_execute_thread(void *arg)
{
    // Now execute fr
    la16_core_t core = arg;

    // Set runs flag
    core->runs = 0b00000001;
    core->term = 0b00000000;

    while(1)
    {
        switch(core->term)
        {
            case LA16_TERM_FLAG_NONE:
                break;
            case LA16_TERM_FLAG_HALT:
                printf("[exec] halted at 0x%x\n", *(core->pc));
                core->runs = 0b00000000;
                return NULL;
            case LA16_TERM_FLAG_BAD_ACCESS:
                printf("[exec] bad access at 0x%x\n", *(core->pc));
                core->runs = 0b00000000;
                return NULL;
            case LA16_TERM_FLAG_PERMISSION:
                printf("[exec] permission denied at 0x%x\n", *(core->pc));
                core->runs = 0b00000000;
                return NULL;
            default:
                printf("[exec] unknown exception at 0x%x\n", *(core->pc));
                break;
        }

        la16_core_decode_instruction_at_pc(core);

        if(core->op < LA16_OPCODE_MAX)
        {
            printf("%d\n", core->op);
            opfunc_table[core->op](core);
        }
        else
        {
            printf("[exec] illegal opcode: 0x%x\n", core->op);
            opfunc_table[LA16_OPCODE_HLT](core);
        }

        *(core->pc) += core->pc_inc;
    }

    core->runs = 0b00000000;
    return NULL;
}


void la16_core_execute(la16_core_t core)
{
    // Check if core already runs
    if(core->runs)
    {
        return;
    }

    // Creating new pthread and joining it
    pthread_t pthread;
    pthread_create(&pthread, NULL, la16_core_execute_thread, (void*)core);
    pthread_join(pthread, NULL);
}

void la16_core_terminate(la16_core_t core)
{
    // Terminates the core
    core->term = 0b00000001;
}
