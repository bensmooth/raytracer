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
       ../build/raytracer/raytracer -i $i -o ../renders/$i.png -w 1280 -h 800 -r 9
    fi
done

echo "Done rendering!"
echo "All renders are in renders/"
