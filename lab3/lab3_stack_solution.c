#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

struct stack* create_stack(void){
	struct stack* s = malloc(sizeof(struct stack));

	if(s){
		s->size= 0;
		s->top = NULL;
	}
	
	return s;
}

void delete_stack(struct stack* s){
	if(s){
		struct stack_entry* top;
		
		while(top=s->top){
			s->top=top->next;
			free(top);
		}
		
		free(s);
	}
}

double pop(struct stack* s){
	if(s && s->size > 0){
		struct stack_entry* top = s->top;
		double top_value = top->value;
		s->top=top->next;
		free(top);
		s->size--;
		
		return top_value;
	}
	
	return 0;
}

int push(struct stack* s, double e){
	if(!s) return -1;
	
	struct stack_entry* top = malloc(sizeof(struct stack_entry));
	
	top->value = e;
	top->next = s->top;
	
	s->top = top;
	s->size++;
	
	return 1;
}
