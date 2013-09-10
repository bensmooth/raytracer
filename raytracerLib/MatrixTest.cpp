#include <iostream>
#include "Matrix.h"

using namespace std;

double a1[] = {0, 1, 1.7, 0};
double a2[] = {2, 0, 0, 2};
double a3[] = {0, 1.7, -1, 1};
double a4[] = {0, 0, 0, 1};

double b1[] = {-2, 1, 7, 1};
double b2[] = {3, -6, 6, -20};
double b3[] = {1, 2, 5, 12};
double b4[] = {9, 8, 3, 7};

int main()
{
	Matrix a;
	a[0] = MatrixRow<4>(a1);
	a[1] = MatrixRow<4>(a2);
	a[2] = MatrixRow<4>(a3);
	a[3] = MatrixRow<4>(a4);

	Matrix b;
	b[0] = MatrixRow<4>(b1);
	b[1] = MatrixRow<4>(b2);
	b[2] = MatrixRow<4>(b3);
	b[3] = MatrixRow<4>(b4);

	cout << "A:" << endl << a.ToString() << endl;
	cout << "B:" << endl << b.ToString() << endl;

	cout << "A*B:" << endl;
	Matrix ab = a*b;
	cout << ab.ToString() << endl;

	Matrix aInv;
	if (a.Inverse(aInv) == true)
	{
		cout << "A inverse:" << endl;
		cout << aInv.ToString() << endl;
	}
	else
	{
		cout << "A is not invertable." << endl;
	}

	Matrix aTimesAInv = a*aInv;
	Matrix identity;
	identity.ConstructIdentity();
	cout << "A*Ainv" << endl;
	cout << aTimesAInv.ToString();
	if (aTimesAInv != identity)
	{
		cerr << "A*Ainv is supposed to be the identity matrix!" << endl << endl;
	}

	double aDet = a.Determinant();
	cout << "det A: " << aDet << endl;
	if (fabs(aDet - 7.78) > 0.005)
	{
		cerr << "Unexpected value for determinant of A; expected 7.78" << endl;
	}

	cout << "A row-reduced:" << endl;
	queue<RowOperation> rowOps;
	Matrix arr = a;
	arr.RowReduce(true, &rowOps);
	cout << arr.ToString() << endl;

	cout << "Operations:" << endl;
	while (rowOps.empty() == false)
	{
		cout << rowOps.front().ToString() << endl;
		rowOps.pop();
	}

	cout << "Vector4 x:" << endl;
	Vector4D x(-2.0, 6.0, 11.0, 17.0);
	cout << x.ToString() << endl;

	cout << "A*x:" << endl;
	cout << (a*x).ToString() << endl;

	cout << "x*A:" << endl;
	cout << (x*a).ToString() << endl;

	return (0);
}
