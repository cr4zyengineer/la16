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

#include <compiler/constant.h>
#include <compiler/parse.h>
#include <compiler/label.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int code_token_constant_lookup(compiler_invocation_t *ci, const char *name)
{
    // Iterating all constants to find the correct constants
    for(int i = 0; i < ci->constant_cnt; i++)
    {
        if(strcmp(ci->constant[i].name, name) == 0)
        {
            return ci->constant[i].value;
        }
    }
    return COMPILER_CONSTANT_NOT_FOUND;
}

void code_token_constant(compiler_invocation_t *ci)
{
    ci->constant_cnt = 0;

    // Find out how many labels there are
    for(int i = 0; i < ci->token_cnt; i++)
    {
        if(ci->token[i].type == COMPILER_TOKEN_TYPE_CONSTANT)
        {
            (ci->constant_cnt)++;
        }
    }

    // Allocate compiler label array
    ci->constant = calloc(ci->constant_cnt, sizeof(compiler_constant_t));
    memset(ci->constant, 0, sizeof(compiler_constant_t) * ci->constant_cnt);

    // Now parse the position of each label
    ci->constant_cnt = 0;
    for(int i = 0; i < ci->token_cnt; i++)
    {
        if(ci->token[i].type == COMPILER_TOKEN_TYPE_CONSTANT)
        {
            char *name = strdup(ci->token[i].subtoken[1]);

            parse_type_return_t pr = parse_type_lc(ci->token[i].subtoken[2]);

            switch(pr.type)
            {
                case PARSE_TYPE_STRING:
                    /* could be a label */
                    ci->constant[ci->constant_cnt].name = name;
                    unsigned int addr = label_lookup(ci, ci->token[i].subtoken[2], NULL);

                    if(addr == COMPILER_LABEL_NOT_FOUND)
                    {
                        printf("[!] lookup: %s not found\n", ci->token[i].subtoken[2]);
                        exit(1);
                    }

                    ci->constant[ci->constant_cnt].value = (unsigned short)addr;

                    break;
                case PARSE_TYPE_NUMBER:
                case PARSE_TYPE_HEX:
                case PARSE_TYPE_BIN:
                case PARSE_TYPE_CHAR:
                    ci->constant[ci->constant_cnt].name = name;
                    ci->constant[ci->constant_cnt].value = pr.value;
                    break;
                case PARSE_TYPE_BUFFER:
                    printf("[!] buffers in constant not supported\n");
                    exit(1);
                default:
                    printf("[!] fatal: parse type not recognized\n");
                    exit(1);
            }

            ci->constant_cnt++;
        }
    }
}