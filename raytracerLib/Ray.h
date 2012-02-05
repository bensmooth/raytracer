#pragma once

#include "Vector3D.h"

/**
 * Represents a ray with a starting position and a direction.
 */
class Ray
{
public:
	/**
	 * Initializes the ray with the position set to the origin, and the direction set to (0,1,0).
	 */
	Ray();


	/**
	 * Constructs the ray from an existing position and direction.
	 * @param position The position to start the ray at.
	 * @param direction The direction to have for the ray.
	 */
	Ray(const sivelab::Vector3D &position, const sivelab::Vector3D &direction);


	/**
	 * Calculates the position of the ray after a given time.
	 * @param t The time at which to calculate the position of the ray.
	 */
	sivelab::Vector3D GetPositionAtTime(double t) const;


	/**
	 * Gets a reference to the position of the ray.
	 */
	const sivelab::Vector3D &GetPosition() const;


	/**
	 * Gets a reference to the direction of the array.
	 */
	const sivelab::Vector3D &GetDirection() const;

private:
	sivelab::Vector3D m_position, m_direction;
};
