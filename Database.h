#pragma once
#include<mysql_driver.h>
#include<mysql_connection.h>
#include<cppconn/driver.h>
#include<cppconn/connection.h>
#include<cppconn/statement.h>
#include<cppconn/resultset.h>
#include <cppconn/exception.h>
#include<string>

class connection {

private:
	sql::Driver* driver;
	sql::Connection* conn ;
	sql::Statement* stmt ;
	sql::ResultSet* res ;

public:
	connection(std::string host, std::string user, std::string password);

	void execute(const std::string& query);
	

	void getData(const std::string& query);

	bool checkEmail(const int id , const std::string& email , const std::string& password);

	std::string getName(const int id);

	~connection();
};