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
	if(ops_decoded.is_data_size_16 == 1){
		cpu.esp = cpu.ebp;
		cpu.ebp = swaddr_read(cpu.esp, 2);
		cpu.esp = cpu.esp + 2;
	}
	else{
		cpu.esp = cpu.ebp;
		cpu.ebp = swaddr_read(cpu.esp, 4);
		cpu.esp = cpu.esp + 4;
	}
	print_asm("leave");
	return 1;
}

make_helper(ret) {
	if(ops_decoded.is_data_size_16 == 1){
		cpu.eip = swaddr_read(cpu.esp,2);
		cpu.esp = cpu.esp + 2;
		cpu.eip = cpu.eip & 0x0000ffff;
	}
	else{
		cpu.eip = swaddr_read(cpu.esp,4);
		cpu.esp = cpu.esp + 4;
	}
	print_asm("ret");
	return decode_i_l(cpu.eip)+1;
}

make_helper(cltd) {
	if(ops_decoded.is_data_size_16 == 1){
		if(cpu.eax & 0x00008000)
			cpu.edx |= 0x0000ffff;
		else
			cpu.edx &= 0xffff0000;
	}
	else{
		if(cpu.eax & 0x80000000)
			cpu.edx = 0xffffffff;
		else
			cpu.edx = 0;
	}

	print_asm("cltd");
	return 1;
}

make_helper(cld) {
	cpu.DF = 0;

	print_asm("CLD");
	return 1;
}