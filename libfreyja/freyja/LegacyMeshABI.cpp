/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the ABI to expose Mesh class and utils to C.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2006.07.09:
 * Mongoose - Created, Split from Bone.cpp
 ==========================================================================*/

#define USING_FREYJA_CPP_ABI

#include <math.h>
#include "Mesh.h"
#include "MeshABI.h"
#include "FreyjaPluginABI.h"

using namespace freyja;

extern Vector<Mesh *> gFreyjaMeshes;
index_t gFreyjaCurrentModel = 0;
index_t gFreyjaCurrentMesh = INDEX_INVALID;


index_t freyjaGetFSMMeshIndex()
{
	return gFreyjaCurrentMesh;	
}


////////////////////////////////////
// Rejected 0.10.0 ABI
////////////////////////////////////


//////////////////////////////////////////////////////
// Model
//
//////////////////////////////////////////////////////

uint32 freyjaGetModelCount()
{
	return 1;  // Egg backend is single model
}


char freyjaGetModelAppendMeshMode(index_t modelIndex)
{
	//if (modelIndex < gCopyModels.size())
	//{
	//	return gCopyModels[modelIndex]->getAppendMeshMode();
	//}

	return -1;
}


void freyjaModelAppendMeshMode(index_t modelIndex, char on)
{
	//if (modelIndex < gCopyModels.size())
	//{
	//	gCopyModels[modelIndex]->setAppendMeshMode(on);
	//}
}


char freyjaModelCopyMesh(index_t modelIndex, index_t mesh, index_t frame)
{
	//if (modelIndex < gCopyModels.size())
	//{
	//	return gCopyModels[modelIndex]->copyMesh(mesh, frame);
	//}

	return -1;
}


char freyjaModelPasteMesh(index_t modelIndex)
{
	//if (modelIndex < gCopyModels.size())
	//{
	//	return gCopyModels[modelIndex]->pasteMesh();
	//}

	return -1;
}


char freyjaModelCopyVertexList(index_t modelIndex, 
							   Vector<unsigned int> &list,
							   int mesh, int frame)
{
	//if (modelIndex < gCopyModels.size())
	//{
	//	return gCopyModels[modelIndex]->copyVertexBuffer(list, mesh, frame);
	//}

	return -1;
}

uint32 freyjaModelGetMeshCount(index_t modelIndex)
{
	return gFreyjaMeshes.size();	
}


index_t freyjaGetModelMeshCount(index_t modelIndex)
{
	return gFreyjaMeshes.size();
}


index_t freyjaGetModelFlags(index_t modelIndex)
{
	return 0x0;
}


index_t freyjaGetModelMeshIndex(index_t modelIndex, uint32 element)
{
	return element;
}


index_t freyjaModelCreateMesh(index_t modelIndex)
{
	return freyjaMeshCreate();
}


void freyjaModelDeleteMesh(index_t modelIndex, index_t mesh)
{
	freyjaMeshDelete(mesh);
}


Mesh *freyjaModelGetMeshClass(index_t modelIndex, index_t meshIndex)
{
	return freyjaGetMeshClass(meshIndex);
}


Mesh *freyjaGetCurrentMeshClass()
{
	return freyjaModelGetMeshClass(gFreyjaCurrentModel, gFreyjaCurrentMesh);
}


char freyjaIsVertexAllocated(index_t vertexIndex)
{
	Mesh *mesh = freyjaGetMeshClass(gFreyjaCurrentMesh);

	if (mesh)
		return (mesh->GetVertex(vertexIndex) != NULL);

	return 0;
}

// Useful for 0.9.3 ABI functions
Vertex *freyjaGetCurrentMeshVertexClass(index_t vertexIndex)
{
	Mesh *mesh = freyjaGetCurrentMeshClass();
	return (mesh) ? mesh->GetVertex(vertexIndex) : NULL;
}


// Useful for 0.9.3 ABI functions
Face *freyjaGetCurrentMeshFaceClass(index_t faceIndex)
{
	Mesh *mesh = freyjaGetCurrentMeshClass();
	return (mesh) ? mesh->GetFace(faceIndex) : NULL;
}


index_t freyjaGetCurrentMesh()
{
	return gFreyjaCurrentMesh;
}


void freyjaCurrentMesh(index_t idx)
{
	gFreyjaCurrentMesh = idx;
}


