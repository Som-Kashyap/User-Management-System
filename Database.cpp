#include "Database.h"

connection::connection(std::string host, std::string user, std::string password) {

		driver = sql::mysql::get_driver_instance();
		conn = driver->connect(host, user, password);
		conn->setSchema("user_management");
		
		stmt = nullptr;
		res = nullptr;
};

void connection::execute(const std::string& query)
{
	try
	{
		stmt = conn->createStatement();
		stmt->execute(query);

		delete stmt;
	}
	catch (sql::SQLException& e)
	{
		std::cout << "-----------------------------------------------" << std::endl;
		std::cout << "SQL Error: " << e.what() << '\n';
		std::cout << "Error Code: " << e.getErrorCode() << '\n';
		std::cout << "SQLState: " << e.getSQLState() << '\n';
		std::cout << "-----------------------------------------------" << std::endl;
	}
}

void connection::getData(const std::string& query) {

	try {
		stmt = conn->createStatement();
		res = stmt->executeQuery(query);


		while (res->next()) {
			std::cout <<"Name: " + res->getString("name") << " | ";
			std::cout <<"user_id: "  << res->getInt("user_id") << " | ";
			std::cout <<"password: " + res->getString("password") << " | ";
			std::cout <<"Email: " + res->getString("email") << " | ";
			std::cout << "Gender: " + res->getString("gender") << std::endl;
			std::cout << "-----------------------------------------------" << std::endl;
		}

		delete res;
		delete stmt;
	}
	catch (sql::SQLException& e)
	{
		std::cout<< "-----------------------------------------------" << std::endl;
		std::cout << "SQL Error: " << e.what() << '\n';
		std::cout << "Error Code: " << e.getErrorCode() << '\n';
		std::cout << "SQLState: " << e.getSQLState() << '\n';
		std::cout << "-----------------------------------------------" << std::endl;
	}
}

bool connection::checkEmail(const int id,const std::string& email,const std::string& password) {
		
	stmt = conn->createStatement();
	res = stmt->executeQuery("SELECT * FROM users WHERE user_id = " + std::to_string(id));

	if (!res->next()) {
		delete res;
		delete stmt;
		return false;
	}

	std::string actualEmail = res->getString("email");
	std::string actualPassword = res->getString("password");

	if (email == actualEmail && password == actualPassword) {
		delete stmt;
		delete res;
		return true;
	}
	
	else {
		delete stmt;
		delete res;
		return false;
	}


	
}

std::string connection::getName(const int id) {

	stmt = conn->createStatement();
	res = stmt->executeQuery("SELECT * FROM users WHERE user_id = " + std::to_string(id));
	
	if (!res->next()) {
		return NULL;
	}

	std::string name = res->getString("name");

	return name;
}

connection::~connection() {
	delete conn;
};