#include "cpu/exec/template-start.h"

#define instr call

static void do_execute () {
	if(ops_decoded.opcode == 0xe8){
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
	}
	else {
#if DATA_BYTE == 2
	cpu.esp = cpu.esp - 2;
	//实现段寄存器
	MEM_W(cpu.esp, cpu.eip);
	cpu.eip = (op_src->val & 0x0000ffff) -2;
#elif DATA_BYTE == 4
	cpu.esp = cpu.esp - 4;
	//实现段寄存器
	MEM_W(cpu.esp,cpu.eip);
	cpu.eip += op_src->val;
#endif
	}
	print_asm_template1();
}

make_instr_helper(i) 
#if DATA_BYTE == 2 || DATA_BYTE == 4
	make_instr_helper(rm)
#endif

#include "cpu/exec/template-end.h"
