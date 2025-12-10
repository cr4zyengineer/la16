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

#ifndef LA16_OPU_ALU_H
#define LA16_OPU_ALU_H

#include <la16/core.h>

void op_mov(la16_core_t core);
void op_mov8(la16_core_t core);
void op_add(la16_core_t core);
void op_sub(la16_core_t core);
void op_mul(la16_core_t core);
void op_div(la16_core_t core);
void op_inc(la16_core_t core);
void op_dec(la16_core_t core);
void op_and(la16_core_t core);
void op_not(la16_core_t core);
void op_bsl(la16_core_t core);
void op_bsr(la16_core_t core);
void op_and(la16_core_t core);
void op_not(la16_core_t core);
void op_bsl(la16_core_t core);
void op_bsr(la16_core_t core);
void op_mst(la16_core_t core);
void op_mst8(la16_core_t core);
void op_mld(la16_core_t core);
void op_mld8(la16_core_t core);
void op_swp(la16_core_t core);
void op_xor(la16_core_t core);
void op_or(la16_core_t core);

#endif /* LA16_OPU_ALU_H */
