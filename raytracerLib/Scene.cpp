#include "Scene.h"

#include <list>
#include <cmath>
#include <boost/filesystem.hpp>
#include <png++/image.hpp>
#include "png++/png.hpp"
#include "Vector3D.h"
#include "XMLSceneParser.h"
#include "Intersection.h"
#include "Color.h"
#include "PerspectiveCamera.h"
#include "Sphere.h"
#include "SolidShader.h"
#include "Box.h"
#include "CosineShader.h"
#include "PointLight.h"
#include "ReferenceTileShader.h"
#include "BlinnPhongShader.h"
#include "GlazeShader.h"
#include "ThreadPool.h"
#include "Cylinder.h"
#include "PerlinShader.h"
#include "BVHNode.h"
#include "Matrix.h"
#include "Instance.h"
#include "Mesh.h"
#include "AreaLight.h"
#include "Image.h"

/**
 * Converts degrees to radians.
 */
#define DEG_TO_RADS(degs) ((degs)*(M_PI/180.0))

using namespace sivelab;
using namespace std;


/**
 * Reads a string from the given map, under the given field.  Assumes that the field is required.
 * @param map The map of keys and scenedata objects.
 * @param fieldName The name of the field the string is under.
 * @param out On success, will contain the string read from the given fieldName.
 * @param required Set to true to throw an exception if the field is not found.
 * @remarks If field is not required, and is not found, "null" is placed in out.
 */
void ReadString(map<string, SceneDataContainer> &map, string fieldName, string &out, bool required = true)
{
	std::map<string, SceneDataContainer>::iterator sdIter = map.find(fieldName);
	if (sdIter == map.end())
	{
		// We didn't find it.
		if (required)
		{
			throw RaytraceException("Missing required field \"" + fieldName + "\" in scene file!");
		}
		else
		{
			// Field is not required, and we didn't find it.
			out = "null";
		}
	}
	else
	{
		// We found it.
		out = sdIter->second.val;
	}
}


/**
 * Reads a vector from the given map, under the given field.  Assumes that the field is required.
 */
void ReadVector(map<string, SceneDataContainer> &map, string fieldName, Vector3D &out)
{
	string content;
	ReadString(map, fieldName, content);

	istringstream buf;
	buf.str(content);
	buf >> out;
}


/**
 * Reads a double from the given map, under the given field.  Assumes that the field is required.
 */
void ReadDouble(map<string, SceneDataContainer> &map, string fieldName, double &out)
{
	// Initialize out to 0.0 in case we don't find the field.
	out = 0.0;

	string content;
	ReadString(map, fieldName, content);

	istringstream buf;
	buf.str(content);
	buf >> out;
}


/**
 * This creator is used to create the camera.
 */
class CameraCreator : public SceneElementCreator
{
public:
	CameraCreator(Scene *scene, int raysPerPixel)
	{
		m_scene = scene;
		m_raysPerPixel = raysPerPixel;
	}
    ~CameraCreator() {}

