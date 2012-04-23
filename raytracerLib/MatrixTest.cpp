#include <iostream>
#include "Matrix.h"

using namespace std;


int main()
{
	Matrix a;
	a[0] = MatrixRow(0, 1, 1.732, 0);
	a[1] = MatrixRow(2, 0, 0, 2);
	a[2] = MatrixRow(0, 1.732, -1, 1);
	a[3] = MatrixRow(0, 0, 0, 1);

	Matrix b;
	b[0] = MatrixRow(-2, 1, 7, 1);
	b[1] = MatrixRow(3, -6, 6, -20);
	b[2] = MatrixRow(1, 2, 5, 12);
	b[3] = MatrixRow(9, 8, 3, 7);

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

	cout << "A*Ainv" << endl;
	cout << (a*aInv).ToString() << endl;

	cout << "det A: " << a.Determinant() << endl;

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
