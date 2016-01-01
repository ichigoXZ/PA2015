#include "cpu/exec/helper.h"

make_helper(cmps) {
	int data_byte = 0;

	if(ops_decoded.opcode == 0xa6) data_byte = 1;
	else if(ops_decoded.opcode == 0xa7 && ops_decoded.is_data_size_16 == 0){
		data_byte = 4;
	}else data_byte = 2;

	if(swaddr_read(cpu.esi,data_byte) - swaddr_read(cpu.edi,data_byte) == 0){
		cpu.ZF = 1;
		cpu.esi+=data_byte ;cpu.edi+=data_byte;
	}
	else {
		cpu.ZF = 0;
		cpu.esi-=data_byte;cpu.edi-=data_byte;
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