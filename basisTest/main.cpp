#include <iostream>
#include <math.h>

#include "Vector3D.h"
#include "Basis.h"

using namespace sivelab;
using namespace std;


/**
 * Converts radians to degrees.
 */
inline double radiansToDegrees(double radians)
{
	return (radians * (180.0 / M_PI));
}


int main(int argc, char *argv[])
{
	Vector3D a(12.42, -62.714, 19.908);
	Basis b;
	b.Calculate(a, Vector3D(0.0, 1.0, 0.0));

	cout << "a = " << a << endl;
	a.normalize();
	cout << "a normalized = " << a << endl;
	cout << "Basis:" << endl;
	cout << "u = " << b.GetU() << endl;
	cout << "v = " << b.GetV() << endl;
	cout << "w = " << b.GetW() << endl;

	// Calculate angles between vectors.
	// Since each vector is normalized, we can just do this:
	// arccos(A dot B)
	cout << "Angle between u and v = " << radiansToDegrees(acos(b.GetU().dot(b.GetV()))) << " degrees" << endl;
	cout << "Angle between u and w = " << radiansToDegrees(acos(b.GetU().dot(b.GetW()))) << " degrees" << endl;
	cout << "Angle between v and w = " << radiansToDegrees(acos(b.GetV().dot(b.GetW()))) << " degrees" << endl;
}

