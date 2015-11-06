#include "cpu/exec/template-start.h"

#define instr jle

static void do_execute () {
	if(cpu.ZF && cpu.SF != cpu.OF){
#if DATA_BYTE == 1
		if(op_src->val&0x80){
			uint32_t val = 0xffffff00 | op_src->val;
			cpu.eip += val;
		}
		else 
			cpu.eip +=op_src->val;
#elif DATA_BYTE == 2 
			cpu.eip += op_src->val;
		cpu.eip = cpu.eip & 0x0000ffff;
#endif
	}

	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"