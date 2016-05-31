#include <irrlicht.h>
#include "CWoWCollisionAnimator.h"

#include "HWowLikeCameraAnimator.h"

#include <Windows.h>


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;




#define req_fps 50




int main()
{
	IrrlichtDevice * dev = createDevice(EDT_OPENGL, dimension2d<u32>(1366, 768), 32, false, false, false);
	IVideoDriver* driver = dev->getVideoDriver();
    ISceneManager* smgr = dev->getSceneManager();
    IGUIEnvironment* guienv = dev->getGUIEnvironment();

	//smgr->setAmbientLight(video::SColorf(1.0f, 1.0f, 1.0f));

	driver->setTextureCreationFlag(E_TEXTURE_CREATION_FLAG::ETCF_ALWAYS_32_BIT);
	
	vector3df a = vector3df(1, 0, 1);
	vector3df b = vector3df(1, 1, 1);
	
	a.normalize();
	b.normalize();

	f32 c = a.dotProduct(b);

	double uhel_cos = acos(c) * 180.0 / PI;
	double uhel_sin = asin(c) * 180.0 / PI;


	IAnimatedMesh* mesh = smgr->getMesh("../../../data/IrrTestScene/human_stand.b3d");
    if (!mesh)
    {
        dev->drop();
        return 1;
    }
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);

	node->setAnimationSpeed(30);

	//IMeshSceneNode* node = smgr->addSphereSceneNode(20.0F, 16);

	//node->setDebugDataVisible(E_DEBUG_SCENE_TYPE::EDS_BBOX_ALL);

	

	if (node)
    {
        node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMaterialFlag(EMF_NORMALIZE_NORMALS, false);
        // node->setMaterialTexture( 0, driver->getTexture("../../../data/IrrTestScene/sydney.bmp") );
		node->setPosition(vector3df(0,1000,0));
		node->setRotation(vector3df(0, 45, 0));
		node->setScale(vector3df(60.0f, 60.0f, 60.0f));
		//node->setMaterialType(E_MATERIAL_TYPE::EMT_SOLID);
	}

	IMeshSceneNode* terrain = smgr->addMeshSceneNode(smgr->getMesh("../../../data/IrrTestScene/gilneas_2.obj"), 0, 0, core::vector3df(0.f, -1000.f, 0.f), core::vector3df(0.f, 0.f, 0.f), core::vector3df(20.0f, 20.0f, 20.0f));
	ITriangleSelector * trg = smgr->createOctreeTriangleSelector(terrain->getMesh(), terrain);


	terrain->setTriangleSelector(trg);
	trg->drop();

	ISceneNodeAnimatorCollisionResponse * colAnim = new irr::scene::CSceneNodeAnimatorWoWCollisionAnimator(smgr, trg, node, core::vector3df(20.0f,30.0f,20.0f), core::vector3df(0.0f,-75.0f,0.0f), core::vector3df(0.0f,-30.0f,0.0f), 0.05f);

	node->addAnimator(colAnim);
	colAnim->drop();
	terrain->setMaterialFlag(video::EMF_LIGHTING, false);
	terrain->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	terrain->setMaterialFlag(E_MATERIAL_FLAG::EMF_BACK_FACE_CULLING, false);

	//terrain->setDebugDataVisible(E_DEBUG_SCENE_TYPE::EDS_MESH_WIRE_OVERLAY);
	

	terrain->setMaterialType(E_MATERIAL_TYPE::EMT_SOLID);




	ICameraSceneNode* cam = smgr->addCameraSceneNode(0, vector3df(0,100,100), vector3df(0,0,0), 1, true);
	cam->setTarget(node->getPosition());
	cam->setNearValue(10);
	cam->setFarValue(100000);
	
	ISceneNodeAnimator * animator = new wwAnim(node, 0.0f, 0.0f, dev->getCursorControl());
	cam->addAnimator(animator);

	u32 fps_time = dev->getTimer()->getTime();
	int frames = 0,fps = 0;

	dev->getTimer()->tick();
	u32 old_time = dev->getTimer()->getTime();
//	u32 old_time = GetTickCount();
	f32 frame_time = 1000.0f / req_fps;
	s32 wait = 0;
	while(dev->run())
	{
		driver->beginScene(true, true, SColor(255,120,120,120));
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
	return 0;
}