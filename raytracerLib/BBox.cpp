#include <limits>

#include "BBox.h"
#include "Ray.h"
#include "Scene.h"

using namespace std;
using namespace sivelab;


BBox BBox::MakeFromPoints(const vector<Vector3D> &points)
{
	double minX;
	double maxX;
	double minY;
	double maxY;
	double minZ;
	double maxZ;
	// No value can be larger than this.
	minX = minY = minZ = numeric_limits<double>::max();

	// No value can be smaller than this.
	maxX = maxY = maxZ = -numeric_limits<double>::max();

	for (size_t i = 0; i < points.size(); i++)
	{
		minX = min(minX, points.at(i)[0]);
		minY = min(minY, points.at(i)[1]);
		minZ = min(minZ, points.at(i)[2]);

		maxX = max(maxX, points.at(i)[0]);
		maxY = max(maxY, points.at(i)[1]);
		maxZ = max(maxZ, points.at(i)[2]);
	}

	BBox result;
	result.minPt[0] = minX;
	result.minPt[1] = minY;
	result.minPt[2] = minZ;

	result.maxPt[0] = maxX;
	result.maxPt[1] = maxY;
	result.maxPt[2] = maxZ;

	return (result);
}


BBox BBox::Combine(const vector<BBox> &boxes)
{
	// Make a temporary list of points, and make it the correct size.
	vector<Vector3D> points;
	points.resize(2 * boxes.size());

	for (size_t i = 0; i < boxes.size(); i++)
	{
		// There are two points per box.
		points[2 * i] = boxes[i].minPt;
		points[2 * i + 1] = boxes[i].maxPt;
	}

	return (MakeFromPoints(points));
}


BBox BBox::Combine(const BBox& a, const BBox& b)
{
	vector<Vector3D> points;

	// 2 points per box.
	points.resize(4);
	points[0] = a.minPt;
	points[1] = a.maxPt;
	points[2] = b.minPt;
	points[3] = b.maxPt;

	return(MakeFromPoints(points));
}


Vector3D BBox::GetCenter()
{
	Vector3D result = minPt + maxPt;
	result /= 2.0;
	return (result);
}


bool BBox::Intersects(const Ray& ray) const
{
	const Vector3D &rayOrig = ray.GetPosition();
	const Vector3D &rayDir = ray.GetDirection();

	// Figure out the minimum and maximum times it will intersect with the x slab.
	double tmin = (minPt[0] - rayOrig[0]) / rayDir[0];
	double tmax = (maxPt[0] - rayOrig[0]) / rayDir[0];
	if (rayDir[0] < 0.0)
	{
		// Swap values if this direction is negative.
		double temp = tmin;
		tmin = tmax;
		tmax = temp;
	}

	// Find min and max times ray will intersect with y slab.
	double tymin = (minPt[1] - rayOrig[1]) / rayDir[1];
	double tymax = (maxPt[1] - rayOrig[1]) / rayDir[1];
	if (rayDir[1] < 0.0)
	{
		// Swap values if this ray direction is negative.
		double temp = tymin;
		tymin = tymax;
		tymax = temp;
	}

	// See if we intersected the column formed by the intersection of the x and y slabs.
	if ((tmin > tymax) || (tymin > tmax))
	{
		// Ray did not hit the intersection of the x and y slabs.
		return false;
	}

	// Find largest minimum t value.
	tmin = max(tmin, tymin);

	// Find smallest maximum t value.
	tmax = min(tmax, tymax);

	// Find min and max times ray will intersect the z slab.
	double tzmin = (minPt[2] - rayOrig[2]) / rayDir[2];
	double tzmax = (maxPt[2] - rayOrig[2]) / rayDir[2];
	if (rayDir[2] < 0.0)
	{
		// Swap values if this direction is negative.
		double temp = tzmin;
		tzmin = tzmax;
		tzmax = temp;
	}

	if ((tmin > tzmax) || (tzmin > tmax))
	{
		return false;
	}
	else
	{
		return true;
	}
}

