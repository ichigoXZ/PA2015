#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute () {
#if DATA_BYTE == 2
	//实现段寄存器
	uint32_t val = MEM_R(cpu.esp);
	OPERAND_W(op_dest,val);
	cpu.esp = cpu.esp + 2;
#elif DATA_BYTE == 4
	//实现段寄存器
	uint32_t val = MEM_R(cpu.esp);
	OPERAND_W(op_dest,val);
	cpu.esp = cpu.esp + 4;
#endif

	print_asm_template1();
}

make_instr_helper(r)

#include "cpu/exec/template-end.h"
