#include "Ray.h"

Ray::Ray()
{
	m_position[0] = 0.0;
	m_position[1] = 0.0;
	m_position[2] = 0.0;
	
	m_direction[0] = 0.0;
	m_direction[1] = 1.0;
	m_direction[2] = 0.0;
}


Ray::Ray(const sivelab::Vector3D& position, const sivelab::Vector3D& direction)
{
	m_position = position;
	m_direction = direction;
}


Ray::Ray(const Ray& other)
{
	m_position = other.m_position;
	m_direction = other.m_direction;
}


const sivelab::Vector3D& Ray::GetDirection() const
{
	return (m_direction);
}


const sivelab::Vector3D& Ray::GetPosition() const
{
	return (m_position);
}


sivelab::Vector3D Ray::GetPositionAtTime(double t) const
{
	return ((t * m_direction) + m_position);
}


void Ray::SetPosition(const sivelab::Vector3D& position)
{
	m_position = position;
}


void Ray::SetDirection(const sivelab::Vector3D& direction)
{
	m_direction = direction;
}

