#include <iostream>
#include <cstdlib>
#include <time.h>

#include "handleGraphicsArgs.h"
#include "Scene.h"
#include "RaytraceException.h"


using namespace std;
using namespace sivelab;


long long GetTickCount()
{
	// This clock should never, ever go backwards.
	timespec time;
	clock_gettime(CLOCK_MONOTONIC_RAW, &time);

	long long result = time.tv_sec * 1000;
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
		long long beginTime = GetTickCount();
		scene = new Scene(args.inputFileName);
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
		long long beginTime = GetTickCount();
		scene->Render(args.outputFileName, args.width, args.height);
		cout << "Rendering scene took " << (GetTickCount() - beginTime) << " ms." << endl;
	}
	catch (RaytraceException &e)
	{
		cerr << "Error rendering scene: " << e.what() << endl;
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}

