#include "Scene.h"

#include <cfloat>
#include <png++/image.hpp>

#include "Intersection.h"


Scene::Scene(std::string filename)
{
	// Parse the XML scene file.
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
			png::rgb_pixel color;

			// If it did intersect, invoke closest object's shader.
			if (closestIntersect.t != DBL_MAX)
			{
				color = closestIntersect.object->GetShader()->Shade(closestIntersect);
			}
			else
			{
				// Use black as the background color for now.
				color.red = 0;
				color.green = 0;
				color.blue = 0;
			}

			// Save color to PNG structure.
			outputImage.set_pixel(imageX, imageY, color);
		}
	}

	// Write out PNG.
	outputImage.write(outfile);
}

