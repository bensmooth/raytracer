#pragma once

#include "IShader.h"
#include "Color.h"


/**
 * This class simply returns the given color.
 */
class SolidShader : public IShader
{
public:
	/**
	 * Sets the color of the shader.
	 */
	SolidShader(const Color &color);

    virtual Color Shade(Intersection& intersection);

private:
	/**
	 * The color of the shader.
	 */
	Color m_color;
};
