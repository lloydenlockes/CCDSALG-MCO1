#include <stdio.h>
#include "stack.h"

static void stack_error(const char *msg) { fprintf(stderr, "Stack error: %s\n", msg); }

void  CREATE(Stack *S)      
{
    S->top = -1;
}

int   ISEMPTY(const Stack *S)
{
    return S->top == -1;
}
int   ISFULL(const Stack *S)  
{
    return S->top == MAX_N - 1;
}

void PUSH(Stack *S, Point e) {
    if (ISFULL(S)) { stack_error("overflow"); return; }
    S->items[++S->top] = e;
}
Point POP(Stack *S) {
    Point none = {0.0, 0.0};
    if (ISEMPTY(S)) { stack_error("underflow"); return none; }
    return S->items[S->top--];
}
Point TOP(const Stack *S) {
    Point none = {0.0, 0.0};
    if (ISEMPTY(S)) { stack_error("TOP on empty stack"); return none; }
    return S->items[S->top];
}
Point NEXT_TO_TOP(const Stack *S) {
    Point none = {0.0, 0.0};
    if (S->top < 1) { stack_error("NEXT-TO-TOP needs 2 elements"); return none; }
    return S->items[S->top - 1];
}
