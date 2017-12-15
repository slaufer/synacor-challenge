#ifndef _INST_H
#define _INST_H

#include <stdint.h>
#include "struct.h"

void instruction_debug(execstate *state, instruction *inst);
instruction *init_instruction();

extern void (*INST_FNS[])(execstate*, instruction*);

#endif