    void instance(map<string, SceneDataContainer> &sdMap)
    {
		istringstream buf;
		map<string, SceneDataContainer>::iterator sdIter;

		string name, type;
		Vector3D position, viewDir, lookatPoint;
		bool lookatSet = false;
		float focalLength;
		float imagePlaneWidth;

		ReadString(sdMap, "camera_name", name);

		ReadString(sdMap, "camera_type", type);

		ReadVector(sdMap, "camera_position", position);

		sdIter = sdMap.find("camera_viewDir");
		assert( sdIter != sdMap.end() );
		buf.str(sdIter->second.val);
		buf >> viewDir;
		buf.clear();

		sdIter = sdMap.find("camera_lookatPoint");
		assert( sdIter != sdMap.end() );
		lookatSet = sdIter->second.isSet;
		if (lookatSet)
		{
			buf.str(sdIter->second.val);
			buf >> lookatPoint;
			buf.clear();
		}

		sdIter = sdMap.find("camera_focalLength");
		assert( sdIter != sdMap.end() );
		buf.str(sdIter->second.val);
		buf >> focalLength;
		buf.clear();

		sdIter = sdMap.find("camera_imagePlaneWidth");
		assert( sdIter != sdMap.end() );
		buf.str(sdIter->second.val);
		buf >> imagePlaneWidth;
		buf.clear();

		if (m_scene->VerboseOutput)
		{
			cout << "Camera: name=" << name << ", type=" << type << endl;
			cout << "\tposition: " << position << endl;
		}

		if (lookatSet)
		{
			if (m_scene->VerboseOutput)
			{
				cout << "\tlook at point: " << lookatPoint << endl;
			}
			// Calculate viewing direction.
			viewDir = lookatPoint - position;
		}

		if (m_scene->VerboseOutput)
		{
			cout << "\tview direction: " << viewDir << endl;
			cout << "\tfocal length: " << focalLength << endl;
			cout << "\timage plane width: " << imagePlaneWidth << endl;
			cout << "\trays per pixel: " << m_raysPerPixel << endl;
		}

		m_scene->m_camera = new PerspectiveCamera(Ray(position, viewDir), focalLength, imagePlaneWidth, m_raysPerPixel);
    }

private:
	Scene *m_scene;
	int m_raysPerPixel;
};


/**
 * This creator is used for lights.
 */
class LightCreator : public SceneElementCreator
{
public:
    LightCreator(Scene *scene)
	{
		m_scene = scene;
	}
    ~LightCreator() {}


    void instance(std::map<std::string, SceneDataContainer> &sdMap)
    {
		string type;
		ReadString(sdMap, "light_type", type);

		if (m_scene->VerboseOutput)
		{
			cout << "Light: type=" << type << endl;
		}

		if (type == "point")
		{
			Vector3D position;
			ReadVector(sdMap, "light_position", position);

			Vector3D intensity;
			ReadVector(sdMap, "light_intensity", intensity);

			m_scene->m_lights.push_back(new PointLight(position, intensity));

			// Print info if verbose.
			if (m_scene->VerboseOutput)
			{
				cout << "\tPosition=" << position << endl;
				cout << "\tIntensity=" << intensity << endl;
			}
		}
		else if (type == "area")
		{
			Vector3D position;
			ReadVector(sdMap, "light_position", position);

			Vector3D intensity;
			ReadVector(sdMap, "light_intensity", intensity);

			Vector3D normal;
			ReadVector(sdMap, "light_normal", normal);

			double width;
			ReadDouble(sdMap, "light_width", width);

			double height;
			ReadDouble(sdMap, "light_length", height);

			m_scene->m_lights.push_back(new AreaLight(position, normal, intensity, width, height));

			// Print info if verbose.
			if (m_scene->VerboseOutput)
			{
				cout << "\tPosition=" << position << endl;
				cout << "\tIntensity=" << intensity << endl;
				cout << "\tNormal=" << normal << endl;
				cout << "\tWidth=" << width << endl;
				cout << "\tLength=" << height << endl;
			}
		}
		else
		{
			cerr << "Light type \"" << type << "\" is unknown!" << endl;
			map<string, SceneDataContainer>::const_iterator sdIter;
			for (sdIter=sdMap.begin(); sdIter != sdMap.end(); sdIter++)
			{
				SceneDataContainer sd = sdIter->second;
				cerr << "Data [" << sdIter->first << "]: "
				<< sd.dtype << ", " << sd.name << ", "
				<< sd.val << ", " << sd.isSet << endl;
			}
		}
	}
private:
	Scene *m_scene;	
	
};


/**
 * This creator handles all shader definitions.
 */
class ShaderCreator : public SceneElementCreator
{
public:
	ShaderCreator(Scene *scene)
	{
		m_scene = scene;
	}
	~ShaderCreator() {}


