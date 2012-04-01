#include <string.h>
#include <stdio.h>
#include <math.h>

#include "Matrix.h"
#include "RaytraceException.h"

using namespace std;


/**
 * Returns true if the two numbers are equal.
 */
#define EQUAL(a, b) (fabs((a) - (b)) < 0.0005)


void MatrixRow::Swap(MatrixRow& row1, MatrixRow& row2)
{
	// Take advantage of our overloaded equals sign.
	MatrixRow temp = row1;
	row1 = row2;
	row2 = temp;
}


MatrixRow::MatrixRow()
{
	for (int i = 0; i < MATRIX_COLS; i++)
	{
		m_columns[i] = 0.0;
	}
}


MatrixRow::MatrixRow(const MatrixRow& other)
{
	*this = other;
}


MatrixRow::MatrixRow(double v0, double v1, double v2, double v3)
{
	m_columns[0] = v0;
	m_columns[1] = v1;
	m_columns[2] = v2;
	m_columns[3] = v3;
}


MatrixRow::MatrixRow(const double values[MATRIX_COLS])
{
	memcpy(m_columns, values, sizeof(double) * MATRIX_COLS);
}


double MatrixRow::FindFirstNonzeroValue() const
{
	for (int i = 0; i < MATRIX_COLS; i++)
	{
		if (!EQUAL(m_columns[i], 0.0))
		{
			return (m_columns[i]);
		}
	}

	// If we got here, the entire row is zero.
	return (0.0);
}


std::string MatrixRow::ToString() const
{
	char buffer[128];

	snprintf(buffer, 128, "[%2.3f %2.3f %2.3f %2.3f]", m_columns[0], m_columns[1], m_columns[2], m_columns[3]);

	return (buffer);
}


MatrixRow& MatrixRow::operator=(const MatrixRow& rhs)
{
	for (int i = 0; i < MATRIX_COLS; i++)
	{
		m_columns[i] = rhs.m_columns[i];
	}

	return (*this);
}


double& MatrixRow::operator[](int index)
{
	if ((index < 0) || (index > MATRIX_COLS))
	{
		throw RaytraceException("Matrix column index out of bounds.");
	}

	return (m_columns[index]);
}


double MatrixRow::operator[](int index) const
{
	if ((index < 0) || (index > MATRIX_COLS))
	{
		throw RaytraceException("Matrix column index out of bounds.");
	}

	return (m_columns[index]);
}


MatrixRow MatrixRow::operator+(const MatrixRow& other) const
{
	double cols[MATRIX_COLS];
	for (int i = 0; i < MATRIX_COLS; i++)
	{
		cols[i] = m_columns[i] + other.m_columns[i];
	}

	MatrixRow result(cols);
	result.SnapToInts();

	return (result);
}


MatrixRow& MatrixRow::operator+=(const MatrixRow& other)
{
	*this = *this + other;

	return (*this);
}


MatrixRow MatrixRow::operator-(const MatrixRow& other) const
{
	double cols[MATRIX_COLS];
	for (int i = 0; i < MATRIX_COLS; i++)
	{
		cols[i] = m_columns[i] - other.m_columns[i];
	}

	MatrixRow result(cols);
	result.SnapToInts();

	return (result);
}


MatrixRow& MatrixRow::operator-=(const MatrixRow& other)
{
	*this = *this - other;

	return (*this);
}


MatrixRow MatrixRow::operator-() const
{
	double cols[MATRIX_COLS];
	for (int i = 0; i < MATRIX_COLS; i++)
	{
		cols[i] = -m_columns[i];
	}

	return (cols);
}


MatrixRow MatrixRow::operator*(double c) const
{
	double cols[MATRIX_COLS];
	for (int i = 0; i < MATRIX_COLS; i++)
	{
		cols[i] = m_columns[i] * c;
	}

	MatrixRow result(cols);
	result.SnapToInts();

	return (result);
}


MatrixRow& MatrixRow::operator*=(double c)
{
	*this = *this * c;

	return (*this);
}


MatrixRow MatrixRow::operator/(double c) const
{
	double cols[MATRIX_COLS];
	for (int i = 0; i < MATRIX_COLS; i++)
	{
		cols[i] = m_columns[i] / c;
	}

	MatrixRow result(cols);
	result.SnapToInts();

	return (result);
}


MatrixRow& MatrixRow::operator/=(double c)
{
	*this = *this / c;

	return (*this);
}


void MatrixRow::SnapToInts()
{
	for (int col = 0; col < MATRIX_COLS; col++)
	{
		int closestInt = (int)round(m_columns[col]);

		if (EQUAL((double)closestInt, m_columns[col]))
		{
			m_columns[col] = closestInt;
		}
	}
}


RowOperation::RowOperation()
{
	// Do nothing.
}


RowOperation RowOperation::Swap(int row1, int row2)
{
	RowOperation rowOp;
	rowOp.type = RowSwap;

	rowOp.targetRow = row1;
	rowOp.swapInfo.otherRow = row2;

	return (rowOp);
}


