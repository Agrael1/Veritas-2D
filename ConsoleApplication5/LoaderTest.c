#include "Assimp\Include\assimp\cimport.h"
#include "Assimp\Include\assimp\postprocess.h"
#include "Assimp\Include\assimp\scene.h"
#include "IndexedTriangleList.h"

#include "GouraudPS.h"
#include "GouraudVS.h"

#include "LoaderTest.h"
#include "Class.h"

#pragma comment(lib, "assimp-vc140-mt.lib")

Constructor(selfptr, va_list *ap)
{
	struct c_class* this = ((struct Class*)TestObject)->ctor(self, ap);

	float scale = (float)va_arg(*ap, double);

	const struct aiScene* pScene = aiImportFile("Models/suzanne.obj",
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices);
	const struct aiMesh* pMesh = pScene->mMeshes[0];

	// Load Verts
	virtual(Vertex)* pVerts = malloc(sizeof(virtual(Vertex))*pMesh->mNumVertices);
	for (unsigned i = 0; i < pMesh->mNumVertices; i++)
	{
		pVerts[i].pos = (SVMVECTOR) {
				pMesh->mVertices[i].x*scale,
				pMesh->mVertices[i].y*scale,
				pMesh->mVertices[i].z*scale,
				1.0f};
		pVerts[i].n = VMLoadFloat3((VMFLOAT3*)&pMesh->mNormals[i]);
	}

	this->model.vertices = pVerts;
	this->model.numVerts = pMesh->mNumVertices;
	this->model.VSize = sizeof(virtual(Vertex));
	this->model.numInds = pMesh->mNumFaces * 3;

	//Load Inds
	size_t* inds = malloc(sizeof(size_t) * pMesh->mNumFaces * 3);
	for (unsigned i = 0; i < pMesh->mNumFaces; i++)
	{
		assert(pMesh->mFaces[i].mNumIndices == 3);
		inds[i * 3 + 0] = pMesh->mFaces[i].mIndices[0];
		inds[i * 3 + 1] = pMesh->mFaces[i].mIndices[1];
		inds[i * 3 + 2] = pMesh->mFaces[i].mIndices[2];
	}
	this->model.indices = inds;

	aiReleaseImport(pScene);

	this->VS = new(GouraudVS);
	this->PS = new(GouraudPS);
	return this; 
}
Destructor(selfptr)
{
	struct c_class* this = ((struct Class*)TestObject)->dtor(self);
	delete(this->VS);
	delete(this->PS);
	return self;
}
ENDCLASSDESC