	void instance(map<string, SceneDataContainer> &sdMap)
	{
		string name;
		ReadString(sdMap, "shader_name", name);

		string type;
		ReadString(sdMap, "shader_type", type);

		if (m_scene->VerboseOutput)
		{
			cout << "Shader: name=" << name << ", type=" << type << endl;
		}

		if (type == "Lambertian")
		{
			Vector3D diffuse;
			ReadVector(sdMap, "shader_diffuse", diffuse);

			m_scene->m_shaders.insert(make_pair(name, new CosineShader(m_scene, diffuse)));

			// Print info if verbose.
			if (m_scene->VerboseOutput)
			{
				cout << "\tdiffuse=" << diffuse << endl;
			}
		}
		else if (type == "BlinnPhong")
		{
			Vector3D diffuse;
			ReadVector(sdMap, "shader_diffuse", diffuse);

			Vector3D specular;
			ReadVector(sdMap, "shader_specular", specular);

			double phongExp = 0;
			ReadDouble(sdMap, "shader_phongExp", phongExp);

			double mirrorCoef = 0;
			ReadDouble(sdMap, "shader_mirrorCoef", mirrorCoef);

			double roughness = 0;
			ReadDouble(sdMap, "shader_roughness", roughness);

			// Add shader to list.
			m_scene->m_shaders.insert(make_pair(name, new BlinnPhongShader(m_scene, diffuse, specular, phongExp, mirrorCoef, roughness)));

			// Print parameters out if verbose.
			if (m_scene->VerboseOutput)
			{
				cout << "\tDiffuse=" << diffuse << endl;
				cout << "\tSpecular=" << specular << endl;
				cout << "\tPhong Exp=" << phongExp << endl;
				cout << "\tMirror Coef=" << mirrorCoef << endl;
				cout << "\t" << endl;
			}
		}
		else if (type == "Glaze")
		{
			Vector3D diffuse;
			ReadVector(sdMap, "shader_diffuse", diffuse);

			double mirrorCoef = 0;
			ReadDouble(sdMap, "shader_mirrorCoef", mirrorCoef);

			// Add shader to list.
			m_scene->m_shaders.insert(make_pair(name, new GlazeShader(m_scene, diffuse, mirrorCoef)));

			// Print parameters out if verbose.
			if (m_scene->VerboseOutput)
			{
				cout << "\tDiffuse=" << diffuse << endl;
				cout << "\tMirror Coef=" << mirrorCoef << endl;
			}
		}
		else if (type == "Tile")
		{
			m_scene->m_shaders.insert(make_pair(name, new ReferenceTileShader()));
		}
		else if (type == "Perlin")
		{
			m_scene->m_shaders.insert(make_pair(name, new PerlinShader(m_scene)));
		}
		else
		{
			cerr << "Shader type \"" << type << "\" is unknown!" << endl;
			map<string, SceneDataContainer>::const_iterator sdIter;
			for (sdIter=sdMap.begin(); sdIter != sdMap.end(); sdIter++)
			{
				SceneDataContainer sd = sdIter->second;
				cerr << "Data [" << sdIter->first << "]: "
				<< sd.dtype << ", " << sd.name << ", "
				<< sd.val << ", " << sd.isSet << endl;
			}
		}
	}

private:
	Scene *m_scene;
};


/**
 * This class is used to create scene objects.
 */
class ObjectCreator : public SceneElementCreator
{
public:
    ObjectCreator(Scene *scene)
	{
		m_scene = scene;
	}

	~ObjectCreator() {}

	/**
	 * Resolves the given reference to the shader, throwing an exception if no shader with the given ref has been defined.
	 */
	IShader *ResolveShaderRef(string objectName, string shaderName)
	{
		ShaderMap::iterator shaderIter = m_scene->m_shaders.find(shaderName);
		if (shaderIter == m_scene->m_shaders.end())
		{
			throw RaytraceException("Unable to find the shader ref \"" + shaderName + "\" for object \"" + objectName + "\" in the list of shaders!");
		}

		return (shaderIter->second);
	}


