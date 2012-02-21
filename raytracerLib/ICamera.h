#pragma once

#include <vector>

#include "Ray.h"


/**
 * The type used to store a list of rays.
 */
typedef std::vector<Ray> RayList;


/**
 * This class represents the camera.  It is used to store the position of the camera 
 * It is also in charge of sampling.
 */
class ICamera
{
public:
	virtual ~ICamera() { }

	/**
	 * Gets a ray that represents the position and viewing direction of the camera.
	 */
	virtual Ray GetPositionAndDirection() = 0;

	/**
	 * Sets the dimensions of the image being generated.
	 * @param width The width of the image.
	 * @param height The height of the image.
	 */
	virtual void SetImageDimensions(double width, double height) = 0;

	/**
	 * Calculates the set of viewing rays for a given image coordinate.
	 * The average color of all of the rays should be taken as the color of the pixel.
	 * @param imageX The x-coordinate to calculate a ray through.
	 * @param imageY The y-coordinate to calculate a ray through.
	 * @returns A list containing all of the rays that should be shot through the given pixel.
	 */
	virtual RayList CalculateViewingRays(double imageX, double imageY) = 0;
};

