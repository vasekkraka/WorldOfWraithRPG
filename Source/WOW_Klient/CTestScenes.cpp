
#include "HTestScenes.h"
#include <irrlicht.h>

#include "HWoWCollisionAnimator.h"
#include "HWowLikeCameraAnimator.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace WowKlient::MeshFunctions;

#define req_fps 50

void SkinnedMeshCopyScene(WowKlient::Core::GameState gState)
{
	irr::gui::IGUIEnvironment* guienv = gState.irrDevice->getGUIEnvironment();
	irr::video::IVideoDriver * driver = gState.irrDevice->getVideoDriver();
	irr::scene::ISceneManager * smgr = gState.irrDevice->getSceneManager();

	smgr->clear();
	guienv->clear();

	ICameraSceneNode * usercamera = smgr->addCameraSceneNodeFPS();
	usercamera->setPosition(vector3df(0, 0, 0));

	scene::ISceneNode* skybox = smgr->addSkyBoxSceneNode(
		driver->getTexture(PATH_PREFIX "/IrrTestScene/irrlicht2_up.jpg"),
		driver->getTexture(PATH_PREFIX "/IrrTestScene/irrlicht2_dn.jpg"),
		driver->getTexture(PATH_PREFIX "/IrrTestScene/irrlicht2_lf.jpg"),
		driver->getTexture(PATH_PREFIX "/IrrTestScene/irrlicht2_rt.jpg"),
		driver->getTexture(PATH_PREFIX "/IrrTestScene/irrlicht2_ft.jpg"),
		driver->getTexture(PATH_PREFIX "/IrrTestScene/irrlicht2_bk.jpg"));

	IAnimatedMesh * dwarf_mesh = smgr->getMesh(PATH_PREFIX "/IrrTestScene/dwarf.x");

	int casti[] = {0};

	ISkinnedMesh * skinned = makeMeshFromParts(&gState, dwarf_mesh, 1, casti);

	IAnimatedMeshSceneNode * clone_node = smgr->addAnimatedMeshSceneNode(skinned, 0, 0, vector3df(30, 30, 0));

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

void WaterRealisticScene(WowKlient::Core::GameState gState)
{
	irr::gui::IGUIEnvironment* guienv = gState.irrDevice->getGUIEnvironment();
	irr::video::IVideoDriver * driver = gState.irrDevice->getVideoDriver();
	irr::scene::ISceneManager * smgr = gState.irrDevice->getSceneManager();

	smgr->clear();
	guienv->clear();

	ICameraSceneNode * usercamera = smgr->addCameraSceneNodeFPS();
	usercamera->setPosition(vector3df(0, 0, 0));


	IMeshSceneNode * teren = smgr->addMeshSceneNode(smgr->getMesh("../../../Data/test_scene/sw_teren.obj"), 0, 0, vector3df(0, 0, 0), vector3df(0, 0, 0), vector3df(1, 1, 1));

	teren->setMaterialFlag(E_MATERIAL_FLAG::EMF_LIGHTING, false);

	teren->setMaterialFlag(E_MATERIAL_FLAG::EMF_ANTI_ALIASING, true);
	teren->setMaterialFlag(E_MATERIAL_FLAG::EMF_ANISOTROPIC_FILTER, true);
	teren->setMaterialFlag(E_MATERIAL_FLAG::EMF_USE_MIP_MAPS, true);

	//teren->setMaterialFlag(E_MATERIAL_FLAG::EMF_WIREFRAME, true);

	teren->setMaterialType(E_MATERIAL_TYPE::EMT_SOLID);
	teren->setMaterialFlag(E_MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);

	for (u32 i = 0; i < teren->getMaterialCount(); i++)
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
		driver->getTexture(PATH_PREFIX "/IrrTestScene/irrlicht2_up.jpg"),
		driver->getTexture(PATH_PREFIX "/IrrTestScene/irrlicht2_dn.jpg"),
		driver->getTexture(PATH_PREFIX "/IrrTestScene/irrlicht2_lf.jpg"),
		driver->getTexture(PATH_PREFIX "/IrrTestScene/irrlicht2_rt.jpg"),
		driver->getTexture(PATH_PREFIX "/IrrTestScene/irrlicht2_ft.jpg"),
		driver->getTexture(PATH_PREFIX "/IrrTestScene/irrlicht2_bk.jpg"));

	RealisticWaterSceneNode * water = new RealisticWaterSceneNode(smgr, 1024, 1024, "../../../Data");

	water->setPosition(vector3df(0, 20, 0));

	water->setParent(smgr->getRootSceneNode());
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

void GilneasCityScene(WowKlient::Core::GameState gState)
{
	IrrlichtDevice * dev = gState.irrDevice;
	IVideoDriver* driver = dev->getVideoDriver();
	ISceneManager* smgr = dev->getSceneManager();
	IGUIEnvironment* guienv = dev->getGUIEnvironment();

	driver->setTextureCreationFlag(E_TEXTURE_CREATION_FLAG::ETCF_ALWAYS_32_BIT);

	vector3df a = vector3df(1, 0, 1);
	vector3df b = vector3df(1, 1, 1);

	a.normalize();
	b.normalize();

	f32 c = a.dotProduct(b);

	double uhel_cos = acos(c) * 180.0 / PI;
	double uhel_sin = asin(c) * 180.0 / PI;


	IAnimatedMesh* mesh = smgr->getMesh("../../../data/IrrTestScene/human_stand.x");
	((ISkinnedMesh *)mesh)->finalize();
	
	if (!mesh)
	{
		dev->drop();
		return;
	}
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);

	node->setAnimationSpeed(30);
	

	if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMaterialFlag(EMF_NORMALIZE_NORMALS, false);
		node->setPosition(vector3df(0, 1000, 0));
		node->setRotation(vector3df(0, 45, 0));
		node->setScale(vector3df(60.0f, 60.0f, 60.0f));
	}

	IMeshSceneNode* terrain = smgr->addMeshSceneNode(smgr->getMesh("../../../data/IrrTestScene/gilneas_2.obj"), 0, 0, core::vector3df(0.f, -1000.f, 0.f), core::vector3df(0.f, 0.f, 0.f), core::vector3df(20.0f, 20.0f, 20.0f));
	ITriangleSelector * trg = smgr->createOctreeTriangleSelector(terrain->getMesh(), terrain);


	terrain->setTriangleSelector(trg);
	trg->drop();

	ISceneNodeAnimatorCollisionResponse * colAnim = new irr::scene::CSceneNodeAnimatorWoWCollisionAnimator(smgr, trg, node, core::vector3df(20.0f, 30.0f, 20.0f), core::vector3df(0.0f, -75.0f, 0.0f), core::vector3df(0.0f, -30.0f, 0.0f), 0.05f);

	node->addAnimator(colAnim);
	colAnim->drop();
	terrain->setMaterialFlag(video::EMF_LIGHTING, false);
	terrain->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	terrain->setMaterialFlag(E_MATERIAL_FLAG::EMF_BACK_FACE_CULLING, false);

	//terrain->setDebugDataVisible(E_DEBUG_SCENE_TYPE::EDS_MESH_WIRE_OVERLAY);


	terrain->setMaterialType(E_MATERIAL_TYPE::EMT_SOLID);




	ICameraSceneNode* cam = smgr->addCameraSceneNode(0, vector3df(0, 100, 100), vector3df(0, 0, 0), 1, true);
	cam->setTarget(node->getPosition());
	cam->setNearValue(10);
	cam->setFarValue(100000);

	ISceneNodeAnimator * animator = new wwAnim(node, 0.0f, 0.0f, dev->getCursorControl());
	cam->addAnimator(animator);

	u32 fps_time = dev->getTimer()->getTime();
	int frames = 0, fps = 0;

	dev->getTimer()->tick();
	u32 old_time = dev->getTimer()->getTime();
	//	u32 old_time = GetTickCount();
	f32 frame_time = 1000.0f / req_fps;
	s32 wait = 0;
	while (dev->run())
	{
		driver->beginScene(true, true, SColor(255, 120, 120, 120));
		smgr->drawAll();
		guienv->drawAll();


		driver->endScene();

		old_time += (u32)frame_time;
		wait = old_time - dev->getTimer()->getTime();

		if (wait > 0)
		{
			dev->sleep(wait);
		}

		int lastFPS = driver->getFPS();
		core::stringw tmp = L"FPS: ";
		tmp += lastFPS;

		dev->setWindowCaption(tmp.c_str());

	}
};