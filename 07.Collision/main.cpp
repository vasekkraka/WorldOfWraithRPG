#include <irrlicht.h>
#include "driverChoice.h"
//#include "exampleHelper.h"

#include "CWoWCollisionAnimator.h"
#include "CColManager.h"

using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

#define REQ_FPS 30

#define okruh 100
#define rotspeed 1
#define krok 0.07

class wwAnim : public ISceneNodeAnimator
{
public:

	//! constructor
	wwAnim(ISceneNode* MetaNode, float StartUhelY, float StartUhelX, gui::ICursorControl * CursorControl, scene::CSceneNodeAnimatorWoWCollisionAnimator* anim)
	{
		metaNode = MetaNode;
		rotX = StartUhelX;
		rotY = StartUhelY;
		cursorControl = CursorControl;
		meta_anim = anim;

		can_jump = true;

		rotating = false;
		looking = false;
		moving = false;

		ci = 0;

		for (u32 i = 0; i<KEY_KEY_CODES_COUNT; ++i)
		{
			KeyIsDown[i] = false;
		}

		for (int i = 0; i < 3; i++)
		{
			buttonStat[i] = false;
		}
	}

	//! animates a scene node
	virtual void animateNode(ISceneNode* node, u32 timeMs)
	{
		u32 diff = timeMs - last_update;
		last_update = timeMs;
		
		vector3df novaPozice;
		vector3df rotace;
		rotace.set(0, 0, 0);


		if (isPresed(KEY_KEY_W))
		{
			vector3df soucasna, nova;
			soucasna = metaNode->getPosition();
			nova.X = soucasna.X + sinf((metaNode->getRotation().Y + 90) * (PI / 180)) * krok * diff;
			nova.Z = soucasna.Z + cosf((metaNode->getRotation().Y + 90) * (PI / 180)) * krok * diff;
			nova.Y = soucasna.Y;
			metaNode->setPosition(nova);


		}

		if (isPresed(KEY_KEY_S))
		{
			vector3df soucasna, nova;
			soucasna = metaNode->getPosition();
			nova.X = soucasna.X - sinf((metaNode->getRotation().Y + 90) * (PI / 180)) * krok * diff;
			nova.Z = soucasna.Z - cosf((metaNode->getRotation().Y + 90) * (PI / 180)) * krok * diff;
			nova.Y = soucasna.Y;
			metaNode->setPosition(nova);
			

		}

		if (isPresed(KEY_KEY_A))
		{
			vector3df soucasna, nova;
			soucasna = metaNode->getPosition();
			nova.X = soucasna.X - cosf((metaNode->getRotation().Y + 90) * (PI / 180)) * krok * diff;
			nova.Z = soucasna.Z + sinf((metaNode->getRotation().Y + 90) * (PI / 180)) * krok * diff;
			nova.Y = soucasna.Y;
			metaNode->setPosition(nova);
		}

		if (isPresed(KEY_KEY_D))
		{
			vector3df soucasna, nova;
			soucasna = metaNode->getPosition();
			nova.X = soucasna.X + cosf((metaNode->getRotation().Y + 90) * (PI / 180)) * krok * diff;
			nova.Z = soucasna.Z - sinf((metaNode->getRotation().Y + 90) * (PI / 180)) * krok * diff;
			nova.Y = soucasna.Y;
			metaNode->setPosition(nova);
		}



		if (isPresed(KEY_SPACE))
		{
			if (!meta_anim->isFalling())
			{
				printf("SPACE\n");
				meta_anim->jump(3);
				KeyIsDown[KEY_SPACE] = false;
			}
			
		}
		

		if (isClicked(1) && !isClicked(2) && !isClicked(3)) //rozhlizeni
		{
			if (looking)
			{
				rotY += (mousePos.X - lookStart.X) * rotspeed;
				rotX += (mousePos.Y - lookStart.Y) * rotspeed;
				//korekce hodnoty
				if (rotX > 90)
				{
					rotX = 90;
				}
				else if (rotX < -90)
				{
					rotX = -90;
				}
				cursorControl->setPosition(lookStart);
			}
			else
			{
				looking = true;
				lookStart = cursorControl->getPosition();
			}

		}
		else
		{
			looking = false;
		}

		if (!isClicked(1) && !isClicked(2) && isClicked(3)) // otaceni
		{
			if (rotating)
			{
				rotY += (mousePos.X - rotStart.X) * rotspeed;
				rotX += (mousePos.Y - rotStart.Y) * rotspeed;
				//korekce hodnoty
				if (rotX > 90)
				{
					rotX = 90;
				}
				else if (rotX < -90)
				{
					rotX = -90;
				}
				cursorControl->setPosition(rotStart);
				rotace.Y = rotY + 90;
				metaNode->setRotation(rotace);
			}
			else
			{
				rotating = true;
				rotStart = cursorControl->getPosition();
			}
		}
		else
		{
			rotating = false;
		}
		novaPozice = caulculatePosition(metaNode->getAbsolutePosition(), rotX, rotY, okruh);
		//printf("%f, %f, %f\n", novaPozice.X, novaPozice.Y, novaPozice.Z);
		node->setPosition(novaPozice);
		node->setRotation(rotace);
		ICameraSceneNode * camera = static_cast<ICameraSceneNode*>(node);
		camera->updateAbsolutePosition();
		camera->setTarget(metaNode->getAbsolutePosition());
		//printf("X: %f, Y: %f,  Z:%f", novaPozice.X, novaPozice.Y, novaPozice.Z);
		//printf("\n\tRotace : X: %f, Y: %f,  Z:%f", rotace.X, rotace.Y, rotace.Z);
		//printf("UHEL: X: %i, Y: %i\n", rotX, rotY);
		//printf("\n");
	}

