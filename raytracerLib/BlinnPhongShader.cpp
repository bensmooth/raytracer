#include "BlinnPhongShader.h"
#include "Intersection.h"

using namespace std;
using namespace sivelab;


BlinnPhongShader::BlinnPhongShader(Scene* scene, const Color& diffuse, const Color& specular, double phongExp, double mirrorCoef)
{
	m_scene = scene;
	m_diffuse = diffuse;
	m_specular = specular;
	m_phongExp = phongExp;
	m_mirrorCoef = mirrorCoef;
}


Color BlinnPhongShader::Shade(Intersection& intersection)
{
	const Vector3D &normal = intersection.surfaceNormal;
	Vector3D viewDir = intersection.collidedRay.GetDirection();
	viewDir *= -1.0;
	viewDir.normalize();
	Vector3D intersectPoint = intersection.collidedRay.GetPositionAtTime(intersection.t);

	Color finalDiffuseColor(0.0, 0.0, 0.0);
	Color finalSpecularColor(0.0, 0.0, 0.0);

	LightList::const_iterator currentLight;
	for (currentLight = m_scene->GetLightsBegin(); currentLight != m_scene->GetLightsEnd(); currentLight++)
	{
		// The direction to the light from the intersection point.
		Vector3D lightDir = (*currentLight)->GetPosition() - intersectPoint;
		lightDir.normalize();

		// The direction halfway between the light direction and the view direction.
		Vector3D halfDir = lightDir + viewDir;
		halfDir.normalize();
		
		// Always add the ambient amount of light.
		Color ambient = m_scene->GetAmbient();
		ambient.MultiplyColors(m_diffuse);
		finalDiffuseColor.AddColors(ambient);

		// See if we are in shadow.
		bool inShadow = m_scene->CastShadowRay(*currentLight, intersectPoint);
		if (inShadow == false)
		{
			// The radiance at the point of intersection.
			Color radiance = (*currentLight)->GetRadiance(intersectPoint);

			// Make sure it is above 0.
			double diffuseIntensity = max(0.0, lightDir.dot(normal));

			Color diffuseColor = radiance;
			diffuseColor.LinearMult(diffuseIntensity).MultiplyColors(m_diffuse);

			// Make sure it is above 0.
			double specularIntensity = pow(max(0.0, halfDir.dot(normal)), m_phongExp);

			// Calculate the specular color, and add it to the sum total of specular colors.
			Color specularColor = radiance;
			specularColor.LinearMult(specularIntensity).MultiplyColors(m_specular);
			finalSpecularColor.AddColors(specularColor);

			// Sum colors into the diffuse color.
			finalDiffuseColor.AddColors(diffuseColor);
		}
	}

	// Calculate the final color.
	Color finalColor;

	// Calculate color of reflected ray if m_mirrorCoef is not close to zero.
	if (m_mirrorCoef > EPSILON)
	{
		// The surface acts as a mirror.
		Color reflectedColor = m_scene->CastReflectionRay(intersection);
		// Combine the reflected color with the diffuse color.
		finalDiffuseColor.LinearMult(1.0 - m_mirrorCoef);
		reflectedColor.LinearMult(m_mirrorCoef);
		finalColor.AddColors(finalDiffuseColor).AddColors(reflectedColor);
	}
	else
	{
		// The surface does not act like a mirror.
		finalColor.AddColors(finalDiffuseColor);
	}

	// Combine the specular color with the result.
	finalColor.AddColors(finalSpecularColor);

	return (finalColor);
}


void BlinnPhongShader::SetMirrorCoef(double mirrorCoef)
{
	m_mirrorCoef = mirrorCoef;
}

