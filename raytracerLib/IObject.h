#pragma once

// Forward declarations.
class IShader;
struct Ray;
struct Intersection;
struct BBox;


class IObject
{
public:
	virtual ~IObject() { }


	/**
	 * Sees if the given ray intersects with the object.
	 * @param ray The ray to test for intersection.
	 * @param result If true is returned, this will contain the intersection data.
	 * @return True if there was an intersection, false if there was not.
	 */
	virtual bool Intersect(const Ray &ray, Intersection &result) = 0;


	/**
	 * Gets the shader associated with this object.
	 */
	virtual IShader *GetShader() = 0;


	/**
	 * Gets a bounding box that contains the entirity of the object.
	 */
	virtual BBox GetBoundingBox() = 0;
};

