/* 			DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *                 Version 2, December 2004
 *
 *	Copyright (C) 2020 Jordan Dalcq
 *
 *	Everyone is permitted to copy and distribute verbatim or modified
 *	copies of this license document, and changing it is allowed as long
 *	as the name is changed.
 *		
 *	DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *	TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 *
 *  0. You just DO WHAT THE FUCK YOU WANT TO.
 */

#ifndef _RISCV_EMULATOR_CPU_H
#define _RISCV_EMULATOR_CPU_H

#include <stdint.h>

struct cpu
{
	uint64_t x[32];			/* Interger registers */
	double f[32]; 			/* Floating point registers */
	uint64_t pc;			/* Program counter */
};

enum 
{
	ZERO,
	RA,
	SP,
	GP,
	TP,
	T0,
	T1,
	T2,
	S1,
	A0,
	A1,
	A2,
	A3,
	A4,
	A5,
	A6,
	A7,
	S2,
	S3,
	S4,
	S5,
	S6,
	S7,
	S8,
	S9,
	S10,
	S11,
	T3,
	T4,
	T5,
	T6
};

enum 
{
	FT0,
	FT1,
	FT2,
	FT3,
	FT4,
	FT5,
	FT6,
	FT7,
	FS0,
	FS1,
	FS0,
	FS1,
	FA0,
	FA1,
	FS2,
	FS3,
	FS4,
	FS5,
	FS6,
	FS7,
	FS8,
	FS9,
	FS10,
	FS11,
	FT8,
	FT9,
	FT10,
	FT11
};

#endif