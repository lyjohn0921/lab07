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
  int (*fn_ptr)(struct tokenStack *);
} ops[] = {
  {"quit", op_quit},
  {"print", op_print},
  {"dump", op_dump},
  {"+", op_add},
  {"-", op_minus},
  {"*", op_product},
  {"/", op_divide},
  {"DROP", op_drop},
  {"HELP", op_help},
  {"S", op_showall},
  {"SWAP", op_swap},
  {"ROT", op_rot},
  {"ROTMINUS", op_rotm},
  {"MODQUOT", op_rem},
  {(char *)NULL, (int(*)(struct tokenStack *)) NULL}
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
	int v1, v2;
  v1 = popInt(stack);
  v2 = popInt(stack);
  pushInt(stack, v2 / v1);
  return(0);
}

static int op_drop(struct tokenStack *stack){
	popInt(stack);
	return(0);
}

static int op_help(struct tokenStack *stack){
	struct operator_struct *op = ops;
  for(op=ops;op->name != (char *)NULL; op++) {
    fprintf(stdout, "%s\n", op->name);
      
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
  v3 = (v2 % v1);

  pushInt(stack, v3);
  return(0);
}