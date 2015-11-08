#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute() {
	uint32_t result = 0;

	switch(ops_decoded.opcode & 0xff){
		case 0xbe:
			result = (uint32_t)(int32_t)(int8_t)op_src->val;
			break;
		case 0xbf:
			result = (uint32_t)(int32_t)(int16_t)op_src->val;
			break;
		default:
			break;
	}
	OPERAND_W(op_dest,result);

	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
