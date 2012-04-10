#include <limits>
#include <string.h>

#include "Box.h"
#include "BBox.h"

using namespace sivelab;

Box::Box(const sivelab::Vector3D& minPoint, const sivelab::Vector3D& maxPoint, IShader* shader)
{
	m_shader = shader;

	memset(m_triangles, 0, TRIANGLES_IN_A_BOX * sizeof(Triangle*));
	ConstructBox(minPoint, maxPoint);
}


Box::~Box()
{
	// Free all triangles.
	for (int i = 0; i < TRIANGLES_IN_A_BOX; i++)
	{
		delete m_triangles[i];
		m_triangles[i] = NULL;
	}
}


IShader* Box::GetShader()
{
	return (m_shader);
}


BBox Box::GetBoundingBox()
{
	return (m_bbox);
}


bool Box::Intersect(const Ray& ray, Intersection& result)
{
	// See if the ray even hits at all.
	if (m_bbox.Intersects(ray) == false)
	{
		return (false);
	}

	// We need to test for more than one interestion.
	double maxDouble = std::numeric_limits<double>::max();
	result.t = maxDouble;
	Intersection intersect;

	for (int i = 0; i < TRIANGLES_IN_A_BOX; i++)
	{
		if (m_triangles[i]->Intersect(ray, intersect) == true)
		{
			if (intersect.t < result.t)
			{
				result = intersect;
			}
		}
	}

	// See if there was a collision.
	if (result.t < maxDouble)
	{
		result.object = this;
		return (true);
	}
	else
	{
		// If we got here, we did not have an intersection.
		return (false);
	}
}


void Box::ConstructBox(const sivelab::Vector3D& minPoint, const sivelab::Vector3D& maxPoint)
{
	// Save minimum and maximum points into bounding box.
	m_bbox.minPt = minPoint;
	m_bbox.maxPt = maxPoint;

	// Free all triangles.
	for (int i = 0; i < TRIANGLES_IN_A_BOX; i++)
	{
		delete m_triangles[i];
		m_triangles[i] = NULL;
	}

	// Calculate box dimensions.
	double dx = maxPoint[0] - minPoint[0];
	double dy = maxPoint[1] - minPoint[1];
	double dz = maxPoint[2] - minPoint[2];

	// Construct vectors that correspond to the vertices of the box.
	Vector3D a(0, 0, 0);	// Bottom back left.
	Vector3D b(0, dy, dz);	// Top front left.
	Vector3D c(0, dy, 0);	// Top back left.
	Vector3D d(dx, dy, 0);	// Top back right.
	Vector3D e(dx, 0, 0);	// Bottom back right.
	Vector3D f(0, 0, dz);	// Bottom front left.
	Vector3D g(dx, 0, dz);	// Bottom front right.
	Vector3D h(dx, dy, dz);	// Top front right.

	// Add offset from origin to each vertex.
	a += minPoint;
	b += minPoint;
	c += minPoint;
	d += minPoint;
	e += minPoint;
	f += minPoint;
	g += minPoint;
	h += minPoint;

	// Add triangles, with points in CCW winding.
	m_triangles[0] = new Triangle(a, b, c, m_shader);
	m_triangles[1] = new Triangle(a, c, d, m_shader);
	m_triangles[2] = new Triangle(a, d, e, m_shader);
	m_triangles[3] = new Triangle(a, e, f, m_shader);
	m_triangles[4] = new Triangle(f, e, g, m_shader);
	m_triangles[5] = new Triangle(e, d, h, m_shader);
	m_triangles[6] = new Triangle(e, h, g, m_shader);
	m_triangles[7] = new Triangle(b, d, c, m_shader);
	m_triangles[8] = new Triangle(d, b, h, m_shader);
	m_triangles[9] = new Triangle(a, f, b, m_shader);
	m_triangles[10] = new Triangle(f, h, b, m_shader);
	m_triangles[11] = new Triangle(f, g, h, m_shader);
}

