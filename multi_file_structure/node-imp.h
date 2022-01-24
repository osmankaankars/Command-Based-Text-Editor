#ifndef NODE_IMP_H // include guard
#define NODE_IMP_H
#include <string>

using namespace std;
typedef struct Node {
	string data;
	Node* next;
} Node;

typedef struct List
{
	Node* head; //head holds the list of nodes. 
	int numberOfNodes; //each list made of number of nodes. 
	List* next; //list nodes are connected which each other.
} List;

void prepareList(List* list);

void insertAtBeginning(List* list, string new_data);

// Insert a node after a node
void insertAfter(Node* prev_node, string new_data);

void insertAtPosition(List* list, int position, string new_data);

// Insert at the end
void insertAtEnd(List* list, string new_data);

// Delete a node
void deleteNodeUsingPosition(Node** head_ref, int position);

// Search and return the line from the Node
int returnLineFromNode(List* list, int position, string* result);

//Replace Text
void replaceText(Node** head_ref, int position, string new_data);

// Print the linked list
void printList(List* list, int DEBUG);

// Clear List
int clearList(List* list);



#endif /* NODE_IMP_H */
