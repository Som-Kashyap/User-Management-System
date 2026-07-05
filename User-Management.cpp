#include<iostream>
#include "Database.h"

using namespace std;

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

int main() {

	user u1;
	connection conn("tcp://127.0.0.1:3306", "root", "password");

	u1.setDetails("JohnDoe", 1, "password123", "joh123@gmail.com");

	//conn.execute("INSERT INTO users (name, user_id, password, email) VALUES ('JohnDoe', 1, 'password123', 'joh123@gmail.com')");
	//conn.getData("SELECT * FROM users");

	user u2;
	u2.setDetails("JaneDoe", 2, "password456", "jane456@gmail.com");

	//conn.execute("INSERT INTO users (name, user_id, password, email) VALUES ('JaneDoe', 2, 'password456', 'jane456@gmail.com')");
	//conn.getData("SELECT * FROM users");

	user u3;
	//u3.setDetails("AliceSmith", 3, "password789", "alice123@gmail.com");
	//conn.execute("INSERT INTO users (name, user_id, password, email) VALUES ('AliceSmith', 3, 'password789', 'alice123@gmail.com')");

	conn.getData("SELECT * FROM users");

	return 0;
}