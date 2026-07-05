#include<iostream>
#include "Database.h"

using namespace std;

enum class state {
	Menu,
	AddUser,
	ViewUsers,
	Exit
};

class user {

private:

	string username;
	int user_id;
	string password;
	string email;

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

	cout << "1. Add User" << endl;
	cout << "2. View Users" << endl;
	cout << "3. Exit" << endl;
	cout << "Enter your choice: ";
	cin >> choice;

}

void addUser(connection& conn)
{
	string name;
	int id;
	string password;
	string email;

	cout << "Enter your Name... ";
	cin >> name;
	cout << "Got name\n";

	cout << "Enter your ID... ";
	cin >> id;
	cout << "Got id\n";

	cout << "Create Password... ";
	cin >> password;
	cout << "Got password\n";

	cout << "Enter your Email... ";
	cin >> email;
	cout << "Got email\n";

	conn.execute("INSERT INTO users (name, user_id, password, email) VALUES ('" + name + "', " + std::to_string(id) + ", '" + password + "', '" + email + "')");
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

void exitProgram() {
	cout << "Program terminated..." << endl;
}

void logic(int &choice , connection& conn) {

	switch (choice) {

	case 1:addUser(conn);
		   break;

	case 2: viewUsers(conn);
		   break;

	case 3: exitProgram();
		break;

	}
}

int main()
{

	connection conn("tcp://127.0.0.1:3306", "root", "password");

	int choice;

	menu(choice);

	logic(choice, conn);

	return 0;
}