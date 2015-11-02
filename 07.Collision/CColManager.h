#include <irrlicht.h>

#include <time.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

struct SCollisionData
		{
			core::vector3df eRadius;

			core::vector3df R3Velocity;
			core::vector3df R3Position;

			core::vector3df velocity;
			core::vector3df normalizedVelocity;
			core::vector3df basePoint;

			bool foundCollision;
			f32 nearestDistance;
			core::vector3df intersectionPoint;

			core::triangle3df intersectionTriangle;
			s32 triangleIndex;
			s32 triangleHits;

			f32 slidingSpeed;

			ITriangleSelector* selector;
		};

inline bool getLowestRoot(f32 a, f32 b, f32 c, f32 maxR, f32* root);

bool testTriangleIntersection(SCollisionData* colData, const core::triangle3df& triangle);

core::vector3df collideWithWorld(s32 recursionDepth, SCollisionData &colData, core::vector3df pos, core::vector3df vel);
core::vector3df collideWithWorldNoSliding(s32 recursionDepth, SCollisionData &colData, core::vector3df pos, core::vector3df vel);



core::vector3df collideEllipsoidWithWorld(
		ITriangleSelector* selector, const core::vector3df &position,
		const core::vector3df& radius,  const core::vector3df& velocity,
		f32 slidingSpeed,
		const core::vector3df& gravity,
		core::triangle3df& triout,
		core::vector3df& hitPosition,
		bool& outFalling,
		ISceneNode*& outNode);

core::vector3df getCollResultPosition(
		ITriangleSelector* selector,
		const core::vector3df &position, const core::vector3df& radius,
		const core::vector3df& direction,
		core::triangle3df& triout,
		core::vector3df& hitPosition,
		bool& outFalling,
		ISceneNode*& outNode,
		f32 slidingSpeed,
		const core::vector3df& gravity);