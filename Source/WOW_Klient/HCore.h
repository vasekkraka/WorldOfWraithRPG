#ifndef HCore
#define HCore

#include "HWOWKlient.h"

namespace WowKlient
{
	namespace Core
	{
		struct GameState
		{
			Configuration::graphicConfiguration * gConf;
			Configuration::soundConfiguration * sConf;
			irr::IrrlichtDevice * irrDevice;
			irrklang::ISoundEngine * klangDevice;
		};

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