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
	}
}
