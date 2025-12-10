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

enum COMPILER_TOKEN_TYPE
{
    COMPILER_TOKEN_TYPE_ASM = 0b00000000,
    COMPILER_TOKEN_TYPE_LABEL = 0b00000001,
    COMPILER_TOKEN_TYPE_SECTION = 0b00000010,
    COMPILER_TOKEN_TYPE_SECTION_DATA = 0b00000011
};

typedef unsigned char compiler_token_type_t;

struct compiler_token
{
    compiler_token_type_t type;
    unsigned short addr;
    char *token;
    char **subtoken;
    unsigned long subtoken_cnt;
};

typedef struct compiler_token compiler_token_t;

struct compiler_section
{
    char *name;
    char **block;
};

typedef struct compiler_section compiler_section_t;

struct compiler_label
{
    char *name;
    unsigned short addr;
    unsigned char rel;
};

typedef struct compiler_label compiler_label_t;

struct compiler_invocation
{
    /* Raw and Compile ready code later */
    char *code;

    /* Tokenized Compile ready code, for the lowlinecompiler */
    compiler_token_t *token;
    unsigned long token_cnt;

    /* Information such as label offsets and section data */
    compiler_section_t *section;
    compiler_label_t *label;
    unsigned long section_cnt;
    unsigned long label_cnt;

    unsigned char image[0xFFFF];
    unsigned short image_uaddr;
    unsigned short image_text_start;
};

typedef struct compiler_invocation compiler_invocation_t;

#endif /* COMPILER_TYPE_H */
