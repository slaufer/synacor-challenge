#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "struct.h"
#include "const.h"

/*
 * init_program - creates a program object from a file on disk
 * param path - path on disk to program binary
 */
uint16_t load_program(execstate *state, const char *path) {
	// make sure program is accessible
	if (access(path, R_OK) == -1) {
		return 0;
	}

	FILE *fh = fopen(path, "r");

	if (fh == NULL) {
		return 0;
	}

	// Get program size
	fseek(fh, 0L, SEEK_END);
	uint16_t prog_size = ftell(fh);
	fseek(fh, 0L, SEEK_SET);

	// Load program into buffer
	uint8_t *prog_buf = (uint8_t*) malloc(prog_size);
	fread(prog_buf, prog_size, 1, fh);
	fclose(fh);

	// convert program and insert into memory
	int i;
	for (i = 0; i < prog_size; i += 2) {
		uint8_t *src = prog_buf + i;
		state->mem[i / 2] = src[0] + (uint16_t) src[1] * 256;
	}

	free(prog_buf);

	return i / 2;
}
