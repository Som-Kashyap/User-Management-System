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

	cout << "---User Management System---" << endl;
	cout << "1. Login" << endl;
	cout << "2. Signup" << endl;
	cout << "3. Exit" << endl;
	cout << "Enter your choice: ";

	while (!(cin >> choice)) {
		cout << "Invalid gender. Please enter a valid gender." << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	if (choice == 1)state = State::Login;
	else if (choice == 2) state = State::Signup;
	else if (choice == 3) state = State::Exit;
}

void editUser(connection& conn, int& id) {

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
	cout << "-----------------------------------------------" << endl;
	cout << "New Details: " << endl;
	conn.getData("SELECT * FROM users WHERE user_id = " + std::to_string(id));

}

void deleteUser(connection& conn, int choice, int& id , State& state) {


	char deleteChoice;
	cout << "-----------------------------------------------" << endl;

	cout << "Are you sure you want to delete your account? (y/n): ";

	while (!(cin >> deleteChoice) || (deleteChoice != 'y' && deleteChoice != 'n')) {
		cout << "Invalid input. Please enter 'y' or 'n': ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	if (deleteChoice == 'y') {
		conn.execute("DELETE FROM users WHERE user_id = " + std::to_string(id));
		cout << "User with ID: " << id << " has been deleted." << endl;
	}
	else menu(conn,state);
}

void userLogin(connection& conn, int choice , State& state , Session& session ) {

	int id;
	string email;
	string password;

	cout << "-----------------------------------------------" << std::endl;
	cout << "LOGIN" << endl;
	cout << "Enter user_id...";

	while (!(cin >> id)) {
		cout << "Invalid input. Please enter a valid integer for ID: ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	session.currUserId = id;

	cout << "Enter email...";
	cin >> email;
	
	cout << "Enter password...";
	cin >> password;

	bool ok = conn.checkEmail(id, email, password);

	if (!ok)
	{
		cout << "Invalid email or password!\n";
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

	cout << "-----------------------------------------------" << std::endl;
	cout << "Welcome " << name << endl;
	cout << "1) View Details" << endl;
	cout << "2) Edit Details" << endl;
	cout << "3) Delete Account" << endl;
	cout << "Enter choice..." << endl;

	while (!(cin >> userChoice) || (userChoice <= 0 || userChoice >= 3)) {
		cout << "Invalid input. Please enter a valid integer for ID: ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	switch (userChoice) {

	case 1: {
		conn.getData("SELECT * FROM users WHERE user_id = " + to_string(session.currUserId));
		break;
	}
	case 2: {
		editUser(conn , session.currUserId);
		break;
	}
	case 3: {
		deleteUser(conn, choice , session.currUserId, state);
		break;
	}
	}
}

void signup(connection& conn , int &choice , State& state , Session& session)
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

void exitProgram() {
	cout << "Program terminated..." << endl;
}

void logic(int &choice , connection& conn , State& state , Session& session) {


	if (state == State::Menu) menu(conn , state);
	else if (state == State::Login) userLogin(conn, choice, state , session);
	else if (state == State::User) user(conn, choice, state, session);

}

int main()
{
	State state = State::Menu;
	Session session;

	connection conn("tcp://127.0.0.1:3306", "root", "password");

	int choice;
	int currentUserid = -1;

	while (state != State::Exit)
	{
		logic(choice, conn, state, session);
	}


	return 0;
}