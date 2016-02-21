#include <irrlicht.h>
#include "CWoWCollisionAnimator.h"

#include <Windows.h>


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


#define okruh 500
#define rotspeed 1
#define krok 3

class wwAnim : public ISceneNodeAnimator
	{
	public:
		
		//! constructor
		wwAnim(ISceneNode* MetaNode, float StartUhelY, float StartUhelX, gui::ICursorControl * CursorControl, IMeshSceneNode* terr)
		{
			metaNode = MetaNode;
			rotX = StartUhelX;
			rotY = StartUhelY;
			cursorControl = CursorControl;
			terrain = terr;

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

			if(isPresed(KEY_KEY_W))
			{
				vector3df soucasna, nova;
				soucasna = metaNode->getPosition();
				nova.X = soucasna.X + sinf((metaNode->getRotation().Y + 90) * (PI / 180)) * krok;
				nova.Z = soucasna.Z + cosf((metaNode->getRotation().Y + 90) * (PI / 180)) * krok;
				nova.Y = soucasna.Y;
				metaNode->setPosition(nova);


			}else
			{
				
			}

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
			printf("%f, %f, %f\n", metaNode->getAbsolutePosition().X, metaNode->getAbsolutePosition().Y, metaNode->getAbsolutePosition().Z);
			novaPozice = caulculatePosition(metaNode->getAbsolutePosition(), rotX, rotY, okruh);
			//printf("%f, %f, %f\n", novaPozice.X, novaPozice.Y, novaPozice.Z);
			node->setPosition(novaPozice);
			node->setRotation(rotace);
			ICameraSceneNode * camera = static_cast<ICameraSceneNode*>(node);
			camera->updateAbsolutePosition();
			camera->setTarget(metaNode->getAbsolutePosition());
			//printf("X: %f, Y: %f,  Z:%f", novaPozice.X, novaPozice.Y, novaPozice.Z);
			//printf("\n\tRotace : X: %f, Y: %f,  Z:%f", rotace.X, rotace.Y, rotace.Z);
			//printf("UHEL: %i", rotY);
			//printf("\n");
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
		IMeshSceneNode* terrain;
		int ci;

		bool isClicked(int buttonNumber)
		{
			return buttonStat[buttonNumber-1];
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
			Cposition.X = centr.X + ( radius * Zx);
			Cposition.Y = centr.Y + ( radius * Zy);
			Cposition.Z = centr.Z + ( radius * Zz);

			return Cposition;
		}
	};


int main()
{
	IrrlichtDevice * dev = createDevice(EDT_DIRECT3D9, dimension2d<u32>(1366, 768), 16, false, false, false);
	IVideoDriver* driver = dev->getVideoDriver();
    ISceneManager* smgr = dev->getSceneManager();
    IGUIEnvironment* guienv = dev->getGUIEnvironment();

	driver->setTextureCreationFlag(E_TEXTURE_CREATION_FLAG::ETCF_ALWAYS_16_BIT);
	
	vector3df a = vector3df(0, 0, 0);
	vector3df b = vector3df(1, 1, 0);

	a.normalize();
	b.normalize();

	f32 c = a.dotProduct(b);

	double uhel_cos = acos(c) * 180.0 / PI;
	double uhel_sin = asin(c) * 180.0 / PI;


	IAnimatedMesh* mesh = smgr->getMesh("../../media/sydney.md2");
    if (!mesh)
    {
        dev->drop();
        return 1;
    }
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);
	if (node)
    {
        //node->setMaterialFlag(EMF_LIGHTING, false);
        node->setMaterialTexture( 0, driver->getTexture("../../media/sydney.bmp") );
		node->setPosition(vector3df(100,1000,100));
		node->setRotation(vector3df(0, 45, 0));
	}

	IMeshSceneNode* terrain = smgr->addMeshSceneNode(smgr->getMesh("./drtt.obj"), 0, 0, core::vector3df(0.f, -1000.f, 0.f), core::vector3df(0.f, 0.f, 0.f), core::vector3df(10.0f, 5.0f, 10.0f));
	ITriangleSelector * trg = smgr->createOctreeTriangleSelector(terrain->getMesh(), terrain);


	printf("Trg : %i\n ", trg->getTriangleCount());

	terrain->setTriangleSelector(trg);
	trg->drop();

	ISceneNodeAnimatorCollisionResponse * colAnim = new irr::scene::CSceneNodeAnimatorWoWCollisionAnimator(smgr, trg, node, core::vector3df(60.0f,20.0f,60.0f), core::vector3df(0.0f,-10.0f,0.0f), core::vector3df(0.0f,0.0f,0.0f));

	node->addAnimator(colAnim);
	colAnim->drop();
	terrain->setMaterialFlag(video::EMF_LIGHTING, false);
    terrain->setMaterialTexture(0,driver->getTexture("./terrain-t.png"));
	

	ICameraSceneNode* cam = smgr->addCameraSceneNode(0, vector3df(0,100,100), vector3df(0,0,0), 1, true);
	cam->setTarget(node->getPosition());
	
	ISceneNodeAnimator * animator = new wwAnim(node, 0.0f, 0.0f, dev->getCursorControl(), terrain);
	cam->addAnimator(animator);

	float time = dev->getTimer()->getTime();
	float wait = 0;
	int i = 0;
	while(dev->run())
	{

		driver->beginScene(true, true, SColor(255,0,0,0));
		smgr->drawAll(); 
		guienv->drawAll();
		driver->endScene();
		float diff = dev->getTimer()->getTime() - time;
		
		/*if((1000 / 30) - diff > 0)
		{
			wait = (1000 / 30) - diff;
			dev->sleep(wait, false);
		}*/


		int lastFPS = driver->getFPS();
        core::stringw tmp = L"FPS: ";
        tmp += lastFPS;
        dev->setWindowCaption(tmp.c_str());
		time = dev->getTimer()->getTime();

	}
	return 0;
}