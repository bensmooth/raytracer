add_library (cs5721Graphics
  ArgumentParsing.cpp ArgumentParsing.h
  handleGraphicsArgs.cpp handleGraphicsArgs.h
  model_obj.cpp model_obj.h
  Random.cpp Random.h
  SceneDataContainer.h
  Timer.cpp Timer.h
  Vector3D.cpp Vector3D.h
  XMLSceneParser.h XMLSceneParser.cpp
)

target_link_libraries(cs5721Graphics ${PNG_LIBRARY})