	//! Writes attributes of the scene node animator.
	virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options = 0) const
	{

	}

	//! Reads attributes of the scene node animator.
	virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options = 0)
	{

	}

	//! Returns type of the scene node animator
	virtual ESCENE_NODE_ANIMATOR_TYPE getType() const { return ESNAT_ROTATION; }

	//! Creates a clone of this animator.
	/** Please note that you will have to drop
	(IReferenceCounted::drop()) the returned pointer after calling this. */
	virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager = 0)
	{
		ISceneNodeAnimator* dds = NULL;
		return dds;
	}

	virtual bool isEventReceiverEnabled() const
	{
		return true;
	}

	virtual bool OnEvent(const SEvent& event)
	{
		switch (event.EventType)
		{
		case EET_MOUSE_INPUT_EVENT:
			mousePos = cursorControl->getPosition();
			buttonStat[0] = event.MouseInput.isLeftPressed();
			buttonStat[1] = event.MouseInput.isMiddlePressed();
			buttonStat[2] = event.MouseInput.isRightPressed();
			break;
		case EET_KEY_INPUT_EVENT:
			if (event.KeyInput.PressedDown)
			{
				KeyIsDown[event.KeyInput.Key] = true;
			}
			else
			{
				KeyIsDown[event.KeyInput.Key] = false;
			}
			break;
		}

		return true;

	}

private:
	ISceneNode * metaNode;
	core::vector3df Rotation;
	core::vector3df Position;
	int rotX, rotY;
	bool buttonStat[3];
	vector2d<s32> mousePos;
	vector2d<s32> rotStart;
	vector2d<s32> lookStart;
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
	bool rotating;
	bool looking;
	gui::ICursorControl * cursorControl;
	bool moving;
	int ci;
	u32 last_update;
	scene::CSceneNodeAnimatorWoWCollisionAnimator* meta_anim;
	bool can_jump;
	u32 last_jump_time;

	bool isClicked(int buttonNumber)
	{
		return buttonStat[buttonNumber - 1];
	}

	bool isPresed(int keyNumber)
	{
		return KeyIsDown[keyNumber];
	}

	vector3df caulculatePosition(vector3df centr, int xUhel, int yUhel, int radius)
	{
		vector3df Cposition;
		ci++;

		float Zy = sinf(xUhel * PI / 180);
		float Zx = sinf(yUhel * PI / 180);
		float Zz = cosf(yUhel * PI / 180);

		Cposition.X = centr.X + (radius * Zx);
		Cposition.Y = centr.Y + (radius * Zy);
		Cposition.Z = centr.Z + (radius * Zz);

		return Cposition;
	}
};


