#include	<iostream>
#include	<iomanip>
#include	<cstdlib>
#include	<cstdio>
#include    <fstream>
#include    <string>
#include	"List.h"
#include    "LibStudent.h"
#include    "LibBook.h"



using namespace std;

//Function prototype
bool ReadFile(string, List*);
bool Redundant(List*, LibStudent&);
bool DeleteRecord(List*, char*);
bool Display(List*, int, int);
bool InsertBook(string, List*);
bool ExtractAuthor(char[], LibBook&);
bool calculateFine(LibBook&);
bool SearchStudent(List*, char* id, LibStudent&);
bool computeAndDisplayStatistics(List*);
bool printStuWithSameBook(List*, char*);
bool displayWarnedStudent(List*, List*, List*);
int rdn(int, int, int);
int menu();


int main() {
	List* list = new List();
	List* type1 = new List(), * type2 = new List();
	LibStudent stu;
	char id[10];
	int source, detail, choice;

	do {
		choice = menu();        //prompt user to input a choice
		if (cin.fail())
		{
			cout << "No alphabet please, please enter a value in range 1-9" << endl;
			cin.clear(); // Clear the error flag
			cin.ignore(); // Clear the input buffer
			continue;
		}

		system("cls"); // Clear the screen after getting the choice
		if (choice == 1) {                            //option 1: Read file
			ReadFile("student.txt", list);
			cout << endl;
			cout << list->count << " records have been successfully read." << endl;
		}
		else if (choice == 2) {                       //option 2: Delete record
			cout << "\nPlease enter the student ID of the record you wish to remove: ";
			cin.ignore();
			cin.getline(id, 10);
			DeleteRecord(list, id);
		}

		else if (choice == 3) {                       //option 3: Search student
			cout << "\nPlease key in the student's ID you want to search: ";
			cin.ignore();
			cin.getline(id, 10);
			SearchStudent(list, id, stu);
		}

		else if (choice == 4) {                       //option 4: Insert book
			InsertBook("book.txt", list);
		}

		else if (choice == 5) {                       //option 5: Display output
			cout << "\n\nDISPLAY OUTPUT" << endl;
			cout << "Where do you want to display the output (1 - File / 2 - Screen): ";
			cin >> source;
			cout << "Do you want to display book list for every student (1 - YES / 2 - NO): ";
			cin >> detail;
			Display(list, source, detail);
			cout << endl;
			cout << "Successfully display output\n\n";

		}

		else if (choice == 6) {                       //option 6: Compute and Display Statistics
			computeAndDisplayStatistics(list);
		}

		else if (choice == 7) {                       //option 7: Student with Same Book
			if (list->empty())
				InsertBook("book.txt", list);
			char* callNum = new char();
			cout << "Please enter the Call Number of a book: ";
			cin >> callNum;
			printStuWithSameBook(list, callNum);

		}

		else if (choice == 8) {                       //option 8: Display Warned Student
			displayWarnedStudent(list, type1, type2);
		}
		else if (choice == 9)   //exit the loop if user selects "exit" option
		{
			break;
		}
		else if (choice < 1 || choice>9)
			cout << "Invalid input, please enter value in range 1-9" << endl;

		cout << endl;

	} while (choice < 1 || choice > 9 || choice != 9);

	cout << "\n\n";
	system("pause");
	return 0;
}

bool ReadFile(string filename, List* list) {          //read from student.txt and store information in a linked list

	string info;
	ifstream readFile;

	if (list->empty()) {                              //ensure there will be no duplicate content of the list if user continue to read the file again
		readFile.open(filename);

		if (readFile.is_open()) {
			while (!readFile.eof()) {

				LibStudent student;

				//Read the text file and store it into student struct
				getline(readFile, info, '=');
				readFile >> student.id;

				getline(readFile, info, '=');
				readFile.ignore();
				readFile.getline(student.name, 30);

				getline(readFile, info, '=');
				readFile >> student.course;

				getline(readFile, info, '=');
				readFile >> student.phone_no;

				if (!Redundant(list, student)) {      //ensure there is no duplicate record of student stored in the list
					list->insert(student);            //insert the content of student struct into the list by using insert function in list.cpp
				}
				else {
					cout << "\nThere are duplicate records for the student with student ID " << student.id << endl;
					cout << "Deleting the duplicate records........." << endl;
					cout << "The duplicated records has been successfully deleted.\n " << endl;
				}
			}
		}
		else {
			cout << "Unable to open " << filename << "\n" << endl;
			return false;
		}

		readFile.close();
	}
	return true;
}

