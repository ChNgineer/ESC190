#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

struct stack *create_stack(){
	struct stack *s = (struct stack*)malloc(256*sizeof(struct stack*));
	if(!s)
		return NULL;
	s->top = NULL;
	s->size = 0;
    return s;
}

void delete_stack(struct stack *s){
	for(int i=0; i < s->size; i++){
		struct stack_entry *temp;
		temp = s->top;
		s->top = s->top->next;
		temp->next = NULL;
		free(temp);
	}
	free(s);
}

double pop(struct stack *s){
	if(s->top == NULL)
		return 0;
	struct stack_entry *temp;
	temp = s->top;
	s->top = s->top->next;
	temp->next = NULL;
	--s->size;
	double ans = temp->value;
	free(temp);
	return ans;
}

int push(struct stack *s, double e){
	struct stack_entry *temp = (struct stack_entry*)malloc(sizeof(struct stack_entry));
	temp->value = e;
	if(!temp)
		return -1;
	temp->next = s->top;
	s->top = temp;
	++s->size;
	return 0;
}
