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
	 * Sets all vectors to the zero vector.
	 */
	Basis();

	/**
	 * Constructs a basis from a single vector.
	 * @param a The vector to construct a basis from.
	 */
	void Calculate(const Vector3D &a);

	/**
	 * Constructs a basis from two vectors.
	 */
	void Calculate(const Vector3D &a, const Vector3D &b);

	const Vector3D &GetU() const;
	const Vector3D &GetV() const;
	const Vector3D &GetW() const;
private:
	Vector3D m_u, m_v, m_w;
};

