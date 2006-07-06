/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : FreyjaMesh
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This is the FreyjaMesh and classes that compose it.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.10.22:
 * Mongoose - Created
 ==========================================================================*/

#include "FreyjaPluginABI.h"

#include "FreyjaMesh.h"

using namespace freyja;

index_t Mesh::mNextUID = 0;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Mesh::Mesh() :
	mUID(mNextUID++),
	mFlags(0),
	mMaterialIndex(0),
	mPosition(),
	mRotation(),  // Store as Euler Angles for 'Size' interface
	mScale(),
	mBoundingVolume(),
	mVertexPool(),
	mFreedVertices(),
	mNormalPool(),
	mFreedNormals(),
	mColorPool(),
	mFreedColors(),
	mTexCoordPool(),
	mFreedTexCoords(),
	mFaces(),
	mVertices(),
	mWeights()
{
	//mUID = mNextUID++;

	snprintf(mName, mNameSize-1, "Mesh%i", mUID);
	mName[mNameSize-1] = 0;
	
	mFlags = 0;
	mMaterialIndex = 0;
	mPosition = Vector3d(0.0f, 0.0f, 0.0f);
	mRotation = Vector3d(0.0f, 0.0f, 0.0f);
	mScale = Vector3d(1.0f, 1.0f, 1.0f);

	// FIXME: Define larger intial boundary? 
	//mBoundingVolume = ...;
}


