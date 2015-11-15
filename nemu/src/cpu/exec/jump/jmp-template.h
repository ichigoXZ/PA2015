#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute () {
	if(ops_decoded.opcode == 0xeb || ops_decoded.opcode == 0xe9){
		cpu.eip +=op_src->val;		
#if DATA_BYTE == 2 
		cpu.eip = cpu.eip & 0x0000ffff;
#endif
	}
	else if(ops_decoded.opcode == 0xff){
#if DATA_BYTE == 2
		cpu.eip = (op_src->val & 0x0000ffff) -2;
#elif DATA_BYTE == 4
		cpu.eip = op_src->val - 2;
#endif
	}
			
	print_asm_template1();
}

make_instr_helper(i) 
#if DATA_BYTE == 2 || DATA_BYTE == 4
	make_instr_helper(rm)
#endif

#include "cpu/exec/template-end.h"
