#pragma once

#include <limits>

#include "Vector3D.h"
#include "Ray.h"
#include "JitteredSampler.h"

class IObject;

/**
 * This structure represents an intersection.
 * It should contain enough information to shade the object that was collided with.
 */
struct Intersection
{
	Intersection()
	{
		// Do 7 reflections before giving up.
		allowedReflectionCount = 7;
		t = 0.0;
		object = NULL;
		
	}

	/**
	 * The ray that collided with the object.
	 */
	Ray collidedRay;

	/**
	 * The time t that the ray collided with the object at.
	 * If the object was intersected multiple times by the ray,
	 * the t value should correspond to the intersection with the smallest t value.
	 */
	double t;

	/**
	 * The list of area light samples.
	 * Generate must be called if area lights are present in the scene.
	 */
	JitteredSampler areaLightSamples;

	/**
	 * The normal of the surface where the ray hit.
	 */
	sivelab::Vector3D surfaceNormal;

	/**
	 * A reference to the object that was intersected.
	 */
	IObject *object;

	/**
	 * The number of reflections that are left for this particular intersection.
	 * Scene::CastReflectionRay() will decrement this every time it is called.
	 * When 0, Scene::CastReflectionRay() will not cast the reflection ray, and instead simply return the background color.
	 */
	int allowedReflectionCount;
};

