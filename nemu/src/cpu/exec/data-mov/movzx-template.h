/*#include "cpu/exec/template-start.h"

#define instr movzx

static void do_execute() {
#if DATA_BYTE == 1	
	uint32_t val = 0x000000ff & op_src->val;
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
*/
#include "cpu/exec/template-start.h"
#define instr movzx

static void do_execute (){
	uint32_t zeroExtend = 0;
	if(DATA_BYTE == 2)
		zeroExtend = 0x000000ff & op_src->val;
	else{
		if(ops_decoded.opcode == (0xb6 | 0x100)){
			zeroExtend = 0x000000ff & op_src->val;
		}else{
			zeroExtend = 0x0000ffff & op_src->val;
		}
	}
		
	write_operand_l(op_dest , zeroExtend);
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
