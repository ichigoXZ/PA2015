#include "cpu/exec/template-start.h"

#define instr movs

static void do_execute() {
	int IncDec = 0;

	IncDec = (0==cpu.DF) ? DATA_BYTE: -DATA_BYTE;

	swaddr_write(cpu.edi,DATA_BYTE,swaddr_read(cpu.eax,DATA_BYTE));

	cpu.edi += IncDec;
	cpu.esi += IncDec;

	print_asm_template2();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"