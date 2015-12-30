
#include "cpu/exec/template-start.h"
#define instr call


static void do_execute (){
	int len=concat(decode_i_, SUFFIX)(cpu.eip + 1) + 1;
	cpu.esp -= DATA_BYTE;
	if(4 == DATA_BYTE){
		MEM_W(cpu.esp,  cpu.eip + len);
		cpu.eip = cpu.eip+ op_src->val;
	}
	else if(2 == DATA_BYTE){
		MEM_W(cpu.esp, (uint16_t)((cpu.eip + len) & 0x0000ffff));
		cpu.eip = (cpu.eip + op_src->val)&0x0000ffff;
	}
	print_asm_template1();
}

make_instr_helper(i)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_helper(concat(call_rm_, SUFFIX)){
	int len=concat(decode_rm_, SUFFIX)(eip + 1) + 1;
	cpu.esp -= DATA_BYTE;
	if(4 == DATA_BYTE){
		MEM_W(cpu.esp,  cpu.eip + len);
		cpu.eip =  op_src->val;
	}
	else if(2 == DATA_BYTE){
		MEM_W(cpu.esp, (uint16_t)((cpu.eip+len)& 0x0000ffff));
		cpu.eip = ( op_src->val)&0x0000ffff;
	}
	print_asm_template1();
	return 0;	
}
#endif

#include "cpu/exec/template-end.h"