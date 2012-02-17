#include "Scene.h"

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

		cout << "Light: type=" << type << endl;

		if (type == "point")
		{
			Vector3D position;
			ReadVector(sdMap, "light_position", position);

			Vector3D intensity;
			ReadVector(sdMap, "light_intensity", intensity);

			m_scene->m_lights.push_back(new PointLight(position, intensity));

			// Print info.
			cout << "\tPosition=" << position << endl;
			cout << "\tIntensity=" << intensity << endl;
		}
		else
		{
			cout << "Light type \"" << type << "\" is unknown!" << endl;
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

			m_scene->m_shaders.insert(make_pair(name, new CosineShader(m_scene, diffuse)));

			// Print info.
			cout << "\tdiffuse=" << diffuse << endl;
		}
		else if (type == "BlinnPhong")
		{
			Vector3D diffuse;
			ReadVector(sdMap, "shader_diffuse", diffuse);

			Vector3D specular;
			ReadVector(sdMap, "shader_specular", specular);

			double phongExp;
			ReadDouble(sdMap, "shader_phongExp", phongExp);

			double mirrorCoef;
			ReadDouble(sdMap, "shader_mirrorCoef", mirrorCoef);

			// Add shader to list.
			m_scene->m_shaders.insert(make_pair(name, new BlinnPhongShader(m_scene, diffuse, specular, phongExp, mirrorCoef)));

			// Print parameters out.
			cout << "\tDiffuse=" << diffuse << endl;
			cout << "\tSpecular=" << specular << endl;
			cout << "\tPhong Exp=" << phongExp << endl;
			cout << "\tMirror Coef=" << mirrorCoef << endl;
		}
		else if (type == "Tile")
		{
			m_scene->m_shaders.insert(make_pair(name, new ReferenceTileShader()));
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
	m_ambient = Color(0.1, 0.1, 0.1);
    m_camera = NULL;

    // Parse the XML scene file.
    XMLSceneParser xmlScene;

    // Register object creation handlers with the scene parser.
    CameraCreator cameraCreator(this);
	ObjectCreator objectCreator(this);
	LightCreator lightCreator(this);
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
			Color color;
			if (CastRayAndShade(ray, color) == false)
			{
				// If we hit nothing, assume background is black.
				color.SetRed(0.0);
				color.SetGreen(0.0);
				color.SetBlue(0.0);
			}

            // Save color to PNG structure.  Flip Y,  because we are rendering upside down.
            outputImage.set_pixel(imageX, imageHeight -1 - imageY, color.GetImageColor());
        }
    }

	// Write out PNG.
	outputImage.write(outfile);
}


inline bool Scene::CastRayAndShade(const Ray& ray, Color& result, double maxT)
{
	Intersection intersect;
	if (CastRay(ray, intersect, maxT) == true)
	{
		result = ShadeIntersection(intersect);
		return (true);
	}
	else
	{
		return (false);
	}
}


bool Scene::CastShadowRay(ILight* light, Vector3D intersectPoint)
{
	// Construct ray from the intersection point to the light.
	Ray shadowRay(intersectPoint, light->GetPosition() - intersectPoint);

	// Step ray forward by a small amount to overcome numerical inaccuracy.
	shadowRay.SetPosition(shadowRay.GetPositionAtTime(EPSILON));

	// Cast ray into scene with max t of 1.0, so that objects beyond the light are not taken into account.
	Intersection intersection;
	return (CastRay(shadowRay, intersection, 1.0));
}


bool Scene::CastRay(const Ray& ray, Intersection &result, double maxT)
{
	Intersection closestIntersect;
	closestIntersect.t = maxT;
	for (size_t i = 0; i < m_objects.size(); i++)
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

	if (closestIntersect.t != maxT)
	{
		// We had an intersection.
		result = closestIntersect;
		return (true);
	}
	else
	{
		// No intersection.
		return (false);
	}
}


inline Color Scene::ShadeIntersection(Intersection& data)
{
	// Invoke the object's shader.
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

