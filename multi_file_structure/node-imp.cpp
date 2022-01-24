// node-imp.cpp
#include <iostream>
#include "stack-imp.h"
#include "node-imp.h"// header in local directory // header in standard library

using namespace std;

void prepareList(List* list) {
	list->head = new Node();
	//list->head->data = NULL;
	list->head->next = NULL;
	list->numberOfNodes = 0;
	list->next = NULL;
}

void insertAtBeginning(List* list, string new_data) {
	// Allocate memory to a node
	struct Node* new_node = new Node();

	// insert the data
	new_node->data = new_data;
	new_node->next = list->head;

	// Move head to new node
	list->head = new_node;
}

// Insert a node after a node
void insertAfter(Node* prev_node, string new_data) {
	if (prev_node == NULL) {
		cout << "the given previous node cannot be NULL";
		return;
	}

	struct Node* new_node = new Node();
	new_node->data = new_data;
	new_node->next = prev_node->next;
	prev_node->next = new_node;
}

void insertAtPosition(List* list, int position, string new_data) {
	Node* newNode;
	newNode = new Node();
	newNode->data = new_data;

	if (position <= list->numberOfNodes) {
		Node* temp = list->head;

		for (int i = 2; i < position; i++) {
			if (temp->next != NULL) {
				temp = temp->next;
			}
		}
		newNode->next = temp->next;
		temp->next = newNode;
		list->numberOfNodes++;
	}
	else {
		Node* temp = list->head;
		for (int i = 2; i < position; i++) {
			if (temp->next != NULL) {
				temp = temp->next;
			}
			else {
				insertAtEnd(list, "");
				temp = temp->next;
			}
		}
		newNode->next = temp->next;
		temp->next = newNode;
		list->numberOfNodes++;
	}

}


void replaceText(Node** head_ref, int position, string new_data) {
	struct Node* current = *head_ref;

	for (int i = 1; i < position; i++) {
		if (current->next != NULL) {
			current = current->next;
		}
	}
	current->data = new_data;
}

// Insert at the end
void insertAtEnd(List* list, string new_data) {

	/*if the list containts only one item it means that it is the head node.
	/ So fill the head node with new_data and increase the number of nodes and return. */
	if (list->numberOfNodes == 0)
	{
		list->head->data = new_data;
		list->numberOfNodes++;
		return;
	}


	struct Node* new_node = new Node();
	//struct Node* last = *head_ref;/* used in step 5*/
	struct Node* last = list->head;/* used in step 5*/

	new_node->data = new_data;
	new_node->next = NULL;

	while (last->next != NULL) last = last->next;

	last->next = new_node;
	list->numberOfNodes++;
	return;
}


// Delete a node
void deleteNodeUsingPosition(Node** head_ref, int position) {
	// If linked list is empty
	if (*head_ref == NULL)
		return;

	// Store head node
	Node* temp = *head_ref;

	// If head needs to be removed
	if (position == 0) {

		// Change head
		*head_ref = temp->next;

		// Free old head
		free(temp);
		return;
	}

	// Find previous node of the node to be deleted
	for (int i = 0; temp != NULL && i < position - 1; i++)
		temp = temp->next;

	// If position is more than number of nodes
	if (temp == NULL || temp->next == NULL)
		return;

	// Node temp->next is the node to be deleted
	// Store pointer to the next of node to be deleted
	Node* next = temp->next->next;

	// Unlink the node from linked list
	free(temp->next); // Free memory

	// Unlink the deleted node from list
	temp->next = next;
}


int returnLineFromNode(List* list, int position, string* result) {
	Node* head_ref = list->head;
	if (head_ref == NULL)
		return 1;

	// Store head node
	Node* temp = head_ref;

	// Find previous node of the node to be deleted
	for (int i = 0; temp != NULL && i < position - 1; i++)
	{
		temp = temp->next;
	}

	// If position is more than number of nodes
	if (temp == NULL)
		return 1;

	*result = temp->data;
}


// Print the linked list
void printList(List* list, int DEBUG) {
	if (DEBUG == 1) {
		if (list->numberOfNodes == 0)
		{
			printf("List is empty.\n");
			return;
		}
		else {
			Node* dummyNode = list->head;
			while (dummyNode != NULL) {
				cout << dummyNode->data << " ";
				dummyNode = dummyNode->next;
			}
		}
	}
}

int clearList(List* list) {

	Node* current = list->head;
	Node* next = NULL;

	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}

	/* deref head_ref to affect the real head back
		in the caller. */
	list->head = NULL;
	return 0;
}