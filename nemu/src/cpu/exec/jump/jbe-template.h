#include "cpu/exec/template-start.h"

#define instr jbe

static void do_execute () {
	if(cpu.CF || cpu.ZF){
#if DATA_TYPE == 1
		cpu.eip = (cpu.eip&0xffffff00)+op_src->val;
#elif DATA_TYPE == 2 
		cpu.eip = (cpu.eip&0xffffff00)+op_src->val;
#else
		cpu.eip = cpu.eip+op_src->val;
#endif
	}

	print_asm_template1();
}

make_instr_helper(i) 

#include "cpu/exec/template-end.h"
