#include <stdlib.h>
#include <iostream>
#include <fstream> //file functions
#include <string> //string library
#include <sstream> //string library used for converting string to integer, required for controls. 

#define PRINT_LIST 0 //1 means that debug is open, it will print everything for debug purposes

using namespace std;

//Each node holds a line. 
typedef struct Node {
	string data;
	Node* next;
} Node;

//Each list holds nodes, also stack is also made of lists.
typedef struct List
{
	Node* head; //head holds the list of nodes. 
	int numberOfNodes; //each list made of number of nodes. 
	List* next; //list nodes are connected which each other.
} List;

/* List Implementation Functions */
void prepareList(List* list);
void insertAtBeginning(List* list, string new_data);
void insertAfter(Node* prev_node, string new_data); // Insert a node after a node
void insertAtPosition(List* list, int position, string new_data);
void insertAtEnd(List* list, string new_data); // Insert at the end
void deleteNodeUsingPosition(Node** head_ref, int position); // Delete a node
int returnLineFromNode(List* list, int position, string* result); // Search and return the line from the Node
void replaceText(Node** head_ref, int position, string new_data); //Replace Text
void printList(List* list, int DEBUG); // Print the linked list
int clearList(List* list); // Clear List

/* Stack Implementation Functions */
void pushListToStack(List* list, List** topOfStack);
void popListFromStack(List** topOfStack);
void printStack(List** topOfStack);
void copyList(List* oldList, List* newList);
int getAdressOfFirstListFromStack(List** list, List** topOfStack);
int clearStack(List** topOfStack);

/* General Functions that both uses Stack and List */
int openAndLoadFile(string FileName, List* applicationList);
int insertLineToList(List* applicationList, List** applicationStack, string line, int position);
int undoTheList(List** applicationList, List** applicationStack);
int deleteLineFromList(List* applicationList, List** applicationStack, int position);
int replaceLine(List* applicationList, List** applicationStack, string line, int position);
int openAndSaveToFile(string FileName, List* applicationList);
int printPage(List* applicationList, int pagePosition);
int moveLine(List* applicationList, List** applicationStack, int position_n, int position_m);

/* Other Functions */
bool isNumber(const string& s);
void printCommands();


