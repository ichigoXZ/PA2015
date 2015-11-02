#include "cpu/exec/template-start.h"

#define instr jbe

static void do_execute () {
	if(cpu.CF || cpu.ZF){
		printf("%x\n", cpu.eip);
		printf("%x\n", op_src->val);
		cpu.eip += 1;
		printf("%x\n",cpu.eip );
		if(DATA_BYTE == 2)
			cpu.eip = cpu.eip & 0x0000ffff;
	}

	print_asm_template1();
}

make_instr_helper(i) 

#include "cpu/exec/template-end.h"
