#include <math.h>
#include "Image.h"
#include "RaytraceException.h"
#include <stack>

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


/**
 * Calculates the Gaussian weight of the given pixel offset from the center tap.
 */
double CalculateWeight(int x, double stdDev)
{
	// Equal to sqrt(2*pi)
	const double sqrt2Pi = 2.506628274631;

	// For a mean of 0, this is the normal function.
	double dividend = exp(-(x*x)/(2.0*stdDev*stdDev));
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
	Image original(*this);

	// Start blurring with the horizontal lines.
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
}


void Image::ConvertToGreyscale()
{
	// Some constants that control how much each channel contributes to the luminance.
	const double redW = 0.299;
	const double greenW = 0.587;
	const double blueW = 0.114;

	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			// Grab current pixel.
			Color &current = operator()(x, y);

			// Calculate luminance of current pixel.
			double luminance = current.GetRed()*redW + current.GetGreen()*greenW + current.GetBlue()*blueW;

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
	Color subtractsOne(-1.0, -1.0, -1.0);
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			Color &current = brightpassed.m_image[y][x];

			// This will subtract one from each channel; the still-positive channels were
			// above one, and should be included in the bright-passed image.
			current.AddColors(subtractsOne);

			// Clamp each channel to be above 0.
			current.SetRed(max(current.GetRed(), 0.0));
			current.SetGreen(max(current.GetGreen(), 0.0));
			current.SetBlue(max(current.GetBlue(), 0.0));
		}
	}

	// Convert the brightpassed image to greyscale.
	brightpassed.ConvertToGreyscale();

	// Blur the brightpassed image.
	brightpassed.GaussianBlur(10.0, 50);

	brightpassed.WriteToDisk("brightpassed.png");

	WriteToDisk("preAdd.png");

	// Combine the brightpassed image with the original.
	Add(brightpassed);

	WriteToDisk("postAdd.png");

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
