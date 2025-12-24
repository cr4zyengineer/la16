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

unsigned char la16_mode_create_from_codings(unsigned char a, unsigned char b)
{
    switch(a)
    {
        case LA16_CODING_REG:
            switch(b)
            {
                case LA16_CODING_NONE:
                    return LA16_PARAMETER_CODING_COMBINATION_REG;
                case LA16_CODING_REG:
                    return LA16_PARAMETER_CODING_COMBINATION_REG_REG;
                case LA16_CODING_IMM:
                    return LA16_PARAMETER_CODING_COMBINATION_REG_IMM16;
            }
            return LA16_PARAMETER_CODING_COMBINATION_NONE;
        case LA16_CODING_IMM:
            switch(b)
            {
                case LA16_CODING_NONE:
                    return LA16_PARAMETER_CODING_COMBINATION_IMM16;
                case LA16_CODING_REG:
                    return LA16_PARAMETER_CODING_COMBINATION_IMM16_REG;
                case LA16_CODING_IMM:
                    return LA16_PARAMETER_CODING_COMBINATION_IMM8_IMM8;
            }
            return LA16_PARAMETER_CODING_COMBINATION_NONE;
        default:
            return LA16_PARAMETER_CODING_COMBINATION_NONE;
    }
}

unsigned int la16_compiler_machinecode(la16_compiler_instruction_t *cinstr)
{
    unsigned int instruction;
    unsigned char *ibuf = (unsigned char*)&instruction;
    unsigned short *ibuw = (unsigned short*)&instruction;

    /* inserting opcode */
    ibuf[0] = cinstr->opcode;

    /* inserting mode */
    ibuf[1] = (cinstr->mode << 5);

    /* now here we go */
    switch(cinstr->mode)
    {
        case LA16_PARAMETER_CODING_COMBINATION_NONE:
            break;
        case LA16_PARAMETER_CODING_COMBINATION_REG:
            if(cinstr->arg[0] > 0b00011111)
            {
                printf("[!] illegal register\n");
                exit(1);
            }
            ibuf[1] |= (unsigned char)cinstr->arg[0];
            break;
        case LA16_PARAMETER_CODING_COMBINATION_REG_REG:
            if(cinstr->arg[0] > 0b00011111 || cinstr->arg[1] > 0b00011111)
            {
                printf("[!] illegal register\n");
                exit(1);
            }
            ibuf[1] |= (unsigned char)cinstr->arg[0];
            ibuf[2] = (unsigned char)cinstr->arg[1];
            break;
        case LA16_PARAMETER_CODING_COMBINATION_IMM16:
            ibuw[1] = cinstr->arg[0];
            break;
        case LA16_PARAMETER_CODING_COMBINATION_IMM16_REG:
            if(cinstr->arg[1] > 0b00011111)
            {
                printf("[!] illegal register\n");
                exit(1);
            }
            ibuf[1] |= (unsigned char)cinstr->arg[1];
            ibuw[1] = cinstr->arg[0];
            break;
        case LA16_PARAMETER_CODING_COMBINATION_REG_IMM16:
            if(cinstr->arg[0] > 0b00011111)
            {
                printf("[!] illegal register\n");
                exit(1);
            }
            ibuf[1] |= (unsigned char)cinstr->arg[0];
            ibuw[1] = cinstr->arg[1];
            break;
        case LA16_PARAMETER_CODING_COMBINATION_IMM8_IMM8:
            if(cinstr->arg[0] > 0b11111111 || cinstr->arg[1] > 0b11111111)
            {
                printf("[!] illegal 8bit intermediate\n");
                exit(1);
            }
            ibuf[2] = (unsigned char)cinstr->arg[0];
            ibuf[3] = (unsigned char)cinstr->arg[1];
            break;
        default:
            printf("[!] illegal mode: 0x%x\n", cinstr->mode);
            exit(1);
    }

    return instruction;
}

void la16_compiler_lowcodeline_parameter_parser(const char *parameter,
                                                const char *scope,
                                                unsigned char *ptcrypt,
                                                unsigned short *value,
                                                compiler_invocation_t *ci)
{
    *ptcrypt = LA16_CODING_ERR;

    if(parameter[0] == '\0')
    {
        *ptcrypt = LA16_CODING_NONE;
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
                        *ptcrypt = LA16_CODING_REG;
                        *value = (parameter[1] - '0') + LA16_REGISTER_R0;
                        return;
                    case 'r':
                        *ptcrypt = LA16_CODING_REG;
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
                        *ptcrypt = LA16_CODING_REG;
                        *value = LA16_REGISTER_ELB;
                        return;
                    }
                    *ptcrypt = LA16_CODING_REG;
                    *value = LA16_REGISTER_EL;
                    return;
                }
                break;
            }
            case 'p':
            {
                if(parameter[1] == 'c')
                {
                    *ptcrypt = LA16_CODING_REG;
                    *value = LA16_REGISTER_PC;
                    return;
                }
                break;
            }
            case 's':
                if(parameter[1] == 'p')
                {
                    *ptcrypt = LA16_CODING_REG;
                    *value = LA16_REGISTER_SP;
                    return;
                }
                break;
            case 'f':
                if(parameter[1] == 'p')
                {
                    *ptcrypt = LA16_CODING_REG;
                    *value = LA16_REGISTER_FP;
                    return;
                }
                break;
            case 'c':
                if(parameter[1] == 'f')
                {
                    *ptcrypt = LA16_CODING_REG;
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
        {
            *ptcrypt = LA16_CODING_IMM;
            *value   = pr.value;
        }
    }
    else
    {
        unsigned int addr = label_lookup(ci, parameter, scope);
        if (addr != COMPILER_LABEL_NOT_FOUND)
        {
            *ptcrypt = LA16_CODING_IMM;
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
            *ptcrypt = LA16_CODING_IMM;
            *value   = const_value;
        }
    }
}

unsigned int la16_compiler_lowcodeline(const char *code_line, const char *scope, compiler_invocation_t *ci)
{
    char space = ' ';
    char pspace = ',';
    char nterm = '\0';

    char opcode_string[20] = {};
    char parameter_string[2][512] = {};

    unsigned char opcode = LA16_OPCODE_HLT;
    unsigned char mode = LA16_PARAMETER_CODING_COMBINATION_NONE;
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
        if(ptc[i] == LA16_CODING_ERR)
        {
            printf("[!] Parameter %d is unrecognised\n", i);
            exit(1);
        }
    }

    // Now combine them
    la16_compiler_instruction_t cinstr;
    unsigned char cn = 0;

    /* write opcode */
    cinstr.opcode = opcode;

    /* combine both modes into the real instruction mode */
    cinstr.mode = la16_mode_create_from_codings(ptc[0], ptc[1]);

    /* iterating through the raw mini modes */
    for(unsigned char i = 0; i < 2; i++)
    {
        cinstr.arg[i] = pv[i];
    }

    return la16_compiler_machinecode(&cinstr);
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
