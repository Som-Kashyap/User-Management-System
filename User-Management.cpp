#include<iostream>
#include "Database.h"

using namespace std;

enum class State {

	Menu,
	Login,
	Signup,
	User,
	Exit
};

class Session {

public:

	bool isLoggedIn = false;
	int currUserId = -1;

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

void menu(connection& conn , State& state) {

	int choice = 0;

	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "---User Management System---" << endl;
	std::cout << "1. Login" << endl;
	std::cout << "2. Signup" << endl;
	std::cout << "3. Exit" << endl;
	std::cout << "Enter your choice: ";

	while (!(cin >> choice)) {
		std::cout << "Invalid gender. Please enter a valid gender." << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	if (choice == 1)state = State::Login;
	else if (choice == 2) state = State::Signup;
	else if (choice == 3) state = State::Exit;
}

void editUser(connection& conn, int& id, State& state) {

	int editChoice;
	bool isEdited = false;

	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "Edit User: " << endl;
	std::cout << "1) Edit Name" << endl;
	std::cout << "2) Edit Password" << endl;
	std::cout << "3) Edit Email" << endl;
	std::cout << "4) Edit Gender" << endl;
	std::cout << "5) Return " << endl;

	std::cout << "Enter your choice: ";

	while (!(cin >> editChoice)) {
		std::cout << "Invalid input. Please enter a valid integer for choice: ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	if (editChoice >= 1 && editChoice <= 4) isEdited = true;

	switch (editChoice) {

	case 1: {

		string name;
		std::cout << "Enter new name...";
		cin >> name;
		conn.execute("UPDATE users SET name = '" + name + "' WHERE user_id = " + std::to_string(id));
		break;
	}

	case 2: {
		string password;
		std::cout << "Enter new password...";
		cin >> password;
		conn.execute("UPDATE users SET password = '" + password + "' WHERE user_id = " + std::to_string(id));
		break;
	}

	case 3: {
		string email;
		std::cout << "Enter new email...";
		cin >> email;
		conn.execute("UPDATE users SET email = '" + email + "' WHERE user_id = " + std::to_string(id));
		break;
	}

	case 4: {
		string gender;
		std::cout << "Enter gender...";
		cin >> gender;
		conn.execute("UPDATE users SET gender = '" + gender + "' WHERE user_id = " + std::to_string(id));
		break;
	}
	case 5: {
		state = State::User;
		break;
	}
	}
	std::cout << "-----------------------------------------------" << endl;
	if (isEdited) {
		std::cout << "New Details: " << endl;
		conn.getData("SELECT * FROM users WHERE user_id = " + std::to_string(id));
	}
	
}

void deleteUser(connection& conn, int choice, int& id , State& state) {


	char deleteChoice;
	std::cout << "-----------------------------------------------" << endl;

	std::cout << "Are you sure you want to delete your account? (y/n): ";

	while (!(cin >> deleteChoice) || (deleteChoice != 'y' && deleteChoice != 'n')) {
		std::cout << "Invalid input. Please enter 'y' or 'n': ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	if (deleteChoice == 'y') {
		conn.execute("DELETE FROM users WHERE user_id = " + std::to_string(id));
		std::cout << "User with ID: " << id << " has been deleted." << endl;
		state = State::Menu;
	}
	else menu(conn,state);
}

void userLogin(connection& conn, int choice , State& state , Session& session ) {

	int id;
	string email;
	string password;

	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "LOGIN" << endl;
	std::cout << "Enter user_id...";

	while (!(cin >> id)) {
		std::cout << "Invalid input. Please enter a valid integer for ID: ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	session.currUserId = id;

	std::cout << "Enter email...";
	cin >> email;
	
	std::cout << "Enter password...";
	cin >> password;

	bool ok = conn.checkEmail(id, email, password);

	if (!ok)
	{
		std::cout << "Invalid email or password!\n";
		state = State::Login;
	}
	else
	{
		state = State::User;
	}
	
}

void user(connection& conn, int choice ,State& state, Session& session) {

	int userChoice;

	string name = conn.getName(session.currUserId);

	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "Welcome " << name << endl;
	std::cout << "1) View Details" << endl;
	std::cout << "2) Edit Details" << endl;
	std::cout << "3) Delete Account" << endl;
	std::cout << "4) Logout " << endl;
	std::cout << "Enter choice...";

	while (!(cin >> userChoice) || (userChoice <= 0 || userChoice > 4)) {
		std::cout << "Invalid input. Please enter a valid choice: ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	switch (userChoice) {

	case 1: {
		std::cout << "-----------------------------------------------" << std::endl;
		std::cout << "Account Details: " << endl;
		conn.getData("SELECT * FROM users WHERE user_id = " + to_string(session.currUserId));
		break;
	}
	case 2: {
		editUser(conn , session.currUserId , state);
		break;
	}
	case 3: {
		deleteUser(conn, choice , session.currUserId, state);
		break;
	}
	case 4: {
		std::cout << "Logging out......." << endl;
		state = State::Menu;
		break;
	}
	}
}

bool signup(connection& conn , int &choice , State& state , Session& session)
{
	try {
		string name;
		int id;
		string password;
		string email;
		string gender;


		std::cout << "-----------------------------------------------" << std::endl;
		std::cout << "Enter your Name... ";
		cin >> name;


		std::cout << "Enter your ID... ";

		while (!(cin >> id)) {
			std::cout << "Invalid input. Please enter a valid integer for ID: ";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}

		std::cout << "Create Password... ";
		cin >> password;

		std::cout << "Enter your Email... ";
		cin >> email;

		std::cout << "Enter your Gender(male/female/other)... ";
		cin >> gender;

		for (int i = 0; i < gender.length(); i++)
		{

			if (!isalpha(gender[i])) {
				std::cout << "Invalid gender. Please enter a valid gender." << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
		}

		conn.execute("INSERT INTO users (name, user_id, password, email, gender) VALUES ('" + name + "', " + std::to_string(id) + ", '" + password + "', '" + email + "', '" + gender + "')");

		return true;
	}

	catch (sql::SQLException& e)
	{
		std::cout << "SQL Error: " << e.what() << '\n';
		std::cout << "Error Code: " << e.getErrorCode() << '\n';
		std::cout << "SQLState: " << e.getSQLState() << '\n';

		return false;
	}
	
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

	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "Search User: " << endl;
	std::cout << "1) Search by Name" << endl;
	std::cout << "2) Search by ID" << endl;
	std::cout << "3) Search by Gender" << endl;
	std::cout << "Enter your choice: ";
	cin >> searchChoice;

	switch (searchChoice) {

	case 1: {
		string name;
		std::cout << "Enter Name to search: ";
		cin >> name;
		std::cout << "-----------------------------------------------" << std::endl;
		std::cout << "Search result....." << endl;
		conn.getData("SELECT * FROM users WHERE name = '" + name + "'");
		std::cout << "-----------------------------------------------" << std::endl;
		break;
	}

	case 2: {
		int id;
		std::cout << "Enter ID to search: ";
		cin >> id;
		std::cout << "-----------------------------------------------" << std::endl;
		std::cout << "Search result....." << endl;
		conn.getData("SELECT * FROM users WHERE user_id = " + std::to_string(id));
		break;
	}
	case 3: {
		string gender;
		std::cout << "Enter gender to search: ";
		cin >> gender;
		std::cout << "-----------------------------------------------" << std::endl;
		std::cout << "Search result....." << endl;
		conn.getData("SELECT * FROM users WHERE gender = '" + gender + "'");
		break;
	}

	}
}

void exitProgram() {
	std::cout << "Program terminated..." << endl;
}

void logic(int &choice , connection& conn , State& state , Session& session) {


	if (state == State::Menu) menu(conn, state);
	else if (state == State::Login) userLogin(conn, choice, state, session);
	else if (state == State::User) user(conn, choice, state, session);
	else if (state == State::Signup) {

		bool res = signup(conn, choice, state, session);

		if (res) state = State::Login;
		else state = State::Signup;
	}
	else if (state == State::Exit) exitProgram();
}

int main()
{
	State state = State::Menu;
	Session session;

	connection conn("tcp://127.0.0.1:3306", "root", "***");

	int choice;
	int currentUserid = -1;

	while (state != State::Exit)
	{
		logic(choice, conn, state, session);
	}


	return 0;
}