#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "const.h"
#include "struct.h"
#include "vm.h"
#include "inst.h"
#include "program.h"

/*
 * init_execstate - creates a program execution state
 * param prog - program binary object to execute
 */
execstate *init_execstate(program *prog) {
	execstate *state = malloc(sizeof(execstate));

	state->prog = prog;
	state->pp = prog->bin;

	state->stack = malloc(HEAP_SIZE * BIN_FIELD_WIDTH);
	state->sp = state->stack;

	state->heap = malloc(HEAP_SIZE * BIN_FIELD_WIDTH);
	state->heap_size = HEAP_SIZE;

	state->regs = malloc(REGS_SIZE * BIN_FIELD_WIDTH);
	state->regs_size = REGS_SIZE;
	memset(state->regs, 0, REGS_SIZE * BIN_FIELD_WIDTH);

	return state;
}

/*
 * get_instruction - retrieves an instruction and its arguments from a given memory location
 * param inst - pointer to an instruction object to be filled out
 */
void get_instruction(execstate *state, instruction *inst) {
	inst->opcode = GET_MEM(state->pp);
	inst->args = state->pp + BIN_FIELD_WIDTH;

	#ifdef CORE_DEBUG
	printf("INSTRUCTION PP=%x OPCODE=%d NARGS=%d ARGS=%d,%d,%d\n", state->pp, inst->opcode, INST_NARGS[inst->opcode], inst->args[0], inst->args[1], inst->args[2]);
	#endif
}

/*
 * vm - main virtual machine execution loop
 */
void vm(execstate *state) {
	printf("=== VM Executing... ===\n");

	instruction *inst = init_instruction();

	while (1) {
		get_instruction(state, inst);
	
		if (inst->opcode >= INST_COUNT) {
			printf("=== UNKNOWN OPCODE %d, DYING ===\n", inst->opcode);
			exit(1);
		}

		INST_FNS[inst->opcode](state, inst);
	}
}


int main (int argc, char **argv) {
	//////////////
	// GET PATH //
	//////////////

	// check if we got the right number of arguments
	if (argc != 2) {
		printf("=== Takes exactly one argument: path to a binary ===\n");
		return 1;
	}

	char *path = argv[1];

	/////////////////
	// LOAD BINARY //
	/////////////////

	program *prog = init_program(path);

	if (prog == NULL) {
		printf("=== Unable to open %s for reading ===\n", path);
		return 1;
	}

	printf("=== Loaded program from %s, size %d ===\n", path, prog->sz);

	/////////////////////
	// EXECUTE PROGRAM //
	/////////////////////

	execstate *state = init_execstate(prog);
	vm(state);

	return 0;
}