Mesh::~Mesh()
{
	mFaces.erase();
	mVertices.erase();
	mWeights.erase();
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

void Mesh::GetSelectedVertices(Vector<index_t> &list) 
{
	BUG_ME("GetSelectedVertices Not Implemented", __FILE__, __LINE__);
}


bool Mesh::Serialize(FreyjaFileWriter &w)
{
	freyja_file_chunk_t chunk;

	chunk.type = FREYJA_CHUNK_MESH;
	chunk.size = 0;
	chunk.flags = 0x0;
	chunk.version = 10;

	/* Compute byte size here */
	chunk.size += 4;  // index_t mUID;
	chunk.size += 4;  // uint32 mFlags;
	chunk.size += 4;  // index_t mMaterialIndex;
	chunk.size += 12; // vec3_t mPosition;
	chunk.size += 12; // vec3_t mRotation;
	chunk.size += 12; // vec3_t mScale;
	chunk.size += 4 + mVertexPool.size() * 4;
	chunk.size += 4 + mFreedVertices.size() * 4;
	chunk.size += 4 + mNormalPool.size() * 4;
	chunk.size += 4 + mFreedNormals.size() * 4;
	chunk.size += 4 + mColorPool.size() * 4;
	chunk.size += 4 + mFreedColors.size() * 4;
	chunk.size += 4 + mTexCoordPool.size() * 4;
	chunk.size += 4 + mFreedTexCoords.size() * 4;
	chunk.size += 4 + mFaces.size() * Face::SerializedSize();
	chunk.size += 4 + mVertices.size() * Vertex::SerializedSize();
	chunk.size += 4 + mWeights.size() * Weight::SerializedSize();

	/* Write chunk header to diskfile */
	w.writeLong(FREYJA_CHUNK_MESH);
	w.writeLong(chunk.size);
	w.writeLong(chunk.flags);
	w.writeLong(chunk.version);

	/* Write chunk data to diskfile */
	w.writeLong(mUID);
	w.writeLong(mFlags);
	w.writeLong(mMaterialIndex);
	w.writeFloat32(mPosition.mVec[0]);
	w.writeFloat32(mPosition.mVec[1]);
	w.writeFloat32(mPosition.mVec[2]);
	w.writeFloat32(mRotation.mVec[0]);
	w.writeFloat32(mRotation.mVec[1]);
	w.writeFloat32(mRotation.mVec[2]);
	w.writeFloat32(mScale.mVec[0]);
	w.writeFloat32(mScale.mVec[1]);
	w.writeFloat32(mScale.mVec[2]);

	SerializePool(w, mVertexPool, mFreedVertices);
	SerializePool(w, mNormalPool, mFreedNormals);
	SerializePool(w, mColorPool, mFreedColors);
	SerializePool(w, mTexCoordPool, mFreedTexCoords);

	w.writeLong(mFaces.size());
	for ( uint32 i = 0; i < mFaces.size(); ++i )
	{
		if ( mFaces[i] ) 
			mFaces[i]->Serialize(w);
	}

	w.writeLong(mVertices.size());
	for ( uint32 i = 0; i < mVertices.size(); ++i )
	{
		if ( mVertices[i] ) 
			mVertices[i]->Serialize(w);
	}

	w.writeLong(mWeights.size());
	for ( uint32 i = 0; i < mWeights.size(); ++i )
	{
		if ( mWeights[i] ) 
			mWeights[i]->Serialize(w);
	}

	return true;
}


void Mesh::GetVertexArrayPos(index_t vertexIndex, vec3_t xyz)
{	
	if (vertexIndex < mVertexPool.size())
		GetTripleVec(mVertexPool, vertexIndex, xyz);	
}


void Mesh::GetVertexPos(index_t vertexIndex, vec3_t xyz)
{
	Vertex *vert = GetVertex(vertexIndex);

	if ( vert ) 
	{
		GetVertexArrayPos(vert->mVertexIndex, xyz);
	}
}

Vertex *Mesh::GetVertex(index_t vertexIndex)
{
	if (vertexIndex < mVertices.size())
		return mVertices[vertexIndex];

	return NULL;
}


Vector3d Mesh::GetVertexPosition(index_t idx)
{
	Vector3d v;
	Vertex *vert = mVertices[idx];
	if ( vert ) 
	{
		GetVertexArrayPos(vert->mVertexIndex, v.mVec);
	}

	return v;
}


Vector3d Mesh::GetVertexNormal(index_t idx)
{
	Vector3d v;
	Vertex *vert = mVertices[idx];
	if ( vert ) 
	{
		GetNormal(vert->mNormalIndex, v.mVec);
	}

	return v;
}


Vector3d Mesh::GetVertexTexCoord(index_t idx)
{
	Vector3d v;
	Vertex *vert = mVertices[idx];
	if ( vert ) 
	{
		GetTexCoord(vert->mTexCoordIndex, v.mVec);
	}

	return v;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void Mesh::Rotate(vec_t x, vec_t y, vec_t z)
{
	BUG_ME("Not Implemented", __FILE__, __LINE__);
}


void Mesh::Scale(vec_t x, vec_t y, vec_t z)
{
	BUG_ME("Not Implemented", __FILE__, __LINE__);
}


void Mesh::Translate(vec_t x, vec_t y, vec_t z)
{
	vec3_t v = {x, y, z};
	TripleVec_Addition(mVertexPool, v);
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////

bool Mesh::SerializePool(FreyjaFileWriter &w, 
						 Vector<vec_t> &v, mstl::stack<index_t> &s)
{
	mstl::stack<index_t> copy; // We don't really care about order

	w.writeLong(v.size());

	for ( uint32 i = 0; i < v.size(); ++i )
	{
		w.writeFloat32(v[i]);
	}
		
	w.writeLong(s.size());

	for ( uint32 i = 0; i < s.size(); ++i )
	{
		index_t item = s.pop();
		copy.push(item);
		w.writeLong(item);
	}

	while ( !copy.empty() )
	{
		s.push(copy.pop());
	}

	return true;
}


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////
// C ABI code
////////////////////////////////////////////////////////////

// FIXME: Currently not using Model, since we're just 
// going to support 0.9.3 API in interface, plugins, etc
// until 0.10 release 
Vector<Mesh *> gFreyjaMeshes;
index_t gFreyjaCurrentModel = 0;
index_t gFreyjaCurrentMesh = INDEX_INVALID;


////////////////////////////////////
// 0.9.5 ABI
////////////////////////////////////

// FIXME: No model usage
index_t freyjaModelCreateMesh(index_t modelIndex)
{
	Mesh **array = gFreyjaMeshes.getVectorArray();
	Mesh *mesh = new Mesh();
	index_t idx = gFreyjaMeshes.size();


	for ( uint32 i = 0; i < idx; ++i )
	{
		if ( array[i] == NULL )
		{
			array[i] = mesh;
			freyjaPrintMessage("Model[%i].Mesh[%i] created.", modelIndex, i );
			return i;
		}
	}

	freyjaPrintMessage("Model[%i].Mesh[%i] created", modelIndex, idx );
	gFreyjaMeshes.pushBack(mesh);
	return idx;
}


// FIXME: No model usage
void freyjaModelDeleteMesh(index_t modelIndex, index_t meshIndex)
{
	Mesh **array = gFreyjaMeshes.getVectorArray();

	if ( meshIndex < gFreyjaMeshes.size() && array[meshIndex] != NULL )
	{
		delete array[meshIndex];
		array[meshIndex] = NULL;
		freyjaPrintMessage("Model[%i].Mesh[%i] deleted.", 
						   modelIndex, meshIndex );
	}
}


// FIXME: No model usage`
Mesh *freyjaModelGetMeshClass(index_t modelIndex, index_t meshIndex)
{
	Mesh **array = gFreyjaMeshes.getVectorArray();

	//ASSERT_MSG( meshIndex < gFreyjaMeshes.size(), "meshIndex [%i] < gFreyjaMeshes.size() [%i]", meshIndex, gFreyjaMeshes.size() );

	if ( meshIndex < gFreyjaMeshes.size() && array[meshIndex] != NULL )
	{
		return array[meshIndex];
	}

	return NULL;
}


Mesh *freyjaGetCurrentMeshClass()
{
	return freyjaModelGetMeshClass(gFreyjaCurrentModel, gFreyjaCurrentMesh);
}


index_t freyjaModelMeshVertexCreate(index_t modelIndex, index_t meshIndex,
									vec3_t xyz, vec3_t uvw, vec3_t nxyz)
{
	Mesh *mesh = freyjaModelGetMeshClass(modelIndex, meshIndex);

	if ( mesh != NULL )
	{
		index_t vertex = mesh->CreateVertex(xyz, uvw, nxyz);
		freyjaPrintMessage("Model[%i].Mesh[%i].Vertex[%i] created.",
						   modelIndex, meshIndex, vertex);

		// Debug Check
		//freyjaPrintMessage("-> %f %f %f", xyz[0], xyz[0], xyz[2]);
		//mesh->GetVertexPos(vertex, xyz);		
		//freyjaPrintMessage("<- %f %f %f", xyz[0], xyz[0], xyz[2]);
		return vertex;
	}

	return INDEX_INVALID;
}

index_t freyjaModelMeshPolygonCreate(index_t modelIndex, index_t meshIndex)
{
	Mesh *mesh = freyjaModelGetMeshClass(modelIndex, meshIndex);

	if ( mesh != NULL )
	{
		index_t polygon = mesh->CreateFace();
		freyjaPrintMessage("Model[%i].Mesh[%i].Polygon[%i] created.",
						   modelIndex, meshIndex, polygon);
		return polygon;
	}

	return INDEX_INVALID;
}


void freyjaModelMeshPolygonAddTexCoord1i(index_t modelIndex, index_t meshIndex, 
										 index_t polygonIndex, index_t texcoordIndex)
{
	DEBUG_MSG("freyjaModelMeshPolygonAddTexCoord1i, Not Implmented.");	
}


void freyjaModelMeshPolygonAddVertex1i(index_t modelIndex, index_t meshIndex, 
									   index_t polygonIndex, index_t vertexIndex)
{
	Mesh *mesh = freyjaModelGetMeshClass(modelIndex, meshIndex);

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
					DEBUG_MSGF("freyjaModelMeshPolygonAddVertex1i, Tried to insert duplicate vertex into face[%i].  %i  %i", polygonIndex, face->mIndices[i], vertexIndex);
					return;
				}
			}

			face->mIndices.pushBack(vertexIndex);
		}
	}
}


