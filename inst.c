#include <stdio.h>
#include <stdlib.h>
#include "const.h"
#include "struct.h"
#include "vm.h"
#include "instdebug.h"

/*
 * init_instruction - creates an instruction object
 * return - an instruction object with space allocated for 3 args
 */
instruction *init_instruction() {
	instruction *inst = malloc(sizeof(instruction));
	inst->opcode = 0;
	inst->args = 0;
}

/*
 * instruction_debug - prints some information on an instruction and its arguments
 */
void instruction_debug(execstate *state, instruction *inst) {
	int i, j;
	char buf[100]; // buffering output makes this a little bit faster

	// print out program pointer and instruction name
	j = sprintf(buf, "|%5hu|%4s|", (state->pp - state->prog->bin) / BIN_FIELD_WIDTH, INST_NAME[inst->opcode]);

	// print out args
	for (i = 0; i < INST_MAX_ARGS; i++) {
		if (i < INST_NARGS[inst->opcode]) {
			uint16_t arg = GET_ARG(inst, i);
			
			if (IS_REG(arg)) {
				j += sprintf(buf + j, "~%-4d|", arg - REG_BOTTOM);
			} else {
				j += sprintf(buf + j, "%5hu|", arg);
			}
		} else {
			j += sprintf(buf + j, "     |");
		}
	}

	// print out stack
	j += sprintf(buf + j, "%-2hu:%5hu|", (state->sp - state->stack) / 2, STACK_GET(state));

	// print out registers
	for (i = 0; i < REGS_SIZE; i++) {
		j += sprintf(buf + j, "%5hu|", GET_REG(state, i + REG_BOTTOM));
	}
	
	puts(buf);
}




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

	SET_REG(state, GET_ARG(inst, 0), TRY_REG(state, GET_ARG(inst, 1)));
	ADVANCE_PP(state, inst);
};

/*
 * instruction_push - services opcode 2 (push)
 * push <a> onto the stack
 */
void instruction_push(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_PUSH_DEBUG | INST_STACK_DEBUG)

	STACK_PUSH(state, GET_ARG(inst, 0));
	ADVANCE_PP(state, inst);
};

/*
 * instruction_pop - services opcode 3 (pop)
 * remove the top element from the stack and write it into <a>; empty stack = error
 */
void instruction_pop(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_POP_DEBUG | INST_STACK_DEBUG)

	if (state->sp == state->stack) {
		instruction_halt(state, inst);
	}

	STACK_POP(state, REG_PTR(state, GET_ARG(inst, 0)))
	ADVANCE_PP(state, inst);
};

/*
 * instruction_eq - services opcode 4, eq
 * set <a> to 1 if <b> is equal to <c>; set it to 0 otherwise
 */
void instruction_eq(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_EQ_DEBUG | INST_COMP_DEBUG)

	uint16_t result = TRY_REG(state, GET_ARG(inst, 1)) == TRY_REG(state, GET_ARG(inst, 2));
	SET_REG(state, GET_ARG(inst, 0), result);
	ADVANCE_PP(state, inst);
}

/*
 * instruction_gt - services opcode 5 (gt)
 * set <a> to 1 if <b> is greater than <c>; set it to 0 otherwise
 */
void instruction_gt(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_GT_DEBUG | INST_COMP_DEBUG)

	uint16_t result = TRY_REG(state, GET_ARG(inst, 1)) > TRY_REG(state, GET_ARG(inst, 2));
	SET_REG(state, GET_ARG(inst, 0), result)
	ADVANCE_PP(state, inst);
};

/*
 * instruction_jmp - services opcode 6, jmp
 * jump to <a>
 */
void instruction_jmp(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_JMP_DEBUG | INST_JMP_DEBUG)

	SET_PP(state, TRY_REG(state, GET_ARG(inst, 0)));
}

/*
 * instruction_jt - services opcode 7, jt
 * if <a> is nonzero, jump to <b>
 */
void instruction_jt(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_JT_DEBUG | INST_JMP_DEBUG)

	if (TRY_REG(state, GET_ARG(inst, 0))) {
		SET_PP(state, TRY_REG(state, GET_ARG(inst, 1)));
	} else {
		ADVANCE_PP(state, inst);
	}
}

/*
 * instruction_jf - services opcode 8, jf
 * if <a> is zero, jump to <b>
 */
void instruction_jf(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_JF_DEBUG | INST_JMP_DEBUG)

	if (TRY_REG(state, GET_ARG(inst, 0)) == 0) {
		SET_PP(state, TRY_REG(state, GET_ARG(inst,1)));
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

	uint16_t result = (TRY_REG(state, GET_ARG(inst, 1)) + TRY_REG(state, GET_ARG(inst, 2))) % MATH_MOD;
	SET_REG(state, GET_ARG(inst, 0), result);
	ADVANCE_PP(state, inst);
}

/*
 * instruction_mult - services opcode 10, mult
 * store into <a> the product of <b> and <c> (modulo 32768)
 */
