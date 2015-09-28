#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);

/* We use the ``readline'' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_si(char *args){
    if(!args)
        cpu_exec(1);
    else
        cpu_exec(atoi(args));
	return 0;
}

static int cmd_info(char *args) {
		printf("%s\n.",args);
	if('r'==*args){    
        printf("eax: 0x%.8x\n",cpu.eax);
        printf("ecx: 0x%.8x\n",cpu.ecx);
        printf("edx: 0x%.8x\n",cpu.edx);
        printf("ebx: 0x%.8x\n",cpu.ebx);
        printf("esp: 0x%.8x\n",cpu.esp);
        printf("ebp: 0x%.8x\n",cpu.ebp);
        printf("esi: 0x%.8x\n",cpu.esi);
        printf("edi: 0x%.8x\n",cpu.edi);
    }
    else
        printf("unknown command.\n");
    return 0;
}

static int cmd_x(char *args){
    char *len = strtok(args," ");
	char *expr = len + strlen(len) + 1;
	int i = 0;
	int addr = (int)strtol(expr,NULL,16);
	printf("0x%x:\t",addr);
	for( ; i < atoi(len); i++){
	printf("0x%.8x\t",swaddr_read(addr+i,4));
	if(i%4==3) printf("\n\t\t");
	}
	printf("\n");
	return 0;
}

static int cmd_p(char *args) {
	bool success;
	uint32_t result = expr(args,&success);	
    //if(success)
		printf("result: %d\n",result);
        printf("HEX: 0x%.8x\n",result);
		//else
	//	printf("Bad expression.");
	return 0;
 }

static int cmd_w(char *args){
	new_wp(args);
	return 0;
}

static int cmd_d(char *args){
	return 0;
}

static int cmd_help(char *args);

static struct {  
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program",cmd_c },
	{ "q", "Exit NEMU",cmd_q },

	/* TODO: Add more commands */
	{"si","do a single step",cmd_si },
	{"info"," r: print registers;/n/t w:print watchpoints",cmd_info },
	{"x","N EXPR: 求出表达式EXPR的值，将结果作为起始内存地址，以十六进制形式输出连续的N个字节",cmd_x },
	{"p"," EXPR: 求出表达式EXPR的值，将结果作为起始内存地址，以十六进制形式输出连续的N个4字节",cmd_p },
	{"w"," EXPR: 当表达式EXPR的值以生变化时，暂停程序执行",cmd_w },
	{"d"," N: 删除序号为N的监视点",cmd_d }

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char  *args)  {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop()  { 
	while(1) {  
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) {return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