	/**
	* Resolves the given reference to the instance, throwing an exception if no instance with the given ref has been defined.
	*/
	IObject *ResolveInstanceRef(string objectName, string instName)
	{
		InstanceableMap::iterator instanceIter = m_scene->m_instances.find(instName);
		if (instanceIter == m_scene->m_instances.end())
		{
			throw RaytraceException("Unable to find the instance ref \"" + instName + "\" for object \"" + objectName + "\" in the list of instanceable object!");
		}

		return (instanceIter->second);
	}


	void instance(std::map<std::string, SceneDataContainer> &sdMap)
	{
		// This is set to true if this object is to be added to the list of
		// instanceable objects instead of the list of objects in the scene.
		bool isInstanceable = (m_otype == INSTANCE);

		string name;
		ReadString(sdMap, "shape_name", name);

		string type;
		ReadString(sdMap, "shape_type", type);

		IObject *toAdd = NULL;

		if (m_scene->VerboseOutput)
		{
			cout << "Shape: name=" << name << ", type=" << type << endl;
		}

		if (type == "instance")
		{
			std::string tname;
			map<string, SceneDataContainer>::iterator sdIter = sdMap.find("transform_name");
			// See if the instance even has transformations.
			if (sdIter->second.isSet)
			{
				stringstream buf;
				tname = sdIter->second.val;

				stack<Matrix> transfStack;

				// Build up transformation matrix.
				for (sdIter = sdMap.begin(); sdIter != sdMap.end(); sdIter++)
				{
					// The current transformation operation.
					Matrix currentTrans;
					if (sdIter->second.name == "translate")
					{
						sivelab::Vector3D trans;
						buf.str(sdIter->second.val);
						buf >> trans;
						buf.clear();
						currentTrans.ConstructTranslate(trans[0], trans[1], trans[2]);
					}
					else if (sdIter->second.name == "rotateX")
					{
						float rot;
						buf.str(sdIter->second.val);
						buf >> rot;
						buf.clear();
						currentTrans.ConstructRotationX(DEG_TO_RADS(rot));
					}
					else if (sdIter->second.name == "rotateY")
					{
						float rot;
						buf.str(sdIter->second.val);
						buf >> rot;
						buf.clear();
						currentTrans.ConstructRotationY(DEG_TO_RADS(rot));
					}
					else if (sdIter->second.name == "rotateZ")
					{
						float rot;
						buf.str(sdIter->second.val);
						buf >> rot;
						buf.clear();
						currentTrans.ConstructRotationZ(DEG_TO_RADS(rot));
					}
					else if (sdIter->second.name == "scale")
					{
						sivelab::Vector3D scale;
						buf.str(sdIter->second.val);
						buf >> scale;
						buf.clear();
						currentTrans.ConstructScale(scale[0], scale[1], scale[2]);
					}
					else
					{
						// This is not a transformation element.
						continue;
					}
					transfStack.push(currentTrans);
				}

				// Build the final resultant matrix.
				Matrix transformation;
				transformation.ConstructIdentity();
				while (!transfStack.empty())
				{
					transformation = transfStack.top() * transformation;
					transfStack.pop();
				}

				// Read the original object name.
				string instName;
				ReadString(sdMap, "shape_id", instName);

				// Extract object to be instanced from the map of instanceable objects.
				IObject *original = ResolveInstanceRef(name, instName);

				// Associate this instance with the correct shader.
				string shaderName;
				ReadString(sdMap, "shader_ref", shaderName, false);
				// If shader name is set to NULL, set the shader to NULL, so that the base object's shader is used.
				IShader *shaderRef;
				if (shaderName == "null" || shaderName == "NULL")
				{
					shaderRef = NULL;
				}
				else
				{
					// Don't use the base object's shader.
					shaderRef = ResolveShaderRef(name, shaderName);
				}

				toAdd = new InstanceObject(transformation, original, shaderRef);
			}
		}
		else if (type == "mesh")
		{
			string filename;
			ReadString(sdMap, "shape_file", filename);

			// Associate this object with the correct shader.
			string shaderName;
			ReadString(sdMap, "shader_ref", shaderName);
			IShader *shaderRef = ResolveShaderRef(name, shaderName);

			// Load the object file relative to the location of the scene file.
			toAdd = new Mesh(m_scene->m_sceneFileDirectory + filename, shaderRef);
		}
		else if (type == "sphere")
		{
			Vector3D center;
			ReadVector(sdMap, "shape_center", center);

			double radius;
			ReadDouble(sdMap, "shape_radius", radius);

			// Associate this object with the correct shader.
			string shaderName;
			ReadString(sdMap, "shader_ref", shaderName);
			IShader *shaderRef = ResolveShaderRef(name, shaderName);

			// Add sphere to the list of objects.
			toAdd = new Sphere(center, radius, shaderRef);

			// Print details if verbose.
			if (m_scene->VerboseOutput)
			{
				cout << "\tShader: " << shaderName << endl;
				cout << "\tCenter: " << center << endl;
				cout << "\tRadius: " << radius << endl;
			}
		}
		else if (type == "cylinder")
		{
			Vector3D center;
			ReadVector(sdMap, "shape_center", center);

			double radius;
			ReadDouble(sdMap, "shape_radius", radius);

			double height;
			ReadDouble(sdMap, "shape_height", height);

			// Associate this object with the correct shader.
			string shaderName;
			ReadString(sdMap, "shader_ref", shaderName);
			IShader *shaderRef = ResolveShaderRef(name, shaderName);

			// Add sphere to the list of objects.
			toAdd = new Cylinder(shaderRef, center, height, radius);

			// Print details if verbose.
			if (m_scene->VerboseOutput)
			{
				cout << "\tShader: " << shaderName << endl;
				cout << "\tCenter: " << center << endl;
				cout << "\tRadius: " << radius << endl;
				cout << "\tHeight: " << height << endl;
			}
		}
		else if(type == "box")
		{
			// Read shader.
			string shaderName;
			ReadString(sdMap, "shader_ref", shaderName);
			IShader *shaderRef = ResolveShaderRef(name, shaderName);

			// Read minimum point.
			Vector3D minPoint;
			ReadVector(sdMap, "shape_minPt", minPoint);

			// Read maximum point.
			Vector3D maxPoint;
			ReadVector(sdMap, "shape_maxPt", maxPoint);

			// Construct box and add it to the list of objects.
			toAdd = new Box(minPoint, maxPoint, shaderRef);

			// Print info if we are verbose.
			if (m_scene->VerboseOutput)
			{
				cout << "\tShader: " << shaderName << endl;
				cout << "\tMin point: " << minPoint << endl;
				cout << "\tMax point: " << maxPoint << endl;
			}
		}
		else
		{
			map<string, SceneDataContainer>::const_iterator sdIter;

			cerr << "Unknown shape type: " << type << endl;
			for (sdIter=sdMap.begin(); sdIter != sdMap.end(); sdIter++)
			{
				SceneDataContainer sd = sdIter->second;
				cerr << "Unknown Shape [" << sdIter->first << "]: "
					<< sd.dtype << ", " << sd.name << ", "
					<< sd.val << ", " << sd.isSet << endl;
			}
		}

		// Ensure we set the object to add.
		if (toAdd == NULL)
		{
			cout << "Warning, shape: name=" << name << ", type=" << type << " not loaded!" << endl;
		}
		else
		{
			// Decide which list we should add this object to.
			if (isInstanceable)
			{
				// It was an instanceable object.
				m_scene->m_instances.insert(make_pair(name, toAdd));
			}
			else
			{
				// It was a regular object.
				m_scene->m_objects.push_back(toAdd);
			}
		}
    }

private:
	Scene *m_scene;	
};


