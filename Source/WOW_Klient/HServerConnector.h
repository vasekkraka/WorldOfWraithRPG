#ifndef HSERVERCONN
#define HSERVERCONN

#include "HWOWKlient.h"

namespace WowKlient
{

	class ServerConnector
	{
	public:
		ServerConnector();
		ServerConnector(WowKlient::Core::GameState * data);
		~ServerConnector();
		bool tryConnectLogin();
		bool retireveAccData();

	private:
		WowKlient::Core::GameState * gState;
	};
}


#endif