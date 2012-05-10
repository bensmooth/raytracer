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
	 * Writes the image to disk as a png.
	 */
	void WriteToDisk(std::string filename);

private:
	void ThrowIfOutOfBounds(int x, int y) const;

	int m_width, m_height;
	Color **m_image;
};
