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

#include <compiler/compiler.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <compiler/parse.h>
#include <compiler/opcode.h>

unsigned int la16_compiler_machinecode(unsigned char opcode,
                                       unsigned char mode,
                                       unsigned char a,
                                       unsigned short b,
                                       unsigned short *c)
{
    unsigned int instruction;

    ((unsigned char*)&instruction)[0] = opcode;

    /* checking which layout to use */
    bool newABILayout = false;
    switch(mode)
    {
        case LA16_PTCRYPT_COMBO_REG_IMM8:
        case LA16_PTCRYPT_COMBO_IMM8_NONE:
        case LA16_PTCRYPT_COMBO_IMM8_REG:
        case LA16_PTCRYPT_COMBO_IMM8_IMM8:
        case LA16_PTCRYPT_COMBO_NONE_IMM8:
            newABILayout = true;
        default:
            break;
    }

    /* now two different realization paths */
    if(newABILayout)
    {
        ((unsigned char*)&instruction)[1] = (mode << 4) | a;
        ((unsigned char*)&instruction)[2] = c[0];
        ((unsigned char*)&instruction)[3] = c[1];
    }
    else
    {
        ((unsigned char*)&instruction)[1] = (mode << 4) | a;
        ((unsigned short*)&instruction)[1] = b;
    }

    return instruction;
}

enum LA16_PTCRYPT
{
    LA16_PTCRYPT_NONE = 0b000,
    LA16_PTCRYPT_REG  = 0b001,
    LA16_PTCRYPT_IMM  = 0b010,
    LA16_PTCRYPT_IMM8 = 0b011,
    LA16_PTCRYPT_ERR  = 0b100,
};

void la16_compiler_lowcodeline_parameter_parser(const char *parameter,
                                                const char *scope,
                                                unsigned char *ptcrypt,
                                                unsigned short *value,
                                                compiler_invocation_t *ci)
{
    *ptcrypt = LA16_PTCRYPT_ERR;

    if(parameter[0] == '\0')
    {
        *ptcrypt = LA16_PTCRYPT_NONE;
        return;
    }

    // Look for register matching
    size_t parameter_size = strlen(parameter);
    if(parameter_size >= 2 && parameter_size <= 3)
    {
        switch(parameter[0])
        {
            case 'r':
                switch(parameter[1])
                {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                        *ptcrypt = LA16_PTCRYPT_REG;
                        *value = (parameter[1] - '0') + LA16_REGISTER_R0;
                        return;
                    case 'r':
                        *ptcrypt = LA16_PTCRYPT_REG;
                        *value = LA16_REGISTER_RR;
                        return;
                    default:
                        break;
                }
                break;
            case 'e':
            {
                if(parameter[1] == 'l')
                {
                    if(parameter_size == 3 &&
                       parameter[2] == 'b')
                    {
                        *ptcrypt = LA16_PTCRYPT_REG;
                        *value = LA16_REGISTER_ELB;
                        return;
                    }
                    *ptcrypt = LA16_PTCRYPT_REG;
                    *value = LA16_REGISTER_EL;
                    return;
                }
                break;
            }
            case 'p':
            {
                if(parameter[1] == 'c')
                {
                    *ptcrypt = LA16_PTCRYPT_REG;
                    *value = LA16_REGISTER_PC;
                    return;
                }
                break;
            }
            case 's':
                if(parameter[1] == 'p')
                {
                    *ptcrypt = LA16_PTCRYPT_REG;
                    *value = LA16_REGISTER_SP;
                    return;
                }
                break;
            case 'f':
                if(parameter[1] == 'p')
                {
                    *ptcrypt = LA16_PTCRYPT_REG;
                    *value = LA16_REGISTER_FP;
                    return;
                }
                break;
            case 'c':
                if(parameter[1] == 'f')
                {
                    *ptcrypt = LA16_PTCRYPT_REG;
                    *value = LA16_REGISTER_CF;
                    return;
                }
                break;
            default:
                break;
        }
    }

    parse_type_return_t pr = parse_type_lc(parameter);
    if(pr.type != PARSE_TYPE_STRING)
    {
        if (pr.value <= 0xFF)
        {
            *ptcrypt = LA16_PTCRYPT_IMM8;
            *value   = pr.value;
        }
        else
        {
            *ptcrypt = LA16_PTCRYPT_IMM;
            *value   = pr.value;
        }
    }
    else
    {
        unsigned int addr = label_lookup(ci, parameter, scope);
        if (addr != COMPILER_LABEL_NOT_FOUND)
        {
            *ptcrypt = LA16_PTCRYPT_IMM;
            *value = addr;
        }
        else
        {
            unsigned int const_value = code_token_constant_lookup(ci, parameter);
            if (const_value == COMPILER_CONSTANT_NOT_FOUND)
            {
                printf("[!] lookup: %s doesnt exist\n", parameter);
                exit(1);
            }
            else if (const_value <= 0xFF)
            {
                *ptcrypt = LA16_PTCRYPT_IMM8;
                *value   = const_value;
            }
            else
            {
                *ptcrypt = LA16_PTCRYPT_IMM;
                *value   = const_value;
            }
        }
    }
}

