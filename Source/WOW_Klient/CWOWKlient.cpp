#include <stdio.h>
#include "HWOWKlient.h"

using namespace WowKlient;
using namespace WowKlient::Core;
using namespace WowKlient::Core::Intro;
using namespace WowKlient::Core::Configuration;

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace irrklang;

namespace WowKlient
{
	WoWGameKlient::WoWGameKlient()
	{
		irrlichtDevice = NULL;
		irrklangDevice = NULL;
	};

	WoWGameKlient::~WoWGameKlient()
	{
	};

	void WoWGameKlient::playIntro()
	{
		irrlichtDevice->getCursorControl()->setVisible(false);

		//irrklangDevice->play2D("..\\..\\..\\Data\\music\\intro\\intro.ogg", false);

		playIntroImage(irrlichtDevice, PATH_PREFIX "/img/intro/gapilogo.png", 7);
		playIntroImage(irrlichtDevice, PATH_PREFIX "/img/intro/irrlogo.png", 7);
		playIntroImage(irrlichtDevice, PATH_PREFIX "/img/intro/present.png", 10);
		playIntroImage(irrlichtDevice, PATH_PREFIX "/img/intro/wowlogo.png", 15);

		irrlichtDevice->getCursorControl()->setVisible(true);
	}

	bool WoWGameKlient::initialize()
	{
		gState.gConf = loadGraphicConfiguration(configFileGraphic);
		gState.sConf = loadSoundConfiguration(configFileSound);

		irrlichtDevice = initializeGraphicEngine(gState.gConf);
		gState.irrDevice = irrlichtDevice;

		irrklangDevice = initializeSoundEngine();

		loginScreen = new LoginManager(&gState);

		return true;
	}


	void WoWGameKlient::runTestScene()
	{
		printf("Which test scene execute?\n\n");

		printf
			(
			"a: realistic water scene node\n"
			"b: skinned mesh copy part\n"
			"c: Gilneas city scene\n"
			"d: BootyBay snene\n"
			"\n"
			);
		char chose = getchar();

		initialize();

		switch (chose)
		{
		case 'a':
			WaterRealisticScene(gState);
			break;
		case 'b':
			SkinnedMeshCopyScene(gState);
			break;
		case 'c':
			GilneasCityScene(gState);
			break;
		case 'd':
			BootyBayScene(gState);
			break;
		default:
			break;
		}

	};

	void WoWGameKlient::gameRun()
	{
		SCursorSprite * cursorSprite = new SCursorSprite();

		ITexture * tex = irrlichtDevice->getVideoDriver()->getTexture("..\\..\\..\\Data\\cursors\\cur116.png");

		cursorSprite->SpriteBank = irrlichtDevice->getGUIEnvironment()->addEmptySpriteBank("cursors");

		cursorSprite->SpriteId = cursorSprite->SpriteBank->addTextureAsSprite(tex);

		irrlichtDevice->getCursorControl()->changeIcon(ECURSOR_ICON::ECI_NORMAL, *cursorSprite);

		ServerConnector * sc = new ServerConnector(&gState);

		while (engineIsRunning(&gState))
		{
			loginScreen->loginPrompt();
			irrlichtDevice->getSceneManager()->clear();

			ILogger * log = gState.irrDevice->getLogger();

			gState.accInfo.userName = loginScreen->getUserName();
			gState.accInfo.userPassword = loginScreen->getUserPassword();

			char pole[100];

			log->log(loginScreen->getUserName().c_str(), ELOG_LEVEL::ELL_INFORMATION);

			log->log(gState.accInfo.userName.c_str(), ELOG_LEVEL::ELL_INFORMATION);
			log->log(gState.accInfo.userPassword.c_str(), ELOG_LEVEL::ELL_INFORMATION);

			//sc->tryConnectLogin();
			
		}
		
		exit(0);

	};
}

