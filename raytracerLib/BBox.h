#pragma once

#include "Vector3D.h"
#include <vector>

class Ray;

/**
 * Represents a bounding box.
 */
struct BBox
{
	/**
	 * Constructs a bounding box that contains the given set of points.
	 */
	static BBox MakeFromPoints(const std::vector<sivelab::Vector3D> &points);

	/**
	 * Constructs a bounding box that contains the given set of boxes.
	 */
	static BBox Combine(const std::vector<BBox> &boxes);

	/**
	 * Returns true if the ray intersects the bounding box.
	 */
	bool Intersects(const Ray &ray) const;

	/**
	 * The minimum and maximum points of the box.
	 */
	sivelab::Vector3D minPt, maxPt;
};
