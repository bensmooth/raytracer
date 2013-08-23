#include <string.h>
#include <stdio.h>
#include <math.h>

#include "Matrix.h"
#include "EngineException.h"
#include "Scene.h"

using namespace std;


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


int MatrixRow::FindFirstNonzeroValue() const
{
	for (int i = 0; i < MATRIX_COLS; i++)
	{
		if (!EQUAL(m_columns[i], 0.0))
		{
			return (i);
		}
	}

	// If we got here, the entire row is zero.
	return (-1);
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
		throw EngineException("Matrix column index out of bounds.");
	}

	return (m_columns[index]);
}


double MatrixRow::operator[](int index) const
{
	if ((index < 0) || (index > MATRIX_COLS))
	{
		throw EngineException("Matrix column index out of bounds.");
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


bool MatrixRow::operator==(const MatrixRow& other) const
{
	for (int col = 0; col < MATRIX_COLS; col++)
	{
		if (!EQUAL(m_columns[col], other.m_columns[col]))
		{
			return false;
		}
	}

	// If we got here, we are equal.
	return true;
}


bool MatrixRow::operator!=(const MatrixRow& other) const
{
	return !(*this == other);
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
			throw EngineException("Unknown row operation type.");
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


void Matrix::ConstructRotationX(double x)
{
	ConstructIdentity();

	m_rows[1][1] = cos(x);
	m_rows[1][2] = -sin(x);
	m_rows[2][1] = sin(x);
	m_rows[2][2] = cos(x);
}


void Matrix::ConstructRotationY(double y)
{
	ConstructIdentity();

	m_rows[0][0] = cos(y);
	m_rows[0][2] = sin(y);
	m_rows[2][0] = -sin(y);
	m_rows[2][2] = cos(y);
}


void Matrix::ConstructRotationZ(double z)
{
	ConstructIdentity();

	m_rows[0][0] = cos(z);
	m_rows[0][1] = -sin(z);
	m_rows[1][0] = sin(z);
	m_rows[1][1] = cos(z);
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
	// First, row reduce the matrix down to echelon form, keeping track of the operations that were applied.
	Matrix rowReduced = *this;
	queue<RowOperation> opsDone;
	rowReduced.RowReduce(false, &opsDone);

	// Calculate the scalar that needs to be applied to the determinant.
	double scalar = 1.0;
	while (!opsDone.empty())
	{
		RowOperation &op = opsDone.front();
		if (op.type == RowOperation::RowSwap)
		{
			scalar *= -1.0;
		}
		else if (op.type == RowOperation::RowScale)
		{
			scalar /= op.scaleInfo.scalar;
		}
		opsDone.pop();
	}

	// Since the matrix is now in upper triangular form, the determinant is the product of the entries in the diagonal.
	double det = 1.0;
	for (int i = 0; i < MATRIX_ROWS; i++)
	{
		det *= rowReduced[i][i];
	}

	// Combine the determinant of the row reduced matrix with the scalar we obtained
	// from the row reduction operations we performed on the matrix to get it there.
	return (scalar * det);
}


bool Matrix::Inverse(Matrix &outInverse) const
{
	// Make a local copy of ourself, so that we can row reduce it without modifying ourself.
	Matrix temp = *this;

	// First, row reduce the matrix to reduced echelon form.
	queue<RowOperation> rowOps;
	temp.RowReduce(true, &rowOps);

	// If the matrix is not equal to the identity matrix, this matrix is not invertible.
	Matrix result;
	result.ConstructIdentity();
	if (temp != result)
	{
		return (false);
	}

	// Apply the operations that were needed to reduce the matrix to the identity matrix.
	while (rowOps.empty() == false)
	{
		result.ApplyOperation(rowOps.front());
		rowOps.pop();
	}

	// If we got here, we were successful.
	outInverse = result;
	return (true);
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
			throw EngineException("Unknown row operation type.");
		} break;
	}
}


bool Matrix::EliminateWithPivot(RowOperation& opNeeded, int pivotRow, int pivotCol, int targetRow) const
{
	const Matrix &self = *this;

	// This is the value we are trying to cancel out.
	double valueToBeCancelled = self[targetRow][pivotCol];

	// The value of the pivot.
	double pivotValue = self[pivotRow][pivotCol];

	// If the pivot or the value to be cancelled is zero, we can't do anything.
	if (EQUAL(valueToBeCancelled, 0.0) || EQUAL(pivotValue, 0.0))
	{
		return (false);
	}

	// This is the scalar that needs to be applied to the current row in order to zero it out.
	double replacementScalar = -valueToBeCancelled/pivotValue;

	opNeeded = RowOperation::Add(targetRow, pivotRow, replacementScalar);
	return (true);
}


// Internally used to produce a matrix in reduced echelon form.
void EchelonForm(Matrix &self, queue<RowOperation> *operationsTaken)
{
	// Starting at the bottom, create zeroes above each pivot.
	for (int pivotRow = MATRIX_ROWS - 1; pivotRow >= 0; pivotRow--)
	{
		// Find the location of the pivot value for this row.
		int pivotCol = self[pivotRow].FindFirstNonzeroValue();
		if (pivotCol == -1)
		{
			// This row is entirely zero.  Skip it.
			continue;
		}

		// Go through each row above the pivot, and make zeros in the pivot column.
		for (int row = 0; row < pivotRow; row++)
		{
			RowOperation addOp;
			if (self.EliminateWithPivot(addOp, pivotRow, pivotCol, row) == false)
			{
				// If we got here, the row already has a zero in it.
				continue;
			}
			self.ApplyOperation(addOp);
			if (operationsTaken != NULL)
			{
				operationsTaken->push(addOp);
			}
		}
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

		// Add the pivot row to other rows and produce all zeroes in the pivot column.
		for (int row = topRow + 1; row < MATRIX_ROWS; row++)
		{
			RowOperation addOp;
			if (EliminateWithPivot(addOp, pivotRow, pivotCol, row) == false)
			{
				// If we got here, the row already has a zero in it.
				continue;
			}
			ApplyOperation(addOp);
			if (operationsTaken != NULL)
			{
				operationsTaken->push(addOp);
			}
		}

		// Ignore the current row, and advance down a row.  Repeat above steps.
	}

	// If we got here, the matrix is in echelon form.
	// Reduce the matrix to reduced echelon form if asked to.
	if (reducedEchelon)
	{
		EchelonForm(self, operationsTaken);
	}

	// Scale each row so that the leading value is 1.0.
	for (int row = 0; row < MATRIX_ROWS; row++)
	{
		int firstNonZeroIndex = self[row].FindFirstNonzeroValue();
		// Skip rows that are all zero or already lead with ones.
		if (firstNonZeroIndex == -1)
		{
			continue;
		}
		double scaleValue = self[row][firstNonZeroIndex];
		if (EQUAL(scaleValue, 1.0))
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
		throw EngineException("Row index is out of range!");
	}

	return (m_rows[index]);
}


MatrixRow Matrix::operator[](int index) const
{
	if ((index < 0) || (index >= MATRIX_ROWS))
	{
		throw EngineException("Row index is out of range!");
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


Vector4D Matrix::operator*(const Vector4D& other) const
{
	const Matrix &self = *this;

	double vector[MATRIX_COLS];
	vector[0] = other.vector3d[0];
	vector[1] = other.vector3d[1];
	vector[2] = other.vector3d[2];
	vector[3] = other.w;

	double result[MATRIX_COLS];

	// Perform the multiplication with other as a column vector.
	for (int row = 0; row < MATRIX_ROWS; row++)
	{
		result[row] = 0.0;
		for (int col = 0; col < MATRIX_COLS; col++)
		{
			result[row] += vector[col] * self[row][col];
		}
	}

	return (result);
}


Ray Matrix::operator*(const Ray& ray) const
{
	// Extract position and direction.
	Vector4D rayPos(ray.GetPosition(), true);
	Vector4D rayDir(ray.GetDirection(), false);

	// Multiply matrix with position and direction vectors.
	rayPos = operator*(rayPos);
	rayDir = operator*(rayDir);

	return (Ray(rayPos.vector3d, rayDir.vector3d));
}


bool Matrix::operator==(const Matrix& other) const
{
	for (int row = 0; row < MATRIX_ROWS; row++)
	{
		if (m_rows[row] != other.m_rows[row])
		{
			return (false);
		}
	}
	return (true);
}


bool Matrix::operator!=(const Matrix& other) const
{
	return !(*this == other);
}

