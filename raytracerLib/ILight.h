#pragma once

#include "Vector3D.h"
#include <png++/rgb_pixel.hpp>


class ILight
{
public:
	~ILight() { }


	/**
	 * Gets the radiance that should be cast by the light at the given position, assuming no other objects are in the way.
	 */
	virtual png::rgb_pixel GetRadiance(const sivelab::Vector3D &position) = 0;
};

