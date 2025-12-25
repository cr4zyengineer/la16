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

#include <la16/instruction/execution.h>
#include <la16/instruction/data.h>

void la16_op_jmp(la16_core_t core)
{
    *(core->pc) = *(core->op.param[0]) - 4;
}

void la16_op_cmp(la16_core_t core)
{
    signed short a = (signed short)*(core->op.param[0]);
    signed short b = (signed short)*(core->op.param[1]);
    
    *(core->cf) = (a == b) * LA16_CMP_Z | (a <  b) * LA16_CMP_L | (a >  b) * LA16_CMP_G;
}

void la16_op_je(la16_core_t core)
{
    if(*(core->cf) & LA16_CMP_Z)
    {
        la16_op_jmp(core);
    }
}

void la16_op_jne(la16_core_t core)
{
    if(!(*(core->cf) & LA16_CMP_Z))
    {
        la16_op_jmp(core);
    }
}

void la16_op_jlt(la16_core_t core)
{
    if(*(core->cf) & LA16_CMP_L)
    {
        la16_op_jmp(core);
    }
}

void la16_op_jgt(la16_core_t core)
{
    if(*(core->cf) & LA16_CMP_G)
    {
        la16_op_jmp(core);
    }
}

void la16_op_jle(la16_core_t core)
{
    if(*(core->cf) & LA16_CMP_L  || *(core->cf) & LA16_CMP_Z)
    {
        la16_op_jmp(core);
    }
}

void la16_op_jge(la16_core_t core)
{
    if(*(core->cf) & LA16_CMP_G || *(core->cf) & LA16_CMP_Z)
    {
        la16_op_jmp(core);
    }
}

void la16_op_bl(la16_core_t core)
{
    la16_op_push_ext(core, *core->rl[LA16_REGISTER_PC]);
    la16_op_push_ext(core, *core->rl[LA16_REGISTER_CF]);
    la16_op_push_ext(core, *core->rl[LA16_REGISTER_R0]);
    la16_op_push_ext(core, *core->rl[LA16_REGISTER_R1]);
    la16_op_push_ext(core, *core->rl[LA16_REGISTER_R2]);
    la16_op_push_ext(core, *core->rl[LA16_REGISTER_R3]);
    la16_op_push_ext(core, *core->rl[LA16_REGISTER_R4]);
    la16_op_push_ext(core, *core->rl[LA16_REGISTER_R5]);
    la16_op_push_ext(core, *core->rl[LA16_REGISTER_R6]);
    la16_op_push_ext(core, *core->rl[LA16_REGISTER_R7]);
    la16_op_push_ext(core, *core->rl[LA16_REGISTER_R8]);
    la16_op_push_ext(core, *core->rl[LA16_REGISTER_FP]);
    *(core->fp) = *(core->sp);
    *(core->pc) = *(core->op.param[0]) - 4;
}

void la16_op_ret(la16_core_t core)
{
    *(core->sp) = *(core->fp);
    la16_op_pop_ext(core, core->rl[LA16_REGISTER_FP]);
    la16_op_pop_ext(core, core->rl[LA16_REGISTER_R8]);
    la16_op_pop_ext(core, core->rl[LA16_REGISTER_R7]);
    la16_op_pop_ext(core, core->rl[LA16_REGISTER_R6]);
    la16_op_pop_ext(core, core->rl[LA16_REGISTER_R5]);
    la16_op_pop_ext(core, core->rl[LA16_REGISTER_R4]);
    la16_op_pop_ext(core, core->rl[LA16_REGISTER_R3]);
    la16_op_pop_ext(core, core->rl[LA16_REGISTER_R2]);
    la16_op_pop_ext(core, core->rl[LA16_REGISTER_R1]);
    la16_op_pop_ext(core, core->rl[LA16_REGISTER_R0]);
    la16_op_pop_ext(core, core->rl[LA16_REGISTER_CF]);
    la16_op_pop_ext(core, core->rl[LA16_REGISTER_PC]);
}
