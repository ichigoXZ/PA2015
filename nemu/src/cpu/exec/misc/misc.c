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

	print_asm("cld");
	return 1;
}

make_helper(movsb){
	int IncDec = (0==cpu.DF) ? 1: -1;
	
	swaddr_write(cpu.edi,1,swaddr_read(cpu.esi,1));
	cpu.edi += IncDec;
	cpu.esi += IncDec;

	return 1;
}

make_helper(movsw){
	int data_byte = 0, IncDec = 0;
	if(ops_decoded.is_data_size_16)
		data_byte = 2;
	else 
		data_byte = 4;

	IncDec = (0==cpu.DF) ? data_byte: -data_byte;

	swaddr_write(cpu.edi,data_byte,swaddr_read(cpu.eax,data_byte));

	cpu.edi += IncDec;
	cpu.esi += IncDec;

	return 1;

}

make_helper(stosb){
	swaddr_write(cpu.edi,1,cpu.gpr[R_EAX]._8[0]);
	cpu.esi--;cpu.edi--;

	return 1;
}

make_helper(stosw){
	if(ops_decoded.is_data_size_16 == 1){
		swaddr_write(cpu.edi,2,cpu.gpr[R_AX]._16);
		cpu.esi -= 2;cpu.edi -= 2;
	}
	else {
		swaddr_write(cpu.edi,4,cpu.eax);
		cpu.esi -= 4;cpu.edi -= 4;
	}

	return 1;
}

make_helper(cmpsb){
	if(swaddr_read(cpu.esi,1) - swaddr_read(cpu.edi,1) == 0){
		cpu.ZF = 1;
		cpu.esi++;cpu.edi++;
	}
	else {
		cpu.ZF = 0;
		cpu.esi--;cpu.edi--;
	}

	return 1;
}

make_helper(cmpsw){
	if(ops_decoded.is_data_size_16 == 1){
		if(swaddr_read(cpu.esi,2) - swaddr_read(cpu.edi,2) == 0){
			cpu.ZF = 1;
			cpu.esi+=2;cpu.edi+=2;
		}
		else {
			cpu.ZF = 0;
			cpu.esi-=2;cpu.edi-=2;
		}
	}
	else {
		if(swaddr_read(cpu.esi,4) - swaddr_read(cpu.edi,4) == 0){
			cpu.ZF = 1;
			cpu.esi+=4;cpu.edi+=4;
		}
		else {
			cpu.ZF = 0;
			cpu.esi-=4;cpu.edi-=4;
		}
	}

	return 1;
}

make_helper(scasb){
	if(swaddr_read(cpu.esi,1) - cpu.gpr[R_EAX]._8[0] == 0){
		cpu.ZF = 1;
		cpu.esi++;cpu.edi++;
	}
	else {
		cpu.ZF = 0;
		cpu.esi--;cpu.edi--;
	}

	return 1;
}

make_helper(scasw){
	if(ops_decoded.is_data_size_16 == 1){
		if(swaddr_read(cpu.esi,2) - cpu.gpr[R_EAX]._16 == 0){
			cpu.ZF = 1;
			cpu.esi+=2;cpu.edi+=2;
		}
		else {
			cpu.ZF = 0;
			cpu.esi-=2;cpu.edi-=2;
		}
	}
	else {
		if(swaddr_read(cpu.esi,4) - cpu.eax == 0){
			cpu.ZF = 1;
			cpu.esi+=4;cpu.edi+=4;
		}
		else {
			cpu.ZF = 0;
			cpu.esi-=4;cpu.edi-=4;
		}
	}

	return 1;
}