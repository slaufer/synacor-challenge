#ifndef _VM_H
#define _VM_H

#include "const.h"

// macros for moving program pointer
#define ADVANCE_PP(state, inst) state->pp += (INST_NARGS[inst->opcode] + 1) * BIN_FIELD_WIDTH
#define SET_PP(state, pos) state->pp = state->prog->bin + pos * BIN_FIELD_WIDTH

// macros for interacting with memory
#define GET_MEM(src_ptr) (uint16_t) (src_ptr[0] + (uint16_t) 256 * src_ptr[1])
#define SET_MEM(dst_ptr, val) dst_ptr[0] = val & 255; dst_ptr[1] = val / 256;
#define CPY_MEM(dst_ptr, val_ptr) dst_ptr[0] = val_ptr[0]; dst_ptr[1] = val_ptr[1];

// macro for reading arguments
#define ARG_PTR(inst, pos) (inst->args + pos * BIN_FIELD_WIDTH) // get pointer to arg
#define GET_ARG(inst, pos) GET_MEM(ARG_PTR(inst, pos)) // get arg as uint16_t

// macros for interacting with registers
#define REG_PTR(state, reg) (state->regs + (reg - 32768) * BIN_FIELD_WIDTH) // get pointer to reg
#define GET_REG(state, reg) GET_MEM(REG_PTR(state, reg)) // get reg value as uint16_t
#define SET_REG(state, reg, val) SET_MEM(REG_PTR(state, reg), val)
#define CPY_REG(state, reg, val_ptr) CPY_MEM(REG_PTR(state, reg), val_ptr)

// macros for interacting with stack
#define STACK_PUSH(state, val) SET_MEM(state->sp, INTERP_VALUE(state, val)); state->sp += BIN_FIELD_WIDTH;
#define STACK_POP(state, dst_ptr) state->sp -= BIN_FIELD_WIDTH; CPY_MEM(dst_ptr, state->sp);

// macro for possibly getting register contents
#define INTERP_VALUE(state, val) (val < REG_BOTTOM ? val : GET_REG(state, val))

#endif
