#pragma once

#include <string>
#include <vector>
#include <map>

#include "ICamera.h"
#include "IObject.h"
#include "IShader.h"
#include "ILight.h"
#include "RaytraceException.h"

typedef std::map<std::string, IShader*> ShaderMap;

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
	 * Frees all memory associated with the scene.
	 */
	~Scene();

	/**
	 * Renders the scene, generating a png file with the given name.
	 * @param outfile The output filename.
	 * @param imageWidth The width of the image.
	 * @param imageHeight The height of the image.
	 * @throws RaytraceException If something goes wrong.
	 */
	void Render(std::string outfile, int imageWidth, int imageHeight);


	friend class GenericCreator;
	friend class CameraCreator;
	friend class ObjectCreator;
	friend class ShaderCreator;

private:
	ICamera *m_camera;
	std::vector<IObject *> m_objects;
	std::vector<ILight *> m_lights;
	ShaderMap m_shaders;
};

