#include "cpu/exec/template-start.h"

#define instr cmov

static void do_execute(){
	int flag=0;

	switch(ops_decoded.opcode & 0x0ff){
		case 0x40: if (cpu.OF == 1); flag = 1;break;					//CMOVO
		case 0x41: if (cpu.OF == 0); flag = 1;break;					//CMOVNO
		case 0x42: if (cpu.CF == 1); flag = 1; break;					//CMOVB, CMOVC, CMOVNAE
		case 0x43: if (cpu.CF == 0); flag = 1;break;					//CMOVAE, CMOVNB, CMOVNC
		case 0x44: if (cpu.ZF == 1);  flag = 1;break;					//CMOVE, CMOVZ
		case 0x45: if (cpu.ZF == 0); flag = 1;break;					//CMOVNE, CMOVNZ
		case 0x46: if (cpu.CF == 1 || cpu.ZF == 1); flag = 1;break;			//CMOVBE, CMOVNA
		case 0x47: if (cpu.CF == 0 && cpu.ZF == 0); flag = 1;break;			//CMOVA, CMOVNBE
		case 0x48: if (cpu.SF == 1); flag = 1;break;					//CMOVS
		case 0x49: if (cpu.SF == 0); flag = 1;break;					//CMOVNS
		case 0x4a: if (cpu.PF == 1); flag = 1;break;					//CMOVP, CMOVpe
		case 0x4b: if (cpu.PF == 0); flag = 1;break;					//CMOVNP, CMOVPO
		case 0x4c: if (cpu.SF !=cpu.OF);  flag = 1;break;					//CMOVL, CMOVNGE
		case 0x4d: if (cpu.SF == cpu.OF); flag = 1; break;					//CMOVGE, CMOVNL
		case 0x4e: if (cpu.ZF == 1 || cpu.SF != cpu.OF); flag = 1;break;			//CMOVLE, CMOVNG
		case 0x4f: if (cpu.ZF == 0 && cpu.SF == cpu.OF); flag = 1;break;			//CMOVG, CMOVNLE

	}

	if(flag){
		OPERAND_W(op_dest, op_src->val);
	}

	print_asm_template2();
}

make_instr_helper(rm2r);

#include "cpu/exec/template-end.h"