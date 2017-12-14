#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "program.h"

/*
 * init_program - creates a program object from a file on disk
 * param path - path on disk to program binary
 */
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
