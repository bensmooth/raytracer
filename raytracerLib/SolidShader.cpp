#include "SolidShader.h"
#include "Color.h"

SolidShader::SolidShader(const Color& color)
{
	m_color = color;
}


Color SolidShader::Shade(Intersection& intersection)
{
	return (m_color);
}
