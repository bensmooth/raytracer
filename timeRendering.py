#!/usr/bin/python
import time
import os
from glob import glob
from subprocess import call

scenePath = './SceneFiles/'
raytracerPath = './build/raytracer/raytracer'
raytracerArguments = '{0} -i ./SceneFiles/{1} -o ./renders/{1}.png -w 100 -h 100 -r 9'

sceneFiles = [ x.replace(scenePath, '') for x in glob(scenePath + "*.xml") ]
sceneFiles.sort()

print('Command line used: ' + raytracerArguments)

# Print out heading of scene names.
print('Scene', end=',')
for filename in sceneFiles:
	print (filename, end=',')

print()

# Run the raytracer with each configuration.
with open(os.devnull, "w") as fnull:
	for scene in sceneFiles:
		start_time = time.time()
		call(raytracerArguments.format(raytracerPath, scene).split(), stdout = fnull, stderr = fnull)
		print(time.time() - start_time, end=',')

print()
