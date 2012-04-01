#pragma once

#include <string>
#include <queue>

/**
 * The number of rows in a Matrix.
 */
const int MATRIX_ROWS = 4;

/**
 * The number of columns in a MatrixRow.
 */
const int MATRIX_COLS = 4;


/**
 * This class represents a single row of a 4x4 matrix.
 */
class MatrixRow
{
public:
	/**
	 * Swaps the content of the given two rows.
	 */
	static void Swap(MatrixRow &row1, MatrixRow &row2);

	/**
	 * Ctor.  Fills columns with zeros.
	 */
	MatrixRow();

	/**
	 * Copy ctor.
	 */
	MatrixRow(const MatrixRow &other);

	/**
	 * Ctor that takes in the 4 values of the row.
	 */
	MatrixRow(double v0, double v1, double v2, double v3);
	MatrixRow(double const values[MATRIX_COLS]);


	/**
	 * Returns the first nonzero value in the row.
	 * Returns 0.0 if all of the entries in the row are zero.
	 */
	double FindFirstNonzeroValue() const;


	/**
	 * Constructs a string representation of the row.
	 */
	std::string ToString() const;

	/**
	 * Assignment operator.
	 */
	MatrixRow &operator=(const MatrixRow &rhs);

	/**
	 * Allows array-like access to each element in the row.
	 */
	double &operator[](int index);
	double operator[](int index) const;

	/**
	 * Adds the content of this row and the other row.
	 */
	MatrixRow operator+(const MatrixRow &other) const;
	MatrixRow &operator+=(const MatrixRow &other);

	/**
	 * Subtracts the content of the other row from this row.
	 */
	MatrixRow operator-(const MatrixRow &other) const;
	MatrixRow &operator-=(const MatrixRow &other);

	/**
	 * Gets the value of the row if everything were negated.
	 */
	MatrixRow operator-() const;

	/**
	 * Multiplies a row by a constant.
	 */
	MatrixRow operator*(double c) const;
	MatrixRow &operator*=(double c);

	/**
	 * Divides a row by a constant.
	 */
	MatrixRow operator/(double c) const;
	MatrixRow &operator/=(double c);

private:
	/**
	 * If any value in the row is within an epsilon of an integer, it is set to that integer.
	 */
	void SnapToInts();

	double m_columns[MATRIX_COLS];
};


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

private:
	/**
	 * Disallow construction by client code.
	 */
	RowOperation();
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
	 * Constructs the inverse of this matrix.
	 */
	Matrix Inverse() const;

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
	MatrixRow &operator[](int index);
	MatrixRow operator[](int index) const;

	/**
	 * Multiplies matrices with each other.
	 */
	Matrix operator*(const Matrix &other) const;
	Matrix &operator*=(const Matrix &other);


private:
	MatrixRow m_rows[MATRIX_ROWS];
};

