/*
 * DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE Version 2, December 2004 Copyright (C)
 * 2020 Jordan Dalcq Everyone is permitted to copy and distribute verbatim or modified
 * copies of this license document, and changing it is allowed as long as the name is
 * changed.  DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE TERMS AND CONDITIONS FOR
 * COPYING, DISTRIBUTION AND MODIFICATION 0. You just DO WHAT THE FUCK YOU WANT TO. 
 */

#include "elf.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stddef.h>
#include <string.h>
#include <byteswap.h>

void
open_elf(char *filename)
{
    size_t i;
    struct ELF elf;
    FILE *fp = fopen(filename, "rb");

    if (fp == NULL)
    {
        fprintf(stderr, "%s\n", strerror(errno));
        exit(1);
    }

    while (i < 8)
    {
        elf.e_ident.array[i++] = fgetc(fp);
    }

    if ((elf.e_ident.name.endianness > 1)
        || (elf.e_ident.name.endianness == 1
            && __bswap_32(elf.e_ident.name.magic) != 0x7f454c46)
        || (elf.e_ident.name.endianness == 0 && elf.e_ident.name.magic != 0x7f454c46))
    {
        printf("%s is not a valid ELF file\nMAGIC: 0x%x", filename,
               __bswap_32(elf.e_ident.name.magic));
        exit(1);
    }

    printf("%x", elf.e_ident.name.endianness);
}
