#include "CosineShader.h"
#include "Scene.h"
#include "Intersection.h"
#include "Vector3D.h"

using namespace sivelab;
using namespace std;


CosineShader::CosineShader(Scene* scene, const Color& diffuse)
{
	m_scene = scene;
	m_diffuse = diffuse;
}


Color CosineShader::Shade(Intersection& intersection)
{
	LightList::const_iterator iter = m_scene->GetLightsBegin();
	LightList::const_iterator end = m_scene->GetLightsEnd();
	Color finalColor;

	Vector3D &normal = intersection.surfaceNormal;
	Vector3D intersectPoint = intersection.collidedRay.GetPositionAtTime(intersection.t);

	// Go through the entire list of lights.
	while (iter != end)
	{
		Vector3D lightDir = (*iter)->GetPosition() - intersectPoint;
		lightDir.normalize();

		// TODO: See if there is anything in between the intersection point and the light.

		// color += diffuse * lightRadiance * max(0, n dot l)
		double nDotL = max(0.0, normal.dot(lightDir));
		const Color &radiance = (*iter)->GetRadiance(intersectPoint);
		double r = m_diffuse.GetRed() * radiance.GetRed();
		double g = m_diffuse.GetGreen() * radiance.GetGreen();
		double b = m_diffuse.GetBlue() * radiance.GetBlue();
		finalColor.AddColors(Color(r, g, b).LinearMult(nDotL));

		// Go to next light.
		iter++;
	}

	return (finalColor);
}

