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
				printf("%s", file);
				getchar();
				nullDev->getFileSystem()->createXMLReaderUTF8(file);

				return gConf;
			}

			soundConfiguration * loadSoundConfiguration(irr::core::string<char> file)
			{
				soundConfiguration * sConf = new soundConfiguration;
				printf("%s", file);
				getchar();
				//naplneni konfigurace dle nacteni xml souboru
				return sConf;
			}
		}
	}
}