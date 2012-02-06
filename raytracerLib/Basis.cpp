#include "Basis.h"


Basis::Basis()
{
}

void Basis::Calculate(const sivelab::Vector3D& a)
{
	// Set m_w to be the opposite direction of a.
	m_w = a * -1;
	m_w.normalize();

	// Calculate m_u.
	m_u = m_w;
	// Find smallest magnitude component, and set it to 1.
	if ((m_u[0] < m_u[1]) && ((m_u[0] < m_u[2])))
	{
		m_u[0] = 1.0;
	}
	else if ((m_u[1] < m_u[0]) && ((m_u[1] < m_u[2])))
	{
		m_u[1] = 1.0;
	}
	else
	{
		// Either the 3rd component is the smallest, or all of the components are equal.
		m_u[2] = 1.0;
	}
	
	m_u = m_u.cross(m_w);
	m_u.normalize();

	// Calculate m_v.
	m_v = m_w.cross(m_u);
}


void Basis::Calculate(const sivelab::Vector3D& a, const sivelab::Vector3D& b)
{
	// Set m_w to be the opposite direction of a.
	m_w = a * -1;
	m_w.normalize();

	m_u = b.cross(m_w);
	m_u.normalize();

	m_v = m_w.cross(m_u);
}


const sivelab::Vector3D& Basis::GetU() const
{
	return(m_u);
}


const sivelab::Vector3D& Basis::GetV() const
{
	return(m_v);
}


const sivelab::Vector3D& Basis::GetW() const
{
	return(m_w);
}

