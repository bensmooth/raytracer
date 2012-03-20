#pragma once

#include "IShader.h"
#include "Scene.h"


class BlinnPhongShader : public IShader
{
public:
	/**
	 * Creates a Blinn-Phong shader with the given parameters.
	 * @param scene The scene this shader is in.
	 * @param diffuse The base diffuse color.
	 * @param specular The specular color.
	 * @param phongExp The glossiness of the surface.
	 * @param mirrorCoef The mirrorness of the surface.
	 */
	BlinnPhongShader(Scene *scene, const Color &diffuse, const Color &specular, double phongExp, double mirrorCoef);

	/**
	 * Sets the value of the mirror coeficient.
	 */
	void SetMirrorCoef(double mirrorCoef);

	virtual ~BlinnPhongShader() { }

	virtual Color Shade(Intersection& intersection);

private:
	Scene *m_scene;

	Color m_diffuse;

	Color m_specular;

	double m_phongExp;

	double m_mirrorCoef;
};
