#!/bin/sh
# Make some folders.
mkdir build
mkdir renders
cd build

# Build it.
cmake .. -DCMAKE_BUILD_TYPE=Release
cd raytracer
make -j

# Render each scene.
cd ../../SceneFiles/
for i in *.xml
do
    if test -f "$i"
    then
       echo "Rendering $i.."
       ../build/raytracer/raytracer -i $i -o ../renders/$i.png -w 600 -h 600
    fi
done

echo "Done rendering!"
echo "All renders are in renders/"
