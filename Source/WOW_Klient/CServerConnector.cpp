#include "HWOWKlient.h"

using namespace irr;

namespace WowKlient
{
	ServerConnector::ServerConnector()
	{
	
	}

	ServerConnector::ServerConnector(WowKlient::Core::GameState * data)
	{
		gState = data;
	}

	bool ServerConnector::tryConnectLogin()
	{
		// tady se navaze spojeni socketem k serveru a pokusi se provest autentizace
		return true;
	}
}