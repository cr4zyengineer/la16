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

typedef struct {
    /* physical */
    unsigned short phys_addr;           /* physical address */
    unsigned char phys_page;            /* physical page */

    /* virtual */
    unsigned short virt_addr;           /* virtual address */
    unsigned short virt_page;           /* virtual page */
    unsigned short virt_page_flags;     /* virtual page flags */
} mpp_address_t;

unsigned char la16_mpp_address(la16_core_t core,
                               mpp_address_t *maddr)
{
    /* null pointer check */
    if(maddr == NULL)
    {
        return 0b0;
    }

    /* getting virtual page by calculation with memory page size */
    maddr->virt_page = maddr->virt_addr / LA16_MEMORY_PAGE_SIZE;

    /* getting virtual page uentry */
    maddr->virt_page_flags = core->pageu[maddr->virt_page];

    /* getting physical page from cores virtual pages */
    maddr->phys_page = core->page[maddr->virt_page];

    /* getting offset from virtual page to the address */
    unsigned short off = maddr->virt_addr - (maddr->virt_page * LA16_MEMORY_PAGE_SIZE);

    /* checking if page is mapped and if the physical page is within bounds */
    if((maddr->virt_page_flags & LA16_PAGEU_FLAG_MAPPED) != LA16_PAGEU_FLAG_MAPPED ||
        core->machine->memory->page_cnt <= maddr->phys_page)
    {
        return 0b0;
    }

    /* getting physical address via physical page and the prio gathered offset */
    maddr->phys_addr = (maddr->phys_page * LA16_MEMORY_PAGE_SIZE) + off;

    return 0b1;
}

unsigned char la16_mpp_access(la16_core_t core,
                              unsigned short *addr,
                              unsigned char vprot,
                              unsigned char width)
{
    /* performing null pointer check */
    if(addr == NULL)
    {
        return 0b0;
    }

    /* wrap around check */
    if(width == 2 &&
       *addr == 0xFFFF)
    {
        return 0b0;
    }

    /* checking if we are executing in kernel level */
    if(*(core->el) == LA16_CORE_MODE_EL1)
    {
        /* bounds check for phys memory access */
        if(*addr + width > core->machine->memory->memory_size)
        {
            /* returning failure because its a out of bounds memory access*/
            return 0b0;
        }
    }
    else
    {
        /* its user level so we first do address resoulution */
        mpp_address_t maddr = {};
        maddr.virt_addr = *addr;

        /* perform address resoulution */
        if(!(la16_mpp_address(core, &maddr) &&
            ((maddr.virt_page_flags & vprot) == vprot)))
        {
            /* either address resoulution failed or page is not readable */
            return 0b0;
        }

        /* for 16-bit access, verify if second byte is also in a valid page with same permissions */
        if(width == 2)
        {
            unsigned short addr2 = maddr.virt_addr + 1;
            unsigned short vpage2 = addr2 / LA16_MEMORY_PAGE_SIZE;
            
            /* if crossing page boundary, check second page permissions */
            if(vpage2 != maddr.virt_page)
            {
                if((core->pageu[vpage2] & vprot) != vprot)
                {
                    return 0b0;
                }
            }
        }
    }

    return 0b1;
}

unsigned char la16_mpp_read(la16_core_t core,
                            unsigned short uaddr,
                            unsigned short *val)
{
    /* accessing memory */
    if(la16_mpp_access(core, &uaddr, LA16_PAGEU_FLAG_READ, 2))
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
    if(la16_mpp_access(core, &uaddr, LA16_PAGEU_FLAG_WRITE, 2))
    {
        *(unsigned short*)&core->machine->memory->memory[uaddr] = val;
        return 0b1;
    }

    /* accessing memory failed*/
    return 0b0;
}

unsigned char la16_mpp_read8(la16_core_t core, unsigned short uaddr, unsigned char *val)
{
    /* accessing memory */
    if(la16_mpp_access(core, &uaddr, LA16_PAGEU_FLAG_READ, 1))
    {
        *val = *(unsigned char*)&core->machine->memory->memory[uaddr];
        return 0b1;
    }

    /* accessing memory failed*/
    return 0b0;
}

unsigned char la16_mpp_write8(la16_core_t core, unsigned short uaddr, unsigned char val)
{
    /* accessing memory */
    if(la16_mpp_access(core, &uaddr, LA16_PAGEU_FLAG_WRITE, 1))
    {
        *(unsigned char*)&core->machine->memory->memory[uaddr] = val;
        return 0b1;
    }

    /* accessing memory failed*/
    return 0b0;
}

void la16_op_ppcnt(la16_core_t core)
{
    /* checking if running in user level which cannot use this opcode */
    if(*(core->el) != LA16_CORE_MODE_EL1)
    {
        core->term = LA16_TERM_FLAG_PERMISSION;
        return;
    }

    /* giving number of pages */
    *(core->op.param[0]) = core->machine->memory->page_cnt;
}

void la16_op_ppktrrset(la16_core_t core)
{
    /* checking if running in user level which cannot use this opcode */
    if(*(core->el) != LA16_CORE_MODE_EL1)
    {
        core->term = LA16_TERM_FLAG_PERMISSION;
        return;
    }

    // NOTE: Implement ktrr
}

void la16_op_vpset(la16_core_t core)
{
    /* checking if running in user level which cannot use this opcode */
    if(*(core->el) != LA16_CORE_MODE_EL1 || *(core->op.param[0]) > 256)
    {
        core->term = LA16_TERM_FLAG_PERMISSION;
        return;
    }

    /* giving number of pages */
    core->page[*(core->op.param[0])] = *(core->op.param[1]);
}

void la16_op_vpget(la16_core_t core)
{
    /* checking if running in user level which cannot use this opcode */
    if(*(core->el) != LA16_CORE_MODE_EL1 || *(core->op.param[1]) > 256)
    {
        core->term = LA16_TERM_FLAG_PERMISSION;
        return;
    }

    /* giving number of pages */
    *(core->op.param[0]) = core->page[*(core->op.param[1])];
}

void la16_op_vpflgset(la16_core_t core)
{
    /* checking if running in user level which cannot use this opcode */
    if(*(core->el) != LA16_CORE_MODE_EL1 || *(core->op.param[0]) > 256)
    {
        core->term = LA16_TERM_FLAG_PERMISSION;
        return;
    }

    /* setting virtual page flags */
    core->pageu[*(core->op.param[0])] = *(core->op.param[1]);
}

void la16_op_vpflgget(la16_core_t core)
{
    /* checking if running in user level which cannot use this opcode */
    if(*(core->el) != LA16_CORE_MODE_EL1 || *(core->op.param[1]) > 256)
    {
        core->term = LA16_TERM_FLAG_PERMISSION;
        return;
    }

    /* getting virtual page flags */
    *(core->op.param[0]) = core->pageu[*(core->op.param[1])];
}

void la16_op_vpaddr(la16_core_t core)
{
    /* checking if running in user level which cannot use this opcode */
    if(*(core->el) != LA16_CORE_MODE_EL1)
    {
        core->term = LA16_TERM_FLAG_PERMISSION;
        return;
    }

    /* getting real address of virtual address */
    mpp_address_t maddr = {};
    maddr.virt_addr = *core->op.param[0];
    la16_mpp_address(core, &maddr);
    *core->op.param[0] = maddr.phys_addr;
}