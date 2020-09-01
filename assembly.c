/*
 * Copyright (c) 2020 Jordan Dalcq
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of mosquitto nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "assembly.h"
#include "emulator.h"
#include "table-opcode.h"

#include <stdio.h>
#include <byteswap.h>
#include <stdlib.h>
#include <stdbool.h>

#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <string.h>

extern int errno;

void
li(struct CPU *cpu, uint32_t rd, uint32_t imm)
{
    cpu->x[rd] = imm;
}


void
addi(struct CPU *cpu, uint32_t rd, uint32_t rs1, uint32_t imm)
{
    cpu->x[rd] = cpu->x[rs1] + imm;
}

void
aupic(struct CPU *cpu, uint32_t rd, uint32_t imm)
{
    cpu->x[rd] = cpu->pc + (imm << 12);
}

void
ecall(struct CPU *cpu)
{
    uint32_t real_syscall;
    uint32_t syscall_nbr = cpu->x[17];

    char *buffer;

    uintptr_t a0 = cpu->x[10];
    uintptr_t a1 = cpu->x[11];
    uintptr_t a2 = cpu->x[12];
    uintptr_t a3 = cpu->x[13];
    uintptr_t a4 = cpu->x[14];
    uintptr_t a5 = cpu->x[15];

    switch (syscall_nbr)
    {
        case 64:
            real_syscall = SYS_write;
            buffer = (char *) malloc(sizeof(char) * a2);
            memcpy(buffer, cpu->ram + a1, a2);

            a1 = (uintptr_t) buffer;

            break;
        case 93:
            real_syscall = SYS_exit;
            break;
        default:
            printf("NOT IMPLEMENTED: %d !\n", syscall_nbr);
            exit(1);
    }

    if (syscall(real_syscall, a0, a1, a2, a3, a4, a5) == -1)
    {
        printf("\nSYSCALL 0x%08x (%d) ERROR: %s\n", syscall_nbr, syscall_nbr,
               strerror(errno));
        printf
            ("REGISTERS VALUE:\n\ta0: 0x%08lx (%ld)\ta1: 0x%08lx (%ld)\ta2: 0x%08lx (%ld)\n\ta3: 0x%08lx (%ld)\ta4: 0x%08lx (%ld)\ta5: 0x%08lx (%ld)\n\n",
             a0, a0, a1, a1, a2, a2, a3, a3, a4, a4, a5, a5);
        exit(1);
    }

    free(buffer);
}

void
interpret(struct CPU *cpu, uint32_t instructions)
{
    union OPCODE opcode;
    bool print_end = true;

    opcode.opcode = instructions;

    printf("\033[90m");

    switch (opcode.I.opcode)
    {
        case 0x13:
            switch (opcode.I.funct3)
            {
                case 0:
                    if (opcode.I.rs1 == 0)
                    {
                        printf("li x%d, %d\n", opcode.I.rd, opcode.I.imm11_0);
                        li(cpu, opcode.I.rd, opcode.I.imm11_0);
                        print_end = false;
                    }

                    else
                    {
                        printf("addi");
                        addi(cpu, opcode.I.rd, opcode.I.rs1, opcode.I.imm11_0);
                    }

                    break;
                case 1:
                    printf("slli");
                    break;
                case 2:
                    printf("slti");
                    break;
                case 3:
                    printf("sltiu");
                    break;
                case 4:
                    printf("xori");
                    break;
                case 5:
                    if (opcode.I.imm11_0 == 0)
                    {
                        printf("srli");
                    }

                    else if (opcode.I.imm11_0 == 0x20)
                    {
                        printf("srai");
                    }

                    else
                    {
                        printf("INVALID OPCODE !\n");
                    }

                    break;
                case 6:
                    printf("ori");
                    break;
                case 7:
                    printf("andi");
                    break;
            }

            if (print_end)
            {
                printf(" x%d, x%d, %d\n", opcode.I.rd, opcode.I.rs1, opcode.I.imm11_0);
            }

            break;

        case 0x17:
            printf("auipc x%d, %d\n", opcode.U.rd, opcode.U.imm31_12);
            aupic(cpu, opcode.U.rd, opcode.U.imm31_12);
            break;

        case 0x73:

            if (opcode.I.imm11_0 == 0)
            {
                printf("ecall\033[39m\n");
                ecall(cpu);
            }

            else if (opcode.I.imm11_0 == 1)
            {
                printf("ebreak\n");
            }

            else
            {
                printf("INVALID OPCODE !\n");
            }

            break;
        
        default:
            printf("The family opcode 0x%x is not implemented\n", opcode.I.opcode);
            /*
             * exit(1); 
             */
        printf("\033[39m");
    }

    cpu->pc += 4;

}