Scene::Scene(std::string filename, int raysPerPixel, bool useBvh, bool verbose)
{
	VerboseOutput = verbose;
	m_ambient = Color(0.1, 0.1, 0.1);
	m_camera = NULL;

	// Extract the path to the scene file for use in loading other included files like textures or meshes.
	boost::filesystem::path pathToSceneFile(filename.c_str());
	m_sceneFileDirectory = pathToSceneFile.branch_path().string() + '/';

	if (verbose)
	{
		cout << "Scene file directory is: " << m_sceneFileDirectory << endl;
	}

	// Register object creation handlers with the scene parser.
	CameraCreator cameraCreator(this, raysPerPixel);
	ObjectCreator objectCreator(this);
	LightCreator lightCreator(this);
	ShaderCreator shaderCreator(this);
	XMLSceneParser xmlScene;
	xmlScene.registerCallback("camera", &cameraCreator);
	xmlScene.registerCallback("light", &lightCreator);
	xmlScene.registerCallback("shader", &shaderCreator);
	xmlScene.registerCallback("shape", &objectCreator);
	xmlScene.registerCallback("instance", &objectCreator);

	xmlScene.parseFile(filename);

	// Make sure they set a camera.
	if (m_camera == NULL)
	{
		throw RaytraceException("Camera not set!");
	}

	// If they wanted to use a BVH, build it up.
	if (useBvh)
	{
		BVHNode *root = BVHNode::ConstructBVH(m_objects);

		// All objects are now in the BVH, so we can clear out the list of objects.
		m_objects.resize(1);
		m_objects[0] = root;
	}
}


