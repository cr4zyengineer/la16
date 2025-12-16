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
        if(ci->token[i].type == COMPILER_TOKEN_TYPE_LABEL ||
           ci->token[i].type == COMPILER_TOKEN_TYPE_LABEL_SCOPED)
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
            if(code_label_exists(ci, name))
            {
                printf("[!] duplicate label: %s\n", name);
                exit(1);
            }

            // Set scope label index
            ci->scope_label_idx = ci->label_cnt;

            ci->label[ci->label_cnt].name = name;
            ci->label[ci->label_cnt].addr = ci->token[i].addr;
            ci->label[ci->label_cnt].rel = 1;
            ci->label_cnt++;
            continue;
        }
        else if(ci->token[i].type == COMPILER_TOKEN_TYPE_LABEL_SCOPED)
        {
            // Woah its a scoped label :/ Im so cooked now
            // What is if people will have problems
            // RAWWRRR I just bite em

            // Getting individual sizes
            size_t scope_label_name_size = strlen(ci->label[ci->scope_label_idx].name);
            size_t scoped_label_name_size = strlen(ci->token[i].token);
            size_t total_size = scope_label_name_size + scoped_label_name_size;

            // Allocating the total size
            char *name = malloc(total_size);

            // Now it gets complex, we copy first the scope label name and then the scoped label name
            memcpy(name, ci->label[ci->scope_label_idx].name, scope_label_name_size);
            memcpy(name + scope_label_name_size, ci->token[i].token, scoped_label_name_size - 1);

            // Null terminating
            name[total_size - 1] = '\0';

            // Checking for duplicated labels
            if(code_label_exists(ci, name))
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
                            const char *name,
                            const char *scope)
{
    // defines the actual lookup target
    char *lookup_target = NULL;

    // Looking if name has a dot
    if(name[0] == '.')
    {
        // Its meant to be in a scope!
        
        // Getting sizes
        size_t name_size = strlen(name);
        size_t scope_size = strlen(scope);
        size_t total_size = name_size + scope_size;

        // Allocating size
        lookup_target = malloc(total_size + 1);

        // Copying to lookup target
        memcpy(lookup_target, scope, scope_size);
        memcpy(lookup_target + scope_size, name, name_size);
        lookup_target[total_size] = '\0';
    }
    else
    {
        // Its not meant to be in a scope!
        lookup_target = (char*)name;
    }

    // Iterating all labels to find the correct label
    unsigned short addr = 0xFFFF;
    for(int i = 0; i < ci->label_cnt; i++)
    {
        if(strcmp(ci->label[i].name, lookup_target) == 0)
        {
            if(ci->label[i].rel == 0)
            {
                addr = ci->label[i].addr;
                break;
            }
            else
            {
                addr = ci->label[i].addr + ci->image_text_start;
                break;
            }
        }
    }

    /* checking if lookup was successful NOTE: might remove this in the future */
    if(addr == 0xFFFF)
    {
        printf("[!] lookup: %s doesnt exist\n", lookup_target);
    }

    /* checking if lookup_target is name, if not release its memory */
    if(lookup_target != name)
    {
        free(lookup_target);
    }

    return addr;
}

void code_token_label_insert_start(compiler_invocation_t *ci)
{
    /* finding start label */
    unsigned short addr = label_lookup(ci, "_start", NULL);
    if(addr == 0xFFFF)
    {
        exit(1);
    }

    /* writing start address into the start of the image */
    unsigned short *entry = (unsigned short*)&(ci->image[0]);
    *entry = addr;
}