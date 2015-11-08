#include "cpu/exec/template-start.h"

#define instr movzx

static void do_execute() {
#if DATA_BYTE == 1	
	printf("DATA_BYTE == 1\n");
	uint32_t val = 0x000000ff & op_src->val;
	OPERAND_W(op_dest,val);
#elif DATA_BYTE == 2
		printf("DATA_BYTE == 2\n");
	uint32_t val = 0X0000ffff & op_src->val;
	OPERAND_W(op_dest,val);
#else
		printf("DATA_BYTE == 4\n");
	OPERAND_W(op_dest, op_src->val);
#endif
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