void freyjaVertexListTransform(Vector<uint32> &list,
							   freyja_transform_action_t action, 
							   vec_t x, vec_t y, vec_t z)
{
	switch (action)
	{
	case fTranslate:
		break;

	case fRotate:
		break;

	case fScale:
		break;

	case fScaleAboutPoint:
		break;

	case fRotateAboutPoint:
		break;
	}

	BUG_ME("Not Implemented");
}


index_t freyjaTexCoordCreate2f(vec_t u, vec_t v)
{
	Mesh *mesh = freyjaGetCurrentMeshClass();

	if (mesh)
	{
		vec3_t uvw = {u, v, 0.0f };
		return mesh->CreateTexCoord(uvw);
	}

	return INDEX_INVALID;
}


void freyjaModelMeshPolygonAddTexCoord1i(index_t modelIndex, index_t meshIndex, 
										 index_t polygonIndex, index_t texcoordIndex)
{
	Mesh *mesh = freyjaModelGetMeshClass(modelIndex, meshIndex);

	if (mesh)
	{
		Face *f = mesh->GetFace(polygonIndex);

		if (f)
		{
			f->mFlags |= Face::fPolyMappedTexCoords;
			f->mTexCoordIndices.pushBack(texcoordIndex);
		}
	}
}


index_t freyjaModelMeshVertexCreate(index_t modelIndex, index_t meshIndex,
									vec3_t xyz, vec3_t uvw, vec3_t nxyz)
{
	Mesh *mesh = freyjaModelGetMeshClass(modelIndex, meshIndex);

	if ( mesh != NULL )
	{
		index_t vertex = mesh->CreateVertex(xyz, uvw, nxyz);
		//freyjaPrintMessage("Model[%i].Mesh[%i].Vertex[%i] created.",
		//				   modelIndex, meshIndex, vertex);

		// Debug Check
		//freyjaPrintMessage("-> %f %f %f", xyz[0], xyz[0], xyz[2]);
		//mesh->GetVertexPos(vertex, xyz);		
		//freyjaPrintMessage("<- %f %f %f", xyz[0], xyz[0], xyz[2]);
		return vertex;
	}

	return INDEX_INVALID;
}


// FIXME: Might move back to old 'weights embedded in wedge/Vertex' later
// This implementation is only for legacy support, and it's very slow...
void freyjaGetVertexWeight(index_t vertexIndex, uint32 element,
						   index_t *bone, vec_t *weight)
{
	Mesh *m = freyjaModelGetMeshClass(gFreyjaCurrentModel, gFreyjaCurrentMesh);
	uint32 count = 0;

	if (m)
	{
		for (uint32 i = 0, n = m->GetWeightCount(); i < n; ++i)
		{
			Weight *w = m->GetWeight(i);

			if (w)
			{
				if (w->mVertexIndex == vertexIndex)
				{
					if (count == element)
					{
						*bone = w->mBoneIndex;
						*weight = w->mWeight;
						return;
					}
					++count;
				}
			}
		}
	}
}


// FIXME: Might move back to old 'weights embedded in wedge/Vertex' later
// This implementation is only for legacy support, and it's very slow...
index_t freyjaGetVertexWeightCount(index_t vertexIndex)
{
	Mesh *m = freyjaModelGetMeshClass(gFreyjaCurrentModel, gFreyjaCurrentMesh);
	uint32 count = 0;

	if (m)
	{
		for (uint32 i = 0, n = m->GetWeightCount(); i < n; ++i)
		{
			Weight *w = m->GetWeight(i);

			if (w)
			{
				if (w->mVertexIndex == vertexIndex)
					++count;
			}
		}
	}

	return count;
}



index_t freyjaGetPolygonVertexIndex(index_t polygonIndex, uint32 element)
{
	Face *f = freyjaGetCurrentMeshFaceClass(polygonIndex);
	
	if (f)
	{
		return f->mIndices[element];
	}

	return 0;
}


void freyjaPolygonTexCoordPurge(index_t polygonIndex)
{
	Mesh *mesh = freyjaModelGetMeshClass(gFreyjaCurrentModel, gFreyjaCurrentMesh);

	if (!mesh)
		return;

	Face *f = mesh->GetFace(polygonIndex);

	if (f)
	{
		f->mTexCoordIndices.clear();
	}
}


