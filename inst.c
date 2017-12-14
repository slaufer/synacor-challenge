#include <stdio.h>
#include <stdlib.h>
#include "inst.h"


// methods to service individual instructions
void (*INST_FNS[])(execstate*, instruction*) = {
	inst_halt, // opcode 0: halt
	inst_halt, // opcode 1: set
	inst_halt, // opcode 2: push
	inst_halt, // opcode 3: pop
	inst_halt, // opcode 4: eq
	inst_halt, // opcode 5: gt
	inst_jmp, // opcode 6: jmp
	inst_jt, // opcode 7: jt
	inst_jf, // opcode 8: jf
	inst_halt, // opcode 9: add
	inst_halt, // opcode 10: mult
	inst_halt, // opcode 11: mod
	inst_halt, // opcode 12: and
	inst_halt, // opcode 13: or
	inst_halt, // opcode 14: mod
	inst_halt, // opcode 15: rmem
	inst_halt, // opcode 16: wmem
	inst_halt, // opcode 17: call
	inst_halt, // opcode 18: ret
	inst_out, // opcode 19: out
	inst_halt, // opcode 20: in
	inst_noop, // opcode 21: noop
};

// argument counts for instructions
uint8_t INST_NARGS[] = {
	0, // opcode 0: halt
	2, // opcode 1: set
	1, // opcode 2: push
	1, // opcode 3: pop
	3, // opcode 4: eq
	3, // opcode 5: gt
	1, // opcode 6: jmp
	2, // opcode 7: jt
	2, // opcode 8: jf
	3, // opcode 9: add
	3, // opcode 10: mult
	3, // opcode 11: mod
	3, // opcode 12: and
	3, // opcode 13: or
	2, // opcode 14: mod
	2, // opcode 15: rmem
	2, // opcode 16: wmem
	1, // opcode 17: call
	0, // opcode 18: ret
	1, // opcode 19: out
	1, // opcode 20: in
	0, // opcode 21: noop
};

/*
 * init_instruction - creates an instruction object
 * return - an instruction object with space allocated for 3 args
 */
instruction *init_instruction() {
	instruction *inst = malloc(sizeof(instruction));
	inst->opcode = 0;
	inst->args = (uint16_t*) malloc(sizeof(uint16_t) * INST_MAX_ARGS); // room for up to 3 args
	inst->nargs = 0;
}

/*
 * inst_halt - services opcode 0 (halt)
 *             also used for unknown opcodes
 */
void inst_halt(execstate *state, instruction *inst) {
	if (inst->opcode == 0) {
		printf("\n=== HALT ===\n");
	} else {
		printf("\n=== HALT UNIMPLEMENTED OPCODE=%d NARGS=%d ARGS=%d,%d,%d ===\n",
			inst->opcode, inst->nargs, inst->args[0], inst->args[1], inst->args[2]);
	}

	// don't bother doing anything else, just exit
	exit(0);
};

/*
 * inst_jmp - services opcode 6, jmp
 */
void inst_jmp(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_JMP_DEBUG)
	printf("JMP %d (0x%08x => 0x%08x)\n",
		inst->args[0], state->pp, state->prog->bin + inst->args[0] * BIN_FIELD_WIDTH);
	#endif

	// new program pointer is base program memory pointer + jump location argument * field width
	SET_PP(state, inst->args[0]);
}

/*
 * inst_jt - services opcode 7, jt
 */
void inst_jt(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_JT_DEBUG)
	printf("JT %d %d (0x%08x => 0x%08x)\n",
		inst->args[0], inst->args[1], state->pp, state->prog->bin + inst->args[1] * BIN_FIELD_WIDTH);
	#endif

	if (inst->args[0]) {
		SET_PP(state, inst->args[1]);
	} else {
		ADVANCE_PP(state, inst);
	}
}

/*
 * inst_jf - services opcode 8, jf
 */
void inst_jf(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_JF_DEBUG)
	printf("JF %d %d (0x%08x => 0x%08x)\n",
		inst->args[0], inst->args[1], state->pp, state->prog->bin + inst->args[1] * BIN_FIELD_WIDTH);
	#endif

	if (inst->args[0] == 0) {
		SET_PP(state, inst->args[1]);
	} else {
		ADVANCE_PP(state, inst);
	}
}

/*
 * inst_out - services opcode 19, out
 */
void inst_out(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_OUT_DEBUG)
	printf("OUT %d\n", inst->args[0]);
	#endif

	printf("%c", inst->args[0]);

	// advance program pointer
	ADVANCE_PP(state, inst);
}

/*
 * inst_noop - services opcode 21, noop
 */
void inst_noop(execstate *state, instruction *inst) {
	#if defined(INST_DEBUG) || defined(INST_NOOP_DEBUG)
	printf("NOOP\n");
	#endif

	// advance program pointer
	ADVANCE_PP(state, inst);
};
