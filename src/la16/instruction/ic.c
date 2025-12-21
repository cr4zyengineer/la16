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
    unsigned short eaddr = core->machine->int_handler[*(core->pa)];
    if(eaddr == 0x0)
    {
        core->term = LA16_TERM_FLAG_PERMISSION;
        return;
    }

    *(core->elb) = *(core->el);
    unsigned short sp_backup = *(core->sp);

    if(*(core->el) == LA16_CORE_MODE_EL0)
    {
        // Fixup stack (is now at a other address)
        if(!la16_mpp_access(core, core->sp, LA16_PAGEU_FLAG_MAPPED))
        {
            core->term = LA16_TERM_FLAG_PERMISSION;
            return;
        }
    }

    *(core->el) = LA16_CORE_MODE_EL1;

    la16_op_push_ext2(core, sp_backup);

    core->pa = &eaddr;
    la16_op_bl(core);
}

void la16_op_intset(la16_core_t core)
{
    if(*(core->el) == LA16_CORE_MODE_EL0)
    {
        core->term = LA16_TERM_FLAG_PERMISSION;
        return;
    }

    core->machine->int_handler[*(core->pa)] = *(core->pb);
}

void la16_op_intret(la16_core_t core)
{
    la16_op_ret(core);

    la16_op_pop_ext(core, LA16_REGISTER_SP);

    *(core->el) = *(core->elb);
}