// Driver program
int main() {
	cout << " ----- COMMAND BASED TEXT EDITOR -----";
	printCommands();

	/* Create a new list and new stack to try first*/
	List applicationList;
	List* adressOfApplicationList = NULL;
	List* applicationStack = NULL;
	prepareList(&applicationList);

	string option;
	int pagePosition = 0;
	int stackAndListModifiedFlag = 0;

	while (1)
	{
		std::cout << "\nWaiting for your command: ";
		getline(cin, option);
		stringstream stringStream(option);
		string token;
		string tokens[3];


		int i = 0;
		while (getline(stringStream, tokens[i], ' ')) {
			i++;
			//if the command is "insert n text" or "replace n text" it will enter here thus not divide the spaces of the line.
			if (i == 2) {
				getline(stringStream, tokens[i]);
				break;
			}
		}
		if (tokens[0] == "open") {
			//if the stack is not modified we dont need to free the stack because it is new already.
			if (stackAndListModifiedFlag == 0) {
				cout << "\nOpening and loading the file...\n";
				if (openAndLoadFile(tokens[1], &applicationList) != 1) {
					stackAndListModifiedFlag = 1;
					printPage(&applicationList, pagePosition);
					printList(&applicationList, PRINT_LIST);
				}
			}
			//if the stack is modified means that we need free the stack so that undo cant be done before new file is loaded.
			else {
				clearStack(&applicationStack);
				applicationStack = NULL;

				clearList(&applicationList);
				adressOfApplicationList = NULL;
				prepareList(&applicationList);

				cout << "\nOpening and loading the file...\n";
				if (openAndLoadFile(tokens[1], &applicationList) != 1) {
					stackAndListModifiedFlag = 1;
					printPage(&applicationList, pagePosition);
					printList(&applicationList, PRINT_LIST);
				}
			}
		}
		else if (tokens[0] == "save") {
			cout << "\nSaving current list to a file...\n";
			openAndSaveToFile(tokens[1], &applicationList);
		}
		else if (tokens[0] == "insert") {
			if (isNumber(tokens[1])) {
				stringstream intValue(tokens[1]);
				int position = 0;
				intValue >> position;
				cout << "\nInserting new lines to the list...\n";
				insertLineToList(&applicationList, &applicationStack, tokens[2], position);
				printPage(&applicationList, pagePosition);
				printList(&applicationList, PRINT_LIST);
			}
			else {
				cout << "\nCommand Error. Position is wrong...\n";
			}
		}
		else if (tokens[0] == "delete") {
			if (isNumber(tokens[1])) {
				stringstream intValue(tokens[1]);
				int position = 0;
				intValue >> position;
				deleteLineFromList(&applicationList, &applicationStack, position);
				printPage(&applicationList, pagePosition);
				printList(&applicationList, PRINT_LIST);
			}
			else {
				cout << "\nCommand Error. Position is wrong...\n";
			}

		}
		else if (tokens[0] == "move") {
			//move deletes the line N and inserts it in the position M thus carrying it. 
			if (isNumber(tokens[1]) && isNumber(tokens[2])) {
				stringstream intValue_n(tokens[1]);
				int position_n = 0;
				intValue_n >> position_n;

				stringstream intValue_m(tokens[2]);
				int position_m = 0;
				intValue_m >> position_m;

				cout << "\nMoving line n to m...\n";
				moveLine(&applicationList, &applicationStack, position_n, position_m);
				printPage(&applicationList, pagePosition);

			}
			else {
				cout << "\nERROR: Command Error. Position is wrong...\n";
			}
		}
		else if (tokens[0] == "replace") {
			if (isNumber(tokens[1])) {
				stringstream intValue(tokens[1]);
				int position = 0;
				intValue >> position;
				cout << "\nReplacing second line from the list...\n";
				replaceLine(&applicationList, &applicationStack, tokens[2], position);
				printPage(&applicationList, pagePosition);
				printList(&applicationList, PRINT_LIST);
			}
			else {
				cout << "\nERROR: Command Error. Position is wrong...\n";
			}
		}
		else if (tokens[0] == "next") {
			int i = 1;
			int emptyPage = 0;
			pagePosition++;
			printPage(&applicationList, pagePosition);
		}
		else if (tokens[0] == "prev") {
			int i = 1;
			int emptyPage = 0;
			if (pagePosition <= 0) {
				cout << "WARNING: There is no page left. Printing the same page...\n";
				printPage(&applicationList, pagePosition);
			}
			else {
				pagePosition--;
				printPage(&applicationList, pagePosition);
			}
		}
		else if (tokens[0] == "undo") {
			cout << "\nUndoing the modified list...\n";
			adressOfApplicationList = &applicationList;
			undoTheList(&adressOfApplicationList, &applicationStack);
			applicationList = *adressOfApplicationList;
			printPage(&applicationList, pagePosition);
			printList(&applicationList, PRINT_LIST);
		}
		else if (tokens[0] == "close" || tokens[0] == "exit") {
			cout << "\nClosing the file without saving...\n";
			exit(0);
		}
		else if (tokens[0] == "help") {
			printCommands();
		}
		else
		{
			cout << "\nERROR: Wrong command. Please try again.\n";
		}
	}
}

