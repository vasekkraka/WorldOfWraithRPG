#include <irrlicht.h>

#include "CColManager.h"

#include <Windows.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


core::array<core::triangle3df> Triangles;

inline bool getLowestRoot(f32 a, f32 b, f32 c, f32 maxR, f32* root)
{
	// check if solution exists
	const f32 determinant = b*b - 4.0f*a*c;

	// if determinant is negative, no solution
	if (determinant < 0.0f || a == 0.f )
		return false;

	// calculate two roots: (if det==0 then x1==x2
	// but lets disregard that slight optimization)

	const f32 sqrtD = sqrtf(determinant);
	const f32 invDA = core::reciprocal(2*a);
	f32 r1 = (-b - sqrtD) * invDA;
	f32 r2 = (-b + sqrtD) * invDA;

	// sort so x1 <= x2
	if (r1 > r2)
		core::swap(r1,r2);

	// get lowest root
	if (r1 > 0 && r1 < maxR)
	{
		*root = r1;
		return true;
	}

	// its possible that we want x2, this can happen if x1 < 0
	if (r2 > 0 && r2 < maxR)
	{
		*root = r2;
		return true;
	}

	return false;
}

bool testTriangleIntersection(SCollisionData* colData, const core::triangle3df& triangle)
{
	const core::plane3d<f32> trianglePlane = triangle.getPlane();

	// only check front facing polygons
	if ( !trianglePlane.isFrontFacing(colData->normalizedVelocity) )
		return false;

	// get interval of plane intersection

	f32 t1, t0;
	bool embeddedInPlane = false;

	// calculate signed distance from sphere position to triangle plane
	f32 signedDistToTrianglePlane = trianglePlane.getDistanceTo(
		colData->basePoint);

	f32 normalDotVelocity =
		trianglePlane.Normal.dotProduct(colData->velocity);

	if ( core::iszero ( normalDotVelocity ) )
	{
		// sphere is traveling parallel to plane

		if (fabs(signedDistToTrianglePlane) >= 1.0f)
			return false; // no collision possible
		else
		{
			// sphere is embedded in plane
			embeddedInPlane = true;
			t0 = 0.0;
			t1 = 1.0;
		}
	}
	else
	{
		normalDotVelocity = core::reciprocal ( normalDotVelocity );

		// N.D is not 0. Calculate intersection interval
		t0 = (-1.f - signedDistToTrianglePlane) * normalDotVelocity;
		t1 = (1.f - signedDistToTrianglePlane) * normalDotVelocity;

		// Swap so t0 < t1
		if (t0 > t1) { f32 tmp = t1; t1 = t0; t0 = tmp;	}

		// check if at least one value is within the range
		if (t0 > 1.0f || t1 < 0.0f)
			return false; // both t values are outside 1 and 0, no collision possible

		// clamp to 0 and 1
		t0 = core::clamp ( t0, 0.f, 1.f );
		t1 = core::clamp ( t1, 0.f, 1.f );
	}

	// at this point we have t0 and t1, if there is any intersection, it
	// is between this interval
	core::vector3df collisionPoint;
	bool foundCollision = false;
	f32 t = 1.0f;

	// first check the easy case: Collision within the triangle;
	// if this happens, it must be at t0 and this is when the sphere
	// rests on the front side of the triangle plane. This can only happen
	// if the sphere is not embedded in the triangle plane.

	if (!embeddedInPlane)
	{
		core::vector3df planeIntersectionPoint =
			(colData->basePoint - trianglePlane.Normal)
			+ (colData->velocity * t0);

		if (triangle.isPointInside(planeIntersectionPoint))
		{
			foundCollision = true;
			t = t0;
			collisionPoint = planeIntersectionPoint;
		}
	}

	// if we havent found a collision already we will have to sweep
	// the sphere against points and edges of the triangle. Note: A
	// collision inside the triangle will always happen before a
	// vertex or edge collision.

	if (!foundCollision)
	{
		core::vector3df velocity = colData->velocity;
		core::vector3df base = colData->basePoint;

		f32 velocitySqaredLength = velocity.getLengthSQ();
		f32 a,b,c;
		f32 newT;

		// for each edge or vertex a quadratic equation has to be solved:
		// a*t^2 + b*t + c = 0. We calculate a,b, and c for each test.

		// check against points
		a = velocitySqaredLength;

		// p1
		b = 2.0f * (velocity.dotProduct(base - triangle.pointA));
		c = (triangle.pointA-base).getLengthSQ() - 1.f;
		if (getLowestRoot(a,b,c,t, &newT))
		{
			t = newT;
			foundCollision = true;
			collisionPoint = triangle.pointA;
		}

		// p2
		if (!foundCollision)
		{
			b = 2.0f * (velocity.dotProduct(base - triangle.pointB));
			c = (triangle.pointB-base).getLengthSQ() - 1.f;
			if (getLowestRoot(a,b,c,t, &newT))
			{
				t = newT;
				foundCollision = true;
				collisionPoint = triangle.pointB;
			}
		}

		// p3
		if (!foundCollision)
		{
			b = 2.0f * (velocity.dotProduct(base - triangle.pointC));
			c = (triangle.pointC-base).getLengthSQ() - 1.f;
			if (getLowestRoot(a,b,c,t, &newT))
			{
				t = newT;
				foundCollision = true;
				collisionPoint = triangle.pointC;
			}
		}

		// check against edges:

		// p1 --- p2
		core::vector3df edge = triangle.pointB - triangle.pointA;
		core::vector3df baseToVertex = triangle.pointA - base;
		f32 edgeSqaredLength = edge.getLengthSQ();
		f32 edgeDotVelocity = edge.dotProduct(velocity);
		f32 edgeDotBaseToVertex = edge.dotProduct(baseToVertex);

		// calculate parameters for equation
		a = edgeSqaredLength* -velocitySqaredLength +
			edgeDotVelocity*edgeDotVelocity;
		b = edgeSqaredLength* (2.f *velocity.dotProduct(baseToVertex)) -
			2.0f*edgeDotVelocity*edgeDotBaseToVertex;
		c = edgeSqaredLength* (1.f -baseToVertex.getLengthSQ()) +
			edgeDotBaseToVertex*edgeDotBaseToVertex;

		// does the swept sphere collide against infinite edge?
		if (getLowestRoot(a,b,c,t,&newT))
		{
			f32 f = (edgeDotVelocity*newT - edgeDotBaseToVertex) / edgeSqaredLength;
			if (f >=0.0f && f <= 1.0f)
			{
				// intersection took place within segment
				t = newT;
				foundCollision = true;
				collisionPoint = triangle.pointA + (edge*f);
			}
		}

		// p2 --- p3
		edge = triangle.pointC-triangle.pointB;
		baseToVertex = triangle.pointB - base;
		edgeSqaredLength = edge.getLengthSQ();
		edgeDotVelocity = edge.dotProduct(velocity);
		edgeDotBaseToVertex = edge.dotProduct(baseToVertex);

		// calculate parameters for equation
		a = edgeSqaredLength* -velocitySqaredLength +
			edgeDotVelocity*edgeDotVelocity;
		b = edgeSqaredLength* (2*velocity.dotProduct(baseToVertex)) -
			2.0f*edgeDotVelocity*edgeDotBaseToVertex;
		c = edgeSqaredLength* (1-baseToVertex.getLengthSQ()) +
			edgeDotBaseToVertex*edgeDotBaseToVertex;

		// does the swept sphere collide against infinite edge?
		if (getLowestRoot(a,b,c,t,&newT))
		{
			f32 f = (edgeDotVelocity*newT-edgeDotBaseToVertex) /
				edgeSqaredLength;
			if (f >=0.0f && f <= 1.0f)
			{
				// intersection took place within segment
				t = newT;
				foundCollision = true;
				collisionPoint = triangle.pointB + (edge*f);
			}
		}


		// p3 --- p1
		edge = triangle.pointA-triangle.pointC;
		baseToVertex = triangle.pointC - base;
		edgeSqaredLength = edge.getLengthSQ();
		edgeDotVelocity = edge.dotProduct(velocity);
		edgeDotBaseToVertex = edge.dotProduct(baseToVertex);

		// calculate parameters for equation
		a = edgeSqaredLength* -velocitySqaredLength +
			edgeDotVelocity*edgeDotVelocity;
		b = edgeSqaredLength* (2*velocity.dotProduct(baseToVertex)) -
			2.0f*edgeDotVelocity*edgeDotBaseToVertex;
		c = edgeSqaredLength* (1-baseToVertex.getLengthSQ()) +
			edgeDotBaseToVertex*edgeDotBaseToVertex;

		// does the swept sphere collide against infinite edge?
		if (getLowestRoot(a,b,c,t,&newT))
		{
			f32 f = (edgeDotVelocity*newT-edgeDotBaseToVertex) /
				edgeSqaredLength;
			if (f >=0.0f && f <= 1.0f)
			{
				// intersection took place within segment
				t = newT;
				foundCollision = true;
				collisionPoint = triangle.pointC + (edge*f);
			}
		}
	}// end no collision found

	// set result:
	if (foundCollision)
	{
		// distance to collision is t
		f32 distToCollision = t*colData->velocity.getLength();

		// does this triangle qualify for closest hit?
		if (!colData->foundCollision ||
			distToCollision	< colData->nearestDistance)
		{
			colData->nearestDistance = distToCollision;
			colData->intersectionPoint = collisionPoint;
			colData->foundCollision = true;
			colData->intersectionTriangle = triangle;
			++colData->triangleHits;
			return true;
		}
	}// end found collision

	return false;
}

