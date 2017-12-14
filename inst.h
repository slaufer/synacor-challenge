#ifndef _INST_H
#define _INST_H

#include <stdint.h>
#include "vm.h"

// creates instruction objects
instruction *init_instruction();

// individual opcode instructions
void instruction_halt(execstate*, instruction*); //  0
void instruction_set (execstate*, instruction*); //  1
void instruction_push(execstate*, instruction*); //  2
void instruction_pop (execstate*, instruction*); //  3
void instruction_eq  (execstate*, instruction*); //  4
void instruction_gt  (execstate*, instruction*); //  5
void instruction_jmp (execstate*, instruction*); //  6
void instruction_jt  (execstate*, instruction*); //  7
void instruction_jf  (execstate*, instruction*); //  8
void instruction_add (execstate*, instruction*); //  9
void instruction_mult(execstate*, instruction*); // 10
void instruction_mod (execstate*, instruction*); // 11
void instruction_and (execstate*, instruction*); // 12
void instruction_or  (execstate*, instruction*); // 13
void instruction_not (execstate*, instruction*); // 14
void instruction_rmem(execstate*, instruction*); // 15
void instruction_call(execstate*, instruction*); // 17
void instruction_ret (execstate*, instruction*); // 18
void instruction_out (execstate*, instruction*); // 19
void instruction_noop(execstate*, instruction*); // 21

void (*INST_FNS[])(execstate*, instruction*) = {
	instruction_halt, // opcode 0: halt
	instruction_set,  // opcode 1: set
	instruction_push, // opcode 2: push
	instruction_pop,  // opcode 3: pop
	instruction_eq,   // opcode 4: eq
	instruction_gt,   // opcode 5: gt
	instruction_jmp,  // opcode 6: jmp
	instruction_jt,   // opcode 7: jt
	instruction_jf,   // opcode 8: jf
	instruction_add,  // opcode 9: add
	instruction_mult, // opcode 10: mult
	instruction_mod,  // opcode 11: mod
	instruction_and,  // opcode 12: and
	instruction_or,   // opcode 13: or
	instruction_not,  // opcode 14: not
	instruction_rmem, // opcode 15: rmem
	instruction_halt, // opcode 16: wmem
	instruction_call, // opcode 17: call
	instruction_ret,  // opcode 18: ret
	instruction_out,  // opcode 19: out
	instruction_halt, // opcode 20: in
	instruction_noop, // opcode 21: noop
};

#endif