index_t freyjaGetPolygonTexCoordIndex(index_t polygonIndex, uint32 element)
{
	Face *f = freyjaGetCurrentMeshFaceClass(polygonIndex);
	
	if (f)
	{
		return f->mTexCoordIndices[element];
	}

	return 0;
}


void freyjaModelClampTexCoords(index_t modelIndex)
{
	uint32 i, count = freyjaModelGetMeshCount(modelIndex);

	for ( i = 0; i < count; ++i )
	{
		// FIXME: GetNext? ForEach?
		Mesh *mesh = freyjaModelGetMeshClass(modelIndex, i);

		if (mesh != NULL)
			mesh->ClampAllTexCoords();
	}
}

index_t freyjaGetPolygonMaterial(index_t polygonIndex)
{
	Face *f = freyjaGetCurrentMeshFaceClass(polygonIndex);
	
	if (f)
	{
		return f->mMaterial;
	}

	return 0;
}


index_t freyjaGetPolygonFlags(index_t polygonIndex)
{
	Face *f = freyjaGetCurrentMeshFaceClass(polygonIndex);
	
	if (f)
	{
		return f->mFlags;
	}

	return 0;
}


void freyjaModelMeshTransform3fv(index_t modelIndex, index_t meshIndex, 
								freyja_transform_action_t action, vec3_t xyz)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (mesh == NULL)
		return;

	switch (action)
	{
	case fRotate:
		{
			Matrix t, r, t2, mat;
			// Rotate about bounding volume center instead of origin
			t.translate(mesh->GetBoundingVolumeCenter().mVec);
			r.rotate(xyz);
			t2.translate((-mesh->GetBoundingVolumeCenter()).mVec);
			//mat = t*r*t2;
			//mesh->TransformVertices(mat);

			// FIXME: Fix the damn matrix backend to avoid such expensive
			//        processing here ( only want to transform once )
			mesh->TransformVertices(t2);
			mesh->TransformVertices(r);
			mesh->TransformVertices(t);

			// Transform normals by inverted rotation to stay correct
			Matrix nr;
			nr.rotate(xyz);
			nr.invert();
			mesh->TransformNormals(nr);
		}
		break;

	case fScale:
		{
			Matrix s;
			s.scale(xyz);
			mesh->TransformVertices(s);
		}
		break;

	case fTranslate:
		{
			mesh->Translate(Vec3(xyz));
		}
		break;
		
	default:
		BUG_ME("Case '%i' Not Implemented", action);
	}
}


void freyjaMeshVertexTranslate3fv(index_t meshIndex, 
								  index_t vertexIndex, vec3_t xyz)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (mesh)
	{
		Vertex *vert = mesh->GetVertex(vertexIndex);

		if (vert)
		{
#if 0
			Vec3 v;
			Vec3 u(xyz);
			mesh->GetVertexArrayPos(vert->mVertexIndex, v.mVec);
			mesh->SetVertexArrayPos(vert->mVertexIndex, (v+(u-v)).mVec);
#else
			mesh->SetVertexArrayPos(vert->mVertexIndex, xyz);
#endif
		}
	}
}

index_t freyjaModelMeshPolygonCreate(index_t modelIndex, index_t meshIndex)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if ( mesh != NULL )
	{
		index_t polygon = mesh->CreateFace();
		return polygon;
	}

	return INDEX_INVALID;
}

void freyjaModelMeshPolygonAddVertex1i(index_t modelIndex, index_t meshIndex, 
									   index_t polygonIndex, index_t vertexIndex)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	ASSERT_MSG(mesh != NULL, "Model[%i].Mesh[%i] == NULL", modelIndex, meshIndex);

	if ( mesh != NULL )
	{
		Face *face = mesh->GetFace(polygonIndex);
		Vertex *vertex = mesh->GetVertex(vertexIndex);

		ASSERT_MSG(face != NULL, "Model[%i].Mesh[%i].Polygon[%i] == NULL", modelIndex, meshIndex, polygonIndex);

		ASSERT_MSG(vertex != NULL, "vertex == NULL");

		if ( face != NULL && vertex != NULL )
		{
			for ( uint32 i = 0; i < face->mIndices.size(); ++i )
			{
				if (face->mIndices[i] == vertexIndex)
				{
					MARK_MSGF("freyjaModelMeshPolygonAddVertex1i, Tried to insert duplicate vertex into face[%i].  %i  %i", polygonIndex, face->mIndices[i], vertexIndex);
					return;
				}
			}

			face->mIndices.pushBack(vertexIndex);
		}
	}
}


