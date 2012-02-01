#pragma once

#include <string>
#include <vector>

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
	 * @throws RaytraceException If something goes wrong.
	 */
	Scene(std::string filename);


	/**
	 * Renders the scene, generating a png file with the given name.
	 * @param outfile The output filename.
	 * @param imageWidth The width of the image.
	 * @param imageHeight The height of the image.
	 * @throws RaytraceException If something goes wrong.
	 */
	void Render(std::string outfile, int imageWidth, int imageHeight);

private:
	ICamera *m_camera;
	std::vector<IObject *> m_objects;
	std::vector<ILight *> m_lights;
	std::vector<IShader *> m_shaders;
};

