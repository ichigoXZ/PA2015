#include "cpu/exec/helper.h"

make_helper(movs) {
	int data_byte = 0, IncDec=0;
	printf("yes!\n");
	if(ops_decoded.opcode == 0xa4) data_byte = 1;
	else if(ops_decoded.opcode == 0xa5 && ops_decoded.is_data_size_16 == 0){
		data_byte = 4;
	}else data_byte = 2;

	IncDec = (cpu.DF == 0) ? data_byte : -data_byte;

	swaddr_write( cpu.edi, data_byte, swaddr_read(cpu.esi, data_byte));

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