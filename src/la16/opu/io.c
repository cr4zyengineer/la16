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

#include <la16/opu/io.h>
#include <la16/opu/int.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>

void op_ior(la16_core_t core)
{
    if(*(core->el) < LA16_CORE_MODE_EL1)
    {
        printf("[IOR] blocked attempt to read from userspace\n");
        core->term = 0b00000001;
        return;
    }

    switch(*(core->pa))
    {
        case LA16_IO_HWR_SERIAL:
        {
            struct termios oldt, newt;
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;
            newt.c_lflag &= ~(ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);
            read(STDIN_FILENO, core->pb, 1);
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            break;
        }
        default:
            break;
    }
}

void op_iow(la16_core_t core)
{
    if(*(core->el) < LA16_CORE_MODE_EL1)
    {
        printf("[IOW] blocked attempt to write from userspace\n");
        core->term = 0b00000001;
        return;
    }

    switch(*(core->pa))
    {
        case LA16_IO_HWR_SERIAL:
            write(STDOUT_FILENO, core->pb, 1);
            break;
        default:
            break;
    }
}

void op_ioc(la16_core_t core)
{
    // Remove
}
