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

#ifndef LA16_MEMORY_H
#define LA16_MEMORY_H

#include <stdio.h>

#define LA16_MEMORY_VALUE_MIN 0x0
#define LA16_MEMORY_VALUE_MAX 0xFFFF
#define LA16_MEMORY_PAGE_SIZE 0xFF

typedef unsigned short la16_memory_address_t;
typedef unsigned short la16_memory_size_t;
typedef unsigned short la16_memory_value_t;

struct la16_memory
{
    unsigned char *memory;
    unsigned short memory_size;
    unsigned short page_cnt;
};

typedef struct la16_memory la16_memory_t;

la16_memory_t *la16_memory_alloc(la16_memory_size_t size);
void la16_memory_dealloc(la16_memory_t *memory);

void la16_memory_load_image(la16_memory_t *memory, const char *image_path);

#endif /* LA16_MEMORY_H */
