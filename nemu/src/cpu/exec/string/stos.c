#include "cpu/exec/helper.h"

make_helper(stos) {
	int data_byte = 0, IncDec = 0;

	if(ops_decoded.opcode == 0xaa) data_byte = 1;
	else if(ops_decoded.opcode == 0xab && ops_decoded.is_data_size_16 == 0){
		data_byte = 4;
	}else data_byte = 2;
	
	IncDec = (cpu.DF == 0)?data_byte:-data_byte;
	if(data_byte == 1)
		swaddr_write(cpu.edi,1,cpu.gpr[R_EAX]._8[0]);
	else if (data_byte == 2)
		swaddr_write(cpu.edi,2,cpu.gpr[R_AX]._16);
	else
		swaddr_write(cpu.edi,4,cpu.eax);
	
	cpu.esi += IncDec;
	cpu.edi += IncDec;

#ifdef DEBUG
	print_asm_template1();
#endif
	
	return 1;
}

make_helper(stos_b){
	return stos(eip);
}

make_helper(stos_v){
	return stos(eip);
}