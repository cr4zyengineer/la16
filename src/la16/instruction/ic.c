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

#include <la16/instruction/ic.h>
#include <la16/instruction/data.h>
#include <la16/instruction/mpp.h>
#include <la16/instruction/execution.h>
#include <la16/machine.h>

void la16_op_int(la16_core_t core)
{
    /* getting physical address of interruption handler */
    unsigned short ih_paddr = core->machine->int_handler[*(core->op.param[0])];

    /* checking if interruption handler is set */
    if(ih_paddr == 0x0)
    {
        core->term = LA16_TERM_FLAG_BAD_ACCESS;
        return;
    }

    /* setting elevation backup  */
    *(core->elb) = *(core->el);

    /* crafting stack pointer backup */
    unsigned short sp_backup = *(core->sp);

    /* checking if we run in user level */
    if(*(core->el) == LA16_CORE_MODE_EL0)
    {
        /* fixing up stack for interrupt */
        if(!la16_mpp_access(core, core->sp, LA16_PAGEU_FLAG_MAPPED, 2))
        {
            core->term = LA16_TERM_FLAG_BAD_ACCESS;
            return;
        }
    }

    /* switching to kernel elevation level */
    *(core->el) = LA16_CORE_MODE_EL1;

    /* pushing stack pointer backup */
    la16_op_push_ext(core, sp_backup);

    /*
     * setting parameter a to interruption handler and
     *invoking branch link to it
     */
    core->op.param[0] = &ih_paddr;
    la16_op_bl(core);
}

void la16_op_intset(la16_core_t core)
{
    /* checking if we run in user land */
    if(*(core->el) == LA16_CORE_MODE_EL0)
    {
        core->term = LA16_TERM_FLAG_PERMISSION;
        return;
    }

    /* setting interruption handler, to clear it use 0x0 */
    core->machine->int_handler[*(core->op.param[0])] = *(core->op.param[1]);
}

void la16_op_intret(la16_core_t core)
{
    /* invoking return */
    la16_op_ret(core);

    /* restoring old stack pointer */
    la16_op_pop_ext(core, core->rl[LA16_REGISTER_SP]);

    /* setting elevation back to before */
    *(core->el) = *(core->elb);
}
