#include "Vector3D.h"
#include "XMLSceneParser.h"
#include "handleGraphicsArgs.h"
#include "png++/png.hpp"
#include "Random.h"

using namespace sivelab;

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

class ShapeCreator : public SceneElementCreator
{
public:
  ShapeCreator() {}
  ~ShapeCreator() {}

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


int main(int argc, char *argv[])
{
  GraphicsArgs args;
  args.process(argc, argv);

  XMLSceneParser xmlScene;

  // register object creation function with xmlScene
  xmlScene.registerCallback("camera", new CameraCreator);
  xmlScene.registerCallback("light", new GenericCreator);
  xmlScene.registerCallback("shader", new GenericCreator);
  xmlScene.registerCallback("shape", new ShapeCreator);

  if (args.inputFileName != "")
    xmlScene.parseFile( args.inputFileName );
  else
    {
      std::cerr << "Need input file!" << std::endl;
      exit(EXIT_FAILURE);
    }

  exit(EXIT_SUCCESS);
}

