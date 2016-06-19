#include "HWOWKlient.h"

using namespace irr;

using boost::asio::ip::tcp;

namespace WowKlient
{
	ServerConnector::ServerConnector()
	{
	
	}

	ServerConnector::ServerConnector(WowKlient::Core::GameState * data)
	{
		gState = data;
		state = STATE_NOT_DEFINED;
	}

	s32 ServerConnector::getState()
	{
		s32 localState;
		mutexState.lock();
		localState = state;
		mutexState.unlock();
		return localState;
	}

	void ServerConnector::setState(s32 newState)
	{
		mutexState.lock();
		state = newState;
		mutexState.unlock();
	}

	bool ServerConnector::tryConnectLogin()
	{
		// tady se navaze spojeni socketem k serveru a pokusi se provest autentizace
		setState(STATE_CONNECTING);


		try
		{

			boost::asio::io_service io_service;

			tcp::resolver resolver(io_service);
			tcp::resolver::query query("127.0.0.1", "800");
			//tcp::resolver::iterator iterator = resolver.resolve(query);

			tcp::socket s(io_service);

			boost::asio::connect(s, resolver.resolve(query));

			

			//boost::bind(&boost::asio::io_service::run, &io_service);

			//while (std::cin.getline(line, chat_message::max_body_length + 1))
			//{
			//	using namespace std; // For strlen and memcpy.
			//	chat_message msg;
			//	msg.body_length(strlen(line));
			//	memcpy(msg.body(), line, msg.body_length());
			//	msg.encode_header();
			//}

		}
		catch (std::exception& e)
		{
			std::cerr << "Exception: " << e.what() << "\n";
		}
		
		return true;
	}
}