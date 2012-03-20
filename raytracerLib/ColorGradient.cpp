#include "ColorGradient.h"


ColorGradient::ColorGradient(Color startColor, Color endColor)
{
	m_startColor = startColor;
	m_endColor = endColor;
}


Color ColorGradient::Sample(double x)
{
	Color a(m_startColor);
	Color b(m_endColor);

	// Lerp!
	a.LinearMult(1.0 - x);
	b.LinearMult(x);

	return (a.AddColors(b));
}


const Color& ColorGradient::GetStart()
{
	return (m_startColor);
}

const Color& ColorGradient::GetEnd()
{
	return (m_endColor);
}