////////////////////////////////////
// 0.9.3 <- 0.9.5 ABI
////////////////////////////////////

char freyjaIsVertexAllocated(index_t vertexIndex)
{
	Mesh *mesh = freyjaModelGetMeshClass(gFreyjaCurrentModel, gFreyjaCurrentMesh);

	if (mesh)
		return (mesh->GetVertex(vertexIndex) != NULL);

	return 0;
}


index_t freyjaGetCurrentMesh()
{
	return gFreyjaCurrentMesh;
}


index_t freyjaMeshCreate()
{
	return freyjaModelCreateMesh(gFreyjaCurrentModel);
}


void freyjaMeshDelete(index_t meshIndex)
{
	return freyjaModelDeleteMesh(gFreyjaCurrentModel, meshIndex);
}


void freyjaMeshTransform(index_t meshIndex, uint32 frame,
						 freyja_transform_action_t action, 
						 vec_t x, vec_t y, vec_t z)
{
	Mesh *mesh = freyjaModelGetMeshClass(gFreyjaCurrentModel, meshIndex);

	if (mesh == NULL)
	{
		DEBUG_MSGF("Tried to transform invalid mesh[%i]", meshIndex);
		return;
	}

	switch (action)
	{
	case fTranslate:
		mesh->Translate(x, y, z);
		break;

#if 0
	case fRotate:
		break;

	case fScale:
		break;

	case fScaleAboutPoint:
		break;

	case fRotateAboutPoint:
		break;
#endif

	default:
		BUG_ME("freyjaMeshTransform Not Implemented", __FILE__, __LINE__);
	}
}


void freyjaMeshFrameTransform(index_t meshIndex, uint32 frame,
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

	BUG_ME("freyjaMeshFrameTransform Not Implemented", __FILE__, __LINE__);
}


void freyjaMeshAddPolygon(index_t meshIndex, index_t polygonIndex)
{
	BUG_ME("freyjaMeshAddPolygon Not Implemented", __FILE__, __LINE__);
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

	BUG_ME("freyjaModelTransform Not Implemented", __FILE__, __LINE__);
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

	BUG_ME("freyjaVertexListTransform Not Implemented", __FILE__, __LINE__);
}


void freyjaGetVertexPolygonRef1i(index_t vertexIndex, Vector<long> &polygons)
{
	BUG_ME("freyjaGetVertexPolygonRef1i Not Implemented", __FILE__, __LINE__);
}


