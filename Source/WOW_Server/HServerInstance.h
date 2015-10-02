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

namespace WowServer
{
	class ServerInstance
	{
	public:
		ServerInstance();
		~ServerInstance();

		void initializeInstance();

	private:
		sql::Driver *sqlDriver = NULL;
		sql::Connection *sqlConnection = NULL;

		bool connectDatabase();
		bool loadStaticData();


	};
}

#endif