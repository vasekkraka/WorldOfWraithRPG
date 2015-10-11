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
				irr::io::IXMLReaderUTF8* xml = nullDev->getFileSystem()->createXMLReaderUTF8(file);

				while (xml->read())
				{
					switch (xml->getNodeType())
					{
					case irr::io::EXN_ELEMENT:
						printf("%s\n", xml->getNodeName());

						if ((irr::core::string<char>)xml->getNodeName() == "resolution")
						{
							for (int i = 0; i < xml->getAttributeCount(); i++)
							{
								if ((irr::core::string<char>)xml->getAttributeName(i) == "width")
								{
									gConf->resolution.Width = xml->getAttributeValueAsInt(i);
								}

								if ((irr::core::string<char>)xml->getAttributeName(i) == "height")
								{
									gConf->resolution.Height = xml->getAttributeValueAsInt(i);
								}

								if ((irr::core::string<char>)xml->getAttributeName(i) == "fullscreen")
								{
									gConf->fullScreen = xml->getAttributeValueAsInt(i);
								}
							}
							printf("\t %i\t%i\n", gConf->resolution.Width, gConf->resolution.Height);
							printf("%i\n", gConf->fullScreen);
						}

						
						break;

						case irr::io::EXN_TEXT:
							printf("%s\n", xml->getNodeData());
						break;
					}
				}

				getchar();

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