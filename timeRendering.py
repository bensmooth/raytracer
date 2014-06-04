#!/usr/bin/python
import time
import os
import sys
from glob import glob
from subprocess import call

scenePath = './SceneFiles/'
raytracerPath = './build/raytracer/raytracer'
raytracerArguments = '{0} -i ./SceneFiles/{1} -o ./renders/{1}.png -w 100 -h 100 -r 4'

# Get number of times to benchmark.
if len(sys.argv) == 2:
	sampleCount = int(sys.argv[1])
else:
	sampleCount = 1

# Create list of scene files to render.
sceneFiles = [ x.replace(scenePath, '') for x in glob(scenePath + "*.xml") ]
sceneFiles.sort()

# Print out scene names.
for filename in sceneFiles:
	print (filename, end=',')

print()

# Run the raytracer with each configuration.
with open(os.devnull, "w") as fnull:
	for i in range(sampleCount):
		for scene in sceneFiles:
			start_time = time.time()
			call(raytracerArguments.format(raytracerPath, scene).split(), stdout = fnull, stderr = fnull)
			print(time.time() - start_time, end=',')
		print()
