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

#include <la16/opu/alu.h>
#include <la16/memory.h>
#include <la16/machine.h>

void op_mov(la16_core_t core)
{
    *(core->pa) = *(core->pb);
    return;
}

void op_mov8(la16_core_t core)
{
    *(unsigned char*)core->pa = *(unsigned char*)core->pb;
    return;
}

void op_add(la16_core_t core)
{
    *(core->pa) += *(core->pb);
    return;
}

void op_sub(la16_core_t core)
{
    *(core->pa) -= *(core->pb);
    return;
}

void op_mul(la16_core_t core)
{
    *(core->pa) *= *(core->pb);
    return;
}

void op_div(la16_core_t core)
{
    *(core->pa) /= *(core->pb);
    return;
}

void op_inc(la16_core_t core)
{
    (*(core->pa))++;
    return;
}

void op_dec(la16_core_t core)
{
    (*(core->pa))--;
    return;
}

void op_and(la16_core_t core)
{
    *(core->pa) &= *(core->pb);
    return;
}

void op_not(la16_core_t core)
{
    *(core->pa) = !*(core->pa);
    return;
}

void op_bsl(la16_core_t core)
{
    *(core->pa) = *(core->pa) << *(core->pb);
    return;
}

void op_bsr(la16_core_t core)
{
    (*core->pa) =  *(core->pa) >> *(core->pb);
    return;
}

void op_mst(la16_core_t core)
{
    // Gather cores machine back pointer
    la16_machine_t *machine = core->machine;

    // Get memory pointer
    unsigned short *ptr = (unsigned short*)&machine->ram->memory[*(core->pb)];
    *ptr = *(core->pa);
}

void op_mst8(la16_core_t core)
{
    // Gather cores machine back pointer
    la16_machine_t *machine = core->machine;

    // Get memory pointer
    machine->ram->memory[*(core->pb)] = (unsigned char)*(core->pa);
}

void op_mld(la16_core_t core)
{
    // Gather cores machine back pointer
    la16_machine_t *machine = core->machine;

    // Get memory pointer
    unsigned short *ptr = (unsigned short*)&machine->ram->memory[*(core->pb)];
    *(core->pa) = *ptr;
}

void op_mld8(la16_core_t core)
{
    // Gather cores machine back pointer
    la16_machine_t *machine = core->machine;

    // Get memory pointer
    *(core->pa) = 0x0;
    *(core->pa) = machine->ram->memory[*(core->pb)];
}

void op_swp(la16_core_t core)
{
    unsigned short a = *(core->pa);
    *(core->pa) = *(core->pb);
    *(core->pb) = a;
}

void op_xor(la16_core_t core)
{
    *(core->pa) = *(core->pa) ^ *(core->pb);
}

void op_or(la16_core_t core)
{
    *(core->pa) = *(core->pa) | *(core->pb);
}
