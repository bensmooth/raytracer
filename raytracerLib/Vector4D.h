#pragma once
#include "Vector3D.h"

class Matrix;

/**
 * A 4D vector.
 */
struct Vector4D
{
	/**
	 * Sets all components to 0.
	 */
	Vector4D();

	/**
	 * Constructs a 4D vector with the given 3D vector and information about if the vector contains a position.
	 */
	Vector4D(const sivelab::Vector3D &inVector3d, bool vectorIsPosition);

	/**
	 * Constructs a 4D vector with the given 3D vector and w value.
	 */
	Vector4D(const sivelab::Vector3D &inVector3d, double inW);

	/**
	 * Constructs a 4D vector with the given set of values.
	 */
	Vector4D(double const values[4]);
	Vector4D(double v0, double v1, double v2, double v3);

	/**
	 * The first three components of the vector.
	 */
	sivelab::Vector3D vector3d;

	/**
	 * The final component of the vector.
	 */
	double w;

	/**
	 * Multiplies the vector, as a row vector, with a matrix.
	 */
	Vector4D operator*(const Matrix &other) const;

	/**
	 * Gets a string representation of the vector.
	 */
	std::string ToString() const;
};
