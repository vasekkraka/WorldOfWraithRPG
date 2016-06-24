#ifndef HSERVERCONN
#define HSERVERCONN

#include "HWOWKlient.h"

#define STATE_NOT_DEFINED 0
#define STATE_ERROR -1
#define STATE_CONNECTING 1
#define STATE_AUTH 2
#define STATE_CHARACTER_LIST 3

#define STATE_RECIEVING 4
#define STATE_SENDING 5

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
		s32 getState();
		void ServerConnector::setState(s32 newState);

	private:
		WowKlient::Core::GameState * gState;
		s32 state;
		boost::mutex mutexState;
	};
}


#endif