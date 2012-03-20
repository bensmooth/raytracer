#pragma once

#include "Color.h"


class ColorGradient
{
public:
	/**
	 * Constructs a gradient with the given starting and ending colors.
	 * @param startColor The value of the color for x=0.
	 * @param endColor The value of the color for x=1.
	 */
	ColorGradient(Color startColor, Color endColor);

	/**
	 * Samples the gradient at the given point.
	 * @param x The point in the range [0,1].
	 */
	Color Sample(double x);

	const Color &GetStart();
	const Color &GetEnd();

private:
	Color m_startColor, m_endColor;
};
