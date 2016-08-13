#include <irrlicht.h>
#include "HWoWCollisionAnimator.h"
#include "HWowLikeCameraAnimator.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#define cam_circle 1000
#define rotspeed 0.1f
#define krok 175

//! constructor
wwAnim::wwAnim(ISceneNode* MetaNode, float StartUhelY, float StartUhelX, gui::ICursorControl * CursorControl)
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
void wwAnim::animateNode(ISceneNode* node, u32 timeMs)
{

	vector3df novaPozice;
	vector3df rotace;
	rotace.set(0, 0, 0);
	anim_cycle++;

	if (isPresed(KEY_KEY_W))
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


		vector3df soucasna, nova;
		soucasna = metaNode->getPosition();
		nova.X = soucasna.X + sinf((metaNode->getRotation().Y + 90) * (PI / 180)) * (krok / delta);
		nova.Z = soucasna.Z + cosf((metaNode->getRotation().Y + 90) * (PI / 180)) * (krok / delta);
		nova.Y = soucasna.Y;


		metaNode->setPosition(nova);
	}
	if (isPresed(KEY_KEY_S))
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


		vector3df soucasna, nova;
		soucasna = metaNode->getPosition();
		nova.X = soucasna.X - sinf((metaNode->getRotation().Y + 90) * (PI / 180)) * (krok / delta);
		nova.Z = soucasna.Z - cosf((metaNode->getRotation().Y + 90) * (PI / 180)) * (krok / delta);
		nova.Y = soucasna.Y;


		metaNode->setPosition(nova);
	}
	if (isPresed(KEY_KEY_A))
	{
		f32 delta;
		if ((timeMs - last_anim_time) == 0)
		{
			delta = 1000.0f;
		}
		else
		{
			delta = 1000.0f / (timeMs - last_anim_time);
		}

		last_anim_time = timeMs;

		if (!isClicked(1) && !isClicked(2) && isClicked(3))
		{
			move_cycle++;

			vector3df soucasna, nova;
			soucasna = metaNode->getPosition();
			nova.X = soucasna.X + sinf((metaNode->getRotation().Y) * (PI / 180)) * (krok / delta);
			nova.Z = soucasna.Z + cosf((metaNode->getRotation().Y) * (PI / 180)) * (krok / delta);
			nova.Y = soucasna.Y;
			metaNode->setPosition(nova);
		}
		else
		{
			vector3df rot = metaNode->getRotation();

			f32 rotY;
			rotY = metaNode->getRotation().Y;
			rotY -= 1 * (krok / delta);
			metaNode->setRotation(vector3df(metaNode->getRotation().X, rotY, metaNode->getRotation().Z));
		}
	}

	if (isPresed(KEY_KEY_D))
	{
		f32 delta;
		if ((timeMs - last_anim_time) == 0)
		{
			delta = 1000.0f;
		}
		else
		{
			delta = 1000.0f / (timeMs - last_anim_time);
		}

		last_anim_time = timeMs;

		if (!isClicked(1) && !isClicked(2) && isClicked(3))
		{
			move_cycle++;

			vector3df soucasna, nova;
			soucasna = metaNode->getPosition();
			nova.X = soucasna.X - sinf((metaNode->getRotation().Y) * (PI / 180)) * (krok / delta);
			nova.Z = soucasna.Z - cosf((metaNode->getRotation().Y) * (PI / 180)) * (krok / delta);
			nova.Y = soucasna.Y;
			metaNode->setPosition(nova);
		}
		else
		{
			vector3df rot = metaNode->getRotation();

			f32 rotY;
			rotY = metaNode->getRotation().Y;
			rotY += 1 * (krok / delta);
			metaNode->setRotation(vector3df(metaNode->getRotation().X, rotY, metaNode->getRotation().Z));
		}
	}
	last_anim_time = timeMs;




	if (isClicked(1) && !isClicked(2) && !isClicked(3)) //rozhlizeni
	{
		if (looking)
		{
			rotY += (mousePos.X - lookStart.X) * rotspeed;
			rotX += (mousePos.Y - lookStart.Y) * rotspeed;
			//korekce hodnoty
			if (rotX > 85)
			{
				rotX = 85;
			}
			else if (rotX < -45)
			{
				rotX = -45;
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
			if (rotX > 85)
			{
				rotX = 85;
			}
			else if (rotX < -45)
			{
				rotX = -45;
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
					collResp->jump(125, 500);
				}
			}
			it++;
		}
	}

	if (wheelPos() != 0)
	{
		okruh += wheelPos();
		if (okruh < 20 || okruh > cam_circle)
		{
			okruh -= wheelPos();
		}
		//printf("%i\n", wheelPos());
	}

	metaNode->updateAbsolutePosition();

	novaPozice = caulculatePosition(metaNode->getSceneManager()->getSceneCollisionManager(), metaNode->getBoundingBox().getCenter() + metaNode->getAbsolutePosition(), rotX, rotY, okruh);
	node->setPosition(novaPozice);
	node->setRotation(rotace);

	ICameraSceneNode * camera = static_cast<ICameraSceneNode*>(node);
	camera->updateAbsolutePosition();
	camera->setTarget(metaNode->getAbsolutePosition());

}

//! Writes attributes of the scene node animator.
void wwAnim::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{

}

//! Reads attributes of the scene node animator.
void wwAnim::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{

}

//! Returns type of the scene node animator
ESCENE_NODE_ANIMATOR_TYPE wwAnim::getType() const { return ESNAT_ROTATION; }

//! Creates a clone of this animator.
/** Please note that you will have to drop
(IReferenceCounted::drop()) the returned pointer after calling this. */
ISceneNodeAnimator* wwAnim::createClone(ISceneNode* node, ISceneManager* newManager)
{
	ISceneNodeAnimator* dds = NULL;
	return dds;
}

bool wwAnim::isEventReceiverEnabled() const
{
	return true;
}

bool wwAnim::OnEvent(const SEvent& event)
{

	switch (event.EventType)
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

bool wwAnim::isClicked(int buttonNumber)
{
	return buttonStat[buttonNumber - 1];
}

bool wwAnim::isPresed(int keyNumber)
{
	return KeyIsDown[keyNumber];
}

f32 wwAnim::wheelPos()
{
	return wheel;
}

vector3df wwAnim::caulculatePosition(ISceneCollisionManager * col, vector3df centr, f32 xUhel, f32 yUhel, f32 radius)
{
	vector3df Cposition;
	ci++;

	if (yUhel > 360)
		yUhel = yUhel - 360;

	float Zy = sinf(xUhel * PI / 180);
	float Zx = sinf(yUhel * PI / 180) - (sinf(yUhel * PI / 180) * sinf(xUhel * PI / 180));
	float Zz = cosf(yUhel * PI / 180) - (cosf(yUhel * PI / 180) * sinf(xUhel * PI / 180));

	vector3df poz_kamery = vector3df(Zx, Zy, Zz);

	poz_kamery.setLength(radius);

	poz_kamery += centr;

	triangle3df colTriangle;
	vector3df colPoint;


	if (poz_kamery.Y < centr.Y)
	{
		poz_kamery.Y = centr.Y;
	}

	ISceneNode * nd = col->getSceneNodeAndCollisionPointFromRay(line3df(centr, poz_kamery), colPoint, colTriangle);

	if (nd != NULL)
	{
		printf("Zasah\n");
		return colPoint;
	}
	else
	{
		return poz_kamery;
	}


}
