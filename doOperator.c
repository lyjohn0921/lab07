#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "doOperator.h"
#include "tokenStack.h"
#include "lexical.h"

static int op_quit(struct tokenStack *stack);
static int op_print(struct tokenStack *stack);
static int op_dump(struct tokenStack *stack);
static int op_add(struct tokenStack *stack);
static int op_showall(struct tokenStack *stack);
static int op_minus(struct tokenStack *stack);
static int op_product(struct tokenStack *stack);
static int op_divide(struct tokenStack *stack);
static int op_drop(struct tokenStack *stack);
static int op_help(struct tokenStack *stack);
static int op_swap(struct tokenStack *stack);
static int op_rot(struct tokenStack *stack);
static int op_rotm(struct tokenStack *stack);
static int op_rem(struct tokenStack *stack);


static struct operator_struct {
  char *name;
  char *doc;
  int (*fn_ptr)(struct tokenStack *);
} ops[] = {
  {"quit", "just exit", op_quit},
  {"print", "pop out last token on stack", op_print},
  {"dump", "show all token with type", op_dump},
  {"+", "push n1 + n2",op_add},
  {"-", "push n1 - n2",op_minus},
  {"*", " (n1 n2 — product) - push n1*n2",op_product},
  {"/", "(n1 n2 — quotient) - push n1/n2",op_divide},
  {"DROP", "(n1 — ) - drop the top element off of the stack",op_drop},
  {"HELP", "(—) - print out all commands plus a line of documentation. Note: This is made easier due to the help string in the table",op_help},
  {"S", " print all elements on the stack non destructively",op_showall},
  {"SWAP", "(n1 n2 — n2 n1) - swap the order of the top two elements on the stack ", op_swap},
  {"ROT", "(n1 n2 n3 — n2 n3 n1) - rotate top 3 elements on the stack ",op_rot},
  {"ROTMINUS", "n1 n2 n3 — n3 n1 n2) - rotate top 3 elements on the stack", op_rotm},
  {"MODQUOT", "n1 n2 — rem quotient) - push remainder then quotient ", op_rem},
  {(char *)NULL, (char *)NULL, (int(*)(struct tokenStack *)) NULL}
};


/* YOU WRITE THIS */
static int popInt(struct tokenStack *s)
{
  int r ;
  int sign;
  int l;
  struct lexToken *token;
  
  l = 0;
  r = 0;
  token = popTokenStack(s);
  if (token->symbol[0] == '-'){
    sign = -1;
    ++l;
  }
  else if (token->symbol[0] == '+') {
    sign = 1;
    ++l;
  }
  else
  {
     sign = 1;
  }
  while(token->symbol[l] != '\0'){
    r = r * 10 + token->symbol[l++] - '0';
  }
  freeToken(token);
  return r * sign;
}

/* YOU WRITE THIS */
static void pushInt(struct tokenStack *s, int v)
{
 
  struct lexToken *token;
  token = allocToken();
  token->kind = LEX_TOKEN_NUMBER;
  sprintf(token->symbol, "%d", v);
  pushTokenStack(s, token);
}

int doOperator(struct tokenStack *stack, char *o) 
{
  struct operator_struct *op = ops;
  for(op=ops;op->name != (char *)NULL; op++) {
    if(!strcmp(op->name, o))
      return op->fn_ptr(stack);
  }
  return(-1);
}

/*ARGSUSED*/
static int op_quit(struct tokenStack *stack)
{
  printf("[quit]\n");
  exit(0);
  /*NOTREACHED*/
}

static int op_print(struct tokenStack *stack)
{
  struct lexToken *t = popTokenStack(stack);
  printToken(stdout, t);
  freeToken(t);
  return(0);
}

static int op_dump(struct tokenStack *stack)
{
  struct lexToken *t = popTokenStack(stack);
  dumpToken(stdout, t);
  freeToken(t);
  return(0);
}

static int op_add(struct tokenStack *stack)
{
  int v1, v2;
  v1 = popInt(stack);
  v2 = popInt(stack);
  pushInt(stack, v1+v2);
  return(0);
}

static int op_showall(struct tokenStack *stack)
{
  int t;

  if (stack->top <= 0) {
    fprintf(stderr,"S: show an empty stack, aborting\n");
    exit(1);
  }
  
  
  for(t = stack->top - 1; t >= 0; --t)
  {
    printToken(stdout, stack->e[t]);
  }
  return (0);
}

static int op_minus(struct tokenStack *stack){
	int v1, v2;
  v1 = popInt(stack);
  v2 = popInt(stack);
  pushInt(stack, v2 - v1);
  return(0);
}

static int op_product(struct tokenStack *stack){
	int v1, v2;
  v1 = popInt(stack);
  v2 = popInt(stack);
  pushInt(stack, v1 * v2);
  return(0);
}

static int op_divide(struct tokenStack *stack){
	int v1, v2, v3;
  v1 = popInt(stack);
  v2 = popInt(stack);
  v3 = 0;
  if (v1 == 0){
		fprintf(stderr,"n2: n1 devide to 0 error\n");
	}else{
		v3 = v2 / v1;
	  pushInt(stack, v3);

	}
  
  return(0);
}

static int op_drop(struct tokenStack *stack){
	popInt(stack);
	return(0);
}

static int op_help(struct tokenStack *stack){
	struct operator_struct *op = ops;
  for(op=ops;op->name != (char *)NULL; op++) {
    fprintf(stdout, "%s %s\n", op->name, op->doc);
      
  }
  return (0);
}

static int op_swap(struct tokenStack *stack){
	struct lexToken *t1, *t2;
	t1 = popTokenStack(stack);
	t2 = popTokenStack(stack);
	pushTokenStack(stack, t1);
	pushTokenStack(stack, t2);
	return(0);
}

static int op_rot(struct tokenStack *stack){
	struct lexToken *t1, *t2, *t3;
	t1 = popTokenStack(stack);
	t2 = popTokenStack(stack);
	t3 = popTokenStack(stack);

	pushTokenStack(stack, t2);
	pushTokenStack(stack, t1);
	pushTokenStack(stack, t3);
	return(0);
}

static int op_rotm(struct tokenStack *stack){
	struct lexToken *t1, *t2, *t3;
	t1 = popTokenStack(stack);
	t2 = popTokenStack(stack);
	t3 = popTokenStack(stack);

	pushTokenStack(stack, t1);
	pushTokenStack(stack, t3);
	pushTokenStack(stack, t2);
	return(0);
}
static int op_rem(struct tokenStack *stack){
	int v1, v2, v3;
  v1 = popInt(stack);
  v2 = popInt(stack);
  v3 = 0;
	if (v1 == 0){
		fprintf(stderr,"n2: n1 devide to 0 aborted\n");
 
	}else{
  v3 = (v2 % v1);
  pushInt(stack, v3);
	}
  return(0);
}
