
#include "cpu/exec/helper.h"

make_helper(movs) {
	int DATA_BYTE = 0, IncDec=0;

	if(ops_decoded.opcode == 0xa4) DATA_BYTE = 1;
	else if(ops_decoded.opcode == 0xa5 && ops_decoded.is_data_size_16 == 0){
		DATA_BYTE = 4;
	}else DATA_BYTE = 2;

	IncDec = (cpu.DF == 0) ? DATA_BYTE : -DATA_BYTE;

	swaddr_write( cpu.edi, DATA_BYTE, swaddr_read(cpu.esi, DATA_BYTE));

	cpu.edi += IncDec;
	cpu.esi += IncDec;

#ifdef DEBUG
	print_asm_template1();
#endif
	
	return 1;
}

make_helper(movs_b){
	return movs(eip);
}

make_helper(movs_v){
	return movs(eip);
}