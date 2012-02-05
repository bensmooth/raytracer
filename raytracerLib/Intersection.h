#pragma once

#include "Vector3D.h"
#include "Ray.h"

class IObject;

/**
 * This structure represents an intersection.
 * It should contain enough information to shade the object that was collided with.
 */
struct Intersection
{
	/**
	 * The ray that collided with the object.
	 */
	Ray collidedWith;

	/**
	 * The time t that the ray collided with the object at.
	 * If the object was intersected multiple times by the ray,
	 * the t value should correspond to the intersection with the smallest t value.
	 */
	double t;

	/**
	 * The normal of the surface where the ray hit.
	 */
	sivelab::Vector3D surfaceNormal;

	/**
	 * A reference to the object that was intersected.
	 */
	IObject *object;
};

