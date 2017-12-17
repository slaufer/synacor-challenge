#include <stdio.h>
#include <stdlib.h>
#include "program.h"
#include "struct.h"
#include "const.h"
#include "vm.h"

void get_instruction(execstate *state, instruction *inst) {
	inst->opcode = state->mem[state->pp];

	int i;
	for (i = 0; i < INST_MAX_ARGS; i++) {
		inst->args[i] = state->mem[state->pp + i + 1];
	}
}

void clear_state(execstate *state) {
	state->pp = 0;
	state->sp = 0;

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

int main(int argc, char **argv) {
	//////////////
	// GET PATH //
	//////////////

	// check if we got the right number of arguments
	if (argc != 4) {
		fprintf(stderr, "=== Takes three arguments: program path, start pointer, instruction count ===\n");
		return 1;
	}

	char *path = argv[1];
	uint16_t start = atoi(argv[2]);
	uint16_t count = atoi(argv[3]);

	/////////////////
	// LOAD BINARY //
	/////////////////

	execstate state;
	clear_state(&state);
	uint16_t prog_len = load_program(&state, path);

	if (prog_len == 0) {
		fprintf(stderr, "=== Unable to open %s for reading ===\n", path);
		return 1;
	}

	int i;
	instruction inst;
	while (state.pp < prog_len) {
		get_instruction(&state, &inst);

		if (inst.opcode >= INST_COUNT) {
			printf("%-5hu INV  %-5hu\n", state.pp, inst.opcode);
			state.pp++;
			continue;
		}

		printf("%-5hu %-4s", state.pp, INST_NAME[inst.opcode]);

		for (i = 0; i < INST_NARGS[inst.opcode]; i++) {
			if (IS_REG(inst.args[i])) {
				printf(" $%-4hu", TO_REG(inst.args[i]));
			} else {
				printf(" %-5hu", inst.args[i]);
			}
		}

		printf("\n");

		state.pp += INST_NARGS[inst.opcode] + 1;
	}

	return 0;
}
