#pragma once

#include "IShader.h"

class Scene;


/**
 * A cosine (Lambertian) shader.
 */
class CosineShader : public IShader
{
public:
	/**
	 * Constructs the shader.
	 * @param scene The scene this shader is associated with.
	 * @param diffuse The diffuse color of the shader.
	 */
	CosineShader(Scene *scene, const Color &diffuse);

	virtual Color Shade(Intersection  &intersection);

private:
	/**
	 * The diffuse color.
	 */
	Color m_diffuse;

	/**
	 * The scene we are associated with.
	 */
	Scene *m_scene;
};
