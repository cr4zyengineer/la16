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
#include <unistd.h>
#include <string.h>
#include <compiler/compile.h>
#include <compiler/compiler.h>
#include <la16/machine.h>

void print_usage(int argc, char **argv)
{
    /* checking if we have atleast one arg to print the usage */
    if(argc >= 1)
    {
        fprintf(stderr, "Usage: %s\n\t-c <l16 files> : compiling a la16 boot image out of la16 assembly files\n\t-r <image file> : running a image file\n", argv[0]);
    }
}

int main(int argc, char *argv[])
{
    /* checking for sufficient arguments */
    if(argc < 2)
    {
        /* printing usage */
        print_usage(argc, argv);

        /* returning 1 because its a failure */
        return 1;
    }

    /* checking if its compilation */
    if(strcmp(argv[1], "-c") == 0)
    {
        /* gettu*/
        char **files = calloc(sizeof(char*), argc - 2);
        for(int i = 0; i < (argc - 2); i++)
        {
            files[i] = strdup(argv[i + 2]);
        }
        compile_files(files, argc - 2);
        for(int i = 0; i < (argc - 2); i++)
        {
            free(files[i]);
        }
        free(files);
    }

    /* checking if its running */
    else if(strcmp(argv[1], "-r") == 0)
    {
        /* creating new la16 virtual machine */
        la16_machine_t *machine = la16_machine_alloc(0xFFFF);

        /* loading boot image into memory of virtual machine */
        la16_memory_load_image(machine->memory, argv[2]);

        /*
         * getting entry point of boot image of virtual machine
         * and setting program pointer of first core to it
         */
        *(machine->core[0]->pc) = *((la16_memory_address_t*)&machine->memory->memory[0x00]);

        /* setting stack pointer of  */
        *(machine->core[0]->sp) = 0xFFFD;

        /* executing virtual machines 1st core TODO: Implement threading */
        la16_core_execute(machine->core[0]);

        /* deallocating machine */
        la16_machine_dealloc(machine);
    }

    /* nothing was matched */
    else
    {
        /* creating new la16 virtual machine */
        la16_machine_t *machine = la16_machine_alloc(0xFFFF);

        /* loading boot image into memory of virtual machine */
        //la16_memory_load_image(machine->memory, argv[2]);

        /* setting _start entry to 0x00 */
       //*((la16_memory_address_t*)&machine->memory->memory[0x00]) = 0x0002;

        la16_coder_instruction_layout_t layout;
        layout.opcode = LA16_OPCODE_ADD;
        layout.mode = LA16_CODING_COMBINATION_REG_REG;
        layout.res.imm6[0] = LA16_REGISTER_R0;
        layout.res.imm6[1] = LA16_REGISTER_R1;

        la16_compiler_machinecode_return_t mr = la16_compiler_machinecode(layout);

        /* insert instruction after entry*/
        *((unsigned int*)&machine->memory->memory[0x00]) = mr.instruction;
        *((unsigned int*)&machine->memory->memory[0x04]) = mr.instruction;

        layout.opcode = LA16_OPCODE_NOP;
        layout.mode = LA16_CODING_COMBINATION_NONE;
        la16_compiler_machinecode_return_t nr = la16_compiler_machinecode(layout);
        *((unsigned int*)&machine->memory->memory[0x08]) = nr.instruction;

        *((unsigned int*)&machine->memory->memory[0x0a]) = mr.instruction;

        /*for(unsigned char i = 0; i < 5; i++)
        {
            printf("%d\n", machine->memory->memory[i]);
        }*/

        /*
         * getting entry point of boot image of virtual machine
         * and setting program pointer of first core to it
         */
        *(machine->core[0]->pc) = 0x00;

        /* setting stack pointer of  */
        *(machine->core[0]->sp) = 0xFFFD;

        /* executing virtual machines 1st core TODO: Implement threading */
        la16_core_execute(machine->core[0]);

        /* deallocating machine */
        la16_machine_dealloc(machine);

        /* printing usage */
        //print_usage(argc, argv);

        return 1;
    }

    return 0;
}
