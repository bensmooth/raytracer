#include "PointLight.h"


PointLight::PointLight(const sivelab::Vector3D& position, const Color& radiance)
{
	m_position = position;
	m_radiance = radiance;
}


sivelab::Vector3D PointLight::GetPosition()
{
    return (m_position);
}


Color PointLight::GetRadiance(const sivelab::Vector3D& position)
{
	return (m_radiance);
}

