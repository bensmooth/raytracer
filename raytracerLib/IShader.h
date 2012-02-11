#pragma once

#include "Color.h"

struct Intersection;

class IShader
{
public:
	~IShader() { }


	/**
	 * Calculates the color of a ray intersection based on intersection data.
	 * @param intersection The intersection data to shade with.
	 */
	virtual Color Shade(Intersection &intersection) = 0;
};