uint32 freyjaGetPolygonEdgeCount(index_t polygonIndex)
{
	return freyjaGetMeshPolygonEdgeCount(gFreyjaCurrentMesh, polygonIndex);
}


void freyjaModelClear(index_t modelIndex)
{
	// Currently there is no multimodel while egg is being used
	freyjaSkeletonPoolClear();
	freyjaBonePoolClear();
	

	// Purge meshes
	uint32 i, count = freyjaModelGetMeshCount(modelIndex);

	for ( i = 0; i < count; ++i )
	{
		freyjaModelDeleteMesh(modelIndex, i);
	}
}


index_t freyjaMeshVertexCreate3f(index_t meshIndex, index_t groupIndex,
								 vec_t x, vec_t y, vec_t z)
{
	vec3_t xyz = {x,y,z};
	vec3_t uvw = {.5,.5,0};
	vec3_t nxyz = {0,1,0};
	return freyjaModelMeshVertexCreate(gFreyjaCurrentModel,
									   meshIndex, xyz, uvw, nxyz);
}


void freyjaVertexWeight(index_t vertexIndex, vec_t weight, index_t bone)
{
	Mesh *mesh = freyjaGetCurrentMeshClass();

	if ( mesh != NULL )
	{
		mesh->AddWeight(vertexIndex, weight, bone);
	}
}


void freyjaTexCoordCombine(uint32 indexA, uint32 indexB)
{
	Mesh *mesh = freyjaGetCurrentMeshClass();

	if ( mesh != NULL )
	{
		mesh->WeldTexCoords(indexA, indexB);
	}	
}


void freyjaPolygonAddVertex1i(index_t polygonIndex, index_t vertexIndex)
{
	MARK_MSG("Obsolete function call");
	Face *f = freyjaGetCurrentMeshFaceClass(polygonIndex);

	if (f)
	{
		f->mIndices.pushBack(polygonIndex);
	}
}


int32 freyjaVertexXYZ3fv(index_t vertexIndex, vec3_t xyz)
{
	Mesh *mesh = freyjaGetCurrentMeshClass();

	if ( mesh != NULL )
	{
		Vertex *vert = mesh->GetVertex(vertexIndex);

		if (vert)
		{
			//mesh->GetVertexArrayPos(vertexIndex, xyz);
			mesh->GetVertexPos(vertexIndex, xyz);
		 
			return 0;
		}
	}

	return -1;
}

void freyjaVertexTexCoord2fv(index_t vIndex, vec2_t uv)
{	
	Mesh *mesh = freyjaGetCurrentMeshClass();

	if ( mesh != NULL )
	{
		mesh->SetTexCoord(vIndex, uv);
	}	
}


void freyjaVertexTexcoord2f(index_t vIndex, vec_t u, vec_t v)
{	
	Mesh *mesh = freyjaGetCurrentMeshClass();

	if ( mesh != NULL )
	{
		vec2_t uv = {u, v};
		mesh->SetTexCoord(vIndex, uv);
	}
}


void freyjaVertexNormalFlip(index_t vIndex)
{	
	Vector3d n;


	freyjaGetVertexNormalXYZ3fv(vIndex, n.mVec);
	n = -n;
	n.normalize();
	freyjaVertexNormal3fv(vIndex, n.mVec);
}


void freyjaVertexNormal3fv(index_t vIndex, vec3_t nxyz)
{
	Mesh *mesh = freyjaGetCurrentMeshClass();

	if ( mesh != NULL )
	{
		mesh->SetNormal(vIndex, nxyz);
	}
}


void freyjaVertexNormal3f(index_t vIndex, vec_t x, vec_t y, vec_t z)
{
	vec3_t nxyz = {x,y,z};
	freyjaVertexNormal3fv(vIndex, nxyz);
}


index_t freyjaPolygonCreate()
{
	Mesh *mesh = freyjaGetCurrentMeshClass();

	if ( mesh != NULL )
	{
		return mesh->CreateFace();
	}

	return INDEX_INVALID;
}



index_t freyjaVertexCombine(index_t vertexIndexA, index_t vertexIndexB)
{	
	Mesh *mesh = freyjaGetCurrentMeshClass();

	if ( mesh != NULL )
	{
		if (mesh->WeldVertices(vertexIndexA, vertexIndexB))
			return vertexIndexA;
	}	

	return INDEX_INVALID;
}

