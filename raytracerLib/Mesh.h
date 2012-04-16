#pragma once

#include "IObject.h"
#include "model_obj.h"
#include "Triangle.h"


class Mesh : public IObject
{
public:
	/**
	 * Creates a mesh from the given OBJ filename, and the shader to use to render it.
	 */
	Mesh(std::string filename, IShader *shader);
	virtual ~Mesh();

	virtual BBox GetBoundingBox();
	virtual IShader* GetShader();
	virtual bool Intersect(const Ray& ray, Intersection& result);

private:
	std::vector<Triangle> m_trilist;

	IShader *m_shader;
};
