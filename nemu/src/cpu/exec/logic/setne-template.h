#include "cpu/exec/template-start.h"

#define instr setne

static void do_execute() {
	if(cpu.ZF==0)
		op_dest->val = 1;
	else
		op_dest->val = 0;

	print_asm_template1();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"