core::vector3df collideWithWorld(s32 recursionDepth, SCollisionData &colData, core::vector3df pos, core::vector3df vel)
{
	f32 veryCloseDistance = colData.slidingSpeed;

	//printf("Depth: %i\n", recursionDepth);

	/*SYSTEMTIME zacatek;
	GetSystemTime(&zacatek);*/





	if (recursionDepth > 5)
		return pos;

	colData.velocity = vel;
	colData.normalizedVelocity = vel;
	colData.normalizedVelocity.normalize();
	colData.basePoint = pos;
	colData.foundCollision = false;
	colData.nearestDistance = FLT_MAX;

	vector3df v1 = colData.normalizedVelocity;
	//v1.X = abs(v1.X);
	//v1.Y = abs(v1.Y);
	//v1.Z = abs(v1.Z);
	vector3df v2 = vector3df(0, -1, 0);

	f32 c = v1.dotProduct(v2.normalize());
	//f32 c = colData.normalizedVelocity.dotProduct(vector3df(0, 1, 0).normalize());

	double uhel_cos = 90 - acos(c) * 180.0 / PI;

	//printf("Uhel ^: %f\n", abs(uhel_cos));

	if (recursionDepth > 0 && vel.getLength() > 0 && vel.Y < 0)
	{
		if (abs(uhel_cos) < 50)
		{
			return pos;
		}
		else
		{
			//printf("%f, %f, %f \n", v1.X, v1.Y, v1.Z);
			//printf("Uhel ^: %f\n", abs(uhel_cos));
		}
	}


	//------------------ collide with world

	// get all triangles with which we might collide
	core::aabbox3d<f32> box(colData.R3Position);
	box.addInternalPoint(colData.R3Position + colData.R3Velocity);
	box.MinEdge -= colData.eRadius;
	box.MaxEdge += colData.eRadius;

	//printf("%f, %f, %f\n", colData.R3Position.X, colData.R3Position.Y, colData.R3Position.Z);

	//printf("%f, %f, %f\n", colData.R3Velocity.X, colData.R3Velocity.Y, colData.R3Velocity.Z);

	s32 totalTriangleCnt = colData.selector->getTriangleCount();
	Triangles.set_used(totalTriangleCnt);

	core::matrix4 scaleMatrix;
	scaleMatrix.setScale(
			core::vector3df(1.0f / colData.eRadius.X,
					1.0f / colData.eRadius.Y,
					1.0f / colData.eRadius.Z));

	s32 triangleCnt = 0;

	//SYSTEMTIME zacatek;
	//GetSystemTime(&zacatek);

	colData.selector->getTriangles(Triangles.pointer(), totalTriangleCnt, triangleCnt, box, &scaleMatrix);

	/*SYSTEMTIME konec;
	GetSystemTime(&konec);
	printf("Cas: %f, %i", (double)((konec.wMilliseconds - zacatek.wMilliseconds)), triangleCnt);*/

	for (s32 i=0; i<triangleCnt; ++i)
		if(testTriangleIntersection(&colData, Triangles[i]))
			colData.triangleIndex = i;

	//SYSTEMTIME konec;
	//GetSystemTime(&konec);
	//printf("Cas: %f, %i, %i\n", (double)((konec.wMilliseconds - zacatek.wMilliseconds)), triangleCnt, recursionDepth);

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
	
	/*SYSTEMTIME konec;
	GetSystemTime(&konec);
	printf("Cas: %f\n", (double)((konec.wMilliseconds - zacatek.wMilliseconds)));*/

	

	//printf("Recurs Depth : %i | %i\n", recursionDepth, Triangles.size());
	

	if (newVelocityVector.getLength() < veryCloseDistance)
		return newBasePoint;

	return collideWithWorld(recursionDepth+1, colData,
		newBasePoint, newVelocityVector);
}

