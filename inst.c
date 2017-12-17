#include <stdio.h>
#include <stdlib.h>
#include "const.h"
#include "struct.h"
#include "vm.h"
#include "instdebug.h"

/*
 * instruction_halt - services opcode 0 (halt)
 * stop execution and terminate the program
 */
void instruction_halt(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_HALT_DEBUG | INST_CORE_DEBUG)

	switch (inst->opcode) {
		case 0:
			printf("=== HALT ===\n");
			exit(0);
		case 3:
		case 18:
			printf("=== STACK EMPTY ===\n");
			exit(0);
		default:
			printf("=== UNIMPLEMENTED INSTRUCTION %d ===\n", inst->opcode);
			exit(1);
	}

	// don't bother doing anything else, just exit
	exit(0);
};

/*
 * instruction_set - services opcode 1 (set)
 * set register <a> to the value of <b>
 */
void instruction_set(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_SET_DEBUG | INST_REG_DEBUG)

	state->regs[TO_REG(inst->args[0])] = TRY_REG(state, inst->args[1]);
	ADVANCE_PP(state, inst);
};

/*
 * instruction_push - services opcode 2 (push)
 * push <a> onto the stack
 */
void instruction_push(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_PUSH_DEBUG | INST_STACK_DEBUG)

	STACK_PUSH(state, TRY_REG(state, inst->args[0]));
	ADVANCE_PP(state, inst);
};

/*
 * instruction_pop - services opcode 3 (pop)
 * remove the top element from the stack and write it into <a>; empty stack = error
 */
void instruction_pop(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_POP_DEBUG | INST_STACK_DEBUG)

	if (state->sp == 0) {
		instruction_halt(state, inst);
	}

	state->regs[TO_REG(inst->args[0])] = STACK_POP(state);
	ADVANCE_PP(state, inst);
};

/*
 * instruction_eq - services opcode 4, eq
 * set <a> to 1 if <b> is equal to <c>; set it to 0 otherwise
 */
void instruction_eq(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_EQ_DEBUG | INST_COMP_DEBUG)

	state->regs[TO_REG(inst->args[0])] = TRY_REG(state, inst->args[1]) == TRY_REG(state, inst->args[2]);
	ADVANCE_PP(state, inst);
}

/*
 * instruction_gt - services opcode 5 (gt)
 * set <a> to 1 if <b> is greater than <c>; set it to 0 otherwise
 */
void instruction_gt(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_GT_DEBUG | INST_COMP_DEBUG)

	state->regs[TO_REG(inst->args[0])] = TRY_REG(state, inst->args[1]) > TRY_REG(state, inst->args[2]);
	ADVANCE_PP(state, inst);
};

/*
 * instruction_jmp - services opcode 6, jmp
 * jump to <a>
 */
void instruction_jmp(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_JMP_DEBUG | INST_PP_DEBUG)

	state->pp = TRY_REG(state, inst->args[0]);
}

/*
 * instruction_jt - services opcode 7, jt
 * if <a> is nonzero, jump to <b>
 */
void instruction_jt(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_JT_DEBUG | INST_PP_DEBUG)

	if (TRY_REG(state, inst->args[0])) {
		state->pp = TRY_REG(state, inst->args[1]);
	} else {
		ADVANCE_PP(state, inst);
	}
}

/*
 * instruction_jf - services opcode 8, jf
 * if <a> is zero, jump to <b>
 */
void instruction_jf(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_JF_DEBUG | INST_PP_DEBUG)

	if (TRY_REG(state, inst->args[0]) == 0) {
		state->pp = TRY_REG(state, inst->args[1]);
	} else {
		ADVANCE_PP(state, inst);
	}
}

/*
 * instruction_add - services opcode 9, add
 * assign into <a> the sum of <b> and <c> (modulo 32768)
 */
void instruction_add(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_ADD_DEBUG | INST_MATH_DEBUG)

	state->regs[TO_REG(inst->args[0])] = (TRY_REG(state, inst->args[1]) + TRY_REG(state, inst->args[2])) % MATH_MOD;
	ADVANCE_PP(state, inst);
}

/*
 * instruction_mult - services opcode 10, mult
 * store into <a> the product of <b> and <c> (modulo 32768)
 */
void instruction_mult(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_MULT_DEBUG | INST_MATH_DEBUG)

	state->regs[TO_REG(inst->args[0])] = (TRY_REG(state, inst->args[1]) * TRY_REG(state, inst->args[2])) % MATH_MOD;
	ADVANCE_PP(state, inst);
}

/*
 * instruction_mod - services opcode 10, mod
 * store into <a> the remainder of <b> divided by <c>
 */
void instruction_mod(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_MOD_DEBUG | INST_MATH_DEBUG)

	state->regs[TO_REG(inst->args[0])] = (TRY_REG(state, inst->args[1]) % TRY_REG(state, inst->args[2]));
	ADVANCE_PP(state, inst);
}

