#include "Color.h"

#include <limits>

using namespace std;


Color::Color()
{
	m_red = 0.0;
	m_blue = 0.0;
	m_green = 0.0;
}


Color::Color(uint8_t red, uint8_t green, uint8_t blue)
{
	// Scale 1.0f to be 255.
	double maxValue = (double)numeric_limits<uint8_t>::max();
	m_red = (double)red / maxValue;
	m_green = (double)green / maxValue;
	m_blue = (double)blue / maxValue;
}


Color::Color(double red, double green, double blue)
{
	m_red = red;
	m_green = green;
	m_blue = blue;
}


Color::Color(const sivelab::Vector3D& color)
{
	m_red = color[0];
	m_green = color[1];
	m_blue = color[2];
}


Color::Color(const Color& source)
{
	m_red = source.m_red;
	m_green = source.m_green;
	m_blue = source.m_blue;
}


Color& Color::AddColors(const Color& other)
{
	m_red += other.m_red;
	m_green += other.m_green;
	m_blue += other.m_blue;

	return *this;
}


Color& Color::LinearMult(double scalar)
{
	m_red *= scalar;
	m_green *= scalar;
	m_blue *= scalar;

	return *this;
}


double Color::GetRed() const
{
	return m_red;
}


double Color::GetBlue() const
{
	return m_blue;
}


double Color::GetGreen() const
{
	return m_green;
}


png::rgb_pixel Color::GetImageColor() const
{
	png::rgb_pixel rgb;

	// For now, simply clip the values over 1.0.
	double red = min(m_red, 1.0);
	double green = min(m_green, 1.0);
	double blue = min(m_blue, 1.0);

	// Map [0.0, 1.0] to [0, 255].
	double maxValue = (double)numeric_limits<uint8_t>::max();
	rgb.red = maxValue * red;
	rgb.green = maxValue * green;
	rgb.blue = maxValue * blue;

	return (rgb);
}


void Color::SetRed(double red)
{
	m_red = red;
}


void Color::SetGreen(double green)
{
	m_green = green;
}


void Color::SetBlue(double blue)
{
	m_blue = blue;
}

