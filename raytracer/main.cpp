#include <iostream>
#include <cstdlib>

#include "handleGraphicsArgs.h"
#include "Scene.h"
#include "RaytraceException.h"


using namespace std;
using namespace sivelab;


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
		scene = new Scene(args.inputFileName);
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
		scene->Render(args.outputFileName, args.width, args.height);
	}
	catch (RaytraceException &e)
	{
		cerr << "Error rendering scene: " << e.what() << endl;
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}

