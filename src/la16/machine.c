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
#include <la16/machine.h>

la16_machine_t *la16_machine_alloc(unsigned short memory_size)
{
    // Allocate base
    la16_machine_t *machine = malloc(sizeof(la16_machine_t));

    // Allocate memory
    machine->memory = la16_memory_alloc(memory_size);

    // Now allocate the cores
    for(unsigned char i = 0; i < 4; i++)
    {
        machine->core[i] = la16_core_alloc();
        machine->core[i]->machine = machine;
    }

    return machine;
}

void la16_machine_dealloc(la16_machine_t *machine)
{
    // Deallocate cores
    for(unsigned char i = 0; i < 4; i++)
    {
        la16_core_dealloc(machine->core[i]);
    }

    // Deallocate memory
    la16_memory_dealloc(machine->memory);

    // Deallocate base
    free(machine);
}