RowOperation RowOperation::Scale(int targetRow, double scalar)
{
	RowOperation rowOp;
	rowOp.type = RowScale;

	rowOp.targetRow = targetRow;
	rowOp.scaleInfo.scalar = scalar;

	return (rowOp);
}


RowOperation RowOperation::Add(int destRow, int sourceRow, double scalar)
{
	RowOperation rowOp;
	rowOp.type = RowAdd;

	rowOp.targetRow = destRow;

	rowOp.addInfo.sourceRow = sourceRow;
	rowOp.addInfo.scalar = scalar;

	return (rowOp);
}


string RowOperation::ToString()
{
	const int bufferLength = 64;
	char buffer[bufferLength];
	switch (type)
	{
		case RowSwap:
		{
			snprintf(buffer, bufferLength, "R%i<==>R%i", targetRow+1, swapInfo.otherRow+1);
		} break;
		case RowScale:
		{
			snprintf(buffer, bufferLength, "R%i *= %f", targetRow+1, scaleInfo.scalar);
		} break;
		case RowAdd:
		{
			snprintf(buffer, bufferLength, "R%i += %f * R%i", targetRow+1, addInfo.scalar, addInfo.sourceRow+1);
		} break;
		default:
		{
			throw RaytraceException("Unknown row operation type.");
		} break;
	}

	return (buffer);
}


Matrix::Matrix()
{
	// Each row of the matrix will already have been initialized by the MatrixRow ctor.
	// We do nothing.
}


Matrix::Matrix(const Matrix& other)
{
	*this = other;
}


void Matrix::ConstructIdentity()
{
	for (int row = 0; row < MATRIX_ROWS; row++)
	{
		for (int col = 0; col < MATRIX_COLS; col++)
		{
			double &currentCell = m_rows[row][col];
			if (row == col)
			{
				// If the row and the column are the same index, we are on the diagonal.
				currentCell = 1.0;
			}
			else
			{
				// We are not on the diagonal.
				currentCell = 0.0;
			}
		}
	}
}


void Matrix::ConstructTranslate(double x, double y, double z)
{
	ConstructIdentity();

	m_rows[0][3] = x;
	m_rows[1][3] = y;
	m_rows[2][3] = z;
}


void Matrix::ConstructRotation(double x, double y, double z)
{
	ConstructIdentity();

	// From http://en.wikipedia.org/wiki/Rotation_matrix#General_rotations
	double cosx = cos(x);
	double cosy = cos(y);
	double cosz = cos(z);
	double sinx = sin(x);
	double siny = sin(y);
	double sinz = sin(z);

	m_rows[0][0] = cosy*cosz;
	m_rows[0][1] = -cosx*sinz + sinx*siny*cosz;
	m_rows[0][2] = sinx*sinz + cosx*siny*cosz;
	m_rows[1][0] = cosy*sinz;
	m_rows[1][1] = cosx*cosz + sinx*siny*sinz;
	m_rows[1][2] = -sinx*cosz + cosx*siny*sinz;
	m_rows[2][0] = -sinx;
	m_rows[2][1] = sinx*cosy;
	m_rows[2][2] = cosx*cosy;
}


void Matrix::ConstructScale(double x, double y, double z)
{
	ConstructIdentity();

	m_rows[0][0] = x;
	m_rows[1][1] = y;
	m_rows[2][2] = z;
}


Matrix Matrix::Transpose() const
{
	Matrix result;

	// Note that this only works for square matrices.
	for (int i = 0; i < MATRIX_COLS; i++)
	{
		for (int j = 0; j < MATRIX_ROWS; j++)
		{
			result[i][j] = m_rows[j][i];
		}
	}

	return (result);
}


double Matrix::Determinant() const
{
	return (0.0);
}


Matrix Matrix::Inverse() const
{
	Matrix result;

	return (result);
}


void Matrix::ApplyOperation(RowOperation& op)
{
	switch (op.type)
	{
		case RowOperation::RowSwap:
		{
			// Simply swap the rows.
			MatrixRow::Swap(operator[](op.targetRow), operator[](op.swapInfo.otherRow));
		} break;
		case RowOperation::RowScale:
		{
			// Scale the given row.
			operator[](op.targetRow) *= op.scaleInfo.scalar;
		} break;
		case RowOperation::RowAdd:
		{
			// First, make a copy of the row we are going to add to the target row.
			MatrixRow source(operator[](op.addInfo.sourceRow));

			// Apply scalar.
			source *= op.addInfo.scalar;

			// Add the scaled source row to destination row.
			operator[](op.targetRow) += source;
		} break;
		default:
		{
			// We don't know what this is.
			throw RaytraceException("Unknown row operation type.");
		} break;
	}
}


