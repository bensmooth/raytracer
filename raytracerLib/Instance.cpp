#include "Instance.h"
#include "RaytraceException.h"
#include "IShader.h"
#include "Ray.h"
#include "Intersection.h"
#include "Matrix.h"


InstanceObject::InstanceObject(Matrix transf, IObject* original, IShader* shader)
{
	// Calculate inverse of transformation matrix.
	if (transf.Inverse(m_invTrans) == false)
	{
		throw RaytraceException("Unable to invert matrix:\n" + transf.ToString() + "\n");
	}

	// Transform the points that make up the original object's bounding box.
	m_bbox = original->GetBoundingBox();
	m_bbox = m_bbox.Transform(transf);

	// Copy in references to original object and shader.
	m_original = original;
	m_shader = shader;
}


InstanceObject::~InstanceObject()
{
	// Free nothing.  m_shader and m_original are part of the Scene class.
}


BBox InstanceObject::GetBoundingBox()
{
	return (m_bbox);
}


IShader* InstanceObject::GetShader()
{
	// If m_shader is NULL, return the original's shader.
	if (m_shader == NULL)
	{
		return (m_original->GetShader());
	}
	else
	{
		return (m_shader);
	}
}


bool InstanceObject::Intersect(const Ray& ray, Intersection& result)
{
	// Transform the ray.
	Ray transRay = m_invTrans * ray;

	// See if the transformed ray intersects the original object.
	if (m_original->Intersect(transRay, result))
	{
		// We hit.
		result.collidedRay = ray;
		result.object = this;

		// Transform the normal with the transpose of the inverse matrix, and normalize.
		Vector4D transNorm(result.surfaceNormal, false);
		transNorm = m_invTrans.Transpose() * transNorm;
		result.surfaceNormal = transNorm.vector3d;
		result.surfaceNormal.normalize();

		return (true);
	}
	else
	{
		// No hit.
		return (false);
	}
}
