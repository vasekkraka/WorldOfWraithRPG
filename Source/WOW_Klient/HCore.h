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
		};

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