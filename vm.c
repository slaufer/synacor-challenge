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
 * get_instruction - retrieves an instruction and its arguments from a given memory location
 * param inst - pointer to an instruction object to be filled out
 */
void get_instruction(execstate *state, instruction *inst) {
	inst->opcode = state->mem[state->pp];

	int i;
	for (i = 0; i < INST_MAX_ARGS; i++) {
		inst->args[i] = state->mem[state->pp + i + 1];
	}
}

/*
 * vm - main virtual machine execution loop
 */
void vm(execstate *state) {
	printf("=== VM Executing... ===\n");

	instruction inst;

	while (1) {
		get_instruction(state, &inst);
	
		if (state->pp == 5451) {
			state->regs[7] = 1;
		}
	
		if (inst.opcode >= INST_COUNT) {
			printf("=== UNKNOWN OPCODE %hu ===\n", inst.opcode);
			exit(1);
		}

		INST_FNS[inst.opcode](state, &inst);
	}
}

void clear_state(execstate *state) {
	int i;
	for (i = 0; i < REGS_SIZE; i++) {
		state->regs[i] = 0;
	}

	for (i = 0; i < MEM_SIZE; i++) {
		state->mem[i] = 0;
	}

	for (i = 0; i < STACK_SIZE; i++) {
		state->stack[i] = 0;
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

	execstate state;
	clear_state(&state);
	uint16_t prog_len = load_program(&state, path);

	if (prog_len == 0) {
		printf("=== Unable to open %s for reading ===\n", path);
		return 1;
	}

	printf("=== Loaded program from %s, %hu instructions ===\n", path, prog_len);

	/////////////////////
	// EXECUTE PROGRAM //
	/////////////////////

	vm(&state);

	return 0;
}
