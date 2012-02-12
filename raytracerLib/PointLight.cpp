#include "PointLight.h"


PointLight::PointLight(const sivelab::Vector3D& position, const Color& radiance)
{
	m_position = position;
	m_radiance = radiance;
}


sivelab::Vector3D PointLight::GetPosition() const
{
    return (m_position);
}


Color PointLight::GetRadiance(const sivelab::Vector3D& position) const
{
	return (m_radiance);
}

