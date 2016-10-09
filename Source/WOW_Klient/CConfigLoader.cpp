
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

				printf("\nNacitani graficke konfigurace:\n-------------------------------------\n");

				while (xml->read())
				{
					switch (xml->getNodeType())
					{
					case irr::io::EXN_ELEMENT:
						printf("\n<%s>\n", xml->getNodeName());

						if ((irr::core::string<char>)xml->getNodeName() == "resolution")
						{
							for (u32 i = 0; i < xml->getAttributeCount(); i++)
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
									int attr = xml->getAttributeValueAsInt(i);
									gConf->fullScreen = attr == 1 ? true : false;
								}

								if ((irr::core::string<char>)xml->getAttributeName(i) == "stencilbuffer")
								{
									int attr = xml->getAttributeValueAsInt(i);
									gConf->stencilBuffer = attr == 1 ? true : false;
								}

								printf("\t%s: \"%i\"", xml->getAttributeName(i), xml->getAttributeValueAsInt(i));
							}
						}
						if ((irr::core::string<char>)xml->getNodeName() == "driver")
						{
							for (u32 i = 0; i < xml->getAttributeCount(); i++)
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
									int attr = xml->getAttributeValueAsInt(i);
									gConf->vSync = attr == 1 ? true : false;
								}

								printf("\t%s: \"%i\"", xml->getAttributeName(i), xml->getAttributeValueAsInt(i));
							}
						}

						if ((irr::core::string<char>)xml->getNodeName() == "user_conf")
						{
							for (u32 i = 0; i < xml->getAttributeCount(); i++)
							{
								if ((irr::core::string<char>)xml->getAttributeName(i) == "req_fps")
								{
									int reqFps = xml->getAttributeValueAsInt(i);
									gConf->reqFps = reqFps;
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
				return gConf;
			}

			soundConfiguration * loadSoundConfiguration(irr::core::string<char> file)
			{
				soundConfiguration * sConf = new soundConfiguration;
				printf("%s", file);
				//naplneni konfigurace dle nacteni xml souboru
				return sConf;
			}
		}
	}
}