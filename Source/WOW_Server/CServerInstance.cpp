/*
|--------------------------------------------------------------------|
|                                                                    |
|    Václav Macura   (c) 2015      World of Wraith - Story Begin     |
|                                                                    |
|    ===> HServerInstance.h - kod tridy instance serveru             |
|                                                                    |
|--------------------------------------------------------------------|
*/

#include "HWOWServer.h"

using namespace WowServer;
using namespace boost::asio::ip;

ServerInstance::ServerInstance()
{

}

ServerInstance::~ServerInstance()
{

}

void ServerInstance::initializeInstance()
{
	IPService = new boost::asio::io_service();
	acceptorTCP = new tcp::acceptor(*IPService, tcp::endpoint(tcp::v4(), 1234));
	
	mainSock = new ip::tcp::socket(*IPService);

	printf("Pripojovani k DB : ");
	if (connectDatabase() == true)
	{
		printf("Ok\n");
	}

	printf("Nacitani statickych dat z DB: ");
	if (loadStaticData())
	{
		printf("Ok\n");
	}
	else
	{
		printf("Error\n");
	}
}

bool ServerInstance::loadStaticData()
{
	return true;
}

bool ServerInstance::connectDatabase()
{
	try
	{
		sqlDriver = sql::mysql::get_driver_instance();
		sqlConnection = sqlDriver->connect("tcp://127.0.0.1:3306", "wow_gapi", "wS10uxFN");
		if (sqlConnection->isValid())
		{
			sqlConnection->setSchema("wow_gapi");
			return true;
		}
		else
		{
			return false;
		}
	}
	catch (sql::SQLException &e) {
		e.getSQLState();
		std::cout << "\n" << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode() << ")" << std::endl;
		return false;
	}
}

bool ServerInstance::login(sql::SQLString *login, sql::SQLString *password)
{
	bool ret = false;

	try
	{

		sql::PreparedStatement * stmt = sqlConnection->prepareStatement("select * from account where login = ? and password = md5(?)");
		sql::ResultSet * resSet;

		stmt->setString(1, *login);
		stmt->setString(2, *password);
		resSet = stmt->executeQuery();

		if (resSet->rowsCount() == 1)
		{
			ret = true;
		}

		delete stmt;
		delete resSet;

	}
	catch (sql::SQLException &e) {
		e.getSQLState();
		std::cout << "\n" << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode() << ")" << std::endl;
		return false;
	}

	return ret;
}
