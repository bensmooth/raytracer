#include "Scene.h"

#include <cfloat>


Scene::Scene(std::string filename)
{
	// Parse the XML scene file.
}


void Scene::Render(std::string outfile, int imageWidth, int imageHeight)
{
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

			// TODO: If it did intersect, invoke closest object's shader.

			// TODO: Save color to PNG structure.
		}
	}

	// TODO: Write out PNG.
}

