#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <stdlib.h>
#include <sys/types.h>
#include <regex.h>
#include <elf.h>

 extern char *strtab;
 extern Elf32_Sym *symtab;
 extern int nr_symtab_entry;

enum {
	LEFT_R,RIGHT_R,NUM,HEX_NUM,PLUS,SUB,MUL,ADDR,DIVIDE,REG,VAR,AND,OR,NOT,EQ,NOT_EQ,NOTYPE = 256

	/* TODO: Add more token types */
};
 
static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{"\\(", LEFT_R},					// left parentheses
	{"\\)", RIGHT_R},					// right parentheses
	{"0x[a-fA-F0-9]+",HEX_NUM},		//hexadecimal numbers
	{"[1-9][0-9]*|0", NUM},					// numbers
	{" +",  NOTYPE},				// spaces
	{"\\+", PLUS},					// plus
	{"\\-", SUB},						// substract
	{"\\*", MUL},					// multiply
	{"\\/", DIVIDE},					// divide
	{"\\$[a-zA-Z]+",REG},				//reg_name
	{"[_|a-zA-Z]+[_|a-zA-Z0-9]*",VAR},	//var_name
	{"&&",AND},						//and
	{"\\|\\|",OR},
	{"!",NOT},
	{"!=",NOT_EQ},					//not equal
	{"==", EQ}						// equal
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules  are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret != 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				//Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */

				switch(rules[i].token_type) {
					case NOTYPE:  break;
					case LEFT_R:  tokens[nr_token++].type = LEFT_R;
							  	  break;
				    case RIGHT_R: tokens[nr_token++].type = RIGHT_R;
  								  break;
					case PLUS:    tokens[nr_token++].type = PLUS;
								  break;
					case SUB: 	  tokens[nr_token++].type = SUB;
								  break;  
				    case MUL:	  if(tokens[nr_token-1].type == PLUS || tokens[nr_token-1].type == SUB || tokens[nr_token-1].type == MUL || tokens[nr_token-1].type == DIVIDE || nr_token == 0)
									  tokens[nr_token++].type = ADDR;
								  else
		 							  tokens[nr_token++].type = MUL;
								  break;
					case DIVIDE:  tokens[nr_token++].type = DIVIDE;
								  break;
					case EQ:  tokens[nr_token++].type = EQ;
							  break;
					case NOT_EQ:  tokens[nr_token++].type = NOT_EQ;
								  break;
					case AND: tokens[nr_token++].type = AND;
							  break;
					case OR:  tokens[nr_token++].type = OR;
							  break;
					case NOT: tokens[nr_token++].type = NOT;
							  break;
					case NUM: tokens[nr_token].type = NUM;
							  strncpy(tokens[nr_token].str,substr_start,substr_len);		
							  nr_token++;
							  break;
					case HEX_NUM: tokens[nr_token].type = HEX_NUM;
								  strncpy(tokens[nr_token].str,substr_start,substr_len);
								  nr_token++;
								  break;
					case REG: tokens[nr_token].type = REG;
    						  strncpy(tokens[nr_token].str,substr_start,substr_len);
							  nr_token++;
							  break;
					case VAR: tokens[nr_token].type = VAR;
							  strncpy(tokens[nr_token].str,substr_start,substr_len);
							  nr_token++;
							  break;
					default: panic("please implement me");
							 assert(0);
				}

				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}

uint32_t eval(uint32_t p,uint32_t q);
uint32_t dominant_operator(uint32_t p,uint32_t q);
bool check_parentheses(uint32_t p,uint32_t q);

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	/* TODO: Insert codes to evaluate the expression. */
   	//panic("please implement me");
	return eval(0,nr_token-1);
}