void Matrix::RowReduce(bool reducedEchelon, queue<RowOperation> *operationsTaken)
{
	// Allow easier access to the [] operator.
	Matrix &self = *this;

	// Step over each row of the matrix, ignoring all rows above topRow.
	for (int topRow = 0; topRow < MATRIX_ROWS; topRow++)
	{
		// Find leftmost nonzero column.  This is the pivot column.
		int pivotCol = -1;
		for (int col = 0; col < MATRIX_COLS; col++)
		{
			if (IsColumnZero(col, topRow) == false)
			{
				// This is the current column.
				pivotCol = col;
				break;
			}
		}
		// If we got here and pivotCol is still -1, the entire matrix is 0, and therefore is already in echelon form.
		if (pivotCol == -1)
		{
			return;
		}

		// Select the largest (abs) value in the column.  This is the pivot row.
		int pivotRow = topRow;
		for (int row = topRow; row < MATRIX_ROWS; row++)
		{
			if (fabs(self[row][pivotCol]) > fabs(self[pivotRow][pivotCol]))
			{
				// This row has a value of greater magnitude than the previously selected row.
				pivotRow = row;
			}
		}

		// Swap the pivot row to the top, if needed.
		if (pivotRow != topRow)
		{
			// A swap is needed.
			RowOperation swapOp = RowOperation::Swap(pivotRow, topRow);
			ApplyOperation(swapOp);
			if (operationsTaken != NULL)
			{
				operationsTaken->push(swapOp);
			}
		}

		double &pivotValue = self[pivotRow][pivotCol];

		// Add the pivot row to other rows and produce all zeroes in the pivot column.
		for (int row = topRow + 1; row < MATRIX_ROWS; row++)
		{
			// This is the value we are trying to cancel out.
			double &valueToBeCancelled = self[row][pivotCol];

			// Skip this row if it is already 0.
			if (EQUAL(valueToBeCancelled, 0.0))
			{
				continue;
			}

			// This is the scalar that needs to be applied to the current row in order to zero it out.
			double replacementScalar = -valueToBeCancelled/pivotValue;

			RowOperation addOp = RowOperation::Add(row, pivotRow, replacementScalar);
			ApplyOperation(addOp);
			if (operationsTaken != NULL)
			{
				operationsTaken->push(addOp);
			}
		}

		// Ignore the current row, and advance down a row.  Repeat above steps.
	}

	// If we got here, the matrix is in echelon form.
	if (reducedEchelon)
	{
		// Reduce the matrix to reduced echelon form.
	}

	// Scale each row so that the leading value is 1.0.
	for (int row = 0; row < MATRIX_ROWS; row++)
	{
		double scaleValue = self[row].FindFirstNonzeroValue();

		// Skip rows that are all zero.
		if (EQUAL(scaleValue, 0.0))
		{
			continue;
		}

		RowOperation scaleOp = RowOperation::Scale(row, 1.0/scaleValue);
		ApplyOperation(scaleOp);
		if (operationsTaken != NULL)
		{
			operationsTaken->push(scaleOp);
		}
	}
}


bool Matrix::IsColumnZero(int column, int startRow)
{
	// Allow easier usage of the [] operator.
	Matrix &self = *this;
	for (int row = startRow; row < MATRIX_ROWS; row++)
	{
		if (!EQUAL(self[row][column], 0.0))
		{
			return (false);
		}
	}

	// If we got here, all of the columns were zero.
	return (true);
}


bool Matrix::IsRowZero(int row)
{
	// Allow easier usage of the [] operator.
	Matrix &self = *this;
	for (int col = 0; col < MATRIX_COLS; col++)
	{
		if (!EQUAL(self[row][col], 0.0))
		{
			return (false);
		}
	}

	// If we got here, all of the rows were zero.
	return (true);
}


string Matrix::ToString() const
{
	string result;

	for (int row = 0; row < MATRIX_ROWS; row++)
	{
		result += m_rows[row].ToString() + '\n';
	}

	return (result);
}


Matrix& Matrix::operator=(const Matrix& rhs)
{
	for (int row = 0; row < MATRIX_ROWS; row++)
	{
		// Take advantage of MatrixRow's equals operator overload.
		m_rows[row] = rhs.m_rows[row];
	}

	return (*this);
}


MatrixRow& Matrix::operator[](int index)
{
	if ((index < 0) || (index >= MATRIX_ROWS))
	{
		throw RaytraceException("Row index is out of range!");
	}

	return (m_rows[index]);
}


MatrixRow Matrix::operator[](int index) const
{
	if ((index < 0) || (index >= MATRIX_ROWS))
	{
		throw RaytraceException("Row index is out of range!");
	}

	return (m_rows[index]);
}


Matrix Matrix::operator*(const Matrix& other) const
{
	Matrix result;

	// Some helpful references.
	const Matrix &a = *this;
	const Matrix &b = other;

	for (int col = 0; col < MATRIX_COLS; col++)
	{
		for (int row = 0; row < MATRIX_ROWS; row++)
		{
			// Invariant: result[row][col] will already be 0 here because of the ctor.
			// Sum all of the products needed for this position in the matrix.
			for (int i = 0; i < MATRIX_COLS; i++)
			{
				result[row][col] += a[row][i] * b[i][col];
			}
		}
	}

	return (result);
}


Matrix& Matrix::operator*=(const Matrix& other)
{
	*this = operator*(other);

	return (*this);
}

