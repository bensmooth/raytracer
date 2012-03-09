#pragma once


#include "IObject.h"


class Cylinder : public IObject
{
public:
	Cylinder(IShader *shader, sivelab::Vector3D center, double height, double radius);
	virtual ~Cylinder();
	virtual IShader* GetShader();

	virtual bool Intersect(const Ray& ray, Intersection& result);

private:
	IShader *m_shader;

	sivelab::Vector3D m_center;

	double m_height, m_radius;
};
