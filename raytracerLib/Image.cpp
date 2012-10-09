#include <math.h>
#include <stack>
#include "Image.h"
#include "RaytraceException.h"

using namespace std;


Image::Image(int width, int height)
{
	AllocateBuffer(width, height);
}


void Image::AllocateBuffer(int width, int height)
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


Image::Image(const Image& other)
{
	// Allocate our image.
	AllocateBuffer(other.m_width, other.m_height);

	// Copy entire other image into this image.
	for (int imageY = 0; imageY < m_height; imageY++)
	{
		for (int imageX = 0; imageX < m_width; imageX++)
		{
			m_image[imageY][imageX] = other.m_image[imageY][imageX];
		}
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


double Exponent(double x)
{
	double result;

	// For x in (-inf, -2] we can approximate e^x as the line through (-2, e^-2) and (-inf, 0)
	if (x <= -2.0)
	{
		double MinDouble = std::numeric_limits<double>::min();
		// The line through (-2, e^-2) and (-inf, 0) is y=(c-x)/(e^2 (c+2))
		double denominator = M_E * M_E * (MinDouble + 2.0);
		return ((MinDouble - x) / denominator);
	}

	// Original.
	//result = exp(x);

	// Less precision.
//	result = (double)expf(x);

	// Taylor series.  Form is (x^n)/(n!).
	// First two terms.
	result = 1.0 + x;

	// Third term.
	result += (x*x)/2.0;

	// Fourth term.
	result += (x*x*x)/6.0;

	// Fifth term.
	result += (x*x*x*x)/24.0;

	return (result);
}


/**
 * Calculates the Gaussian weight of the given pixel offset from the center tap.
 */
double CalculateWeight(int x, double stdDev)
{
	// Equal to sqrt(2*pi)
	const double sqrt2Pi = 2.506628274631;

	// For a mean of 0, this is the normal function.
	double dividend = Exponent(-(x*x)/(2.0*stdDev*stdDev));
	double divisor = stdDev * sqrt2Pi;

	return (dividend / divisor);
}


/**
 * Combines the given adjacentColors with the weighting
 * computed from the offset away from the center tap, and the std deviation.
 * Both stacks will be empty upon success.
 */
Color CombineAdjacentColors(stack<Color> &adjacentColors, stack<int> &offsets, double stdDev)
{
	if (adjacentColors.size() != offsets.size())
	{
		throw RaytraceException("adjacentColors and offset stacks are different sizes in CombineAdjacentColors()!");
	}

	// Build up weighted result until the stacks are empty.
	Color result;
	while (!adjacentColors.empty())
	{
		// Pull current color and current offset away from the center tap from the stacks.
		Color currentColor = adjacentColors.top();
		adjacentColors.pop();
		int currentOffset = offsets.top();
		offsets.pop();

		// Compute weight to be given to current color.
		double weight = CalculateWeight(currentOffset, stdDev);

		// Apply weight, and combine result with running total.
		currentColor.LinearMult(weight);
		result.AddColors(currentColor);
	}

	return (result);
}


void Image::GaussianBlur(double stdDev, int radius)
{
	// Make a copy of the original image to read from while blurring.
	Image &original = *this;

	// Blur horizontally.
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			// Apply Gaussian filter around current pixel.
			// These are the colors and corresponding local offsets around the current pixel.
			stack<Color> adjacentColors;
			stack<int> offsets;

			for (int localX = -radius; localX < radius; localX++)
			{
				int absoluteX = x + localX;
				
				// Clamp absoluteX into the image dimensions [0, width-1].
				absoluteX = max(absoluteX, 0);
				absoluteX = min(absoluteX, m_width-1);
				
				// By the time we get here, we are within the bounds of the image.
				Color current = original(absoluteX, y);
				
				// Add the current local offset and the current color to the stacks.
				adjacentColors.push(current);
				offsets.push(localX);
			}

			// Compute and set the resultant color for the current pixel.
			this->operator()(x, y) = CombineAdjacentColors(adjacentColors, offsets, stdDev);
		}
	}

	// Blur vertically.
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			// Apply Gaussian filter around current pixel.
			// These are the colors and corresponding local offsets around the current pixel.
			stack<Color> adjacentColors;
			stack<int> offsets;

			for (int localY = -radius; localY < radius; localY++)
			{
				int absoluteY = y + localY;
				
				// Clamp absoluteY into the image dimensions [0, height-1].
				absoluteY = max(absoluteY, 0);
				absoluteY = min(absoluteY, m_height-1);
				
				// By the time we get here, we are within the bounds of the image.
				Color current = original(x, absoluteY);
				
				// Add the current local offset and the current color to the stacks.
				adjacentColors.push(current);
				offsets.push(localY);
			}

			// Compute and set the resultant color for the current pixel.
			this->operator()(x, y) = CombineAdjacentColors(adjacentColors, offsets, stdDev);
		}
	}
}