bool Redundant(List* list, LibStudent& tempstd) {      //check for redundant student information

	LibStudent store;                                  //get the item inside the node and do comparison

	if (list->empty())
		return false;


	for (int i = 1; i <= list->size(); i++) {

		if (list->get(i, store)) {

			if (store.compareName2(tempstd))
				return true;

		}
	}

	return false;
}

bool DeleteRecord(List* list, char* id) {             //delete a student record from the linked list

	if (list->empty())                                //check if the list is empty
		ReadFile("student.txt", list);

	int i = 1;
	Node* cur;
	cur = list->head;                                 //traverse the list

	while (strcmp(id, cur->item.id) != 0) {           //check if the student ID is in the list or not
		cur = cur->next;

		if (cur == NULL) {                              //indicates no record of student ID in the list
			cout << "There are no records for student with student ID " << id << endl;
			cout << endl;
			return false;
		}

		i++;
	}

	if (strcmp(id, cur->item.id) == 0) {
		list->remove(i);                              //Remove node from the list by using the remove function in list.cpp
		cout << "Student record with the student ID " << id << " has been deleted.\n" << endl;
	}

	return true;
}


bool SearchStudent(List* list, char* id, LibStudent& stu) {       //search a sudent in the linked list

	if (list->empty())                                //check if the list is empty
		ReadFile("student.txt", list);

	int i = 1;
	Node* cur;
	cur = list->head;                                 //traverse the list

	while (strcmp(id, cur->item.id) != 0) {            //check if the student ID is in the list or not
		cur = cur->next;

		if (cur == NULL) {                            //indicates no record of student ID in the list
			cout << "There are no records for student with student ID " << id << endl;
			cout << endl;
			return false;
		}

		if (strcmp(id, cur->item.id) == 0) {
			strcpy(stu.id, cur->item.id);
			strcpy(stu.name, cur->item.name);
			strcpy(stu.course, cur->item.course);
			strcpy(stu.phone_no, cur->item.phone_no);
			break;
		}

		i++;
	}

	cout << "\nSearched Student Information: " << endl;          //print the searched student info
	cout << "Student ID: " << stu.id << endl;
	cout << "Student Name: " << stu.name << endl;
	cout << "Course: " << stu.course << endl;
	cout << "Phone No: " << stu.phone_no << endl;

	return true;
}

bool InsertBook(string filename, List* list) {        //insert book borrowed for a student to the linked list

	if (list->empty())							      //check if the list is empty 
		ReadFile("student.txt", list);

	ifstream readFile;
	readFile.open(filename);
	int i = 0;
	char id[20];
	char authorname[200];
	Node* cur;

	cur = list->head;

	if (cur->item.totalbook == 0) {              //ensure there will be no duplicate record of book in the list if user continue to call InsertBook function again
		if (readFile.is_open()) {

			while (!readFile.eof()) {

				LibBook books;

				readFile >> id;                       //read the text file and store the information into the variables and struct declared
				readFile >> authorname;

				if (readFile)
					ExtractAuthor(authorname, books);      //extract several authors

				readFile >> books.title >> books.publisher >> books.ISBN >> books.yearPublished >> books.callNum;

				for (int i = 0; i <= strlen(books.title); i++) {        //traverse the book title and replace the underscore with space
					if (books.title[i] == '_')
						books.title[i] = ' ';
				}

				for (int i = 0; i <= strlen(books.publisher); i++) {    //traverse the book publisher and replace the underscore with space
					if (books.publisher[i] == '_')
						books.publisher[i] = ' ';
				}

				readFile >> books.borrow.day;
				readFile.ignore(2, '/');
				readFile >> books.borrow.month;
				readFile.ignore(2, '/');
				readFile >> books.borrow.year;
				readFile.ignore();
				readFile >> books.due.day;
				readFile.ignore(2, '/');
				readFile >> books.due.month;
				readFile.ignore(2, '/');
				readFile >> books.due.year;
				readFile.ignore();

				if (readFile) {

					books.fine = 0;
					calculateFine(books);              //calculate the fine of each book record

					//traverse through the list and insert the read book info to the array book and total fine based on ID read
					for (int j = 1; j <= list->size(); j++) {

						cur = list->find(j);

						if (strcmp(cur->item.id, id) == 0) {                 //add each book's information that stored in the struct into the list if the student ID is matched
							cur->item.book[cur->item.totalbook++] = books;
							cur->item.calculateTotalFine();                  //calculate the total fine
						}
					}
				}

				i++;
			}
		}
		else {
			cout << "Unable to open " << filename << "\n" << endl;
			return false;
		}
		readFile.close();
	}
	cout << "Books inserted successfully to student list." << endl;
	return true;
}

