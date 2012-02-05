#include "SolidShader.h"
#include "Color.h"

Color SolidShader::Shade(Intersection& intersection)
{
	return (m_color);
}
