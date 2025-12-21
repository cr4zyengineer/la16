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
#include <la16/machine.h>

unsigned char la16_mpp_virtual_address_resoulution(la16_core_t core,
                                                   unsigned short *addr,
                                                   unsigned short *rpage,
                                                   unsigned short *vpage)
{
    /* performing null pointer check */
    if(addr == NULL || rpage == NULL || vpage == NULL)
    {
        return 0b0;
    }

    /* getting specified virtual page */
    *vpage = *addr / LA16_MEMORY_PAGE_SIZE;

    /* getting real page from cores virtual pages */
    *rpage = core->page[*vpage];

    /* getting vpage uentry */
    unsigned char vpageu = core->pageu[*vpage];

    /* getting offset from specified virtual page to the address */
    unsigned short off = *addr - (*vpage * LA16_MEMORY_PAGE_SIZE);

    /* getting machine pointer */
    la16_machine_t *machine = core->machine;

    /* checking if page is mapped and if the real page is within bounds */
    if((vpageu & LA16_PAGEU_FLAG_MAPPED) != LA16_PAGEU_FLAG_MAPPED || machine->memory->page_cnt <= *rpage)
    {
        return 0b0;
    }

    /* calculating real address via rpage */
    *addr = (*rpage * LA16_MEMORY_PAGE_SIZE) + off;

    return 0b1;
}

unsigned char la16_mpp_access(la16_core_t core,
                              unsigned short *addr,
                              unsigned char vprot)
{
    /* performing null pointer check */
    if(addr == NULL)
    {
        return 0b0;
    }

    /* checking if we are executing in kernel level */
    if(*(core->el) == LA16_CORE_MODE_EL1)
    {
        /* bounds check for phys memory access */
        if(core->machine->memory->memory_size < *addr)
        {
            /* returning failure because its a out of bounds memory access*/
            return 0b0;
        }
    }
    else
    {
        /* its user level so we first do address resoulution */
        unsigned short rpage = 0b0;
        unsigned short vpage = 0b0;

        if(!(la16_mpp_virtual_address_resoulution(core, addr, &rpage, &vpage) &&
            ((core->pageu[vpage] & vprot) == vprot)))
        {
            /* either address resoulution failed or page is not readable */
            return 0b0;
        }
    }

    return 0b1;
}

unsigned char la16_mpp_read(la16_core_t core,
                            unsigned short uaddr,
                            unsigned short *val)
{
    /* accessing memory */
    if(la16_mpp_access(core, &uaddr, LA16_PAGEU_FLAG_READ))
    {
        *val = *(unsigned short*)&core->machine->memory->memory[uaddr];
        return 0b1;
    }

    /* accessing memory failed*/
    return 0b0;
}

unsigned char la16_mpp_write(la16_core_t core,
                            unsigned short uaddr,
                            unsigned short val)
{
    /* accessing memory */
    if(la16_mpp_access(core, &uaddr, LA16_PAGEU_FLAG_WRITE))
    {
        *(unsigned short*)&core->machine->memory->memory[uaddr] = val;
        return 0b1;
    }

    /* accessing memory failed*/
    return 0b0;
}

void la16_op_mpagemap(la16_core_t core)
{
    /* checking if running in user level which cannot use this opcode */
    if(*(core->el) != LA16_CORE_MODE_EL1 || *(core->pa) > 256)
    {
        core->term = LA16_TERM_FLAG_PERMISSION;
        return;
    }

    /* writing blindly to the virtual page array, fix this shit */
    core->page[*(core->pa)] = *(core->pb);
}

void la16_op_mpageunmap(la16_core_t core)
{
    /* checking if running in user level which cannot use this opcode */
    if(*(core->el) != LA16_CORE_MODE_EL1 || *(core->pa) > 256)
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
    memset(core->pageu, 0, sizeof(unsigned char) * 256);
}

void la16_op_mpageprot(la16_core_t core)
{
    /* checking if running in user level which cannot use this opcode */
    if(*(core->el) != LA16_CORE_MODE_EL1  || *(core->pa) > core->machine->memory->page_cnt)
    {
        core->term = LA16_TERM_FLAG_PERMISSION;
        return;
    }

    /* changing page protection level, shall be on virtual page lolll */
    core->pageu[*(core)->pa] = *(core->pb);
}
