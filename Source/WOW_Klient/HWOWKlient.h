#ifndef HWOWKlient
#define HWOWKlient

#include "HBuildConfig.h"
#include <irrlicht.h>
#include <irrKlang.h>
#include "HConfigLoader.h"
#include "HCore.h"
#include "HMeshFunctions.h"
#include "HIntro.h"
#include "HServerConnector.h"
#include "HLoginManager.h"
#include "HTestScenes.h"



using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace irrklang;

#define PATH_PREFIX "..\\..\\..\\Data"

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
			void runMapEditor();
			

		private:
			Core::GameState gState;
			IrrlichtDevice * irrlichtDevice;
			ISoundEngine * irrklangDevice;
			LoginManager * loginScreen;
	};

}

#endif