Scene::~Scene()
{
	delete m_camera;
	m_camera = NULL;

	// Delete all objects.
	for (size_t i = 0; i < m_objects.size(); i++)
	{
		delete m_objects[i];
		m_objects[i] = NULL;
	}

	// Delete all lights.
	for (size_t i = 0; i < m_lights.size(); i++)
	{
		delete m_lights[i];
		m_lights[i] = NULL;
	}

	// Delete all instanceable objects.
	InstanceableMap::iterator instanceIter;
	for (instanceIter = m_instances.begin(); instanceIter != m_instances.end(); instanceIter++)
	{
		delete instanceIter->second;
		instanceIter->second = NULL;
	}

	// Delete all shaders.
	map<string, IShader *>::iterator iter;
	for (iter = m_shaders.begin(); iter != m_shaders.end(); iter++)
	{
		delete iter->second;
		iter->second = NULL;
	}
}


Color Scene::RaytracePixel(int x, int y)
{
	// Calculate the rays we need to shoot for this pixel.
	RayList rayList = m_camera->CalculateViewingRays(x, y);

	int raysPerPixel = rayList.size();

	// Fill our intersection structure with samples.
	Intersection intersect;
	intersect.areaLightSamples.Generate(raysPerPixel);

	Color finalColor;
	for (int i = 0; i < raysPerPixel; i++)
	{
		const Ray &ray = rayList[i];
		// See if ray intersects any objects.
		Color rayColor;
		if (CastRayAndShade(ray, rayColor, intersect) == false)
		{
			// We hit nothing, add in the background color.
			finalColor.AddColors(Color(0.0, 0.0, 0.0));
		}
		else
		{
			// We hit something.
			finalColor.AddColors(rayColor);
		}

		// Advance to the next sample.
		intersect.areaLightSamples.Next();
	}

	// Take average of each ray's color.
	finalColor.LinearMult(1.0/raysPerPixel);
	return (finalColor);
}


void Scene::Render(string outfile, int imageWidth, int imageHeight, int threadCount)
{
	// See if we are guessing the number of threads.
	if (threadCount <= 0)
	{
		threadCount = ThreadEngine::ThreadPool::GetNumberOfProcessors();
	}

	if (threadCount == 1)
	{
		RenderSingleThreaded(outfile, imageWidth, imageHeight);
	}
	else
	{
		RenderMultiThreaded(outfile, imageWidth, imageHeight, threadCount);
	}
}


