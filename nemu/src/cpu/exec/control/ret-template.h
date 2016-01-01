#include "cpu/exec/template-start.h"

#define instr ret

make_helper(concat(ret_i_, SUFFIX)){
	concat(decode_i_, SUFFIX)(cpu.eip + 1);
	if(ops_decoded.opcode == 0xc2){
		if(DATA_BYTE == 2){
			cpu.eip = MEM_R(cpu.esp);
			cpu.esp += 2;
			cpu.eip &= 0x0000ffff;
		}else{
			cpu.eip = MEM_R(cpu.esp);
			cpu.esp += 4;
		}
	}
	return 0;
}

#include "cpu/exec/template-end.h"