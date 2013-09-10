#pragma once

#include <string>
#include <sstream>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <float.h>

#include "EngineException.h"


/**
 * This class represents a single row of a square matrix, templatized on dimensions and element type.
 * Note that Type is expected to be a float, double, or class that can store fractional data.
 */
template <uint16_t Dimension, typename Type = double> class MatrixRow
{
public:
	/**
	 * Swaps the content of the given two rows.
	 */
	static void Swap(MatrixRow &row1, MatrixRow &row2)
	{
		// Take advantage of our overloaded equals sign.
		MatrixRow temp = row1;
		row1 = row2;
		row2 = temp;
	}


	/**
	 * Ctor.  Fills columns with zeros.
	 */
	MatrixRow()
	{
		for (int i = 0; i < Dimension; i++)
		{
			m_columns[i] = Type();
		}
	}


	/**
	 * Copy ctor.
	 */
	MatrixRow(const MatrixRow &other)
	{
		*this = other;
	}


	/**
	 * Ctor that takes in the values of the row from an array.
	 */
	MatrixRow(Type const values[Dimension])
	{
		for (int i = 0; i < Dimension; i++)
		{
			m_columns[i] = values[i];
		}
	}


	/**
	 * Returns the column of the first nonzero value in the row.
	 * Returns -1 if all of the entries in the row are zero.
	 */
	int FindFirstNonzeroValue() const
	{
		for (int i = 0; i < Dimension; i++)
		{
			Type zero = Type();
			if (!ElementEqual(m_columns[i], zero))
			{
				return (i);
			}
		}

		// If we got here, the entire row is zero.
		return (-1);
	}


	/**
	 * Constructs a string representation of the row.
	 */
	std::string ToString() const
	{
		std::stringstream ss;

		ss << "[";
		for (int i = 0; i < Dimension; i++)
		{
			char buffer[128];
			snprintf(buffer, 128, "%2.3f", (double)m_columns[i]);
			ss << buffer;

			if (i != Dimension-1)
			{
				ss << ' ';
			}
		}
		ss << "]";

		return (ss.str());
	}

	/**
	 * Assignment operator.
	 */
	MatrixRow &operator=(const MatrixRow &rhs)
	{
		for (int i = 0; i < Dimension; i++)
		{
			m_columns[i] = rhs.m_columns[i];
		}

		return (*this);
	}

	/**
	 * Allows array-like access to each element in the row.
	 */
	double &operator[](int index)
	{
		if ((index < 0) || (index > Dimension))
		{
			throw EngineException("Matrix column index out of bounds.");
		}

		return (m_columns[index]);
	}


	double operator[](int index) const
	{
		if ((index < 0) || (index > Dimension))
		{
			throw EngineException("Matrix column index out of bounds.");
		}

		return (m_columns[index]);
	}

	/**
	 * Adds the content of this row and the other row.
	 */
	MatrixRow operator+(const MatrixRow &other) const
	{
		Type cols[Dimension];
		for (int i = 0; i < Dimension; i++)
		{
			cols[i] = m_columns[i] + other.m_columns[i];
		}

		MatrixRow result(cols);
		result.SnapToInts();

		return (result);
	}


	MatrixRow &operator+=(const MatrixRow &other)
	{
		*this = *this + other;

		return (*this);
	}


	/**
	 * Subtracts the content of the other row from this row.
	 */
	MatrixRow operator-(const MatrixRow &other) const
	{
		Type cols[Dimension];
		for (int i = 0; i < Dimension; i++)
		{
			cols[i] = m_columns[i] - other.m_columns[i];
		}

		MatrixRow result(cols);
		result.SnapToInts();

		return (result);
	}


	MatrixRow &operator-=(const MatrixRow &other)
	{
		*this = *this - other;

		return (*this);
	}


	/**
	 * Gets the value of the row if everything were negated.
	 */
	MatrixRow operator-() const
	{
		double cols[Dimension];
		for (int i = 0; i < Dimension; i++)
		{
			cols[i] = -m_columns[i];
		}

		return (cols);
	}


	/**
	 * Multiplies a row by a constant.
	 */
	MatrixRow operator*(Type c) const
	{
		Type cols[Dimension];
		for (int i = 0; i < Dimension; i++)
		{
			cols[i] = m_columns[i] * c;
		}

		MatrixRow result(cols);
		result.SnapToInts();

		return (result);
	}


	MatrixRow &operator*=(Type c)
	{
		*this = *this * c;

		return (*this);
	}


	/**
	 * Divides a row by a constant.
	 */
	MatrixRow operator/(double c) const
	{
		double cols[Dimension];
		for (int i = 0; i < Dimension; i++)
		{
			cols[i] = m_columns[i] / c;
		}

		MatrixRow result(cols);
		result.SnapToInts();

		return (result);
	}


	MatrixRow &operator/=(double c)
	{
		*this = *this / c;

		return (*this);
	}


	/**
	 * Equality testing.
	 */
	bool operator==(const MatrixRow &other) const
	{
		for (int col = 0; col < Dimension; col++)
		{
			if (!ElementEqual(m_columns[col], other.m_columns[col]))
			{
				return false;
			}
		}

		// If we got here, we are equal.
		return true;
	}


	bool operator!=(const MatrixRow &other) const
	{
		return !(*this == other);
	}

private:
	/**
	 * If any value in the row is within an epsilon of an integer, it is set to that integer.
	 */
	void SnapToInts()
	{
		for (int col = 0; col < Dimension; col++)
		{
			int closestInt = (int)round((double)m_columns[col]);

			if (ElementEqual((Type)closestInt, m_columns[col]))
			{
				m_columns[col] = closestInt;
			}
		}
	}

	/**
	 * Returns true if the given elements are equal.
	 * Note: Casts both elements as doubles if you have insufficiently defined the equals operator for your type.
	 */
	static bool ElementEqual(Type a, Type b)
	{
		if (a == b)
		{
			return (true);
		}

		// Calculate the difference.
		const double maxDiff = DBL_EPSILON*4;
		double diff = fabs((double)a - (double)b);
		if (diff <= maxDiff)
		{
			return (true);
		}

		a = (Type)fabs((double)a);
		b = (Type)fabs((double)b);
		// Find the largest
		double largest = (double)((b > a) ? b : a);

		const double maxRelDiff = DBL_EPSILON*4;
		if (diff <= largest * maxRelDiff)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	Type m_columns[Dimension];

	friend class Matrix;
};
