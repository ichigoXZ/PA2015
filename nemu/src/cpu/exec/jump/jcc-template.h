#include "cpu/exec/template-start.h"

#define instr jcc

static void do_execute () {
	uint8_t flag = 0;
	uint8_t byte_t = 0;

	switch(ops_decoded.opcode){
		case 0x77: if(cpu.CF ==0 && cpu.ZF ==0)flag=1;break;				//JA, JNBE
		case 0x73: if(cpu.CF ==0)flag=1;break; 					//JAE, JNB, JNC
		case 0x72: if(cpu.CF == 1)flag=1;break; 					//JB, JC, JNAE
		case 0x76: if(cpu.CF == 1 || cpu.ZF == 1)flag=1;break;			//JBE, JNA
		case 0xE3: if(reg_w(R_CX) == 0 || reg_l(R_ECX) == 0)flag=1; break;	//JCXZ, JECXZ
		case 0x74: if(cpu.ZF == 1)flag=1; break; 					//JE, JZ, JZ
		case 0x7f: if(cpu.ZF == 0 && cpu.SF == cpu.OF)flag=1;break;			//JG, JNLE
		case 0x7d: if(cpu.SF == cpu.OF)flag=1; break;					//JGE, JNL
		case 0x7c: if(cpu.SF != cpu.OF)flag=1; break;					//JL , JNGE
		case 0x7e: if(cpu.ZF == 1 ||cpu.SF != cpu.OF)flag=1; break;			//JLE, JNG
		case 0x75: if(cpu.ZF == 0)flag=1;break;					//JNE,JNZ
		case 0x71: if(cpu.OF == 0)flag=1; break;					//JNO
		case 0x7b: if(cpu.PF == 0)flag=1;break;					//JNP, JPO
		case 0x79: if(cpu.SF == 0)flag=1;break;					//JNS
		case 0x70: if(cpu.OF == 1)flag=1; break;					//JO
		case 0x7a: if(cpu.PF == 1)flag=1; break;					//JP, JPE
		case 0x78: if(cpu.SF == 1)flag=1; break;					//JS
		default: byte_t = 1;
	}

	switch(ops_decoded.opcode & 0x0ff){
		case 0x87: if(cpu.CF == 0 && cpu.ZF == 0)flag=1; break;			//JA, JNBE
		case 0x83: if(cpu.CF == 0)flag=1; break;					//JAE, JNB, JNC
		case 0x82: if(cpu.CF == 1)flag=1; break;					//JB, JC, JNAE
		case 0x86: if(cpu.CF == 1 || cpu.ZF == 1)flag=1; break;			//JBE, JNA
		case 0x84: if(cpu.ZF == 1)flag=1; break;					//JE, JZ
		case 0x8f: if(cpu.ZF == 0 && cpu.SF == cpu.OF)flag=1; break;			//JG, JNLE
		case 0x8d: if(cpu.SF == cpu.OF)flag=1; break;					//JGE, JNL
		case 0x8c: if(cpu.SF !=cpu.OF)flag=1; break;					//JL, JNGE
		case 0x8e: if(cpu.ZF == 1 || cpu.SF != cpu.OF)flag=1; break;			//JLE, JNG
		case 0x85: if(cpu.ZF == 0)flag=1; break;					//JNE, JNZ
		case 0x81: if(cpu.OF == 0)flag=1; break;					//JNO
		case 0x8b: if(cpu.PF == 0)flag=1; break;					//JNP, JPO
		case 0x89: if(cpu.SF == 0)flag=1; break;					//JNS
		case 0x80: if(cpu.OF == 1)flag=1; break;					//JO
		case 0x8a: if(cpu.PF == 1)flag=1; break;					//JP, jpe
		case 0x88: if(cpu.SF == 1)flag=1; break;					//JS
	}

	if(flag){
		if(byte_t ==0) 
			cpu.eip+= (int32_t)((int8_t) (op_src->val));
		else if(DATA_BYTE == 2){
			cpu.eip += (int32_t)((int16_t)op_src -> val);
		}
		else cpu.eip +=(int32_t)op_src -> val;
		if(DATA_BYTE == 2){
			cpu.eip = cpu.eip & 0x0000ffff;
		}	
	}

	print_asm_template1();
}

make_instr_helper(i) 

#include "cpu/exec/template-end.h"
