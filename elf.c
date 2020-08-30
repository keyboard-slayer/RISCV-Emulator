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
swap_bytes(struct ELF *elf)
{
    if (elf->name.e_ident.name.endianness > 1)
    {
        fprintf(stderr, "Invalid ELF Header !\n");
        exit(1);
    }


    if (elf->name.e_ident.name.endianness == 0)
    {
        return;
    }

    elf->name.e_ident.name.magic = __bswap_32(elf->name.e_ident.name.magic);
    elf->name.e_type = __bswap_16(elf->name.e_type);
    elf->name.e_machine = __bswap_16(elf->name.e_machine);
    elf->name.e_version = __bswap_32(elf->name.e_machine);
    elf->name.e_entry = __bswap_64(elf->name.e_entry);
    elf->name.e_phoff = __bswap_64(elf->name.e_phoff);
    elf->name.e_flags = __bswap_32(elf->name.e_flags);
    elf->name.e_ehsize = __bswap_16(elf->name.e_ehsize);
    elf->name.e_phentsize = __bswap_16(elf->name.e_phentsize);
    elf->name.e_phnum = __bswap_16(elf->name.e_phnum);
    elf->name.e_shentsize = __bswap_16(elf->name.e_shentsize);
    elf->name.e_shnum = __bswap_16(elf->name.e_shnum);
    elf->name.e_shstrndx = __bswap_16(elf->name.e_shstrndx);
}


struct ELF
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

    while (i < 64)
    {
        elf.name.e_ident.array[i++] = fgetc(fp);
    }

    swap_bytes(&elf);

    if (elf.name.e_ident.name.magic != 0x7f454c46)
    {
        printf("%s is not a valid ELF file\nMAGIC: 0x%x", filename,
               elf.name.e_ident.name.magic);
        exit(1);
    }

    if (elf.name.e_ident.name.abi != 0x03 && elf.name.e_ident.name.abi != 0x00)
    {
        printf("This emulator can only emulate Linux Binaries\n");
        exit(1);
    }


    if (elf.name.e_machine != 0xf300)
    {
        printf("%s is not a valid RISV binary\n", filename);
        exit(1);
    }

    fclose(fp);
    return elf;
}
