#ifndef HConfigLoader
#define HConfigLoader

#include "HWOWKlient.h"

namespace WowKlient
{
	namespace Core
	{
		namespace Configuration
		{
			/*
			struktura uchovavajici konfiguraci grafickeho enginu
			*/
			struct graphicConfiguration
			{
				irr::video::E_DRIVER_TYPE driverType;
				irr::core::dimension2du resolution;
				irr::u32 bits;
				bool fullScreen;
				bool vSync;
				bool stencilBuffer;
			};

			/*
			struktura uchovavajici konfiguraci zvukového enginu
			*/
			struct soundConfiguration
			{
				bool enableSound;
				irr::u32 volume;

			};

			/*
			nacte konfiguraci grafickeho rozhrani
			file -> cesta k XML souboru
			*/
			graphicConfiguration * loadGraphicConfiguration(irr::core::string<char> file);

			/*
			nacte konfiguraci zvukoveho rozhrani
			file -> cesta k XML souboru
			*/
			soundConfiguration * loadSoundConfiguration(irr::core::string<char> file);
		}
	}
}

#endif