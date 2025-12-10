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
#include <string.h>
#include <compiler/section.h>
#include <la16/memory.h>
#include <compiler/parse.h>

void code_token_section(compiler_invocation_t *ci)
{
    // First we have to find out where each section is
    for(unsigned long i = 0; i < ci->token_cnt; i++)
    {
        // Just check the token type
        if(ci->token[i].type == COMPILER_TOKEN_TYPE_SECTION)
        {
            // Check if its the data section
            if(strcmp(ci->token[i].subtoken[1], ".data") == 0)
            {
                // Find variable name
                i++;
                for(; i < ci->token_cnt && ci->token[i].type == COMPILER_TOKEN_TYPE_SECTION_DATA; i++)
                {
                    // Insert label into label array
                    ci->label_cnt++;
                    ci->label = realloc(ci->label, sizeof(struct compiler_label) * ci->label_cnt);
                    ci->label[ci->label_cnt - 1].name = strdup(ci->token[i].subtoken[0]);
                    ci->label[ci->label_cnt - 1].addr = ci->image_uaddr;
                    ci->label[ci->label_cnt - 1].rel = 0;

                    // Write string into memory
                    char *token = strdup(ci->token[i].subtoken[2]);

                    // Gathering chain
                    unsigned long chain_cnt = 0;
                    const char *tok = strtok(token, ",");

                    while(tok != NULL)
                    {
                        chain_cnt++;
                        tok = strtok(NULL, ",");
                    }

                    char **chain = calloc(chain_cnt, sizeof(char*));

                    free(token);
                    token = strdup(ci->token[i].subtoken[2]);

                    chain_cnt = 0;
                    tok = strtok(token, ",");

                    while(tok != NULL)
                    {
                        chain[chain_cnt++] = strdup(tok);
                        tok = strtok(NULL, ",");
                    }

                    for(unsigned long a = 0; a < chain_cnt; a++)
                    {
                        parse_type_return_t pr = parse_type_lc(chain[a]);
                        if(pr.type == PARSE_TYPE_BUFFER)
                        {
                            char *buffer = (char*)pr.value;
                            for(unsigned short i = 0; i < pr.len; i++)
                            {
                                ci->image[ci->image_uaddr + i] = (unsigned char)buffer[i];
                            }
                            ci->image_uaddr += pr.len;
                        } else
                        {
                            ci->image[ci->image_uaddr] = pr.value;
                            (ci->image_uaddr)++;
                        }
                        free(chain[a]);
                    }
                    free(chain);
                }
                i--;
            }
            else if(strcmp(ci->token[i].subtoken[1], ".bss") == 0)
            {
                // Find variable name
                i++;
                for(; i < ci->token_cnt && ci->token[i].type == COMPILER_TOKEN_TYPE_SECTION_DATA; i++)
                {
                    // Insert label into label array
                    ci->label_cnt++;
                    ci->label = realloc(ci->label, sizeof(struct compiler_label) * ci->label_cnt);
                    ci->label[ci->label_cnt - 1].name = strdup(ci->token[i].subtoken[0]);
                    ci->label[ci->label_cnt - 1].addr = ci->image_uaddr;
                    ci->label[ci->label_cnt - 1].rel = 0;

                    // Write string into memory
                    parse_type_return_t pr = parse_type_lc(ci->token[i].subtoken[1]);
                    ci->image_uaddr += pr.value;
                }
                i--;
            }
        }
    }

    // Set text start, but alligned with 4
    ci->image_uaddr = (ci->image_uaddr + 3) & ~0x3;
    ci->image_text_start = ci->image_uaddr;
}
