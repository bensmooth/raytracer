#pragma once

#include "Vector3D.h"
#include "IObject.h"
#include "Intersection.h"

class IShader;
class Ray;

class Sphere : public IObject
{
public:
	/**
	 * Constructs the object.
	 */
	Sphere(sivelab::Vector3D &center, double radius, IShader *shader);

    virtual IShader* GetShader();

    virtual bool Intersect(const Ray& ray, Intersection& result);

private:
	sivelab::Vector3D m_center;

	double m_radius;

	IShader *m_shader;
};
