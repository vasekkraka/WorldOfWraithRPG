#include "HWOWKlient.h"

using namespace irr;

namespace WowKlient
{
	LoginManager::~LoginManager()
	{
	}

	LoginManager::LoginManager(WowKlient::Core::GameState * gameS)
	{
		gState = gameS;
	}

	void LoginManager::loginPrompt()
	{
		irr::gui::IGUIEnvironment* guienv = gState->irrDevice->getGUIEnvironment();
		irr::video::IVideoDriver * driver = gState->irrDevice->getVideoDriver();

		irr::core::dimension2d<s32> center;
		center.set((gState->gConf->resolution.Width + 0.0) / 2, (gState->gConf->resolution.Height + 0.0) / 2);

		irr::gui::IGUIImage * backGround = guienv->addImage(irr::core::rect<irr::s32>(0, 0, gState->gConf->resolution.Width, gState->gConf->resolution.Height));
		irr::video::ITexture * backImage;

		if ((gState->gConf->resolution.Width + 0.0) / gState->gConf->resolution.Height > 1.4)
		{
			backImage = driver->getTexture(L"..\\..\\..\\Data\\img\\login_screen\\background_wide.png");
		}
		else
		{
			backImage = driver->getTexture("..\\..\\..\\Data\\img\\login_screen\\background_normal.png");
		}
		backGround->setImage(backImage);
		backGround->setScaleImage(true);

		irr::gui::IGUIImage * loginBorder = guienv->addImage(irr::core::rect<irr::s32>(center.Width - (loginBoxWidth / 2), center.Height - (loginBoxHeight / 2), (loginBoxWidth / 2) + center.Width, center.Height + (loginBoxHeight / 2)));
		loginBorder->setImage(driver->getTexture(L"..\\..\\..\\Data\\img\\login_screen\\login_border.png"));



		irr::gui::IGUIImage * rain = guienv->addImage(core::rect<s32>(0, 0, gState->gConf->resolution.Width, gState->gConf->resolution.Height));
		rain->setScaleImage(true);
		rain->setImage(driver->getTexture(L"..\\..\\..\\Data\\img\\login_screen\\rain.png"));


		irr::gui::IGUIImage * cloud = guienv->addImage(core::rect<s32>(0, 0, 512, 256));
		cloud->setImage(driver->getTexture(L"..\\..\\..\\Data\\img\\login_screen\\cloud.png"));

		gui::IGUIEditBox * editboxUserName = guienv->addEditBox(L"Jméno", core::rect<s32>(loginNameLeft, loginNameTop, loginNameLeft + loginNameWidth, loginNameTop + loginNameHeight ), true, loginBorder); // vycentrovano 
		gui::IGUIEditBox * editboxUserPass = guienv->addEditBox(L"Heslo", core::rect<s32>(loginPassLeft, loginPassTop, loginPassLeft + loginPassWidth, loginPassTop + loginPassHeight), true, loginBorder); // vycentrovano 
		editboxUserPass->setPasswordBox(true, L'*');

		editboxUserName->getActiveFont()->setKerningHeight(20);

		
		int f = 0;
		while (gState->irrDevice->run())
		{
			f++;
			if ((f % 5) == 0)
			{
				cloud->move(core::vector2d<s32>(1, 0));
			}
			rain->move(core::vector2d<s32>(0, 2));
			
			driver->beginScene(true, true, SColor(255, 0, 0, 0)); // zacnu, s cernym pozadim
			guienv->drawAll();
			driver->endScene();
		}
	}
}