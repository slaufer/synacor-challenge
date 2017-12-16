#ifndef _INST_H
#define _INST_H

#include <stdint.h>
#include "struct.h"

extern void (*INST_FNS[])(execstate*, instruction*);

#endif
