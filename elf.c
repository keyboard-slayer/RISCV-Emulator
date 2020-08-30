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

#include "elf.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stddef.h>
#include <string.h>
#include <byteswap.h>
#include <unistd.h>

void
swap_bytes(struct ELF *elf)
{
    if (elf->endianness > 2 || elf->endianness > 1)
    {
        fprintf(stderr, "Invalid ELF endianness: %d !\n", elf->endianness);
        exit(1);
    }


    if (elf->endianness == 2)
    {
        return;
    }

    elf->magic = __bswap_32(elf->magic);
    elf->e_type = __bswap_16(elf->e_type);
    elf->e_machine = __bswap_16(elf->e_machine);
    elf->e_version = __bswap_32(elf->e_machine);
    elf->e_entry = __bswap_64(elf->e_entry);
    elf->e_phoff = __bswap_64(elf->e_phoff);
    elf->e_flags = __bswap_32(elf->e_flags);
    elf->e_ehsize = __bswap_16(elf->e_ehsize);
    elf->e_phentsize = __bswap_16(elf->e_phentsize);
    elf->e_phnum = __bswap_16(elf->e_phnum);
    elf->e_shentsize = __bswap_16(elf->e_shentsize);
    elf->e_shnum = __bswap_16(elf->e_shnum);
    elf->e_shstrndx = __bswap_16(elf->e_shstrndx);
}


struct ELF
open_elf(char *filename)
{
    struct ELF elf;
    FILE *fp = fopen(filename, "rb");

    if (fp == NULL)
    {
        fprintf(stderr, "%s\n", strerror(errno));
        exit(1);
    }


    fread(&elf, sizeof(struct ELF), 1, fp);
    swap_bytes(&elf);

    if (elf.magic != 0x7f454c46)
    {
        printf("%s is not a valid ELF file\nMAGIC: 0x%x", filename,
               elf.magic);
        exit(1);
    }

    if (elf.abi != 0x03 && elf.abi != 0x00)
    {
        printf("This emulator can only emulate Linux Binaries\n");
        exit(1);
    }


    if (elf.e_machine != 0xf300)
    {
        printf("%s is not a valid RISV binary\n", filename);
        exit(1);
    }

    fclose(fp);
    return elf;
}
