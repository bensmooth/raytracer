#include "Cylinder.h"
#include "Ray.h"
#include "Intersection.h"
#include "Scene.h"


Cylinder::Cylinder(IShader* shader, sivelab::Vector3D center, double height, double radius)
{
	m_shader = shader;
	m_center = center;
	m_height = height;
	m_radius = radius;
}


Cylinder::~Cylinder()
{
}


IShader *Cylinder::GetShader()
{
	return (m_shader);
}


bool Cylinder::Intersect(const Ray& ray, Intersection& result)
{
	double minY = m_center[1] - m_height / 2.0;
	double maxY = m_center[1] + m_height / 2.0;

	// The x and z coordinates of the center of the cylinder.
	double x_0 = m_center[0];
	double z_0 = m_center[2];

	// The x and z positions of the ray.
	double o_x = ray.GetPosition()[0];
	double o_z = ray.GetPosition()[2];

	// The x and z directions of the ray.
	double d_x = ray.GetDirection()[0];
	double d_z = ray.GetDirection()[2];

	// Use the quadradic equation to solve for t.
	double a = d_x*d_x + d_z*d_z;
	double b = 2.0 * (o_x*d_x + o_z*d_z - x_0*d_x - z_0*d_z);
	double c = x_0*x_0 + z_0*z_0 - 2.0*x_0*o_x - 2.0*z_0*o_z + o_x*o_x + o_z*o_z - m_radius*m_radius;

	// Evaluate descriminant.
	double descriminant = b*b - 4.0*a*c;
	if (descriminant < 0.0)
	{
		// No solutions.
		return (false);
	}

	// If we got here, there are two roots.  Grab the one that is closer, but still above 0.
	descriminant = sqrt(descriminant);
	double t1 = (-b + descriminant)/(2.0*a);
	double t2 = (-b - descriminant)/(2.0*a);
	bool t1Negative = (t1 < 0.0);
	bool t2Negative = (t2 < 0.0);

	// If both times are negative, there is no intersection.
	if (t1Negative && t2Negative)
	{
		return (false);
	}

	// Test intersection points against the minimum and maximum y values of the cylinder.
	sivelab::Vector3D intersectPointT1 = ray.GetPositionAtTime(t1);
	if ((intersectPointT1[1] < minY) || (intersectPointT1[1] > maxY))
	{
		// Set t1 to be negative so it doesn't get chosen.
		t1Negative = true;
	}
	sivelab::Vector3D intersectPointT2 = ray.GetPositionAtTime(t2);
	if ((intersectPointT2[1] < minY) || (intersectPointT2[1] > maxY))
	{
		// Set t2 to be negative so it doesn't get chosen.
		t2Negative = true;
	}

	// Recheck to see if both intersections didn't hit the cylinder within the y bounds.
	// If both times are negative, there is no intersection.
	if (t1Negative && t2Negative)
	{
		return (false);
	}

	// If either time is negative, ensure it doesn't get picked as the intersection time.
	if (t1Negative)
	{
		t1 = std::numeric_limits<double>::max();
	}
	if (t2Negative)
	{
		t2 = std::numeric_limits<double>::max();
	}

	// Select the smallest time value as the intersection time.
	sivelab::Vector3D *intersectPoint = NULL;
	if (t1 < t2)
	{
		// Choose t1.
		result.t = t1;
		intersectPoint = &intersectPointT1;
	}
	else
	{
		// Choose t2.
		result.t = t2;
		intersectPoint = &intersectPointT2;
	}

	// If we got here, we hit the cylinder.
	// Fill in the rest of the intersection structure.
	result.collidedRay = ray;
	result.object = this;

	// Calculate the outside-facing normal.
	result.surfaceNormal = *intersectPoint - m_center;
	result.surfaceNormal[1] = 0;
	result.surfaceNormal.normalize();

	// If the angle of incidence of the incoming ray and the normal is greater than 180 degrees, flip the normal.
	sivelab::Vector3D incoming = ray.GetDirection();
	incoming.normalize();
	if (result.surfaceNormal.dot(incoming) > 0)
	{
		// This ray hit on the inside.
		result.surfaceNormal *= -1.0;
	}

	// Ensure that the ray stays on the right side of whatever it hit by dialing back the time a little.
	result.t -= EPSILON;

	return (true);
}

