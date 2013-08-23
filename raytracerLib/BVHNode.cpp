#include <algorithm>
#include "BVHNode.h"
#include "Intersection.h"
#include "EngineException.h"

using namespace std;


BVHNode::BVHNode()
{
	m_rightChild = NULL;
	m_leftChild = NULL;
}


// These are used to compare bounding boxes.
bool compareBBoxX(pair<BBox, IObject*> a, pair<BBox, IObject*> b)
{
	return (a.first.GetCenter()[0] < b.first.GetCenter()[0]);
}

bool compareBBoxY(pair<BBox, IObject*> a, pair<BBox, IObject*> b)
{
	return (a.first.GetCenter()[1] < b.first.GetCenter()[1]);
}

bool compareBBoxZ(pair<BBox, IObject*> a, pair<BBox, IObject*> b)
{
	return (a.first.GetCenter()[2] < b.first.GetCenter()[2]);
}


void BVHNode::SortInDimension(int dimension, BBObjectList& objects)
{
	dimension %= 3;

	// Sort by the given dimension.
	if (dimension == 0)
	{
		stable_sort(objects.begin(), objects.end(), compareBBoxX);
	}
	else if (dimension == 1)
	{
		stable_sort(objects.begin(), objects.end(), compareBBoxY);
	}
	else
	{
		// Must be z.
		stable_sort(objects.begin(), objects.end(), compareBBoxZ);
	}
}


BVHNode::BVHNode(BBObjectList objects, int dimensionToSortOn)
{
	size_t objectCount = objects.size();

	// Termination cases.
	if (objectCount == 2)
	{
		m_rightChild = objects[0].second;
		m_leftChild = objects[1].second;
		m_bbox = BBox::Combine(objects[0].first, objects[1].first);
		return;
	}
	if (objectCount == 1)
	{
		m_rightChild = NULL;
		m_leftChild = objects[0].second;
		m_bbox = objects[0].first;
		return;
	}
	if (objectCount == 0)
	{
		throw EngineException("List with 0 objects passed into BVH builder.");
	}

	// Sort the list along the given dimension.
	SortInDimension(dimensionToSortOn, objects);

	// Split list in half.
	BBObjectList leftObjs(objects.begin(), objects.begin() + objectCount / 2);
	BBObjectList rightObjs(objects.begin() + objectCount / 2, objects.end());

	// Assign each half to each child.
	m_leftChild = new BVHNode(leftObjs, dimensionToSortOn + 1);
	m_rightChild = new BVHNode(rightObjs, dimensionToSortOn + 1);

	// Calculate bounding box.
	m_bbox = BBox::Combine(m_leftChild->GetBoundingBox(), m_rightChild->GetBoundingBox());
}


BVHNode *BVHNode::ConstructBVH(vector< IObject* > objects)
{
	// First, compute the bounding boxes for all objects.
	BBObjectList bbObjList;
	bbObjList.resize(objects.size());
	for (size_t i = 0; i < objects.size(); i++)
	{
		bbObjList[i] = make_pair(objects[i]->GetBoundingBox(), objects[i]);
	}

	// Make root node.  This will build the entire tree.
	return (new BVHNode(bbObjList, 0));
}


BVHNode::~BVHNode()
{
	// Delete each child.
	delete m_rightChild;
	delete m_leftChild;

	m_rightChild = NULL;
	m_leftChild = NULL;
}


BBox BVHNode::GetBoundingBox()
{
	return (m_bbox);
}


IShader* BVHNode::GetShader()
{
	// This should never be called.
	throw EngineException("BVHNode::GetShader() should never be called!");
}


bool BVHNode::Intersect(const Ray& ray, Intersection& result)
{
	// See if they hit our bounding box.
	if (m_bbox.Intersects(ray) == false)
	{
		return (false);
	}

	// If we got here, the ray hit our bbox, we need to see if it hit any of our contents.
	// See if the right child is valid.
	bool rightHit;
	Intersection rightResult;
	if (m_rightChild == NULL)
	{
		// No hit.
		rightHit = false;
	}
	else
	{
		// Intersect ray with right child.
		rightHit = m_rightChild->Intersect(ray, rightResult);

		// Discard results that have a negative t value.
		if (rightHit && (rightResult.t < 0.0))
		{
			rightHit = false;
		}
	}

	// See if left child is valid.
	bool leftHit;
	Intersection leftResult;
	if (m_leftChild == NULL)
	{
		// No hit.
		leftHit = false;
	}
	else
	{
		// Intersect ray with left child.
		leftHit = m_leftChild->Intersect(ray, leftResult);

		// Discard results that have a negative t value.
		if (leftHit && (leftResult.t < 0.0))
		{
			leftHit = false;
		}
	}

	// If both rays missed, we have no hit.
	if (!rightHit && !leftHit)
	{
		return (false);
	}
	else if (rightHit && leftHit)
	{
		// Both rays hit.  See which one is closer.
		if (rightResult.t < leftResult.t)
		{
			result = rightResult;
		}
		else
		{
			result = leftResult;
		}
	}
	else if (rightHit)
	{
		// Right ray hit.
		result = rightResult;
	}
	else if (leftHit)
	{
		// Left ray hit.
		result = leftResult;
	}
	else
	{
		// Something really funky is going on.
		throw EngineException("Invalid BVH intersection!");
	}

	// If we got here, we had a hit.
	return (true);
}

