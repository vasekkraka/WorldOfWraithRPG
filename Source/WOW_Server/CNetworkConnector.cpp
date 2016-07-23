#include "HNetworkConnector.h"

using namespace boost::asio::ip;
using boost::asio::ip::udp;

void posli(int cislo)
{
	boost::asio::io_service io_service;
	udp::resolver resolver(io_service);
	udp::resolver::query query(udp::v4(), "gapi.cz", "1234");
	udp::endpoint receiver_endpoint = *resolver.resolve(query);
	udp::socket socket(io_service);
	socket.open(udp::v4());
	
	

	std::string retezec = "slovo min nez 30cet\n";

	int velikost = retezec.length();

	std::vector<boost::asio::const_buffer> buf;

	buf.push_back(boost::asio::buffer((void *)&velikost, sizeof(velikost)));
	buf.push_back(boost::asio::buffer(retezec));

	boost::asio::const_buffer a = boost::asio::buffer(retezec);

	velikost = -456;

	int vel_tcp = htonl(velikost);
	int vel_norm = ntohl(vel_tcp);

	const std::string* b = boost::asio::buffer_cast<const std::string *>(a);
	

	socket.send_to(buf, receiver_endpoint);
	boost::array<char, 128> recv_buf;
	udp::endpoint sender_endpoint;
	size_t len = socket.receive_from(
		boost::asio::buffer(recv_buf), sender_endpoint);

	std::cout.write(recv_buf.data(), len);
}