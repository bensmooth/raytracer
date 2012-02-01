#pragma once

#include "IShader.h"

struct Ray;

struct Intersection;


class IObject
{
public:
	/**
	 * Sees if the given ray intersects with the object.
	 * @param ray The ray to test for intersection.
	 * @param result If true is returned, this will contain the intersection data.
	 * @return True if there was an intersection, false if there was not.
	 */
	virtual bool Intersect(const Ray &ray, Intersection &result) = 0;


	/**
	 * Gets the shader associated with this object.
	 */
	virtual IShader *GetShader() = 0;
};

