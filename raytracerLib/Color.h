#pragma once

#include <stdint.h>
#include <png++/png.hpp>

#include "Vector3D.h"


class Color
{
public:
	/**
	 * Sets color to black.
	 */
	Color();


	/**
	 * Constructs a color with the given components.
	 */
	Color(uint8_t red, uint8_t green, uint8_t blue);
	Color(double red, double green, double blue);


	/**
	 * Copy constructor.
	 */
	Color(const Color &source);


	/**
	 * Initializes the color from a vector.
	 */
	Color(const sivelab::Vector3D &color);


	/**
	 * Gets the color as a png pixel color.
	 * Colors that are out of range get clamped.
	 */
	png::rgb_pixel GetImageColor() const;


	/**
	 * Destructively adds the given color to this color.
	 * @param other The color to add to this color.
	 * @return A reference to this color, so that add calls can be chained.
	 */
	Color &AddColors(const Color &other);


	/**
	 * Destructively multiplies each component of this color with each component of the other color.
	 * @param other The other color to multiply with this color.
	 * @return A reference to this color, so that calls can be chained.
	 */
	Color &MultiplyColors(const Color &other);


	/**
	 * Destructively multiplies each component in this color by a scalar.
	 * Results are undefined if the scalar is zero.
	 */
	Color &LinearMult(double scalar);

	/**
	 * Gets the human-scaled luminance of the color.
	 */
	double GetLuminance();

	// Gets each component.
	double GetRed() const;
	double GetGreen() const;
	double GetBlue() const;


	// Sets each component.
	void SetRed(double red);
	void SetGreen(double green);
	void SetBlue(double blue);

private:
	double m_red, m_green, m_blue;
};
