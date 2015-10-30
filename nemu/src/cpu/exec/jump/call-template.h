#include "cpu/exec/template-start.h"

#define instr call

static void do_execute () {
#if DATA_BYTE == 4
	cpu.eip = (cpu.eip + op_src->val) & 0x0000ffff;
#endif

	print_asm_template1();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"
