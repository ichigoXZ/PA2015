#include "cpu/exec/helper.h"

make_helper(cmps) {
	int DATA_BYTE = 0;

	if(ops_decoded.opcode == 0xa4) DATA_BYTE = 1;
	else if(ops_decoded.opcode == 0xa5 && ops_decoded.is_data_size_16 == 0){
		DATA_BYTE = 4;
	}else DATA_BYTE = 2;

	if(swaddr_read(cpu.esi,DATA_BYTE) - swaddr_read(cpu.edi,DATA_BYTE) == 0){
		cpu.ZF = 1;
		cpu.esi+=DATA_BYTE ;cpu.edi+=DATA_BYTE;
	}
	else {
		cpu.ZF = 0;
		cpu.esi-=DATA_BYTE;cpu.edi-=DATA_BYTE;
	}


#ifdef DEBUG
	print_asm_template1();
#endif
	
	return 1;
}

make_helper(cmps_b){
	return cmps(eip);
}

make_helper(cmps_v){
	return cmps(eip);
}