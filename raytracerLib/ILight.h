#pragma once

#include "Vector3D.h"
#include "Color.h"

class ILight
{
public:
	virtual ~ILight() { }


	/**
	 * Gets the position of the light.
	 */
	virtual sivelab::Vector3D GetPosition() const = 0;


	/**
	 * Gets the radiance that should be cast by the light at the given position, assuming no other objects are in the way.
	 */
	virtual Color GetRadiance(const sivelab::Vector3D &position) const = 0;
};

