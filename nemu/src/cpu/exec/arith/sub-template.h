#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute () {
		#if DATA_BYTE == 1
		if(op_src->val&0x80){
			uint32_t val = 0xffffff00 | op_src->val;
			op_src->val = (DATA_TYPE) val;
		}
	#elif DATA_BYTE == 2
		if(op_src->val&0x8000){
			uint32_t val = 0xffff0000 | op_src->val;
			op_src->val = (DATA_TYPE) val;
		}
	#endif
		DATA_TYPE result = op_dest->val - op_src->val;
		OPERAND_W(op_dest, result);
				/* TODO: Update. */
	if(result == 0)
			cpu.ZF = 1;
			else 
			cpu.ZF = 0;
	if(op_dest->val < op_src->val)
			cpu.CF = 1;
			else 
			cpu.CF = 0;
#if DATA_BYTE == 1
		if(result&0x80)
			cpu.SF = 1;
			else 
			cpu.SF = 0;
		if(((op_dest->val&0x80)^(op_src->val&0x80))&&((op_dest->val&0x80)^(result&0x80)))
			cpu.OF = 1;
			else
			cpu.OF = 0;
#elif DATA_BYTE == 2
		if(result&0x8000)
			cpu.SF = 1;
			else 
			cpu.SF = 0;
		if(((op_dest->val&0x8000)^(op_src->val&0x8000))&&((op_dest->val&0x8000)^(result&0x8000)))
			cpu.OF = 1;
			else
			cpu.OF = 0;
#else 
		if(result&0x80000000)
			cpu.SF = 1;
			else 
			cpu.SF = 0;
		if(((op_dest->val&0x80000000)^(op_src->val&0x80000000))&&((op_dest->val&0x80000000)^(result&0x80000000)))
			cpu.OF = 1;
			else
			cpu.OF = 0;
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