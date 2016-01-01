#include "cpu/exec/helper.h"

make_helper(scas) {
	int data_byte = 0,flag = 0;

	if(ops_decoded.opcode == 0xae) data_byte = 1;
	else if(ops_decoded.opcode == 0xaf && ops_decoded.is_data_size_16 == 0){
		data_byte = 4;
	}else data_byte = 2;

	if(data_byte == 1)
		flag = swaddr_read(cpu.esi,1) - cpu.gpr[R_EAX]._8[0] ;
	else if (data_byte == 2)
		flag = swaddr_read(cpu.esi,1) - cpu.gpr[R_AX]._16;
	else
		flag = swaddr_read(cpu.esi,1) - cpu.eax;

	if(flag == 0){
		cpu.ZF = 1;
		cpu.esi += data_byte;
		cpu.edi += data_byte;
	}
	else{
		cpu.ZF = 0;
		cpu.esi -= data_byte;
		cpu.edi -= data_byte;
	}



#ifdef DEBUG
	print_asm_template1();
#endif
	
	return 1;
}

make_helper(scas_b){
	return scas(eip);
}

make_helper(scas_v){
	return scas(eip);
}