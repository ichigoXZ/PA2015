#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute () {
		DATA_TYPE result = op_dest->val - op_src->val;
		OPERAND_W(op_dest, result);

				/* TODO: Update EFLAGS. */
		if(result == 0)
			cpu.Eflags.ZF = 1;
			else 
			cpu.Eflags.ZF = 0;

#if DATA_TYPE == 1
		if(result&0x80)
			cpu.Eflags.SF = 0;
			else 
			cpu.Eflags.SF = 1;
		if(((op_dest->val&0x80)^(op_src->val&0x80))&&((op_dest->val&0x80)^(result&0x8000)))
			cpu.Eflags.OF = 1;
			else
			cpu.Eflags.OF = 0;
#elif DATA_TYPE == 2
		if(result&0x8000)
			cpu.Eflags.SF = 0;
			else 
			cpu.Eflags.SF = 1;
		if(((op_dest->val&0x8000)^(op_src->val&0x8000))&&((op_dest->val&0x8000)^(result&0x8000)))
			cpu.Eflags.OF = 1;
			else
			cpu.Eflags.OF = 0;
#else 
		if(result&0x80000000)
			cpu.Eflags.SF = 0;
			else 
			cpu.Eflags.SF = 1;
		if(((op_dest->val&0x80000000)^(op_src->val&0x80000000))&&((op_dest->val&0x80000000)^(result&0x8000)))
			cpu.Eflags.OF = 1;
			else
			cpu.Eflags.OF = 0;
#endif

		print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"