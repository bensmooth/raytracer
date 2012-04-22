#include "GlazeShader.h"
#include "Scene.h"


GlazeShader::GlazeShader(Scene* scene, Color diffuse, double mirrorCoef): CosineShader(scene, diffuse)
{
	m_mirrorCoef = mirrorCoef;
}


GlazeShader::~GlazeShader()
{
}


Color GlazeShader::Shade(Intersection& intersection)
{
	// Calculate the base diffuse color.
	Color diffuseColor = CosineShader::Shade(intersection);

	// Shoot off a ray to get the reflection.
	Color reflectedColor = m_scene->CastReflectionRay(intersection, 0.0);

	// Combine the reflected color and the diffuse color.
	diffuseColor.LinearMult(1.0 - m_mirrorCoef);
	reflectedColor.LinearMult(m_mirrorCoef);
	Color finalColor;
	finalColor.AddColors(diffuseColor).AddColors(reflectedColor);

	return (finalColor);
}

