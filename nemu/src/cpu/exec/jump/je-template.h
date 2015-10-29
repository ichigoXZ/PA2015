#include "cpu/exec/template-start.h"

#define instr je

static void do_execute () {

#if DATA_TYPE == 2
	if(cpu.Eflags.ZF)
		cpu.eip += op_src->val;
#endif

	print_asm_template1();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"