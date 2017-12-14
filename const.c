#include <stdint.h>

// names for instructions
const char* const INST_NAME[] = {
	"HALT", // 0
	"SET",  // 1
	"PUSH", // 2
	"POP",  // 3
	"EQ",   // 4
	"GT",   // 5
	"JMP",  // 6
	"JT",   // 7
	"JF",   // 8
	"ADD",  // 9
	"MULT", // 10
	"MOD",  // 11
	"AND",  // 12
	"OR",   // 13
	"MOD",  // 14
	"RMEM", // 15
	"WMEM", // 16
	"CALL", // 17
	"RET",  // 18
	"OUT",  // 19
	"IN",   // 20
	"NOOP"  // 21
 };

// argument counts for instructions
const uint8_t const INST_NARGS[] = {
	0, // opcode 0: halt
	2, // opcode 1: set
	1, // opcode 2: push
	1, // opcode 3: pop
	3, // opcode 4: eq
	3, // opcode 5: gt
	1, // opcode 6: jmp
	2, // opcode 7: jt
	2, // opcode 8: jf
	3, // opcode 9: add
	3, // opcode 10: mult
	3, // opcode 11: mod
	3, // opcode 12: and
	3, // opcode 13: or
	2, // opcode 14: mod
	2, // opcode 15: rmem
	2, // opcode 16: wmem
	1, // opcode 17: call
	0, // opcode 18: ret
	1, // opcode 19: out
	1, // opcode 20: in
	0, // opcode 21: noop
};
