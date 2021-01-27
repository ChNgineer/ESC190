#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "calc.h"
#include "stack.h"

double compute_rpn(char* rpn){
	struct stack *s= create_stack();
	
	char *tok = strtok(rpn," ");
	
	while(tok){
		double op = atof(tok);
		
		if(op != 0.0 || *tok =='0'){
			push(s,op);
		}else if(binary_op(*tok)){
			double op2=pop(s);
			double op1=pop(s);
			
			switch(*tok){
				case '+': push(s,op1+op2);break;
				case '-': push(s,op1-op2);break;
				case '*': push(s,op1*op2);break;
				case '/': push(s,op1/op2);break;
				case '^': push(s,pow(op1,op2));break;
				case 'f': push(s,op2);push(s,op1);break;
			}
			
		}else if(unary_op(*tok)){
			double op1=pop(s);
			
			switch(*tok){
				case 's': push(s,sin(op1));break;
				case 'c': push(s,cos(op1));break;
				case 't': push(s,tan(op1));break;
				case 'e': push(s,exp(op1));break;
				case 'i': push(s,(1/op1));break;
				case 'm': push(s,(-op1));break;
			}
		}
		
		tok=strtok(NULL, " ");
	}
	
	double result = pop(s);
	
	delete_stack(s);
	
	return result;
}

char* get_expressions(char* filename){
	FILE* f = fopen(filename, "r");
	
	char *expr=malloc(256*sizeof(char));
	char *expressions = malloc(256*256*sizeof(char));
	
	*expressions='\0';
	
	while(fgets(expr, 256, f)){
		strcat(expressions, expr);
	}
	
	fclose(f);
	free(expr);	
	return expressions;
}

void process_expressions(char* expressions, char* filename){
	FILE* f = fopen(filename, "w");
	
	char **saveptr = malloc(sizeof(char*));
	char *tok = strtok_r(expressions,"\n", saveptr);
	
	while(tok){
		double res = compute_rpn(tok);
		fprintf(f, "%.5g\n",res);
		tok=strtok_r(NULL, "\n",saveptr);
	}

	fclose(f);

	free(saveptr);
}
