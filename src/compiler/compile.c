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
#include <la16/compiler.h>

void compile_files(char **files,
                   int file_cnt)
{
    compiler_invocation_t ci = {};
    ci.image_uaddr += 2;

    /* Code prepare */
    get_code_buffer(files, file_cnt, &ci);
    code_remove_comments(&ci);
    code_remove_newlines(&ci);

    printf("%s\n", ci.code);

    code_tokengen(&ci);
    code_token_label(&ci);
    code_token_section(&ci);
    la16_compiler_lowlevel(&ci);

    // Lockup label _start
    unsigned short addr = label_lookup(&ci, "_start", NULL);
    if(addr == 0xFFFF)
    {
        exit(1);
    }

    // Write start entry
    unsigned short *entry = (unsigned short*)&ci.image[0];
    *entry = addr;

    // Spitting binary
    code_binary_spitout(&ci);

    // One shot cli utility
    // NONO freeing!
    // The iOS version will free tho obviously
}
