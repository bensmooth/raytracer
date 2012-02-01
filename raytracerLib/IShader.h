#pragma once

#include <png++/rgb_pixel.hpp>

struct Intersection;

class IShader
{
public:
	/**
	 * Calculates the color of a ray intersection based on intersection data.
	 * @param intersection The intersection data to shade with.
	 */
	virtual png::rgb_pixel Shade(Intersection &intersection) = 0;
};

