#include <stdlib.h>
#include <stdio.h>
#include "PerspectiveCamera.h"
#include "Vector3D.h"
#include "RaytraceException.h"

using namespace sivelab;


PerspectiveCamera::PerspectiveCamera(const Ray& positionAndDirection, double viewPlaneDist, double viewPlaneWidth, int samplesPerPixel)
{
	m_positionAndDirection = positionAndDirection;
	m_viewPlaneDist = viewPlaneDist;
	m_viewPlaneWidth = viewPlaneWidth;
	m_samplesPerPixel = samplesPerPixel;
	m_basis.Calculate(positionAndDirection.GetDirection(), Vector3D(0.0, 1.0, 0.0));

	// Make sure that the number of samples per pixel is a perfect square.
	int sqrtSamplesPerPixel = (int)sqrt(m_samplesPerPixel);
	if ((sqrtSamplesPerPixel * sqrtSamplesPerPixel) != m_samplesPerPixel)
	{
		char buffer[128];
		sprintf(buffer, "Unable to construct jittered sampling: %i is not a perfect square!", m_samplesPerPixel);
		throw RaytraceException(buffer);
	}
}


void PerspectiveCamera::SetImageDimensions(double width, double height)
{
	m_imageWidth = width;
	m_imageHeight = height;
}


Ray PerspectiveCamera::GetPositionAndDirection()
{
	return (m_positionAndDirection);
}


double PerspectiveCamera::GetRandomOnUnitLine()
{
	return ((double)rand()/(double)RAND_MAX);
}


Ray PerspectiveCamera::GetRayThroughPoint(double imageX, double imageY)
{
	// The dimensions of the screen in the world.
	double viewPlaneHeight = m_viewPlaneWidth * m_imageHeight / m_imageWidth;
	double right = m_viewPlaneWidth / 2.0;
	double left = -right;
	double top = viewPlaneHeight / 2.0;
	double bottom = -top;

	double u = left + (right - left) * imageX / m_imageWidth;
	double v = bottom + (top - bottom) * imageY / m_imageHeight;

	Vector3D dw = -m_viewPlaneDist * m_basis.GetW();
	Vector3D uu = u * m_basis.GetU();
	Vector3D vv = v * m_basis.GetV();

	// The ray will have the same position as the camera.
	return (Ray(m_positionAndDirection.GetPosition(), dw + uu + vv));
}


RayList PerspectiveCamera::CalculateViewingRays(double imageX, double imageY)
{
	// The list of rays for this pixel.
	RayList rayList;

	// Shoot a single ray through the center of the pixel if we are only doing one sample per pixel.
	if (m_samplesPerPixel == 1)
	{
		rayList.push_back(GetRayThroughPoint(imageX + 0.5, imageY + 0.5));
		return (rayList);
	}

	// Generate some jittered rays.
	// Divide the pixel up into a grid of equally-sized squares.
	// Calculate the number of grid squares for both the width and the height of the pixel.
	int gridUnitsPerSide = (int)sqrt(m_samplesPerPixel);

	// Calculate the side length of a single grid square in pixels.
	double sideLengthOfUnit = 1.0 / gridUnitsPerSide;

	// Generate a ray for each grid unit.
	for (int gridY = 0; gridY < gridUnitsPerSide; gridY++)
	{
		for (int gridX = 0; gridX < gridUnitsPerSide; gridX++)
		{
			// Generate the random position of the ray inside the current square, and map it into pixel space.
			double pixelX = (GetRandomOnUnitLine() + gridX) * sideLengthOfUnit;
			double pixelY = (GetRandomOnUnitLine() + gridY) * sideLengthOfUnit;
			rayList.push_back(GetRayThroughPoint(imageX + pixelX, imageY + pixelY));
		}
	}

	// Return the list of rays.
	return (rayList);
}

