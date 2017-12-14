#ifndef _STRUCT_H
#define _STRUCT_H

#include <stdint.h>

/*
 * struct program - program binary data
 * this usually shouldn't be modified at runtime
 */
struct program {
	uint16_t sz; // program size in bytes
	uint8_t *bin; // binary instruction data
};

typedef struct program program;

/*
 * struct execstate - execution state of program
 */
struct execstate {
	program *prog; // program data
	uint8_t *pp; // program pointer

	uint8_t *stack; // execution stack
	uint8_t *sp; // stack pointer

	uint8_t *heap; // heap/general purpose memory
	uint16_t heap_size;

	uint8_t *regs; // registers
	uint16_t regs_size;
};

typedef struct execstate execstate;

/*
 * struct instruction - a single machine instruction
 */
struct instruction {
	uint16_t opcode;
	uint8_t *args;
};

typedef struct instruction instruction;

#endif
