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
		irr::scene::ISceneManager * smgr = gState->irrDevice->getSceneManager();

		irr::core::dimension2d<s32> center;
		center.set((s32)(gState->gConf->resolution.Width + 0.0) / 2, (s32)(gState->gConf->resolution.Height + 0.0) / 2);

		irr::gui::IGUIImage * backGround = guienv->addImage(irr::core::rect<irr::s32>(0, 0, gState->gConf->resolution.Width, gState->gConf->resolution.Height));
		irr::video::ITexture * backImage;

		core::vector3df campFirePosition;

		if ((gState->gConf->resolution.Width + 0.0) / gState->gConf->resolution.Height > 1.4)
		{
			backImage = driver->getTexture(L"..\\..\\..\\Data\\img\\login_screen\\background_wide.png");
			campFirePosition.set(campfirePositionWide);
		}
		else
		{
			backImage = driver->getTexture("..\\..\\..\\Data\\img\\login_screen\\background_normal.png");
			campFirePosition.set(campfirePositionNormal);
		}
		backGround->setImage(backImage);
		backGround->setScaleImage(true);

		irr::gui::IGUIImage * loginBorder = guienv->addImage(irr::core::rect<irr::s32>(center.Width - (loginBoxWidth / 2), center.Height - (loginBoxHeight / 2), (loginBoxWidth / 2) + center.Width, center.Height + (loginBoxHeight / 2)));
		loginBorder->setImage(driver->getTexture(L"..\\..\\..\\Data\\img\\login_screen\\login_border.png"));


		irr::gui::IGUIImage * rain = guienv->addImage(core::rect<s32>(0, 0, 512, 1080));
		rain->setImage(driver->getTexture(L"..\\..\\..\\Data\\img\\login_screen\\rain.png"));
		rain->setScaleImage(true);

		irr::gui::IGUIImage * rain1 = guienv->addImage(core::rect<s32>(512, -100, 1024, 1412));
		rain1->setImage(driver->getTexture(L"..\\..\\..\\Data\\img\\login_screen\\rain.png"));
		rain1->setScaleImage(true);
		//rain1->

		irr::gui::IGUIImage * rain2 = guienv->addImage(core::rect<s32>(1024, 100, 1536, 1612));
		rain2->setImage(driver->getTexture(L"..\\..\\..\\Data\\img\\login_screen\\rain.png"));
		rain2->setScaleImage(true);


		irr::gui::IGUIImage * cloud = guienv->addImage(core::rect<s32>(0, 0, 512, 256));
		cloud->setImage(driver->getTexture(L"..\\..\\..\\Data\\img\\login_screen\\cloud.png"));

		irr::gui::IGUIImage * cloud2 = guienv->addImage(core::rect<s32>(-450, 100, 62, 356));
		cloud2->setImage(driver->getTexture(L"..\\..\\..\\Data\\img\\login_screen\\cloud.png"));

		irr::gui::IGUIImage * cloud3 = guienv->addImage(core::rect<s32>(700, 50, 1312, 306));
		cloud3->setImage(driver->getTexture(L"..\\..\\..\\Data\\img\\login_screen\\cloud.png"));

		gui::IGUIEditBox * editboxUserName = guienv->addEditBox(L"Jméno", core::rect<s32>(loginNameLeft, loginNameTop, loginNameLeft + loginNameWidth, loginNameTop + loginNameHeight ), true, loginBorder); // vycentrovano 
		gui::IGUIEditBox * editboxUserPass = guienv->addEditBox(L"Heslo", core::rect<s32>(loginPassLeft, loginPassTop, loginPassLeft + loginPassWidth, loginPassTop + loginPassHeight), true, loginBorder); // vycentrovano 
		editboxUserPass->setPasswordBox(true, L'*');

		guienv->getSkin()->setColor(gui::EGUI_DEFAULT_COLOR::EGDC_3D_FACE, video::SColor(255, 0, 0, 0));

		gui::IGUIButton * loginButton = guienv->addButton(core::rect<s32>(loginButtonLeft, loginButtonTop, loginButtonLeft + loginButtonWidth, loginButtonTop + loginButtonHeight), loginBorder, 0, L"", L"Prihlasi vas do hry");
		loginButton->setImage(driver->getTexture(L"..\\..\\..\\Data\\img\\login_screen\\login.png"));
		loginButton->setScaleImage(true);
		loginButton->setUseAlphaChannel(true);
		loginButton->setDrawBorder(false);
		
		scene::IParticleSystemSceneNode* ps =
			smgr->addParticleSystemSceneNode(false);

		scene::IParticleEmitter* em = ps->createBoxEmitter(
			core::aabbox3d<f32>(10, 0, -1, 15, 0, 7), // emitter size
			core::vector3df(0.0f, 0.02f, 0.0f),   // initial direction
			10, 50,                             // emit rate
			video::SColor(0, 255, 255, 255),       // darkest color
			video::SColor(0, 255, 255, 255),       // brightest color
			500, 700, 0,                         // min and max age, angle
			core::dimension2df(5.f, 5.f),         // min size
			core::dimension2df(7.f, 7.f));        // max size

		ps->setEmitter(em); // this grabs the emitter
		em->drop(); // so we can drop it here without deleting it

		scene::IParticleAffector* paf = ps->createFadeOutParticleAffector();

		ps->addAffector(paf); // same goes for the affector
		paf->drop();

		ps->setPosition(campFirePosition);
		ps->setScale(core::vector3df(1, 1, 1));
		ps->setMaterialFlag(video::EMF_LIGHTING, false);
		ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
		ps->setMaterialTexture(0, driver->getTexture("..\\..\\..\\Data\\img\\login_screen\\fire.bmp"));
		ps->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);

		scene::ICameraSceneNode* camera = smgr->addCameraSceneNode();
		camera->setPosition(core::vector3df(400, 300, 0));

		camera->setTarget(core::vector3df(400,300,-150));

		rain->setEnabled(false);
		rain1->setEnabled(false);
		rain2->setEnabled(false);

		int f = 0;
		while (gState->irrDevice->run())
		{
			u32 oldtime = gState->irrDevice->getTimer()->getTime();
			f++;
			if ((f % 5) == 0)
			{
				cloud->move(core::vector2d<s32>(1, 0));
				cloud2->move(core::vector2d<s32>(1, 0));
				cloud3->move(core::vector2d<s32>(1, 0));
			}

			if (cloud3->getAbsolutePosition().UpperLeftCorner.X > (s32)gState->gConf->resolution.Width)
			{
				cloud3->setRelativePosition(core::rect<s32>(0 - 512, 0 , 0, 256));
				core::rect<s32> poloha = cloud3->getAbsolutePosition();
				printf("%i, %i\n", poloha.UpperLeftCorner.X, poloha.UpperLeftCorner.Y);
			}

			rain->move(core::vector2d<s32>(0, 1));
			rain1->move(core::vector2d<s32>(0, 1));
			rain2->move(core::vector2d<s32>(0, 1));
			
			int fps = driver->getFPS();

			core::stringw str = L"World of Wraith - Story Begin[";
			str += driver->getName();
			str += "] FPS:";
			str += fps;

			gState->irrDevice->setWindowCaption(str.c_str());

			driver->beginScene(true, true, SColor(255, 0, 0, 0)); // zacnu, s cernym pozadim
			
			guienv->drawAll();
			smgr->drawAll();
			
			driver->endScene();
			u32 newtime = gState->irrDevice->getTimer()->getTime();
			if ((newtime - oldtime) < 1000 / 60)
			{
				gState->irrDevice->sleep((1000/60) - (newtime - oldtime));
			}
		}
	}
}