#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <vector>

#include "Vector3D.h"

class BoundingBox
{
public:
  BoundingBox()
  {
    bboxMin[0] = std::numeric_limits<float>::max();
    bboxMin[1] = std::numeric_limits<float>::max();
    bboxMin[2] = std::numeric_limits<float>::max();
    
    bboxMax[0] = -bboxMin[0];
    bboxMax[1] = -bboxMin[1];
    bboxMax[2] = -bboxMin[2];
  }

  void update(float x, float y, float z)
  {
    if (x > bboxMax[0]) bboxMax[0] = x;
    if (y > bboxMax[1]) bboxMax[1] = y;
    if (z > bboxMax[2]) bboxMax[2] = z;

    if (x < bboxMin[0]) bboxMin[0] = x;
    if (y < bboxMin[1]) bboxMin[1] = y;
    if (z < bboxMin[2]) bboxMin[2] = z;
  }

  float bboxMin[3];
  float bboxMax[3];  
};

using namespace sivelab;

int main(int argc, char *argv[])
{
  if (argc != 2) exit(EXIT_FAILURE);
  std::string filename = argv[1];

  std::vector<int> indexList;
  std::vector<Vector3D> vertexList;
  
  BoundingBox bbox;
  
  std::cerr << "Parsing render file: " << filename << "..." << std::endl;
  
  std::ifstream obj_file( filename.c_str() );
  if (obj_file.is_open() == false)
    {
      std::cerr << "Error opening file \"" << filename << "\".  Exiting." << std::endl;
      exit(1);
    }

  vertexList.push_back(sivelab::Vector3D(0, 0, 0));

  int linemax = 1024;
  char *linebuf = new char[ linemax ];
  while (obj_file.good() && !obj_file.eof())
    {
      // read a line off the file and append to string list
      obj_file.getline( linebuf, linemax );

      std::istringstream is(linebuf);

      char keychar;
      is >> keychar;

      if (keychar == 'v')
	{
	  double x, y, z;
	  is >> x >> y >> z;
	  bbox.update(x, y, z);

	  vertexList.push_back(sivelab::Vector3D(x, y, z));
	}
      else if (keychar == 'f')
	{
	  int ix, iy, iz;
	  is >> ix >> iy >> iz;
	  indexList.push_back(ix);
	  indexList.push_back(iy);
	  indexList.push_back(iz);
	}
    }
  obj_file.close();

  std::cerr << "Read " << filename << "." << std::endl;
  std::cerr << "\tTriangles: " << (indexList.size() / 3) - 1 << std::endl;
  std::cerr << "\tBounding Box: [" << bbox.bboxMin[0] << ", " << bbox.bboxMin[1] << ", " << bbox.bboxMin[2] << "] x ["
	    << bbox.bboxMax[0] << ", " << bbox.bboxMax[1] << ", " << bbox.bboxMax[2] << "]" << std::endl;
  std::cerr << "\tDimensions: " 
	    << bbox.bboxMax[0] - bbox.bboxMin[0] << " X " 
	    << bbox.bboxMax[1] - bbox.bboxMin[1] << " X " 
	    << bbox.bboxMax[2] - bbox.bboxMin[2] << std::endl;

  std::cerr << "\tObject Center: (" 
	    << (bbox.bboxMax[0] + bbox.bboxMin[0])/2.0 << ", " 
	    << (bbox.bboxMax[1] + bbox.bboxMin[1])/2.0 << ", " 
	    << (bbox.bboxMax[2] + bbox.bboxMin[2])/2.0 << ")" << std::endl;

  float scale;
  scale = 5.0 / (bbox.bboxMax[0] - bbox.bboxMin[0]);

  float trans[3];
  trans[0] = 0.0;
  trans[1] = -1.0 * (bbox.bboxMin[1] * scale);
  trans[2] = -5.0;

#if 0
  for (int i=0; i<vertexList.size(); i++)
    std::cerr << "Vertex: " << vertexList[i] << std::endl;
#endif

  std::cout << "shape triangle -50.0 0.0 50.0 50.0 0.0 50.0 50.0 0.0 -50.0 blinnphong 0.5 0.5 0.5 1.0 1.0 1.0 m 0.7" << std::endl;
  std::cout << "shape triangle -50.0 0.0 50.0 50.0 0.0 -50.0 -50.0 0.0 -50.0 blinnphong 0.5 0.5 0.5 1.0 1.0 1.0 m 0.7" << std::endl;

  for (int i=0; i<indexList.size(); i+=3)
    {
      int idx0 = indexList[i+0];
      int idx1 = indexList[i+1];
      int idx2 = indexList[i+2];

      std::cout << "shape triangle " 
		<< vertexList[ idx0 ][0] * scale + trans[0] << ' ' << vertexList[ idx0 ][1] * scale + trans[1] << ' ' << vertexList[ idx0 ][2] * scale + trans[2]  << ' '
		<< vertexList[ idx1 ][0] * scale + trans[0] << ' ' << vertexList[ idx1 ][1] * scale + trans[1] << ' ' << vertexList[ idx1 ][2] * scale + trans[2] << ' '
		<< vertexList[ idx2 ][0] * scale + trans[0] << ' ' << vertexList[ idx2 ][1] * scale + trans[1] << ' ' << vertexList[ idx2 ][2] * scale + trans[2] << ' '
		<< " blinnphong 0.2 0.2 0.7 1.0 1.0 1.0 32.0 m 0.7" << std::endl;
      // << " lambertian 0.2 0.8 0.2" << std::endl;
    }

  Vector3D camOrigin( 0.0,
		      5.0,
		      2.0 );
  Vector3D camGazePt( ( ((bbox.bboxMax[0]*scale)+trans[0]) + ((bbox.bboxMin[0]*scale)+trans[0]) ) / 2.0, 
		      ( ((bbox.bboxMax[1]*scale)+trans[1]) + ((bbox.bboxMin[1]*scale)+trans[1]) ) / 2.0,
		      ( ((bbox.bboxMax[2]*scale)+trans[2]) + ((bbox.bboxMin[2]*scale)+trans[2]) ) / 2.0 );
  Vector3D gaze = camGazePt - camOrigin;
  double len = gaze.normalize();

  // with image plane width set, compute the focal length to fit the image
  double objHalfSpan = (((bbox.bboxMax[0]*scale)+trans[0]) - ((bbox.bboxMin[0]*scale)+trans[0])) / 2.0;
  double halfFov = atan2(objHalfSpan, len);

  // increase the fov by 15% to 
  halfFov *= 1.15;

  double ipWidth = 0.5;
  double focalLength = (ipWidth/2.0) / tan(halfFov);

  std::cout << "camera " 
	    << camOrigin[0] << ' ' << camOrigin[1] << ' ' << camOrigin[2] << ' ' 
	    << gaze[0] << ' ' << gaze[1] << ' ' << gaze[2] << ' ' 
	    << focalLength << " " << ipWidth << std::endl;
  std::cout << "light -5.0 5.0 5.0 1.0 1.0 1.0" << std::endl;
  std::cout << "light 8.0 5.0 4.0 1.0 1.0 1.0" << std::endl;
}
	     
