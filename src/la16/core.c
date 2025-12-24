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

la16_opfunc_t opfunc_table[LA16_OPCODE_MAX + 1] = {
    /* core operations */
    la16_op_hlt,
    la16_op_nop,

    /* io operations */
    la16_op_in,
    la16_op_out,

    /* memory operations */
    la16_op_ldb,
    la16_op_stb,
    la16_op_ldw,
    la16_op_stw,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    /* data operations */
    la16_op_mov,
    la16_op_swp,
    la16_op_swpz,
    la16_op_push,
    la16_op_pop,

    /* arithmetic operations */
    la16_op_add,
    la16_op_sub,
    la16_op_mul,
    la16_op_div,
    la16_op_idiv,
    la16_op_inc,
    la16_op_dec,
    la16_op_not,
    la16_op_and,
    la16_op_or,
    la16_op_xor,
    la16_op_shr,
    la16_op_shl,
    la16_op_ror,
    la16_op_rol,

    /* control flow operations */
    la16_op_jmp,
    la16_op_cmp,
    la16_op_je,
    la16_op_jne,
    la16_op_jlt,
    la16_op_jgt,
    la16_op_jle,
    la16_op_jge,
    la16_op_bl,
    la16_op_ret,

    /* interupt controller operations */
    la16_op_int,
    la16_op_intset,
    la16_op_intret,

    /* memory protection operations */
    la16_op_ppcnt,
    la16_op_ppktrrset,
    la16_op_vpset,
    la16_op_vpget,
    la16_op_vpflgset,
    la16_op_vpflgget,
    la16_op_vpaddr,

    /* core concurrency */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

la16_core_t la16_core_alloc()
{
    // Allocate new core
    la16_core_t core = calloc(1, sizeof(struct la16_core));

    // Allocate all 8 registers
    for(unsigned char i = 0b0000; i < (LA16_REGISTER_EL1_MAX + 1); i++)
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

static void la16_core_decode_instruction_at_pc(la16_core_t core)
{
    /* reset operation structure */
    memset(&(core->op), 0, sizeof(la16_operation_t));

    /* preparing 4 byte buffer for the entire instruction */
    unsigned char instruction[4] = {};

    /* preparing real address for memory */
    unsigned short pc_real_addr = *(core->pc);

    /* using la16 memory page protection access */
    if(!la16_mpp_access(core, &pc_real_addr, LA16_PAGEU_FLAG_EXEC))
    {
        /* setting operation to halt */
        core->op.op = LA16_OPCODE_HLT;

        /* setting termination flag to bad access */
        core->term = LA16_TERM_FLAG_BAD_ACCESS;
        return;
    }

    /* copying instruction from memory into instruction copy */
    memcpy(instruction, &core->machine->memory->memory[pc_real_addr], 4);

    /* setting opcode according to instruction */
    core->op.op = instruction[0];

    /* extracting mode byte */
    unsigned char mdbyte = (instruction[1] & 0b11100000) >> 5;

    /* resetting intermediate */
    core->op.imm = 0;

    /* setting parameter to intermediate */
    core->op.pa = &(core->op.imm);
    core->op.pb = &(core->op.imm);

    /* handling parameter mode */
    switch(mdbyte)
    {
        case LA16_PARAMETER_CODING_COMBINATION_REG:
        {
            unsigned char reg = instruction[1] & 0b00011111;
            core->op.pa = core->rl[reg];
            goto out_res_a_check;
        }
        case LA16_PARAMETER_CODING_COMBINATION_REG_REG:
        {
            unsigned char reg[2] = {
                instruction[1] & 0b00011111,
                instruction[2] & 0b00011111
            };
            core->op.pa = core->rl[reg[0]];
            core->op.pb = core->rl[reg[1]];
            goto out_res_a_check;
        }
        case LA16_PARAMETER_CODING_COMBINATION_IMM16:
        {
            core->op.imm = ((unsigned short)instruction[3] << 8) | instruction[2];
            break;
        }
        case LA16_PARAMETER_CODING_COMBINATION_IMM16_REG:
        {
            core->op.imm = ((unsigned short)instruction[3] << 8) | instruction[2];
            unsigned char reg = instruction[1] & 0b00011111;
            core->op.pb = core->rl[reg];
            goto out_res_a_check;
        }
        case LA16_PARAMETER_CODING_COMBINATION_REG_IMM16:
        {   core->op.imm = ((unsigned short)instruction[3] << 8) | instruction[2];
            unsigned char reg = instruction[1] & 0b00011111;
            core->op.pa = core->rl[reg];
            goto out_res_a_check;
        }
        case LA16_PARAMETER_CODING_COMBINATION_IMM8_IMM8:
        {
            core->op.imm8[0] = instruction[2];
            core->op.imm8[1] = instruction[3];
            core->op.pa = &(core->op.imm8[0]);
            core->op.pb = &(core->op.imm8[1]);
            break;
        }
        default:
            break;
    }

    return;

out_res_a_check:
    // Find out what res contains
    if(core->op.reg[0] >= LA16_REGISTER_EL0_MAX ||
       core->op.reg[1] >= LA16_REGISTER_EL0_MAX)
    {
        if(*(core->el) == LA16_CORE_MODE_EL0)
        {
            core->op.op = LA16_OPCODE_HLT;
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

        if(core->op.op <= LA16_OPCODE_MAX && opfunc_table[core->op.op] != NULL)
        {
            opfunc_table[core->op.op](core);
        }
        else
        {
            printf("[exec] illegal opcode: 0x%x\n", core->op.op);
            opfunc_table[LA16_OPCODE_HLT](core);
        }

        *(core->pc) += 4;
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
