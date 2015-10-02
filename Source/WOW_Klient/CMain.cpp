#include "HWOWKlient.h"
#include <Windows.h>

using namespace WowKlient;

#ifdef BUILD_SUBSYSTEM_WINDOWS
int CALLBACK WinMain(_In_ HINSTANCE hInstance,_In_ HINSTANCE hPrevInstance,	_In_ LPSTR lpCmdLine,_In_ int nCmdShow)
#else
int main()
#endif
{

	WoWGameKlient * localGame = new WoWGameKlient();

	localGame->gameRun();

	return 0;
}