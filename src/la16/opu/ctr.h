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

#ifndef LA16_OPU_CTR_H
#define LA16_OPU_CTR_H

#include <la16/core.h>

void op_cmp(la16_core_t core);
void op_jeq(la16_core_t core);
void op_jil(la16_core_t core);
void op_jig(la16_core_t core);
void op_jmp(la16_core_t core);
void op_pop(la16_core_t core);
void op_psh(la16_core_t core);
void op_bra(la16_core_t core);
void op_beq(la16_core_t core);
void op_bil(la16_core_t core);
void op_big(la16_core_t core);
void op_ret(la16_core_t core);
void op_jnq(la16_core_t core);
void op_bnq(la16_core_t core);
void op_reteq(la16_core_t core);
void op_retnq(la16_core_t core);
void op_retil(la16_core_t core);
void op_retig(la16_core_t core);

#endif /* LA16_OPU_CTR_H */
