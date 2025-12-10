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
#include <termios.h>
#include <unistd.h>
#include <la16/instruction/data.h>
#include <la16/instruction/mpp.h>

void la16_op_push_ext(la16_core_t core, unsigned short regid)
{
    if(!la16_mpp_write16(core, *(core->sp), *(core->rl[regid])))
    {
        core->term = LA16_TERM_FLAG_BAD_ACCESS;
    }

    *(core->sp) -= 2;
}

void la16_op_pop_ext(la16_core_t core, unsigned short regid)
{
    *(core->sp) += 2;

    if(!la16_mpp_read16(core, *(core->sp), core->rl[regid]))
    {
        core->term = LA16_TERM_FLAG_BAD_ACCESS;
    }
}

void la16_op_push_ext2(la16_core_t core, unsigned short val)
{
    if(!la16_mpp_write16(core, *(core->sp), val))
    {
        core->term = LA16_TERM_FLAG_BAD_ACCESS;
    }

    *(core->sp) -= 2;
}

void la16_op_pop_ext2(la16_core_t core, unsigned short *val)
{
    *(core->sp) += 2;

    if(!la16_mpp_read16(core, *(core->sp), val))
    {
        core->term = LA16_TERM_FLAG_BAD_ACCESS;
    }
}

void la16_op_mov(la16_core_t core)
{
    *(core->pa) = *(core->pb);
}

void la16_op_movb(la16_core_t core)
{
    *(core->pa) = (unsigned char)*(core->pb);
}

void la16_op_mld(la16_core_t core)
{
    if(!la16_mpp_read16(core, *(core->pb), core->pa))
    {
        core->term = LA16_TERM_FLAG_BAD_ACCESS;
    }
}

void la16_op_mldb(la16_core_t core)
{
    if(!la16_mpp_read8(core, *(core->pb), (unsigned char*)core->pa))
    {
        core->term = LA16_TERM_FLAG_BAD_ACCESS;
    }
}

void la16_op_mst(la16_core_t core)
{
    if(!la16_mpp_write16(core, *(core->pb), *(core->pa)))
    {
        core->term = LA16_TERM_FLAG_BAD_ACCESS;
    }
}

void la16_op_mstb(la16_core_t core)
{
    if(!la16_mpp_write8(core, *(core->pb), (unsigned char)*(core->pa)))
    {
        core->term = LA16_TERM_FLAG_BAD_ACCESS;
    }
}

void la16_op_in(la16_core_t core)
{
    if(*(core->el) == LA16_CORE_MODE_EL0)
    {
        core->term = LA16_TERM_FLAG_PERMISSION;
        return;
    }

    switch(*(core->pb))
    {
        case LA16_IO_PORT_SERIAL:
        {
            struct termios oldt, newt;
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;
            newt.c_lflag &= ~(ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);
            read(STDIN_FILENO, core->pa, 1);
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            break;
        }
        default:
            break;
    }
}

void la16_op_out(la16_core_t core)
{
    if(*(core->el) == LA16_CORE_MODE_EL0)
    {
        core->term = LA16_TERM_FLAG_PERMISSION;
        return;
    }

    switch(*(core->pa))
    {
        case LA16_IO_PORT_SERIAL:
        {
            write(STDOUT_FILENO, core->pb, 1);
            break;
        }
        default:
            break;
    }
}

void la16_op_push(la16_core_t core)
{
    if(!la16_mpp_write16(core, *(core->sp), *(core->pa)))
    {
        core->term = LA16_TERM_FLAG_BAD_ACCESS;
    }

    *(core->sp) -= 2;
}

void la16_op_pop(la16_core_t core)
{
    *(core->sp) += 2;

    if(!la16_mpp_read16(core, *(core->sp), core->pa))
    {
        core->term = LA16_TERM_FLAG_BAD_ACCESS;
    }
}