enum
{
	// I use this ISceneNode ID to indicate a scene node that is
	// not pickable by getSceneNodeAndCollisionPointFromRay()
	ID_IsNotPickable = 0,

	// I use this flag in ISceneNode IDs to indicate that the
	// scene node can be picked by ray selection.
	IDFlag_IsPickable = 1 << 0,

	// I use this flag in ISceneNode IDs to indicate that the
	// scene node can be highlighted.  In this example, the
	// homonids can be highlighted, but the level mesh can't.
	IDFlag_IsHighlightable = 1 << 1
};

int main()
{
	// ask user for driver
	video::E_DRIVER_TYPE driverType=driverChoiceConsole();
	if (driverType==video::EDT_COUNT)
		return 1;

	// create device

	IrrlichtDevice *device =
		createDevice(driverType, core::dimension2d<u32>(800, 600), 16, false);

	if (device == 0)
		return 1; // could not create selected driver.

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	//const io::path mediaPath = getExampleMediaPath();

	//device->getFileSystem()->addFileArchive(mediaPath + "map-20kdm2.pk3");

	scene::IAnimatedMesh* q3levelmesh = smgr->getMesh("drtt.obj");
	scene::IMeshSceneNode* q3node = 0;

	// The Quake mesh is pickable, but doesn't get highlighted.
	if (q3levelmesh)
		q3node = smgr->addOctreeSceneNode(q3levelmesh->getMesh(0), 0, IDFlag_IsPickable);
	q3node->setScale(vector3df(2,6,3));
	q3node->getMaterial(0).Lighting = true;
	q3node->setMaterialType(EMT_SOLID);
	q3node->setMaterialFlag(EMF_LIGHTING, false);


	scene::ITriangleSelector* selector = 0;

	if (q3node)
	{
		q3node->setPosition(core::vector3df(0,-1000,-0));

		selector = smgr->createOctreeTriangleSelector(
				q3node->getMesh(), q3node, 128);
		q3node->setTriangleSelector(selector);
		// We're not done with this selector yet, so don't drop it.
	}

	// START -------------------------------- player char

	scene::IAnimatedMeshSceneNode* playernode = 0;

	// And this mdl file uses skinned skeletal animation.
	playernode = smgr->addAnimatedMeshSceneNode(smgr->getMesh("yodan.mdl"),
		0);
	playernode->setPosition(core::vector3df(-40, -25, 20));
	playernode->setScale(core::vector3df(0.2f));
	playernode->getMaterial(0).Lighting = true;
	playernode->setMaterialType(EMT_SOLID);
	playernode->setMaterialFlag(EMF_LIGHTING, false);
	playernode->setAnimationSpeed(20.f);
	playernode->setFrameLoop(0, 60);

	scene::CSceneNodeAnimatorWoWCollisionAnimator* anim = new irr::scene::CSceneNodeAnimatorWoWCollisionAnimator(smgr, selector, playernode, 
		core::vector3df(8, 18, 5), // elipsoid size 
		core::vector3df(0, -10, 0), // gravity
		core::vector3df(0, -11, 2), // translation
		0.003f); // sliding speed

	playernode->addAnimator(anim);


	playernode->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(playernode->isDebugDataVisible() ^ scene::EDS_BBOX));

	// END -------------------------------- player char


	// START -------------------------------- wow like camera

	ISceneNodeAnimator * animator = new wwAnim(playernode, 0.0f, 0.0f, device->getCursorControl(), anim);

	scene::ICameraSceneNode* camera = smgr->addCameraSceneNode(); 

	camera->addAnimator(animator);

	// END -------------------------------- wow like camera

	//device->getCursorControl()->setVisible(false);

	scene::IAnimatedMeshSceneNode* node = 0;

	video::SMaterial material;

	// Add an MD2 node, which uses vertex-based animation.
	node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("faerie.md2"),
						0, IDFlag_IsPickable | IDFlag_IsHighlightable);
	node->setPosition(core::vector3df(-90,-15,-140)); // Put its feet on the floor.
	node->setScale(core::vector3df(1.6f)); // Make it appear realistically scaled
	node->setMD2Animation(scene::EMAT_POINT);
	node->setAnimationSpeed(20.f);
	material.setTexture(0, driver->getTexture("faerie2.bmp"));
	material.Lighting = true;
	material.NormalizeNormals = true;
	node->getMaterial(0) = material;


	selector = smgr->createTriangleSelector(node);
	node->setTriangleSelector(selector);
	selector->drop(); // We're done with this selector, so drop it now.

	// And this B3D file uses skinned skeletal animation.
	node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("ninja.b3d"),
						0, IDFlag_IsPickable | IDFlag_IsHighlightable);
	node->setScale(core::vector3df(10));
	node->setPosition(core::vector3df(-75,-66,-80));
	node->setRotation(core::vector3df(0,90,0));
	node->setAnimationSpeed(8.f);
	node->getMaterial(0).NormalizeNormals = true;
	node->getMaterial(0).Lighting = true;
	// Just do the same as we did above.
	selector = smgr->createTriangleSelector(node);
	node->setTriangleSelector(selector);
	selector->drop();

	// This X files uses skeletal animation, but without skinning.
	node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("dwarf.x"),
						0, IDFlag_IsPickable | IDFlag_IsHighlightable);
	node->setPosition(core::vector3df(-70,-66,-30)); // Put its feet on the floor.
	node->setRotation(core::vector3df(0,-90,0)); // And turn it towards the camera.
	node->setAnimationSpeed(20.f);
	node->getMaterial(0).Lighting = true;
	selector = smgr->createTriangleSelector(node);
	node->setTriangleSelector(selector);
	selector->drop();


	// And this mdl file uses skinned skeletal animation.
	node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("yodan.mdl"),
						0, IDFlag_IsPickable | IDFlag_IsHighlightable);
	node->setPosition(core::vector3df(-90,-25,20));
	node->setScale(core::vector3df(0.8f));
	node->getMaterial(0).Lighting = true;
	node->setAnimationSpeed(20.f);

	// Just do the same as we did above.
	selector = smgr->createTriangleSelector(node);
	node->setTriangleSelector(selector);
	selector->drop();

	material.setTexture(0, 0);
	material.Lighting = false;


	// Add a light, so that the unselected nodes aren't completely dark.
	/*scene::ILightSceneNode * light = smgr->addLightSceneNode(0, core::vector3df(-60,100,400),
		video::SColorf(1.0f,1.0f,1.0f,1.0f), 600.0f);*/
	/*light->setID(ID_IsNotPickable); */// Make it an invalid target for selection.

	// Remember which scene node is highlighted
	scene::ISceneCollisionManager* collMan = smgr->getSceneCollisionManager();
	int lastFPS = -1;

	// draw the selection triangle only as wireframe
	material.Wireframe=true;

	while(device->run())
	if (device->isWindowActive())
	{
		u32 oldtime = device->getTimer()->getTime();
		driver->beginScene(true, true, 0);
		smgr->drawAll();
		
		playernode->updateAbsolutePosition();
		aabbox3d<f32> playerbox = playernode->getTransformedBoundingBox();

		//printf("Bound: %f, %f, %f\n", playerbox.MaxEdge.X, playerbox.MaxEdge.Y, playerbox.MaxEdge.Z);
		//printf("Pos: %f, %f, %f\n", playernode->getAbsolutePosition().X, playernode->getAbsolutePosition().Y, playernode->getAbsolutePosition().Z);
		driver->draw3DBox(playerbox, SColor(25536));

		driver->endScene();

		int fps = driver->getFPS();

		if (lastFPS != fps)
		{
			core::stringw str = L"Collision detection test [";
			str += driver->getName();
			str += "] FPS:";
			str += fps;

			device->setWindowCaption(str.c_str());
			lastFPS = fps;
		}

		u32 newtime = device->getTimer()->getTime();

		u32 diff = newtime - oldtime;

		if (diff == 0)
		{
			diff = 1;
		}

		//printf("Cekani: %u, %u\n", newtime,  oldtime);

		if (diff < (3.3))
		{
			device->sleep((3.3) - diff);
		}

	}

	device->drop();

	return 0;
}

/*
**/

