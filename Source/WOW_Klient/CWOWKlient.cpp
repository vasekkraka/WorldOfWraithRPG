#include <stdio.h>
#include "HWOWKlient.h"
#include "RealisticWater.h"

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
		//irrklangDevice->play2D("..\\..\\..\\Data\\music\\intro\\intro.ogg", false);

		playIntroImage(irrlichtDevice, "..\\..\\..\\Data\\img\\intro\\gapilogo.png", 7);
		playIntroImage(irrlichtDevice, "..\\..\\..\\Data\\img\\intro\\irrlogo.png", 7);
		playIntroImage(irrlichtDevice, "..\\..\\..\\Data\\img\\intro\\present.png", 10);
		playIntroImage(irrlichtDevice, "..\\..\\..\\Data\\img\\intro\\wowlogo.png", 15);
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
		irr::gui::IGUIEnvironment* guienv = gState.irrDevice->getGUIEnvironment();
		irr::video::IVideoDriver * driver = gState.irrDevice->getVideoDriver();
		irr::scene::ISceneManager * smgr = gState.irrDevice->getSceneManager();

		smgr->clear();
		guienv->clear();

		ICameraSceneNode * usercamera = smgr->addCameraSceneNodeFPS();
		usercamera->setPosition(vector3df(0,0,0));


		IMeshSceneNode * teren = smgr->addMeshSceneNode(smgr->getMesh("..\\..\\..\\Data\\test_scene\\sw_teren.obj"), 0, 0, vector3df(0, 0, 0), vector3df(0, 0, 0), vector3df(1, 1, 1));

		teren->setMaterialFlag(E_MATERIAL_FLAG::EMF_LIGHTING, false);

		teren->setMaterialFlag(E_MATERIAL_FLAG::EMF_ANTI_ALIASING, true);
		teren->setMaterialFlag(E_MATERIAL_FLAG::EMF_ANISOTROPIC_FILTER, true);
		teren->setMaterialFlag(E_MATERIAL_FLAG::EMF_USE_MIP_MAPS, true);

		//teren->setMaterialFlag(E_MATERIAL_FLAG::EMF_WIREFRAME, true);

		teren->setMaterialType(E_MATERIAL_TYPE::EMT_SOLID);
		teren->setMaterialFlag(E_MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);

		for (int i = 0; i < teren->getMaterialCount(); i++)
		{
			if (teren->getMaterial(i).getTexture(0))
			{
				printf("%i: \t %s\n", i, teren->getMaterial(i).getTexture(0)->getName());
			}
			if (i == 3 || i == 4 || i == 5)
			{
				teren->getMaterial(i).MaterialType = E_MATERIAL_TYPE::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
				teren->getMaterial(i).setFlag(E_MATERIAL_FLAG::EMF_BACK_FACE_CULLING, false);
			}
			
		}

		scene::ISceneNode* skybox = smgr->addSkyBoxSceneNode(
			driver->getTexture("../../../media/irrlicht2_up.jpg"),
			driver->getTexture("../../../media/irrlicht2_dn.jpg"),
			driver->getTexture("../../../media/irrlicht2_lf.jpg"),
			driver->getTexture("../../../media/irrlicht2_rt.jpg"),
			driver->getTexture("../../../media/irrlicht2_ft.jpg"),
			driver->getTexture("../../../media/irrlicht2_bk.jpg"));

		//RealisticWaterSceneNode * water = new RealisticWaterSceneNode(smgr, 1024, 1024, "../../../Data");

		//water->setPosition(vector3df(0, 20, 0));

		//water->setParent(smgr->getRootSceneNode());

		//smgr->addCubeSceneNode(10.0F, NULL, NULL, vector3df(0, 10, 0));

		IAnimatedMeshSceneNode * qeen_node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("..\\..\\..\\Data\\test_scene\\bloodqueen.obj"), 0, 0, vector3df(0, 30, 0));

		IAnimatedMesh * qeen_mesh = qeen_node->getMesh();

		for (int i = 0; i < qeen_mesh->getMeshBufferCount(); i++)
		{
			printf("Vertexu v %i-tem bufferu: %i\n", i, qeen_mesh->getMeshBuffer(i)->getVertexCount());
		}

		SAnimatedMesh * minimal_mesh = new SAnimatedMesh(0, E_ANIMATED_MESH_TYPE::EAMT_SKINNED);

		SMesh * minimal_static_mesh = new SMesh();

		minimal_static_mesh->addMeshBuffer(qeen_mesh->getMeshBuffer(1));

		minimal_mesh->addMesh(minimal_static_mesh);

		printf("Buffer count: %i", minimal_mesh->getMeshBufferCount());

		IMeshSceneNode * minimal_node = smgr->addMeshSceneNode(minimal_static_mesh, 0, 0, vector3df(30, 30, 0));


		while (gState.irrDevice->run())
		{
			u32 oldtime = gState.irrDevice->getTimer()->getTime();
			int fps = driver->getFPS();

			core::stringw str = L"World of Wraith - Story Begin TEST SCENE[";
			str += driver->getName();
			str += "] FPS:";
			str += fps;

			gState.irrDevice->setWindowCaption(str.c_str());

			driver->beginScene(true, true, SColor(255, 128, 128, 128)); // zacnu, s cernym pozadim

			guienv->drawAll();
			smgr->drawAll();

			driver->endScene();
			u32 newtime = gState.irrDevice->getTimer()->getTime();
			if ((newtime - oldtime) < 1000 / 60)
			{
				gState.irrDevice->sleep((1000 / 60) - (newtime - oldtime));
			}
		}
	};

	void WoWGameKlient::gameRun()
	{
		SCursorSprite * cursorSprite = new SCursorSprite();

		ITexture * tex = irrlichtDevice->getVideoDriver()->getTexture("..\\..\\..\\Data\\cursors\\cur116.png");

		cursorSprite->SpriteBank = irrlichtDevice->getGUIEnvironment()->addEmptySpriteBank("cursors");

		cursorSprite->SpriteId = cursorSprite->SpriteBank->addTextureAsSprite(tex);

		irrlichtDevice->getCursorControl()->changeIcon(ECURSOR_ICON::ECI_NORMAL, *cursorSprite);

		while (1)
		{
			loginScreen->loginPrompt();
			exit(0);
		}
	};
}

