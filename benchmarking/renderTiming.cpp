#include <iostream>
#include <cstdlib>

#include <Scene.h>
#include <EngineException.h>
#include <Image.h>
#include <ThreadPool.h>
#include <hayai.hpp>


using namespace std;
using namespace sivelab;


void RenderImage(string sceneFilename, string outputFileName, int width, int height, bool postprocess)
{
	try
	{
		Scene scene(sceneFilename, 4, true, false);
		int threads = ThreadEngine::ThreadPool::GetNumberOfProcessors();

		Image image(width, height);
		scene.Render(image, threads);

		if (postprocess)
		{
			image.Postprocess();
		}

		image.WriteToDisk(outputFileName);
	}
	catch (const EngineException &e)
	{
		cerr << "Error rendering scene: " << e.what() << endl;
	}
}


BENCHMARK(Image, Postprocess, 1, 5)
{
	Image image(100, 100);
	image.Postprocess();
}


BENCHMARK(Scene, Render, 1, 5)
{
	RenderImage("../../SceneFiles/bhart_01_2012.xml", "temp.png", 100, 100, false);
}


BENCHMARK(Scene, RenderPostprocess, 1, 5)
{
	RenderImage("../../SceneFiles/bhart_01_2012.xml", "temp.png", 100, 100, true);
}
