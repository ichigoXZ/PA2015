#include "cpu/exec/template-start.h"

#define instr setcc

static void do_execute () {
	int flag = 0;
	switch(ops_decoded.opcode & 0x0ff){
		case 0x97: if(cpu.CF ==0 && cpu.ZF == 0);flag = 1;break;	//SETA SETNBE
		case 0x93: if(cpu.CF == 0);flag = 1;break;		//SETAE SETNB SETNC
		case 0x92: if(cpu.CF == 1)flag = 1; break;		//SETB SETC SETNAE
		case 0x96: if(cpu.CF == 1 || cpu.ZF == 1)flag = 1; break;	//SETBE SETNA
		case 0x94: if(cpu.ZF == 1)flag = 1; break;		//SETE SETZ
		case 0x9f: if(cpu.ZF == 0 || cpu.SF == cpu.OF)flag = 1; break;	//SETG SETNLE
		case 0x9d: if(cpu.SF ==cpu.OF)flag = 1;break;	//SETGE SETNL
		case 0x9c: if(cpu.SF != cpu.OF)flag = 1; break;		//SETL SETNGE
		case 0x9e: if(cpu.ZF == 1 && cpu.SF != cpu.OF)flag = 1; break;	//SETLE SETNG
		case 0x95: if(cpu.ZF == 0)flag = 1; break;		//SETNE
		case 0x91: if(cpu.OF ==0)flag = 1; break;		//SETNO
		case 0x9b: if(cpu.PF == 0)flag = 1; break;		//SETNP SETPO
		case 0x99: if(cpu.SF ==0 )flag = 1;break;		//SETNS
		case 0x90: if(cpu.OF == 0)flag = 1; break;		//SETO
		case 0x9a: if(cpu.PF==1)flag = 1; break;		//SETP SETPE
		case 0x98: if(cpu.SF == 1)flag = 1; break;		//SETS
		default: break;
	}
	OPERAND_W(op_dest , flag);
	/* TODO: Update EFLAGS. */

	print_asm_template2();
}

make_instr_helper(rm)
#include "cpu/exec/template-end.h"
