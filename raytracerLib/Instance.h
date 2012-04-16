#pragma once

#include "IObject.h"
#include "Matrix.h"
#include "BBox.h"

class InstanceObject : public IObject
{
public:
	/**
	 * Creates an instance of an object, with the given transformation matrix to use.
	 * @param transf The transformation matrix.
	 * @param original The object that this instances.
	 * @param shader If non-NULL, will override the original's shader.
	 */
	InstanceObject(Matrix transf, IObject *original, IShader *shader);
	virtual ~InstanceObject();

	virtual bool Intersect(const Ray& ray, Intersection& result);
	virtual IShader* GetShader();
	virtual BBox GetBoundingBox();

private:
	/**
	 * Contains the inverse of the transformation matrix.
	 */
	Matrix m_invTrans;

	/**
	 * Pointer to the original object
	 */
	IObject *m_original;

	/**
	 * The shader to use.  If NULL, use the original's shader.
	 */
	IShader *m_shader;

	/**
	 * The bouding box of the transformed object.
	 */
	BBox m_bbox;
};