bool ExtractAuthor(char authorname[], LibBook& book) {             //extract the authors into the author array of book struct

	char temp[200];
	int idx = 0;
	int z = 0;

	for (int i = 0; i <= strlen(authorname); i++) {   //traverse the authorname and replace the underscore with space

		if (authorname[i] == '_') {
			authorname[i] = ' ';
		}

		if (authorname[i] != '/') {                   //extract the authorname if slash symbol is found in the authorname
			temp[idx++] = authorname[i];
		}
		else {
			temp[idx++] = NULL;
		}

		if (temp[idx - 1] == NULL) {
			idx = 0;
			book.author[z] = new char[100];
			strcpy(book.author[z++], temp);
		}

	}
	return true;
}

bool calculateFine(LibBook& book) {                   //calculate total due days and calculate the total fine amount

	Date dateNow;
	int DateNow, DateDue, totalDueDay;

	dateNow.year = 2020;
	dateNow.month = 3;
	dateNow.day = 29;

	//calculation of the due and current date with Julian Date Format
	DateNow = (1461 * (dateNow.year + 4800 + (dateNow.month - 14) / 12)) / 4 +
		(367 * (dateNow.month - 2 - 12 * ((dateNow.month - 14) / 12))) / 12 - (3 * ((dateNow.year + 4900 + (dateNow.month - 14) / 12) / 100)) / 4 + dateNow.day - 32075;

	DateDue = (1461 * (book.due.year + 4800 + (book.due.month - 14) / 12)) / 4 +
		(367 * (book.due.month - 2 - 12 * ((book.due.month - 14) / 12))) / 12 -
		(3 * ((book.due.year + 4900 + (book.due.month - 14) / 12) / 100)) / 4 + book.due.day - 32075;

	totalDueDay = DateNow - DateDue;      //calculate how many days the book is due

	if (totalDueDay > 0)                              //check if the book is due and perform calculations to calculate the fine amount
		book.fine = totalDueDay * 0.5;
	else
		book.fine = 0;

	return true;
}

bool Display(List* list, int source, int detail) {    //display the information from the list 
	LibBook books;
	int studentno = 1;

	//InsertBook("book.txt", list);
	if (list->empty())
		InsertBook("book.txt", list);                 //ensure there will be no duplicate content of list if user continue to run the function again

	ofstream out("student_info.txt");
	ofstream outbook("student_booklist.txt");

	Node* cur;

	cur = list->head;                                 //traverse the list

	while (cur != NULL) {
		calculateFine(books);
		if (source == 1 && detail == 2) {              //display student information in txt file without book list
			out << "\nStudent " << studentno;
			cur->item.print(out);
			out << "\n*********************************************************" << endl;
		}

		if (source == 1 && detail == 1) {             //display student information in txt file with book list
			outbook << "\nStudent " << studentno;
			cur->item.print(outbook);
			cout << endl;
			outbook << "BOOK LIST:\n\n";

			for (int bookno = 0; bookno < cur->item.totalbook; bookno++) {
				outbook << "Book " <<
					bookno + 1 << endl;
				cur->item.book[bookno].print(outbook);
				outbook << endl;
			}
			outbook << "\n*********************************************************" << endl;

		}

		if (source == 2 && detail == 1) {             //display student information on screen with book list
			cout << "STUDENT " << studentno;
			cur->item.print(cout);
			cout << endl;
			cout << "BOOK LIST:" << endl;

			for (int bookno = 0; bookno < cur->item.totalbook; bookno++) {
				cout << "\nBook " << bookno + 1 << endl;
				cur->item.book[bookno].print(cout);
			}
			cout << "\n****************************************************\n";
		}

		if (source == 2 && detail == 2) {             //display student information on screen without book list

			cout << "STUDENT " << studentno;
			cur->item.print(cout);

			cout << endl;
			cout << "\n****************************************************\n";
		}

		studentno++;
		cur = cur->next;
	}
	out.close();
	outbook.close();
	return true;
}

