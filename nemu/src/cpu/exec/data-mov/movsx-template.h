#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute() {
#if DATA_BYTE == 1
	//printf("DATA_BYTE == 1\n");
		if(op_src->val&0x80){
			printf("ini:%x\n",op_src->val );
			uint32_t val = 0xffffff00 | op_src->val;
			printf("%x\n",val );
			OPERAND_W(op_dest,op_src->val);
			printf("op_src:%x\n",op_dest->val );
			OPERAND_W(op_dest,val);
			printf("val--dest:%x\n",op_dest->val);
		}
		else 
	OPERAND_W(op_dest, op_src->val);
#elif DATA_BYTE == 2 
	//printf("DATA_BYTE == 2\n");
			if(op_src->val&0x8000){
			uint32_t val = 0xffff0000 | op_src->val;
			OPERAND_W(op_dest,val);
		}
		else 
	OPERAND_W(op_dest, op_src->val);
#else
	//printf("DATA_BYTE == 4\n");
	OPERAND_W(op_dest, op_src->val);
#endif

	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
