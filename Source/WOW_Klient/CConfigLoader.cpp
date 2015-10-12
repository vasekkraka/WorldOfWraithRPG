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

				printf("\nNacitani graficke konfigurace:\n-------------------------------------------\n");

				while (xml->read())
				{
					switch (xml->getNodeType())
					{
					case irr::io::EXN_ELEMENT:
						printf("\n<%s>\n", xml->getNodeName());

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

								if ((irr::core::string<char>)xml->getAttributeName(i) == "stencilbuffer")
								{
									gConf->stencilBuffer = xml->getAttributeValueAsInt(i);
								}

								printf("\t%s: \"%i\"", xml->getAttributeName(i), xml->getAttributeValueAsInt(i));
							}
						}
						if ((irr::core::string<char>)xml->getNodeName() == "driver")
						{
							for (int i = 0; i < xml->getAttributeCount(); i++)
							{
								if ((irr::core::string<char>)xml->getAttributeName(i) == "type")
								{
									switch (xml->getAttributeValueAsInt(i))
									{
									case 0:
										gConf->driverType = irr::video::EDT_NULL;
										break;
									case 1:
										gConf->driverType = irr::video::EDT_OPENGL;
										break;
									case 2:
										gConf->driverType = irr::video::EDT_DIRECT3D9;
										break;
									}
								}

								if ((irr::core::string<char>)xml->getAttributeName(i) == "bits")
								{
									gConf->bits = xml->getAttributeValueAsInt(i);
								}

								if ((irr::core::string<char>)xml->getAttributeName(i) == "vsync")
								{
									gConf->vSync = xml->getAttributeValueAsInt(i);
								}

								printf("\t%s: \"%i\"", xml->getAttributeName(i), xml->getAttributeValueAsInt(i));
							}
						}
							break;

						case irr::io::EXN_TEXT:
							printf("%s\n", xml->getNodeData());
							break;

						case irr::io::EXN_ELEMENT_END :
							printf("\n<\\%s>\n", xml->getNodeName());
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