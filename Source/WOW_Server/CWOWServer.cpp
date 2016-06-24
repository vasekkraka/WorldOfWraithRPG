/*
|--------------------------------------------------------------------|
|                                                                    |
|    Václav Macura   (c) 2015      World of Wraith - Story Begin     |
|                                                                    |
|    ===> CWOWServer.cpp - hlavní bod programu                       |
|                                                                    |
|--------------------------------------------------------------------|
*/


#include "HWOWServer.h"

using namespace std;
using namespace WowServer;
using boost::asio::ip::tcp;


int main()
{	
	printServerInfo();

	ServerInstance * localServer = new ServerInstance();

	localServer->initializeInstance();

	boost::asio::io_service io_service;

	tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), 1234));
	for (;;)
	{
		tcp::socket sock(io_service);
		a.accept(sock);
		printf("Accepted :-) \n");

		boost::asio::write(sock, boost::asio::buffer("ahoj\n", 4));

		for (;;)
		{
			char data[1024];

			boost::system::error_code error;
			size_t length = sock.read_some(boost::asio::buffer(data), error);
			if (error == boost::asio::error::eof)
				break; // Connection closed cleanly by peer.
			else if (error)
				throw boost::system::system_error(error); // Some other error.

			printf("%c", data[0]);

			//boost::asio::write(sock, boost::asio::buffer("joj", 3));
		}

	}

	system("PAUSE");



}