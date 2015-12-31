/*#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute() {
	uint32_t val = 0;
if (DATA_BYTE == 1){
		if(op_src->val&0x80)
			val = 0xffffff00 | op_src->val;
		else 
			val = 0x000000ff & op_src->val;
		write_operand_l(op_dest,val);
	}
else if (DATA_BYTE == 2 ){
		if(op_src->val&0x8000)
			val = 0xffff0000 | op_src->val;
		else
			val = 0x0000ffff & op_src->val;
		write_operand_l(op_dest,val);
	}
else
	write_operand_l(op_dest, op_src->val);

	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
*/
#include "cpu/exec/template-start.h"
#define instr movsx

static void do_execute (){
	if(DATA_BYTE ==2){
		int16_t signExtend = (int16_t)((int8_t) op_src->val);
		OPERAND_W(op_dest , signExtend);
	}else{
		if(ops_decoded.opcode == (0xbf | 0x100)){
			int32_t signExtend = (int32_t)((int16_t) op_src->val);
			OPERAND_W(op_dest , signExtend);
		}else{
			int32_t signExtend = (int32_t)((int8_t) op_src->val);
			OPERAND_W(op_dest , signExtend);
		}
	}
	
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"