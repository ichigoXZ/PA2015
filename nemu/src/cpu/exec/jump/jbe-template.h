#include "cpu/exec/template-start.h"

#define instr jbe

static void do_execute () {
	if(cpu.CF || cpu.ZF){
		cpu.eip = cpu.eip + op_src->val;
		printf("%d\n",DATA_BYTE );
#if DATA_BYTE == 1
		cpu.eip = cpu.eip & 0x00ff;
#elif DATA_BYTE == 2 
		cpu.eip = cpu.eip & 0x0000ffff;
#endif
	}

	print_asm_template1();
}

make_instr_helper(i) 

#include "cpu/exec/template-end.h"
