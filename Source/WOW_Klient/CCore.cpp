#include "HWOWKlient.h"

namespace WowKlient
{
	namespace Core
	{

		irr::IrrlichtDevice * initializeGraphicEngine(Configuration::graphicConfiguration * gConf)
		{
			return createDevice(EDT_DIRECT3D9, gConf->resolution, gConf->bits, gConf->fullScreen, gConf->stencilBuffer, gConf->vSync);
		}

		irrklang::ISoundEngine * initializeSoundEngine()
		{
			return irrklang::createIrrKlangDevice();
		}

		bool engineIsRunning(WowKlient::Core::GameState * gstate)
		{
			gstate->irrDevice->getTimer()->stop();
			bool ret = gstate->irrDevice->run();
			gstate->irrDevice->getTimer()->stop();
			return ret;
		}

	}
}