void freyjaVertexDelete(index_t vertexIndex)
{	
	Mesh *mesh = freyjaGetCurrentMeshClass();

	if ( mesh != NULL )
	{
		mesh->DeleteVertex(vertexIndex);
	}
}


void freyjaVertexPosition3fv(index_t vertexIndex, vec3_t xyz)
{
	Mesh *mesh = freyjaGetCurrentMeshClass();

	if ( mesh != NULL )
	{
		// FIXME: I think to keep 0.9.3 compat it should ref GetVertexPos
		mesh->GetVertexArrayPos(vertexIndex, xyz);
	}
}


index_t freyjaTexCoordCreate2fv(const vec2_t uv)
{
	return freyjaTexCoordCreate2f(uv[0], uv[1]);
}



vec3_t *freyjaGetVertexXYZ(index_t vertexIndex)
{
	Mesh *mesh = freyjaModelGetMeshClass(gFreyjaCurrentModel, gFreyjaCurrentMesh);

	if (mesh && vertexIndex < mesh->GetVertexCount())
	{
		return (vec3_t *)(mesh->GetVertexArray() + vertexIndex * 3);
	}

	return NULL;
}


uint32 freyjaGetPolygonVertexCount(index_t polygonIndex)
{
	Face *f = freyjaGetCurrentMeshFaceClass(polygonIndex);
	return f ? f->mIndices.size() : 0;
}


uint32 freyjaGetPolygonTexCoordCount(index_t polygonIndex)
{
	Mesh *mesh = freyjaGetCurrentMeshClass();

	if (mesh)
	{
		Face *f = mesh->GetFace(polygonIndex);

		if (f && f->mFlags & Face::fPolyMappedTexCoords)
		{
			return f->mTexCoordIndices.size();
		}
	}

	return 0;
}



uint32 freyjaGetMeshVertexGroupVertexCount(index_t meshIndex, uint32 element)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (mesh == NULL)
		return 0;

	return mesh->GetVertexCount();
}


// NOTE: Only 1 'vertex group' per mesh in 0.9.5
index_t freyjaGetMeshVertexGroupVertexIndex(index_t meshIndex, uint32 element,
										  uint32 vertexElement)
{
	//FIXME("freyjaGetMeshVertexGroupVertexIndex Implementation might need artifical gobal mapping to retian 0.9.3 function", __FILE__, __LINE__);
	Mesh *mesh = freyjaGetMeshClass( meshIndex);

	if ( !mesh || vertexElement > mesh->GetVertexCount() || element > 0 )
		return INDEX_INVALID;

	return element;
}


// NOTE: Only 1 'vertex group' per mesh in 0.9.5
uint32 freyjaGetMeshVertexGroupCount(index_t meshIndex)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (mesh && mesh->GetVertexCount()) 
		return 1;

	return 0;
}

void freyjaGetVertexTexcoord2fv(index_t vertexIndex, vec2_t uv)
{
	Mesh *mesh = freyjaGetCurrentMeshClass();

	if (mesh)
	{
		Vertex *v = mesh->GetVertex(vertexIndex);

		if (v)
		{
			vec3_t uvw;
			mesh->GetTexCoord(v->mTexCoordIndex, uvw);
			uv[0] = uvw[0];
			uv[1] = uvw[1];
		}
	}
}


void freyjaGetVertexNormalXYZ3fv(index_t vertexIndex, vec3_t nxyz)
{
	Mesh *mesh = freyjaGetCurrentMeshClass();

	if (mesh)
	{
		Vertex *v = mesh->GetVertex(vertexIndex);

		if (v)
		{
			mesh->GetNormal(v->mNormalIndex, nxyz);
		}
	}
}


void freyjaGetVertexXYZ3fv(index_t vertexIndex, vec3_t xyz)
{
	Mesh *mesh = freyjaModelGetMeshClass(gFreyjaCurrentModel, gFreyjaCurrentMesh);

	if (mesh)
	{
		Vector3d v = mesh->GetVertexPosition(vertexIndex);
		HEL_VEC3_COPY(v.mVec, xyz);
	}
}



index_t freyjaGetVertexFlags(index_t vertexIndex)
{
	Vertex *v = freyjaGetCurrentMeshVertexClass(vertexIndex);
	return v ? v->mFlags : 0;
}



