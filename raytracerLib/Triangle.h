#pragma once

#include "IObject.h"
#include "Intersection.h"
#include "Vector3D.h"

class Triangle : public IObject
{
public:
	/**
	 * Constructs a triangle from the given points.
	 */
	Triangle(const sivelab::Vector3D &v1, const sivelab::Vector3D &v2, const sivelab::Vector3D &v3, IShader *shader);

	virtual IShader* GetShader();

	virtual bool Intersect(const Ray& ray, Intersection& result);

	virtual BBox GetBoundingBox();

	/**
	 * The number of vertices in a triangle.
	 */
	static const int VERTEX_COUNT = 3;

private:
	/**
	 * Array of vertices.
	 */
	sivelab::Vector3D m_vertices[VERTEX_COUNT];

	/**
	 * The normal of the vertex.
	 */
	sivelab::Vector3D m_normal;

	/**
	 * The shader associated with this object.
	 */
	IShader *m_shader;
};
