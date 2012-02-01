#pragma once

#include <string>

#include "ICamera.h"
#include "IObject.h"
#include "IShader.h"
#include "ILight.h"
#include "RaytraceException.h"

/**
 * Represents the entirety of a scene.
 * Is also responsible for loading and rendering a scene.
 */
class Scene
{
public:
	/**
	 * Loads a scene from the given XML file.
	 * @param filename The filename to load from.
	 * @throws RaytraceException when something goes wrong.
	 */
	Scene(std::string filename);


	/**
	 * Renders the scene, generating the following png file.
	 */
	void Render(std::string outfile);

private:
	ICamera *camera;
	vector<const IObject const *> objects;
	vector<const ILight const *> lights;
	vector<const IShader const *> shaders;
};

