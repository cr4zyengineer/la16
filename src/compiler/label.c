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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <compiler/label.h>

bool code_label_exists(compiler_invocation_t *ci,
                       const char *name)
{
    for (int i = 0; i < ci->label_cnt; i++)
    {
        if (strcmp(ci->label[i].name, name) == 0)
        {
            return true;
        }
    }
    return false;
}

void code_token_label(compiler_invocation_t *ci)
{
    ci->label_cnt = 0;

    // Find out how many labels there are
    for(int i = 0; i < ci->token_cnt; i++)
    {
        if(ci->token[i].type == COMPILER_TOKEN_TYPE_LABEL)
        {
            (ci->label_cnt)++;
        }
    }

    // Allocate compiler label array
    ci->label = calloc(ci->label_cnt, sizeof(compiler_label_t));
    memset(ci->label, 0, sizeof(compiler_label_t) * ci->label_cnt);

    // Now parse the position of each label
    ci->label_cnt = 0;
    for(int i = 0; i < ci->token_cnt; i++)
    {
        if(ci->token[i].type == COMPILER_TOKEN_TYPE_LABEL)
        {
            size_t size = strlen(ci->token[i].token);
            char *name = malloc(size);
            memcpy(name, ci->token[i].token, size - 1);
            name[size - 1] = '\0';

            // Checking for duplicated labels
            if (code_label_exists(ci, name))
            {
                printf("[!] duplicate label: %s\n", name);
                exit(-1);
            }

            ci->label[ci->label_cnt].name = name;
            ci->label[ci->label_cnt].addr = ci->token[i].addr;
            ci->label[ci->label_cnt].rel = 1;
            ci->label_cnt++;
            continue;
        }
    }
}

unsigned short label_lookup(compiler_invocation_t *ci,
                            const char *name)
{
    for(int i = 0; i < ci->label_cnt; i++)
    {
        if(strcmp(ci->label[i].name, name) == 0)
        {
            if(ci->label[i].rel == 0)
            {
                return ci->label[i].addr;
            }
            else
            {
                return ci->label[i].addr + ci->image_text_start;
            }
        }
    }
    printf("[!] lookup: %s doesnt exist\n", name);
    return 0xFFFF;
}