bool computeAndDisplayStatistics(List* list)
{
	string course[5] = { "CS", "IA", "IB", "CN", "CT" };
	// create array to store the data-easier access using for loop when printing
	int num[5] = { 0,0,0,0,0 }, total[5] = { 0,0,0,0,0 }, totaoOverdue[5] = { 0, 0, 0, 0, 0 };
	double fines[5] = { 0,0,0,0,0 };
	LibBook book;
	LibStudent stu;
	if (list->empty())
	{
		cout << "There is no record." << endl;
		return false;
	}
	else
	{
		for (int i = 1; i <= list->size(); i++)
		{
			list->get(i, stu);
			if (strcmp(stu.course, "CS") == 0)
			{
				num[0] += 1;
				total[0] += stu.totalbook;
				fines[0] += stu.total_fine;
				//calculate number of books overdue
				for (int j = 0; j < stu.totalbook; j++)  // identify total book borrowed by stu[i]
				{
					if (stu.book->fine == NULL)  // arrive last node 
						break;
					if (stu.book->fine > 0.0)  // there is book overdue exist
					{
						totaoOverdue[0] += 1;
					}
				}

			}
			else if (strcmp(stu.course, "IA") == 0)
			{
				num[1] += 1;
				total[1] += stu.totalbook;
				fines[1] += stu.total_fine;
				for (int j = 0; j < stu.totalbook; j++)
				{
					if (stu.book->fine > 0.0)
					{
						totaoOverdue[1] += 1;
						if (stu.book->fine == NULL)
							break;
					}
				}
			}
			else if (strcmp(stu.course, "IB") == 0)
			{
				num[2] += 1;
				total[2] += stu.totalbook;
				fines[2] += stu.total_fine;
				for (int j = 0; j < stu.totalbook; j++)
				{
					if (stu.book->fine > 0.0)
					{
						totaoOverdue[2] += 1;
						if (stu.book->fine == NULL)
							break;
					}
				}
			}
			else if (strcmp(stu.course, "CN") == 0)
			{
				num[3] += 1;
				total[3] += stu.totalbook;
				fines[3] += stu.total_fine;
				for (int j = 0; j < stu.totalbook; j++)
				{
					if (stu.book->fine > 0.0)
					{
						totaoOverdue[3] += 1;
						if (stu.book->fine == NULL)
							break;
					}
				}
			}
			else if (strcmp(stu.course, "CT") == 0)
			{
				num[4] += 1;
				total[4] += stu.totalbook;
				fines[4] += stu.total_fine;
				for (int j = 0; j < stu.totalbook; j++)
				{
					if (stu.book->fine > 0.0)
					{
						totaoOverdue[4] += 1;
						if (stu.book->fine == NULL)
							break;
					}
				}
			}
		}
		int blank = 23;
		cout << "Course" << setw(blank) << "Number of Students" << setw(blank)
			<< "Total Books Borrowed" << setw(blank) << "Total Overdue Books"
			<< setw(blank) << "   Total Overdue Fine (RM)" << endl;
		for (int i = 0; i < 5; i++) {
			cout << course[i] << setw(blank) << num[i] << setw(blank) << total[i]
				<< setw(blank) << totaoOverdue[i] << setw(blank) << fines[i] << endl;
		}
		return true;
	}
}

