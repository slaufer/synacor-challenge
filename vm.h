#ifndef _VM_H
#define _VM_H

// macros for moving program pointer
#define ADVANCE_PP(state, inst) state->pp += INST_NARGS[inst->opcode] + 1;

// macros for interacting with registers
#define IS_REG(reg) (reg >= REG_BOTTOM && reg < REG_BOTTOM + REGS_SIZE)
#define TO_REG(reg) (reg - REG_BOTTOM)
#define TRY_REG(state, val) (IS_REG(val) ? state->regs[TO_REG(val)] : val)

// macros for interacting with stack
#define STACK_PUSH(state, val) state->stack[state->sp] = val; state->sp += 1;
#define STACK_GET(state) (state->stack[state->sp - 1])
#define STACK_POP(state) (state->sp -= 1, state->stack[state->sp])

#endif
