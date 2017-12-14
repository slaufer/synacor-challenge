#ifndef _PROGRAM_H
#define _PROGRAM_H

#include <stdint.h>

/*
 * struct program - program binary data
 * this usually shouldn't be modified at runtime
 */
struct program {
	size_t sz; // program size in bytes
	uint8_t *bin; // binary instruction data
};

typedef struct program program;


program *init_program(const char*);

#endif
