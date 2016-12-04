
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

#define req_fps 60

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
		driver->getTexture(PATH_PREFIX "/IrrTestScene/skybox/irrlicht2_up.jpg"),
		driver->getTexture(PATH_PREFIX "/IrrTestScene/skybox/irrlicht2_dn.jpg"),
		driver->getTexture(PATH_PREFIX "/IrrTestScene/skybox/irrlicht2_lf.jpg"),
		driver->getTexture(PATH_PREFIX "/IrrTestScene/skybox/irrlicht2_rt.jpg"),
		driver->getTexture(PATH_PREFIX "/IrrTestScene/skybox/irrlicht2_ft.jpg"),
		driver->getTexture(PATH_PREFIX "/IrrTestScene/skybox/irrlicht2_bk.jpg"));

	//IAnimatedMesh * dwarf_mesh = smgr->getMesh(PATH_PREFIX "/IrrTestScene/dwarf/dwarf.x");

	IAnimatedMesh * dwarf_mesh = smgr->getMesh(PATH_PREFIX "/IrrTestScene/draenei/draenei_female.blend.x");

	int casti[] = {1,6,11,16};

	ISkinnedMesh * skinned = makeMeshFromParts(&gState, dwarf_mesh, 4, casti);

	IAnimatedMeshSceneNode * clone_node = smgr->addAnimatedMeshSceneNode(skinned, 0, 0, vector3df(30, 30, 0));

	clone_node->setAnimationSpeed(25);

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
		driver->getTexture(PATH_PREFIX "/IrrTestScene/skybox/irrlicht2_up.jpg"),
		driver->getTexture(PATH_PREFIX "/IrrTestScene/skybox/irrlicht2_dn.jpg"),
		driver->getTexture(PATH_PREFIX "/IrrTestScene/skybox/irrlicht2_lf.jpg"),
		driver->getTexture(PATH_PREFIX "/IrrTestScene/skybox/irrlicht2_rt.jpg"),
		driver->getTexture(PATH_PREFIX "/IrrTestScene/skybox/irrlicht2_ft.jpg"),
		driver->getTexture(PATH_PREFIX "/IrrTestScene/skybox/irrlicht2_bk.jpg"));

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

	IAnimatedMesh* mesh = smgr->getMesh("../../../data/IrrTestScene/human/human_stand.x");
	//((ISkinnedMesh *)mesh)->finalize();
	
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

	IMeshSceneNode* terrain = smgr->addMeshSceneNode(smgr->getMesh("../../../data/IrrTestScene/gilneas/gilneas_2.obj"), 0, 0, core::vector3df(0.f, -1000.f, 0.f), core::vector3df(0.f, 0.f, 0.f), core::vector3df(20.0f, 20.0f, 20.0f));
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


	terrain->setMaterialFlag(video::EMF_FOG_ENABLE, true);
	node->setMaterialFlag(video::EMF_FOG_ENABLE, true);
	driver->setFog(video::SColor(0, 234, 234, 234), video::EFT_FOG_LINEAR, -300, 2000, .0003f, true, false);


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

