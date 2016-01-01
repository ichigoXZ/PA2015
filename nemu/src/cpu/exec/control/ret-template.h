#include "cpu/exec/template-start.h"

#define instr ret

static void do_execute () {
	if(DATA_BYTE == 2){
		cpu.eip = MEM_R(cpu.esp);
		cpu.esp += 2;
		cpu.eip &= 0x0000ffff;
	}else{
		cpu.eip = MEM_R(cpu.esp);
		cpu.esp += 4;
	}
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"