void Scene::RenderSingleThreaded(std::string outfile, int imageWidth, int imageHeight)
{
	// This is the image we will be writing to.
	Image outputImage(imageWidth, imageHeight);

	m_camera->SetImageDimensions(imageWidth, imageHeight);

	// Get color values for each pixel.
	for (int imageX = 0; imageX < imageWidth; imageX++)
	{
		for (int imageY = 0; imageY < imageHeight; imageY++)
		{
			Color color = RaytracePixel(imageX, imageY);

			// Save color to image structure.  Flip Y,  because we are rendering upside down.
			outputImage(imageX, imageHeight -1 - imageY) = color;
		}
	}

	// Write out to disk.
	outputImage.WriteToDisk(outfile);
}


/**
 * A structure that contains everything needed for a thread to render a portion of an image.
 */
struct RenderingThreadInfo
{
	/**
	 * The scene we are rendering.
	 */
	Scene *scene;

	/**
	 * The image that will be written to.
	 */
	Image *outputImage;

	/**
	 * The dimensions of the rectangle required to be rendered by the thread.
	 */
	int startX, startY, width, height;

	/**
	 * The final image height.  Used to flip the image upside down when writing pixels.
	 */
	int finalImageHeight;
};


/**
 * The function that renders a portion of an image.
 * @param info Pointer to the RenderingThreadInfo structure for this section of image.
 */
void *RenderThread(void *info)
{
	// Extract our rendering thread information.
	RenderingThreadInfo *threadInfo = (RenderingThreadInfo*)info;
	int endX = threadInfo->width + threadInfo->startX;
	int endY = threadInfo->height + threadInfo->startY;

	// Get color values for each pixel we have been assigned to render.
	for (int imageX = threadInfo->startX; imageX < endX; imageX++)
	{
		for (int imageY = threadInfo->startY; imageY < endY; imageY++)
		{
			Color color = threadInfo->scene->RaytracePixel(imageX, imageY);

			// Save color to PNG structure.  Flip Y,  because we are rendering upside down.
			threadInfo->outputImage->operator()(imageX, threadInfo->finalImageHeight -1 - imageY) = color;
		}
	}

	return (NULL);
}


void Scene::RenderMultiThreaded(string outfile, int imageWidth, int imageHeight, int threadCount)
{
	// The image we will be rendering to.
	Image outputImage(imageWidth, imageHeight);

	// Tell the camera how big the image is.
	m_camera->SetImageDimensions(imageWidth, imageHeight);

	// The thread pool we will be using to render the scene.
	ThreadEngine::ThreadPool renderPool(threadCount);

	// Start pool so that jobs will be started immediately on adding them.
	renderPool.StartProcessing();

	// The list of job information for each image chunk.
	RenderingThreadInfo *threadInfoList = new RenderingThreadInfo[imageHeight];

	// Divide screen up into chunks.
	for (int y = 0; y < imageHeight; y++)
	{
		RenderingThreadInfo &renderInfo = threadInfoList[y];
		renderInfo.startX = 0;
		renderInfo.startY = y;
		renderInfo.width = imageWidth;
		renderInfo.height = 1;
		renderInfo.finalImageHeight = imageHeight;
		renderInfo.scene = this;
		renderInfo.outputImage = &outputImage;

		// Add a job to the pool for each chunk.
		renderPool.AddJob(RenderThread, &threadInfoList[y]);
	}

	// Wait for all jobs to be completed.
	renderPool.JoinAll();

	// Free list of renderinfos.
	delete[] threadInfoList;

	// Write out PNG.
	outputImage.WriteToDisk(outfile);
}


inline bool Scene::CastRayAndShade(const Ray& ray, Color& result, Intersection &intersect, double maxT, int allowedReflectionCount)
{
	if (CastRay(ray, intersect, maxT) == true)
	{
 		result = ShadeIntersection(intersect, allowedReflectionCount);
		return (true);
	}
	else
	{
		return (false);
	}
}


