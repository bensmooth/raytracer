#include "Triangle.h"
#include <vector>
#include "BBox.h"


Triangle::Triangle(const sivelab::Vector3D& v1, const sivelab::Vector3D& v2, const sivelab::Vector3D& v3, IShader *shader)
{
	m_vertices[0] = v1;
	m_vertices[1] = v2;
	m_vertices[2] = v3;

	// Calculate the normal.  Valid for CCW vertex ordering.
	sivelab::Vector3D normal = (v2 - v1).cross(v3 - v1);
	normal.normalize();
	m_normal[0] = normal;
	m_normal[1] = normal;
	m_normal[2] = normal;

	m_shader = shader;
}


Triangle::Triangle(sivelab::Vector3D vertices[3], sivelab::Vector3D normals[3], IShader* shader)
{
	m_shader = shader;

	for (int i = 0; i < VERTEX_COUNT; i++)
	{
		m_vertices[i] = vertices[i];
		m_normal[i] = normals[i];
	}
}


IShader* Triangle::GetShader()
{
	return m_shader;
}


BBox Triangle::GetBoundingBox()
{
	std::vector<sivelab::Vector3D> points;
	points.resize(VERTEX_COUNT);

	// Add each point in the triangle to the list of points.
	for (int i = 0; i < VERTEX_COUNT; i++)
	{
		points[i] = m_vertices[i];
	}

	return (BBox::MakeFromPoints(points));
}


/**
 * Calculates the distance between two points.
 */
double GetDist(const sivelab::Vector3D &v0, const sivelab::Vector3D &v1)
{
	double dx = v0[0] - v1[0];
	double dy = v0[1] - v1[1];
	double dz = v0[2] - v1[2];

	return sqrt(dx*dx + dy*dy + dz*dz);
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
	result.collidedRay = ray;
	result.t = t;
	result.object = this;

	// Interpolate between all three normals.
	sivelab::Vector3D intersectPoint = ray.GetPositionAtTime(t);
	result.surfaceNormal.set(0.0, 0.0, 0.0);
	for (int i = 0; i < VERTEX_COUNT; i++)
	{
		// Calculate distance between this vertex and the intersection point.
		double dist = GetDist(intersectPoint, m_vertices[i]);
		result.surfaceNormal += m_normal[i] * (1.0/dist);
	}
	result.surfaceNormal.normalize();

	return (true);
}

