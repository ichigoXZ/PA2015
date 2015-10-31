#include "cpu/exec/template-start.h"

#define instr call

static void do_execute () {
	printf("%d\n",op_src->val);


	print_asm_template1();
}

make_instr_helper(rm) 

#include "cpu/exec/template-end.h"
