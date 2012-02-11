#pragma once

#include "Ray.h"


/**
 * This class represents the camera.  It is used to store the position of the camera 
 */
class ICamera
{
public:
	~ICamera() { }


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
	 * Calculates the viewing ray for a given image coordinate.
	 * @param imageX The x-coordinate to calculate a ray through.
	 * @param imageY The y-coordinate to calculate a ray through.
	 */
	virtual Ray CalculateViewingRay(double imageX, double imageY) = 0;
};

