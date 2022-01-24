#ifndef STACK_IMP_H // include guard
#define STACK_IMP_H
#include"node-imp.h"

void pushListToStack(List* list, List** topOfStack);

void popListFromStack(List** topOfStack);

void printStack(List** topOfStack);

void copyList(List* oldList, List* newList);

int getAdressOfFirstListFromStack(List** list, List** topOfStack);

int clearStack(List** topOfStack);

#endif /* STACK_IMP_H */