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
#include "Material.h"
#include "FreyjaPluginABI.h"
#include "ModelABI.h"
#include "MeshABI.h"
#include "BoneABI.h"
#include "MaterialABI.h"
#include "SkeletonABI.h"

#include "LegacyABI.h"


using namespace freyja;

index_t gFreyjaCurrentModel = 0;
index_t gFreyjaCurrentMesh = INDEX_INVALID;


index_t freyjaGetFSMMeshIndex()
{
	return gFreyjaCurrentMesh;	
}


////////////////////////////////////
// Rejected 0.10.0 ABI
////////////////////////////////////

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
	uint32 i, count = freyjaGetModelMeshCount(modelIndex);

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

	FREYJA_ASSERTMSG(mesh != NULL, "Model[%i].Mesh[%i] == NULL", modelIndex, meshIndex);

	if ( mesh != NULL )
	{
		Face *face = mesh->GetFace(polygonIndex);
		Vertex *vertex = mesh->GetVertex(vertexIndex);

		FREYJA_ASSERTMSG(face != NULL, "Model[%i].Mesh[%i].Polygon[%i] == NULL", modelIndex, meshIndex, polygonIndex);

		FREYJA_ASSERTMSG(vertex != NULL, "vertex == NULL");

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
	uint32 i, count = freyjaGetModelMeshCount(modelIndex);

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
	int32  polygonCount = freyjaGetMeshPolygonCount(meshIndex);

    for (int32 i = 0; i < polygonCount; ++i)
    {
		int32 polygonIndex = i;//freyjaGetMeshPolygonIndex(meshIndex, i);
		int32 vertexCount = freyjaGetPolygonVertexCount(polygonIndex);
		int32 texcoordCount = freyjaGetPolygonTexCoordCount(polygonIndex);

		// Mongoose 2006.12.06: This changes old behavoir! 
		// Don't overwrite existing polymapped UVs
		if (vertexCount == texcoordCount)
			continue;

		for (int32 j = 0; j < vertexCount; ++j)
		{
			// NOTE: I just update all UV -> polymapp to avoid corrupt 
			//       'texture faces' eg not completely polymapped
			freyjaPolygonTexCoordPurge(polygonIndex);

			vec2_t uv;

			for (int32 k = 0; k < vertexCount; ++k) 
			{
				int32 vertexIndex = freyjaGetPolygonVertexIndex(polygonIndex, k);
				freyjaGetVertexTexcoord2fv(vertexIndex, uv);

				int32 texcoordIndex = freyjaTexCoordCreate2fv(uv);
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




Vector<unsigned int> *freyjaFindVerticesByBox(vec3_t bbox[2])
{
	Vector<unsigned int> *list;
	int32 i, count;
	vec3_t xyz;

	index_t lock = freyjaCriticalSectionLock();

	count = freyjaGetCount(FREYJA_VERTEX); 

	list = new Vector<unsigned int>();

	if (count < 1)
	{
		/* Return empty list */
		return list;
	}


	/* Using freyja iterator interface */
	freyjaIterator(FREYJA_VERTEX, FREYJA_RESET);
	
	for (i = 0; i < count; ++i)
	{
		freyjaGetVertex3fv(xyz);
		
		if (xyz[0] >= bbox[0][0] && xyz[0] <= bbox[1][0])
		{
			if (xyz[1] >= bbox[0][1] && xyz[1] <= bbox[1][1])
			{
				if (xyz[2] >= bbox[0][2] && xyz[2] <= bbox[1][2])
				{
					list->pushBack(freyjaIterator(FREYJA_VERTEX, 
												  FREYJA_CURRENT));
				}
			}
		}

		freyjaIterator(FREYJA_VERTEX, FREYJA_NEXT);
	}

	freyjaCriticalSectionUnlock(lock);
	
	return list;
}


Vector<unsigned int> *freyjaFindVerticesByBoundingVolume(BoundingVolume &vol)
{
	Vector<unsigned int> *list;
	unsigned int i, count, index;
	vec3_t xyz;


	index_t lock = freyjaCriticalSectionLock();

	count = freyjaGetCount(FREYJA_VERTEX); 

	if (count < 1)
	{
		return 0x0;
	}

	list = new Vector<unsigned int>();

	/* Using freyja iterator interface */
	freyjaIterator(FREYJA_VERTEX, FREYJA_RESET);
	
	for (i = 0; i < count; ++i)
	{
		index = freyjaIterator(FREYJA_VERTEX, FREYJA_CURRENT);

		freyjaGetVertexXYZ3fv(index, xyz);
		
		if (vol.IsPointInside(xyz))
		{
			list->pushBack(index);
		}

		freyjaIterator(FREYJA_VERTEX, FREYJA_NEXT);
	}

	freyjaCriticalSectionUnlock(lock);
	
	return list;
}


Vector<unsigned int> *freyjaFindVerticesInBox(vec3_t bbox[2],
											  Vector<unsigned int> &vertices)
{
	Vector<unsigned int> *list;
	unsigned int i, count, index;
	vec3_t xyz;


	index_t lock = freyjaCriticalSectionLock();

	list = new Vector<unsigned int>();

	/* Using freyja iterator interface */
	for (i = 0; i < count; ++i)
	{
		if (INDEX_INVALID == freyjaIterator(FREYJA_VERTEX, vertices[i]))
			continue;

		index = freyjaIterator(FREYJA_VERTEX, FREYJA_CURRENT);
		freyjaGetVertexXYZ3fv(index, xyz);
		
		if (xyz[0] >= bbox[0][0] && xyz[0] <= bbox[1][0])
		{
			if (xyz[1] >= bbox[0][1] && xyz[1] <= bbox[1][1])
			{
				if (xyz[0] >= bbox[0][2] && xyz[0] <= bbox[1][2])
				{
					list->pushBack(index);
				}
			}
		}

		freyjaIterator(FREYJA_VERTEX, FREYJA_NEXT);
	}

	freyjaCriticalSectionUnlock(lock);
	
	return list;
}


void freyjaModelGenerateVertexNormals(index_t modelIndex)
{
	// Legacy ////////////////////////////////////////////////

	Vector <Vector3d *> faceNormals;
	Vector <long> ref;
	Vector3d a, b, c, aa, bb, normal;
	unsigned int i, j, vertexCount, faceCount;
	int32 v0, v1, v2, index;
	index_t face, mesh;


	index_t lock = freyjaCriticalSectionLock();
	
	freyjaPrintMessage("freyjaGenerateVertexNormals()");
	mesh = freyjaGetCurrent(FREYJA_MESH);
	vertexCount = freyjaGetCount(FREYJA_VERTEX);
	faceCount = freyjaGetCount(FREYJA_POLYGON); 

	freyjaIterator(FREYJA_POLYGON, FREYJA_RESET);

    for (i = 0; i < faceCount; ++i)
    {
		face = freyjaGetCurrent(FREYJA_POLYGON);
		v0 = freyjaGetPolygonVertexIndex(face, 0);
		v1 = freyjaGetPolygonVertexIndex(face, 1);
		v2 = freyjaGetPolygonVertexIndex(face, 2);

		freyjaPrintMessage("<%d %d %d>", v0, v1, v2);
		freyjaGetVertexXYZ3fv(v0, a.mVec);
		freyjaGetVertexXYZ3fv(v1, b.mVec);
		freyjaGetVertexXYZ3fv(v2, c.mVec);

		/* Compute 2 vectors from the triangle face */	
		//aa = b - a;
		//bb = b - c;
		
		/* Compute normal for the face, and store it */
		normal = Vector3d::cross(a - b, c - b);
		normal.normalize();
		faceNormals.pushBack(new Vector3d(normal));

		freyjaIterator(FREYJA_POLYGON, FREYJA_NEXT);
	}

	freyjaIterator(FREYJA_VERTEX, FREYJA_RESET);

	/* Compute vertex normals */
    for (i = 0; i < vertexCount; ++i)
    {
		index = freyjaIterator(FREYJA_VERTEX, FREYJA_CURRENT);

		if (index < 0)
		{
			freyjaPrintError("freyjaGenerateVertexNormals> ERROR bad vertex\n");
			continue;
		}

		normal.zero();

		//freyjaGetVertexPolygonRef(ref);
		//for (j = ref.begin(); j < ref.end(); ++j)
		uint32 jn = freyjaGetMeshVertexPolygonRefCount(mesh, index);
		for (j = 0; j < jn; ++j)
		{
			normal += *faceNormals[freyjaGetMeshVertexPolygonRefIndex(mesh, index, j)];
		}

		normal.normalize();

		freyjaVertexNormal3fv(index,normal.mVec);

		freyjaPrintMessage("%d :: %d faces :: %f %f %f", index,
						   ref.size(),
						   normal.mVec[0], normal.mVec[1], normal.mVec[2]);

		freyjaIterator(FREYJA_VERTEX, FREYJA_NEXT);
    }

	faceNormals.erase();
	
	freyjaCriticalSectionUnlock(lock);
}


void freyjaGetGenericTransform3fv(freyja_transform_t transform,
								  freyja_transform_action_t action,
								  index_t id, vec3_t xyz)
{
	switch (transform)
	{
	case fTransformMesh:
		switch (action)
		{
		case fTranslate:
			freyjaGetMeshPosition(id, xyz);
			break;

		default:
			MSTL_MSG("%s(%s, %s) not supported\n", 
					 __func__,
					 freyjaTransformToString(transform),
					 freyjaActionToString(action));
		}
		break;


	case fTransformBone:
		switch (action)
		{
		case fTranslate:
			freyjaGetBoneTranslation3fv(id, xyz);
			break;

		default:
			MSTL_MSG("%s(%s, %s) not supported\n", 
					 __func__,
					 freyjaTransformToString(transform),
					 freyjaActionToString(action));
		}
		break;

	case fTransformVertex:
		switch (action)
		{
		case fTranslate:
			freyjaGetVertexXYZ3fv(id, xyz);
			break;

		default:
			MSTL_MSG("%s(%s, %s) not supported\n", 
					 __func__,
					 freyjaTransformToString(transform),
					 freyjaActionToString(action));
		}
		break;

	default:
		MSTL_MSG("%s(%s, %s) not supported\n", 
				 __func__,
				 freyjaTransformToString(transform),
				 freyjaActionToString(action));
	}
}

		
void freyjaGenericTransform3fv(freyja_transform_t transform,
							   freyja_transform_action_t action,
							   index_t id, vec3_t xyz)
{
	switch (transform)
	{
	case fTransformMesh:
		switch (action)
		{
		case fTranslate:
			freyjaMeshTransform3fv(id, action, xyz);
			break;

		default:
			MSTL_MSG("%s(%s, %s) not supported\n", 
					 __func__,
					 freyjaTransformToString(transform),
					 freyjaActionToString(action));
		}
		break;

	case fTransformBone:
		switch (action)
		{
		case fTranslate:
			freyjaBoneTransform(id, action, xyz[0], xyz[1], xyz[2]);
			break;

		default:
			MSTL_MSG("%s(%s, %s) not supported\n", 
					 __func__,
					 freyjaTransformToString(transform),
					 freyjaActionToString(action));
		}
		break;

	case fTransformVertex:
		switch (action)
		{
		case fTranslate:
			freyjaVertexPosition3fv(id, xyz);
			break;

		default:
			MSTL_MSG("%s(%s, %s) not supported\n", 
					 __func__,
					 freyjaTransformToString(transform),
					 freyjaActionToString(action));
		}
		break;

	default:
		MSTL_MSG("%s(%s, %s) not supported\n", 
				 __func__,
				 freyjaTransformToString(transform),
				 freyjaActionToString(action));
	}
}

// JA Version 1 file format


int32 freyjaCheckModel(const char *filename)
{
	SystemIO::FileReader r;
	freyja_file_header_t header;
	int32 offset;


	if (!r.Open(filename))
		return -1;

	/* Data is LITTLE_ENDIAN */
	r.SetByteOrder(SystemIO::File::LITTLE);

	/* Read header */
	offset = r.GetOffset();
	r.ReadString(16, header.magic);
	r.Close();

	if (!strncmp(header.magic, FREYJA_API_VERSION, 7)) // 'Freyja '
	{
		return 0;
	}

	return -1;
}


int32 freyjaLoadModel(const char *filename)
{
	SystemIO::FileReader r;
	freyja_file_header_t header;
	Vector<long> vertices, texcoords, bones;
	freyja_file_chunk_t chunk;
	int32 offset, i, j, index, flags;
	vec4_t wxyz;
	vec3_t xyz;
	char buffer[64];
	
	
	if (freyjaCheckModel((char *)filename) != 0)
		return -1;

	if (!r.Open(filename))
		return -1;

	/* Data is LITTLE_ENDIAN */
	r.SetByteOrder(SystemIO::File::LITTLE);

	/* Read header */
	offset = r.GetOffset();
	r.ReadString(8, header.magic);
	header.version = r.ReadLong();

	if (header.version != 1)
	{
		r.SetOffset(offset);
		r.ReadString(16, header.magic);
		header.version = r.ReadLong();
	}

	header.flags = r.ReadLong();
	header.reserved = r.ReadLong();
	r.ReadString(64, header.comment);	

	if (strncmp(header.magic, FREYJA_API_VERSION, 7)) // 'Freyja '
	{
		return -1;
	}

	while (!r.IsEndOfFile())
	{
		chunk.type = r.ReadLong();
		chunk.size = r.ReadLong();
		chunk.flags = r.ReadLong();
		chunk.version = r.ReadLong();
		offset = r.GetOffset() + chunk.size;

		switch (chunk.type)
		{
		case FREYJA_CHUNK_BONE:
			freyjaBegin(FREYJA_BONE);
			index = freyjaGetCurrent(FREYJA_BONE);
			bones.pushBack(index);

			memset(buffer, 0, 64);
			r.ReadString(64, buffer);
			freyjaBoneName(index, buffer);
			freyjaBoneFlags(index, 0x0);
			freyjaBoneParent(index, r.ReadLong());
			flags = r.ReadLong();
 
			for (j = 0; j < 3; ++j)
				xyz[j] = r.ReadFloat32();

			freyjaBoneTranslate3fv(index, xyz);
			
			if (flags & 32)
			{
				r.ReadLong();

				for (j = 0; j < 3; ++j)
					xyz[j] = r.ReadFloat32();

				freyjaBoneRotateEuler3fv(index, xyz);
			}
			else
			{
				for (j = 0; j < 4; ++j)
					wxyz[j] = r.ReadFloat32();

				freyjaBoneRotateQuat4fv(index, wxyz);
			}

			freyjaEnd(); // FREYJA_BONE

			if ((long)r.GetOffset() != offset)
				printf("BONE @ %li not %i!\n", r.GetOffset(), offset);
			break;


		/* Materials */
		case FREYJA_CHUNK_MATERIAL:
			{
				Material *mat;

				index = freyjaMaterialCreate();
				mat = freyjaGetMaterialClass(index);

				if (mat)
				{
					/* Read in new material */
					mat->serialize(r);

					// FIXME use string name matching like skeletalkeyframes
                    // to avoid dupe textures in the future

					if (SystemIO::File::DoesFileExist(mat->getTextureName()))
					{
						index_t textureIndex = freyjaTextureCreateFilename(mat->getTextureName());

						if (textureIndex != INDEX_INVALID)
						{
							mat->mTexture = textureIndex;
							mat->mFlags |= fFreyjaMaterial_Texture;
						}
					}
				}
			}
			break;


		case FREYJA_CHUNK_MESH:
			freyjaMeshLoadChunkJA(r, chunk);

			if ((long)r.GetOffset() != offset)
				printf("MESH @ %li not %i!\n", r.GetOffset(), offset);
			break;


		default:
			continue;
		}

		r.SetOffset(offset);
	}


	if (!bones.empty())
	{
		freyjaBegin(FREYJA_SKELETON);
		index_t skeleton = freyjaGetCurrent(FREYJA_SKELETON);

		for (i = bones.begin(); i < (long)bones.end(); ++i)
		{
			freyjaSkeletonAddBone(skeleton, bones[i]);

			for (j = bones.begin(); j < (long)bones.end(); ++j)
			{
				if (bones[i] == (long)freyjaGetBoneParent(bones[j]))
				{
					freyjaBoneAddChild(bones[i], bones[j]);
				}
			} 		
		} 

		freyjaEnd(); // FREYJA_SKELETON
	}

	r.Close();

	return 0;
}


#define QUAT_BACKEND 0
int32 freyjaSaveModel(const char *filename)
{
	Vector<long> vertices, texcoords;
	SystemIO::FileWriter w;
	freyja_file_header_t header;
	freyja_file_chunk_t chunk;
	vec3_t xyz;
	//vec4_t wxyz;
	char buffer[64];
	int32 i, j, index, idx, count;


	if (!w.Open(filename))
		return -1;

	memset(header.magic, 0, 16);
	memset(header.comment, 0, 64);
	strncpy(header.magic, FREYJA_API_VERSION, 12);
	header.version = 2;
	header.flags = 0x0;
	header.reserved = 0x0;
	strcpy(header.comment, "Freyja 3d: http://icculus.org/freyja");

	/* Data is LITTLE_ENDIAN */
	w.SetByteOrder(SystemIO::File::LITTLE);

	/* Write header */
	w.WriteString(16, header.magic);
	w.WriteLong(header.version);
	w.WriteLong(header.flags);
	w.WriteLong(header.reserved);
	w.WriteString(64, header.comment);


	/* Write chunks... */


	/* Bones */
	if (freyjaGetCount(FREYJA_BONE))
	{
		freyjaIterator(FREYJA_SKELETON, FREYJA_RESET);
		index = freyjaIterator(FREYJA_BONE, FREYJA_RESET);
		count = freyjaGetCount(FREYJA_BONE);

		for (i = 0; i < count; ++i)
		{
			memset(buffer, 0, 64);
			freyjaGetBoneName(index, 64, buffer);
			idx = freyjaGetBoneParent(index);

			chunk.type = FREYJA_CHUNK_BONE;
			chunk.size = 64 + 4 + 4 + 12 + 16;
			chunk.flags = 0x0;
			chunk.version = 1;

			w.WriteLong(chunk.type);
			w.WriteLong(chunk.size);
			w.WriteLong(chunk.flags);
			w.WriteLong(chunk.version);
			w.WriteString(64, buffer);
			w.WriteLong(idx);
#if QUAT_BACKEND
			w.WriteLong(0x0);
#else
			w.WriteLong(32); // Flag 32 - Using euler angles
#endif

			freyjaGetBoneTranslation3fv(index, xyz);

			for (j = 0; j < 3; ++j)
				w.WriteFloat32(xyz[j]);

#if QUAT_BACKEND
			freyjaGetBoneRotationQuat4fv(index, wxyz);
			
			for (j = 0; j < 4; ++j)
				w.WriteFloat32(wxyz[j]);
#else
			freyjaGetBoneRotationEuler3fv(index, xyz);
			w.WriteLong(0x0); // pad out
			for (j = 0; j < 3; ++j)
				w.WriteFloat32(xyz[j]);
#endif

			index = freyjaIterator(FREYJA_BONE, FREYJA_NEXT);
		}
	}


	/* Skeletons */


	/* Materials */
	if (freyjaGetMaterialCount())
	{
		count = freyjaGetMaterialCount();

		for (i = 0; i < count; ++i)
		{
			Material *mat = freyjaGetMaterialClass(i);

			if (mat)
			{
				chunk.type = FREYJA_CHUNK_MATERIAL;
				chunk.size = mat->getSerializeSize();
				chunk.flags = 0x0;
				chunk.version = Material::mVersion;

				w.WriteLong(chunk.type);
				w.WriteLong(chunk.size);
				w.WriteLong(chunk.flags);
				w.WriteLong(chunk.version);

				mat->serialize(w);
			}
		}		
	}


	/* Vertex groups */


	/* Meshes */
#if 0
	count = freyjaGetModelMeshCount();
	for (i = 0; i < count; ++i)
	{
		int meshIndex = freyjaGetModelMeshIndex(0, i);
		freyjaSaveMeshChunkV1(w, meshIndex);
	}
#else
	for (i = 0, count = freyjaGetMeshCount(); i < count; ++i)
	{
		freyjaPrintMessage("Writing mesh %i/%i...", i, count);
		freyjaMeshSaveChunkJA(w, i);
	}
#endif

	/* Metadata */


	/* That wasn't so bad, was it? */
	w.Close();

	return 0;
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

int32 freyjaFindPolygonByVertices(Vector<uint32> vertices)
{
#ifdef USING_EGG
	if (freyja__getEggBackend())
		return freyja__getEggBackend()->selectPolygon(&vertices);
#else
	BUG_ME("Not Implemented");
#endif

	return -1;
}




void freyjaBoneRemoveMesh(index_t boneIndex, index_t meshIndex)
{
	uint32 i, count;

	count = freyjaGetMeshVertexCount(meshIndex);

	for (i = 0; i < count; ++i)
	{
		//freyjaBoneRemoveVertex(boneIndex, i);//freyjaGetMeshVertexIndex(meshIndex, i));
	}
}


void freyjaBoneAddMesh(index_t boneIndex, index_t meshIndex)
{
	uint32 i, count;

	count = freyjaGetMeshVertexCount(meshIndex);

	for (i = 0; i < count; ++i)
	{
		//freyjaBoneAddVertex(boneIndex, i);//freyjaGetMeshVertexIndex(meshIndex, i));
	}
}


index_t freyjaGetBoneSkeletalBoneIndex(index_t boneIndex)
{
	return boneIndex; // Assumes single skeleton model, FIXME when Skeleton added to backend API
}


void freyjaBoneAddVertex(index_t boneIndex, index_t vertexIndex)
{
	freyjaVertexWeight(vertexIndex, 1.0f, boneIndex);
}



#endif // FREYJA_OBSOLETE_ABI
