#include "cpu/exec/template-start.h"

#define instr jbe

static void do_execute () {
	if(cpu.CF || cpu.ZF){
#if DATA_BYTE == 1
		cpu.eip = (cpu.eip&0xffffff00)+op_src->val;
#elif DATA_BYTE == 2 
		cpu.eip = (cpu.eip&0xffffff00)+op_src->val;
		cpu.eip = cpu.eip & 0x0000ffff;
#else
		cpu.eip = cpu.eip+op_src->val;
#endif
	}

	print_asm_template1();
}

make_instr_helper(i) 

#include "cpu/exec/template-end.h"
