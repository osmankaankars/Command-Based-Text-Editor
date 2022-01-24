// stack-imp.cpp
#include <iostream> // header in standard library
#include "stack-imp.h" 
#include "node-imp.h"// header in local directory

using namespace std;

void pushListToStack(List* list, List** topOfStack) {
	// Create new node temp and allocate memory
	List* temp;
	temp = new List();
	prepareList(temp); //added new
	copyList(list, temp); //added new

	// Check if stack (heap) is full.
	// Then inserting an element would
	// lead to stack overflow
	if (!temp)
	{
		cout << "\nHeap Overflow";
		exit(1);
	}

	// Initialize data into temp data field
	// temp->head = list->head;
	//temp->numberOfNodes = list->numberOfNodes;

	// Put top pointer reference into temp link
	temp->next = *topOfStack;

	// Make temp as top of Stack
	*topOfStack = temp;
}

void copyList(List* oldList, List* newList) {
	if (oldList->numberOfNodes == 0)
	{
		printf("List is empty.\n");
		return;
	}
	else {
		Node* dummyNode = oldList->head;
		while (dummyNode != NULL) {
			insertAtEnd(newList, dummyNode->data);
			dummyNode = dummyNode->next;
		}
	}
}


void popListFromStack(List** topOfStack) {
	List* temp;

	// Check for stack underflow
	if (topOfStack == NULL)
	{
		cout << "\nStack Underflow" << endl;
		exit(1);
	}
	else
	{

		// Top assign into temp
		temp = *topOfStack;

		// Assign second node to top
		*topOfStack = (*topOfStack)->next;

		// Destroy connection between
		// first and second
		temp->next = NULL;

		// Release memory of top node
		free(temp);
	}
}


void printStack(List** topOfStack) {
	List* temp;

	// Check for stack underflow
	if (*topOfStack == NULL)
	{
		cout << "\nStack Underflow - No item left in the stack.";
		return;
	}
	else
	{
		temp = *topOfStack;
		cout << "\nPrinting stack\n";
		while (temp != NULL)
		{

			// Print node data
			//cout << "Number of nodes:" << temp->numberOfNodes << " --> ";
			//printList(temp,1);

			// Assign temp link to temp
			temp = temp->next;
		}
	}
}

int getAdressOfFirstListFromStack(List** list, List** topOfStack)
{

	// Check for empty stack
	if (!(*topOfStack == NULL))
		//*list = (*topOfStack)->next;
		*list = (*topOfStack);
	else {
		cout << "Stack is empty";
		return 1;
	}
	return 0;
}

int clearStack(List** topOfStack) {
	List* temp;

	// Check for stack underflow
	if (*topOfStack == NULL)
	{
		cout << "\nStack Underflow - No item left in the stack.";
		return 1;
	}
	else
	{
		temp = *topOfStack;
		while (temp != NULL)
		{

			// Print node data
			clearList(temp);

			// Assign temp link to temp
			temp = temp->next;
		}
	}
	return 0;
}