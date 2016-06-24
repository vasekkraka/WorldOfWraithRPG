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
		provede inicializaci grafick�ho enginu 
		*/
		irr::IrrlichtDevice * initializeGraphicEngine(Configuration::graphicConfiguration * gConf);

		/*
		provede inicializaci zvukov�ho enginu
		*/
		irrklang::ISoundEngine * initializeSoundEngine();
	}
}

#endif