#include "cpu/exec/helper.h"

make_helper(exec);

make_helper(rep) {
	int len;
	int count = 0;
	printf("rep\n");
	if(instr_fetch(eip + 1, 1) == 0xc3) {
		/* repz ret */
		printf("len==0\n");
		exec(eip + 1);
		len = 0;
	}
	else {
		printf("else \n");
		while(cpu.ecx) {
			exec(eip + 1);
			count ++;
			cpu.ecx --;
			/*assert(ops_decoded.opcode == 0xa4	// movsb
				|| ops_decoded.opcode == 0xa5	// movsw
				|| ops_decoded.opcode == 0xaa	// stosb
				|| ops_decoded.opcode == 0xab	// stosw
				|| ops_decoded.opcode == 0xa6	// cmpsb
				|| ops_decoded.opcode == 0xa7	// cmpsw
				|| ops_decoded.opcode == 0xae	// scasb
				|| ops_decoded.opcode == 0xaf	// scasw
		);*/
			switch(ops_decoded.opcode) {
				case 0xa4: swaddr_write(cpu.edi,1,swaddr_read(cpu.esi,1));
							cpu.esi--;cpu.edi--;
							break;
				case 0xa5: if(ops_decoded.is_data_size_16 == 1){
								swaddr_write(cpu.edi,2,swaddr_read(cpu.esi,2));
								cpu.esi -= 2;cpu.edi -= 2;
							}
							else {
								swaddr_write(cpu.edi,4,swaddr_read(cpu.esi,4));
								cpu.esi -= 4;cpu.edi -= 4;
							}
							break;
				case 0xaa: swaddr_write(cpu.edi,1,cpu.gpr[R_EAX]._8[0]);
							cpu.esi--;cpu.edi--;
							break;
				case 0xab: if(ops_decoded.is_data_size_16 == 1){
								swaddr_write(cpu.edi,2,cpu.gpr[R_AX]._16);
								cpu.esi -= 2;cpu.edi -= 2;
							}
							else {
								swaddr_write(cpu.edi,4,cpu.eax);
								cpu.esi -= 4;cpu.edi -= 4;
							}
							break;
				case 0xa6: if(swaddr_read(cpu.esi,1) - swaddr_read(cpu.edi,1) == 0){
						 		cpu.ZF = 1;
						 		cpu.esi++;cpu.edi++;
							}
							else {
								cpu.ZF = 0;
								cpu.esi--;cpu.edi--;
							}
							break;
				case 0xa7: if(ops_decoded.is_data_size_16 == 1){
								if(swaddr_read(cpu.esi,2) - swaddr_read(cpu.edi,2) == 0){
						 			cpu.ZF = 1;
						 			cpu.esi+=2;cpu.edi+=2;
								}
								else {
									cpu.ZF = 0;
									cpu.esi-=2;cpu.edi-=2;
								}
							}
							else {
								if(swaddr_read(cpu.esi,4) - swaddr_read(cpu.edi,4) == 0){
						 			cpu.ZF = 1;
						 			cpu.esi+=4;cpu.edi+=4;
								}
								else {
									cpu.ZF = 0;
									cpu.esi-=4;cpu.edi-=4;
								}
							}
							break;
				case 0xae: if(swaddr_read(cpu.esi,1) - cpu.gpr[R_EAX]._8[0] == 0){
						 		cpu.ZF = 1;
						 		cpu.esi++;cpu.edi++;
							}
							else {
								cpu.ZF = 0;
								cpu.esi--;cpu.edi--;
							}
							break;
				case 0xaf:  if(ops_decoded.is_data_size_16 == 1){
								if(swaddr_read(cpu.esi,2) - cpu.gpr[R_EAX]._16 == 0){
						 			cpu.ZF = 1;
						 			cpu.esi+=2;cpu.edi+=2;
								}
								else {
									cpu.ZF = 0;
									cpu.esi-=2;cpu.edi-=2;
								}
							}
							else {
								if(swaddr_read(cpu.esi,4) - cpu.eax == 0){
						 			cpu.ZF = 1;
						 			cpu.esi+=4;cpu.edi+=4;
								}
								else {
									cpu.ZF = 0;
									cpu.esi-=4;cpu.edi-=4;
								}
							}
							break;
				default:  printf("rep error!\n");
							assert(0);
			}

			/* TODO: Jump out of the while loop if necessary. */
			if(ops_decoded.opcode==0xa6 || ops_decoded.opcode==0xa7 || ops_decoded.opcode==0xae || ops_decoded.opcode==0xaf)
				if(cpu.ZF == 1)
					break;
		}
		len = 1;
	}
printf("%d\n",len );
#ifdef DEBUG
	char temp[80];
	sprintf(temp, "rep %s", assembly);
	sprintf(assembly, "%s[cnt = %d]", temp, count);
#endif
	return len + 1;
}
