#include "Scene.h"

#include <cfloat>
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

using namespace sivelab;
using namespace std;


/**
 * Reads a string from the given map, under the given field.  Assumes that the field is required.
 */
void ReadString(map<string, SceneDataContainer> &map, string fieldName, string &out)
{
	std::map<string, SceneDataContainer>::iterator sdIter = map.find(fieldName);
	if (sdIter == map.end())
	{
		throw RaytraceException("Missing required field \"" + fieldName + "\" in scene file!");
	}

	out = sdIter->second.val;
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
    CameraCreator(Scene *scene)
	{
		m_scene = scene;
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

		cout << "Camera: name=" << name << ", type=" << type << endl;
		cout << "\tposition: " << position << endl;
		if (lookatSet)
		{
			cout << "\tlook at point: " << lookatPoint << endl;
			// Calculate viewing direction.
			viewDir = lookatPoint - position;
		}

		cout << "\tview direction: " << viewDir << endl;
        cout << "\tfocal length: " << focalLength << endl;
        cout << "\timage plane width: " << imagePlaneWidth << endl;

		m_scene->m_camera = new PerspectiveCamera(Ray(position, viewDir), focalLength, imagePlaneWidth);
    }

private:
	Scene *m_scene;	
};


/**
 * This creator is used for elements that we don't know how to create yet.
 */
class GenericCreator : public SceneElementCreator
{
public:
    GenericCreator(Scene *scene)
	{
		m_scene = scene;
	}
    ~GenericCreator() {}

