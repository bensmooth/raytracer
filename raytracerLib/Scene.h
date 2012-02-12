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
	 * Casts the given ray in the scene.
	 * @param ray The ray to cast through the scene.
	 * @param result If the ray collided with something, this will contain the result of the collision.
	 * @return True if the ray hit something.
	 */
	bool CastRay(const Ray &ray, Intersection &result);

	/**
	 * Returns the appropriate color for the given intersection data.
	 * @param data The data to use to get the color of the object at the intersection.
	 */
	Color ShadeIntersection(Intersection &data);

	/**
	 * Casts the given ray into the scene and returns the color it hit.
	 * @param ray The ray to cast.
	 * @param result If this returns true, the color the ray ran into.
	 * @return If true there was an intersection, or false otherwise.
	 */
	bool CastRayAndShade(const Ray &ray, Color &result);

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

