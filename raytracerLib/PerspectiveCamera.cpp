#include "PerspectiveCamera.h"
#include "Vector3D.h"

using namespace sivelab;


PerspectiveCamera::PerspectiveCamera(const Ray& positionAndDirection, double viewPlaneDist, double viewPlaneWidth)
{
	m_positionAndDirection = positionAndDirection;
	m_viewPlaneDist = viewPlaneDist;
	m_viewPlaneWidth = viewPlaneWidth;
	m_basis.Calculate(positionAndDirection.GetDirection(), Vector3D(0.0, 1.0, 0.0));
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


Ray PerspectiveCamera::CalculateViewingRay(double imageX, double imageY)
{
	// The dimensions of the screen in the world.
	double viewPlaneHeight = m_viewPlaneWidth * m_imageHeight / m_imageWidth;
	double right = m_viewPlaneWidth / 2.0;
	double left = -right;
	double top = viewPlaneHeight / 2.0;
	double bottom = -top;

	double u = left + (right - left) * (imageX + 0.5) / m_imageWidth;
	double v = bottom + (top - bottom) * (imageY + 0.5) / m_imageHeight;

	Vector3D dw = -m_viewPlaneDist * m_basis.GetW();
	Vector3D uu = u * m_basis.GetU();
	Vector3D vv = v * m_basis.GetV();

	// The final ray will have the same position as the camera.
	return (Ray(m_positionAndDirection.GetPosition(), dw + uu + vv));
}

