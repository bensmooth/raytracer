#pragma once

#include "ILight.h"
#include "Color.h"
#include "Vector3D.h"


class PointLight : public ILight
{
public:
	/**
	 * Constructs a light at the given position with the given radiance.
	 */
	PointLight(const sivelab::Vector3D &position, const Color &radiance);


	virtual sivelab::Vector3D GetPosition() const;


	virtual Color GetRadiance(const sivelab::Vector3D& position) const;

private:
	Color m_radiance;

	sivelab::Vector3D m_position;
};
