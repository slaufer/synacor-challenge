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
 * instruction_halt - services opcode 0 (halt)
 *             also used for unknown opcodes
 */
void instruction_halt(execstate *state, instruction *inst) {
	if (inst->opcode == 0) {
		printf("\n=== HALT ===\n");
	} else {
		printf("\n=== HALT UNIMPLEMENTED OPCODE=%d NARGS=%d ARGS=%d,%d,%d ===\n",
			inst->opcode, INST_NARGS[inst->opcode], GET_ARG(inst, 0), GET_ARG(inst, 1), GET_ARG(inst, 2));
	}

	// don't bother doing anything else, just exit
	exit(0);
};

/*
 * instruction_set - services opcode 1 (set)
 */
void instruction_set(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_SET_DEBUG) || defined(REG_DEBUG)
	printf("SET %d %d\n", GET_ARG(inst, 0), GET_ARG(inst, 1));
	#endif

	CPY_REG(state, GET_ARG(inst, 0), ARG_PTR(inst, 1));
	ADVANCE_PP(state, inst);
};

/*
 * instruction_push - services opcode 2 (push)
 */
void instruction_push(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_SET_DEBUG) || defined(STACK_DEBUG)
	printf("PUSH %d\n", GET_ARG(inst, 0));
	#endif

	STACK_PUSH(state, GET_ARG(inst, 0));
	ADVANCE_PP(state, inst);
};

/*
 * instruction_pop - services opcode 3 (pop)
 */
void instruction_pop(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_POP_DEBUG) || defined(STACK_DEBUG)
	printf("POP %d\n", GET_ARG(inst, 0));
	#endif

	STACK_POP(state, REG_PTR(state, GET_ARG(inst, 0)))
	ADVANCE_PP(state, inst);
};

/*
 * instruction_eq - services opcode 4, eq
 */
void instruction_eq(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_EQ_DEBUG) || defined(COMP_DEBUG)
	printf("EQ %d %d %d\n", GET_ARG(inst, 0), GET_ARG(inst, 1), GET_ARG(inst, 2));
	#endif

	uint16_t result = INTERP_VALUE(state, GET_ARG(inst, 1)) == INTERP_VALUE(state, GET_ARG(inst, 2));
	SET_REG(state, GET_ARG(inst, 0), result);
	ADVANCE_PP(state, inst);
}

/*
 * instruction_gt - services opcode 5 (gt)
 */
void instruction_gt(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_GT_DEBUG) || defined(COMP_DEBUG)
	printf("GT %d %d %d\n", GET_ARG(inst, 0), GET_ARG(inst, 1), GET_ARG(inst, 2));
	#endif

	uint16_t result = INTERP_VALUE(state, GET_ARG(inst, 1)) > INTERP_VALUE(state, GET_ARG(inst, 2));
	SET_REG(state, GET_ARG(inst, 0), result)
	ADVANCE_PP(state, inst);
};

/*
 * instruction_jmp - services opcode 6, jmp
 */
void instruction_jmp(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_JMP_DEBUG) || defined(JMP_DEBUG)
	printf("JMP %d\n", GET_ARG(inst, 0));
	#endif

	SET_PP(state, INTERP_VALUE(state, GET_ARG(inst, 0)));
}

/*
 * instruction_jt - services opcode 7, jt
 */
void instruction_jt(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_JT_DEBUG) || defined(JMP_DEBUG)
	printf("JT %d %d\n", GET_ARG(inst, 0), GET_ARG(inst, 1));
	#endif

	if (INTERP_VALUE(state, GET_ARG(inst, 0))) {
		SET_PP(state, INTERP_VALUE(state, GET_ARG(inst, 1)));
	} else {
		ADVANCE_PP(state, inst);
	}
}

/*
 * instruction_jf - services opcode 8, jf
 */
void instruction_jf(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_JF_DEBUG) || defined(JMP_DEBUG)
	printf("JF %d %d\n", inst->args[0], inst->args[1]);
	#endif

	if (INTERP_VALUE(state, GET_ARG(inst, 0)) == 0) {
		SET_PP(state, INTERP_VALUE(state, GET_ARG(inst,1)));
	} else {
		ADVANCE_PP(state, inst);
	}
}

