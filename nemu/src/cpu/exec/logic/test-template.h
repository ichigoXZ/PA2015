#include "cpu/exec/template-start.h"

#define instr test

static void do_execute () {
	
	DATA_TYPE result = op_dest->val & op_src->val;
	OPERAND_W(op_dest, result);
                             
	/* TODO: Update EFLAGS. */
	cpu.Eflags.CF = 0;
	cpu.Eflags.OF = 0;
	if(result == 0)
		cpu.Eflags.ZF = 1;
		else 
		cpu.Eflags.ZF = 0;


	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"