#include "cpu/exec/template-start.h"

#define instr jbe

static void do_execute () {
	if(cpu.Eflags.CF || cpu.Eflags.ZF)
	cpu.eip += op_src->val;
#if DATA_BYTE == 2
	cpu.eip = (cpu.eip + op_src->val) & 0x0000ffff;
#endif

	print_asm_template1();
}

make_instr_helper(i) 

#include "cpu/exec/template-end.h"