void instruction_mult(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_MULT_DEBUG | INST_MATH_DEBUG)

	uint16_t result = ((uint32_t) TRY_REG(state, GET_ARG(inst, 1)) * TRY_REG(state, GET_ARG(inst, 2))) % MATH_MOD;
	SET_REG(state, GET_ARG(inst, 0), result);
	ADVANCE_PP(state, inst);
}

/*
 * instruction_mod - services opcode 10, mod
 * store into <a> the remainder of <b> divided by <c>
 */
void instruction_mod(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_MOD_DEBUG | INST_MATH_DEBUG)

	uint16_t result = ((uint32_t) TRY_REG(state, GET_ARG(inst, 1)) % TRY_REG(state, GET_ARG(inst, 2)));
	SET_REG(state, GET_ARG(inst, 0), result);
	ADVANCE_PP(state, inst);
}

/*
 * instruction_and - services opcode 12, and
 * stores into <a> the bitwise and of <b> and <c>
 */
void instruction_and(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_AND_DEBUG | INST_BITWISE_DEBUG)

	uint16_t result = TRY_REG(state, GET_ARG(inst, 1)) & TRY_REG(state, GET_ARG(inst, 2));
	SET_REG(state, GET_ARG(inst, 0), result);
	ADVANCE_PP(state, inst);
}

/*
 * instruction_or - services opcode 13, or
 * stores into <a> the bitwise or of <b> and <c>
 */
void instruction_or(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_OR_DEBUG | INST_BITWISE_DEBUG)

	uint16_t result = TRY_REG(state, GET_ARG(inst, 1)) | TRY_REG(state, GET_ARG(inst, 2));
	SET_REG(state, GET_ARG(inst, 0), result);
	ADVANCE_PP(state, inst);
}

/*
 * instruction_not - services opcode 14, not
 * stores 15-bit bitwise inverse of <b> in <a>
 */
void instruction_not(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_NOT_DEBUG | INST_BITWISE_DEBUG)

	uint16_t result = TRY_REG(state, GET_ARG(inst, 1)) ^ (uint16_t) 32767;
	SET_REG(state, GET_ARG(inst, 0), result);
	ADVANCE_PP(state, inst);
}

/*
 * instruction_rmem - services opcode 15, rmem
 * read memory at address <b> and write it to <a>
 */
void instruction_rmem(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_RMEM_DEBUG | INST_HEAP_DEBUG)

	uint16_t value = GET_PROG(state, TRY_REG(state, GET_ARG(inst, 1)));
	SET_REG(state, GET_ARG(inst, 0), value % MATH_MOD)
	// CPY_REG(state, GET_ARG(inst, 0), PROG_PTR(state, TRY_REG(state, GET_ARG(inst, 1))));
	ADVANCE_PP(state, inst);
}

/*
 * instruction_wmem - services opcode 16, wmem
 * write the value from <b> into memory at address <a>
 */
void instruction_wmem(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_WMEM_DEBUG | INST_HEAP_DEBUG)

	SET_MEM(PROG_PTR(state, TRY_REG(state, GET_ARG(inst, 0))), TRY_REG(state, GET_ARG(inst, 1)));
	ADVANCE_PP(state, inst);
}

/*
 * instruction_call - services opcode 17, call
 * write the address of the next instruction to the stack and jump to <a>
 */
void instruction_call(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_CALL_DEBUG | INST_JMP_DEBUG)

	// printf("calling to %hu, pushing %hu\n", TRY_REG(state, GET_ARG(inst, 0)), NEXT_PP(state, inst));

	STACK_PUSH(state, NEXT_PP(state, inst));
	SET_PP(state, TRY_REG(state, GET_ARG(inst, 0)));
}

/*
 * instruction_ret - services opcode 18, ret
 * remove the top element from the stack and jump to it; empty stack = halt
 */
void instruction_ret(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_RET_DEBUG | INST_JMP_DEBUG)

	if (state->sp == state->stack) {
		instruction_halt(state, inst);
	}

	uint8_t target[2];
	STACK_POP(state, target);
	// printf("returning to %hu\n", GET_MEM(target));

	SET_PP(state, GET_MEM(target));
}

/*
 * instruction_out - services opcode 19, out
 * write the character represented by ascii code <a> to the terminal
 */
void instruction_out(execstate *state, instruction *inst) {
	INST_DEBUG_MSG(state, inst, INST_OUT_DEBUG | INST_IO_DEBUG)

	printf("%c", TRY_REG(state, GET_ARG(inst, 0)));

	// advance program pointer
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

	// check if we just enabled debug mode
	// read on to next char if we did
	#ifdef INST_DEBUG_CODE
	if (ch == INST_DEBUG_CODE) {
		INST_DEBUG_OVERRIDE = ! INST_DEBUG_OVERRIDE;
		printf("=== DEBUG MODE %s ===", INST_DEBUG_OVERRIDE ? "ENABLED" : "DISABLED");

		instruction_in(state, inst);
		return;
	}
	#endif

	SET_REG(state, GET_ARG(inst, 0), ch);

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
