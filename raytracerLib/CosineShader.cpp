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

		// Always add the ambient amount of light.
		Color ambient = m_scene->GetAmbient();
		ambient.MultiplyColors(m_diffuse);
		finalColor.AddColors(ambient);

		// See if we are in shadow.
		if (m_scene->CastShadowRay(*iter, intersectPoint) == false)
		{
			// We are not in shadow.
			// color = diffuse * lightRadiance * max(0, n dot l)
			double nDotL = max(0.0, normal.dot(lightDir));
			const Color &radiance = (*iter)->GetRadiance(intersectPoint);
			Color diffuse;
			diffuse = m_diffuse;
			diffuse.MultiplyColors(radiance).LinearMult(nDotL);
			finalColor.AddColors(diffuse);
		}

		// Go to next light.
		iter++;
	}

	return (finalColor);
}

