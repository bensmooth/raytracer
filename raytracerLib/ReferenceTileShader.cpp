#include "ReferenceTileShader.h"
#include "Intersection.h"

using namespace sivelab;


Color ReferenceTileShader::Shade(Intersection& intersection)
{
	Vector3D intersectPoint = intersection.collidedRay.GetPositionAtTime(intersection.t);
	// Shift intersect point up so that we don't get wierd aliasing when things are laying on an axis.
	intersectPoint += Vector3D(5000.0, 5000.0, 5000.0);

	Color light(0.05, 0.05, 0.05);
	Color dark(0.95, 0.95, 0.95);

	bool useLight = true;

	for (int i = 0; i < 3; i++)
	{
		// Ensure that around the axis, everything is opposite, instead of the same.
		if (intersectPoint[i] <= 0.0)
		{
			intersectPoint[i] -= 1.0;
		}

		if ((((int)intersectPoint[i]) % 2) == 0)
		{
			useLight = !useLight;
		}
	}

	if (useLight)
	{
		return (light);
	}
	else
	{
		return (dark);
	}

	// Figure out the color of the tile.
/**
	if ((((int)intersectPoint[0]) % 2) == 0)
	{
		if ((((int)intersectPoint[2]) % 2) == 0)
		{
			return (dark);
		}
		else
		{
			return (light);
		}
	}
	else
	{
		if ((((int)intersectPoint[2]) % 2) == 0)
		{
			return (light);
		}
		else
		{
			return (dark);
		}
	}
**/
}

