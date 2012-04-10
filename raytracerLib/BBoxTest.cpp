#include <iostream>

#include "BBox.h"
#include "Box.h"

using namespace std;
using namespace sivelab;


double randInRange(double lower, double upper)
{
	double range = upper - lower;
	double rand = drand48() * range;
	return (rand + lower);
}


int main()
{
	BBox boundingBox;
	boundingBox.minPt[0] = -1.5;
	boundingBox.minPt[1] = -1;
	boundingBox.minPt[2] = -1.5;
	boundingBox.maxPt[0] = 1.125;
	boundingBox.maxPt[1] = 1.25;
	boundingBox.maxPt[2] = 1.5;

	Box box(boundingBox.minPt, boundingBox.maxPt, NULL);

	int bothHitCount = 0;
	int bothMissCount = 0;
	int noMatchCount = 0;
	int falsePositive = 0;
	int iterations = 100000;

	// Generate a bunch of rays with random directions and origins.
	for (int i = 0; i < iterations; i++)
	{
		Vector3D rayOrig;
		rayOrig[0] = randInRange(-5, 5);
		rayOrig[1] = randInRange(-5, 5);
		rayOrig[2] = randInRange(-5, 5);

		Vector3D rayDir;
		rayDir[0] = randInRange(-1, 1);
		rayDir[1] = randInRange(-1, 1);
		rayDir[2] = randInRange(-1, 1);
		rayDir.normalize();

		Ray ray(rayOrig, rayDir);

		Intersection intersect;
		bool boxHit = box.Intersect(ray, intersect);
		bool bboxHit = boundingBox.Intersects(ray);

		if (bboxHit != boxHit)
		{
			cout << "No match: boxHit=" << boxHit << ", bboxHit=" << bboxHit << ", t=" << intersect.t << ",\trayOrig=" << rayOrig << ",\trayDir=" << rayDir << endl;
			noMatchCount++;

			// Test for a bbox false positive.
			if (bboxHit == true)
			{
				falsePositive++;
			}
		}
		else
		{
			// Results agree.
			if (boxHit)
			{
				bothHitCount++;
			}
			else
			{
				bothMissCount++;
			}
		}
	}

	cout << falsePositive << " false positives" << endl;
	cout << bothHitCount << " hits agreed, " << bothMissCount << " misses agreed" << endl;
	cout << noMatchCount << " of " << iterations << " (" << (((double)noMatchCount / iterations) * 100.0) << "%) failed to match" << endl;

	return (0);
}
