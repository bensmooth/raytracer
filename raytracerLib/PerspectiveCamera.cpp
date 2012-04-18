#include <stdlib.h>
#include <stdio.h>
#include "PerspectiveCamera.h"
#include "Vector3D.h"
#include "RaytraceException.h"
#include "JitteredSampler.h"

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
		sprintf(buffer, "Unable to construct jittered samples: %i is not a perfect square!", m_samplesPerPixel);
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

	// Generate some jittered rays to shoot through the pixel.
	JitteredSampler sampleGenerator;
	sampleGenerator.Generate(m_samplesPerPixel);
	const SampleList &samples = sampleGenerator.GetSampleList();

	rayList.resize(samples.size());
	for (size_t i = 0; i < samples.size(); i++)
	{
		rayList[i] = GetRayThroughPoint(imageX + samples[i].first, imageY + samples[i].second);
	}

	// Return the list of rays.
	return (rayList);
}