core::vector3df collideWithWorldNoSliding(s32 recursionDepth, SCollisionData &colData, core::vector3df pos, core::vector3df vel)
{
	f32 veryCloseDistance = colData.slidingSpeed;

	/*SYSTEMTIME zacatek;
	GetSystemTime(&zacatek);*/

	if (recursionDepth > 5)
		return pos;

	colData.velocity = vel;

	//printf("%f, %f, %f\n | ", vel.X, vel.Y, vel.Z);

	colData.normalizedVelocity = vel;
	colData.normalizedVelocity.normalize();
	//printf("%f, %f, %f\n | ", colData.normalizedVelocity.X, colData.normalizedVelocity.Y, colData.normalizedVelocity.Z);
	colData.basePoint = pos;
	colData.foundCollision = false;
	colData.nearestDistance = FLT_MAX;

	if (vel.getLength() > 0 && vel.Y < 0)
	{

		f32 c = colData.normalizedVelocity.dotProduct(vector3df(1, 0, 0).normalize());

		double uhel_cos = 90 - acos(c) * 180.0 / PI;
		//printf("Uhel dolu: %f\n", uhel_cos);
	}

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

	/*SYSTEMTIME zacatek;
	GetSystemTime(&zacatek);*/

	//printf("Max : %i\n", totalTriangleCnt);

	colData.selector->getTriangles(Triangles.pointer(), totalTriangleCnt, triangleCnt, box, &scaleMatrix);

	//printf("Box : %i\n", triangleCnt);

	/*SYSTEMTIME konec;
	GetSystemTime(&konec);*/
	//printf("Cas: %f, %i\n", (double)((konec.wMilliseconds - zacatek.wMilliseconds)), triangleCnt);
	//SYSTEMTIME zacatek;
	//GetSystemTime(&zacatek);
	for (s32 i=0; i<triangleCnt; ++i)
		if(testTriangleIntersection(&colData, Triangles[i]))
			colData.triangleIndex = i;

	//SYSTEMTIME konec;
	//GetSystemTime(&konec);
	//printf("Cas: %f, %i, %i\n", (double)((konec.wMilliseconds - zacatek.wMilliseconds)), triangleCnt, recursionDepth);

	//---------------- end collide with world

	if (!colData.foundCollision)
	{
		return pos + vel;
	}

	return pos;

	// original destination point
	const core::vector3df destinationPoint = pos + vel;
	core::vector3df newBasePoint = pos;

	// only update if we are not already very close
	// and if so only move very close to intersection, not to the
	// exact point


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
	
	/*SYSTEMTIME konec;
	GetSystemTime(&konec);
	printf("Cas: %f\n", (double)((konec.wMilliseconds - zacatek.wMilliseconds)));*/

	

	//printf("Recurs Depth : %i | %i\n", recursionDepth, Triangles.size());
	

	if (newVelocityVector.getLength() < veryCloseDistance)
	{
		return newBasePoint;
	}

	return collideWithWorld(recursionDepth+1, colData,
		newBasePoint, newVelocityVector);
}


//! Collides a moving ellipsoid with a 3d world with gravity and returns
//! the resulting new position of the ellipsoid.
core::vector3df collideEllipsoidWithWorld(
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


//! Collides a moving ellipsoid with a 3d world with gravity and returns
//! the resulting new position of the ellipsoid.
core::vector3df getCollResultPosition(
		ITriangleSelector* selector,
		const core::vector3df &position, const core::vector3df& radius,
		const core::vector3df& direction, // move velocity
		core::triangle3df& triout,
		core::vector3df& hitPosition,
		bool& outFalling,
		ISceneNode*& outNode,
		f32 slidingSpeed,
		const core::vector3df& gravity) // fall velocity
{

	//printf("%f, %f, %f\n | ", gravity.X, gravity.Y, gravity.Z);

	return collideEllipsoidWithWorld(selector, position,
		radius, direction, slidingSpeed, gravity, triout, hitPosition, outFalling, outNode);
}


