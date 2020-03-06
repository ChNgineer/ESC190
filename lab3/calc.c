#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "calc.h"
#include "stack.h"

double compute_rpn(char* rpn){
	double a;
	double b;
	struct stack* s = create_stack();
	char *arr = strtok(rpn, " ");
	while(arr != NULL){
		printf("%s\n",arr);
		double temp;
		temp = atof(arr);
		printf("%f\n",temp);
		if(isdigit(arr[(int)strlen(arr)-1]) > 0){
			push(s, temp);
			printf("top:%f\n",s->top);
			printf("size: %d\n", s->size);
		}
		else if(unary_op(*arr)){
			printf("check unary op before\n");
			printf("top:%f\n",s->top->value);
			printf("size: %d\n", s->size);
			printf("%d\n",strcmp(arr,"e"));
			a = pop(s);
			if(strcmp(arr, "s") == 0)
				push(s, sin(a));
			else if(strcmp(arr, "c") == 0)
				push(s, cos(a));
			else if(strcmp(arr, "t") == 0)
				push(s, tan(a));
			else if(strcmp(arr, "e") == 0)
				push(s, exp(a));
			else if(strcmp(arr, "i") == 0)
				push(s, 1/a);
			else if(strcmp(arr, "m") == 0)
				push(s, -1*a);
			else if(strcmp(arr, "r") == 0)
				push(s, sqrt(a));
			printf("check unary op after\n");
			printf("top: %f\n",s->top->value);
		}
		else if(binary_op(*arr)){
			a = pop(s);
			b = pop(s);
			printf("check binary op before\n");
                        printf("a: %f\n",a);
			printf("b: %f\n",b);
			if(strcmp(arr, "+") == 0)
				push(s, b+a);
			else if(strcmp(arr, "-") == 0)
				push(s, b-a);
			else if(strcmp(arr, "*") == 0)
				push(s, b*a);
			else if(strcmp(arr, "/") == 0)
				push(s, b/a);
			else if(strcmp(arr, "^") == 0)
				push(s, pow(b,a));
			else if(strcmp(arr, "f") == 0){
				push(s, a);
				push(s, b);
			}
			printf("check binary op after\n");
			printf("top:%f\n",s->top->value);
			printf("size: %d\n", s->size);
		}
		arr = strtok(NULL, " ");
	}
	free(arr);
	double ans = pop(s);
	delete_stack(s);
	return ans;
}

char* get_expressions(char* filename){	
	FILE *f = fopen(filename,"r");
	if(f == NULL)
		exit(-1);
	char *list = malloc(256*sizeof(char));
	int count = 0;
	for (char c = getc(f); c != EOF; c = getc(f)){
		//printf("%c",c);
        	if (c == '\n')
        		++count;
		char *cp = &c;
		strncat(list,cp,sizeof(char));
	}	
	/*for (int i=0; i < count; i++){
		fgets(list,256,f);
		printf("%s\n", list);
	}*/
	fclose(f);
	return list;
}

void process_expressions(char* expressions, char* filename){
	FILE *f = fopen(filename,"w");
	double answer;
	char *arr;
	printf("%s\n",expressions);
	for (arr = strtok_r(expressions, "\n", &expressions);
         arr != NULL;
         arr = strtok_r(NULL, "\n", &expressions)){
		printf("%s\n",arr);
		answer = compute_rpn(arr);
		fprintf(f,"%f\n",answer);
	}
	fclose(f);
}





