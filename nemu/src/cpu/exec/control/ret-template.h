#include "cpu/exec/template-start.h"

#define instr ret

static void do_execute () {
	printf("yes! DATA_BYTE:%d\n",DATA_BYTE);
	cpu.eip = MEM_R(cpu.esp);
	cpu.esp += DATA_BYTE;
#if DATA_BYTE == 2
	cpu.eip = cpu.eip & 0x0000ffff;
#endif
	cpu.esp += op_src->val;

	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"