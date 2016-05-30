#include "HWOWKlient.h"

namespace WowKlient
{
	namespace Core
	{
		namespace Intro
		{
			// zobrazi plynule animaci s pruhledností, uvodního obrázku
			void playIntroImage(irr::IrrlichtDevice *dev, irr::core::string<char> imagePath, u32 speed)
			{
				IVideoDriver* driver = dev->getVideoDriver();
				ISceneManager* smgr = dev->getSceneManager();
				IGUIEnvironment* guienv = dev->getGUIEnvironment();

				ITexture* image = driver->getTexture(imagePath);

				if (image != 0)
				{

					core::dimension2d<u32> screenSize = driver->getScreenSize();
					core::dimension2d<u32> imageSize(image->getSize().Width, image->getSize().Height);

					u32 start_time = dev->getTimer()->getTime();
					u32 alpha = 0;
					bool up = true;
					bool done = false;
					while (dev->run() && !done)
					{
						driver->beginScene(true, true, SColor(255, 0, 0, 0)); // zacnu, s cernym pozadim

						if (alpha < 255 && up) // z 0% -> 100%
						{
							u32 now_time = dev->getTimer()->getTime();
							if (now_time - start_time > speed)
							{
								alpha++;
								start_time = now_time;
							}
						}
						else // z 100% -> 0%
						{
							up = false;
						}
						if (!up && alpha > 0)
						{
							u32 now_time = dev->getTimer()->getTime();
							if (now_time - start_time > speed)
							{
								alpha--;
								start_time = now_time;
							}
						}
						if (!up && alpha == 0) // konec animace, ukoncit smycku
						{
							done = true;
						}
						// vykresleni obrazku, pouziti vypoctu velikosti a centrování
						driver->draw2DImage(image, position2d<s32>((screenSize.Width - imageSize.Width) / 2, (screenSize.Height - imageSize.Height) / 2), core::rect<s32>(0, 0, imageSize.Width, imageSize.Height), 0, SColor(alpha, 255, 255, 255), true);

						driver->endScene();
					}
					driver->removeTexture(image);
					image = 0;
				}
				else
				{
					printf("!-! :: Chyba nacitani textury %s\n", imagePath);
				}

			}
		}
	}
}