bool Scene::CastShadowRay(ILight* light, Intersection &intersection)
{
	Vector3D intersectPoint = intersection.collidedRay.GetPositionAtTime(intersection.t);

	// If this is an area light, we need to sample it at the right spot.
	Vector3D lightPos;
	AreaLight *areaLight = dynamic_cast<AreaLight*>(light);
	if (areaLight != NULL)
	{
		// Grab the sample of the light that we want.
		lightPos = areaLight->GetPosition(intersection.areaLightSamples.GetCurrentSample());
	}
	else
	{
		// This is a regular light.
		lightPos = light->GetPosition();
	}

	// Construct ray from the intersection point to the light.
	Ray shadowRay(intersectPoint, lightPos - intersectPoint);

	// Step ray towards light by a small amount to overcome numerical inaccuracy.
	shadowRay.SetPosition(shadowRay.GetPositionAtTime(EPSILON));

	// Cast ray into scene with max t of 1.0, so that objects beyond the light are not taken into account.
	Intersection unused;
	return (CastRay(shadowRay, unused, 1.0));
}


Color Scene::CastReflectionRay(Intersection& intersection, double roughness)
{
	// Terminate if we have bounced around too much.
	if (intersection.allowedReflectionCount <= 0)
	{
		return (Color(0.5, 0.5, 0.5));
	}

	intersection.allowedReflectionCount--;

	// Calculate the ray we need to shoot for the reflection.
	Vector3D intersectPoint = intersection.collidedRay.GetPositionAtTime(intersection.t);
	const Vector3D &normal = intersection.surfaceNormal;
	Vector3D viewRay = intersection.collidedRay.GetDirection();
	Vector3D rayDirection = viewRay - 2 * (viewRay.dot(normal)) * normal;

	// If roughness is not equal to 0, we need to perturb the reflected ray.
	if (!EQUAL(roughness, 0.0))
	{
		// Calculate a basis around the reflected ray.
		Basis reflectedBasis;
		reflectedBasis.Calculate(rayDirection);

		// Compute perturbed direction.
		Sample sample = intersection.areaLightSamples.GetCurrentSample();
		double u = -roughness/2.0 + sample.second*roughness;
		double v = -roughness/2.0 + sample.first*roughness;
		rayDirection = rayDirection + u*reflectedBasis.GetU() + v*reflectedBasis.GetV();
	}

	Ray reflectedRay(intersectPoint, rayDirection);

	// Step reflectedRay out of any object it may be inside of.
	reflectedRay.SetPosition(reflectedRay.GetPositionAtTime(EPSILON));

	Color rayColor;
	CastRayAndShade(reflectedRay, rayColor, intersection, numeric_limits<double>::max(), intersection.allowedReflectionCount);
	return (rayColor);
}


bool Scene::CastRay(const Ray& ray, Intersection &result, double maxT)
{
	// Ensure that any in-data (like area light samples) doesn't get wiped.
	JitteredSampler areaLightSamples = result.areaLightSamples;

	Intersection closestIntersect;
	closestIntersect.t = maxT;
	for (size_t i = 0; i < m_objects.size(); i++)
	{
		Intersection currentIntersect;
		if (m_objects.at(i)->Intersect(ray, currentIntersect) == true)
		{
			// If this intersection is closer to the camera, and in front of it, this intersection is the one we care about.
			if ((currentIntersect.t < closestIntersect.t) && (currentIntersect.t > 0))
			{
				closestIntersect = currentIntersect;
			}
		}
	}

	if (closestIntersect.t != maxT)
	{
		// We had an intersection.
		result = closestIntersect;

		// Copy in-data back into result.
		result.areaLightSamples = areaLightSamples;

		return (true);
	}
	else
	{
		// No intersection.
		return (false);
	}
}


inline Color Scene::ShadeIntersection(Intersection& data, int allowedReflectionCount)
{
	// Invoke the object's shader.
	// Set the intersection data's reflection count.
	data.allowedReflectionCount = allowedReflectionCount;
	return (data.object->GetShader()->Shade(data));
}


LightList::const_iterator Scene::GetLightsBegin() const
{
	return (m_lights.begin());
}


LightList::const_iterator Scene::GetLightsEnd() const
{
	return (m_lights.end());
}


const Color& Scene::GetAmbient() const
{
	return (m_ambient);
}