bool printStuWithSameBook(List* list, char* callNum) {
	LibBook book;

	if (list->empty()) {
		InsertBook("book.txt", list); // Insert initial books if the list is empty
	}

	Node* cur;
	int i = 1, stdct = 0;
	cur = list->head;

	bool foundMatch = false; // A flag to check if any student with the given callNum is found

	while (cur != NULL) {
		for (int bookno = 0; bookno < cur->item.totalbook; bookno++) {
			if (strcmp(cur->item.book[bookno].callNum, callNum) == 0) {
				stdct++;
				foundMatch = true; // Set the flag to true if a match is found
			}
		}
		cur = cur->next;
	}

	if (!foundMatch) {
		cout << "Invalid Call Number" << endl; // Print the message if no match was found
		return false;
	}

	cout << "\nThere are " << stdct << " students that borrowed the book with call number " << callNum << " as shown below:" << endl;

	cur = list->head;
	while (cur != NULL) {
		for (int bookno = 0; bookno < cur->item.totalbook; bookno++) {
			if (strcmp(cur->item.book[bookno].callNum, callNum) == 0) {
				cout << "\nSTUDENT " << i << endl;
				cout << "\nName: " << cur->item.name;
				cout << "\nId: " << cur->item.id;
				cout << "\nCourse: " << cur->item.course;
				cout << "\nPhone No: " << cur->item.phone_no;
				cout << "\nBorrow Date: ";
				cout << cur->item.book[bookno].borrow.day << "/" << cur->item.book[bookno].borrow.month;
				cout << "/" << cur->item.book[bookno].borrow.year;
				cout << "\nDue Date: ";
				cout << cur->item.book[bookno].due.day << "/" << cur->item.book[bookno].due.month;
				cout << "/" << cur->item.book[bookno].due.year;
				cout << endl;
				i++;
			}
		}
		cur = cur->next;
	}

	cout << "\n****************************************************\n";
	return true;
}


bool displayWarnedStudent(List* list, List* type1, List* type2) {
	LibBook book;
	if (list->empty())
		InsertBook("book.txt", list);

	Node* cur;
	cur = list->head;

	while (cur != NULL) {
		int dueOver10 = 0, totalOverdue = 0, totalOverdueFine = 0;

		for (int i = 0; i < cur->item.totalbook; i++) {
			int diff = rdn(2020, 3, 29) - rdn(cur->item.book[i].due.year, cur->item.book[i].due.month, cur->item.book[i].due.day);

			if (diff >= 10) {
				dueOver10++;
			}

			if (diff > 0) {
				totalOverdue++;
				totalOverdueFine += cur->item.book[i].fine;
			}
		}

		if (dueOver10 >= 2) {
			type1->insert(cur->item);
		}

		if (totalOverdue == cur->item.totalbook && totalOverdueFine > 50) {
			type2->insert(cur->item);
		}

		cur = cur->next;
	}

	// Display students from list type1
	Node* war;
	war = type1->head;
	cout << "Type 1 - Students with more than 2 books overdue for >= 10 days:" << endl;
	while (war != NULL) {
		war->item.print(cout);
		cout << "\n****************************************************\n";
		war = war->next;
	}

	// Display students from list type2
	Node* war2;
	war2 = type2->head;
	cout << "Type 2 - Students with total fine > RM50 and all books overdue:" << endl;
	while (war2 != NULL) {
		war2->item.print(cout);
		cout << "\n****************************************************\n";
		war2 = war2->next;
	}

	if (list->empty())
		return false;

	return true;
}


int rdn(int y, int m, int d) {
	if (m < 3)
		y--, m += 12;
	return 365 * y + y / 4 - y / 100 + y / 400 + (153 * m - 457) / 5 + d - 306;
}


int menu() {                                          //main menu

	int choice;

	cout << "|-----------------------|" << endl;
	cout << "|      Main Menu        |" << endl;
	cout << "|-----------------------|" << endl;
	cout << "| 1. Read file          |" << endl;
	cout << "| 2. Delete record      |" << endl;
	cout << "| 3. Search student     |" << endl;
	cout << "| 4. Insert book        |" << endl;
	cout << "| 5. Display output     |" << endl;
	cout << "| 6. Compute and Display|" << endl;
	cout << "|    Statistics         |" << endl;
	cout << "| 7. Student with Same  |" << endl;
	cout << "|    Book               |" << endl;
	cout << "| 8. Display Warned     |" << endl;
	cout << "|    Student            |" << endl;
	cout << "| 9. Exit               |" << endl;
	cout << "|-----------------------|" << endl;

	cout << "Enter your choice: ";                    //prompt user to enter their choice
	cin >> choice;


	return choice;
}

