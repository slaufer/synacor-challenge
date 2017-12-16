#ifndef _CONST_H
#define _CONST_H

#include <stdint.h>

// field width in program binary data
#define BIN_FIELD_WIDTH 2

// cutoff where literals become register numbers
#define REG_BOTTOM 32768

// heap and register sizes (in elements, not bytes)
#define STACK_SIZE 32768
#define MEM_SIZE 32768
#define REGS_SIZE 8

// all math is mod MATH_MOD
#define MATH_MOD 32768

// maximum instruction arguments
#define INST_MAX_ARGS 3

// number of instructions
#define INST_COUNT 22

// names of instructions
extern const char* const INST_NAME[];

// argument counts for instructions
extern const uint8_t const INST_NARGS[];

#endif
