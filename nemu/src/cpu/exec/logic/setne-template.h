#include "cpu/exec/template-start.h"

#define instr setne

static void do_execute() {
	printf("%d\n",cpu.ZF );
	if(cpu.ZF==0){
		op_dest->val = 1;
		printf("op_dest->val = 1\n");
	}
	else{
		op_dest->val = 0;
		printf("op_dest-.val = 0\n");
		
	}

	print_asm_template1();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"