//---------------------------------- GENERAL FUNCTIONS START----------------------------------//
bool isNumber(const string& s)
{
	string::const_iterator it = s.begin();
	while (it != s.end() && isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

int openAndLoadFile(string FileName, List* applicationList) {
	string currentLine;
	ifstream myfile(FileName);
	if (myfile.is_open())
	{
		while (getline(myfile, currentLine))
		{
			//cout << currentLine << '\n';
			insertAtEnd(applicationList, currentLine);
		}
		myfile.close();
		return 0;
	}
	else { cout << "Error: Unable to open file"; return 1; }
	return 0;
}

int openAndSaveToFile(string FileName, List* applicationList) {
	ofstream myfile(FileName);
	Node* current = applicationList->head;
	if (myfile.is_open())
	{
		while (current != NULL) {
			myfile << current->data << "\n";
			current = current->next;
		}
		myfile.close();
		return 0;
	}
	else { cout << "Unable to open file"; return 1; }
	return 0;
}

int insertLineToList(List* applicationList, List** applicationStack, string line, int position) {

	pushListToStack(applicationList, applicationStack);
	insertAtPosition(applicationList, position, line); //in the function add new line to the end if it is not added.
	return 0;
}

int undoTheList(List** applicationList, List** applicationStack) {
	if (*applicationStack != NULL) {
		List* temp;
		temp = new List();
		prepareList(temp); //added new
		getAdressOfFirstListFromStack(applicationList, applicationStack);
		//printList(*applicationList);
		copyList(*applicationList, temp); //added new
		*applicationList = temp;
		popListFromStack(applicationStack);
	}
	else {
		cout << "Cant Undo because nothing is added.\n";
	}
	return 0;
}

int deleteLineFromList(List* applicationList, List** applicationStack, int position) {
	pushListToStack(applicationList, applicationStack);
	deleteNodeUsingPosition(&(*applicationList).head, position - 1);
	return 0;
}

int replaceLine(List* applicationList, List** applicationStack, string line, int position) {
	pushListToStack(applicationList, applicationStack);
	replaceText(&(*applicationList).head, position, line);
	return 0;
}

int moveLine(List* applicationList, List** applicationStack, int position_n, int position_m) {
	pushListToStack(applicationList, applicationStack);
	string currentString;
	returnLineFromNode(applicationList, position_n, &currentString);
	deleteNodeUsingPosition(&(*applicationList).head, position_n - 1);
	cout << "After delete\n";
	if (position_m == 1) {
		insertAtBeginning(applicationList, currentString);
	}
	else {
		insertAtPosition(applicationList, position_m, currentString); //in the function add new line to the end if it is not added.
	}
	cout << "After insert\n";
	return 0;
}

int printPage(List* applicationList, int pagePosition) {
	int emptyPage = 0;
	int i = 1;
	Node* current = applicationList->head;
	while (current != NULL) {
		if (i > pagePosition * 10 && i <= ((pagePosition * 10) + 10)) {
			cout << i << ") " << current->data << "\n";
			emptyPage++;
		}
		current = current->next;
		i++;
	}
	if (emptyPage == 0) {
		cout << "Empty Page\n";
		cout << "\n\n--- Page " << pagePosition + 1 << " ---\n";
		return 1;
	}
	else
	{
		cout << "\n\n--- Page " << pagePosition + 1 << " ---\n";
		return 0;
	}
}

void printCommands() {
	cout << "\n ----- Command List ----- \n";
	cout << "open filename -opens the file and loads them.\n";
	cout << "save filename -saves the current changes to file, cant undo after this command.\n";
	cout << "insert n text: -n equals position. inserts text to any position(line).\n";
	cout << "delete n : -n equals position. deletes any line.\n";
	cout << "move n m : -n and m equals position. moves any line from n to m\n";
	cout << "replace n text : -n position. replaces text in the line n.\n";
	cout << "next : -displays next page (next 10 line)\n";
	cout << "prev : -displays next page (next 10 line)\n";
	cout << "undo : -reverts the last taken action. starts with open filename command.\n";
	cout << "help : -prints command list\n";
	cout << "exit : -exit program without saving\n";
	cout << " ----- Command List ----- \n";
}

//---------------------------------- GENERAL FUNCTIONS FINISH ----------------------------------//




//---------------------------------- STACK FUNCTIONS START -------------------------------------//

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

//---------------------------------- STACK FUNCTIONS FINISH -------------------------------------//

//---------------------------------- LIST FUNCTIONS START -------------------------------------//
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

//---------------------------------- LIST FUNCTIONS FINISH -------------------------------------//