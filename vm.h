#ifndef _VM_H
#define _VM_H

// field width in program binary data
#define BIN_FIELD_WIDTH 2

// maximum instruction arguments
#define MAX_INST_ARGS 3

// argument counts for instructions
const uint8_t INST_NARGS[] = {
	0, // opcode 0: halt
	2, // opcode 1: set
	1, // opcode 2: push
	1, // opcode 3: pop
	3, // opcode 4: eq
	3, // opcode 5: gt
	1, // opcode 6: jmp
	2, // opcode 7: jt
	2, // opcode 8: jf
	3, // opcode 9: add
	3, // opcode 10: mult
	3, // opcode 11: mod
	3, // opcode 12: and
	3, // opcode 13: or
	2, // opcode 14: mod
	2, // opcode 15: rmem
	2, // opcode 16: wmem
	1, // opcode 17: call
	0, // opcode 18: ret
	1, // opcode 19: out
	1, // opcode 20: in
	0, // opcode 21: noop
};

const int *INST_FNS[] = {};

/*************************************************
 * struct program - program binary data          *
 * this usually shouldn't be modified at runtime *
 *************************************************/
struct program {
	size_t sz; // program size in bytes
	uint8_t *bin; // binary instruction data
};

typedef struct program program;

/*************************************************
 * struct execstate - execution state of program *
 *************************************************/
struct execstate {
	program *prog; // program data
	uint8_t *pp; // program pointer

	uint8_t *stack; // execution stack
	uint8_t *sp; // stack pointer

	uint8_t *heap; // heap/general purpose memory
	uint8_t *regs; // registers
};

typedef struct execstate execstate;

/*****************************************************
 * struct instruction - a single machine instruction *
 *****************************************************/
struct instruction {
	uint16_t opcode;
	uint16_t *args;
	uint8_t nargs;
};

typedef struct instruction instruction;

#endif
