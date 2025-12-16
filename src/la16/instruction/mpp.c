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

#include <string.h>
#include <la16/instruction/mpp.h>
#include <la16/memory.h>
#include <la16/machine.h>

unsigned char la16_mpp_virtual_address_resoulution(la16_core_t core,
                                                   unsigned short vaddr,
                                                   unsigned short *rpage,
                                                   unsigned short *raddr)
{
    unsigned short vpage = vaddr / LA16_MEMORY_PAGE_SIZE;
    *rpage = core->page[vpage];
    unsigned char vpageu = core->pageu[vpage];
    unsigned short offt = vaddr - (vpage * LA16_MEMORY_PAGE_SIZE);

    la16_machine_t *machine = core->machine;

    if(!vpageu || machine->ram->page_cnt <= *rpage)
    {
        return 0b0;
    }

    *raddr = machine->ram->page[*rpage].start + offt;

    return 0b1;
}

unsigned char la16_mpp_read8(la16_core_t core,
                             unsigned short uaddr,
                             unsigned char *val)
{
    la16_machine_t *machine = core->machine;

    if(*(core->el) == LA16_CORE_MODE_EL1)
    {
        if(machine->ram->memory_size < uaddr)
        {
            return 0b0;
        }

        *val = machine->ram->memory[uaddr];
    }
    else
    {
        unsigned short rpage = 0b0;
        unsigned short raddr = 0b0;

        if(!(la16_mpp_virtual_address_resoulution(core, uaddr, &rpage, &raddr) &&
           ((machine->ram->page[rpage].prot & LA16_MEMORY_PROT_READ) == LA16_MEMORY_PROT_READ)))
        {
            printf("[memory] illegal read operation not permitted: page %d addr: 0x%x(0x%x)\n", rpage, raddr, uaddr);
            return 0b0;
        }

        *val = machine->ram->memory[raddr];
    }

    return 0b1;
}

unsigned char la16_mpp_write8(la16_core_t core,
                              unsigned short uaddr,
                              unsigned char val)
{
    la16_machine_t *machine = core->machine;

    if(*(core->el) == LA16_CORE_MODE_EL1)
    {
        if(machine->ram->memory_size < uaddr)
        {
            return 0b0;
        }

        machine->ram->memory[uaddr] = val;
    }
    else
    {
        unsigned short rpage = 0b0;
        unsigned short raddr = 0b0;

        if(!(la16_mpp_virtual_address_resoulution(core, uaddr, &rpage, &raddr) &&
            ((machine->ram->page[rpage].prot & LA16_MEMORY_PROT_WRITE) == LA16_MEMORY_PROT_WRITE)))
        {
            printf("[memory] illegal write operation not permitted: page %d addr: 0x%x(0x%x)\n", rpage, raddr, uaddr);
            return 0b0;
        }

        machine->ram->memory[raddr] = val;
    }

    return 0b1;
}

unsigned char la16_mpp_read16(la16_core_t core,
                              unsigned short uaddr,
                              unsigned short *val)
{
    la16_machine_t *machine = core->machine;

    if(*(core->el) == LA16_CORE_MODE_EL1)
    {
        if(machine->ram->memory_size < uaddr)
        {
            return 0b0;
        }

        *val = *(unsigned short*)&machine->ram->memory[uaddr];
    }
    else
    {
        unsigned short rpage = 0b0;
        unsigned short raddr = 0b0;

        if(!(la16_mpp_virtual_address_resoulution(core, uaddr, &rpage, &raddr) &&
            ((machine->ram->page[rpage].prot & LA16_MEMORY_PROT_READ) == LA16_MEMORY_PROT_READ)))
        {
            printf("[memory] illegal read operation not permitted: page %d addr: 0x%x(0x%x)\n", rpage, raddr, uaddr);
            return 0b0;
        }

        *val = *(unsigned short*)&machine->ram->memory[raddr];
    }

    return 0b1;
}

unsigned char la16_mpp_write16(la16_core_t core,
                               unsigned short uaddr,
                               unsigned short val)
{
    la16_machine_t *machine = core->machine;

    if(*(core->el) == LA16_CORE_MODE_EL1)
    {
        if(machine->ram->memory_size < uaddr)
        {
            return 0b0;
        }

        *(unsigned short*)&machine->ram->memory[uaddr] = val;
    }
    else
    {
        unsigned short rpage = 0b0;
        unsigned short raddr = 0b0;

        if(!(la16_mpp_virtual_address_resoulution(core, uaddr, &rpage, &raddr) &&
            ((machine->ram->page[rpage].prot & LA16_MEMORY_PROT_WRITE) == LA16_MEMORY_PROT_WRITE)))
        {
            printf("[memory] illegal write operation not permitted: page %d addr: 0x%x(0x%x)\n", rpage, raddr, uaddr);
            return 0b0;
        }

        *(unsigned short*)&machine->ram->memory[raddr] = val;
    }

    return 0b1;
}

void la16_op_mpagemap(la16_core_t core)
{
    if(*(core->el) != LA16_CORE_MODE_EL1)
    {
        core->term = LA16_TERM_FLAG_PERMISSION;
        return;
    }

    core->page[*(core->pa)] = *(core->pb);
    core->pageu[*(core->pa)] = 0b1;
}

void la16_op_mpageunmap(la16_core_t core)
{
    if(*(core->el) != LA16_CORE_MODE_EL1)
    {
        core->term = LA16_TERM_FLAG_PERMISSION;
        return;
    }

    core->pageu[*(core->pa)] = 0b0;
}

void la16_op_mpageunmapall(la16_core_t core)
{
    if(*(core->el) != LA16_CORE_MODE_EL1)
    {
        core->term = LA16_TERM_FLAG_PERMISSION;
        return;
    }

    memset(core->pageu, 0, sizeof(unsigned char) * 300);
}

void la16_op_mpageprot(la16_core_t core)
{
    if(*(core->el) != LA16_CORE_MODE_EL1)
    {
        core->term = LA16_TERM_FLAG_PERMISSION;
        return;
    }

    la16_machine_t *machine = core->machine;
    machine->ram->page[*(core)->pa].prot = *(core->pb);
}
