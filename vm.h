#ifndef _VM_H
#define _VM_H

#include <unistd.h>
#include "program.h"
#include "string.h"

// field width in program binary data
#define BIN_FIELD_WIDTH 2

// heap and register sizes (in elements, not bytes)
#define HEAP_SIZE 32768
#define REGS_SIZE 8

#define MATH_MOD 32768

// macro for converting between in memory/registers to integers
#define GET_VALUE(val_ptr) (val_ptr[0] + val_ptr[1] * 256)
#define SET_VALUE(val_ptr, val) val_ptr[0] = val & 255; val_ptr[1] = val / 256;

// macros for moving program pointer
#define ADVANCE_PP(state, inst) state->pp += (inst->nargs + 1) * BIN_FIELD_WIDTH
#define SET_PP(state, pos) state->pp = state->prog->bin + pos * BIN_FIELD_WIDTH

// macro for reading arguments
#define ARG_PTR(inst, pos) (inst->args + pos * BIN_FIELD_WIDTH) // get pointer to arg
#define GET_ARG(inst, pos) GET_VALUE(ARG_PTR(inst, pos)) // get arg as uint16_t

// macros for interacting with registers
#define REG_PTR(state, reg) (state->regs + (reg - 32768) * BIN_FIELD_WIDTH) // get pointer to reg
#define GET_REG(state, reg) GET_VALUE(REG_PTR(state, reg)) // get reg value as uint16_t
#define SET_REG(state, reg, val_ptr) memcpy(REG_PTR(state, reg), val_ptr, BIN_FIELD_WIDTH) // set reg to data at val_ptr

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

#endif