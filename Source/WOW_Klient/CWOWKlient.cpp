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

	void writeChilds(ISkinnedMesh::SJoint * joint, int hloubka)
	{
		for (int i = 0; i < joint->Children.size(); i++)
		{
			for (int j = 0; j < hloubka; j++)
			{
				printf(" -> ");
			}
			printf(" %s\n", joint->Name);
			if (joint->Children[i]->Children.size() > 0)
			{
				writeChilds(joint->Children[i], hloubka + 1);
			}
		}
	}

	void parseJointParents(ISkinnedMesh * mesh, ISkinnedMesh::SJoint * Sjoint, ISkinnedMesh::SJoint * Pjoint)
	{
		ISkinnedMesh::SJoint * Njoint = mesh->addJoint(Pjoint);
		*Njoint = *Sjoint;
		//mesh->addJoint(Njoint);
		Njoint->Weights.clear();
		Njoint->Children.clear();
		for (int i = 0; i < Sjoint->Children.size(); i++)
		{
			parseJointParents(mesh, Sjoint->Children[i], Njoint);
		}
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

		

		ISkinnedMesh * clone = smgr->createSkinnedMesh();

		SSkinMeshBuffer * buf = clone->addMeshBuffer();

		buf->recalculateBoundingBox();
		buf->setDirty();

		printf("V novem meshi je %i bufferu\n", clone->getMeshBufferCount());

		for (int i = 0; i < ((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getVertexCount(); i++)
		{
			S3DVertex * v = new S3DVertex(((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getVertex(i)->Pos.X, ((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getVertex(i)->Pos.Y, ((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getVertex(i)->Pos.Z, ((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getVertex(i)->Normal.X, ((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getVertex(i)->Normal.Y, ((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getVertex(i)->Normal.Z, ((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getVertex(i)->Color, ((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getVertex(i)->TCoords.X, ((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getVertex(i)->TCoords.Y);
			buf->Vertices_Standard.push_back(*v);

		}

		skinned->finalize();

		IAnimatedMeshSceneNode * dwarf_node = smgr->addAnimatedMeshSceneNode(dwarf_mesh, 0, 0, vector3df(0, 30, 0));

		for (int i = 0; i < ((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getIndexCount(); i++)
		{
			buf->Indices.push_back(u16(((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getIndices()[i]));
		}

		buf->Material = ((SSkinMeshBuffer*)dwarf_mesh->getMeshBuffer(0))->getMaterial();

		printf("Copying joints... \n");

		parseJointParents(clone, skinned->getAllJoints()[0], 0);

		printf("Jointu ve starem: %i\n", skinned->getJointCount());
		printf("Jointu v novem: %i\n", clone->getJointCount());
		
		clone->setDirty();
		
		dwarf_node->setDebugDataVisible(irr::scene::E_DEBUG_SCENE_TYPE::EDS_SKELETON);
		
		dwarf_node->setAnimationSpeed(10);



		for (int i = 0; i < skinned->getJointCount(); i++)
		{
			printf("Cycle %i:\n", i);
			ISkinnedMesh::SJoint * joint;

			joint = clone->getAllJoints()[clone->getJointNumber(skinned->getAllJoints()[i]->Name.c_str())];

			printf("Joint name: %s\n", joint->Name.c_str());

			for (int j = 0; j < skinned->getAllJoints()[i]->Weights.size(); j++)
			{
				if (skinned->getAllJoints()[i]->Weights[j].buffer_id == 0)
				{
					ISkinnedMesh::SWeight * w = clone->addWeight(joint);
					w->buffer_id = 0;
					w->strength = skinned->getAllJoints()[i]->Weights[j].strength;
					w->vertex_id = skinned->getAllJoints()[i]->Weights[j].vertex_id;
				}
			}

		}


		printf("Copy done... \n");


		clone->finalize();

		clone->setDirty();
		//getchar();
		IAnimatedMeshSceneNode * clone_node = smgr->addAnimatedMeshSceneNode(clone, 0, 0, vector3df(30, 30, 0));

		

		clone_node->setAnimationSpeed(5);

		clone_node->setMaterialFlag(E_MATERIAL_FLAG::EMF_LIGHTING, false);

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

