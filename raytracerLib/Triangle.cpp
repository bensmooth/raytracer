#include "Triangle.h"


Triangle::Triangle(const sivelab::Vector3D& v1, const sivelab::Vector3D& v2, const sivelab::Vector3D& v3, IShader *shader)
{
	m_vertices[0] = v1;
	m_vertices[1] = v2;
	m_vertices[2] = v3;

	// Calculate the normal.  Valid for CCW vertex ordering.
	m_normal = (v2 - v1).cross(v3 - v1);
	m_normal.normalize();

	m_shader = shader;
}


IShader* Triangle::GetShader()
{
	return m_shader;
}


bool Triangle::Intersect(const Ray& ray, Intersection& result)
{
	double xa = m_vertices[0][0];
	double xb = m_vertices[1][0];
	double xc = m_vertices[2][0];

	double ya = m_vertices[0][1];
	double yb = m_vertices[1][1];
	double yc = m_vertices[2][1];

	double za = m_vertices[0][2];
	double zb = m_vertices[1][2];
	double zc = m_vertices[2][2];

	double a = xa - xb;
	double b = ya - yb;
	double c = za - zb;
	double d = xa - xc;
	double e = ya - yc;
	double f = za - zc;
	double g = ray.GetDirection()[0];
	double h = ray.GetDirection()[1];
	double i = ray.GetDirection()[2];
	double j = xa - ray.GetPosition()[0];
	double k = ya - ray.GetPosition()[1];
	double l = za - ray.GetPosition()[2];

	double M = a*(e*i - h*f) + b*(g*f - d*i) + c*(d*h - e*g);

	// Compute t.
	double t = f*(a*k - j*b) + e*(j*c - a*l) + d*(b*l - c*k);
	t /= -M;

	// Compute gamma.
	double gamma = i*(a*k - j*b) + h*(j*c - a*l) + g*(b*l - c*k);
	gamma /= M;

	// See if we are where we are supposed to be.
	if ((gamma < 0.0) || (gamma > 1.0))
	{
		return (false);
	}

	// Calculate beta.
	double beta = j*(e*i - h*f) + k*(g*f - d*i) + l*(d*h - e*g);
	beta /= M;

	// See if we are where we are supposed to be.
	if ((beta < 0.0) || (beta > (1.0 - gamma)))
	{
		return (false);
	}

	// If we got here, we had an instersection.
	result.collidedWith = ray;
	result.t = t;
	result.object = this;
	result.surfaceNormal = m_normal;

	return (true);
}

