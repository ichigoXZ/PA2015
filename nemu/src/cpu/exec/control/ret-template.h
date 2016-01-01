#include "cpu/exec/template-start.h"

#define instr ret

static void do_execute () {
#if DATA_BYTE == 2
	cpu.eip = cpu.eip & 0x0000ffff;
	cpu.eip = MEM_R(cpu.esp);
	cpu.esp += DATA_BYTE;
#elif DATA_BYTE == 4
	cpu.eip = MEM_R(cpu.esp);
	cpu.esp += DATA_BYTE;
#endif
	cpu.esp += op_src->val & 0x0000ffff;

	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"