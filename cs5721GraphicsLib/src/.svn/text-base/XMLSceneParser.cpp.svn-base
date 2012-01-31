#include "XMLSceneParser.h"

using namespace sivelab;

XMLSceneParser::XMLSceneParser()
  : m_doc(0), m_rootElement(0)
{
    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION
}

XMLSceneParser::XMLSceneParser(const std::string &filename)
  : m_doc(0), m_rootElement(0), m_currFileName(filename)
{
    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
  LIBXML_TEST_VERSION;
  
  f_parse();
}

XMLSceneParser::~XMLSceneParser()
{
  f_cleanup();
}

void XMLSceneParser::parseFile(const std::string &filename)
{
  f_cleanup();
  m_currFileName = filename;
  f_parse();
}

void XMLSceneParser::f_parse()
{
  // parse the file and get the DOM 
  m_doc = xmlReadFile(m_currFileName.c_str(), NULL, 0);

  if (m_doc == NULL) {
    std::cerr << "Error: could not parse file " << m_currFileName << std::endl;
  }

  // Get the root element node
  m_rootElement = xmlDocGetRootElement(m_doc);

  assert(m_rootElement);

  xmlNode *cur_node = m_rootElement;
  if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"scene")))
    {
      parseScene(cur_node);
    }
}

void XMLSceneParser::f_cleanup()
{
  // free the document
  xmlFreeDoc(m_doc);

  //
  // Free the global variables that may
  // have been allocated by the parser.
  // 
  xmlCleanupParser();
}

bool XMLSceneParser::retrieveElementData(const std::string &tag, xmlNodePtr ptr, SceneDataContainer &d)
{
  // If the element tag matches and it wasn't previously set, so
  // replace the contents appropriately in the SceneDataContainer, d
  if (matchesNodeName(ptr->name, tag) && d.isSet == false)
    {
      xmlChar *key = xmlNodeListGetString(m_doc, ptr->xmlChildrenNode, 1);
  
      d.val = (const char *)key;
      d.isSet = true;
      
      xmlFree(key);
      return true;
    }

  return false;
}

void XMLSceneParser::parseScene(xmlNode *nPtr)
{
  xmlNode *currNodePtr = nPtr->xmlChildrenNode;

  std::map<std::string, SceneDataContainer> nodeData;

  while (currNodePtr != NULL) 
    {
      // make sure we start with a clean slate
      nodeData.clear();

      if (matchesNodeName(currNodePtr->name, "camera"))
	{
	  parseCamera(currNodePtr, nodeData);
	  // f_printNodeMapData(nodeData, "camData");

	  SceneElementCreator *creator = m_elemCallbackMap["camera"];
	  creator->m_otype = SceneElementCreator::CAMERA;
	  creator->instance( nodeData );
	}

      if (matchesNodeName(currNodePtr->name, "light"))
	{
	  parseLight(currNodePtr, nodeData);
	  // f_printNodeMapData(nodeData, "lightData");

	  SceneElementCreator *creator = m_elemCallbackMap["light"];
	  creator->m_otype = SceneElementCreator::LIGHT;
	  creator->instance( nodeData );
	}

      if (matchesNodeName(currNodePtr->name, "shader"))
	{
	  parseShader(currNodePtr, nodeData);
	  // f_printNodeMapData(nodeData, "shaderData");

	  SceneElementCreator *creator = m_elemCallbackMap["shader"];
	  creator->m_otype = SceneElementCreator::SHADER;
	  creator->instance( nodeData );
	}

      if (matchesNodeName(currNodePtr->name, "shape"))
	{
	  parseShape(currNodePtr, nodeData);
	  // f_printNodeMapData(nodeData, "shapeData");

	  SceneElementCreator *creator = m_elemCallbackMap["shape"];
	  creator->m_otype = SceneElementCreator::SHAPE;
	  creator->instance( nodeData );
	}
      
      currNodePtr = currNodePtr->next;
    }

  return;
}


void XMLSceneParser::parseCamera(xmlNode *nPtr, std::map<std::string, SceneDataContainer>& nodeData)
{
  //
  // register scene data components for camera
  //
  nodeData["camera_name"] = SceneDataContainer::emptyProp("name");
  nodeData["camera_type"] = SceneDataContainer::emptyProp("type");

  nodeData["camera_position"] = SceneDataContainer::emptyElem("position");
  nodeData["camera_viewDir"] = SceneDataContainer::emptyElem("viewDir");
  nodeData["camera_lookatPoint"] = SceneDataContainer::emptyElem("lookatPoint");
  nodeData["camera_focalLength"] = SceneDataContainer::emptyElem("focalLength");
  nodeData["camera_imagePlaneWidth"] = SceneDataContainer::emptyElem("imagePlaneWidth");

  //
  // Attempt to set the correct values in the properties and elements
  //
  retrieveProperty("name", nPtr, nodeData["camera_name"]);
  retrieveProperty("type", nPtr, nodeData["camera_type"]);

  xmlNode *currNodePtr = nPtr->xmlChildrenNode;
  while (currNodePtr != NULL) 
    {
      // Camera position
      retrieveElementData("position", currNodePtr, nodeData["camera_position"]);
      
      // Camera view direction
      retrieveElementData("viewDir", currNodePtr, nodeData["camera_viewDir"]);
      
      // Camera lookat point
      retrieveElementData("lookatPoint", currNodePtr, nodeData["camera_lookatPoint"]);

      // Camera focal length
      retrieveElementData("focalLength", currNodePtr, nodeData["camera_focalLength"]);

      // Camera image plane width
      retrieveElementData("imagePlaneWidth", currNodePtr, nodeData["camera_imagePlaneWidth"]);

      currNodePtr = currNodePtr->next;
    }

  return;
}

