#pragma once

#include "IObject.h"
#include "BBox.h"


class BVHNode : public IObject
{
public:
	/**
	 * Constructs a BVH node that contains default values.
	 */
	BVHNode();

	/**
	 * Frees the node, and all of its children.
	 */
	virtual ~BVHNode();

	virtual bool Intersect(const Ray& ray, Intersection& result);

	virtual BBox GetBoundingBox();

	/**
	 * This should never be called.
	 * It throws an exception.
	 */
	virtual IShader* GetShader();

	/**
	 * Constructs a BVH containing the given objects.
	 * @param objects The list of objects to create the BVH with.
	 * @remarks The objects are assumed to have been allocated with new, and will be freed with delete in the destructor.
	 * @return The root node of the constructed tree.  Must be freed with delete when done.
	 */
	static BVHNode *ConstructBVH(std::vector<IObject*> objects);

private:
	// A list of pointers to objects and their bounding boxes.
	typedef std::vector< std::pair< BBox, IObject* > > BBObjectList;

	/**
	 * Recursively constructs a BVH containing the given list of objects.
	 * @param objects The list of objects to add to the BVH.  The bounding boxes are assumed to be already calculated.
	 * @param dimensionToSortOn The dimension to sort the list on in this iteration.
	 * @remarks The objects are assumed to have been allocated with new, and will be freed with delete in the destructor.
	 */
	BVHNode(BBObjectList objects, int dimensionToSortOn);

	/**
	 * Sorts the list of objects and bounding boxes by a dimension.
	 * @param dimension The dimension; 0 for x, 1 for y, 2 for z.  Values are modded by 3.
	 * @param objects The list of objects and bounding boxes to be sorted.
	 */
	static void SortInDimension(int dimension, BBObjectList &objects);

	// Children.
	IObject *m_leftChild;
	IObject *m_rightChild;

	/**
	 * The bounding box of the node.
	 */
	BBox m_bbox;
};
