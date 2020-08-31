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
#include "elf.h"
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

void
emulate(struct ELF elf, char *filename)
{
    size_t i;
    struct CPU cpu;
    struct PROG prog;
    uint64_t file_size;

    FILE *fp = fopen(filename, "rb");

    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);

    reset_cpu(&cpu, file_size);
    
    for(i = 0; i < elf.e_phnum; i++)
    {
        fseek(fp, elf.e_phoff + (i * elf.e_phentsize), SEEK_SET);
        fread(&prog, 1, sizeof(struct PROG), fp);

        if(prog.p_type == 0x1)
        {
            printf("%lx %lx %lx\n", prog.p_offset, prog.p_filesz, prog.p_vaddr);
            copy2ram(&cpu, fp, prog.p_offset, prog.p_filesz, prog.p_vaddr);
        }

    }
    
    dump_ram(&cpu); 
}

void 
copy2ram(struct CPU *cpu, FILE *fp, uint64_t src, uint64_t size, uint64_t dst)
{
    fseek(fp, src, SEEK_SET);
    printf("%ld == %ld\n", fread(&cpu->ram[dst],size, 1,  fp), size);
}


void 
dump_ram(struct CPU *cpu)
{
    size_t i;

    printf("          ");
    for(i = 0; i < 16; i++)
    {
        printf("%02lx ", i);
    }

    for(i = 0; i < cpu->ram_size; i++)
    {
        if(i % 16 == 0)
        {
            printf("\n%08lx  ", i);
        }

        printf("%02x ", cpu->ram[i]);
    }

}