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

	void WoWGameKlient::gameRun()
	{
		gState.gConf = loadGraphicConfiguration("");
		gState.sConf = loadSoundConfiguration("");

		irrlichtDevice = createDevice(EDT_OPENGL, dimension2d<u32>(1366, 768), 32, false, false, false);

		irrklangDevice = initializeSoundEngine();

		SCursorSprite * cursorSprite = new SCursorSprite();

		ITexture * tex = irrlichtDevice->getVideoDriver()->getTexture("..\\..\\..\\Data\\cursors\\cur116.png");

		cursorSprite->SpriteBank = irrlichtDevice->getGUIEnvironment()->addEmptySpriteBank("cursors");

		cursorSprite->SpriteId = cursorSprite->SpriteBank->addTextureAsSprite(tex);

		irrlichtDevice->getCursorControl()->changeIcon(ECURSOR_ICON::ECI_NORMAL, *cursorSprite);

		irrklangDevice->play2D("..\\..\\..\\Data\\music\\intro\\intro.ogg", false);

		playIntroImage(irrlichtDevice, "..\\..\\..\\Data\\img\\intro\\gapilogo.png", 7);
		playIntroImage(irrlichtDevice, "..\\..\\..\\Data\\img\\intro\\irrlogo.png", 7);
		playIntroImage(irrlichtDevice, "..\\..\\..\\Data\\img\\intro\\present.png", 10);
		playIntroImage(irrlichtDevice, "..\\..\\..\\Data\\img\\intro\\wowlogo.png", 15);
	};
}

