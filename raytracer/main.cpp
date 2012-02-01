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

private:
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

private:
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


////////////////// HERE STARTS PNG TEST!!!!!!!!!!!!!!
int main(int argc, char *argv[])
{
	GraphicsArgs args;
	args.process(argc, argv);
	
	//
	// Create a red image
	//
	int w = args.width, h = args.height;
	png::image< png::rgb_pixel > imData( w, h );
	for (size_t y = 0; y < imData.get_height(); ++y)
	{
		for (size_t x = 0; x < imData.get_width(); ++x)
		{
			// non-checking equivalent of image.set_pixel(x, y, ...);
			imData[y][x] = png::rgb_pixel(255, 0, 0);
		}
	}
	imData.write( "red_DualLoop.png" );
	
	//
	// Alternatively, you can do the same using a single loop:
	//
	for (unsigned int idx=0; idx<imData.get_height()*imData.get_width(); ++idx)
	{
		size_t x = idx % w;
		size_t y = static_cast<size_t>( floor(idx / static_cast<float>(imData.get_width())) );
		
		// non-checking equivalent of image.set_pixel(x, y, ...);
		imData[y][x] = png::rgb_pixel(0, 255, 0);
	}
	imData.write( "green_SingleLoop.png" );



	//
	// create an image with random colors in every pixel
	//
	Random prng;
	for (unsigned int idx=0; idx<imData.get_height()*imData.get_width(); ++idx)
	{
		size_t x = idx % w;
		size_t y = static_cast<size_t>( floor(idx / static_cast<float>(imData.get_width())) );
		
		// assert((y >= 0) && (y < h) && x >= 0 && x < w);
		
		Vector3D c(static_cast<int>(floor(prng.uniform() * 255)), 
				   static_cast<int>(floor(prng.uniform() * 255)), 
				   static_cast<int>(floor(prng.uniform() * 255)));
		
		// The origin for indexing the height is in lower left...
		imData[y][x] = png::rgb_pixel( c[0],
									   c[1],
								 c[2] );
	}
	
	imData.write( "random.png" );
	
	for (unsigned int idx=0; idx<imData.get_height()*imData.get_width(); ++idx)
	{
		size_t x = idx % w;
		size_t y = static_cast<size_t>( floor(idx / static_cast<float>(imData.get_width())) );
		
		// assert((y >= 0) && (y < h) && x >= 0 && x < w);
		
		// radial distance to edge of image
		float max_distance = sqrt( static_cast<float>((w/2.0*w/2.0) + (h/2.0*h/2.0)) );
		float dist = sqrt( static_cast<float>((x - w/2.0)*(x - w/2.0) + (y - h/2.0)*(y - h/2.0)) ) / max_distance;
		
		Vector3D c(static_cast<int>(dist * 255), 
				   static_cast<int>(dist * 255), 
				   static_cast<int>(dist * 255));
		
		// The origin for indexing the height is in lower left...
		imData[y][x] = png::rgb_pixel( c[0],
									   c[1],
								 c[2] );
	}
	
	imData.write( "radial_Center.png" );
	
	// Creates a radial gradient image from the origin of the image.
	// Note that the origin is in the upper left.
	for (unsigned int idx=0; idx<imData.get_height()*imData.get_width(); ++idx)
	{
		size_t x = idx % w;
		size_t y = static_cast<size_t>( floor(idx / static_cast<float>(imData.get_width())) );
		
		// assert((y >= 0) && (y < h) && x >= 0 && x < w);
		
		// radial distance to edge of image
		float max_distance = sqrt( static_cast<float>((w*w) + (h*h)) );
		float dist = sqrt( static_cast<float>((x*x) + (y*y)) ) / max_distance;
		
		Vector3D c(static_cast<int>(dist * 255), 
				   static_cast<int>(dist * 255), 
				   static_cast<int>(dist * 255));
		
		// The origin for indexing the height is in lower left...
		imData[y][x] = png::rgb_pixel( c[0],
									   c[1],
								 c[2] );
	}
	
	imData.write( "radial_ImageOrigin.png" );
	
	
	//
	// Checkerboard
	//
	for (unsigned int idx=0; idx<imData.get_height()*imData.get_width(); ++idx)
	{
		size_t x = idx % w;
		size_t y = static_cast<size_t>( floor(idx / static_cast<float>(imData.get_width())) );
		
		// bitwise AND of each value XOR'd with each other
		int v = (((x & 0x8)==0) ^ ((y & 0x8)==0)) * 255;
		Vector3D c( v, v, v );
		
		// The origin for indexing the height is in lower left...
		imData[y][x] = png::rgb_pixel( c[0],
									   c[1],
								 c[2] );
	}
	
	imData.write( "checkerboard.png" );
	
	exit(EXIT_SUCCESS);
}

