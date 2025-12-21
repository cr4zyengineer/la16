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
    /* getting specified virtual page */
    unsigned short vpage = vaddr / LA16_MEMORY_PAGE_SIZE;

    /* getting real page from cores virtual pages */
    *rpage = core->page[vpage];

    /* getting vpage uentry */
    unsigned char vpageu = core->pageu[vpage];

    /* getting offset from specified virtual page to the address */
    unsigned short off = vaddr - (vpage * LA16_MEMORY_PAGE_SIZE);

    /* getting machine pointer */
    la16_machine_t *machine = core->machine;

    /* checking if page is mapped and if the real page is within bounds */
    if(!vpageu || machine->memory->page_cnt <= *rpage)
    {
        return 0b0;
    }

    /* calculating real address via raddr */
    *raddr = machine->memory->page[*rpage].start + off;

    return 0b1;
}

unsigned char la16_mpp_access(la16_core_t core,
                              unsigned short uaddr,
                              la16_memory_prot_t prot,
                              unsigned short *raddr)
{
    /* getting machine pointer */
    la16_machine_t *machine = core->machine;

    /* checking if we are executing in kernel level */
    if(*(core->el) == LA16_CORE_MODE_EL1)
    {
        /* bounds check for phys memory access */
        if(machine->memory->memory_size < uaddr)
        {
            /* returning failure because its a out of bounds memory access*/
            return 0b0;
        }

        // its kernel so setting raddr to uaddr
        *raddr = uaddr;
    }
    else
    {
        /* its user level so we first do address resoulution */
        unsigned short rpage = 0b0;

        if(!(la16_mpp_virtual_address_resoulution(core, uaddr, &rpage, raddr) &&
            ((machine->memory->page[rpage].prot & prot) == prot)))
        {
            /* either address resoulution failed or page is not readable */
            return 0b0;
        }
    }

    return 0b1;
}

unsigned char la16_mpp_read16(la16_core_t core,
                              unsigned short uaddr,
                              unsigned short *val)
{
    /* getting machine pointer */
    la16_machine_t *machine = core->machine;

    /* accessing memory */
    la16_memory_address_t raddr = 0b0;
    if(la16_mpp_access(core, uaddr, LA16_MEMORY_PROT_READ, &raddr))
    {
        *val = *(unsigned short*)&machine->memory->memory[raddr];
        return 0b1;
    }

    /* accessing memory failed*/
    return 0b0;
}

unsigned char la16_mpp_write16(la16_core_t core,
                               unsigned short uaddr,
                               unsigned short val)
{
    /* getting machine pointer */
    la16_machine_t *machine = core->machine;

    /* accessing memory */
    la16_memory_address_t raddr = 0b0;
    if(la16_mpp_access(core, uaddr, LA16_MEMORY_PROT_WRITE, &raddr))
    {
        *(unsigned short*)&machine->memory->memory[raddr] = val;
        return 0b1;
    }

    /* accessing memory failed*/
    return 0b0;
}

void la16_op_mpagemap(la16_core_t core)
{
    /* checking if running in user level which cannot use this opcode */
    if(*(core->el) != LA16_CORE_MODE_EL1)
    {
        core->term = LA16_TERM_FLAG_PERMISSION;
        return;
    }

    /* writing blindly to the virtual page array, fix this shit */
    core->page[*(core->pa)] = *(core->pb);

    /* marking page as mapped */
    core->pageu[*(core->pa)] = 0b1;
}

void la16_op_mpageunmap(la16_core_t core)
{
    /* checking if running in user level which cannot use this opcode */
    if(*(core->el) != LA16_CORE_MODE_EL1)
    {
        core->term = LA16_TERM_FLAG_PERMISSION;
        return;
    }

    /* marking page as unmapped */
    core->pageu[*(core->pa)] = 0b0;
}

void la16_op_mpageunmapall(la16_core_t core)
{
    /* checking if running in user level which cannot use this opcode */
    if(*(core->el) != LA16_CORE_MODE_EL1)
    {
        core->term = LA16_TERM_FLAG_PERMISSION;
        return;
    }

    /* marking all pages as unmapped */
    memset(core->pageu, 0, sizeof(unsigned char) * 300);
}

void la16_op_mpageprot(la16_core_t core)
{
    /* checking if running in user level which cannot use this opcode */
    if(*(core->el) != LA16_CORE_MODE_EL1)
    {
        core->term = LA16_TERM_FLAG_PERMISSION;
        return;
    }

    /* changing page protection level, shall be on virtual page lolll */
    la16_machine_t *machine = core->machine;
    machine->memory->page[*(core)->pa].prot = *(core->pb);
}
