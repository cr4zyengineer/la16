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

#define LA16_MEMORY_VALUE_MIN 0b00000000
#define LA16_MEMORY_VALUE_MAX 0b11111111
#define LA16_MEMORY_VALUE_16_MIN = 0b0000000000000000
#define LA16_MEMORY_VALUE_16_MAX = 0b1111111111111111

enum LA16_MEMORY_RETURN
{
    LA16_MEMORY_RETURN_SUCCESS       = 0b00000000,
    LA16_MEMORY_RETURN_FAILURE       = 0b00000001,
    LA16_MEMORY_RETURN_OUT_OF_BOUNDS = 0b00000010,
    LA16_MEMORY_RETURN_PROTECTION    = 0b00000011
};

enum LA16_MEMORY_PROT
{
    LA16_MEMORY_PROT_NONE    = 0b00000000,
    LA16_MEMORY_PROT_READ    = 0b00000001,
    LA16_MEMORY_PROT_WRITE   = 0b00000010,
    LA16_MEMORY_PROT_EXEC    = 0b00000100
};

#define LA16_MEMORY_PAGE_SIZE 0xFF

typedef unsigned short la16_memory_address_t;
typedef unsigned short la16_memory_size_t;
typedef unsigned short la16_memory_value_16_t;
typedef unsigned char la16_memory_value_t;
typedef unsigned char la16_memory_return_t;
typedef unsigned char la16_memory_prot_t;

struct la16_memory_page
{
    unsigned char prot;
    la16_memory_address_t start;
};

typedef struct la16_memory_page la16_memory_page_t;

struct la16_memory
{
    unsigned char *memory;
    unsigned short memory_size;
    la16_memory_page_t *page;
    unsigned short page_cnt;
};

typedef struct la16_memory la16_memory_t;

la16_memory_t *la16_memory_alloc(la16_memory_size_t size);
void la16_memory_dealloc(la16_memory_t *memory);

void la16_memory_load_image(la16_memory_t *memory, const char *image_path);

#endif /* LA16_MEMORY_H */
