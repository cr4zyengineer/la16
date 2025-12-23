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

#ifndef COMPILER_TYPE_H
#define COMPILER_TYPE_H

/* token types */
#define COMPILER_TOKEN_TYPE_ASM             0b000
#define COMPILER_TOKEN_TYPE_LABEL           0b001
#define COMPILER_TOKEN_TYPE_SECTION         0b010
#define COMPILER_TOKEN_TYPE_SECTION_DATA    0b011
#define COMPILER_TOKEN_TYPE_LABEL_SCOPED    0b100

typedef unsigned char compiler_token_type_t;

typedef struct {
    compiler_token_type_t type;
    unsigned short addr;
    char *token;
    char **subtoken;
    unsigned long subtoken_cnt;
} compiler_token_t;

typedef struct {
    char *name;
    unsigned short addr;
    unsigned char rel;
} compiler_label_t;

typedef struct {
    char *label_name;
    unsigned short imm16_uaddr;
} compiler_reloc_action_t;

typedef struct {
    char *code;                             /* raw code */
    unsigned long scope_label_idx;          /* current index of the scope label */
    compiler_token_t *token;                /* token array */
    unsigned long token_cnt;                /* count of tokens */
    compiler_label_t *label;                /* label array */
    unsigned long label_cnt;                /* count of labels */
    unsigned char image[0xFFFF];            /* compiled image */
    unsigned short image_uaddr;             /* address marker for compiled image */
    unsigned short image_text_start;        /* start of the images text region */
    compiler_reloc_action_t reloc_table[0xFFFF];
    unsigned short reloc_table_cnt;
} compiler_invocation_t;

#endif /* COMPILER_TYPE_H */
