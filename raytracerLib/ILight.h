#pragma once

#include "Vector3D.h"


class ILight
{
public:
	/**
	 * Gets the radiance that should be cast by the light at the given position, assuming no other objects are in the way.
	 */
	virtual Color GetRadiance(const sivelab::Vector3D &position) = 0;
};

