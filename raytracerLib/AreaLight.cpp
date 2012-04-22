#include "AreaLight.h"
#include "Basis.h"

AreaLight::AreaLight(const sivelab::Vector3D& position, const sivelab::Vector3D& normal, const Color& intensity, double width, double height)
{
	m_position = position;
	m_normal = normal;
	m_intensity = intensity;
	m_width = width;
	m_height = height;
}


AreaLight::~AreaLight()
{
}


sivelab::Vector3D AreaLight::GetPosition() const
{
	return (m_position);
}


sivelab::Vector3D AreaLight::GetPosition(const Sample& sample) const
{
	// m_position is the center of the box.
	Basis basis;
	basis.Calculate(m_normal);

	// Basis.w is the opposite direction as the normal.
	// We don't want to deviate from the light plane at all, so it is zero.
	// Pick u as the width.
	double u = m_width * (sample.first - 0.5);
	Vector3D uu = u * basis.GetU();

	// Pick v as the height.
	double v = m_height * (sample.second - 0.5);
	Vector3D vv = v * basis.GetV();

	return (m_position + uu + vv);
}


Color AreaLight::GetRadiance(const sivelab::Vector3D& position) const
{
	return (m_intensity);
}

