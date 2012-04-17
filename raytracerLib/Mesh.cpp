#include "Mesh.h"
#include "IShader.h"
#include "BVHNode.h"
#include "RaytraceException.h"


Mesh::Mesh(std::string filename, IShader* shader)
{
	m_shader = shader;

	ModelOBJ mOBJ;
	if (mOBJ.import(filename.c_str()) == false)
	{
		// Something went wrong.
		throw RaytraceException("Mesh at \"" + filename + "\" was unable to be read!");
	}

	const ModelOBJ::Mesh *pMesh = 0;
	const ModelOBJ::Vertex *pVertices = 0;

	// The index buffer is the list of indices used to reference the vertices in
	// the list of vertices (also known as a vertex buffer).
	const int *idxBuffer = mOBJ.getIndexBuffer();

	// The list of tris in the mesh.
	std::vector<IObject*> triList;

	// Walk over all of the meshes associated with this OBJ file
	for (int mIdx=0; mIdx<mOBJ.getNumberOfMeshes(); mIdx++)
	{
		// For each mesh, get a reference to the mesh, a pointer to the material
		// associated with the mesh, and a pointer to the vertex buffer segment associated
		// with the mesh.
		pMesh = &mOBJ.getMesh(mIdx);
		pVertices = mOBJ.getVertexBuffer();

		// TODO: Something with this.
		//const ModelOBJ::Material *pMaterial = pMesh->pMaterial;

		// TODO: With the material pointer, you'll likely want to infer what type of shader to
		// create.  Take a look at the pMaterial class declaration/definition in the
		// model_obj.h file for more detailed information.
		// pMaterial->diffuse[0], pMaterial->diffuse[1], pMaterial->diffuse[2]

		// With the mesh, you have a starting index to use in the index array.  You
		// also have the number of triangles in the mesh.  Using the two you can iterate
		// over the indices and access the vertices in the vertex buffer appropriately.
		for (int i=pMesh->startIndex; i<(pMesh->startIndex + pMesh->triangleCount*3); i+=3)
		{
			// With the starting index, we can access three indices in the index buffer
			// to get the indices into the vertex buffer, and thus, the vertices of a
			// triangle.
			// Extract vertices.
			ModelOBJ::Vertex v0, v1, v2;
			v0 = pVertices[ idxBuffer[i] ];
			v1 = pVertices[ idxBuffer[i+1] ];
			v2 = pVertices[ idxBuffer[i+2] ];
			sivelab::Vector3D vertices[3];
			vertices[0].set(v0.position[0], v0.position[1], v0.position[2]);
			vertices[1].set(v1.position[0], v1.position[1], v1.position[2]);
			vertices[2].set(v2.position[0], v2.position[1], v2.position[2]);

			// Extract normals.
			sivelab::Vector3D normals[3];
			normals[0].set(v0.normal[0], v0.normal[1], v0.normal[2]);
			normals[1].set(v1.normal[0], v1.normal[1], v1.normal[2]);
			normals[2].set(v2.normal[0], v2.normal[1], v2.normal[2]);

			// Add tri to list.
			triList.push_back(new Triangle(vertices, normals, shader));
		}
	}

	// Construct BVH.
	m_bvh = BVHNode::ConstructBVH(triList);
}


Mesh::~Mesh()
{
	delete m_bvh;
	m_bvh = NULL;
}


BBox Mesh::GetBoundingBox()
{
	return m_bvh->GetBoundingBox();
}


IShader* Mesh::GetShader()
{
	return (m_shader);
}


bool Mesh::Intersect(const Ray& ray, Intersection& result)
{
	return (m_bvh->Intersect(ray, result));
}
