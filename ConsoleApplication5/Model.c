#include "Assimp\Include\assimp\cimport.h"
#include "Assimp\Include\assimp\postprocess.h"
#include "Assimp\Include\assimp\scene.h"

#include "BindingIncludes.h"
#include "Window.h"
#include "GouraudPST.h"
#include "GouraudVST.h"
#include "Header.h"
#include "Vector.h"
#include "Model.h"
#include "Class.h"

#undef c_class
#define c_class Mesh

#pragma warning(disable:4133)

void virtual(Draw)(selfptr, struct VLine* gfx, FXMMATRIX accumulatedTransform)
{
	const account(self);
	VMStoreFloat4x4A(&self->transform, accumulatedTransform);
	base.method->Draw(self, gfx);
}
VMMATRIX virtual(GetTransformXM)(const void* self)
{
	const account(self);
	return VMLoadFloat4x4A(&this->transform);
}

Constructor(selfptr, va_list *ap)
{
	struct c_class *this = ((struct Class*)Drawable)->ctor(self, ap);
	self->Draw = virtual(Draw);
	override(GetTransformXM);

	shared_ptr* bindptrs = va_arg(*ap, shared_ptr*);
	size_t nbinds = va_arg(*ap, size_t);

	for (size_t i = 0; i < nbinds; i++)
	{
		base.method->AddBind(this, bindptrs[i]);
	}

	base.method->AddBind(self, make_shared(TransformBuffer, self, 0));
	return self;
}
Destructor(selfptr)
{
	struct c_class *this = ((struct Class*)Drawable)->dtor(self);
	return this;
}
ENDCLASSDESC

#undef c_class
#define c_class MNode

void _AddChild(selfptr, Node* pChild)
{
	assert(pChild);
	void* _x = self->childPtrs.method->get_next(&self->childPtrs);
	*(Node**)_x = pChild;
}
void virtual(Draw)(selfptr, struct VLine* gfx, FXMMATRIX accumulatedTransform)
{
	FXMMATRIX built = VMMatrixMultiply(
		VMMatrixMultiply(VMLoadFloat4x4A(&self->appliedTransform), &self->transform),
		&accumulatedTransform);

	for (size_t i = 0; i < self->nmeshes; i++)
	{
		Draw_Mesh(self->meshPtrs[i], gfx, built);
		//gfx->Debug->method->OutputToScreen(gfx->Debug, gfx->gfx->localFrame);
	}
	if (self->childPtrs.elements == -1) return;
	
	for (size_t i = 0; i<self->childPtrs.elements+1; i++)
	{
		virtual(Draw)(*(void**)self->childPtrs.method->get(&self->childPtrs, i), gfx, built);
	}

}

Constructor(selfptr, va_list *ap)
{
	self->Draw = virtual(Draw);
	self->id = va_arg(*ap, int);
	self->name = va_arg(*ap, char*);
	self->nmeshes = va_arg(*ap, size_t);

	if(self->nmeshes) self->meshPtrs = malloc(self->nmeshes * sizeof(struct Mesh*));

	VMStoreFloat4x4A(&self->transform, va_arg(*ap, VMMATRIX));
	VMStoreFloat4x4A(&self->appliedTransform, VMMatrixIdentity());
	construct(&self->childPtrs, Vector, sizeof(struct MNode*));
	return self;
}
Destructor(selfptr)
{
	if(self->meshPtrs) free(self->meshPtrs);
	deconstruct(&self->childPtrs);
	return self;
}
ENDCLASSDESC

#undef c_class
#define c_class Model

typedef struct
{
	VMFLOAT3A pos;
	VMFLOAT3A n;
	VMFLOAT2A tc;
}virtual(Vertex);

struct Mesh* ParseMesh(const struct aiMesh* mesh)
{
	virtual(Vertex)* vertices = malloc(sizeof(virtual(Vertex))*mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		VMStoreFloat3A(&(vertices + i)->pos, VMLoadFloat3((VMFLOAT3*)&mesh->mVertices[i]));
		VMStoreFloat3A(&(vertices + i)->n, VMLoadFloat3((VMFLOAT3*)&mesh->mNormals[i]));
		VMStoreFloat3A(&(vertices + i)->tc, VMLoadFloat3((VMFLOAT2*)&mesh->mTextureCoords[0][i])); 
	}

	size_t* indices = malloc(mesh->mNumFaces * 3 * sizeof(size_t));
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const struct aiFace* face = mesh->mFaces+i;
		assert(face->mNumIndices == 3);
		indices[3 * i + 0] = face->mIndices[0];
		indices[3 * i + 1] = face->mIndices[1];
		indices[3 * i + 2] = face->mIndices[2];
	}
	shared_ptr* pBinds = malloc(sizeof(shared_ptr) * 3);
	pBinds[0] = Resolve_VertexShader((char*)stringOf(GouraudVST), GouraudVST);
	pBinds[1] = Resolve_PixelShader((char*)stringOf(GouraudPST), GouraudPST);
	pBinds[2] = Resolve_TextureBuffer((char*)"Tex0", tex, 0u);

	struct Mesh* pMesh = new(Mesh, pBinds, 3);
	pMesh->_base.trilist.indices = indices;
	pMesh->_base.trilist.vertices = vertices;
	pMesh->_base.trilist.numInds = mesh->mNumFaces * 3;
	pMesh->_base.trilist.numVerts = mesh->mNumVertices;
	pMesh->_base.trilist.VSize = sizeof(Vertex_Model);

	free(pBinds);
	return pMesh;
}
struct MNode* ParseNode(selfptr, int* nextId, const struct aiNode* node)
{
	FXMMATRIX transform = VMMatrixTranspose(VMLoadFloat4x4(
		(const VMFLOAT4X4A*)(&node->mTransformation)
	));

	struct MNode* pNode = new(MNode, (*nextId)++, node->mName.data, node->mNumMeshes, transform);
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		const unsigned meshIdx = node->mMeshes[i];
		pNode->meshPtrs[i] = self->meshPtrs[meshIdx];
	}
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		_AddChild(pNode, ParseNode(self, nextId, node->mChildren[i]));
	}
	return pNode;
}
void virtual(Draw)(const selfptr, struct VLine* gfx)
{
	Draw_MNode(self->pRoot, gfx, VMMatrixIdentity());
}

VirtualTable{
	.Draw = virtual(Draw)
};
Constructor(selfptr, va_list *ap)
{
	assignMethodTable(self);
	const char* filename = va_arg(*ap, const char*);

	const struct aiScene* pScene = aiImportFile(filename,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals);

	if (pScene == nullptr)
	{
		return nullptr;
	}

	
	self->nmeshes = pScene->mNumMeshes;
	self->meshPtrs = _aligned_malloc(self->nmeshes * sizeof(struct Mesh*), sizeof(struct Mesh*));
	for (size_t i = 0; i < pScene->mNumMeshes; i++)
	{
		self->meshPtrs[i] = ParseMesh(pScene->mMeshes[i]);
	}

	int nextId = 0;
	self->pRoot = ParseNode(self, &nextId, pScene->mRootNode);
	aiReleaseImport(pScene);
	return self;
}
Destructor(selfptr)
{
	for (size_t i = 0; i < self->nmeshes; i++)
	{
		delete(self->meshPtrs[i]);
	}
	_aligned_free(self->meshPtrs);
	return self;
}
ENDCLASSDESC