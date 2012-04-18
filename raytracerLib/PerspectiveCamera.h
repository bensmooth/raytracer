#pragma once

#include "ICamera.h"
#include "Basis.h"

/**
 * A camera that uses the perspective projection, and can act as either a jittered or regular sampler.
 */
class PerspectiveCamera : public ICamera
{
public:
	/**
	 * Constructs a perspective camera with the given position, direction, and view plane.
	 * If samplesPerPixel is one, the camera acts as a regular sampler, otherwise, it acts as a jittered sampler.
	 * @param positionAndDirection The position and direction of the camera in world space.
	 * @param viewPlaneDist The distance from the camera to the view plane.
	 * @param viewPlaneWidth The width of the view plane.
	 * @param samplesPerPixel The number of samples per pixel.  Must be a perfect square.
	 * @throws RaytraceException if the number of samples per pixel is not a perfect square.
	 */
	PerspectiveCamera(const Ray &positionAndDirection, double viewPlaneDist, double viewPlaneWidth, int samplesPerPixel = 1);

	virtual Ray GetPositionAndDirection();

	virtual RayList CalculateViewingRays(double imageX, double imageY);

	virtual void SetImageDimensions(double width, double height);

private:
	/**
	 * Calculates a single viewing ray starting at the camera exactly through the given image coordinates.
	 */
	Ray GetRayThroughPoint(double imageX, double imageY);

	/**
	 * The camera's location and facing direction.
	 */
	Ray m_positionAndDirection;

	/**
	 * The ONB that was calculated from the direction of the camera.
	 */
	Basis m_basis;

	/**
	 * The distance between the camera and the view plane.
	 */
	double m_viewPlaneDist;

	/**
	 * The width of the view plane, in world units.
	 */
	double m_viewPlaneWidth;

	/**
	 * The dimensions of the image, in pixels.
	 */
	double m_imageWidth, m_imageHeight;

	/**
	 * The number of samples per pixel.
	 */
	int m_samplesPerPixel;
};
