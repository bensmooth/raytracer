#include <iostream>
#include <cstdlib>
#include <time.h>

#include "handleGraphicsArgs.h"
#include "Scene.h"
#include "RaytraceException.h"
#include <Image.h>
#include "ThreadPool.h"


using namespace std;
using namespace sivelab;


int64_t GetTickCount()
{
	// This clock will probably not overflow and go backwards.
	timespec time;
	clock_gettime(CLOCK_MONOTONIC, &time);

	int64_t result = time.tv_sec * 1000;
	result += time.tv_nsec /  1000000;

	return (result);
}


int main(int argc, char *argv[])
{
	GraphicsArgs args;
	args.process(argc, argv);

	// This is the scene object that we primarily interact with.
	Scene *scene = NULL;

	// Make sure they passed in an input filename.
	if (args.inputFileName == "")
	{
		cerr << "Need input file!" << endl;
		exit(EXIT_FAILURE);
	}

	// Try to read in the given scene file.
	try
	{
		int64_t beginTime = GetTickCount();
		scene = new Scene(args.inputFileName, args.rpp, true, args.verbose);
		cout << "Parsing scene took " << (GetTickCount() - beginTime) << " ms." << endl;
	}
	catch (RaytraceException &e)
	{
		cerr << "Error reading in scene file " << args.inputFileName << ": " << e.what() << endl;
		exit(EXIT_FAILURE);
	}

	// Make sure they passed in the output filename.
	if (args.outputFileName == "")
	{
		cerr << "Need output file!" << endl;
		exit(EXIT_FAILURE);
	}

	// Make sure they passed in a width and a height.
	if ((args.width <= 0) || (args.height <= 0))
	{
		cerr << "You must pass in a width and a height!" << endl;
		exit(EXIT_FAILURE);
	}

	// Try to render the scene.
	try
	{
		// See if we need to autodetect the optimal thread count.
		if (args.numCpus <= 0)
		{
			args.numCpus = ThreadEngine::ThreadPool::GetNumberOfProcessors();
		}
		cout << "Rendering with " << args.numCpus << " threads..." << endl;
		int64_t beginTime = GetTickCount();
		Image image(args.width, args.height);
		scene->Render(image, args.numCpus);

		if (args.doHdr)
		{
			image.Postprocess();
		}

		image.WriteToDisk(args.outputFileName);
		cout << "Rendering scene took " << (GetTickCount() - beginTime) << " ms." << endl;
	}
	catch (RaytraceException &e)
	{
		cerr << "Error rendering scene: " << e.what() << endl;
		exit(EXIT_FAILURE);
	}

	delete scene;
	scene = NULL;

	exit(EXIT_SUCCESS);
}

