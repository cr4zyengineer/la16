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

#include <stdio.h>
#include <stdlib.h>
#include <la16/opu/int.h>
#include <la16/opu/ctr.h>

/*unsigned short intHandler[0xFFFF];

extern void op_call_pop(la16_core_t core, unsigned long rid);
extern void op_call_psh(la16_core_t core, unsigned long rid);

void op_int(la16_core_t core)
{
    // Finding out address
    unsigned short handler = intHandler[*(core->pa)];
    if(handler == 0x00)
    {
        core->term = 0b00000001;
    }
    core->pa = &handler;

    op_bra(core);

    *(core->el) = LA16_CORE_MODE_EL1;
}

void op_intst(la16_core_t core)
{
    if(*(core->el) < LA16_CORE_MODE_EL1)
    {
        core->term = 0b00000001;
        return;
    }

    intHandler[*(core->pa)] = *(core->pb);
}

void op_intcl(la16_core_t core)
{
    if(*(core->el) < LA16_CORE_MODE_EL1)
    {
        core->term = 0b00000001;
        return;
    }

    intHandler[*(core->pa)] = 0x00;
}
void op_iret(la16_core_t core)
{
    if(*(core->el) < LA16_CORE_MODE_EL1)
    {
        core->term = 0b00000001;
        return;
    }

    *(core->el) = LA16_CORE_MODE_EL0;

    op_ret(core);
}*/
