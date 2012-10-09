#pragma once

#include <string>
#include <vector>
#include <map>
#include <cfloat>

#include "ICamera.h"
#include "IObject.h"
#include "IShader.h"
#include "ILight.h"
#include "RaytraceException.h"

class Image;
typedef std::map<std::string, IShader*> ShaderMap;
typedef std::map<std::string, IObject*> InstanceableMap;
typedef std::vector<IObject*> ObjectList;
typedef std::vector<ILight*> LightList;


/**
 * A small value that is useful for comparing doubles.
 */
const double EPSILON = 0.00005;

/**
 * Returns true if the two numbers are equal.
 */
#define EQUAL(a, b) (fabs((a) - (b)) < EPSILON)


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
	 * @param raysPerPixel The number of rays per pixel.  Must be a perfect square.
	 * @param useBvh Set to true to use a BVH structure.
	 * @param verbose Set to true if you want lots of information printed out during scene loading.
	 * @throws RaytraceException If something goes wrong.
	 */
	Scene(std::string filename, int raysPerPixel, bool useBvh, bool verbose);

	/**
	 * Frees all memory associated with the scene.
	 */
	~Scene();

	/**
	 * Renders the scene, generating a png file with the given name.
	 * @param image The image to render to.
	 * @param threadCount The number of threads to use when rendering the image.  Set to -1 to guess at the number that would be most effecient.
	 * @throws RaytraceException If something goes wrong.
	 */
	void Render(Image &image, int threadCount);

	/**
	 * Casts the given ray in the scene.
	 * @param ray The ray to cast through the scene.
	 * @param result If the ray collided with something, this will contain the result of the collision.
	 * @return True if the ray hit something.
	 */
	bool CastRay(const Ray &ray, Intersection &result, double maxT = DBL_MAX);

	/**
	 * Returns the appropriate color for the given intersection data.
	 * @param data The data to use to get the color of the object at the intersection.
	 * @param allowedReflectionCount Used to prevent recursive calls from going on forever.  When >0, does not call shader, and returns background color.
	 */
	Color ShadeIntersection(Intersection &data, int allowedReflectionCount = DEFAULT_REFLECTION_DEPTH);

	/**
	 * Casts the given ray into the scene and returns the color it hit.
	 * @param ray The ray to cast.
	 * @param result If this returns true, the color the ray ran into.
	 * @param intersect The intersection structure to use.
	 * @param maxT The maximum t value that should be considered.
	 * @param allowedReflectionCount Used to prevent recursive calls from going on forever.  When >0, does not call shader, and returns the background color.
	 * @return If true there was an intersection, or false otherwise.
	 */
	bool CastRayAndShade(const Ray &ray, Color &result, Intersection &intersect, double maxT = DBL_MAX, int allowedReflectionCount = DEFAULT_REFLECTION_DEPTH);

	/**
	 * Casts a shadow ray.
	 * @param light The light that will be testing for casting a shadow.
	 * @param intersection The position that we want to know if there is a shadow at.
	 * @return True if we are in shadow, false otherwise.
	 */
	bool CastShadowRay(ILight *light, Intersection &intersection);

	/**
	 * Casts an intersection ray.
	 * Decrements the number of allowed reflections.
	 * @param intersection The intersection that this ray will be bouncing from.
	 * @param roughness The roughness of the reflection.  0.0 is perfectly reflective.
	 */
	Color CastReflectionRay(Intersection &intersection, double roughness);

	/**
	 * Get a constant iterator to the beginning of the list of lights.
	 */
	LightList::const_iterator GetLightsBegin() const;

	/**
	 * Gets an iterator to one element past the end of the list of lights.
	 */
	LightList::const_iterator GetLightsEnd() const;

	/**
	 * Gets the amount of ambient light in the scene.
	 */
	const Color &GetAmbient() const;

	/**
	 * Set this to true to have verbose output printed out.
	 */
	bool VerboseOutput;

	friend class LightCreator;
	friend class CameraCreator;
	friend class ObjectCreator;
	friend class ShaderCreator;

private:
	/**
	 * Raytraces the given pixel.
	 */
	Color RaytracePixel(int x, int y);

	/**
	 * Same idea as public Render() above.
	 * Comes in single and multithreaded flavors.
	 */
	void RenderSingleThreaded(Image &image);
	void RenderMultiThreaded(Image &image, int threadCount);

	ICamera *m_camera;
	ObjectList m_objects;
	LightList m_lights;
	ShaderMap m_shaders;
	InstanceableMap m_instances;

	/**
	 * The amount of ambient light in the scene.
	 */
	Color m_ambient;

	/**
	 * The path to the directory that contains the scene file.
	 * Contains the trailing '/'
	 */
	std::string m_sceneFileDirectory;

	/**
	 * The default number of reflections that are allowed to happen for each ray.
	 */
	static const int DEFAULT_REFLECTION_DEPTH = 5;

	// Multithreaded rendering function.
	friend void *RenderThread(void *info);
};

