/*
 * DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE Version 2, December 2004 Copyright (C)
 * 2020 Jordan Dalcq Everyone is permitted to copy and distribute verbatim or modified
 * copies of this license document, and changing it is allowed as long as the name is
 * changed.  DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE TERMS AND CONDITIONS FOR
 * COPYING, DISTRIBUTION AND MODIFICATION 0. You just DO WHAT THE FUCK YOU WANT TO. 
 */

#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "elf.h"

int
main(int argc, char *argv[])
{
    struct ELF elf;

    if (argc == 1)
    {
        fprintf(stderr, "Please specify a RISCV binary file\n");
        exit(1);
    }

    elf = open_elf(argv[1]);
    printf("0x%x\n", elf.name.e_ident.name.magic);

    return 1;
}
