#pragma once

#include "Vector3D.h"

using namespace sivelab;

/**
 * This class represents an orthonormal basis.
 */
class Basis
{
public:
	/**
	 * Constructs a basis from a vector.
	 * @param a The vector to construct a basis from.
	 */
	Basis(const Vector3D &a);

	const Vector3D &GetU();
	const Vector3D &GetV();
	const Vector3D &GetW();
private:
	Vector3D m_u, m_v, m_w;
};

