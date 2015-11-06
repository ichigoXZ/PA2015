#include "cpu/exec/template-start.h"

#define instr setne

static void do_execute() {
	printf("setne: initial :op_dest->val%d\n",op_dest->val );
	if(cpu.ZF==0)
		op_dest->val = 1;
	else
		op_dest->val = 0;

	printf("setne:op_dest->val%d\n",op_dest->val );
	print_asm_template1();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"