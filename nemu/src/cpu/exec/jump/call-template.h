#include "cpu/exec/template-start.h"

#define instr call

static void do_execute () {
	printf("%d\n",op_src->val);
#if DATA_BYTE == 2
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

make_instr_helper(rm) 

#include "cpu/exec/template-end.h"