    void instance(std::map<std::string, SceneDataContainer> &sdMap)
    {
        if (m_otype == CAMERA)
            std::cout << "SceneElementCreator type =  CAMERA" << std::endl;
        else if (m_otype == LIGHT)
            std::cout << "SceneElementCreator type =  LIGHT" << std::endl;
        else if (m_otype == SHADER)
            std::cout << "SceneElementCreator type =  SHADER" << std::endl;
        else if (m_otype == SHAPE)
            std::cout << "SceneElementCreator type =  SHAPE" << std::endl;
        else
            std::cout << "BOGUS TYPE" << std::endl;


        // Figure out how to instance the other things...
        std::map<std::string, SceneDataContainer>::const_iterator sdIter;
        for (sdIter=sdMap.begin(); sdIter != sdMap.end(); sdIter++)
        {
            SceneDataContainer sd = sdIter->second;
            std::cout << "Data [" << sdIter->first << "]: "
                      << sd.dtype << ", " << sd.name << ", "
                      << sd.val << ", " << sd.isSet << std::endl;
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

		cout << "Shader: name=" << name << ", type=" << type << endl;

		if (type == "Lambertian")
		{
			Vector3D diffuse;
			ReadVector(sdMap, "shader_diffuse", diffuse);

			// TODO: Actually create a lambertian shader.
			m_scene->m_shaders.insert(make_pair(name, new SolidShader(diffuse)));

			// Print info.
			cout << "\tdiffuse=" << diffuse << endl;
		}
		else
		{
			cout << "Shader type \"" << type << "\" is unknown!" << endl;
			map<string, SceneDataContainer>::const_iterator sdIter;
			for (sdIter=sdMap.begin(); sdIter != sdMap.end(); sdIter++)
			{
				SceneDataContainer sd = sdIter->second;
				cout << "Data [" << sdIter->first << "]: "
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


    void instance(std::map<std::string, SceneDataContainer> &sdMap)
    {
		string name;
		ReadString(sdMap, "shape_name", name);

		string type;
		ReadString(sdMap, "shape_type", type);

		cout << "Shape: name=" << name << ", type=" << type << endl;

		if (type == "sphere")
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
			m_scene->m_objects.push_back(new Sphere(center, radius, shaderRef));

			// Print details.
			cout << "\tShader: " << shaderName << endl;
			cout << "\tCenter: " << center << endl;
			cout << "\tRadius: " << radius << endl;
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
			m_scene->m_objects.push_back(new Box(minPoint, maxPoint, shaderRef));

			// Print info.
			cout << "\tShader: " << shaderName << endl;
			cout << "\tMin point: " << minPoint << endl;
			cout << "\tMax point: " << maxPoint << endl;
		}
		else
		{
			map<string, SceneDataContainer>::const_iterator sdIter;

			cout << "Unknown shape type: " << type << endl;
			for (sdIter=sdMap.begin(); sdIter != sdMap.end(); sdIter++)
			{
				SceneDataContainer sd = sdIter->second;
				cout << "Unknown Shape [" << sdIter->first << "]: "
					<< sd.dtype << ", " << sd.name << ", "
					<< sd.val << ", " << sd.isSet << endl;
			}
		}
    }

private:
	Scene *m_scene;	
	
};


Scene::Scene(std::string filename)
{
    m_camera = NULL;

    // Parse the XML scene file.
    XMLSceneParser xmlScene;

    // Register object creation handlers with the scene parser.
    CameraCreator cameraCreator(this);
	ObjectCreator objectCreator(this);
	GenericCreator lightCreator(this);
	ShaderCreator shaderCreator(this);
    xmlScene.registerCallback("camera", &cameraCreator);
    xmlScene.registerCallback("light", &lightCreator);
    xmlScene.registerCallback("shader", &shaderCreator);
    xmlScene.registerCallback("shape", &objectCreator);

    xmlScene.parseFile(filename);

    // Make sure they set a camera.
    if (m_camera == NULL)
    {
        throw RaytraceException("Camera not set!");
    }
}


Scene::~Scene()
{
	delete m_camera;
	m_camera = NULL;

	// Delete all objects.
	for (int i = 0; i < m_objects.size(); i++)
	{
		delete m_objects[i];
		m_objects[i] = NULL;
	}

	// Delete all lights.
	for (int i = 0; i < m_lights.size(); i++)
	{
		delete m_lights[i];
		m_lights[i] = NULL;
	}

	// Delete all shaders.
	map<string, IShader *>::iterator iter;
	for (iter = m_shaders.begin(); iter != m_shaders.end(); iter++)
	{
		delete iter->second;
		iter->second = NULL;
	}
}


void Scene::Render(std::string outfile, int imageWidth, int imageHeight)
{
    // This is the image we will be writing to.
    png::image<png::rgb_pixel> outputImage(imageWidth, imageHeight);

	m_camera->SetImageDimensions(imageWidth, imageHeight);

    // Get color values for each pixel.
    for (int imageX = 0; imageX < imageWidth; imageX++)
    {
        for (int imageY = 0; imageY < imageHeight; imageY++)
        {
            // Calculate ray we need to shoot for this pixel.
            Ray ray = m_camera->CalculateViewingRay(imageX, imageY);

            // See if ray intersects any objects.
            Intersection closestIntersect;
            closestIntersect.t = DBL_MAX;
            for (int i = 0; i < m_objects.size(); i++)
            {
                Intersection currentIntersect;
                if (m_objects.at(i)->Intersect(ray, currentIntersect) == true)
                {
                    // If this intersection is closer to the camera, this intersection is the one we care about.
                    if ((currentIntersect.t < closestIntersect.t) && (currentIntersect.t > 0))
                    {
                        closestIntersect = currentIntersect;
                    }
                }
            }

            // The color of the pixel.
            Color color;

            // If it did intersect, invoke closest object's shader.
            if (closestIntersect.t != DBL_MAX)
            {
                color = closestIntersect.object->GetShader()->Shade(closestIntersect);
            }
            else
            {
                // Set background color.
                color.SetRed(1.0);
                color.SetGreen(1.0);
                color.SetBlue(1.0);
            }

            // Save color to PNG structure.  Flip Y,  because we are rendering upside down.
            outputImage.set_pixel(imageX, imageHeight -1 - imageY, color.GetImageColor());
        }
    }

	// Write out PNG.
	outputImage.write(outfile);
}

