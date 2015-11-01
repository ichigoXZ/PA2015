#include "cpu/exec/template-start.h"

#define instr jbe

static void do_execute () {
	if(cpu.Eflags.CF==1 || cpu.Eflags.ZF==1){
#if DATA_TYPE == 1
		uint8_t val = (uint8_t)(cpu.eip&0xff);
		val += (uint8_t)op_src->val;
		cpu.eip=(cpu.eip&0xffffff00)+val;
#elif DATA_TYPE == 2
		uint16_t val = (uint16_t)(cpu.eip&0xffff);
		val += (uint16_t)op_src->val;
		cpu.eip=(cpu.eip&0xffff0000)+val;
#else
		cpu.eip=cpu.eip+(int32_t)op_src->val;
#endif
	}

	print_asm_template1();
}

make_instr_helper(i) 

#include "cpu/exec/template-end.h"
