/*
 * DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE Version 2, December 2004 Copyright (C)
 * 2020 Jordan Dalcq Everyone is permitted to copy and distribute verbatim or modified
 * copies of this license document, and changing it is allowed as long as the name is
 * changed.  DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE TERMS AND CONDITIONS FOR
 * COPYING, DISTRIBUTION AND MODIFICATION 0. You just DO WHAT THE FUCK YOU WANT TO. 
 */



#ifndef _RISCV_EMULATOR_ELF_H
#define _RISCV_EMULATOR_ELF_H

#include <stdint.h>
union IDENT
{
    struct IDENT_NAME
    {
        uint32_t magic;         /* The ELF magic number */
        uint8_t bits;           /* 32 bits (1) or 64 bits (2) */
        uint8_t endianness;     /* little (1) or bit(2) */
        uint8_t elf_bit;
        uint8_t abi;
        uint8_t abiver;
        uint8_t pad[7];
    } name;

    uint8_t array[16];
};

struct ELF
{
    struct
    {
        union IDENT e_ident;
        uint16_t e_type;
        uint16_t e_machine;
        uint32_t e_version;
        uint64_t e_entry;
        uint64_t e_phoff;
        uint64_t e_shoff;
        uint32_t e_flags;
        uint16_t e_ehsize;
        uint16_t e_phentsize;
        uint16_t e_phnum;
        uint16_t e_shentsize;
        uint16_t e_shnum;
        uint16_t e_shstrndx;
    } name;

    uint8_t array[64];
};

struct ELF open_elf(char *);
void swap_bytes(struct ELF *);

#endif
