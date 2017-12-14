#include <stdio.h>
#include <stdlib.h>
#include "const.h"
#include "struct.h"
#include "vm.h"

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
instruction_debug(execstate *state, instruction *inst) {
	printf("%5hu  %2hu %-4s ", (state->pp - state->prog->bin) / BIN_FIELD_WIDTH, inst->opcode, INST_NAME[inst->opcode]);
	char* buf[256];

	int i;
	for (i = 0; i < INST_NARGS[inst->opcode]; i++) {
		uint16_t arg = GET_ARG(inst, i);
		uint16_t arg_i = TRY_REG(state, arg);
		
		if (arg == arg_i) {
			printf("%-8hu  ", arg);
		} else {
			printf("R%hu:%-5hu  ", arg - REG_BOTTOM, arg_i);
		}
	}
	
	printf("\n");
	
	#ifdef REG_DEBUG
	for (i = 0; i < REGS_SIZE; i++) {
		printf("R%d:%5hu  ", i, GET_REG(state, i + REG_BOTTOM));
	}
	printf("\n");
	#endif
}

/*
 * instruction_halt - services opcode 0 (halt)
 *             also used for unknown opcodes
 */
void instruction_halt(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_HALT_DEBUG) || defined(CORE_DEBUG)
	instruction_debug(state, inst);
	#endif

	switch (inst->opcode) {
		case 0:
			printf("\n=== HALT ===\n");
			exit(0);
		case 18:
			printf("\n=== STACK EMPTY ===\n");
			exit(0);
		default:
			instruction_debug(state, inst);
			printf("=== UNIMPLEMENTED INSTRUCTION %d ===\n", inst->opcode);
			exit(1);
	}

	// don't bother doing anything else, just exit
	exit(0);
};

/*
 * instruction_set - services opcode 1 (set)
 */
void instruction_set(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_SET_DEBUG) || defined(REG_DEBUG)
	instruction_debug(state, inst);
	#endif

	CPY_REG(state, GET_ARG(inst, 0), ARG_PTR(inst, 1));
	ADVANCE_PP(state, inst);
};

/*
 * instruction_push - services opcode 2 (push)
 */
void instruction_push(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_SET_DEBUG) || defined(STACK_DEBUG)
	instruction_debug(state, inst);
	#endif

	STACK_PUSH(state, GET_ARG(inst, 0));
	ADVANCE_PP(state, inst);
};

/*
 * instruction_pop - services opcode 3 (pop)
 */
void instruction_pop(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_POP_DEBUG) || defined(STACK_DEBUG)
	instruction_debug(state, inst);
	#endif

	STACK_POP(state, REG_PTR(state, GET_ARG(inst, 0)))
	ADVANCE_PP(state, inst);
};

/*
 * instruction_eq - services opcode 4, eq
 */
void instruction_eq(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_EQ_DEBUG) || defined(COMP_DEBUG)
	instruction_debug(state, inst);
	#endif

	uint16_t result = TRY_REG(state, GET_ARG(inst, 1)) == TRY_REG(state, GET_ARG(inst, 2));
	SET_REG(state, GET_ARG(inst, 0), result);
	ADVANCE_PP(state, inst);
}

/*
 * instruction_gt - services opcode 5 (gt)
 */
void instruction_gt(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_GT_DEBUG) || defined(COMP_DEBUG)
	instruction_debug(state, inst);
	#endif

	uint16_t result = TRY_REG(state, GET_ARG(inst, 1)) > TRY_REG(state, GET_ARG(inst, 2));
	SET_REG(state, GET_ARG(inst, 0), result)
	ADVANCE_PP(state, inst);
};

/*
 * instruction_jmp - services opcode 6, jmp
 */
void instruction_jmp(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_JMP_DEBUG) || defined(JMP_DEBUG)
	instruction_debug(state, inst);
	#endif

	SET_PP(state, TRY_REG(state, GET_ARG(inst, 0)));
}

/*
 * instruction_jt - services opcode 7, jt
 */
void instruction_jt(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_JT_DEBUG) || defined(JMP_DEBUG)
	instruction_debug(state, inst);
	#endif

	if (TRY_REG(state, GET_ARG(inst, 0))) {
		SET_PP(state, TRY_REG(state, GET_ARG(inst, 1)));
	} else {
		ADVANCE_PP(state, inst);
	}
}

/*
 * instruction_jf - services opcode 8, jf
 */
void instruction_jf(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_JF_DEBUG) || defined(JMP_DEBUG)
	instruction_debug(state, inst);
	#endif

	if (TRY_REG(state, GET_ARG(inst, 0)) == 0) {
		SET_PP(state, TRY_REG(state, GET_ARG(inst,1)));
	} else {
		ADVANCE_PP(state, inst);
	}
}

/*
 * instruction_add - services opcode 9, add
 */
