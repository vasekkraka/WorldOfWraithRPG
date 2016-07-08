/*
|--------------------------------------------------------------------|
|                                                                    |
|    Václav Macura   (c) 2015      World of Wraith - Story Begin     |
|                                                                    |
|    ===> HServerInstance.h - hlavicka instance serveru              |
|                                                                    |
|--------------------------------------------------------------------|
*/

#ifndef HServerInstance
#define HServerInstance

#include "HWOWServer.h"

using namespace boost::asio;

namespace WowServer
{
	class ServerInstance
	{
	public:
		ServerInstance();
		~ServerInstance();

		void initializeInstance();
		bool login(sql::SQLString *login, sql::SQLString *password);
		ip::tcp::acceptor *acceptorTCP = NULL;
		boost::asio::io_service  *IPService;

	private:
		sql::Driver *sqlDriver = NULL;
		sql::Connection *sqlConnection = NULL;
		ip::tcp::socket *mainSock = NULL;

		bool connectDatabase();
		bool loadStaticData();


	};
}

#endif