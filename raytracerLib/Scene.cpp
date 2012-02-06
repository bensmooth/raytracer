#include "Scene.h"

#include <cfloat>
#include <png++/image.hpp>

#include "png++/png.hpp"
#include "Vector3D.h"
#include "XMLSceneParser.h"
#include "Intersection.h"
#include "Color.h"


using namespace sivelab;

/**
 * This creator is used to create the camera.
 */
class CameraCreator : public SceneElementCreator
{
public:
  CameraCreator() {}
  ~CameraCreator() {}

  void instance(std::map<std::string, SceneDataContainer> &sdMap)
  {
    std::istringstream buf;
    std::map<std::string, SceneDataContainer>::iterator sdIter;

    std::string name, type;
    Vector3D position, viewDir, lookatPoint;
    bool lookatSet = false;
    float focalLength;
    float imagePlaneWidth;

    sdIter = sdMap.find("camera_name");
    assert( sdIter != sdMap.end() );
    name = sdIter->second.val;

    sdIter = sdMap.find("camera_type");
    assert( sdIter != sdMap.end() );
    type = sdIter->second.val;

    sdIter = sdMap.find("camera_position");
    assert( sdIter != sdMap.end() );
    buf.str(sdIter->second.val);
    buf >> position;
    buf.clear();

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

    std::cout << "Camera: name=" << name << ", type=" << type << std::endl;
    std::cout << "\tposition: " << position << std::endl;
    if (lookatSet)
      std::cout << "\tlook at point: " << lookatPoint << std::endl;
    else 
      std::cout << "\tview direction: " << viewDir << std::endl;
    std::cout << "\tfocal length: " << focalLength << std::endl;
    std::cout << "\timage plane width: " << imagePlaneWidth << std::endl;
  }
};


/**
 * This creator is used for elements that we don't know how to create yet.
 */
class GenericCreator : public SceneElementCreator
{
public:
  GenericCreator() {}
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
};


/**
 * This class is used to create scene objects.
 */
class ObjectCreator : public SceneElementCreator
{
public:
  ObjectCreator() {}
  ~ObjectCreator() {}

  void instance(std::map<std::string, SceneDataContainer> &sdMap)
  {
    // Figure out how to instance the other things...
    std::map<std::string, SceneDataContainer>::const_iterator sdIter;
    for (sdIter=sdMap.begin(); sdIter != sdMap.end(); sdIter++)
      {
	SceneDataContainer sd = sdIter->second;
	std::cout << "Shape [" << sdIter->first << "]: " 
		  << sd.dtype << ", " << sd.name << ", " 
		  << sd.val << ", " << sd.isSet << std::endl;
      }
  }
};


Scene::Scene(std::string filename)
{
	m_camera = NULL;

	// Parse the XML scene file.
	XMLSceneParser xmlScene;

	// Register object creation handlers with the scene parser.
	// TODO: Are these all memory leaks?
	xmlScene.registerCallback("camera", new CameraCreator());
	xmlScene.registerCallback("light", new GenericCreator());
	xmlScene.registerCallback("shader", new GenericCreator());
	xmlScene.registerCallback("shape", new ObjectCreator());

	xmlScene.parseFile(filename);

	// Make sure they set a camera.
	if (m_camera == NULL)
	{
		throw RaytraceException("Camera not set!");
	}
}


void Scene::Render(std::string outfile, int imageWidth, int imageHeight)
{
	// This is the image we will be writing to.
	png::image<png::rgb_pixel> outputImage(imageWidth, imageHeight);

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
					if (currentIntersect.t < closestIntersect.t)
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
				// Use green as the background color for now.
				color.SetRed(0.0);
				color.SetGreen(1.0);
				color.SetBlue(0.0);
			}

			// Save color to PNG structure.
			outputImage.set_pixel(imageX, imageY, color.GetImageColor());
		}
	}

	// Write out PNG.
	outputImage.write(outfile);
}