void XMLSceneParser::parseLight(xmlNode *nPtr, std::map<std::string, SceneDataContainer>& nodeData)
{
  //
  // register scene data components for camera
  //
  nodeData["light_type"] = SceneDataContainer::emptyProp("type");

  nodeData["light_position"] = SceneDataContainer::emptyElem("position");
  nodeData["light_intensity"] = SceneDataContainer::emptyElem("intensity");

  //
  // Attempt to set the correct values in the properties and elements
  //
  retrieveProperty("type", nPtr, nodeData["light_type"]);

  xmlNode *currNodePtr = nPtr->xmlChildrenNode;
  while (currNodePtr != NULL) 
    {
      // Light position
      retrieveElementData("position", currNodePtr, nodeData["light_position"]);

      // Light Intensity
      retrieveElementData("intensity", currNodePtr, nodeData["light_intensity"]);
      
      currNodePtr = currNodePtr->next;
    }

  return;
}

void XMLSceneParser::parseShader(xmlNode *nPtr, std::map<std::string, SceneDataContainer>& nodeData)
{
  //
  // register scene data components for camera
  //
  nodeData["shader_ref"] = SceneDataContainer::emptyProp("ref");
  nodeData["shader_name"] = SceneDataContainer::emptyProp("name");
  nodeData["shader_type"] = SceneDataContainer::emptyProp("type");

  nodeData["shader_diffuse"] = SceneDataContainer::emptyElem("diffuse");
  nodeData["shader_specular"] = SceneDataContainer::emptyElem("specular");
  nodeData["shader_phongExp"] = SceneDataContainer::emptyElem("phongExp");
  nodeData["shader_mirrorCoef"] = SceneDataContainer::emptyElem("mirrorCoef");

  //
  // Attempt to set the correct values in the properties and elements
  //
  retrieveProperty("ref", nPtr, nodeData["shader_ref"]);
  retrieveProperty("name", nPtr, nodeData["shader_name"]);
  retrieveProperty("type", nPtr, nodeData["shader_type"]);

  xmlNode *currNodePtr = nPtr->xmlChildrenNode;
  while (currNodePtr != NULL) 
    {
      retrieveElementData("diffuse", currNodePtr, nodeData["shader_diffuse"]);
      retrieveElementData("specular", currNodePtr, nodeData["shader_specular"]);
      retrieveElementData("phongExp", currNodePtr, nodeData["shader_phongExp"]);
      retrieveElementData("mirrorCoef", currNodePtr, nodeData["shader_mirrorCoef"]);
      
      currNodePtr = currNodePtr->next;
    }

  return;
}

void XMLSceneParser::parseShape(xmlNode *nPtr, std::map<std::string, SceneDataContainer>& nodeData)
{
  //
  // register scene data components for camera
  //
  nodeData["shape_name"] = SceneDataContainer::emptyProp("name");
  nodeData["shape_type"] = SceneDataContainer::emptyProp("type");
  
  nodeData["shape_center"] = SceneDataContainer::emptyElem("center");
  nodeData["shape_radius"] = SceneDataContainer::emptyElem("radius");
  nodeData["shape_minPt"] = SceneDataContainer::emptyElem("minPt");
  nodeData["shape_maxPt"] = SceneDataContainer::emptyElem("maxPt");

  //
  // Attempt to set the correct values in the properties and elements
  //
  retrieveProperty("name", nPtr, nodeData["shape_name"]);
  retrieveProperty("type", nPtr, nodeData["shape_type"]);
  
  xmlNode *currNodePtr = nPtr->xmlChildrenNode;
  while (currNodePtr != NULL) 
    {
      // shader information
      if (matchesNodeName(currNodePtr->name, "shader"))
	{
	  parseShader(currNodePtr, nodeData);
	}

      // sphere values
      retrieveElementData("center", currNodePtr, nodeData["shape_center"]);
      retrieveElementData("radius", currNodePtr, nodeData["shape_radius"]);

      // box values
      retrieveElementData("minPt", currNodePtr, nodeData["shape_minPt"]);
      retrieveElementData("maxPt", currNodePtr, nodeData["shape_maxPt"]);
      
      currNodePtr = currNodePtr->next;
    }

  return;
}

bool XMLSceneParser::retrieveProperty(const std::string &name, xmlNodePtr ptr, SceneDataContainer &d)
{
  xmlChar *propName = xmlGetProp(ptr, (const xmlChar*)name.c_str());
  if (propName && d.isSet == false)
    {
      // we found it and it wasn't previously set, so replace the
      // contents appropriately in the SceneDataContainer, d
      d.val = (char *)propName;
      d.isSet = true;

      xmlFree(propName);
      return true;
    }

  xmlFree(propName);
  return false;
}


void XMLSceneParser::f_printNodeMapData(const std::map<std::string, SceneDataContainer>& nodeMap, const std::string &name) const
{
  std::map<std::string, SceneDataContainer>::const_iterator sdIter;
  for (sdIter=nodeMap.begin(); sdIter != nodeMap.end(); sdIter++)
    {
      SceneDataContainer sd = sdIter->second;
      std::cout << name << "[" << sdIter->first << "]: " 
		<< sd.dtype << ", " << sd.name << ", " 
		<< sd.val << ", " << sd.isSet << std::endl;
    }
}

void XMLSceneParser::registerCallback(const std::string &elementName, SceneElementCreator *secPtr)
{
  m_elemCallbackMap[elementName] = secPtr;
}
