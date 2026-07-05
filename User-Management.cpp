#include<iostream>
#include "Database.h"

using namespace std;

enum class state {
	Menu,
	AddUser,
	ViewUsers,
	SearchUser,
	Exit
};

class user {

private:

	string username;
	int user_id;
	string password;
	string email;
	string gender;

public:

	void setDetails(string name, int id, string password, string email)
	{
		this->username = name;
		this->user_id = id;
		this->password = password;
		this->email = email;
	}

	
};

void menu(int &choice) {

	cout << "---User Management System---" << endl;
	cout << "1. Add User" << endl;
	cout << "2. View Users" << endl;
	cout << "3. Search User" << endl;
	cout << "4. Edit User" << endl;
	cout << "5. Exit" << endl;
	cout << "Enter your choice: ";
	cin >> choice;

}

void addUser(connection& conn , int &choice)
{
	string name;
	int id;
	string password;
	string email;
	string gender;

	cout << "-----------------------------------------------" << std::endl;
	cout << "Enter your Name... ";
	cin >> name;

	cout << "Got name\n";

	cout << "Enter your ID... ";

	while (!(cin >> id)) {
		cout << "Invalid input. Please enter a valid integer for ID: ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	cout << "Got id\n";

	cout << "Create Password... ";
	cin >> password;
	cout << "Got password\n";

	cout << "Enter your Email... ";
	cin >> email;
	cout << "Got email\n";

	cout << "Enter your Gender(male/female/other)... ";
	cin >> gender;

	for ( int i = 0; i < gender.length(); i++)
	{

	    if (!isalpha(gender[i])) {
			cout << "Invalid gender. Please enter a valid gender." << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
	cout << "Got gender\n";


	conn.execute("INSERT INTO users (name, user_id, password, email, gender) VALUES ('" + name + "', " + std::to_string(id) + ", '" + password + "', '" + email + "', '" + gender + "')");

}

void viewUsers(connection& conn) {

	try {
		conn.getData("SELECT * FROM users");
	}
	catch (sql::SQLException& e)
	{
		std::cout << "SQL Error: " << e.what() << '\n';
		std::cout << "Error Code: " << e.getErrorCode() << '\n';
		std::cout << "SQLState: " << e.getSQLState() << '\n';
	}
}

void searchUser(connection& conn) {

	int searchChoice;

	cout << "-----------------------------------------------" << std::endl;
	cout << "Search User: " << endl;
	cout << "1) Search by Name" << endl;
	cout << "2) Search by ID" << endl;
	cout << "3) Search by Gender" << endl;
	cout << "Enter your choice: ";
	cin >> searchChoice;

	switch (searchChoice) {

	case 1: {
		string name;
		cout << "Enter Name to search: ";
		cin >> name;
		cout << "-----------------------------------------------" << std::endl;
		cout << "Search result....." << endl;
		conn.getData("SELECT * FROM users WHERE name = '" + name + "'");
		cout << "-----------------------------------------------" << std::endl;
		break;
	}

	case 2: {
		int id;
		cout << "Enter ID to search: ";
		cin >> id;
		cout << "-----------------------------------------------" << std::endl;
		cout << "Search result....." << endl;
		conn.getData("SELECT * FROM users WHERE user_id = " + std::to_string(id));
		break;
	}
	case 3: {
		string gender;
		cout << "Enter gender to search: ";
		cin >> gender;
		cout << "-----------------------------------------------" << std::endl;
		cout << "Search result....." << endl;
		conn.getData("SELECT * FROM users WHERE gender = '" + gender + "'");
		break;
	}

	}
}

void editUser(connection& conn) {

	int id;

	cout << "Enter the ID of the user you want to edit: ";
	while (!(cin >> id)) {
		cout << "Invalid input. Please enter a valid integer for ID: ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	int editChoice;

	cout << "-----------------------------------------------" << std::endl;
	cout << "Edit User: " << endl;
	cout << "1) Edit Name" << endl;
	cout << "2) Edit Password" << endl;
	cout << "3) Edit Email" << endl;
	cout << "4) Edit Gender" << endl;

	cout << "Enter your choice: ";

	while (!(cin >> editChoice)) {
		cout << "Invalid input. Please enter a valid integer for choice: ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	switch (editChoice) {

	case 1: {

		string name;
		cout << "Enter new name...";
		cin >> name;
		conn.execute("UPDATE users SET name = '" + name + "' WHERE user_id = " + std::to_string(id));
		break;
	}

	case 2: {
		string password;
		cout << "Enter new password...";
		cin >> password;
		conn.execute("UPDATE users SET password = '" + password + "' WHERE user_id = " + std::to_string(id)); 
		break;
	}

	case 3: {
		string email;
		cout << "Enter new email...";
		cin >> email;
		conn.execute("UPDATE users SET email = '" + email + "' WHERE user_id = " + std::to_string(id));
		break;
	}

	case 4: {
		string gender;
		cout << "Enter gender...";
		cin >> gender;
		conn.execute("UPDATE users SET gender = '" + gender + "' WHERE user_id = " + std::to_string(id));
		break;
	}

	}
	std::cout << "-----------------------------------------------" << std::endl;
	cout << "New Details: " << endl;
	conn.getData("SELECT * FROM users WHERE user_id = " + std::to_string(id));

}

void exitProgram() {
	cout << "Program terminated..." << endl;
}

void logic(int &choice , connection& conn) {

	switch (choice) {

	case 1:addUser(conn , choice);
		   break;

	case 2: viewUsers(conn);
		   break;

	case 3: searchUser(conn);
		   break;

	case 4: editUser(conn);
		break;
		
	case 5: exitProgram();
		break;

	}
}

int main()
{

	connection conn("tcp://127.0.0.1:3306", "root", "password");

	int choice;


		while (1) {
			menu(choice);

			if (choice == 5) {
				exitProgram();
				break;
			}
			logic(choice, conn);
		}

	return 0;
}