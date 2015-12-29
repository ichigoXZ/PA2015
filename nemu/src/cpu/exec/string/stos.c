#include "cpu/exec/helper.h"

make_helper(stos) {
	int DATA_BYTE = 0;

	if(ops_decoded.opcode == 0xaa) DATA_BYTE = 1;
	else if(ops_decoded.opcode == 0xab && ops_decoded.is_data_size_16 == 0){
		DATA_BYTE = 4;
	}else DATA_BYTE = 2;
	if(DATA_BYTE == 1)
		swaddr_write(cpu.edi,1,cpu.gpr[R_EAX]._8[0]);
	else if (DATA_BYTE == 2)
		swaddr_write(cpu.edi,2,cpu.gpr[R_AX]._16);
	else
		swaddr_write(cpu.edi,4,cpu.eax);

	cpu.esi -= DATA_BYTE;
	cpu.edi -= DATA_BYTE;

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