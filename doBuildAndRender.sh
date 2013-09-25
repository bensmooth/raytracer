#!/bin/sh
# Script that renders all of the scenes.

# These variables control the dimensions of the generated images.
imageWidth=1920
imageHeight=1080
raysPerPixel=9

# Build it.
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cd raytracer
make -j

# Render each scene.
mkdir renders
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
