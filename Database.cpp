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

connection::~connection() {
	delete conn;
};