void BootyBayScene(WowKlient::Core::GameState gState)
{
	IrrlichtDevice * dev = gState.irrDevice;
	IVideoDriver* driver = dev->getVideoDriver();
	ISceneManager* smgr = dev->getSceneManager();
	IGUIEnvironment* guienv = dev->getGUIEnvironment();

	driver->setTextureCreationFlag(E_TEXTURE_CREATION_FLAG::ETCF_ALWAYS_32_BIT);

	scene::ISceneNode* skybox = smgr->addSkyBoxSceneNode(
		driver->getTexture(PATH_PREFIX "/IrrTestScene/skybox/irrlicht2_up.jpg"),
		driver->getTexture(PATH_PREFIX "/IrrTestScene/skybox/irrlicht2_dn.jpg"),
		driver->getTexture(PATH_PREFIX "/IrrTestScene/skybox/irrlicht2_lf.jpg"),
		driver->getTexture(PATH_PREFIX "/IrrTestScene/skybox/irrlicht2_rt.jpg"),
		driver->getTexture(PATH_PREFIX "/IrrTestScene/skybox/irrlicht2_ft.jpg"),
		driver->getTexture(PATH_PREFIX "/IrrTestScene/skybox/irrlicht2_bk.jpg"));

	IAnimatedMesh* mesh = smgr->getMesh("../../../data/IrrTestScene/draenei/draenei_female.blend.x");
	//((ISkinnedMesh *)mesh)->finalize();

	printf("--> Node\n");

	if (!mesh)
	{
		dev->drop();
		return;
	}

	int casti[] = { 1, 6, 11, 16, 26, 27, 37, 57 };

	ISkinnedMesh * skinned = makeMeshFromParts(&gState, mesh, 8, casti);

	IAnimatedMeshSceneNode * node = smgr->addAnimatedMeshSceneNode(skinned, 0, 0, vector3df(0, 0, -2500));

	node->setAnimationSpeed(20);

	node->setFrameLoop(2119, 2158);

	//node->setFrameLoop(1490, 1506);

	node->setMaterialFlag(E_MATERIAL_FLAG::EMF_LIGHTING, false);

	//node->setDebugDataVisible(irr::scene::E_DEBUG_SCENE_TYPE::EDS_SKELETON);

	node->setScale(vector3df(30.0f, 30.0f, 30.0f));
	node->setMaterialFlag(EMF_LIGHTING, false);
	node->setMaterialFlag(EMF_NORMALIZE_NORMALS, false);

	printf("--> Node parts\n");

	IMeshSceneNode* terrain = smgr->addMeshSceneNode(smgr->getMesh("../../../data/IrrTestScene/BootyBay/BootyBay.obj"), 0, 0, core::vector3df(0.f, -1000.f, 0.f), core::vector3df(0.f, 0.f, 0.f), core::vector3df(20.0f, 20.0f, 20.0f));
	ITriangleSelector * trg = smgr->createOctreeTriangleSelector(terrain->getMesh(), terrain);
	printf("--> Teren\n");
	terrain->setTriangleSelector(trg);
	trg->drop();
	printf("--> Selector\n");
	ISceneNodeAnimatorCollisionResponse * colAnim = new irr::scene::CSceneNodeAnimatorWoWCollisionAnimator(smgr, trg, node, core::vector3df(20.0f, 30.0f, 20.0f), core::vector3df(0.0f, -75.0f, 0.0f), core::vector3df(0.0f, -30.0f, 0.0f), 0.05f);

	node->addAnimator(colAnim);
	colAnim->drop();
	terrain->setMaterialFlag(video::EMF_LIGHTING, false);
	terrain->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	terrain->setMaterialFlag(E_MATERIAL_FLAG::EMF_BACK_FACE_CULLING, false);

	terrain->setMaterialType(E_MATERIAL_TYPE::EMT_SOLID);

	RealisticWaterSceneNode * water = new RealisticWaterSceneNode(smgr, 10000, 10000, "../../../Data");

	water->setColorBlendFactor(0.0f);

	water->setPosition(vector3df(0, -2048, -4500));

	water->setParent(smgr->getRootSceneNode());

	for (u32 i = 0; i < terrain->getMaterialCount(); i++)
	{
		std::string ttu = terrain->getMaterial(i).getTexture(0)->getName().getPath().c_str();
		
		if ((ttu.find("JLO_BBAY_NET.png") != std::string::npos) || (ttu.find("VASHJIR_BULLKELP06.PNG") != std::string::npos) || (ttu.find("StranglethornTree_Leaves_Set.png") != std::string::npos) || (ttu.find("None_StrangleTree1_Vine256.png") != std::string::npos))
		{
			terrain->getMaterial(i).MaterialType = E_MATERIAL_TYPE::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
		}

	}

	ICameraSceneNode* cam = smgr->addCameraSceneNode(0, vector3df(0, 100, 100), vector3df(0, 0, 0), 1, true);
	cam->setTarget(node->getPosition());
	cam->setNearValue(10);
	cam->setFarValue(50000);

	ISceneNodeAnimator * animator = new wwAnim(node, 0.0f, 0.0f, dev->getCursorControl());
	cam->addAnimator(animator);

	terrain->setMaterialFlag(video::EMF_FOG_ENABLE, true);
	node->setMaterialFlag(video::EMF_FOG_ENABLE, true);
	driver->setFog(video::SColor(0, 234, 234, 234), video::EFT_FOG_LINEAR, -1000, 6000, .0003f, true, false);

	u32 fps_time = dev->getTimer()->getTime();
	int frames = 0, fps = 0;

	dev->getTimer()->tick();
	u32 old_time = dev->getTimer()->getTime();
	//	u32 old_time = GetTickCount();
	f32 frame_time = 1000.0f / gState.gConf->reqFps;
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

		tmp += L" Triangles count: ";

		tmp += terrain->getTriangleSelector()->getTriangleCount();

		dev->setWindowCaption(tmp.c_str());

	}
};