unsigned char la16_compiler_lowcodeline_ptcrypt_combo(unsigned char ptca,
                                                      unsigned char ptcb)
{
    unsigned char ptcrypt_combo = (ptca << 2) | ptcb;
    return ptcrypt_combo;
}

unsigned int la16_compiler_lowcodeline(const char *code_line, const char *scope, compiler_invocation_t *ci)
{
    char space = ' ';
    char pspace = ',';
    char nterm = '\0';

    char opcode_string[20] = {};
    char parameter_string[2][512] = {};

    unsigned char opcode = LA16_OPCODE_HLT;
    unsigned char mode = LA16_PTCRYPT_COMBO_NONE_NONE;
    unsigned char ptc[2] = {};
    unsigned short pv[2] = {};

    size_t len = strlen(code_line);
    size_t i = 0;

    // Opcode pass
    for(size_t off = i; i < len; i++)
    {
        if(code_line[i] == space)
        {
            i++;
            break;
        }
        opcode_string[i - off] = code_line[i];
    }

    opcode = opcode_from_string(opcode_string);

    if(opcode == 0xFF)
    {
        printf("[!] illegal opcode: %s\n", opcode_string);
        exit(1);
    }

    // Now Find out the parameters
    for(size_t param = 0; param < 2 && i < len; param++)
    {
        for(size_t off = 0; i < len; i++)
        {
            // Handle codeline special cases
            if(code_line[i] == space)
            {
                continue;
            }
            else if(code_line[i] == pspace)
            {
                i++;
                break;
            }
            else if(code_line[i] == nterm)
            {
                if(param == 0) param = 1;
                i++;
                break;
            }

            // If there is no special case we store it in param
            parameter_string[param][off++] = code_line[i];
        }
    }

    // Now decode parameters
    la16_compiler_lowcodeline_parameter_parser(parameter_string[0], scope, &ptc[0], &pv[0], ci);
    la16_compiler_lowcodeline_parameter_parser(parameter_string[1], scope, &ptc[1], &pv[1], ci);

    // Check if their valid
    for(unsigned char i = 0; i < 2; i++)
    {
        if(ptc[i] == LA16_PTCRYPT_ERR)
        {
            printf("[!] Parameter %d is unrecognised\n", i);
            exit(1);
        }
    }

    // Now combine them
    mode = la16_compiler_lowcodeline_ptcrypt_combo(ptc[0], ptc[1]);

    unsigned char a = 0b0;
    unsigned short b = 0b0;
    unsigned short c[2] = { 0b0, 0b0};
    unsigned char cn = 0;

    /* iterating through the raw mini modes */
    for(unsigned char i = 0; i < 2; i++)
    {
        if(ptc[i] == LA16_PTCRYPT_IMM)
        {
            b = pv[i];
        }
        else if(ptc[i] == LA16_PTCRYPT_IMM8)
        {
            c[cn++] = (unsigned char)(pv[i] & 0xFF);
        }
        else if(ptc[i] == LA16_PTCRYPT_REG)
        {
            if(i == 0 || ptc[0] == LA16_PTCRYPT_IMM || ptc[0] == LA16_PTCRYPT_IMM8)
            {
                a = pv[i];
            }
            else
            {
                b = pv[i];
            }
        }
    }

    return la16_compiler_machinecode(opcode, mode, a, b, c);
}

void la16_compiler_lowlevel(compiler_invocation_t *ci)
{
    // Holds current scope
    char *scope = NULL;

    // Iterating through tokens
    for(unsigned long i = 0; i < ci->token_cnt; i++)
    {
        // Checking for label
        if(ci->token[i].type == COMPILER_TOKEN_TYPE_LABEL)
        {
            // If we encounter a none scoped label it means its a new scope

            // Checking if scope is null
            if(scope != NULL)
            {
                free(scope);
            }

            size_t scope_size = strlen(ci->token[i].token);
            scope = malloc(scope_size);
            memcpy(scope, ci->token[i].token, scope_size - 1);
            scope[scope_size - 1] = '\0';
        }
        else if(ci->token[i].type == COMPILER_TOKEN_TYPE_ASM)
        {
            unsigned int instruction = la16_compiler_lowcodeline(ci->token[i].token, scope, ci);
            unsigned char *ibuf = (unsigned char*)&instruction;
            for(unsigned char i = 0; i < 4; i++)
            {
                ci->image[ci->image_uaddr + i] = ibuf[i];
            }
            ci->image_uaddr += 4;
        }
    }
}
