#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute () {
#if DATA_BYTE == 1
	op_src->val = MEM_R(cpu.esp);
	cpu.esp = cpu.esp +1;
#elif DATA_BYTE == 2
	//实现段寄存器
	op_src->val = MEM_R(cpu.esp);
	cpu.esp = cpu.esp + 2;
#else 
	//实现段寄存器
	op_src->val = MEM_R(cpu.esp);
	cpu.esp = cpu.esp + 4;
#endif

	print_asm_template1();
}

make_instr_helper(r)

#include "cpu/exec/template-end.h"
