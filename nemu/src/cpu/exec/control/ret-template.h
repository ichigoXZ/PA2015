#include "cpu/exec/template-start.h"

#define instr ret

static void do_execute () {
	cpu.eip = MEM_R(cpu.esp);
	cpu.esp += DATA_BYTE;
#if DATA_BYTE == 2
	cpu.eip = cpu.eip & 0x0000ffff;
#endif
	printf("cpu.esp: 0x%x\n",cpu.esp );
	cpu.esp += op_src->val;
	printf("op_src:0x%x\n",op_src->val);
	printf("cpu.esp: 0x%x\n",cpu.esp );

	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"