void freyjaMeshTransform(index_t meshIndex, uint32 frame,
						 freyja_transform_action_t action, 
						 vec_t x, vec_t y, vec_t z)
{
	MARK_MSG("WARNING 0.9.5 API has no concept of 0.9.3 'mesh frames'");
	vec3_t v = {x,y,z};
	freyjaModelMeshTransform3fv(gFreyjaCurrentModel, meshIndex, action, v);
}


void freyjaPolygonAddTexCoord1i(index_t polygonIndex, index_t texcoordIndex)
{
	Face *f = freyjaGetCurrentMeshFaceClass(polygonIndex);
	
	if (f)
	{
		f->mFlags |= Face::fPolyMappedTexCoords;
		f->mTexCoordIndices.pushBack(texcoordIndex);
	}
}


void freyjaPolygonSetMaterial1i(index_t polygonIndex, index_t materialIndex)
{
	Face *f = freyjaGetCurrentMeshFaceClass(polygonIndex);
	
	if (f)
	{
		f->mMaterial = materialIndex;
	}
}


void freyjaMeshPromoteTexcoordsToPloymapping(index_t meshIndex)
{
	vec2_t uv;
	int32 i, j, k, vertexCount, vertexIndex, texcoordIndex, polygonIndex, polygonCount;


	polygonCount = freyjaGetMeshPolygonCount(meshIndex);

    for (i = 0; i < polygonCount; ++i)
    {
		polygonIndex = i;//freyjaGetMeshPolygonIndex(meshIndex, i);
		vertexCount = freyjaGetPolygonVertexCount(polygonIndex);

		for (j = 0; j < vertexCount; ++j)
		{
			vertexCount = freyjaGetPolygonVertexCount(polygonIndex);

			// NOTE: I just update all UV -> polymapp to avoid corrupt 
			//       'texture faces' eg not completely polymapped
			freyjaPolygonTexCoordPurge(polygonIndex);

			for (k = 0; k < vertexCount; ++k) 
			{
				vertexIndex = freyjaGetPolygonVertexIndex(polygonIndex, k);
				freyjaGetVertexTexcoord2fv(vertexIndex, uv);

				texcoordIndex = freyjaTexCoordCreate2fv(uv);
				freyjaPolygonAddTexCoord1i(polygonIndex, texcoordIndex);
			}
		}
	}
}


void freyjaTexCoord2f(index_t texcoordIndex, vec_t u, vec_t v)
{
	vec2_t uv = {u, v};

	freyjaMeshTexCoord2fv(gFreyjaCurrentMesh, texcoordIndex, uv);
}


index_t freyjaGetMeshPolygonIndex(index_t meshIndex, uint32 element)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (!mesh || element > mesh->GetFaceCount())
		return INDEX_INVALID;

	return element;
}


#if FREYJA_OBSOLETE_ABI

void freyjaTexCoord2fv(index_t texcoordIndex, const vec2_t uv)
{
	BUG_ME("Not Implemented");
}


void freyjaGetTexCoord2fv(index_t tindex, vec2_t uv)
{
	BUG_ME("Not Implemented");
}


uint32 freyjaGetMeshVertexFrameCount(index_t meshIndex)
{
	uint32 polygonCount = freyjaGetMeshPolygonCount(meshIndex);
	uint32 i, j, frames, maxFrames = 0;
	index_t vertexIndex, vertexCount, polygonIndex;

	/* Mongoose 2005.01.01, 
	 * Vertex frames are stored in the vertices themselves, find the 
	 * max frame count -- indexing greater than frame count will
	 * return frame[0] position in vertices with less than maxFrames
	 * frame, so it's safe and does what one would expect asymmetrically */
	for (i = 0; i < polygonCount; ++i)
	{
		polygonIndex = i;//freyjaGetMeshPolygonIndex(meshIndex, i);
		vertexCount = freyjaGetPolygonVertexCount(polygonIndex);

		for (j = 0; j < vertexCount; ++j)
		{
			vertexIndex = j;//freyjaGetPolygonVertexIndex(polygonIndex, j);
			frames = freyjaGetVertexFrameCount(vertexIndex);

			if (frames > maxFrames)
				maxFrames = frames;
		}
	}

	return maxFrames;
}

index_t freyjaGetMeshTexCoordIndex(index_t meshIndex, uint32 element)
{
	BUG_ME("Not Implemented");
	return INDEX_INVALID;
}

