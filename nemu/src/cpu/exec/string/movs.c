#include "cpu/exec/helper.h"

make_helper(movs) {
	int data_byte = 0, IncDec=0;

	if(ops_decoded.opcode == 0xa4) data_byte = 1;
	else if(ops_decoded.opcode == 0xa5 && ops_decoded.is_data_size_16 == 0){
		data_byte = 4;
	}else data_byte = 2;

	IncDec = (cpu.DF == 0) ? data_byte : -data_byte;

	swaddr_write( reg_l(R_EDI), data_byte, swaddr_read(reg_l(R_ESI), data_byte));

	reg_l(R_EDI) += IncDec;
	reg_l(R_ESI) += IncDec;

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