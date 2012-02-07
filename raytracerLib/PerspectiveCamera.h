#pragma once

#include "ICamera.h"
#include "Basis.h"

class PerspectiveCamera : public ICamera
{
public:
	/**
	 * Constructs a perspective camera with the given position, direction, and view plane.
	 */
	PerspectiveCamera(const Ray &positionAndDirection, double viewPlaneDist, double viewPlaneWidth);

    virtual Ray GetPositionAndDirection();

    virtual Ray CalculateViewingRay(double imageX, double imageY);

    virtual void SetImageDimensions(double width, double height);

private:
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
};
