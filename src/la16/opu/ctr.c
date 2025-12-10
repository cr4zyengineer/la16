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

#include <la16/opu/ctr.h>
#include <la16/memory.h>
#include <la16/machine.h>

/*void op_cmp(la16_core_t core)
{
    if(*(core->pa) > *(core->pb))
        *(core->cf) = LA16_CMP_IG;
    else if(*(core->pa) < *(core->pb))
        *(core->cf) = LA16_CMP_IL;
    else
        *(core->cf) = LA16_CMP_EQ;
}

void op_jeq(la16_core_t core)
{
    if(*(core->cf) == LA16_CMP_EQ)
        op_jmp(core);
}

void op_jil(la16_core_t core)
{
    if(*(core->cf) == LA16_CMP_IL)
        op_jmp(core);
}

void op_jig(la16_core_t core)
{
    if(*(core->cf) == LA16_CMP_IG)
        op_jmp(core);
}

void op_jmp(la16_core_t core)
{
    *(core->pc) = *(core->pa) - 4;
}

void op_pop(la16_core_t core)
{
    *(core->sp) += 2;

    // Gather cores machine back pointer
    la16_machine_t *machine = core->machine;

    // Get memory pointer
    unsigned short *ptr = (unsigned short*)&machine->ram->memory[*(core->sp)];
    *(core->pa) = *ptr;
}

void op_psh(la16_core_t core)
{
    // Gather cores machine back pointer
    la16_machine_t *machine = core->machine;

    // Get memory pointer
    unsigned short *ptr = (unsigned short*)&machine->ram->memory[*(core->sp)];
    *ptr = *(core->pa);

    *(core->sp) -= 2;
}

void op_call_pop(la16_core_t core,
                 unsigned long rid)
{
    unsigned short *pa = core->pa;
    core->pa = core->rl[rid];
    op_pop(core);
    core->pa = pa;
}

void op_call_psh(la16_core_t core,
                 unsigned long rid)
{
    unsigned short *pa = core->pa;
    core->pa = core->rl[rid];
    op_psh(core);
    core->pa = pa;
}

void op_bra(la16_core_t core)
{

    // Pushing none sensitive registers onto stack
    op_call_psh(core, LA16_REGISTER_PC);
    op_call_psh(core, LA16_REGISTER_RA);
    op_call_psh(core, LA16_REGISTER_RB);
    op_call_psh(core, LA16_REGISTER_RC);
    op_call_psh(core, LA16_REGISTER_RD);
    op_call_psh(core, LA16_REGISTER_RE);
    op_call_psh(core, LA16_REGISTER_RF);
    op_call_psh(core, LA16_REGISTER_EA);
    op_call_psh(core, LA16_REGISTER_EB);

    // Pushing frame pointer onto stack so calls can be recursive
    op_call_psh(core, LA16_REGISTER_FP);

    // Setting frame pointer to current stack pointer
    *(core->fp) = *(core->sp);

    // Setting program counter to the address - 4
    *(core->pc) = *(core->pa) - 4;
}

void op_beq(la16_core_t core)
{
    if(*(core->cf) == LA16_CMP_EQ)
        op_bra(core);
}

void op_bil(la16_core_t core)
{
    if(*(core->cf) == LA16_CMP_IL)
        op_bra(core);
}

void op_big(la16_core_t core)
{
    if(*(core->cf) == LA16_CMP_IG)
        op_bra(core);
}

void op_ret(la16_core_t core)
{
    // Setting stack pointer to current frame pointer
    *(core->sp) = *(core->fp);

    // Popping old frame pointer to current frame pointer
    op_call_pop(core, LA16_REGISTER_FP);

    // Popping none sensitive registers from stack
    op_call_pop(core, LA16_REGISTER_EB);
    op_call_pop(core, LA16_REGISTER_EA);
    op_call_pop(core, LA16_REGISTER_RF);
    op_call_pop(core, LA16_REGISTER_RE);
    op_call_pop(core, LA16_REGISTER_RD);
    op_call_pop(core, LA16_REGISTER_RC);
    op_call_pop(core, LA16_REGISTER_RB);
    op_call_pop(core, LA16_REGISTER_RA);
    op_call_pop(core, LA16_REGISTER_PC);
}

void op_jnq(la16_core_t core)
{
    if(*(core->cf) != LA16_CMP_EQ)
        *(core->pc) = *(core->pa) - 4;
}

void op_bnq(la16_core_t core)
{
    if(*(core->cf) != LA16_CMP_EQ)
        op_bra(core);
}

void op_reteq(la16_core_t core)
{
    if(*(core->cf) == LA16_CMP_EQ)
        op_ret(core);
}

void op_retnq(la16_core_t core)
{
    if(*(core->cf) != LA16_CMP_EQ)
        op_ret(core);
}

void op_retil(la16_core_t core)
{
    if(*(core->cf) == LA16_CMP_IL)
        op_ret(core);
}
void op_retig(la16_core_t core)
{
    if(*(core->cf) == LA16_CMP_IG)
        op_ret(core);
}*/
