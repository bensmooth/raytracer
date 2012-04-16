#include "Mesh.h"
#include "IShader.h"


Mesh::Mesh(std::string filename, IShader* shader)
{
	m_shader = shader;

	ModelOBJ mOBJ;
	mOBJ.import(filename.c_str());

	const ModelOBJ::Mesh *pMesh = 0;
	const ModelOBJ::Material *pMaterial = 0;
	const ModelOBJ::Vertex *pVertices = 0;

	// The index buffer is the list of indices used to reference the vertices in
	// the list of vertices (also known as a vertex buffer).
	const int *idxBuffer = mOBJ.getIndexBuffer();

	// Walk over all of the meshes associated with this OBJ file
	for (int mIdx=0; mIdx<mOBJ.getNumberOfMeshes(); mIdx++)
	{
		// For each mesh, get a reference to the mesh, a pointer to the material
		// associated with the mesh, and a pointer to the vertex buffer segment associated
		// with the mesh.
		pMesh = &mOBJ.getMesh(mIdx);
		pMaterial = pMesh->pMaterial;
		pVertices = mOBJ.getVertexBuffer();

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
			ModelOBJ::Vertex v0, v1, v2;
			v0 = pVertices[ idxBuffer[i] ];
			v1 = pVertices[ idxBuffer[i+1] ];
			v2 = pVertices[ idxBuffer[i+2] ];
			

			// TODO: With a vertex, you have several pieces of data (see the model_obj.h file for
			// more detail about what is stored in ModelOBJ::Vertex.  You certainly have
			// positions (for instance v2.position[0], v2.position[1], v2.position[2]).  You
			// also have smoothed normals if you want really nice looking OBJ renderings:
			// v0.normal[0],v0.normal[1],v0.normal[2].
		}
	}
}

Mesh::~Mesh()
{

}

