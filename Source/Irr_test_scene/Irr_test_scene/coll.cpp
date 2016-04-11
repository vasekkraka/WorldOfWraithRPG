//! Creates a special scene node animator for doing automatic collision detection
//! and response.
#include <irrlicht.h>
using namespace irr;
using namespace scene;

ISceneNodeAnimatorCollisionResponse* CSceneManager::createCollisionResponseAnimator(
	ITriangleSelector* world, ISceneNode* sceneNode, const core::vector3df& ellipsoidRadius,
	const core::vector3df& gravityPerSecond,
	const core::vector3df& ellipsoidTranslation, f32 slidingValue)
{
	ISceneNodeAnimatorCollisionResponse* anim = new
		CSceneNodeAnimatorCollisionResponse(this, world, sceneNode,
			ellipsoidRadius, gravityPerSecond,
			ellipsoidTranslation, slidingValue);

	return anim;
}

//! constructor
CSceneNodeAnimatorCollisionResponse::CSceneNodeAnimatorCollisionResponse(
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
	setDebugName("CSceneNodeAnimatorCollisionResponse");
	#endif

	if (World)
		World->grab();

	setNode(Object);
}

void CSceneNodeAnimatorCollisionResponse::animateNode(ISceneNode* node, u32 timeMs)
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

		bool f = false;
		CollisionResultPosition
			= SceneManager->getSceneCollisionManager()->getCollisionResultPosition(
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

//! Collides a moving ellipsoid with a 3d world with gravity and returns
//! the resulting new position of the ellipsoid.
core::vector3df CSceneCollisionManager::getCollisionResultPosition(
		ITriangleSelector* selector,
		const core::vector3df &position, const core::vector3df& radius,
		const core::vector3df& direction,
		core::triangle3df& triout,
		core::vector3df& hitPosition,
		bool& outFalling,
		ISceneNode*& outNode,
		f32 slidingSpeed,
		const core::vector3df& gravity)
{
	return collideEllipsoidWithWorld(selector, position,
		radius, direction, slidingSpeed, gravity, triout, hitPosition, outFalling, outNode);
}

//! Collides a moving ellipsoid with a 3d world with gravity and returns
//! the resulting new position of the ellipsoid.
core::vector3df CSceneCollisionManager::collideEllipsoidWithWorld(
		ITriangleSelector* selector, const core::vector3df &position,
		const core::vector3df& radius,  const core::vector3df& velocity,
		f32 slidingSpeed,
		const core::vector3df& gravity,
		core::triangle3df& triout,
		core::vector3df& hitPosition,
		bool& outFalling,
		ISceneNode*& outNode)
{
	if (!selector || radius.X == 0.0f || radius.Y == 0.0f || radius.Z == 0.0f)
		return position;

	// This code is based on the paper "Improved Collision detection and Response"
	// by Kasper Fauerby, but some parts are modified.

	SCollisionData colData;
	colData.R3Position = position;
	colData.R3Velocity = velocity;
	colData.eRadius = radius;
	colData.nearestDistance = FLT_MAX;
	colData.selector = selector;
	colData.slidingSpeed = slidingSpeed;
	colData.triangleHits = 0;
	colData.triangleIndex = -1;

	core::vector3df eSpacePosition = colData.R3Position / colData.eRadius;
	core::vector3df eSpaceVelocity = colData.R3Velocity / colData.eRadius;

	// iterate until we have our final position

	core::vector3df finalPos = collideWithWorld(
		0, colData, eSpacePosition, eSpaceVelocity);

	outFalling = false;

	// add gravity

	if (gravity != core::vector3df(0,0,0))
	{
		colData.R3Position = finalPos * colData.eRadius;
		colData.R3Velocity = gravity;
		colData.triangleHits = 0;

		eSpaceVelocity = gravity/colData.eRadius;

		finalPos = collideWithWorld(0, colData,
			finalPos, eSpaceVelocity);

		outFalling = (colData.triangleHits == 0);
	}

	if (colData.triangleHits)
	{
		triout = colData.intersectionTriangle;
		triout.pointA *= colData.eRadius;
		triout.pointB *= colData.eRadius;
		triout.pointC *= colData.eRadius;
		outNode = selector->getSceneNodeForTriangle(colData.triangleIndex);
	}

	finalPos *= colData.eRadius;
	hitPosition = colData.intersectionPoint * colData.eRadius;
	return finalPos;
}

core::vector3df CSceneCollisionManager::collideWithWorld(s32 recursionDepth,
	SCollisionData &colData, core::vector3df pos, core::vector3df vel)
{
	f32 veryCloseDistance = colData.slidingSpeed;

	if (recursionDepth > 5)
		return pos;

	colData.velocity = vel;
	colData.normalizedVelocity = vel;
	colData.normalizedVelocity.normalize();
	colData.basePoint = pos;
	colData.foundCollision = false;
	colData.nearestDistance = FLT_MAX;

	//------------------ collide with world

	// get all triangles with which we might collide
	core::aabbox3d<f32> box(colData.R3Position);
	box.addInternalPoint(colData.R3Position + colData.R3Velocity);
	box.MinEdge -= colData.eRadius;
	box.MaxEdge += colData.eRadius;

	s32 totalTriangleCnt = colData.selector->getTriangleCount();
	Triangles.set_used(totalTriangleCnt);

	core::matrix4 scaleMatrix;
	scaleMatrix.setScale(
			core::vector3df(1.0f / colData.eRadius.X,
					1.0f / colData.eRadius.Y,
					1.0f / colData.eRadius.Z));

	s32 triangleCnt = 0;
	colData.selector->getTriangles(Triangles.pointer(), totalTriangleCnt, triangleCnt, box, &scaleMatrix);

	for (s32 i=0; i<triangleCnt; ++i)
		if(testTriangleIntersection(&colData, Triangles[i]))
			colData.triangleIndex = i;

	//---------------- end collide with world

	if (!colData.foundCollision)
		return pos + vel;

	// original destination point
	const core::vector3df destinationPoint = pos + vel;
	core::vector3df newBasePoint = pos;

	// only update if we are not already very close
	// and if so only move very close to intersection, not to the
	// exact point
	if (colData.nearestDistance >= veryCloseDistance)
	{
		core::vector3df v = vel;
		v.setLength( colData.nearestDistance - veryCloseDistance );
		newBasePoint = colData.basePoint + v;

		v.normalize();
		colData.intersectionPoint -= (v * veryCloseDistance);
	}

	// calculate sliding plane

	const core::vector3df slidePlaneOrigin = colData.intersectionPoint;
	const core::vector3df slidePlaneNormal = (newBasePoint - colData.intersectionPoint).normalize();
	core::plane3d<f32> slidingPlane(slidePlaneOrigin, slidePlaneNormal);

	core::vector3df newDestinationPoint =
		destinationPoint -
		(slidePlaneNormal * slidingPlane.getDistanceTo(destinationPoint));

	// generate slide vector

	const core::vector3df newVelocityVector = newDestinationPoint -
		colData.intersectionPoint;

	if (newVelocityVector.getLength() < veryCloseDistance)
		return newBasePoint;

	return collideWithWorld(recursionDepth+1, colData,
		newBasePoint, newVelocityVector);
}