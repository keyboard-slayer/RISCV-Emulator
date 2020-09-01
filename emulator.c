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

#include "emulator.h"
#include "assembly.h"
#include "elf.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

void
emulate(struct ELF elf, char *filename)
{
    size_t i;
    uint64_t file_size;
    uint32_t instructions;

    struct CPU cpu;
    struct PROG prog;
    struct SECTION *section;

    FILE *fp = fopen(filename, "rb");

    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);

    reset_cpu(&cpu, file_size);

    for (i = 0; i < elf.e_phnum; i++)
    {
        fseek(fp, elf.e_phoff + (i * elf.e_phentsize), SEEK_SET);
        fread(&prog, sizeof(struct PROG), 1, fp);

        if (prog.p_type == 0x1)
        {
            copy2ram(&cpu, fp, prog.p_offset, prog.p_filesz, prog.p_vaddr);
        }

        else
        {
            printf("0x%x is undefined !\n", prog.p_type);
            exit(1);
        }
    }

    section = (struct SECTION *) malloc(sizeof(struct SECTION) * elf.e_shnum);

    for (i = 0; i < elf.e_shnum; i++)
    {
        fseek(fp, elf.e_shoff + (i * elf.e_shentsize), SEEK_SET);
        fread(&section[i], sizeof(struct SECTION), 1, fp);

        if (section->sh_offset != 0)
        {
            copy2ram(&cpu, fp, section->sh_offset, section->sh_size, section->sh_addr);
        }
    }

    cpu.pc = elf.e_entry;
    while (true)
    {
        instructions =
            cpu.ram[cpu.pc] | cpu.ram[cpu.pc + 1] << 8 | cpu.ram[cpu.pc + 2] << 16 |
            cpu.ram[cpu.pc + 3] << 24;

        interpret(&cpu, instructions);
    }
}

int8_t
copy2ram(struct CPU *cpu, FILE * fp, uint64_t src, uint64_t size, uint64_t dst)
{
    size_t i;
    struct CPU cpu_snapshot;

    reset_cpu(&cpu_snapshot, cpu->ram_size - 0x1000000);
    memcpy(cpu_snapshot.ram, cpu->ram, cpu->ram_size);

    fseek(fp, src, SEEK_SET);
    fread(&cpu->ram[dst], size, 1, fp);

    for (i = dst; i < dst + size; i++)
    {
        if (cpu->ram[i] != cpu_snapshot.ram[i] && cpu_snapshot.ram[i] != 0)
        {
            printf("\nMEMORY OVERWRITTEN !\n");
            printf("Offset: 0x%lx\n", dst + i);
            printf("Before:\n");
            dump_ram_region(&cpu_snapshot, dst, size);
            printf("\nAfter:\n");
            dump_ram_region(cpu, dst, size);
            printf("\n");
            memcpy(cpu->ram, cpu_snapshot.ram, cpu->ram_size);
            exit(1);
        }
    }

    return 0;
}

#define MIN(__a, __b) (__a) < (__b) ? (__a) : (__b)
void
dump_ram_region(struct CPU *cpu, size_t start, size_t size)
{
    size_t i;

    while (start % 16)
    {
        start -= 1;
        size++;
    }


    printf("\nMemory dump start: 0x%08lx (%08ld) \n", start, start);
    printf("          ");

    for (i = 0; i < 16; i++)
    {
        printf("%02lx  ", i);
    }

    for (i = start; MIN(i < start + size, cpu->ram_size); i++)
    {
        if (i % 16 == 0)
        {
            printf("\n%08lx  ", i);
        }

        printf("%02x  ", cpu->ram[i]);
    }

    printf("\n");
}
