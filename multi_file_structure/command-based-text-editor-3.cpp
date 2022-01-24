// Linked list operations in C++

#include <stdlib.h>
#include <iostream>
#include <fstream> //file functions
#include <string> //string library
#include <sstream> //string library used for converting string to integer, required for controls. 
#include "node-imp.h" //Node related functions are here. 
#include "stack-imp.h" //Stack implementations are here.

#define PRINT_LIST 0 //1 means that debug is open, it will print everything for debug purposes


using namespace std;

int openAndLoadFile(string FileName, List* applicationList);
int insertLineToList(List* applicationList, List** applicationStack, string line, int position);
int undoTheList(List** applicationList, List** applicationStack);
int deleteLineFromList(List* applicationList, List** applicationStack, int position);
int replaceLine(List* applicationList, List** applicationStack, string line, int position);
int openAndSaveToFile(string FileName, List* applicationList);
int printPage(List* applicationList, int pagePosition);
int moveLine(List* applicationList, List** applicationStack, int position_n, int position_m);

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