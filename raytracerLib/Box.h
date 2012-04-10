#pragma once

#include "IObject.h"
#include "Vector3D.h"
#include "Triangle.h"
#include "BBox.h"

/**
 * This is an axis-aligned box.
 */
class Box : public IObject
{
public:
	/**
	 * Constructs a box with the given min and max points, and the given shader.
	 */
	Box(const sivelab::Vector3D &minPoint, const sivelab::Vector3D &maxPoint, IShader *shader);

	~Box();

	virtual IShader* GetShader();

	virtual bool Intersect(const Ray& ray, Intersection& result);

	virtual BBox GetBoundingBox();

	/**
	 * Sets the min and max points of the box.
	 */
	void ConstructBox(const sivelab::Vector3D &minPoint, const sivelab::Vector3D &maxPoint);

private:

	/**
	 * The shader used for this box.
	 */
	IShader *m_shader;

	/**
	 * The number of triangles in a box.
	 */
	static const int TRIANGLES_IN_A_BOX = 12;

	/**
	 * The triangles that make up the box.
	 */
	Triangle *m_triangles[TRIANGLES_IN_A_BOX];

	/**
	 * The bounding box.
	 */
	BBox m_bbox;
};