void freyjaVertexFrame3f(index_t idx, vec_t x, vec_t y, vec_t z)
{
	BUG_ME("OBSOLETE ABI CALL");
}


void freyjaMeshVertexGroupAppendGobalVertex(index_t meshIndex, uint32 element, 
											index_t vertexIndex)
{
	BUG_ME("Not Implemented");
}


void freyjaModelMirrorTexCoord(index_t modelIndex, index_t texcoordIndex,
								Vector<int32> uvMap, bool x, bool y)
{
	BUG_ME("Not Implemented");
}


void freyjaModelTransformTexCoord(index_t modelIndex, index_t texcoordIndex,
									freyja_transform_action_t action,
									vec_t x, vec_t y)
{
	BUG_ME("Not Implemented");
}


index_t freyjaGetMeshVertexGroupIndex(index_t meshIndex, uint32 element)
{
	//BUG_ME("Not Implemented");

	if (element > 0)
		return INDEX_INVALID;

	return 0;
}


index_t freyjaGetMeshVertexFrameIndex(index_t meshIndex, uint32 element)
{
	BUG_ME("Not Implemented");
	return 0;//INDEX_INVALID;
}

index_t freyjaGetMeshPolygonVertexIndex(index_t meshIndex, index_t faceVertexIndex)
{
	BUG_ME("Not Implemented");
	return INDEX_INVALID;
}


index_t freyjaGetMeshVertexIndex(index_t meshIndex, uint32 element)
{
	BUG_ME("Not Implemented");
	return INDEX_INVALID;
}


void freyjaMeshAddPolygon(index_t meshIndex, index_t polygonIndex)
{
	BUG_ME("Not Implemented");
}


void freyjaGetVertexPolygonRef1i(index_t vertexIndex, Vector<long> &polygons)
{
	BUG_ME("Not Implemented");
}


void freyjaGetVertexPolygonRef(Vector<long> &polygons)
{
	BUG_ME("Not Implemented");
}

void freyjaMeshFrameTransform(index_t meshIndex, uint32 frame,
							  freyja_transform_action_t action, 
							  vec_t x, vec_t y, vec_t z)
{
	MARK_MSG("ERROR 0.9.5 API has no concept of 0.9.3 'mesh frames', No object on which to operate.");
}


void freyjaModelTransform(index_t modelIndex,
						  freyja_transform_action_t action, 
						  vec_t x, vec_t y, vec_t z)
{
	switch (action)
	{
	case fTranslate:
		break;

	case fRotate:
		break;

	case fScale:
		break;

	case fScaleAboutPoint:
		break;

	case fRotateAboutPoint:
		break;
	}

	BUG_ME("Not Implemented");
}

void freyja__PolygonReplaceReference(index_t polygonIndex, 
									 index_t vertexA, index_t vertexB)
{
	BUG_ME("Not Implemented");
}



void freyja__GetCommonPolygonReferences(index_t vertexA, index_t vertexB,
										Vector<unsigned int> &common)
{
	BUG_ME("Not Implemented");
}


void freyja__GetDifferenceOfPolygonReferences(index_t vertexA, index_t vertexB,
											  Vector<unsigned int> &diffA)
{
	BUG_ME("Not Implemented");
}


void freyjaMeshPolygonSplitTexCoords(index_t meshIndex, index_t polygonIndex)
{	
	BUG_ME("Not Implemented");
}


void freyjaPolygonDelete(index_t polygonIndex)
{
	BUG_ME("Not Implemented");
}


void freyjaPolygonFlagAlpha(index_t polygonIndex, char on)
{
	BUG_ME("Not Implemented");
}



void freyjaMeshRemovePolygon(index_t meshIndex, index_t polygonIndex)
{
	BUG_ME("Not Implemented");
}


void freyjaBoneRemoveVertex(index_t boneIndex, index_t vertexIndex)
{
	BUG_ME("Not Implemented");
}


index_t freyjaGetTexCoordPolygonRefIndex(index_t texcoordIndex, uint32 element)
{
	BUG_ME("Not Implemented");
	return INDEX_INVALID;
}


uint32 freyjaGetTexCoordPolygonRefCount(index_t texcoordIndex)
{
	BUG_ME("Not Implemented");
	return 0;
}


vec2_t *freyjaGetVertexUV(index_t vertexIndex)
{
	BUG_ME("Not Implemented");
	return NULL;
}


