#include "HWOWKlient.h"

using namespace irr;

using boost::asio::ip::udp;

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

			udp::resolver resolver(io_service);
			udp::resolver::query query("gapi.cz", "110");

			udp::socket s(io_service);

			//boost::asio::connect(s, resolver.resolve(query));

			char * request = "Text z BOOST TCP!!\n";

			size_t request_length = strlen(request);

			s.open(udp::v4());

			s.send_to(boost::asio::buffer(request, request_length), *resolver.resolve(query));
			

			//boost::asio::write(s, boost::asio::buffer(request, request_length));  // tcp only

			char recvData[100];

			udp::endpoint sender_endp;

			size_t recvLength = s.receive_from(boost::asio::buffer(recvData, 100), sender_endp);

			//size_t recvLength = s.receive(boost::asio::buffer(recvData, 100));

			std::cout.write(recvData, recvLength);

			s.close();

		}
		catch (std::exception& e)
		{
			std::cerr << "Síový problem: " << e.what() << "\n";
			setState(STATE_ERROR);
			return false;
		}
		
		return true;
	}
}