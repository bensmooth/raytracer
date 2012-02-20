#pragma once

#include "CosineShader.h"


class GlazeShader : public CosineShader
{
public:
	/**
	 * Constructs the shader with the given diffuse and mirror coefficient.
	 */
	GlazeShader(Scene *scene, Color diffuse, double mirrorCoef);

	virtual ~GlazeShader();

	virtual Color Shade(Intersection& intersection);

private:
	double m_mirrorCoef;
};