/*
 * instruction_add - services opcode 9, add
 */
void instruction_add(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_ADD_DEBUG) || defined(MATH_DEBUG)
	printf("ADD %d %d %d\n", GET_ARG(inst, 0), GET_ARG(inst, 1), GET_ARG(inst, 2));
	#endif

	uint16_t result = (INTERP_VALUE(state, GET_ARG(inst, 1)) + INTERP_VALUE(state, GET_ARG(inst, 2))) % MATH_MOD;
	SET_REG(state, GET_ARG(inst, 0), result);
	ADVANCE_PP(state, inst);
}

/*
 * instruction_and - services opcode 12, and
 */
void instruction_and(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_AND_DEBUG) || defined(BITWISE_DEBUG)
	printf("AND %d %d %d\n", GET_ARG(inst, 0), GET_ARG(inst, 1), GET_ARG(inst, 2));
	#endif

	uint16_t result = INTERP_VALUE(state, GET_ARG(inst, 1)) & INTERP_VALUE(state, GET_ARG(inst, 2));
	SET_REG(state, GET_ARG(inst, 0), result);
	ADVANCE_PP(state, inst);
}

/*
 * instruction_or - services opcode 13, or
 */
void instruction_or(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_OR_DEBUG) || defined(BITWISE_DEBUG)
	printf("OR %d %d %d\n", GET_ARG(inst, 0), GET_ARG(inst, 1), GET_ARG(inst, 2));
	#endif

	uint16_t result = INTERP_VALUE(state, GET_ARG(inst, 1)) | INTERP_VALUE(state, GET_ARG(inst, 2));
	SET_REG(state, GET_ARG(inst, 0), result);
	ADVANCE_PP(state, inst);
}

/*
 * instruction_not - services opcode 14, not
 */
void instruction_not(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_NOT_DEBUG) || defined(BITWISE_DEBUG)
	printf("NOT %d %d\n", GET_ARG(inst, 0), GET_ARG(inst, 1));
	#endif

	uint16_t result = INTERP_VALUE(state, GET_ARG(inst, 1)) ^ (uint16_t) 32767;
	SET_REG(state, GET_ARG(inst, 0), result);
	ADVANCE_PP(state, inst);
}

/*
 * instruction_call - services opcode 17, call
 */
void instruction_call(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_CALL_DEBUG) || defined(JMP_DEBUG)
	printf("CALL %d\n", GET_ARG(inst, 0));
	#endif

	uint16_t old_pp = (state->pp - state->prog->bin + BIN_FIELD_WIDTH * 2) / BIN_FIELD_WIDTH;
	STACK_PUSH(state, old_pp);
	SET_PP(state, INTERP_VALUE(state, GET_ARG(inst, 0)));
}

/*
 * instruction_ret - services opcode 18, ret
 */
void instruction_ret(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_RET_DEBUG) || defined(JMP_DEBUG)
	printf("RET %d\n", GET_ARG(inst, 0));
	#endif

	if (state->sp == state->stack) {
		instruction_halt(state, inst);
	}

	STACK_POP(state, REG_PTR(state, GET_ARG(inst, 0)))
	uint16_t new_pp = state->prog->bin + (GET_ARG(inst, 0) + BIN_FIELD_WIDTH) * 2;
	SET_PP(state, new_pp);
}

/*
 * instruction_out - services opcode 19, out
 */
void instruction_out(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_OUT_DEBUG) || defined(IO_DEBUG)
	printf("OUT %d\n", GET_ARG(inst, 0));
	#endif

	printf("%c", GET_ARG(inst, 0));

	// advance program pointer
	ADVANCE_PP(state, inst);
}

/*
 * instruction_noop - services opcode 21, noop
 */
void instruction_noop(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_NOOP_DEBUG)
	printf("NOOP\n");
	#endif

	// advance program pointer
	ADVANCE_PP(state, inst);
};

