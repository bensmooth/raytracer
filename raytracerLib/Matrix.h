#pragma once

#include <string>
#include <queue>
#include "MatrixRow.h"
#include "Vector4D.h"
#include "Ray.h"


/**
 * The number of rows in a Matrix.
 */
const int MATRIX_ROWS = 4;

/**
 * The number of columns in a MatrixRow
 */
const int MATRIX_COLS = 4;


/**
 * Represents a row operation on a matrix.
 */
struct RowOperation
{
	enum
	{
		RowSwap,
		RowScale,
		RowAdd
	} type;

	/**
	 * The row that is to be modified.
	 */
	int targetRow;

	union
	{
		struct
		{
			/**
			 * The other row that was swapped with modifiedRow.
			 */
			int otherRow;
		} swapInfo;
		struct
		{
			/**
			 * The constant that the row should be multiplied with.
			 */
			double scalar;
		} scaleInfo;
		struct
		{
			/**
			 * The scalar to apply to the source row before adding it with the target row.
			 */
			double scalar;

			/**
			 * The row to add to the target row.
			 */
			int sourceRow;
		} addInfo;
	};

	/**
	 * Constructs a row swap operation.
	 * @param row1 The row index to swap with row2.
	 * @param row1 The row index to swap with row1.
	 */
	static RowOperation Swap(int row1, int row2);


	/**
	 * Constructs a row scale operation.
	 * @param targetRow The row to scale.
	 * @param scalar The scalar to apply to the row.
	 */
	static RowOperation Scale(int targetRow, double scalar);


	/**
	 * Constructs a row add operation.
	 * @param destRow The row that will be modified.
	 * @param sourceRow The row that will be added to the destination row.
	 * @param scalar The scalar to apply to the source row before adding it to the destination row.
	 */
	static RowOperation Add(int destRow, int sourceRow, double scalar);


	/**
	 * Gets a string representation of the operation.
	 */
	std::string ToString();
};


/**
 * A 4x4 matrix.
 */
class Matrix
{
public:
	/**
	 * Constructs a 4x4 matrix filled with zeros.
	 */
	Matrix();

	/**
	 * Copy ctor.
	 */
	Matrix(const Matrix &other);

	/**
	 * Fills the matrix with the identity matrix.
	 */
	void ConstructIdentity();

	/**
	 * Fills the matrix with a translation transformation with the given parameters.
	 */
	void ConstructTranslate(double x, double y, double z);

	/**
	 * Fills the matrix with a rotation transformation with the given angles for each axis.
	 */
	void ConstructRotation(double x, double y, double z);
	void ConstructRotationX(double x);
	void ConstructRotationY(double y);
	void ConstructRotationZ(double z);

	/**
	 * Constructs a scale transformation matrix with the given scaling factors for each coordinate.
	 */
	void ConstructScale(double x, double y, double z);

	/**
	 * Gets the transpose of the matrix.
	 */
	Matrix Transpose() const;

	/**
	 * Calculates the determinant of the matrix.
	 */
	double Determinant() const;

	/**
	 * Finds the inverse of this matrix.
	 * @param outInverse If true is returned, this will contain the inverse of the matrix.
	 * @return Returns true if this matrix is invertible, false otherwise.
	 */
	bool Inverse(Matrix &outInverse) const;

	/**
	 * Does row reduction operations on the matrix.
	 * @param reducedEchelon If false, only reduce matrix to echelon form.  If true, reduce matrix to reduced echelon form.
	 * @param operationsTaken If not NULL, will contain a queue of the operations that were needed to reduce the matrix.
	 */
	void RowReduce(bool reducedEchelon = false, std::queue<RowOperation> *operationsTaken = NULL);

	/**
	 * Applies the given row operation to the matrix.
	 */
	void ApplyOperation(RowOperation &op);

	/**
	 * Tries to find the add operation needed to reduce the given column in the given row to zero with the given pivot row.
	 * @param opNeeded If true is returned, this will contain the operation needed to zero out the given row and column with the pivot.
	 * @param pivotRow The row the pivot is in.  Should not be the same as targetRow.
	 * @param pivotCol The column the pivot is in.
	 * @param targetRow The row to produce a zero in. Should not be the same as pivotRow.
	 * @return True if the operation in opNeeded will successfully zero out [targetRow][pivotCol], false if either the pivot value or row to zero is already zero.
	 */
	bool EliminateWithPivot(RowOperation &opNeeded, int pivotRow, int pivotCol, int targetRow) const;

	/**
	 * Returns true if the given column is entirely zero.
	 * @param column The column to test if it is entirely zero.
	 * @param startRow The row to start testing for zero at.  Any rows above this will not be checked.
	 */
	bool IsColumnZero(int column, int startRow = 0);

	/**
	 * Returns true if the given row is entirely zero.
	 */
	bool IsRowZero(int row);

	/**
	 * Constructs a string representation of the matrix.
	 */
	std::string ToString() const;

	/**
	 * Assignment operator.
	 */
	Matrix &operator=(const Matrix &rhs);

	/**
	 * Gets a reference to the row at the given index.
	 */
	MatrixRow<MATRIX_COLS> &operator[](int index);
	MatrixRow<MATRIX_COLS> operator[](int index) const;

	/**
	 * Multiplies matrices with each other.
	 */
	Matrix operator*(const Matrix &other) const;
	Matrix &operator*=(const Matrix &other);

	/**
	 * Multiplies the matrix with a column vector.
	 */
	Vector4D operator*(const Vector4D &other) const;

	/**
	 * Applies the transformation in this matrix to the ray.
	 */
	Ray operator*(const Ray &ray) const;

	/**
	 * Equality testing.
	 */
	bool operator==(const Matrix &other) const;
	bool operator!=(const Matrix &other) const;

private:
	MatrixRow<MATRIX_COLS> m_rows[MATRIX_ROWS];
};

