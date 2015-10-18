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
#include <boost\asio.hpp>

using namespace std;
using namespace WowServer;
using boost::asio::ip::tcp;

class thr
{
public:
	thr();
	~thr();
	void thread_alfa();
	void thread_beta(int a);
	int get_cislo();

private:
	boost::mutex mut;
	int cislo;
};

thr::thr()
{
}

thr::~thr()
{
}

int thr::get_cislo()
{
	return cislo;
}

void thr::thread_alfa()
{
	//mut.lock();
	//for (int i = 0; i < 100; i++)
	//{
	//	printf("%i\t", i);
	//}
	//printf("\nBlbina\n");
	//mut.unlock();
	//

	mut.lock();
	for (int i = 0; i < 10000; i++)
	{
		printf("+");
	}
	mut.unlock();
	cislo = 123456789;
}

void thr::thread_beta(int a)
{
	//mut.lock();
	//for (int i = 100; i < 200; i++)
	//{
	//	printf("%i\t", i);
	//}
	//printf("\nBlbina\n");
	//mut.unlock();

	

	mut.lock();
	for (int i = 0; i < 10000; i++)
	{
		printf("-");
	}
	cislo = 11299984;
	mut.unlock();
	printf("%i\n", a);
}


int main()
{

	thr vlakno;
	//boost::thread alfa{ &thr::thread_alfa, &vlakno };
	//boost::thread beta{ &thr::thread_beta, &vlakno, 99 };
	//alfa.join();
	//beta.join();
	
	printServerInfo();

	//printf("\n\n ---- %i\n\n", vlakno.get_cislo());

	//boost::thread alfa{ thread_alfa }; 
	//boost::thread beta{ thread_beta };

	//alfa.join();
	//beta.join();

	//ServerInstance * localServer = new ServerInstance();

	//localServer->initializeInstance();




	system("PAUSE");



}