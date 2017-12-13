#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include "vm.h"
#include "hexdump.h"


int instruction_nop() {
	return 0;
}

const int* const ops[] = {
	
};

/*****************************************************************
 * init_program - creates a program object from a file on disk *
 * param path - path on disk to program binary                   *
 *****************************************************************/
program *init_program(const char *path) {
	FILE *fh;
	program *prog;

	if (access(path, R_OK) == -1) {
		return (program*) NULL;
	}

	prog = (program*) malloc(sizeof(program));

	fh = fopen(path, "r");

	/* Get program size */
	fseek(fh, 0L, SEEK_END);
	prog->sz = ftell(fh);
	fseek(fh, 0L, SEEK_SET);

	/* Load program into memory */
	prog->bin = (uint8_t*) malloc(prog->sz);
	fread(prog->bin, prog->sz, 1, fh);
	fclose(fh);

	return prog;
}

/*******************************************************
 * init_execstate - creates a program execution state    *
 * param prog - program binary object to execute       *
 *******************************************************/
execstate *init_execstate(program *prog) {
	execstate *state = malloc(sizeof(execstate));

	state->prog = prog;
	state->pp = prog->bin;

	state->stack = NULL;
	state->sp = 0;

	state->heap = malloc(32768);
	state->regs = malloc(16);
}

/******************************************************************
 * init_instruction - creates an instruction object               *
 * return - an instruction object with space allocated for 3 args *
 ******************************************************************/
instruction *init_instruction() {
	instruction *inst = malloc(sizeof(instruction));
	inst->opcode = 0;
	inst->args = malloc(sizeof(uint16_t) * 3); // room for up to 3 args
	inst->nargs = 0;
}

/*************************************************************************************************
 * get_value - Grabs data from a pointer in the VM's int16 format, and converts it to a local    *
 *             unsigned short                                                                    *
 * param data - pointer to VM formatted int16                                                    *
 *************************************************************************************************/
unsigned short get_value(uint8_t *data) {
	// this will need to be changed if the local machine doesn't have MSB bit endianness.
	// any decent compiler *should* translate this locally into the relevant bitwise shift.
	return data[0] + (data[1] * 256);
}

/*********************************************************************************************
 * get_instruction - retrieves an instruction and its arguments from a given memory location *
 * param inst - pointer to an instruction object to be filled out                            *
 *********************************************************************************************/
void get_instruction(execstate *state, instruction *inst) {
	inst->opcode = get_value(state->pp);
	inst->nargs = INST_NARGS[inst->opcode];

	uint32_t i;
	for (i = 0; i < MAX_INST_ARGS && state->pp + i < state->pp + state->prog->sz; i++) {
		inst->args[i] = get_value(state->pp + BIN_FIELD_WIDTH * (i+1));
	} 

	printf("%d %d %d %d %d\n", inst->opcode, inst->nargs, inst->args[0], inst->args[1], inst->args[2]);
}

/********************************************
 * vm - main virtual machine execution loop *
 ********************************************/
void vm(execstate *state) {
	instruction *inst = init_instruction();

	get_instruction(state, inst);
}


int main (int argc, char **argv) {
	//////////////
	// GET PATH //
	//////////////

	// check if we got the right number of arguments
	if (argc != 2) {
		printf("Takes exactly one argument: path to a binary\n");
		return 1;
	}

	char *path = argv[1];

	/////////////////
	// LOAD BINARY //
	/////////////////

	program *prog = init_program(path);

	if (prog == NULL) {
		printf("Unable to open %s for reading\n", path);
		return 1;
	}

	printf("Loaded program from %s, size %d\n", path, prog->sz);

	/////////////////////
	// EXECUTE PROGRAM //
	/////////////////////

	execstate *state = init_execstate(prog);
	vm(state);

	return 0;
}
