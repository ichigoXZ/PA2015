#include "cpu/exec/template-start.h"

#define instr ret

static void do_execute () {
	cpu.eip = MEM_R(cpu.esp);
	cpu.esp += DATA_BYTE;
	cpu.eip = cpu.eip & 0x0000ffff;
	cpu.esp += op_src->val;

	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"