#include <stdio.h>
#include "struct.h"
#include "vm.h"

#ifdef INST_DEBUG_START
uint8_t INST_DEBUG_OVERRIDE = 1;
#else
uint8_t INST_DEBUG_OVERRIDE = 0;
#endif

/*
 * instruction_debug - prints some information on an instruction and its arguments
 */
void instruction_debug(execstate *state, instruction *inst) {
	int i, j;
	char buf[100]; // buffering output makes this a little bit faster

	// print out program pointer and instruction name
	j = sprintf(buf, "|%5hu|%4s|", state->pp , INST_NAME[inst->opcode]);

	// print out args
	for (i = 0; i < INST_MAX_ARGS; i++) {
		if (i < INST_NARGS[inst->opcode]) {
			if (IS_REG(inst->args[i])) {
				j += sprintf(buf + j, "$%4d|", inst->args[i] - REG_BOTTOM);
			} else {
				j += sprintf(buf + j, "%5hu|", inst->args[i]);
			}
		} else {
			j += sprintf(buf + j, "     |");
		}
	}

	// print out stack
	j += sprintf(buf + j, "%-2hu:%5hu|", state->sp, state->stack[state->sp > 0 ? state->sp - 1 : 0]);

	// print out registers
	for (i = 0; i < REGS_SIZE; i++) {
		j += sprintf(buf + j, "%hu~%5hu|", i, state->regs[i]);
	}
	
	puts(buf);
}