void freyjaGetVertexPolygonRef(Vector<long> &polygons)
{
	BUG_ME("freyjaGetVertexPolygonRef Not Implemented", __FILE__, __LINE__);
}



index_t freyjaTexCoordCreate2f(vec_t u, vec_t v)
{
	BUG_ME("freyjaTexCoordCreate2f Not Implemented", __FILE__, __LINE__);
	return INDEX_INVALID;
}


index_t freyjaTexCoordCreate2fv(const vec2_t uv)
{
	return freyjaTexCoordCreate2f(uv[0], uv[1]);
}


void freyjaVertexWeight(index_t vertexIndex, vec_t weight, index_t bone)
{
	Mesh *mesh = freyjaGetCurrentMeshClass();

	if ( mesh != NULL )
	{
		mesh->AddWeight(vertexIndex, weight, bone);
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


void freyjaVertexDelete(index_t vertexIndex)
{	
	Mesh *mesh = freyjaGetCurrentMeshClass();

	if ( mesh != NULL )
	{
		mesh->DeleteVertex(vertexIndex);
	}
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


void freyjaVertexFrame3f(index_t index, vec_t x, vec_t y, vec_t z)
{
	// FIXME: Should use keyframes
	BUG_ME("freyjaVertexFrame3f Not Implemented", __FILE__, __LINE__);
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



void freyja__PolygonReplaceReference(index_t polygonIndex, 
									 index_t vertexA, index_t vertexB)
{
	BUG_ME("freyja__PolygonReplaceReference Not Implemented", __FILE__, __LINE__);
}



void freyja__GetCommonPolygonReferences(index_t vertexA, index_t vertexB,
										Vector<unsigned int> &common)
{
	BUG_ME("freyja__GetCommonPolygonReferences Not Implemented", __FILE__, __LINE__);
}


void freyja__GetDifferenceOfPolygonReferences(index_t vertexA, index_t vertexB,
											  Vector<unsigned int> &diffA)
{
	BUG_ME("freyja__GetDifferenceOfPolygonReferences Not Implemented", __FILE__, __LINE__);
}


void freyjaMeshPolygonSplitTexCoords(index_t meshIndex, index_t polygonIndex)
{	
	BUG_ME("freyja__GetDifferenceOfPolygonReferences Not Implemented", __FILE__, __LINE__);
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


void freyjaPolygonDelete(index_t polygonIndex)
{
	BUG_ME("freyjaPolygonDelete Not Implemented", __FILE__, __LINE__);
}


void freyjaPolygonFlagAlpha(index_t polygonIndex, char on)
{
	BUG_ME("freyjaPolygonFlagAlpha Not Implemented", __FILE__, __LINE__);
}


void freyjaPolygonSplit(index_t meshIndex, index_t polygonIndex)
{
	BUG_ME("freyjaPolygonSplit Not Implemented", __FILE__, __LINE__);
}


void freyjaPolygonExtrudeQuad1f(index_t polygonIndex, vec_t dist)
{
	Vector3d faceNormal, a, b, c;
	int32 i, v, count;

	if (freyjaIterator(FREYJA_POLYGON, polygonIndex) == INDEX_INVALID)
		return;

	// 1. Get face normal
	faceNormal.zero();

	/* Generated face normal, which won't match preclac vertex normalsx */
	//freyjaGetPolygon1u(FREYJA_VERTEX, 0, &v1);
	//freyjaGetPolygon1u(FREYJA_VERTEX, 1, &v1);
	//freyjaGetPolygon1u(FREYJA_VERTEX, 2, &v2);
	//freyjaGetVertexXYZ3fv(v0, a.mVec);
	//freyjaGetVertexXYZ3fv(v1, b.mVec);
	//freyjaGetVertexXYZ3fv(v2, c.mVec);
	//faceNormal = Vector3d::cross(a - b, c - b);
	
	/* Use existing vertex normals ( perfered, also like n-patch ) */
	count = freyjaGetPolygonVertexCount(polygonIndex);
	for (i = 0; i < count; ++i)
	{
		v = freyjaGetPolygonVertexIndex(polygonIndex, i);
		freyjaGetVertexNormalXYZ3fv(v, a.mVec);
		faceNormal += a;
	}


	// 2. Scale face normal by dist
	faceNormal.normalize();
	faceNormal *= dist;

	freyjaPolygonExtrudeQuad(polygonIndex, faceNormal.mVec);
}


int32 freyjaVertexXYZ3fv(index_t vertexIndex, vec3_t xyz)
{
	Mesh *mesh = freyjaGetCurrentMeshClass();

	if ( mesh != NULL )
	{
		mesh->GetVertexArrayPos(vertexIndex, xyz);

		BUG_ME("freyjaVertexXYZ3fv Not fully Reimplemented", __FILE__, __LINE__);
		//if (vertexexists) return 0;
	}

	return -1;
}


void freyjaPolygonExtrudeQuad(index_t polygonIndex, vec3_t normal)
{
	Vector<unsigned int> common, face;
	Vector<long> ref;
	vec3_t xyz;
	vec2_t uv;
	unsigned int j;
	int32 A, B, C, D, i, material, count;


	material = freyjaGetPolygonMaterial(polygonIndex);
	count = freyjaGetPolygonVertexCount(polygonIndex);

	if (!count)
		return;

	for (i = 0; i < count; ++i)
	{
		/* 1. Make duplicate vertices with same wind for 'face' */
		A = freyjaGetPolygonVertexIndex(polygonIndex, i);
		freyjaGetVertexXYZ3fv(A, xyz);
		B = freyjaVertexCreate3fv(xyz);
		freyjaGetVertexTexcoord2fv(A, uv);
		freyjaVertexTexCoord2fv(B, uv);
		freyjaGetVertexNormalXYZ3fv(A, xyz);
		freyjaVertexNormal3fv(B, xyz);
		
		face.pushBack(B);


		/* 2. Replace all references to A with B ( dupe of A ), 
		 * except polygonIndex */
		freyjaGetVertexPolygonRef1i(A, ref);

		for (j = ref.begin(); j < ref.end(); ++j)
		{
			if (ref[j] != (int32)polygonIndex)
			{
				freyja__PolygonReplaceReference(ref[j], A, B);
			}
		}
	}

	for (i = 0; i < count; ++i)
	{
		// 3. Generate new quad ABCD connecting 'face' and ploygonIndex vertices
		A = freyjaGetPolygonVertexIndex(polygonIndex, i);
		B = face[i];

		if (i+1 < count)
		{
			C = freyjaGetPolygonVertexIndex(polygonIndex, i+1);
			D = face[i+1];
		}
		else
		{
			C = freyjaGetPolygonVertexIndex(polygonIndex, 0);
			D = face[0];
		}

		freyjaBegin(FREYJA_POLYGON);
		freyjaPolygonMaterial1i(material);
		freyjaPolygonVertex1i(A);
		freyjaPolygonVertex1i(C);
		freyjaPolygonVertex1i(D);
		freyjaPolygonVertex1i(B);

		// FIXME: Should be able to generate mixing both uvs
		if (freyjaGetPolygonTexCoordCount(polygonIndex))
		{
			freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(0.25, 0.25));
			freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(0.5, 0.25));
			freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(0.5, 0.5));
			freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(0.25, 0.5));
		}

		freyjaEnd();
	}

	// FIXME: Should recompute normals for 'face' vertices here


	// Move polygonIndex vertices by 'normal'
	for (i = 0; i < count; ++i)
	{
		A = freyjaGetPolygonVertexIndex(polygonIndex, i);
		freyjaGetVertexXYZ3fv(A, xyz);
		xyz[0] += normal[0];
		xyz[1] += normal[1];
		xyz[2] += normal[2];
		freyjaVertexXYZ3fv(A, xyz);
	}
}


void freyjaPolygonAddVertex1i(index_t polygonIndex, index_t vertexIndex)
{
	BUG_ME("freyjaPolygonAddVertex1i Not Implemented", __FILE__, __LINE__);
}


void freyjaTexCoordCombine(uint32 indexA, uint32 indexB)
{
	Mesh *mesh = freyjaGetCurrentMeshClass();

	if ( mesh != NULL )
	{
		mesh->WeldTexCoords(indexA, indexB);
	}	
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


void freyjaPolygonAddTexCoord1i(index_t polygonIndex, index_t texcoordIndex)
{
	BUG_ME("freyjaPolygonAddTexCoord1i Not Implemented", __FILE__, __LINE__);
}


void freyjaPolygonSetMaterial1i(index_t polygonIndex, index_t materialIndex)
{
	BUG_ME("freyjaPolygonSetMaterial1i Not Implemented", __FILE__, __LINE__);
}


const char *freyjaGetMeshNameString(index_t meshIndex)
{
	Mesh *mesh = freyjaModelGetMeshClass(gFreyjaCurrentModel, meshIndex);

	if (mesh)
	{
		return mesh->GetName();
	}

	return 0x0;
}


void freyjaGetMeshName1s(index_t meshIndex, int32 lenght, char *name)
{
	Mesh *mesh = freyjaModelGetMeshClass(gFreyjaCurrentModel, meshIndex);

	if (mesh)
	{
		strncpy(name, mesh->GetName(), (lenght < 32) ? lenght : 32);
		name[lenght-1] = 0;
	}
}


void freyjaPolygonTexCoordPurge(index_t polygonIndex)
{
	BUG_ME("freyjaPolygonTexCoordPurge Not Implemented", __FILE__, __LINE__);
}


void freyjaGetMeshFrameCenter(index_t meshIndex, uint32 frame, vec3_t xyz)
{
	Mesh *mesh = freyjaModelGetMeshClass(gFreyjaCurrentModel, meshIndex);

	if (mesh)
	{
		mesh->GetPosition(xyz);
	}
}


void freyjaMeshMaterial(index_t meshIndex, uint32 materialIndex)
{
	Mesh *mesh = freyjaModelGetMeshClass(gFreyjaCurrentModel, meshIndex);

	if (mesh)
	{
		mesh->SetMaterial(materialIndex);
	}
}


void freyjaMeshFrameCenter(index_t meshIndex, uint32 frame, vec3_t xyz)
{
	Mesh *mesh = freyjaModelGetMeshClass(gFreyjaCurrentModel, meshIndex);

	if (mesh)
	{
		mesh->SetPosition(xyz);
	}
}


void freyjaGetMeshFrameBoundingBox(index_t meshIndex, uint32 frame, 
									vec3_t min, vec3_t max)
{
	Mesh *mesh = freyjaModelGetMeshClass(gFreyjaCurrentModel, meshIndex);

	if (mesh)
	{
		mesh->SetBBox(min, max);
	}
}



void freyjaMeshName1s(index_t meshIndex, const char *name)
{
	Mesh *mesh = freyjaModelGetMeshClass(gFreyjaCurrentModel, meshIndex);

	if (mesh)
	{
		mesh->SetName(name);
	}
}


void freyjaMeshPosition(index_t meshIndex, vec3_t xyz)
{
	Mesh *mesh = freyjaModelGetMeshClass(gFreyjaCurrentModel, meshIndex);

	if (mesh)
	{
		mesh->SetPosition(xyz);
	}
}


void freyjaMeshRemovePolygon(index_t meshIndex, index_t polygonIndex)
{
	BUG_ME("freyjaMeshRemovePolygon Not Implemented", __FILE__, __LINE__);
}


void freyjaBoneRemoveVertex(index_t boneIndex, index_t vertexIndex)
{
	BUG_ME("freyjaBoneRemoveVertex Not Implemented", __FILE__, __LINE__);
}


index_t freyjaGetTexCoordPolygonRefIndex(index_t texcoordIndex, uint32 element)
{
	BUG_ME("freyjaGetTexCoordPolygonRefIndex Not Implemented", __FILE__, __LINE__);
	return INDEX_INVALID;
}


uint32 freyjaGetTexCoordPolygonRefCount(index_t texcoordIndex)
{
	BUG_ME("freyjaGetTexCoordPolygonRefCount Not Implemented", __FILE__, __LINE__);
	return 0;
}


void freyjaTexCoord2fv(index_t texcoordIndex, const vec2_t uv)
{
	BUG_ME("freyjaTexCoord2fv Not Implemented", __FILE__, __LINE__);
}


void freyjaGetTexCoord2fv(index_t tindex, vec2_t uv)
{
	BUG_ME("freyjaGetTexCoord2fv Not Implemented", __FILE__, __LINE__);
}


vec3_t *freyjaGetVertexXYZ(index_t vertexIndex)
{
	//BUG_ME("freyjaGetVertexXYZ Not Implemented", __FILE__, __LINE__);
	Mesh *mesh = freyjaModelGetMeshClass(gFreyjaCurrentModel, gFreyjaCurrentMesh);

	if (mesh && vertexIndex < mesh->GetVertexCount())
	{
		return (vec3_t *)(mesh->GetVertexArray() + vertexIndex * 3);
	}

	return NULL;
}


vec2_t *freyjaGetVertexUV(index_t vertexIndex)
{
	BUG_ME("freyjaGetVertexUV Not Implemented", __FILE__, __LINE__);
	return NULL;
}


vec2_t *freyjaGetTexCoordUV(index_t texcoordIndex)
{
	BUG_ME("freyjaGetTexCoordUV Not Implemented", __FILE__, __LINE__);
	return NULL;
}


index_t freyjaGetVertexPolygonRefIndex(index_t vertexIndex, uint32 element)
{
	BUG_ME("freyjaGetVertexPolygonRefIndex Not Implemented", __FILE__, __LINE__);

	return INDEX_INVALID;
}


uint32 freyjaGetVertexPolygonRefCount(index_t vertexIndex)
{
	BUG_ME("freyjaGetVertexPolygonRefCount Not Implemented", __FILE__, __LINE__);

	return 0;
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


uint32 freyjaGetPolygonVertexCount(index_t polygonIndex)
{
	BUG_ME("freyjaGetPolygonVertexCount Not Implemented", __FILE__, __LINE__);
	return 0;
}


uint32 freyjaGetPolygonTexCoordCount(index_t polygonIndex)
{
	BUG_ME("freyjaGetPolygonTexCoordCount Not Implemented", __FILE__, __LINE__);
	return 0;
}


void freyjaGetVertexTexcoord2fv(index_t vertexIndex, vec2_t uv)
{
	BUG_ME("freyjaGetVertexTexcoord2fv Not Implemented", __FILE__, __LINE__);
}


void freyjaGetVertexNormalXYZ3fv(index_t vertexIndex, vec3_t nxyz)
{
	BUG_ME("freyjaGetVertexNormalXYZ3fv Not Implemented", __FILE__, __LINE__);
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


void freyjaGetVertexFrame(index_t vertexIndex, uint32 element,
						  index_t *frameIndex, vec3_t xyz)
{
	BUG_ME("freyjaGetVertexFrame Not Implemented", __FILE__, __LINE__);
}


uint32 freyjaGetVertexFrameCount(index_t vertexIndex)
{
	BUG_ME("freyjaGetVertexFrameCount Not Implemented", __FILE__, __LINE__);
	return 0;
}


void freyjaGetVertexWeight(index_t vertexIndex, uint32 element,
						   index_t *bone, vec_t *weight)
{
	BUG_ME("freyjaGetVertexWeight Not Implemented", __FILE__, __LINE__);
}


index_t freyjaGetVertexWeightCount(index_t vertexIndex)
{
	BUG_ME("freyjaGetVertexWeightCount Not Implemented", __FILE__, __LINE__);
	return 0;
}


index_t freyjaGetVertexFlags(index_t vertexIndex)
{
	BUG_ME("freyjaGetVertexFlags Not Implemented", __FILE__, __LINE__);
	return 0;
}


index_t freyjaGetModelFlags(index_t modelIndex)
{
	BUG_ME("freyjaGetModelFlags Not Implemented", __FILE__, __LINE__);
	return 0x0;
}


index_t freyjaGetModelMeshIndex(index_t modelIndex, uint32 element)
{
	BUG_ME("freyjaGetModelMeshIndex Not Implemented", __FILE__, __LINE__);
	return 0;
}


index_t freyjaGetModelMeshCount(index_t modelIndex)
{
	// FIXME: Not using model
	return gFreyjaMeshes.size();
}


char freyjaIsMeshAllocated(index_t meshIndex)
{
	return (freyjaModelGetMeshClass(gFreyjaCurrentModel, meshIndex) != NULL);
}


void freyjaGetMeshPosition(index_t meshIndex, vec3_t xyz)
{
	BUG_ME("freyjaGetMeshPosition Not Implemented", __FILE__, __LINE__);
}


uint32 freyjaGetMeshFlags(index_t meshIndex)
{
	BUG_ME("freyjaGetMeshFlags Not Implemented", __FILE__, __LINE__);
	return 0x0;
}


index_t freyjaGetMeshVertexFrameIndex(index_t meshIndex, uint32 element)
{
	BUG_ME("freyjaGetMeshVertexFrameIndex Not Implemented", __FILE__, __LINE__);
	return 0;//INDEX_INVALID;
}


uint32 freyjaGetMeshPolygonCount(index_t meshIndex)
{
	Mesh *mesh = freyjaModelGetMeshClass( gFreyjaCurrentModel, meshIndex );

	if (mesh)
	{
		return mesh->GetFaceCount();
	}

	return 0;
}


index_t freyjaGetMeshPolygonVertexIndex(index_t meshIndex, index_t faceVertexIndex)
{
	BUG_ME("freyjaGetMeshPolygonVertexIndex Not Implemented", __FILE__, __LINE__);
	return INDEX_INVALID;
}


index_t freyjaGetMeshVertexIndex(index_t meshIndex, uint32 element)
{
	BUG_ME("freyjaGetMeshVertexIndex Not Implemented", __FILE__, __LINE__);
	return INDEX_INVALID;
}


uint32 freyjaGetMeshVertexCount(index_t meshIndex)
{
	Mesh *mesh = freyjaModelGetMeshClass( gFreyjaCurrentModel, meshIndex );

	if (mesh)
	{
		return mesh->GetVertexCount();
	}

	return 0;
}


index_t freyjaGetMeshTexCoordIndex(index_t meshIndex, uint32 element)
{
	BUG_ME("freyjaGetMeshTexCoordIndex Not Implemented", __FILE__, __LINE__);
	return INDEX_INVALID;
}


uint32 freyjaGetMeshTexCoordCount(index_t meshIndex)
{
	Mesh *mesh = freyjaModelGetMeshClass( gFreyjaCurrentModel, meshIndex );

	if (mesh)
	{
		return mesh->GetTexCoordCount();
	}

	return 0;
}


void freyjaMeshVertexGroupAppendGobalVertex(index_t meshIndex, uint32 element, 
											index_t vertexIndex)
{
	BUG_ME("freyjaMeshVertexGroupAppendGobalVertex Not Implemented", __FILE__, __LINE__);
}


uint32 freyjaGetMeshVertexGroupVertexCount(index_t meshIndex, uint32 element)
{
	Mesh *mesh = freyjaModelGetMeshClass(gFreyjaCurrentModel, meshIndex);

	if (mesh == NULL)
		return 0;

	return mesh->GetVertexCount();
}


// NOTE: Only 1 'vertex group' per mesh in 0.9.5
index_t freyjaGetMeshVertexGroupVertexIndex(index_t meshIndex, uint32 element,
										  uint32 vertexElement)
{
	//FIXME("freyjaGetMeshVertexGroupVertexIndex Implementation might need artifical gobal mapping to retian 0.9.3 function", __FILE__, __LINE__);
	Mesh *mesh = freyjaModelGetMeshClass(gFreyjaCurrentModel, meshIndex);

	if ( !mesh || vertexElement > mesh->GetVertexCount() || element > 0 )
		return INDEX_INVALID;

	return element;
}


// NOTE: Only 1 'vertex group' per mesh in 0.9.5
uint32 freyjaGetMeshVertexGroupCount(index_t meshIndex)
{
	Mesh *mesh = freyjaModelGetMeshClass(gFreyjaCurrentModel, meshIndex);

	if (mesh && mesh->GetVertexCount()) 
		return 1;

	return 0;
}


index_t freyjaGetMeshPolygonIndex(index_t meshIndex, uint32 element)
{
	Mesh *mesh = freyjaModelGetMeshClass(gFreyjaCurrentModel, meshIndex);

	if (!mesh || element > mesh->GetFaceCount())
		return INDEX_INVALID;

	return element;
}


index_t freyjaGetMeshVertexGroupIndex(index_t meshIndex, uint32 element)
{
	//BUG_ME("freyjaGetMeshVertexGroupIndex Not Implemented", __FILE__, __LINE__);

	if (element > 0)
		return INDEX_INVALID;

	return 0;
}


index_t freyjaGetPolygonVertexIndex(index_t polygonIndex, uint32 element)
{
	BUG_ME("freyjaGetPolygonVertexIndex Not Implemented", __FILE__, __LINE__);
	return 0;
}


index_t freyjaGetPolygonTexCoordIndex(index_t polygonIndex, uint32 element)
{
	BUG_ME("freyjaGetPolygonTexCoordIndex Not Implemented", __FILE__, __LINE__);
	return 0;
}


index_t freyjaGetPolygonMaterial(index_t polygonIndex)
{
	BUG_ME("freyjaGetPolygonMaterial Not Implemented", __FILE__, __LINE__);
	return 0;
}


index_t freyjaGetPolygonFlags(index_t polygonIndex)
{
	BUG_ME("freyjaGetPolygonFlags Not Implemented", __FILE__, __LINE__);
	return 0;
}


uint32 freyjaGetPolygonEdgeCount(index_t polygonIndex)
{
	BUG_ME("freyjaGetPolygonEdgeCount Not Implemented", __FILE__, __LINE__);
	return 0;
}


void freyjaModelMirrorTexCoord(index_t modelIndex, index_t texcoordIndex,
								Vector<int32> uvMap, bool x, bool y)
{
	BUG_ME("freyjaModelMirrorTexCoord Not Implemented", __FILE__, __LINE__);
}


void freyjaModelTransformTexCoord(index_t modelIndex, index_t texcoordIndex,
									freyja_transform_action_t action,
									vec_t x, vec_t y)
{
	BUG_ME("freyjaModelTransformTexCoord Not Implemented", __FILE__, __LINE__);
}


void freyjaModelClear(index_t modelIndex)
{
	BUG_ME("freyjaModelClear Not fully Implemented", __FILE__, __LINE__);

	// Currently there is no multimodel while egg is being used
	freyjaSkeletonPoolClear();
	freyjaBonePoolClear();
}


void freyjaModelClampTexCoords(index_t modelIndex)
{
	BUG_ME("freyjaModelClampTexCoords Not fully Implemented", __FILE__, __LINE__);
	uint32 i, count = 0;//freyjaModelGetMeshCount(modelIndex);

	for ( i = 0; i < count; ++i )
	{
		// FIXME: GetNext? ForEach?
		Mesh *mesh = freyjaModelGetMeshClass(modelIndex, i);

		if (mesh != NULL)
			mesh->ClampAllTexCoords();
	}
}


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_FREYJAMESH
int runFreyjaMeshUnitTest(int argc, char *argv[])
{
	FreyjaMesh test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[FreyjaMesh class test]\n");

	return runFreyjaMeshUnitTest(argc, argv);
}
#endif
