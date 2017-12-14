#ifndef _VM_H
#define _VM_H

#include <unistd.h>

/*
 * struct program - program binary data
 * this usually shouldn't be modified at runtime
 */
struct program {
	size_t sz; // program size in bytes
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
	uint8_t *regs; // registers
};

typedef struct execstate execstate;


#endif
