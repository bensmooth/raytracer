#include "Vector4D.h"
#include "Matrix.h"
#include <stdio.h>

Vector4D::Vector4D()
{
	vector3d.set(0.0, 0.0, 0.0);
	w = 0.0;
}


Vector4D::Vector4D(const sivelab::Vector3D& inVector3d, bool vectorIsPosition)
{
	vector3d = inVector3d;

	// If it is a position, w=1, otherwise, w=0.
	if (vectorIsPosition)
	{
		w = 1.0;
	}
	else
	{
		w = 0.0;
	}
}


Vector4D::Vector4D(const sivelab::Vector3D& inVector3d, double inW)
{
	vector3d = inVector3d;
	w = inW;
}


Vector4D::Vector4D(double const values[4])
{
	vector3d[0] = values[0];
	vector3d[1] = values[1];
	vector3d[2] = values[2];
	w = values[3];
}


Vector4D::Vector4D(double v0, double v1, double v2, double v3)
{
	vector3d[0] = v0;
	vector3d[1] = v1;
	vector3d[2] = v2;
	w = v3;
}


Vector4D Vector4D::operator*(const Matrix& other) const
{
	double result[4];

	double self[4];
	self[0] = vector3d[0];
	self[1] = vector3d[1];
	self[2] = vector3d[2];
	self[3] = w;

	// Multiply the vector as a row vector.
	for (int col = 0; col < 4; col++)
	{
		result[col] = 0.0;
		for (int row = 0; row < 4; row++)
		{
			result[col] += self[row] * other[row][col];
		}
	}

	return (result);
}


std::string Vector4D::ToString() const
{
	const int BUFFER_LENGTH = 128;
	char buffer[BUFFER_LENGTH];
	snprintf(buffer, BUFFER_LENGTH, "[%.3f %.3f %.3f %.3f]", vector3d[0], vector3d[1], vector3d[2], w);
	return (buffer);
}
