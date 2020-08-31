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

    fread(&elf, 1, sizeof(struct ELF), fp);

    if (elf.e_ident[0] != 0x7f && elf.e_ident[1] != 'E' && elf.e_ident[2] != 'L'
        && elf.e_ident[3] != 'F')
    {
        printf("%s is not a valid ELF file\nMAGIC: 0x%x%x%x%x", filename,
               elf.e_ident[0], elf.e_ident[1], elf.e_ident[2], elf.e_ident[3]);
        exit(1);
    }

    if (elf.e_ident[7] != 0x03 && elf.e_ident[7] != 0x00)
    {
        printf("This emulator can only emulate Linux Binaries\n");
        exit(1);
    }


    if (elf.e_machine != 0xf3)
    {
        printf("%s is not a valid RISV binary\n", filename);
        exit(1);
    }

    fclose(fp);
    return elf;
}