void instruction_add(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_ADD_DEBUG) || defined(MATH_DEBUG)
	instruction_debug(state, inst);
	#endif

	uint16_t result = (TRY_REG(state, GET_ARG(inst, 1)) + TRY_REG(state, GET_ARG(inst, 2))) % MATH_MOD;
	SET_REG(state, GET_ARG(inst, 0), result);
	ADVANCE_PP(state, inst);
}

/*
 * instruction_mult - services opcode 10, mult
 */
void instruction_mult(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_MULT_DEBUG) || defined(MATH_DEBUG)
	instruction_debug(state, inst);
	#endif

	uint16_t result = ((uint32_t) TRY_REG(state, GET_ARG(inst, 1)) * TRY_REG(state, GET_ARG(inst, 2))) % MATH_MOD;
	SET_REG(state, GET_ARG(inst, 0), result);
	ADVANCE_PP(state, inst);
}

/*
 * instruction_mod - services opcode 10, mod
 */
void instruction_mod(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_MOD_DEBUG) || defined(MATH_DEBUG)
	instruction_debug(state, inst);
	#endif

	uint16_t result = ((uint32_t) TRY_REG(state, GET_ARG(inst, 1)) % TRY_REG(state, GET_ARG(inst, 2))) % MATH_MOD;
	SET_REG(state, GET_ARG(inst, 0), result);
	ADVANCE_PP(state, inst);
}

/*
 * instruction_and - services opcode 12, and
 */
void instruction_and(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_AND_DEBUG) || defined(BITWISE_DEBUG)
	instruction_debug(state, inst);
	#endif

	uint16_t result = TRY_REG(state, GET_ARG(inst, 1)) & TRY_REG(state, GET_ARG(inst, 2));
	SET_REG(state, GET_ARG(inst, 0), result);
	ADVANCE_PP(state, inst);
}

/*
 * instruction_or - services opcode 13, or
 */
void instruction_or(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_OR_DEBUG) || defined(BITWISE_DEBUG)
	instruction_debug(state, inst);
	#endif

	uint16_t result = TRY_REG(state, GET_ARG(inst, 1)) | TRY_REG(state, GET_ARG(inst, 2));
	SET_REG(state, GET_ARG(inst, 0), result);
	ADVANCE_PP(state, inst);
}

/*
 * instruction_not - services opcode 14, not
 */
void instruction_not(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_NOT_DEBUG) || defined(BITWISE_DEBUG)
	instruction_debug(state, inst);
	#endif

	uint16_t result = TRY_REG(state, GET_ARG(inst, 1)) ^ (uint16_t) 32767;
	SET_REG(state, GET_ARG(inst, 0), result);
	ADVANCE_PP(state, inst);
}

/*
 * instruction_rmem - services opcode 15, rmem
 * read memory at address <b> and write it to <a>
 */
void instruction_rmem(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_RMEM_DEBUG) || defined(HEAP_DEBUG)
	instruction_debug(state, inst);
	#endif

	CPY_REG(state, GET_ARG(inst, 0), PROG_PTR(state, TRY_REG(state, GET_ARG(inst, 1))));
	ADVANCE_PP(state, inst);
}

/*
 * instruction_wmem - services opcode 16, wmem
 * write the value from <b> into memory at address <a>
 */
void instruction_wmem(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_WMEM_DEBUG) || defined(HEAP_DEBUG)
	instruction_debug(state, inst);
	#endif

	SET_MEM(PROG_PTR(state, TRY_REG(state, GET_ARG(inst, 0))), TRY_REG(state, GET_ARG(inst, 1)));
	ADVANCE_PP(state, inst);
}

/*
 * instruction_call - services opcode 17, call
 * write the address of the next instruction to the stack and jump to <a>
 */
void instruction_call(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_CALL_DEBUG) || defined(JMP_DEBUG)
	instruction_debug(state, inst);
	#endif

	// printf("calling to %hu, pushing %hu\n", TRY_REG(state, GET_ARG(inst, 0)), NEXT_PP(state, inst));

	STACK_PUSH(state, NEXT_PP(state, inst));
	SET_PP(state, TRY_REG(state, GET_ARG(inst, 0)));
}

/*
 * instruction_ret - services opcode 18, ret
 * remove the top element from the stack and jump to it; empty stack = halt
 */
void instruction_ret(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_RET_DEBUG) || defined(JMP_DEBUG)
	instruction_debug(state, inst);
	#endif

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
 */
void instruction_out(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_OUT_DEBUG) || defined(IO_DEBUG)
	instruction_debug(state, inst);
	#endif

	printf("%c", GET_ARG(inst, 0));

	// advance program pointer
	ADVANCE_PP(state, inst);
}

/*
 * instruction_noop - services opcode 21, noop
 */
void instruction_noop(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_NOOP_DEBUG) || defined(CORE_DEBUG)
	instruction_debug(state, inst);
	#endif

	// advance program pointer
	ADVANCE_PP(state, inst);
};

