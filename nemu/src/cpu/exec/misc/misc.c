#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(nop) {
	print_asm("nop");
	return 1;
}

make_helper(int3) {
	void do_int3();
	do_int3();
	print_asm("int3");

	return 1;
}

make_helper(lea) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = load_addr(eip + 1, &m, op_src);
	reg_l(m.reg) = op_src->addr;

	print_asm("leal %s,%%%s", op_src->str, regsl[m.reg]);
	return 1 + len;
}

make_helper(leave) {
#if DATA_TYPE == 2 || DATA_TYPE == 4
	cpu.esp = cpu.ebp;

	cpu.ebp = MEM_R(cpu.esp);
	cpu.esp = cpu.esp + DATA_TYPE;
#endif

	print_asm("leave");
	return 1;
}

make_helper(ret) {
	#if DATA_BYTE == 2
	//实现段寄存器
	cpu.eip = MEM_R(cpu.esp);
	cpu.esp = cpu.esp + 2;
	cpu.eip = cpu.eip & 0x0000ffff;
#elif DATA_BYTE == 4
	//实现段寄存器
	cpu.eip = MEM_R(cpu.esp);
	cpu.esp = cpu.esp + 4;
#endif

	print_asm("ret");
	return 1;
}