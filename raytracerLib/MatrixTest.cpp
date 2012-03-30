#include <iostream>
#include "Matrix.h"

using namespace std;


int main()
{
	Matrix a;
	a[0] = MatrixRow(6, 2, 3, 0);
	a[1] = MatrixRow(3, -7, 5, 0);
	a[2] = MatrixRow(0, 0, 0, 0);
	a[3] = MatrixRow(5, 4, 1, 0);

	Matrix b;
	b[0] = MatrixRow(-2, 1, 7, 0);
	b[1] = MatrixRow(3, -6, 6, 0);
	b[2] = MatrixRow(1, 2, 5, 0);
	b[3] = MatrixRow(0, 0, 0, 0);

	cout << "A:" << endl << a.ToString() << endl;
	cout << "B:" << endl << b.ToString() << endl;

	cout << "A*B:" << endl;
	Matrix ab = a*b;
	cout << ab.ToString() << endl;

	cout << "A row-reduced:" << endl;
	a.RowReduce();
	cout << a.ToString() << endl;

	return (0);
}