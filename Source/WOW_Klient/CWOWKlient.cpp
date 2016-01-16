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

		RealisticWaterSceneNode * water = new RealisticWaterSceneNode(smgr, 1024, 1024, "../../../Data");

		water->setPosition(vector3df(0, 20, 0));

		water->setParent(smgr->getRootSceneNode());

		
		IAnimatedMesh * dwarf_mesh = smgr->getMesh("..\\..\\..\\Data\\test_scene\\dwarf.x");
		ISkinnedMesh * skinned = (ISkinnedMesh *)dwarf_mesh;

		for (int i = 0; i < skinned->getJointCount(); i++)
		{
			printf("Joint %i name: %s\n", i, skinned->getAllJoints()[i]->Name);
		}

		IAnimatedMeshSceneNode * dwarf_node = smgr->addAnimatedMeshSceneNode(dwarf_mesh, 0, 0, vector3df(0, 30, 0));

		ISkinnedMesh * clone = smgr->createSkinnedMesh();

		//IMeshSceneNode * static_node = smgr->addMeshSceneNode(dwarf_mesh, 0, 0, vector3df(60, 30, 0));

		SSkinMeshBuffer * buf = clone->addMeshBuffer();

		printf("V novem meshi je %i bufferu\n", clone->getMeshBufferCount());

		for (int i = 0; i < ((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getVertexCount(); i++)
		{
			//printf("X:%i Y:%i Z:%i U:%i V:%i\n", ((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getVertex(i)->Pos.X, ((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getVertex(i)->Pos.Y, ((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getVertex(i)->Pos.Z, ((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getVertex(i)->TCoords.X, ((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getVertex(i)->TCoords.Y);
			S3DVertex * v = new S3DVertex(((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getVertex(i)->Pos.X, ((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getVertex(i)->Pos.Y, ((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getVertex(i)->Pos.Z, ((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getVertex(i)->Normal.X, ((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getVertex(i)->Normal.Y, ((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getVertex(i)->Normal.Z, ((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getVertex(i)->Color, ((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getVertex(i)->TCoords.X, ((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getVertex(i)->TCoords.Y);
			buf->Vertices_Standard.push_back(*v);
		}

		for (int i = 0; i < ((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getIndexCount(); i++)
		{
			buf->Indices.push_back(u16(((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getIndices()[i]));
		}

		

		//ISkinnedMesh::SJoint * p = clone->addJoint();
		//*p = *skinned->getAllJoints()[0];
		//p->Weights.clear();
		//p->Children.clear();

		//for (int i = 1; i < skinned->getJointCount(); i++)
		//{
		//	ISkinnedMesh::SJoint * j = clone->addJoint(p);
		//	*j = *skinned->getAllJoints()[i];
		//	j->Weights.clear();
		//	j->Children.clear();
		//}

		for (int i = 0; i < skinned->getJointCount(); i++)
		{
			ISkinnedMesh::SJoint * joint = clone->addJoint();
			*joint = *skinned->getAllJoints()[i];
			joint->Weights.clear();
			joint->Children.clear();

			if (skinned->getAllJoints()[i]->Children.size() > 0)
			{
				for (int j = 0; j < skinned->getAllJoints()[i]->Children.size(); j++)
				{
					ISkinnedMesh::SJoint * child_joint = clone->addJoint(joint);
					*child_joint = *skinned->getAllJoints()[i]->Children[j];
					child_joint->Children.clear();
					child_joint->Weights.clear();
				}
			}
		}

		printf("Jointu ve starem: %i\n", skinned->getJointCount());
		printf("Jointu v novem: %i\n", clone->getJointCount());
		

		
		dwarf_node->setDebugDataVisible(irr::scene::E_DEBUG_SCENE_TYPE::EDS_SKELETON);
		
		dwarf_node->setAnimationSpeed(5);

		//clone->useAnimationFrom(skinned);
		
		//buf->Material.setTexture(0, (((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getMaterial().getTexture(0)));
		//buf->Material.Lighting = false;

		buf->Material = ((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getMaterial();

		clone->finalize();

		printf("Trianglu v novem meshi : %i\n", buf->getVertexCount());

		printf("Indicii v novem meshi : %i V puvodnim: %i\n", buf->getIndexCount(), dwarf_mesh->getMeshBuffer(0)->getIndexCount());

		IAnimatedMeshSceneNode * clone_node = smgr->addAnimatedMeshSceneNode(clone, 0, 0, vector3df(45, 30, 0));

		clone_node->setDebugDataVisible(irr::scene::E_DEBUG_SCENE_TYPE::EDS_SKELETON);

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

