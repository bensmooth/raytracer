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
typedef std::vector<IObject*> ObjectList;
typedef std::vector<ILight*> LightList;

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

	/**
	 * Get a constant iterator to the beginning of the list of lights.
	 */
	LightList::const_iterator GetLightsBegin();

	/**
	 * Gets an iterator to one element past the end of the list of lights.
	 */
	LightList::const_iterator GetLightsEnd();

	friend class LightCreator;
	friend class CameraCreator;
	friend class ObjectCreator;
	friend class ShaderCreator;

private:
	ICamera *m_camera;
	ObjectList m_objects;
	LightList m_lights;
	ShaderMap m_shaders;
};

