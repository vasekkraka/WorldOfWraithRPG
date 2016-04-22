#include <irrlicht.h>
#include "CWoWCollisionAnimator.h"

#include <Windows.h>


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


#define cam_circle 1000
#define rotspeed 0.5f
#define krok 150

#define req_fps 50

class wwAnim : public ISceneNodeAnimator
	{
	public:
		
		//! constructor
		wwAnim(ISceneNode* MetaNode, float StartUhelY, float StartUhelX, gui::ICursorControl * CursorControl)
		{
			metaNode = MetaNode;
			rotX = StartUhelX;
			rotY = StartUhelY;
			wheel = 0;
			okruh = cam_circle;
			cursorControl = CursorControl;
			last_anim_time = 0;
			move_cycle = 0;
			anim_cycle = 0;

			rotating = false;
			looking = false;
			moving = false;

			ci = 0;

			for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
			{
				KeyIsDown[i] = false;
			}

			for(int i = 0; i < 3; i++)
			{
				buttonStat[i] = false;
			}	
		}

		//! animates a scene node
		virtual void animateNode(ISceneNode* node, u32 timeMs)
		{
			
			vector3df novaPozice;
			vector3df rotace;
			rotace.set(0, 0, 0);
			anim_cycle++;

			if(isPresed(KEY_KEY_W))
			{
				move_cycle++;

				
				f32 delta;
				if ((timeMs - last_anim_time) == 0)
				{
					delta = 1000.0f;
				}
				else
				{
					delta = 1000.0f / (timeMs - last_anim_time);
				}
				//printf("Cas: %u, %f %f\n", (timeMs - last_anim_time), delta, sinf((metaNode->getRotation().Y + 90) * (PI / 180) * (krok / delta)));
				last_anim_time = timeMs;


				vector3df soucasna, nova, rozdil;
				soucasna = metaNode->getPosition();
				nova.X = soucasna.X + sinf((metaNode->getRotation().Y + 90) * (PI / 180)) * (krok / delta);
				nova.Z = soucasna.Z + cosf((metaNode->getRotation().Y + 90) * (PI / 180)) * (krok / delta);
				nova.Y = soucasna.Y;

				rozdil.X = nova.X - soucasna.X;
				rozdil.Y = nova.Y - soucasna.Y;
				rozdil.Z = nova.Z - soucasna.Z;

				metaNode->setPosition(nova);
			}
			last_anim_time = timeMs;
			
			


			if(isClicked(1) && !isClicked(2) && !isClicked(3)) //rozhlizeni
			{
				if(looking)
				{
					rotY += (mousePos.X - lookStart.X) * rotspeed;
					rotX += (mousePos.Y - lookStart.Y) * rotspeed;
					//korekce hodnoty
					if(rotX > 90)
					{
						rotX = 90;
					}else if(rotX < -90)
					{
						rotX = -90;
					}
					cursorControl->setPosition(lookStart);
				}else
				{
					looking = true;
					lookStart = cursorControl->getPosition();
				}

			}else
			{
				looking = false;
			}

			if(!isClicked(1) && !isClicked(2) && isClicked(3)) // otaceni
			{
				if(rotating)
				{
					rotY += (mousePos.X - rotStart.X) * rotspeed;
					rotX += (mousePos.Y - rotStart.Y) * rotspeed;
					//korekce hodnoty
					if(rotX > 90)
					{
						rotX = 90;
					}else if(rotX < -90)
					{
						rotX = -90;
					}
					cursorControl->setPosition(rotStart);
					rotace.Y = rotY + 90;
					metaNode->setRotation(rotace);
				}else
				{
					rotating = true;
					rotStart = cursorControl->getPosition();
				}
			}else
			{
				rotating = false;
			}
			

			if (isPresed(KEY_SPACE))
			{
				const ISceneNodeAnimatorList& animators = metaNode->getAnimators();
				ISceneNodeAnimatorList::ConstIterator it = animators.begin();
				while (it != animators.end())
				{
					if ((*it)->getType() == ESNAT_COLLISION_RESPONSE)
					{
						CSceneNodeAnimatorWoWCollisionAnimator * collResp = static_cast<CSceneNodeAnimatorWoWCollisionAnimator *>(*it);
						if (!collResp->isFalling())
						{
							collResp->jump(350, 700);
						}
					}
					it++;
				}
			}

			if (wheelPos() != 0)
			{
				okruh += wheelPos();
				if (okruh < 20 || okruh > 1000)
				{
					okruh -= wheelPos();
				}
				//printf("%i\n", wheelPos());
			}

			novaPozice = caulculatePosition(metaNode->getAbsolutePosition(), rotX, rotY, okruh);
			node->setPosition(novaPozice);
			node->setRotation(rotace);

			ICameraSceneNode * camera = static_cast<ICameraSceneNode*>(node);
			camera->updateAbsolutePosition();
			camera->setTarget(metaNode->getAbsolutePosition());

		}

		//! Writes attributes of the scene node animator.
		virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const
		{
		
		}

		//! Reads attributes of the scene node animator.
		virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0)
		{
		
		}

		//! Returns type of the scene node animator
		virtual ESCENE_NODE_ANIMATOR_TYPE getType() const { return ESNAT_ROTATION; }
		
		//! Creates a clone of this animator.
		/** Please note that you will have to drop
		(IReferenceCounted::drop()) the returned pointer after calling this. */
		virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager=0)
		{
			ISceneNodeAnimator* dds= NULL;
			return dds;
		}

		virtual bool isEventReceiverEnabled() const
		{
			return true;
		}

		virtual bool OnEvent(const SEvent& event)
		{

			switch(event.EventType)
			{
				case EET_MOUSE_INPUT_EVENT:
					mousePos = cursorControl->getPosition();
					buttonStat[0] = event.MouseInput.isLeftPressed();
					buttonStat[1] = event.MouseInput.isMiddlePressed();
					buttonStat[2] = event.MouseInput.isRightPressed();

					wheel += event.MouseInput.Wheel;

					if (wheel > 0 && event.MouseInput.Wheel < 0)
					{
						wheel = 0;
					}

					if (wheel < 0 && event.MouseInput.Wheel > 0)
					{
						wheel = 0;
					}
				break;
				case EET_KEY_INPUT_EVENT:
					if(event.KeyInput.PressedDown)
					{
						KeyIsDown[event.KeyInput.Key] = true;
					}else
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
		int rotX, rotY, wheel, okruh;
		bool buttonStat[3];
		vector2d<s32> mousePos;
		vector2d<s32> rotStart;
		vector2d<s32> lookStart;
		bool KeyIsDown[KEY_KEY_CODES_COUNT];
		bool rotating;
		bool looking;
		gui::ICursorControl * cursorControl;
		bool moving;
		IMeshSceneNode* terrain;
		int ci;
		u32 last_anim_time;
		int anim_cycle, move_cycle;

		bool isClicked(int buttonNumber)
		{
			return buttonStat[buttonNumber-1];
		}

		bool isPresed(int keyNumber)
		{
			return KeyIsDown[keyNumber];
		}

		int wheelPos()
		{
			return wheel;
		}

		vector3df caulculatePosition(vector3df centr, int xUhel, int yUhel, int radius)
		{
			vector3df Cposition;
			ci++;

			float Zy = sinf(xUhel * PI / 180);
			float Zx = sinf(yUhel * PI / 180);
			float Zz = cosf(yUhel * PI / 180);
			Cposition.X = centr.X + ( radius * Zx);
			Cposition.Y = centr.Y + ( radius * Zy);
			Cposition.Z = centr.Z + ( radius * Zz);

			return Cposition;
		}
	};


int main()
{
	IrrlichtDevice * dev = createDevice(EDT_OPENGL, dimension2d<u32>(800, 600), 16, false, false, false);
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


	IAnimatedMesh* mesh = smgr->getMesh("../../../data/IrrTestScene/panda.obj");
    if (!mesh)
    {
        dev->drop();
        return 1;
    }
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);

	node->setDebugDataVisible(E_DEBUG_SCENE_TYPE::EDS_MESH_WIRE_OVERLAY);

	printf("Primaka: %f", node->getBoundingBox().MaxEdge);

	if (node)
    {
        node->setMaterialFlag(EMF_LIGHTING, false);
        // node->setMaterialTexture( 0, driver->getTexture("../../../data/IrrTestScene/sydney.bmp") );
		node->setPosition(vector3df(0,1000,0));
		node->setRotation(vector3df(0, 45, 0));
		node->setScale(vector3df(10.0f, 10.0f, 10.0f));
	}

	IMeshSceneNode* terrain = smgr->addMeshSceneNode(smgr->getMesh("../../../data/IrrTestScene/chram.obj"), 0, 0, core::vector3df(0.f, -1000.f, 0.f), core::vector3df(0.f, 0.f, 0.f), core::vector3df(10.0f, 10.0f, 10.0f));
	ITriangleSelector * trg = smgr->createOctreeTriangleSelector(terrain->getMesh(), terrain);


	terrain->setTriangleSelector(trg);
	trg->drop();

	ISceneNodeAnimatorCollisionResponse * colAnim = new irr::scene::CSceneNodeAnimatorWoWCollisionAnimator(smgr, trg, node, core::vector3df(8.0f,8.0f,8.0f), core::vector3df(0.0f,-75.0f,0.0f), core::vector3df(0.0f,-7.9f,0.0f));

	node->addAnimator(colAnim);
	colAnim->drop();
	terrain->setMaterialFlag(video::EMF_LIGHTING, false);
	terrain->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	terrain->setMaterialFlag(E_MATERIAL_FLAG::EMF_BACK_FACE_CULLING, false);

	terrain->setDebugDataVisible(E_DEBUG_SCENE_TYPE::EDS_MESH_WIRE_OVERLAY);
	
	
	terrain->setMaterialType(E_MATERIAL_TYPE::EMT_SOLID);



	ICameraSceneNode* cam = smgr->addCameraSceneNode(0, vector3df(0,100,100), vector3df(0,0,0), 1, true);
	cam->setTarget(node->getPosition());
	cam->setFarValue(10000);
	
	ISceneNodeAnimator * animator = new wwAnim(node, 0.0f, 0.0f, dev->getCursorControl());
	cam->addAnimator(animator);

	float fps_time = dev->getTimer()->getTime();
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
		
		old_time += frame_time;
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