void Image::ConvertToGreyscale()
{
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			// Grab current pixel.
			Color &current = operator()(x, y);

			// Calculate luminance of current pixel.
			double luminance = current.GetLuminance();

			// Set each channel to the luminance.
			current.SetRed(luminance);
			current.SetGreen(luminance);
			current.SetBlue(luminance);
		}
	}
}


void Image::Add(const Image& other)
{
	if ((m_width != other.m_width) || (m_height != other.m_height))
	{
		throw RaytraceException("Images are not the same size in Image::Add()");
	}

	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			// Add each pixel in this image with the corresponding pixel in the other image.
			m_image[y][x].AddColors(other.m_image[y][x]);
		}
	}
}


void Image::DoGlobalHDR()
{
	Image &self = *this;

	// Used to scale the bright colors up to 1.0.
	double maxColor = 0.0;

	for (int imageY = 0; imageY < m_height; imageY++)
	{
		for (int imageX = 0; imageX < m_width; imageX++)
		{
			Color &current = self(imageX, imageY);
			double red = current.GetRed();
			double green = current.GetGreen();
			double blue = current.GetBlue();

			// Do some global HDR.  Maps [0, inf) to [0, 1].
			current.SetRed(red / (red + 1.0));
			current.SetGreen(green / (green + 1.0));
			current.SetBlue(blue / (blue + 1.0));
			maxColor = max(maxColor, max(current.GetRed(), max(current.GetGreen(), current.GetBlue())));
		}
	}

	// Stretch each component in the image to make the maximium color from the last step 1.0.
	double factor = 1.0 / maxColor;
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			// Hitting each component in the pixel with the factor will make
			// the colors stretch across the entire range [0,1].
			self(x, y).LinearMult(factor);
		}
	}
}


void Image::Postprocess()
{
	// Create the brightpassed copy.
	Image brightpassed(*this);

	// Apply bright-pass filter.
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			Color &current = brightpassed.m_image[y][x];

			// Scale pixels based on the luminance.
			double lum = current.GetLuminance();
			if (lum < 1.0)
			{
				// Scale colors down.
				current.LinearMult(max(lum - 0.5, 0.0));
			}
		}
	}

	// Blur the brightpassed image.
	brightpassed.GaussianBlur(4, 10);

	// Create a more blurry brightpassed image, and combine with the original.
	Image moreBlurry(brightpassed);
	brightpassed.GaussianBlur(8, 15);
	brightpassed.Add(moreBlurry);

	// Make more blur.
	moreBlurry.GaussianBlur(10, 20);
	brightpassed.Add(moreBlurry);

	moreBlurry.GaussianBlur(30, 50);
	brightpassed.Add(moreBlurry);
	brightpassed.DoGlobalHDR();

	// Combine the brightpassed image with the original.
	Add(brightpassed);

	// Scale everything back to [0, 1].
	DoGlobalHDR();
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


int Image::GetWidth()
{
	return (m_width);
}


int Image::GetHeight()
{
	return (m_height);
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