/*
 * instruction_and - services opcode 12, and
 * stores into <a> the bitwise and of <b> and <c>
 */
void instruction_and(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_AND_DEBUG | INST_BITWISE_DEBUG)

	state->regs[TO_REG(inst->args[0])] = (TRY_REG(state, inst->args[1]) & TRY_REG(state, inst->args[2]));
	ADVANCE_PP(state, inst);
}

/*
 * instruction_or - services opcode 13, or
 * stores into <a> the bitwise or of <b> and <c>
 */
void instruction_or(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_OR_DEBUG | INST_BITWISE_DEBUG)

	state->regs[TO_REG(inst->args[0])] = (TRY_REG(state, inst->args[1]) | TRY_REG(state, inst->args[2]));
	ADVANCE_PP(state, inst);
}

/*
 * instruction_not - services opcode 14, not
 * stores 15-bit bitwise inverse of <b> in <a>
 */
void instruction_not(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_NOT_DEBUG | INST_BITWISE_DEBUG)

	state->regs[TO_REG(inst->args[0])] = TRY_REG(state, inst->args[1]) ^ (uint16_t) 32767;
	ADVANCE_PP(state, inst);
}

/*
 * instruction_rmem - services opcode 15, rmem
 * read memory at address <b> and write it to <a>
 */
void instruction_rmem(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_RMEM_DEBUG | INST_MEM_DEBUG)

	state->regs[TO_REG(inst->args[0])] = state->mem[TRY_REG(state, inst->args[1])];
	ADVANCE_PP(state, inst);
}

/*
 * instruction_wmem - services opcode 16, wmem
 * write the value from <b> into memory at address <a>
 */
void instruction_wmem(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_WMEM_DEBUG | INST_MEM_DEBUG)

	state->mem[TRY_REG(state, inst->args[0])] = TRY_REG(state, inst->args[1]);
	ADVANCE_PP(state, inst);
}

/*
 * instruction_call - services opcode 17, call
 * write the address of the next instruction to the stack and jump to <a>
 */
void instruction_call(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_CALL_DEBUG | INST_PP_DEBUG | INST_STACK_DEBUG)

	STACK_PUSH(state, state->pp + INST_NARGS[inst->opcode] + 1);
	state->pp = TRY_REG(state, inst->args[0]);
}

/*
 * instruction_ret - services opcode 18, ret
 * remove the top element from the stack and jump to it; empty stack = halt
 */
void instruction_ret(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_RET_DEBUG | INST_PP_DEBUG | INST_STACK_DEBUG)

	if (state->sp == 0) {
		instruction_halt(state, inst);
	}

	state->pp = STACK_POP(state);
}

/*
 * instruction_out - services opcode 19, out
 * write the character represented by ascii code <a> to the terminal
 */
void instruction_out(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_OUT_DEBUG | INST_IO_DEBUG)

	printf("%c", TRY_REG(state, inst->args[0]));
	ADVANCE_PP(state, inst);
}

/*
 * instruction_in - services opcode 20, in
 * read a character from the terminal and write its ascii code to <a>
 * it can be assumed that once input starts, it will continue until a newline is encountered; this means that you can
 * safely read whole lines from the keyboard and trust that they will be fully read
 */
void instruction_in(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_IN_DEBUG | INST_IO_DEBUG)

	uint8_t ch = getchar();
	state->regs[TO_REG(inst->args[0])] = ch;

	#ifdef INPUT_ECHO
	putchar(ch);
	#endif

	// advance program pointer
	ADVANCE_PP(state, inst);
}

/*
 * instruction_noop - services opcode 21, noop
 * no operation
 */
void instruction_noop(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_NOOP_DEBUG | INST_CORE_DEBUG)

	// advance program pointer
	ADVANCE_PP(state, inst);
};

// array of opcode service functions
void (*INST_FNS[])(execstate*, instruction*) = {
	instruction_halt, // opcode 0: halt
	instruction_set,  // opcode 1: set
	instruction_push, // opcode 2: push
	instruction_pop,  // opcode 3: pop
	instruction_eq,   // opcode 4: eq
	instruction_gt,   // opcode 5: gt
	instruction_jmp,  // opcode 6: jmp
	instruction_jt,   // opcode 7: jt
	instruction_jf,   // opcode 8: jf
	instruction_add,  // opcode 9: add
	instruction_mult, // opcode 10: mult
	instruction_mod,  // opcode 11: mod
	instruction_and,  // opcode 12: and
	instruction_or,   // opcode 13: or
	instruction_not,  // opcode 14: not
	instruction_rmem, // opcode 15: rmem
	instruction_wmem, // opcode 16: wmem
	instruction_call, // opcode 17: call
	instruction_ret,  // opcode 18: ret
	instruction_out,  // opcode 19: out
	instruction_in,   // opcode 20: in
	instruction_noop, // opcode 21: noop
};
