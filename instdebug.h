#ifndef _INSTDEBUG_H
#define _INSTDEBUG_H

// debug individual instructions
#ifndef INST_HALT_DEBUG
#define INST_HALT_DEBUG 0
#endif

#ifndef INST_SET_DEBUG
#define INST_SET_DEBUG 0
#endif

#ifndef INST_PUSH_DEBUG
#define INST_PUSH_DEBUG 0
#endif

#ifndef INST_POP_DEBUG
#define INST_POP_DEBUG 0
#endif

#ifndef INST_EQ_DEBUG
#define INST_EQ_DEBUG 0
#endif

#ifndef INST_GT_DEBUG
#define INST_GT_DEBUG 0
#endif

#ifndef INST_JMP_DEBUG
#define INST_JMP_DEBUG 0
#endif

#ifndef INST_JT_DEBUG
#define INST_JT_DEBUG 0
#endif

#ifndef INST_JF_DEBUG
#define INST_JF_DEBUG 0
#endif

#ifndef INST_ADD_DEBUG
#define INST_ADD_DEBUG 0
#endif

#ifndef INST_MULT_DEBUG
#define INST_MULT_DEBUG 0
#endif

#ifndef INST_MOD_DEBUG
#define INST_MOD_DEBUG 0
#endif

#ifndef INST_AND_DEBUG
#define INST_AND_DEBUG 0
#endif

#ifndef INST_OR_DEBUG
#define INST_OR_DEBUG 0
#endif

#ifndef INST_NOT_DEBUG
#define INST_NOT_DEBUG 0
#endif

#ifndef INST_RMEM_DEBUG
#define INST_RMEM_DEBUG 0
#endif

#ifndef INST_WMEM_DEBUG
#define INST_WMEM_DEBUG 0
#endif

#ifndef INST_CALL_DEBUG
#define INST_CALL_DEBUG 0
#endif

#ifndef INST_RET_DEBUG
#define INST_RET_DEBUG 0
#endif

#ifndef INST_OUT_DEBUG
#define INST_OUT_DEBUG 0
#endif

#ifndef INST_IN_DEBUG
#define INST_IN_DEBUG 0
#endif

#ifndef INST_NOOP_DEBUG
#define INST_NOOP_DEBUG 0
#endif


// debug instruction groups
#ifndef INST_CORE_DEBUG
#define INST_CORE_DEBUG 0
#endif

#ifndef INST_REG_DEBUG
#define INST_REG_DEBUG 0
#endif

#ifndef INST_STACK_DEBUG
#define INST_STACK_DEBUG 0
#endif

#ifndef INST_COMP_DEBUG
#define INST_COMP_DEBUG 0
#endif

#ifndef INST_IO_DEBUG
#define INST_IO_DEBUG 0
#endif

#ifndef INST_JMP_DEBUG
#define INST_JMP_DEBUG 0
#endif

#ifndef INST_HEAP_DEBUG
#define INST_HEAP_DEBUG 0
#endif

#ifndef INST_BITWISE_DEBUG
#define INST_BITWISE_DEBUG 0
#endif

#ifndef INST_MATH_DEBUG
#define INST_MATH_DEBUG 0
#endif

// debug all instructions
#ifndef INST_DEBUG
#define INST_DEBUG 0
#endif

// debug macro
#define INST_DEBUG_MSG(state, inst, flag) if (flag | INST_DEBUG | INST_DEBUG_OVERRIDE) instruction_debug(state, inst);

// debug override so we can enable it at runtime
uint8_t INST_DEBUG_OVERRIDE = 0;

#endif
