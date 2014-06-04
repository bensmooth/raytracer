#!/bin/sh
mkdir renders > /dev/null 2>&1

# Render and time each scene.
cd SceneFiles/
echo -n "Scene,"
for i in *.xml
do
  echo -n "$i,"
done
echo ""

echo -n "Time,"
for i in *.xml
do
  if test -f "$i"
  then
    cd ../build/raytracer/
    timeTaken=`(time ./raytracer -i ../../SceneFiles/$i -o ../../renders/$i.png -w 300 -h 300 -r 9) 2>&1 | grep real | cut -f2`
    echo -n "$timeTaken"
    echo -n ","
    cd ../../SceneFiles/
  fi
done