uint32_t eval(uint32_t p,uint32_t q){
	if(p > q){
		printf("Bad expression./n");
		assert(0);}
	else if(p == q-1){
		if(tokens[p].type == ADDR)
			return swaddr_read(eval(p+1,q),4);
		else if(tokens[p].type == NOT)
			return !eval(p+1,q);}
	else if(p == q){ 		//single token
		if(tokens[p].type == NUM)
			return (uint32_t)atoi(tokens[p].str);
		else if(tokens[p].type == HEX_NUM)
			return (uint32_t)strtol(tokens[p].str,NULL,16);
		else if(tokens[p].type == REG){
			if(strcmp(tokens[p].str+1,"eax")==0||strcmp(tokens[p].str+1,"EAX")==0)
				return cpu.eax;
			else if(strcmp(tokens[p].str+1,"ecx")==0||strcmp(tokens[p].str+1,"ECX")==0)
				return cpu.ecx;
			else if(strcmp(tokens[p].str+1,"edx")==0||strcmp(tokens[p].str+1,"EDX")==0)
				return cpu.edx;
			else if(strcmp(tokens[p].str+1,"ebx")==0||strcmp(tokens[p].str+1,"EBX")==0)
				return cpu.ebx;
			else if(strcmp(tokens[p].str+1,"esp")==0||strcmp(tokens[p].str+1,"ESP")==0)
				return cpu.esp;
			else if(strcmp(tokens[p].str+1,"ebp")==0||strcmp(tokens[p].str+1,"EBP")==0)
				return cpu.ebp;
			else if(strcmp(tokens[p].str+1,"esi")==0||strcmp(tokens[p].str+1,"ESI")==0)
				return cpu.esi;
			else if(strcmp(tokens[p].str+1,"edi")==0||strcmp(tokens[p].str+1,"EDI")==0)
				return cpu.edi;
			else if(strcmp(tokens[p].str+1,"eip")==0||strcmp(tokens[p].str+1,"EIP")==0)
				return cpu.eip;
			else if(strcmp(tokens[p].str+1,"ax")==0||strcmp(tokens[p].str+1,"AX")==0)
				return cpu.gpr[R_AX]._16;
			else if(strcmp(tokens[p].str+1,"cx")==0||strcmp(tokens[p].str+1,"CX")==0)
				return cpu.gpr[R_CX]._16;
			else if(strcmp(tokens[p].str+1,"dx")==0||strcmp(tokens[p].str+1,"DX")==0)
				return cpu.gpr[R_DX]._16;
			else if(strcmp(tokens[p].str+1,"bx")==0||strcmp(tokens[p].str+1,"BX")==0)
				return cpu.gpr[R_BX]._16;
			else if(strcmp(tokens[p].str+1,"sp")==0||strcmp(tokens[p].str+1,"SP")==0)
				return cpu.gpr[R_SP]._16;
			else if(strcmp(tokens[p].str+1,"bp")==0||strcmp(tokens[p].str+1,"BP")==0)
				return cpu.gpr[R_BP]._16;
			else if(strcmp(tokens[p].str+1,"si")==0||strcmp(tokens[p].str+1,"SI")==0)
				return cpu.gpr[R_SI]._16;
			else if(strcmp(tokens[p].str+1,"al")==0||strcmp(tokens[p].str+1,"AL")==0)
				return cpu.gpr[R_EAX]._8[0];
			else if(strcmp(tokens[p].str+1,"ah")==0||strcmp(tokens[p].str+1,"AH")==0)
				return cpu.gpr[R_EAX]._8[1];
			else if(strcmp(tokens[p].str+1,"cl")==0||strcmp(tokens[p].str+1,"CL")==0)
				return cpu.gpr[R_ECX]._8[0];
			else if(strcmp(tokens[p].str+1,"ch")==0||strcmp(tokens[p].str+1,"CH")==0)
				return cpu.gpr[R_ECX]._8[1];
			else if(strcmp(tokens[p].str+1,"dl")==0||strcmp(tokens[p].str+1,"DL")==0)
				return cpu.gpr[R_EDX]._8[0];
			else if(strcmp(tokens[p].str+1,"dh")==0||strcmp(tokens[p].str+1,"DH")==0)
				return cpu.gpr[R_EDX]._8[1];
			else if(strcmp(tokens[p].str+1,"bl")==0||strcmp(tokens[p].str+1,"BL")==0)
				return cpu.gpr[R_EBX]._8[0];
			else if(strcmp(tokens[p].str+1,"bh")==0||strcmp(tokens[p].str+1,"BH")==0)
				return cpu.gpr[R_EBX]._8[1];
			else 
				assert(0);}
		else if(tokens[p].type == VAR){
			int i=0;
			for(i=0;i<nr_symtab_entry;i++)
				printf("%d\n",nr_symtab_entry );
				if(strcmp(tokens[p].str,(strtab+i))==0){
					return (symtab+i)-> st_size;
				}
				else
					assert("no such val");
		}
		else
			assert(0);
				}
	else if(check_parentheses(p,q) == true)		
		return eval(p+1,q-1);
	else{
		uint32_t op = dominant_operator(p,q);
		uint32_t val1 = eval(p,op-1);
  	    uint32_t val2 = eval(op+1,q);

		switch(tokens[op].type){
			case PLUS: return val1 + val2;
			case SUB:  return val1 - val2;
			case MUL:  return val1 * val2;
			case DIVIDE: if(!val2){
						    assert(0);}	
    					 else              
    					    return val1 / val2;
			case AND:  return val1 && val2;
			case OR:   return val1 || val2;
			case EQ:   return val1 == val2;
			case NOT_EQ: return val1 != val2;
			//case NOT:  return !val2;
			default: printf("not surported");
					 assert(0);}
	}
	return 0;
}

uint32_t dominant_operator(uint32_t p,uint32_t q){
	int  pos =(int) p;
	int  i = 0,j = 0;
	typedef struct mark{
		int position;
		int op;
	}Mark;
	Mark stack[32];
	for( ;pos <= q;pos++)
		switch(tokens[pos] .type){
			case PLUS : stack[i].position = pos;
						stack[i++].op = 1;
						break;
		    case SUB  : stack[i].position = pos;
						stack[i++].op = 1;
						break;
			case MUL  : stack[i].position = pos;
						stack[i++].op = 0;
						break;
			case DIVIDE:stack[i].position = pos;
						stack[i++].op = 0; 
						break;
			case AND  : stack[i].position = pos;
						stack[i++].op = 2;
						break;
			case OR   : stack[i].position = pos;
						stack[i++].op = 2;
			case EQ   : stack[i].position = pos;
						stack[i++].op = 2;
			case NOT_EQ : stack[i].position = pos;
						stack[i++].op = 2;
			case LEFT_R:
						 while(tokens[pos].type != RIGHT_R && pos<q)
					   	{
							pos++;
						}
						 break;
			default: break;
			}
	for(j = i-1 ;j >= 0;j--)
		if(stack[j].op==2){
			return stack[j].position;}
	for(j = i-1 ;j >= 0;j--)
		if(stack[j].op)
			return stack[j].position;
	return stack[i-1].position;
}

bool check_parentheses(uint32_t p, uint32_t q){
	int flag = 1;
	uint32_t pos = p+1;
	if(tokens[p].type != LEFT_R)
		return false;
	else{
		for( ; pos<q; pos++){
			if(tokens[pos].type == LEFT_R)
				flag++;
			if(tokens[pos].type == RIGHT_R){
				if(flag) flag--;
				else 
					return false;}
			if(!flag)
				return false;
		}
		if(tokens[q].type == RIGHT_R)
			return true;
		else
			return false;
	}
}

