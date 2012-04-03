#include <iostream>
#include "Matrix.h"

using namespace std;


int main()
{
	Matrix a;
	a[0] = MatrixRow(6, 2, 3, 9);
	a[1] = MatrixRow(3, -7, 5, 1);
	a[2] = MatrixRow(3, -7, 5, -2);
	a[3] = MatrixRow(5, 4, 1, 7);

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
	a.RowReduce(true, &rowOps);
	cout << a.ToString() << endl;

	cout << "Operations:" << endl;
	while (rowOps.empty() == false)
	{
		cout << rowOps.front().ToString() << endl;
		rowOps.pop();
	}

	return (0);
}
