#pragma once

#include "Color.h"


/**
 * Encapsulates a HDR image.
 * Threadsafe as long as no two threads try to access the same pixel at the same time.
 */
class Image
{
public:
	Image(int width, int height);
	Image(const Image &other);
	virtual ~Image();

	/**
	 * Allows array-like access to each pixel.
	 */
	Color &operator()(int x, int y);
	const Color &operator()(int x, int y) const;

	/**
	 * Applies a global tone mapping technique that guarantees that none of the colors will be above 1.0.
	 */
	void DoGlobalHDR();

	/**
	 * Blurs the image.  A larger standard deviation will produce a blurrier image.
	 * @param stdDev The standard deviation to use when weighting the pixels.
	 * @param radius The number of pixels to go out from the center of each blurred pixel.
	 */
	void GaussianBlur(double stdDev, int radius);

	/**
	 * Converts the image, in-place, to greyscale, using a luminance operator that takes into account human perception.
	 */
	void ConvertToGreyscale();

	/**
	 * Destructively adds the other image to this image.
	 * @warning If images are not the same size, an exception will be thrown.
	 */
	void Add(const Image &other);

	/**
	 * Postprocesses, performing a global HDR technique, and producing bloom effect.
	 */
	void Postprocess();

	/**
	 * Writes the image to disk as a png.
	 */
	void WriteToDisk(std::string filename);

	/**
	 * Gets the width of the image.
	 */
	int GetWidth();

	/**
	 * Gets the height of the image.
	 */
	int GetHeight();
private:
	/**
	 * Throws an exception if the given coodinate pair is out of bounds.
	 */
	void ThrowIfOutOfBounds(int x, int y) const;

	/**
	 * Allocates the image buffer with the given width and height.
	 * Also sets the width and height member variables.
	 * @warning Does not free anything already in the buffer.
	 */
	void AllocateBuffer(int width, int height);

	int m_width, m_height;
	Color **m_image;
};


/**
 * Fast and dirty exp() implementation.
 */
double Exponent(double x);
