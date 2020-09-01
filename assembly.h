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

#ifndef _RISCV_EMULATOR_ASSEMBLY_H
#define _RISCV_EMULATOR_ASSEMBLY_H
#pragma GCC diagnostic ignored "-Wpedantic"

#include "cpu.h"
#include <stdint.h>

union OPCODE
{
    struct
    {
        uint32_t opcode:7;
        uint32_t rd:5;
        uint32_t funct3:3;
        uint32_t rs1:5;
        uint32_t rs2:5;
        uint32_t funct7:7;
    } R;

    struct
    {
        uint32_t opcode:7;
        uint32_t rd:5;
        uint32_t funct3:3;
        uint32_t rs1:5;
        uint32_t imm11_0:12;
    } I;

    struct
    {
        uint32_t opcode:7;
        uint32_t imm4_0:5;
        uint32_t funct3:3;
        uint32_t rs1:5;
        uint32_t rs2:5;
        uint32_t imm11_5:7;
    } S;

    struct
    {
        uint32_t opcode:7;
        uint32_t imm4_1_11:5;
        uint32_t funct3:3;
        uint32_t rs1:5;
        uint32_t rs2:5;
        uint32_t imm_12_10_5:7;
    } SB;

    struct
    {
        uint32_t opcode:7;
        uint32_t rd:5;
        uint32_t imm31_12:20;
    } U;

    struct
    {
        uint32_t opcode:7;
        uint32_t rd:5;
        uint32_t imm_20_10_1_11_19_12:20;
    } UJ;

    uint32_t opcode;
};

void interpret(struct CPU *, uint32_t);
void li(struct CPU *, uint32_t, uint32_t);
void addi(struct CPU *, uint32_t, uint32_t, uint32_t);
void aupic(struct CPU *, uint32_t, uint32_t);
void ecall(struct CPU *);

#endif
