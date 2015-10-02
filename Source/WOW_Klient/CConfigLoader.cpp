#include "HWOWKlient.h"

namespace WowKlient
{
	namespace Core
	{
		namespace Configuration
		{
			graphicConfiguration * loadGraphicConfiguration(irr::core::string<char> file)
			{
				graphicConfiguration * gConf = new graphicConfiguration;
				irr::IrrlichtDevice *nullDev = irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_NULL);
				nullDev->getFileSystem()->createXMLReaderUTF8(configFileGraphic);

				return gConf;
			}

			soundConfiguration * loadSoundConfiguration(irr::core::string<char> file)
			{
				soundConfiguration * sConf = new soundConfiguration;
				//naplneni konfigurace dle nacteni xml souboru
				return sConf;
			}
		}
	}
}