vec2_t *freyjaGetTexCoordUV(index_t texcoordIndex)
{
	BUG_ME("Not Implemented");
	return NULL;
}


index_t freyjaGetVertexPolygonRefIndex(index_t vertexIndex, uint32 element)
{
	BUG_ME("Not Implemented");

	return INDEX_INVALID;
}


uint32 freyjaGetVertexPolygonRefCount(index_t vertexIndex)
{
	BUG_ME("Not Implemented");

	return 0;
}

int freyjaVertexExtrude(index_t vertexIndex, vec_t midpointScale, vec3_t normal)
{
	Vector<long> faces, edges;
	Vector3d a, b, c, ab, ac;
	vec3_t xyz;
	vec2_t uv;
	uint32 A, B, C, i, j, material, polygonCount, polygonIndex, vertexCount, texcoordCount, v;
	int32 last;


	freyjaGetVertexPolygonRef1i(vertexIndex, faces);
	polygonCount = faces.end();

	if (!polygonCount)
		return -1;

	for (i = 0; i < polygonCount; ++i)
	{
		polygonIndex = faces[i];
		vertexCount = freyjaGetPolygonVertexCount(polygonIndex);
		texcoordCount = freyjaGetPolygonTexCoordCount(polygonIndex);
		material = freyjaGetPolygonMaterial(polygonIndex);


		// 1. Get edges containing vertexIndex in ith face
		edges.clear();
		
		for (j = 0, last = -1; j < vertexCount; ++j)
		{
			v = freyjaGetPolygonVertexIndex(polygonIndex, j);

			if (v == vertexIndex)
			{
				if (j == vertexCount - 1)
				{
					A = v;
					B = freyjaGetPolygonVertexIndex(polygonIndex, 0);
					C = last;
				}
				else
				{
					A = v;
					B = freyjaGetPolygonVertexIndex(polygonIndex, j+1);
					C = last;

					if (j == 0)
					{
						C = freyjaGetPolygonVertexIndex(polygonIndex, 
														vertexCount - 1);
					}
				}
				break;
			}

			last = v;
		}


		// 2. Compute midpoint of edges AB AC and scale by midpointScale
		freyjaGetVertexXYZ3fv(A, a.mVec);
		freyjaGetVertexXYZ3fv(B, b.mVec);
		freyjaGetVertexXYZ3fv(C, c.mVec);
		ab = a + b;
		ab *= 0.5 * midpointScale;
		ac = a + c;
		ac *= 0.5 * midpointScale;


		// 3. Generate new vertices at scaled midpoints of edges
		B = freyjaVertexCreate3fv(ab.mVec);
		freyjaGetVertexTexcoord2fv(A, uv);
		freyjaVertexTexCoord2fv(B, uv); // FIXME
		freyjaGetVertexNormalXYZ3fv(A, xyz);
		freyjaVertexNormal3fv(B, xyz); // FIXME

		C = freyjaVertexCreate3fv(ac.mVec);
		freyjaGetVertexTexcoord2fv(A, uv);
		freyjaVertexTexCoord2fv(C, uv); // FIXME
		freyjaGetVertexNormalXYZ3fv(A, xyz);
		freyjaVertexNormal3fv(C, xyz); // FIXME


		// 4. Replace references to vertexIndex with each new vertex
		freyja__PolygonReplaceReference(polygonIndex, A, B);
		//freyja__PolygonInsertReferenceAfter(polygonIndex, B, C);

		// 5. Generate new a face ABC for new vertices and vertexIndex
		freyjaBegin(FREYJA_POLYGON);
		freyjaPolygonMaterial1i(material);
		freyjaPolygonVertex1i(A);
		freyjaPolygonVertex1i(B);
		freyjaPolygonVertex1i(C);

		// FIXME: Should be able to generate mixing both uvs
		if (freyjaGetPolygonTexCoordCount(polygonIndex))
		{
			freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(0.25, 0.25));
			freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(0.5, 0.25));
			freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(0.5, 0.5));
		}

		freyjaEnd();
	}


	// 6. Move vertexIndex vertex by 'normal'
	freyjaGetVertexXYZ3fv(vertexIndex, xyz);
	xyz[0] += normal[0];
	xyz[1] += normal[1];
	xyz[2] += normal[2];
	freyjaVertexXYZ3fv(vertexIndex, xyz);

	return 0;
}



#endif
