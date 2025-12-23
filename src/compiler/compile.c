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
#include <compiler/compile.h>
#include <compiler/code.h>
#include <compiler/label.h>
#include <compiler/section.h>
#include <compiler/constant.h>
#include <compiler/compiler.h>

compiler_invocation_t *compiler_invocation_alloc(void)
{
    compiler_invocation_t *ci = calloc(1, sizeof(compiler_invocation_t));
    ci->image_uaddr += 4;   /* image has to stay alligned ;w; */
    return ci;
}

void compiler_invocation_dealloc(compiler_invocation_t *ci)
{
    /* freeing the code it self */
    free(ci->code);

    /* freeing the token structures */
    for(unsigned long i = 0; i < ci->token_cnt; i++)
    {
        free(ci->token[i].token);
        for(unsigned long a = 0; a < ci->token[i].subtoken_cnt; i++)
        {
            free(ci->token[i].subtoken[a]);
        }
        free(ci->token[i].subtoken);
    }
    free(ci->token);

    /* freeing labels */
    for(unsigned long i = 0; i < ci->label_cnt; i++)
    {
        free(ci->label[i].name);
    }
    free(ci->label);

    free(ci);
}

void compile_files(char **files,
                   int file_cnt)
{
    /* allocating compiler invocation */
    compiler_invocation_t *ci = compiler_invocation_alloc();

    /* gathering code */
    get_code_buffer(files, file_cnt, ci);

    /* formatting code by removing all the useless
     * and ignored stuff and extracting solely usable
     * information to process
     */
    code_remove_comments(ci);
    code_replace_tab_with_spaces(ci);
    code_remove_newlines(ci);

    /* generating tokens,labels,sections out of the code */
    code_tokengen(ci);
    code_token_label(ci);
    code_token_section(ci);
    code_token_label_insert_start(ci);
    code_token_constant(ci);

    /* finally compiling it to machine code */
    la16_compiler_lowlevel(ci);

    /* spitting out binary */
    code_binary_spitout(ci);

    /* deallocating compiler invocation */
    //compiler_invocation_dealloc(ci);
}
