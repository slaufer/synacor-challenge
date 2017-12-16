#ifndef _STRUCT_H
#define _STRUCT_H

#include <stdint.h>
#include "const.h"

typedef struct program program;

/*
 * struct execstate - execution state of program
 */
struct execstate {
	uint16_t mem[MEM_SIZE]; // program data
	uint16_t pp; // program pointer

	uint16_t stack[STACK_SIZE]; // execution stack
	uint16_t sp; // stack pointer

	uint16_t regs[REGS_SIZE]; // registers
};

typedef struct execstate execstate;

/*
 * struct instruction - a single machine instruction
 */
struct instruction {
	uint16_t opcode;
	uint16_t args[INST_MAX_ARGS];
};

typedef struct instruction instruction;

#endif
