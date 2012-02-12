#include <algorithm>

#include "Sphere.h"
#include "Vector3D.h"
#include "Ray.h"

using namespace sivelab;

Sphere::Sphere(sivelab::Vector3D& center, double radius, IShader* shader)
{
	m_center = center;
	m_radius = radius;
	m_shader = shader;
}


IShader* Sphere::GetShader()
{
	return (m_shader);
}


bool Sphere::Intersect(const Ray& ray, Intersection& result)
{
	const Vector3D &rayPos = ray.GetPosition();
	const Vector3D &rayDir = ray.GetDirection();
	Vector3D offset = rayPos - m_center;
	double a = rayDir.dot(rayDir);
	double b = (2 * rayDir).dot(offset);
	double c = offset.dot(offset) - m_radius * m_radius;

	double descriminant = b * b - 4 * a * c;
	if (descriminant < 0)
	{
		// No intersections.
		return (false);
	}
	else
	{
		// We have an intersection somewhere.
		double t1 = (-b + sqrt(descriminant)) / (2 * a);
		double t2 = (-b - sqrt(descriminant)) / (2 * a);

		// Record intersection.
		result.t = std::min(t1, t2);
		result.collidedRay = ray;
		result.object = this;

		// Calculate surface normal.
		result.surfaceNormal = ray.GetPositionAtTime(result.t) - m_center;
		result.surfaceNormal.normalize();

		return (true);
	}
}

