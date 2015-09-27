#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;

	/* TODO: Add more members if necessary */
	char info[32];
	uint32_t value;

} WP;

WP* new_wp(char *);
void free_wp(int);

#endif
