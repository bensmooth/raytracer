#pragma once

#include "IShader.h"

class Scene;

class PerlinShader : public IShader
{
public:
	PerlinShader(Scene *scene);

	virtual ~PerlinShader();

	virtual Color Shade(Intersection& intersection);

	/**
	 * A dirty mirror shader.
	 */
	Color DirtyMirror(Intersection& intersection);

	/**
	 * A brushed metal shader.
	 */
	Color BrushedMetal(Intersection& intersection);

	/**
	 * A marble shader.
	 */
	Color Marble(Intersection& intersection);

private:
	Scene *m_scene;
};
