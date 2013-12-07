#!/bin/sh
# Script that renders all of the scenes.

if [ "$#" != "3" ]; then
	echo "Usage:"
	echo "doBuildAndRender.sh <image width> <image height> <rays per pixel>"
	exit 1
fi

# These variables control the dimensions and parameters of the generated images.
imageWidth=${1}
imageHeight=${2}
raysPerPixel=${3}
outputDir="../../renders/"

# Build it.
mkdir build
mkdir ${outputDir}
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cd raytracer
make -j

# Make sure we were actually able to build it before running anything.
if [ ! -f ./raytracer ]; then
	echo "Build failed!"
	exit
fi

# Render each scene.
mkdir ${outputDir}
cd ../../SceneFiles/
for filename in *.xml
do
    if test -f "${filename}"
    then
		# Image filename will be <scene name>.<raysPerPixel>
       imageFilename=${filename%%???}${raysPerPixel}

       echo "Rendering ${filename}.."
       cd ../build/raytracer/
		# Do a normal one and an HDR one.
       ./raytracer -i ../../SceneFiles/${filename} -o ${outputDir}${imageFilename}.png -w ${imageWidth} -h ${imageHeight} -r ${raysPerPixel}
       ./raytracer -i ../../SceneFiles/${filename} -o ${outputDir}${imageFilename}HDR.png -w ${imageWidth} -h ${imageHeight} -r ${raysPerPixel} -b
       cd ../../SceneFiles/
    fi
done

echo "Done rendering"
