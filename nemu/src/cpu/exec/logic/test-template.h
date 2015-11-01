#include "cpu/exec/template-start.h"

#define instr test

static void do_execute () {
	
	DATA_TYPE result = op_dest->val & op_src->val;
	OPERAND_W(op_dest, result);
                             
	/* TODO: Update EFLAGS. */
	cpu.CF = 0;
	cpu.OF = 0;
	if(result == 0)
		cpu.ZF = 1;
		else 
		cpu.ZF = 0;
#if DATA_TYPE == 1
		if(result&0x80)
			cpu.SF = 1;
			else 
			cpu.SF = 0;
#elif DATA_TYPE == 2
		if(result&0x8000)
			cpu.SF = 1;
			else 
			cpu.SF = 0;
#else 
		if(result&0x80000000)
			cpu.SF = 1;
			else 
			cpu.SF = 0;
#endif
	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"