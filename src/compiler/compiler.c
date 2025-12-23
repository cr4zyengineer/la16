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

unsigned char la16_compiler_pc_inc_for_mode(unsigned char mode)
{
    switch(mode)
    {
        case LA16_CODING_COMBINATION_NONE:
        case LA16_CODING_COMBINATION_IMM6:
        case LA16_CODING_COMBINATION_REG:
            return 2;
        default:
            return 4;
    }
}

la16_compiler_machinecode_return_t la16_compiler_machinecode(la16_coder_instruction_layout_t instruction_layout)
{
    la16_compiler_machinecode_return_t retval;

    retval.instruction_size = la16_compiler_pc_inc_for_mode(instruction_layout.mode);

    unsigned char *instruction = ((unsigned char*)&retval.instruction);

    /* writing opcode */
    instruction[0] = ((instruction_layout.opcode & 0x7F) << 1) | ((instruction_layout.mode   & 0x04) >> 2);

    /* writing mode  */
    instruction[1] = (instruction_layout.mode & 0x03) << 6;

    /* now compiling the rest according to mode */
    switch(instruction_layout.mode)
    {
        case LA16_CODING_COMBINATION_NONE:
            /* nothing to encode here */
            break;
        case LA16_CODING_COMBINATION_IMM6:
        {
            instruction[1] |= (instruction_layout.res.imm6[0] & 0x3F);
            break;
        }
        case LA16_CODING_COMBINATION_REG:
        {
            instruction[1] |= (instruction_layout.res.imm6[0] & 0x3F);
            break;
        }
        case LA16_CODING_COMBINATION_REG_REG:
        {
            instruction[1] |= (instruction_layout.res.imm6[0] & 0x3F);
            instruction[2] = instruction_layout.res.imm6[1];
            break;
        }
        case LA16_CODING_COMBINATION_IMM16:
        {
            *((unsigned short*)&instruction[2]) = instruction_layout.res.imm16;
            break;
        }
        case LA16_CODING_COMBINATION_REG_IMM16:
        case LA16_CODING_COMBINATION_IMM16_REG:
        {
            *((unsigned short*)&instruction[2]) = instruction_layout.res.imm16;
            instruction[1] |= (instruction_layout.res.imm6[0] & 0x3F);
            break;
        }
        case LA16_CODING_COMBINATION_IMM11_IMM11:
        {
            /* fuck my life actually */
            break;
        }
        default:
            printf("fuck my life\n");
            exit(1);
    }

    return retval;
}

unsigned char la16_compiler_lowcodeline_opcode_parse(const char *opcode_string)
{
    if(strcmp(opcode_string, "hlt") == 0) return LA16_OPCODE_HLT;
    else if(strcmp(opcode_string, "nop") == 0) return LA16_OPCODE_NOP;
    else if(strcmp(opcode_string, "in") == 0) return LA16_OPCODE_IN;
    else if(strcmp(opcode_string, "out") == 0) return LA16_OPCODE_OUT;
    else if(strcmp(opcode_string, "ldb") == 0) return LA16_OPCODE_LDB;
    else if(strcmp(opcode_string, "stb") == 0) return LA16_OPCODE_STB;
    else if(strcmp(opcode_string, "ldw") == 0) return LA16_OPCODE_LDW;
    else if(strcmp(opcode_string, "stw") == 0) return LA16_OPCODE_STW;
    else if(strcmp(opcode_string, "casb") == 0) return LA16_OPCODE_CASB;
    else if(strcmp(opcode_string, "casw") == 0) return LA16_OPCODE_CASW;
    else if(strcmp(opcode_string, "faab") == 0) return LA16_OPCODE_FAAB;
    else if(strcmp(opcode_string, "faaw") == 0) return LA16_OPCODE_FAAW;
    else if(strcmp(opcode_string, "fence") == 0) return LA16_OPCODE_FENCE;
    else if(strcmp(opcode_string, "mov") == 0) return LA16_OPCODE_MOV;
    else if(strcmp(opcode_string, "swp") == 0) return LA16_OPCODE_SWP;
    else if(strcmp(opcode_string, "swpz") == 0) return LA16_OPCODE_SWPZ;
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
    else if(strcmp(opcode_string, "neg") == 0) return LA16_OPCODE_NEG;
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
    else if(strcmp(opcode_string, "ret") == 0) return LA16_OPCODE_RET;
    else if(strcmp(opcode_string, "int") == 0) return LA16_OPCODE_INT;
    else if(strcmp(opcode_string, "intset") == 0) return LA16_OPCODE_INTSET;
    else if(strcmp(opcode_string, "intret") == 0) return LA16_OPCODE_INTRET;
    else if(strcmp(opcode_string, "ppcnt") == 0) return LA16_OPCODE_PPCNT;
    else if(strcmp(opcode_string, "ppktrrset") == 0) return LA16_OPCODE_PPKTRRSET;
    else if(strcmp(opcode_string, "vpset") == 0) return LA16_OPCODE_VPSET;
    else if(strcmp(opcode_string, "vpget") == 0) return LA16_OPCODE_VPGET;
    else if(strcmp(opcode_string, "vpflgset") == 0) return LA16_OPCODE_VPFLGSET;
    else if(strcmp(opcode_string, "vplflgget") == 0) return LA16_OPCODE_VPFLGGET;
    else if(strcmp(opcode_string, "vpaddr") == 0) return LA16_OPCODE_HLT;
    else return 0xFF;
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
    /**ptcrypt = LA16_PTCRYPT_ERR;

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
        if (addr != 0xFFFF)
        {
            *ptcrypt = LA16_PTCRYPT_IMM;
            *value = addr;
        }
        else
        {
            exit(1);
        }
    }*/
}

unsigned char la16_compiler_lowcodeline_ptcrypt_combo(unsigned char ptca,
                                                      unsigned char ptcb)
{
    unsigned char ptcrypt_combo = (ptca << 2) | ptcb;
    return ptcrypt_combo;
}

unsigned int la16_compiler_lowcodeline(const char *code_line, const char *scope, compiler_invocation_t *ci)
{
    /*char space = ' ';
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
    unsigned short c[2] = { 0b0, 0b0};
    unsigned char cn = 0;

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

    return la16_compiler_machinecode(opcode, mode, a, b, c);*/
    return 0;
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
