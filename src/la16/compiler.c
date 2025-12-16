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

#include <la16/compiler.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <compiler/parse.h>

unsigned int la16_compiler_machinecode(unsigned char opcode,
                                       unsigned char mode,
                                       unsigned char a,
                                       unsigned short b)
{
    unsigned int instruction;

    ((unsigned char*)&instruction)[0] = opcode;
    ((unsigned char*)&instruction)[1] = (mode << 4) | a;
    ((unsigned short*)&instruction)[1] = b;

    return instruction;
}

unsigned char la16_compiler_lowcodeline_opcode_parse(const char *opcode_string)
{
    if(strcmp(opcode_string, "hlt") == 0) return LA16_OPCODE_HLT;
    else if(strcmp(opcode_string, "mov") == 0) return LA16_OPCODE_MOV;
    else if(strcmp(opcode_string, "mld") == 0) return LA16_OPCODE_MLD;
    else if(strcmp(opcode_string, "mst") == 0) return LA16_OPCODE_MST;
    else if(strcmp(opcode_string, "in") == 0) return LA16_OPCODE_IN;
    else if(strcmp(opcode_string, "out") == 0) return LA16_OPCODE_OUT;
    else if(strcmp(opcode_string, "push") == 0) return LA16_OPCODE_PUSH;
    else if(strcmp(opcode_string, "pop") == 0) return LA16_OPCODE_POP;
    else if(strcmp(opcode_string, "add") == 0) return LA16_OPCODE_ADD;
    else if(strcmp(opcode_string, "sub") == 0) return LA16_OPCODE_SUB;
    else if(strcmp(opcode_string, "mul") == 0) return LA16_OPCODE_MUL;
    else if(strcmp(opcode_string, "div") == 0) return LA16_OPCODE_DIV;
    else if(strcmp(opcode_string, "idiv") == 0) return LA16_OPCODE_IDIV;
    else if(strcmp(opcode_string, "inc") == 0) return LA16_OPCODE_INC;
    else if(strcmp(opcode_string, "dec") == 0) return LA16_OPCODE_DEC;
    else if(strcmp(opcode_string, "not") == 0) return LA16_OPCODE_NOT;
    else if(strcmp(opcode_string, "and") == 0) return LA16_OPCODE_AND;
    else if(strcmp(opcode_string, "or") == 0) return LA16_OPCODE_OR;
    else if(strcmp(opcode_string, "xor") == 0) return LA16_OPCODE_XOR;
    else if(strcmp(opcode_string, "shr") == 0) return LA16_OPCODE_SHR;
    else if(strcmp(opcode_string, "shl") == 0) return LA16_OPCODE_SHL;
    else if(strcmp(opcode_string, "ror") == 0) return LA16_OPCODE_ROR;
    else if(strcmp(opcode_string, "rol") == 0) return LA16_OPCODE_ROL;
    else if(strcmp(opcode_string, "jmp") == 0) return LA16_OPCODE_JMP;
    else if(strcmp(opcode_string, "cmp") == 0) return LA16_OPCODE_CMP;
    else if(strcmp(opcode_string, "je") == 0) return LA16_OPCODE_JE;
    else if(strcmp(opcode_string, "jne") == 0) return LA16_OPCODE_JNE;
    else if(strcmp(opcode_string, "jlt") == 0) return LA16_OPCODE_JLT;
    else if(strcmp(opcode_string, "jgt") == 0) return LA16_OPCODE_JGT;
    else if(strcmp(opcode_string, "bl") == 0) return LA16_OPCODE_BL;
    else if(strcmp(opcode_string, "ble") == 0) return LA16_OPCODE_BLE;
    else if(strcmp(opcode_string, "blne") == 0) return LA16_OPCODE_BLNE;
    else if(strcmp(opcode_string, "bllt") == 0) return LA16_OPCODE_BLLT;
    else if(strcmp(opcode_string, "blgt") == 0) return LA16_OPCODE_BLGT;
    else if(strcmp(opcode_string, "ret") == 0) return LA16_OPCODE_RET;
    else if(strcmp(opcode_string, "int") == 0) return LA16_OPCODE_INT;
    else if(strcmp(opcode_string, "intset") == 0) return LA16_OPCODE_INTSET;
    else if(strcmp(opcode_string, "intclear") == 0) return LA16_OPCODE_INTCLEAR;
    else if(strcmp(opcode_string, "intret") == 0) return LA16_OPCODE_INTRET;
    else if(strcmp(opcode_string, "mpagemap") == 0) return LA16_OPCODE_MPAGEMAP;
    else if(strcmp(opcode_string, "mpageunmap") == 0) return LA16_OPCODE_MPAGEUNMAP;
    else if(strcmp(opcode_string, "mpageunmapall") == 0) return LA16_OPCODE_MPAGEUNMAPALL;
    else if(strcmp(opcode_string, "mpageprot") == 0) return LA16_OPCODE_MPAGEPROT;
    else if(strcmp(opcode_string, "mpageclear") == 0) return LA16_OPCODE_MPAGECLEAR;
    else if(strcmp(opcode_string, "mpageaval") == 0) return LA16_OPCODE_MPAGEAVAL;
    else if(strcmp(opcode_string, "mpagecount") == 0) return LA16_OPCODE_MPAGECOUNT;
    else if(strcmp(opcode_string, "maddr") == 0) return LA16_OPCODE_MADDR;
    else if(strcmp(opcode_string, "crresume") == 0) return LA16_OPCODE_CRRESUME;
    else if(strcmp(opcode_string, "crstop") == 0) return LA16_OPCODE_CRSTOP;
    else if(strcmp(opcode_string, "crdump") == 0) return LA16_OPCODE_CRDUMP;
    else if(strcmp(opcode_string, "crflash") == 0) return LA16_OPCODE_CRFLASH;
    else if(strcmp(opcode_string, "crtimeset") == 0) return LA16_OPCODE_CRTIMESET;
    else if(strcmp(opcode_string, "crctxhndl") == 0) return LA16_OPCODE_CRCTXHNDLSET;
    else if(strcmp(opcode_string, "crexchndl") == 0) return LA16_OPCODE_CREXCHNDLSET;
    else if(strcmp(opcode_string, "ktrrset") == 0) return LA16_OPCODE_KTRRSET;
    else return 0xFF;
}

