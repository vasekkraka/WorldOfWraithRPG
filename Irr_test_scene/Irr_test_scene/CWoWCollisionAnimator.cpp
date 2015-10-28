// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CWoWCollisionAnimator.h"

#include <irrlicht.h>

#include "CColManager.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

namespace irr
{
namespace scene
{

//! constructor
CSceneNodeAnimatorWoWCollisionAnimator::CSceneNodeAnimatorWoWCollisionAnimator(
		ISceneManager* scenemanager,
		ITriangleSelector* world, ISceneNode* object,
		const core::vector3df& ellipsoidRadius,
		const core::vector3df& gravityPerSecond,
		const core::vector3df& ellipsoidTranslation,
		f32 slidingSpeed)
: Radius(ellipsoidRadius), Gravity(gravityPerSecond), Translation(ellipsoidTranslation),
	World(world), Object(object), SceneManager(scenemanager), LastTime(0),
	SlidingSpeed(slidingSpeed), CollisionNode(0), CollisionCallback(0),
	Falling(false), IsCamera(false), AnimateCameraTarget(true), CollisionOccurred(false),
	FirstUpdate(true)
{
	#ifdef _DEBUG
	setDebugName("CSceneNodeAnimatorWoWCollisionAnimator");
	#endif

	if (World)
		World->grab();

	setNode(Object);
}


//! destructor
CSceneNodeAnimatorWoWCollisionAnimator::~CSceneNodeAnimatorWoWCollisionAnimator()
{
	if (World)
		World->drop();

	if (CollisionCallback)
		CollisionCallback->drop();
}


//! Returns if the attached scene node is falling, which means that
//! there is no blocking wall from the scene node in the direction of
//! the gravity.
bool CSceneNodeAnimatorWoWCollisionAnimator::isFalling() const
{
//	_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
	return Falling;
}


//! Sets the radius of the ellipsoid with which collision detection and
//! response is done.
void CSceneNodeAnimatorWoWCollisionAnimator::setEllipsoidRadius(
	const core::vector3df& radius)
{
	Radius = radius;
	FirstUpdate = true;
}


//! Returns the radius of the ellipsoid with wich the collision detection and
//! response is done.
core::vector3df CSceneNodeAnimatorWoWCollisionAnimator::getEllipsoidRadius() const
{
	return Radius;
}


//! Sets the gravity of the environment.
void CSceneNodeAnimatorWoWCollisionAnimator::setGravity(const core::vector3df& gravity)
{
	Gravity = gravity;
	FirstUpdate = true;
}


//! Returns current vector of gravity.
core::vector3df CSceneNodeAnimatorWoWCollisionAnimator::getGravity() const
{
	return Gravity;
}


//! 'Jump' the animator, by adding a jump speed opposite to its gravity
void CSceneNodeAnimatorWoWCollisionAnimator::jump(f32 jumpSpeed)
{
	FallingVelocity -= (core::vector3df(Gravity).normalize()) * jumpSpeed;
	Falling = true;
}


//! Sets the translation of the ellipsoid for collision detection.
void CSceneNodeAnimatorWoWCollisionAnimator::setEllipsoidTranslation(const core::vector3df &translation)
{
	Translation = translation;
}


//! Returns the translation of the ellipsoid for collision detection.
core::vector3df CSceneNodeAnimatorWoWCollisionAnimator::getEllipsoidTranslation() const
{
	return Translation;
}


//! Sets a triangle selector holding all triangles of the world with which
//! the scene node may collide.
void CSceneNodeAnimatorWoWCollisionAnimator::setWorld(ITriangleSelector* newWorld)
{
	if (newWorld)
		newWorld->grab();

	if (World)
		World->drop();

	World = newWorld;

	FirstUpdate = true;
}


//! Returns the current triangle selector containing all triangles for
//! collision detection.
ITriangleSelector* CSceneNodeAnimatorWoWCollisionAnimator::getWorld() const
{
	return World;
}


void CSceneNodeAnimatorWoWCollisionAnimator::animateNode(ISceneNode* node, u32 timeMs)
{
	CollisionOccurred = false;

	if (node != Object)
		setNode(node);

	if(!Object || !World)
		return;

	// trigger reset
	if ( timeMs == 0 )
	{
		FirstUpdate = true;
		timeMs = LastTime;
	}

	if ( FirstUpdate )
	{
		LastPosition = Object->getPosition();
		Falling = false;
		LastTime = timeMs;
		FallingVelocity.set ( 0, 0, 0 );

		FirstUpdate = false;
	}

	//printf("%f, %f, %f\n", LastPosition.X, LastPosition.Y, LastPosition.Z);

	const u32 diff = timeMs - LastTime;
	LastTime = timeMs;

	CollisionResultPosition = Object->getPosition();
	core::vector3df vel = CollisionResultPosition - LastPosition;

	FallingVelocity += Gravity * (f32)diff * 0.001f;

	CollisionTriangle = RefTriangle;
	CollisionPoint = core::vector3df();
	CollisionResultPosition = core::vector3df();
	CollisionNode = 0;

	// core::vector3df force = vel + FallingVelocity;

	if ( AnimateCameraTarget )
	{
		// TODO: divide SlidingSpeed by frame time
		//printf("Fall : %f, %f, %f\n | ", FallingVelocity.X, FallingVelocity.Y, FallingVelocity.Z);
		bool f = false;
		CollisionResultPosition
			= getCollResultPosition(
				World, LastPosition-Translation,
				Radius, vel, CollisionTriangle, CollisionPoint, f,
				CollisionNode, SlidingSpeed, FallingVelocity);

		CollisionOccurred = (CollisionTriangle != RefTriangle);

		CollisionResultPosition += Translation;

		if (f)//CollisionTriangle == RefTriangle)
		{
			Falling = true;
		}
		else
		{
			Falling = false;
			FallingVelocity.set(0, 0, 0);
		}

		bool collisionConsumed = false;

		if (CollisionOccurred && CollisionCallback)
			collisionConsumed = CollisionCallback->onCollision(*this);

		if(!collisionConsumed)
			Object->setPosition(CollisionResultPosition);
	}

	// move camera target
	if (AnimateCameraTarget && IsCamera)
	{
		const core::vector3df pdiff = Object->getPosition() - LastPosition - vel;
		ICameraSceneNode* cam = (ICameraSceneNode*)Object;
		cam->setTarget(cam->getTarget() + pdiff);
	}

	LastPosition = Object->getPosition();
}


void CSceneNodeAnimatorWoWCollisionAnimator::setNode(ISceneNode* node)
{
	Object = node;

	if (Object)
	{
		LastPosition = Object->getPosition();
		IsCamera = (Object->getType() == ESNT_CAMERA);
	}

	ITimer* Timer;
	LastTime = 0;//Timer->getTime();
}


//! Writes attributes of the scene node animator.
void CSceneNodeAnimatorWoWCollisionAnimator::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{
	out->addVector3d("Radius", Radius);
	out->addVector3d("Gravity", Gravity);
	out->addVector3d("Translation", Translation);
	out->addBool("AnimateCameraTarget", AnimateCameraTarget);
}


//! Reads attributes of the scene node animator.
void CSceneNodeAnimatorWoWCollisionAnimator::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{
	Radius = in->getAttributeAsVector3d("Radius");
	Gravity = in->getAttributeAsVector3d("Gravity");
	Translation = in->getAttributeAsVector3d("Translation");
	AnimateCameraTarget = in->getAttributeAsBool("AnimateCameraTarget");
}


ISceneNodeAnimator* CSceneNodeAnimatorWoWCollisionAnimator::createClone(ISceneNode* node, ISceneManager* newManager)
{
	if (!newManager) newManager = SceneManager;

	CSceneNodeAnimatorWoWCollisionAnimator * newAnimator =
		new CSceneNodeAnimatorWoWCollisionAnimator(newManager, World, Object, Radius,
				(Gravity * 1000.0f), Translation, SlidingSpeed);

	return newAnimator;
}

void CSceneNodeAnimatorWoWCollisionAnimator::setCollisionCallback(ICollisionCallback* callback)
{
	if ( CollisionCallback == callback )
		return;

	if (CollisionCallback)
		CollisionCallback->drop();

	CollisionCallback = callback;

	if (CollisionCallback)
		CollisionCallback->grab();
}

//! Should the Target react on collision ( default = true )
void CSceneNodeAnimatorWoWCollisionAnimator::setAnimateTarget ( bool enable )
{
	AnimateCameraTarget = enable;
	FirstUpdate = true;
}

//! Should the Target react on collision ( default = true )
bool CSceneNodeAnimatorWoWCollisionAnimator::getAnimateTarget () const
{
	return AnimateCameraTarget;
}

} // end namespace scene
} // end namespace irr

