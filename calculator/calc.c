/**
 * @Author: S. Sharma <silentcat>
 * @Date:   2020-01-01T12:25:29-06:00
 * @Email:  silentcat@protonmail.com
 * @Last modified by:   silentcat
 * @Last modified time: 2020-01-02T14:08:27-06:00
 */

/**
 * Task: Make a calculator that performs basic mathematical expressions
 * (+, -, /, *, ^, and parentheses).
 * Solution: Keep track of all the results on 2 stacks. To evaluate an
 * expression, we evaluate the expression in layers according to the order of
 * operations, i.e, we first evaluate parentheses, then exponents, etc.
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>

#define print_err() fprintf(stderr, "Error: %s\n", strerror(errno))
#define MAX_LINE 1000

#define MULT_DIV 0
#define ADD_SUB 1
#define EXP 2

typedef struct stack
{
  char *value;
  struct stack *next;
} STACK;

void push(STACK **s, char *value);
char *pop(STACK **s);
void stack_to_stack(STACK **aux, STACK **s);
char *format_expression(char *expr);
STACK *expr_to_stack(char *expr);
double perform_op(double a, double b, char *token);
STACK *eval_expr_parenth(STACK *s, STACK *tofree);
double eval_expr(STACK *s);


void push(STACK **s, char *value)
{
  if (!*s) {
    *s = calloc(1, sizeof(STACK));
    if (!*s) {
      print_err();
      return;
    }
    (*s)->value = value;
    (*s)->next = NULL;
  } else {
    STACK *n = calloc(1, sizeof(STACK));
    if (!n) {
      print_err();
      return;
    }
    n->value = value;
    n->next = *s;
    *s = n;
  }
}

char *pop(STACK **s)
{
  if (!*s)
    return NULL;
  char *value = (*s)->value;
  STACK *next = (*s)->next;
  free(*s);
  *s = next;
  return value;
}

void stack_to_stack(STACK **aux, STACK **s)
{
  while (*aux)
    push(s, pop(aux));
}

char *format_expression(char *expr)
{
  char *nexpr = calloc(1, 2*strlen(expr)+1);
  if (!nexpr) {
    print_err();
    return NULL;
  }

  int i = 0, k = 0;
  while (i < strlen(expr)) {
    if (expr[i] != ' ' && expr[i] != '\n' && expr[i] != '\t') {
      if (isdigit(expr[i]) || (expr[i] == '-' && isdigit(expr[i+1]) && (i == 0 || !isdigit(expr[i-1])))) {
        k = i;
        if (expr[k] == '-')
          strncat(nexpr, &expr[k++], 1);
        while (isdigit(expr[k]) || expr[k] == '.')
          strncat(nexpr, &expr[k++], 1);
        if (k < strlen(expr)-1)
          strncat(nexpr, " ", 1);
        i = k;
      } else {
        strncat(nexpr, &expr[i++], 1);
        if (i < strlen(expr)-1)
          strncat(nexpr, " ", 1);
      }
    } else
        ++i;
  }
  return nexpr;
}

STACK *expr_to_stack(char *expr)
{
  char *cpy = strdup(expr), *token;
  STACK *s = NULL, *aux;
  while ((token = strsep(&cpy, " ")))
    push(&aux, token);
  free(cpy);
  while (aux)
    push(&s, pop(&aux));
  return s;
}

double perform_op(double a, double b, char *token)
{
  double res = 0;
  switch(token[0]) {
    case '+':
      res = a + b;
      break;
    case '-':
      res = a - b;
      break;
    case '*':
      res = a * b;
      break;
    case '/':
      res = a / b;
      break;
    case '^':
      res = pow(a, b);
      break;
  }
  return res;
}

STACK *eval_expr_parenth(STACK *s, STACK *tofree)
{
  STACK *aux = NULL;
  while (s) {
    char *token = pop(&s);
    if (!strcmp(token, ")")) {
      /* Pop off all tokens until we reach a '(' */
      token = pop(&aux);
      STACK *tmp = NULL;
      while (strcmp(token, "(")) {
        push(&tmp, token);
        token = pop(&aux);
      }
      double res = eval_expr(tmp);
      char *res_str = calloc(1, 20);
      snprintf(res_str, 20, "%e", res);
      push(&tofree, res_str);
      push(&aux, res_str);
    } else
      push(&aux, token);
  }
  return aux;
}

double eval_expr(STACK *s)
{
  int op_found = 0, types[] = { EXP, MULT_DIV, ADD_SUB }, len = 3, i = 0;
  double final = 0;
  STACK *aux = NULL;

  do {
    int type = types[i++];
    stack_to_stack(&aux, &s); // From aux to s.
    while (s) {
      char *token = pop(&s);
      char res_str[20];
      if ((type == ADD_SUB && (!strcmp(token, "+") || !strcmp(token, "-"))) ||
          (type == MULT_DIV && (!strcmp(token, "*") || !strcmp(token, "/"))) ||
           (type == EXP && !strcmp(token, "^"))) {
             op_found = 1;
             push(&aux, token);
      } else if (op_found && (isdigit(token[0]) || token[0] == '-')) {
        char *operator = pop(&aux), *operand = pop(&aux);
        double op2 = strtod(token, NULL), op1 = strtod(operand, NULL);
        double res = perform_op(op1, op2, operator);
        snprintf(res_str, 20, "%e", res);
        push(&s, res_str);
        op_found = 0;
      } else if (!op_found) {
        push(&aux, token);
      }
    }
  } while (aux->next && i < len);
  final = strtod(pop(&aux), NULL);
  return final;
}

int main(int argc, char *argv[])
{
  char buffer[MAX_LINE+1], *expr;
  printf("Type 'quit' to exit\n");
  while (1) {
    printf("> ");
    fgets(buffer, MAX_LINE+1, stdin);
    if (!strcmp(buffer, "quit\n"))
      break;
    expr = format_expression(buffer);
    STACK *s = expr_to_stack(expr);
    STACK *tmp = NULL, *tofree = NULL;
    s = eval_expr_parenth(s, tofree);
    stack_to_stack(&s, &tmp);
    printf("%f\n", eval_expr(tmp));
    while (tofree) {
      char *value = pop(&tofree);
      free(value);
    }
    free(expr);
  }
  return 0;
}