enum LA16_PTCRYPT
{
    LA16_PTCRYPT_NONE = 0b000,
    LA16_PTCRYPT_REG  = 0b001,
    LA16_PTCRYPT_IMM  = 0b010,
    LA16_PTCRYPT_ADDR = 0b011,
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
    if(parameter_size == 2)
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
                    if(parameter[2] == 'b')
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
        *ptcrypt = LA16_PTCRYPT_IMM;
        *value = pr.value;
    }
    else
    {


        unsigned int addr = label_lookup(ci, parameter, scope);
        if (addr != 0xFFFF)
        {
            *ptcrypt = LA16_PTCRYPT_IMM;
            *value = addr;
        }
        else
        {
            exit(1);
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

    opcode = la16_compiler_lowcodeline_opcode_parse(opcode_string);

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

    for(unsigned char i = 0; i < 2; i++)
    {
        if(ptc[i] == LA16_PTCRYPT_IMM | ptc[i] == LA16_PTCRYPT_ADDR)
        {
            b = pv[i];
        }
        else if(ptc[i] == LA16_PTCRYPT_REG)
        {
            if(i == 0 || ptc[0] == LA16_PTCRYPT_IMM || ptc[0] == LA16_PTCRYPT_ADDR)
            {
                a = pv[i];
            }
            else
            {
                b = pv[i];
            }
        }
    }

    return la16_compiler_machinecode(opcode, mode, a, b);
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
