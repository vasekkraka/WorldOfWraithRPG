#ifndef HIntro
#define HIntro

#include "HWOWKlient.h"

namespace WowKlient
{
	namespace Core
	{
		namespace Intro
		{
			void playIntroImage(irr::IrrlichtDevice *dev, irr::core::string<char> imagePath, irr::u32 speed);
		}
	}
}

#endif