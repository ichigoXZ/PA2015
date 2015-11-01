#include "cpu/exec/template-start.h"

#define instr jbe

static void do_execute () {
	if(cpu.Eflags.CF==1 || cpu.Eflags.ZF==1){

	}

	print_asm_template1();
}

make_instr_helper(i) 

#include "cpu/exec/template-end.h"
