#ifndef _INST_H
#define _INST_H

#include <stdint.h>
#include "vm.h"

// maximum instruction arguments
#define INST_MAX_ARGS 3

// number of instructions
#define INST_COUNT 22

/*
 * struct instruction - a single machine instruction
 */
struct instruction {
	uint16_t opcode;
	uint8_t *args;
	uint8_t nargs;
};

typedef struct instruction instruction;

// methods to service individual instructions
extern void (*INST_FNS[])(execstate*, instruction*);

// argument counts for instructions
extern uint8_t INST_NARGS[];

// creates instruction objects
instruction *init_instruction();

// argument service function declarations
void inst_halt(execstate*, instruction*); //  0
void inst_set (execstate*, instruction*); //  1
void inst_jmp (execstate*, instruction*); //  6
void inst_jt  (execstate*, instruction*); //  7
void inst_jf  (execstate*, instruction*); //  8
void inst_out (execstate*, instruction*); // 19
void inst_noop(execstate*, instruction*); // 21

#endif
