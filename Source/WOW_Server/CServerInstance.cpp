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

ServerInstance::ServerInstance()
{

}

ServerInstance::~ServerInstance()
{

}

void ServerInstance::initializeInstance()
{
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

	system("PAUSE");

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
	}
}
