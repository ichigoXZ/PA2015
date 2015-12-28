#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_list[NR_WP];
WP *head, *free_;

void init_wp_list() {
	int i;
	for(i  = 0; i < NR_WP; i ++) {
		wp_list[i].NO = i;
		wp_list[i].next = &wp_list[i + 1];
	}
	wp_list[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_list;
}

/* TODO: Implement the functionality of watchpoint */
WP* new_wp(char *args){
   	if(head == NULL){
		head = free_;
		free_ = head->next;
		strcpy(head->info,args);
		head->value = expr(head->info,NULL);
		head->next = NULL;}
	else if(free_){ 
		WP *p = head;
		strcpy(free_->info,args);
		free_->value = expr(free_->info,NULL);
		head = free_;
		free_ = free_->next;
		head->next = p;}
	else 
		assert(0);
	return head;
}

void free_wp (int del){
	WP *p = head, *q = free_;
	if(!head)
		assert(0);
	else if(head->NO == del){
		head = head->next;
		p->next = free_;
		free_  = p;}
	else{
		while(p->next->NO != del){ p = p->next;}
		p->next = p->next->next;
		wp_list[del].next = q;
		free_  = &wp_list[del];}
}
