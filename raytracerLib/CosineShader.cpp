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

		// This will contain the calculated color that the current light is producing on the surface.
		Color color;

		// See if there is anything in between the intersection point and the light.
		Ray ray(intersectPoint, lightDir);
		// Scootch the ray forward a bit, so that it doesn't get stuck inside the object it just came out of.
		ray.SetPosition(ray.GetPositionAtTime(EPSILON));

		Intersection intersect;
		if (m_scene->CastRay(ray, intersect) == true)
		{
			// There was an intersection, use the color of the object.
			color = m_diffuse;
			color.LinearMult(0.25);
		}
		else
		{
			// No intersection.
			// color = diffuse * lightRadiance * max(0, n dot l)
			double nDotL = max(0.0, normal.dot(lightDir));
			const Color &radiance = (*iter)->GetRadiance(intersectPoint);
			color.SetRed(m_diffuse.GetRed() * radiance.GetRed());
			color.SetGreen(m_diffuse.GetGreen() * radiance.GetGreen());
			color.SetBlue(m_diffuse.GetBlue() * radiance.GetBlue());
			color.LinearMult(nDotL);
		}

		finalColor.AddColors(color);

		// Go to next light.
		iter++;
	}

	return (finalColor);
}

