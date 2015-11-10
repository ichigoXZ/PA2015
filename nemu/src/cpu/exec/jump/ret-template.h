#include "cpu/exec/template-start.h"

#define instr ret

static void do_execute () {
	printf("ennnn\n");

	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"