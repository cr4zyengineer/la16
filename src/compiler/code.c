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

#include <compiler/code.h>
#include <compiler/cmptok.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

void get_code_buffer(char **files,
                     int file_cnt,
                     compiler_invocation_t *ci)
{
    int *fd = calloc(file_cnt, sizeof(int));
    struct stat *fdstat = calloc(file_cnt, sizeof(struct stat));
    size_t size_needed = 0;
    size_t size_written = 0;

    // --- Calculate total size ---
    for (int i = 0; i < file_cnt; i++)
    {
        fd[i] = open(files[i], O_RDONLY);
        if (fd[i] < 0) {
            perror(files[i]);
            exit(EXIT_FAILURE);
        }

        if (fstat(fd[i], &fdstat[i]) < 0) {
            perror("fstat");
            exit(EXIT_FAILURE);
        }

        size_needed += fdstat[i].st_size;
    }
    size_needed++;

    // --- Allocate +1 for null terminator ---
    char *buf = malloc(size_needed + 1);
    if (!buf) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // --- Read files into buffer ---
    for (int i = 0; i < file_cnt; i++)
    {
        ssize_t bytes = read(fd[i], buf + size_written, fdstat[i].st_size);
        if (bytes < 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        size_written += bytes;
    }

    // --- Null terminate ---
    buf[size_written++] = '\n';
    buf[size_written] = '\0';

    // --- Cleanup ---
    for (int i = 0; i < file_cnt; i++)
        close(fd[i]);
    free(fd);
    free(fdstat);

    ci->code = buf;
}

void code_remove_comments(compiler_invocation_t *ci)
{
    size_t i = 0;
    while (ci->code[i] != '\0')
    {
        if (ci->code[i] == ';')
        {
            size_t s = i;
            while (s > 0 && ci->code[s - 1] == ' ')
            {
                s--;
            }
            if (s == 0 || ci->code[s - 1] == '\n')
            {
                i = s;
            }

            size_t e = i;
            while (ci->code[e] != '\n' && ci->code[e] != '\0')
            {
                e++;
            }

            memmove(&ci->code[i], &ci->code[e], strlen(&ci->code[e]) + 1);
            if (ci->code[i] == '\0')
            {
                break;
            }
        }
        else if (ci->code[i] == '/' && ci->code[i + 1] == '*')
        {
            size_t comment_start = i;  // Save BEFORE modifying i

            size_t s = i;
            while (s > 0 && ci->code[s - 1] == ' ')
            {
                s--;
            }
            if (s == 0 || ci->code[s - 1] == '\n')
            {
                i = s;
            }

            // Find closing */
            size_t e = comment_start + 2;
            while (ci->code[e] != '\0')
            {
                if (ci->code[e] == '*' && ci->code[e + 1] == '/')
                {
                    e += 2;
                    break;
                }
                e++;
            }

            // Skip trailing whitespace until newline
            while (ci->code[e] == ' ')
            {
                e++;
            }

            memmove(&ci->code[i], &ci->code[e], strlen(&ci->code[e]) + 1);
            if (ci->code[i] == '\0')
            {
                break;
            }
        }
        else
        {
            i++;
        }
    }
}

void code_remove_newlines(compiler_invocation_t *ci)
{
    size_t src = 0, dst = 0;
    bool last_was_newline = true;
    while (ci->code[src] != '\0')
    {
        if (ci->code[src] == '\r' || ci->code[src] == '\n') {
            if (!last_was_newline) {
                ci->code[dst++] = '\n';
                last_was_newline = true;
            }
            if (ci->code[src] == '\r' && ci->code[src + 1] == '\n') {
                src++;
            }
            src++;
            continue;
        }
        ci->code[dst++] = ci->code[src++];
        last_was_newline = false;
    }
    ci->code[dst] = '\0';
}

void code_replace_tab_with_spaces(compiler_invocation_t *ci)
{
    size_t src = 0;
    while(ci->code[src] != '\0')
    {
        if(ci->code[src] == '\t')
        {
            /* replacing with space */
            ci->code[src] = ' ';
        }
        src++;
    }
}

void code_tokengen(compiler_invocation_t *ci)
{
    /* Gathering token count */
    size_t token_cnt = 0;
    size_t code_size = strlen(ci->code);

    for(size_t i = 0; i < code_size; i++)
    {
        // Look for '\n'
        if(ci->code[i] == '\n')
        {
            token_cnt += 1;
        }
    }

    /* Gathering base tokens */
    ci->token = calloc(token_cnt, sizeof(compiler_token_t));
    ci->token_cnt = token_cnt;

    token_cnt = 0;
    size_t last_line_offset = 0;
    for(size_t i = 0; i < code_size; i++)
    {
        // Look for '\n'
        if(ci->code[i] == '\n')
        {
            size_t start_off = last_line_offset;
            size_t end_off = i;

            for(; start_off < end_off; start_off++)
            {
                if(ci->code[start_off] != ' ')
                {
                    break;
                }
            }

            for(; end_off > start_off; end_off--)
            {
                if(ci->code[end_off] != ' ' && ci->code[end_off] != '\n')
                {
                    end_off++;
                    break;
                }
            }

            size_t len = end_off - start_off;
            ci->token[token_cnt].token = malloc(len + 1);
            memcpy(ci->token[token_cnt].token, &(ci->code[start_off]), len);
            ci->token[token_cnt].token[len] = '\0';
            last_line_offset = i + 1;
            token_cnt++;
        }
    }

    /* Gathering their subtokens */
    for(unsigned long i = 0; i < ci->token_cnt; i++)
    {
        // Get amount of sub tokens
        ci->token[i].subtoken_cnt = 0;
        const char *token = cmptok(ci->token[i].token);
        while(token != NULL)
        {
            ci->token[i].subtoken_cnt++;
            token = cmptok(NULL);
        }

        // Allocate subtoken array
        ci->token[i].subtoken = calloc(sizeof(char*), ci->token[i].subtoken_cnt);

        // Get the actual subtokens
        ci->token[i].subtoken_cnt = 0;
        token = cmptok(ci->token[i].token);
        while(token != NULL)
        {
            ci->token[i].subtoken[ci->token[i].subtoken_cnt] = strdup(token);
            ci->token[i].subtoken_cnt++;
            token = cmptok(NULL);
        }
    }

    /* Evaluate Type and address */
    unsigned short addr = 0x00;
    unsigned char section_mode = 0b0;
    for(unsigned long i = 0; i < ci->token_cnt; i++)
    {
        if(ci->token[i].subtoken_cnt < 2)
        {
            // Label Check
            size_t size = strlen(ci->token[i].subtoken[0]);

            // Check if the last character of the first subtoken is a ':'
            if(ci->token[i].subtoken[0][size - 1] == ':')
            {
                section_mode = 0b0;

                // Its a scoped label if its first character is a dot
                ci->token[i].type = (ci->token[i].subtoken[0][0] == '.') ? COMPILER_TOKEN_TYPE_LABEL_SCOPED : COMPILER_TOKEN_TYPE_LABEL;
                
                ci->token[i].addr = addr;
                continue;
            }
        }
        else if(ci->token[i].subtoken_cnt < 3)
        {
            if(strcmp(ci->token[i].subtoken[0], "section") == 0)
            {
                section_mode = 0b1;
                ci->token[i].type = COMPILER_TOKEN_TYPE_SECTION;
                ci->token[i].addr = 0x0;
                continue;
            }
        }

        if(section_mode)
        {
            // Its part of a section
            ci->token[i].type = COMPILER_TOKEN_TYPE_SECTION_DATA;
            ci->token[i].addr = 0x0;
        }
        else
        {
            // Its probably ASM
            // Before blindly assuming it we check if its a call
            if(ci->token[i].subtoken_cnt >= 2 &&
               strcmp(ci->token[i].subtoken[0], "call") == 0)
            {
                unsigned char needed_cycleops = ci->token[i].subtoken_cnt - 2;
    
                // Extract all subtokens without delimiter
                char **subtoken = calloc(ci->token[i].subtoken_cnt, sizeof(char*));

                for(unsigned long a = 0; a < ci->token[i].subtoken_cnt; a++)
                {
                    size_t subtoken_len = strlen(ci->token[i].subtoken[a]);
                    subtoken[a] = malloc(subtoken_len + 1);
                    memcpy(subtoken[a], ci->token[i].subtoken[a], subtoken_len + 1);
                    if(subtoken_len > 0 && subtoken[a][subtoken_len - 1] == ',')
                    {
                        subtoken[a][subtoken_len - 1] = '\0';
                    }
                }

                #define PUSH_MAX 7

                const char *push_map[PUSH_MAX] = {
                    "r0", "r1", "r2", "r3", "r4", "r5", "r6"
                };

                // Check max args
                if(needed_cycleops > PUSH_MAX)
                {
                    printf("[!] call can have maximum of %d arguments!\n", PUSH_MAX);
                    exit(EXIT_FAILURE);
                }

                // Determine which registers actually need action
                bool needs_action[PUSH_MAX] = {false};
                unsigned char action_count = 0;

                for(unsigned char arg = 0; arg < needed_cycleops; arg++)
                {
                    // If argument is already in the target register, skip it
                    if(strcmp(push_map[arg], subtoken[2 + arg]) != 0)
                    {
                        needs_action[arg] = true;
                        action_count++;
                    }
                }

                // Check for conflicts: is any source register going to be clobbered?
                // Example: call _func, r1, r0  → mov r0, r1; mov r1, r0 (r0 clobbered before read!)
                bool has_conflict = false;
                for(unsigned char arg = 0; arg < needed_cycleops && !has_conflict; arg++)
                {
                    if(!needs_action[arg]) continue;
        
                    const char *source = subtoken[2 + arg];
        
                    // Check if this source is a target register that will be overwritten
                    // by an EARLIER mov (lower index)
                    for(unsigned char earlier = 0; earlier < arg; earlier++)
                    {
                        if(needs_action[earlier] && strcmp(source, push_map[earlier]) == 0)
                        {
                            has_conflict = true;
                            break;
                        }
                    }
                }

                // If conflict detected, fall back to saving ALL argument registers
                if(has_conflict)
                {
                    for(unsigned char arg = 0; arg < needed_cycleops; arg++)
                    {
                        needs_action[arg] = true;
                    }
                    action_count = needed_cycleops;
                }

                // Calculate token count
                // push for each action + mov for each action + 1 bl + pop for each action
                unsigned char needed_tokens = (action_count * 3) + 1;
    
                // Edge case: no actions needed, just the bl
                if(action_count == 0)
                {
                    needed_tokens = 1;
                }

                unsigned char token_idx = 0;
                compiler_token_t *tci = calloc(needed_tokens, sizeof(compiler_token_t));

                // Injecting pushes (only for registers that need it)
                for(unsigned char push = 0; push < needed_cycleops; push++)
                {
                    if(!needs_action[push]) continue;
        
                    const char *reg = push_map[push];
                    tci[token_idx].type = COMPILER_TOKEN_TYPE_ASM;
                    tci[token_idx].addr = addr;
                    tci[token_idx].token = malloc(strlen(reg) + 6);
                    sprintf(tci[token_idx].token, "push %s", reg);
                    tci[token_idx].subtoken_cnt = 2;
                    tci[token_idx].subtoken = calloc(2, sizeof(char*));
                    tci[token_idx].subtoken[0] = strdup("push");
                    tci[token_idx].subtoken[1] = strdup(reg);
                    addr += 4;
                    token_idx++;
                }

                // Injecting moves (only for registers that need it)
                for(unsigned char mov = 0; mov < needed_cycleops; mov++)
                {
                    if(!needs_action[mov]) continue;
        
                    const char *dest_reg = push_map[mov];
                    const char *source = subtoken[2 + mov];
                    tci[token_idx].type = COMPILER_TOKEN_TYPE_ASM;
                    tci[token_idx].addr = addr;
                    tci[token_idx].token = malloc(strlen(dest_reg) + strlen(source) + 7);
                    sprintf(tci[token_idx].token, "mov %s, %s", dest_reg, source);
                    tci[token_idx].subtoken_cnt = 3;
                    tci[token_idx].subtoken = calloc(3, sizeof(char*));
                    tci[token_idx].subtoken[0] = strdup("mov");
                    tci[token_idx].subtoken[1] = strdup(dest_reg);
                    tci[token_idx].subtoken[2] = strdup(source);
                    addr += 4;
                    token_idx++;
                }

                // Injecting branch link
                size_t call_name_len = strlen(subtoken[1]);
                tci[token_idx].type = COMPILER_TOKEN_TYPE_ASM;
                tci[token_idx].addr = addr;
                tci[token_idx].token = malloc(4 + call_name_len);
                sprintf(tci[token_idx].token, "bl %s", subtoken[1]);
                tci[token_idx].subtoken_cnt = 2;
                tci[token_idx].subtoken = calloc(2, sizeof(char*));
                tci[token_idx].subtoken[0] = strdup("bl");
                tci[token_idx].subtoken[1] = strdup(subtoken[1]);
                addr += 4;
                token_idx++;

                // Injecting pops (reverse order)
                for(unsigned char pop = 0; pop < needed_cycleops; pop++)
                {
                    unsigned char pop_idx = needed_cycleops - 1 - pop;
                    if(!needs_action[pop_idx]) continue;
        
                    const char *reg = push_map[pop_idx];
                    tci[token_idx].type = COMPILER_TOKEN_TYPE_ASM;
                    tci[token_idx].addr = addr;
                    tci[token_idx].token = malloc(strlen(reg) + 5);
                    sprintf(tci[token_idx].token, "pop %s", reg);
                    tci[token_idx].subtoken_cnt = 2;
                    tci[token_idx].subtoken = calloc(2, sizeof(char*));
                    tci[token_idx].subtoken[0] = strdup("pop");
                    tci[token_idx].subtoken[1] = strdup(reg);
                    addr += 4;
                    token_idx++;
                }

                /* Injecting tci into tokens */
                // Free the original call token's data
                free(ci->token[i].token);
                for(unsigned long st = 0; st < ci->token[i].subtoken_cnt; st++)
                {
                    free(ci->token[i].subtoken[st]);
                }
                free(ci->token[i].subtoken);

                // Calculate new token count: remove 1 (the call), add needed_tokens
                size_t new_token_cnt = ci->token_cnt - 1 + needed_tokens;

                // Allocate new token array
                compiler_token_t *new_tokens = calloc(new_token_cnt, sizeof(compiler_token_t));

                // Copy tokens before the call instruction
                if(i > 0)
                {
                    memcpy(new_tokens, ci->token, i * sizeof(compiler_token_t));
                }

                // Copy the expanded tci tokens
                memcpy(&new_tokens[i], tci, needed_tokens * sizeof(compiler_token_t));

                // Copy tokens after the call instruction
                if(i + 1 < ci->token_cnt)
                {
                    memcpy(&new_tokens[i + needed_tokens], 
                           &ci->token[i + 1], 
                           (ci->token_cnt - i - 1) * sizeof(compiler_token_t));
                }

                // Free old token array and tci (but not tci contents, they're now in new_tokens)
                free(ci->token);
                free(tci);

                // Free subtoken copies
                for(unsigned long a = 0; a < ci->token[i].subtoken_cnt; a++)
                {
                    free(subtoken[a]);
                }
                free(subtoken);

                // Update ci
                ci->token = new_tokens;
                ci->token_cnt = new_token_cnt;

                // Adjust loop counter: we replaced 1 token with needed_tokens,
                // so skip past the injected tokens (minus 1 because loop will increment)
                i += needed_tokens - 1;
            }
            else
            {
                ci->token[i].type = COMPILER_TOKEN_TYPE_ASM;
                ci->token[i].addr = addr;
                addr += 4;
            }
        }
    }
}

void code_binary_spitout(compiler_invocation_t *ci)
{
    // Open a.out
    int fd = open("a.out", O_RDWR | O_CREAT | O_TRUNC, 0777);

    // Now write
    write(fd, ci->image, ci->image_uaddr);

    // Close file descriptor
    close(fd);
}