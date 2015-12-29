#include "cpu/exec/template-start.h"

#define instr call

static void do_execute () {
#if DATA_BYTE == 2
	cpu.esp = cpu.esp - 2;
	//实现段寄存器
	MEM_W(cpu.esp, cpu.eip);
	cpu.eip = (cpu.eip + op_src->val) & 0x0000ffff;
#elif DATA_BYTE == 4
	cpu.esp = cpu.esp - 4;
	//实现段寄存器
	MEM_W(cpu.esp, cpu.eip);
	cpu.eip = cpu.eip + op_src->val;
#endif
	print_asm_template1();
}

make_instr_helper(i) 
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_helper(concat(call_rm_, SUFFIX)){
	concat(decode_rm_,SUFFIX)(cpu.eip+1);
	if(2 == DATA_BYTE){
		cpu.esp =cpu.esp - 2;
		MEM_W(cpu.esp, (uint16_t)(cpu.eip& 0x0000ffff));
		cpu.eip = ( op_src->val)&0x0000ffff;
	}
	else if(4 == DATA_BYTE){
		printf("yes!\n");
		cpu.esp = cpu.esp-4;
		MEM_W(cpu.esp,  cpu.eip);
		cpu.eip =  op_src->val-2;
	}
	return 0;	
}
#endif

#include "cpu/exec/template-end.h"
