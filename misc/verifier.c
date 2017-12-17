#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define STACK_SIZE 65336
#define MATH_MOD 32768

uint16_t verify2(uint16_t arg0, uint16_t arg1, uint16_t seed) {
	if (!arg0) return arg1 + 1;
	if (!arg1) return verify2((arg0 + 32767) % MATH_MOD, seed, seed);

	arg1 = verify2(arg0, (arg1 + 32767) % MATH_MOD, seed);
	return verify2(arg0, arg1, seed);
}

void verify(uint16_t *arg0, uint16_t *arg1, uint16_t *arg7, uint16_t *stack, uint64_t *sp) {
	                                         // LAB0:                       |
	if (*arg0 == 0) {                        // 6027 JT   $0    LAB1        | 6027 JT   $0    6035
		*arg0 = (*arg1 + 1) % MATH_MOD;      // 6030 ADD  $0    $1    1     | 6030 ADD  $0    $1    1     
		return;                              // 6034 RET                    | 6034 RET                    
	}                                        //                             |
	                                         // LAB1:                       |
	if (*arg1 == 0) {                        // 6035 JT   $1    LAB2        | 6035 JT   $1    6048
		*arg0 = (*arg0 + 32767) % MATH_MOD;  // 6038 SUB  $0    $0    1     | 6038 ADD  $0    $0    32767
		*arg1 = *arg7;                       // 6042 SET  $1    $7          | 6042 SET  $1    $7          
		verify(arg0, arg1, arg7, stack, sp); // 6045 CALL LAB0              | 6045 CALL 6027
		return;                              // 6047 RET                    | 6047 RET                    
	}                                        //                             |
	                                         // LAB2:                       |
	if (*sp == STACK_SIZE) {
		printf("STACK OVERFLOW\n");
		exit(1);
	}
	stack[*sp] = *arg0;                      // 6048 PUSH $0                | 6048 PUSH $0                
	*sp += 1;                                //                             |
	*arg1 = (*arg1 + 32767) % MATH_MOD;      // 6050 SUB  $1    $1    1     | 6050 ADD  $1    $1    32767
	verify(arg0, arg1, arg7, stack, sp);     // 6054 CALL LAB0              | 6054 CALL 6027
	*arg1 = *arg0;                           // 6056 SET  $1    $0          | 6056 SET  $1    $0          
	*sp -= 1;                                // 6059 POP  $0                | 6059 POP  $0                
	*arg0 = stack[*sp];                      //                             |
	*arg0 = (*arg0 + 32767) % MATH_MOD;      // 6061 SUB  $0    $0    1     | 6061 ADD  $0    $0    32767
	verify(arg0, arg1, arg7, stack, sp);     // 6065 CALL LAB0              | 6065 CALL 6027
	return;                                  // 6067 RET                    | 6067 RET                    
}

uint16_t main(int argc, char **argv) {
	uint16_t stack[STACK_SIZE];
	uint64_t sp = 0;
	
	uint16_t test_val = atoi(argv[1]);

	uint16_t arg0 = 4;
	uint16_t arg1 = 1;
	uint16_t arg7 = test_val;

	printf("Testing %hu... ", test_val);
	verify(&arg0, &arg1, &arg7, stack, &sp);
	// verify2(arg0, arg1, arg7);
	printf("Result: %hu\n", arg0);

	return 0;
}
