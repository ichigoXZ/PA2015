#include "cpu/exec/template-start.h"

#define instr movzx

static void do_execute() {
#if DATA_BYTE == 1	
	uint32_t val = 0x00ff & op_src->val;
	write_operand_l(op_dest,val);
#elif DATA_BYTE == 2
	uint32_t val = 0X0000ffff & op_src->val;
	write_operand_l(op_dest,val);
#else
	write_operand_l(op_dest,op_src->val);
#endif
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
