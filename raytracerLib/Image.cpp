#include "Image.h"
#include "RaytraceException.h"


Image::Image(int width, int height)
{
	m_width = width;
	m_height = height;

	// Allocate the array that will hold the horizontal lines.
	m_image = new Color*[m_height];

	// Allocate each horizontal line.
	for (int i = 0; i < m_height; i++)
	{
		m_image[i] = new Color[width];
	}
}


Image::~Image()
{
	// Free memory in the opposite order it was aquired.
	for (int i = 0; i < m_height; i++)
	{
		delete[] m_image[i];
	}
	delete[] m_image;
}


Color& Image::operator()(int x, int y)
{
	ThrowIfOutOfBounds(x, y);
	return (m_image[y][x]);
}


const Color& Image::operator()(int x, int y) const
{
	ThrowIfOutOfBounds(x, y);
	return (m_image[y][x]);
}


void Image::WriteToDisk(std::string filename)
{
	// Copy entire image into a png image.
	png::image<png::rgb_pixel> outputImage(m_width, m_height);
	for (int imageY = 0; imageY < m_height; imageY++)
	{
		for (int imageX = 0; imageX < m_width; imageX++)
		{
			outputImage.set_pixel(imageX, imageY, m_image[imageY][imageX].GetImageColor());
		}
	}

	outputImage.write(filename);
}


void Image::ThrowIfOutOfBounds(int x, int y) const
{
	if (x < 0)
	{
		throw RaytraceException("X too small in Image.");
	}
	if (y < 0)
	{
		throw RaytraceException("Y too small in Image.");
	}
	if (x >= m_width)
	{
		throw RaytraceException("X too big in Image.");
	}
	if (y >= m_height)
	{
		throw RaytraceException("Y too big in Image.");
	}
}
