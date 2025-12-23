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

#ifndef LA16_INSTRUCTION_MPP_H
#define LA16_INSTRUCTION_MPP_H

#include <la16/core.h>
#include <la16/memory.h>

unsigned char la16_mpp_access(la16_core_t core, unsigned short *addr, unsigned char vprot);
unsigned char la16_mpp_read(la16_core_t core, unsigned short uaddr, unsigned short *val);
unsigned char la16_mpp_write(la16_core_t core, unsigned short uaddr, unsigned short val);
unsigned char la16_mpp_read8(la16_core_t core, unsigned short uaddr, unsigned char *val);
unsigned char la16_mpp_write8(la16_core_t core, unsigned short uaddr, unsigned char val);

void la16_op_ppcnt(la16_core_t core);
void la16_op_ppktrrset(la16_core_t core);

void la16_op_vpset(la16_core_t core);
void la16_op_vpget(la16_core_t core);
void la16_op_vpflgset(la16_core_t core);
void la16_op_vpflgget(la16_core_t core);
void la16_op_vpaddr(la16_core_t core);

#endif /* LA16_INSTRUCTION_MPP_H */
