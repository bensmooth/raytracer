#!/bin/sh
# Script that renders all of the scenes.

if [ "$#" != "3" ]; then
	echo "Usage:"
	echo "doBuildAndRender.sh <image width> <image height> <rays per pixel>"
	exit 1
fi

# These variables control the dimensions of the generated images.
imageWidth=${1}
imageHeight=${2}
raysPerPixel=${3}

echo "${imageWidth}x${imageHeight} and ${raysPerPixel} rays per pixel"

# Build it.
mkdir build
mkdir renders
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cd raytracer
make -j

# Render each scene.
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
       ./raytracer -i ../../SceneFiles/${filename} -o ../../renders/${imageFilename}.png -w ${imageWidth} -h ${imageHeight} -r ${raysPerPixel}
       ./raytracer -i ../../SceneFiles/${filename} -o ../../renders/${imageFilename}HDR.png -w ${imageWidth} -h ${imageHeight} -r ${raysPerPixel} -b
       cd ../../SceneFiles/
    fi
done

echo "Done rendering!"
echo "All renders are in renders/"
