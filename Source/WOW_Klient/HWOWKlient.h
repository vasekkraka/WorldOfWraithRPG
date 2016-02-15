#ifndef HWOWKlient
#define HWOWKlient

#include "HBuildConfig.h"
#include <irrlicht.h>
#include <irrKlang.h>
#include "HConfigLoader.h"
#include "HCore.h"
#include "HIntro.h"
#include "HLoginManager.h"
#include "HTestScenes.h"



using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace irrklang;

namespace WowKlient
{
	class WoWGameKlient
	{
		public:
			WoWGameKlient();
			~WoWGameKlient();
			bool initialize();
			void gameRun();
			void playIntro();
			void runTestScene();
			

		private:
			Core::GameState gState;
			IrrlichtDevice * irrlichtDevice;
			ISoundEngine * irrklangDevice;
			LoginManager * loginScreen;
	};

}

#endif