#ifndef HCore
#define HCore

#include "HWOWKlient.h"

namespace WowKlient
{
	namespace Core
	{
		struct accInformation
		{
			irr::core::string<char> userName;
			irr::core::string<char> userPassword;
			irr::u32 charactersCount;
		};

		struct GameState
		{
			Configuration::graphicConfiguration * gConf;
			Configuration::soundConfiguration * sConf;
			irr::IrrlichtDevice * irrDevice;
			irrklang::ISoundEngine * klangDevice;
			accInformation accInfo;
		};

		bool engineIsRunning(WowKlient::Core::GameState * gstate);

		/*
		provede inicializaci grafického enginu 
		*/
		irr::IrrlichtDevice * initializeGraphicEngine(Configuration::graphicConfiguration * gConf);

		/*
		provede inicializaci zvukového enginu
		*/
		irrklang::ISoundEngine * initializeSoundEngine();
	}
}

#endif