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

#include "Mesh.h"

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
	BUG_ME("Not Implemented");
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


#if 0
void Mesh::GetVertexPos(index_t vertexIndex, vec3_t xyz)
{
	Vertex *vert = GetVertex(vertexIndex);

	if ( vert ) 
	{
		GetVertexArrayPos(vert->mVertexIndex, xyz);
	}
}
#endif


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


bool Mesh::IntersectFaces(Ray &r, int &face0, bool markAll)
{
	vec_t bestDist = 99999.0f;
	r.mDir.normalize();
	face0 = -1;
	
	for (uint32 i = 0, iCount = GetFaceCount(); i < iCount; ++i)
	{
		Face *f = GetFace(i);
		
		if (!f) 
			continue;
	
		// Quick and dirty hit test that pretends you can 
		// pusedo tesselate a face and get as good results
		bool intersect = false;
					
		if (f->mIndices.size() > 2)
		{
			uint32 jCount = f->mIndices.size();
			Vec3 a, b, c, tuv;

			GetVertexPos(f->mIndices[0], a.mVec);
			GetVertexPos(f->mIndices[1], b.mVec);


			for (uint32 j = 2; j < jCount; ++j)
			{
				GetVertexPos(f->mIndices[j], c.mVec);
				intersect = r.IntersectTriangle(a.mVec, b.mVec, c.mVec, tuv.mVec);

				if (intersect)
				{
					break;
				}
			
				a = b;
				b = c;
			}
		
			// Clear old hit flags
			f->mFlags |= Face::fRayHit;
			f->mFlags ^= Face::fRayHit;

			if (intersect)
			{
				vec_t dist = tuv.mVec[0];

				if (face0 == -1 || dist < bestDist)
				{
					freyjaPrintMessage("--- t = %f, uv = %f, %f", tuv.mVec[0],tuv.mVec[1],tuv.mVec[2]);
					bestDist = dist;
					face0 = i;
				}

				// Only mark hit flags for every face hit with markAll==true
				if (markAll) f->mFlags |= Face::fRayHit;
			}
		}
	}

	// Did we have a 'best hit'?
	if (face0 > -1)
	{
		Face *f = GetFace(face0);
		
		if (f) 
		{
			f->mFlags |= Face::fRayHit;
		}
	}

	return (face0 > -1);
}


bool Mesh::IntersectClosestVertex(Ray &r, int &vertex0, vec_t radius)
{
	Vec3 center, normal;
	vec_t t, bestDist = 99999.0f;
	r.mDir.normalize();
	vertex0 = -1;
	
	for (uint32 i = 0, iCount = GetVertexCount(); i < iCount; ++i)
	{
		Vertex *v = GetVertex(i);

		if (!v) 
			continue;

		// Clear old ray hit results
		v->mFlags |= Vertex::fRayHit;
		v->mFlags ^= Vertex::fRayHit;

		GetVertexArrayPos(v->mVertexIndex, center.mVec);
		
		bool intersect = r.IntersectSphere(center.mVec, radius, t);
			
		freyjaPrintMessage("--- t = %f", t);
	
		if (intersect)
		{
			if (vertex0 == -1 || t < bestDist)
			{
				bestDist = t;
				vertex0 = i;
			}
		}
	}

	// Did we have a 'best hit'?
	if (vertex0 > -1)
	{
		Vertex *v = GetVertex(vertex0);
		
		if (v) 
		{
			v->mFlags |= Vertex::fRayHit;
		}
	}

	return (vertex0 > -1);
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void Mesh::ExtrudeFace(index_t faceIndex, vec3_t displacement)
{
	Face *face = GetFace(faceIndex);

	if (!face || face->mIndices.size() == 0)
		return;

	Vector<unsigned int> common, faceWind;
	Vector<long> ref;
	vec3_t xyz;
	Vec3 uv, nxyz;
	int32 A, B, C, D;


	for (uint32 i = 0, count = face->mIndices.size(); i < count; ++i)
	{
		/* 1. Make duplicate vertices with same wind for 'face' */
		A = face->mIndices[i];
		GetVertexPos(A, xyz);
		uv = GetVertexTexCoord(A);
		nxyz = GetVertexNormal(A);

		// Push vertices along 'displacement'
		xyz[0] += displacement[0];
		xyz[1] += displacement[1];
		xyz[2] += displacement[2];

		B = CreateVertex(xyz, uv.mVec, nxyz.mVec);
		faceWind.pushBack(A);

		/* 2. Replace all references to A with B in old face */
		face->mIndices[i] = B;
	}

	for (uint32 i = 0, count = face->mIndices.size(); i < count; ++i)
	{
		// 3. Generate new quad ABCD connecting 'face' and ploygonIndex vertices

		// Make edge #1
		A = faceWind[i];
		B = face->mIndices[i];

		// Make edge #2
		if (i+1 < count)
		{
			C = face->mIndices[i+1];
			D = faceWind[i+1];
		}
		else
		{
			C = face->mIndices[0];
			D = faceWind[0];
		}

		// Make the quad face from #1 and #2
		Face *genFace = GetFace(CreateFace());
		genFace->mMaterial = face->mMaterial;
		genFace->mIndices.pushBack(A);
		genFace->mIndices.pushBack(B);
		genFace->mIndices.pushBack(C);
		genFace->mIndices.pushBack(D);
		
		if (face->mFlags & Face::fPolyMappedTexCoords)
		{
			// This will have to change when ploymapping changes
			genFace->mTexCoordIndices.pushBack(A);
			genFace->mTexCoordIndices.pushBack(B);
			genFace->mTexCoordIndices.pushBack(C);
			genFace->mTexCoordIndices.pushBack(D);
		}
	}
}


void Mesh::SetFaceFlags(index_t face, uint32 flags)
{
	Face *f = GetFace(face);
		
	if (f)
	{
		// Set flags
		f->mFlags |= flags;
	}
}


void Mesh::SetVertexArrayPos(index_t vertexIndex, const vec3_t xyz)
{	
	if (vertexIndex < mVertexPool.size())
		SetTripleVec(mVertexPool, vertexIndex, xyz);	
}


void Mesh::ClearFaceFlags(index_t face, uint32 flags)
{
	Face *f = GetFace(face);
		
	if (f)
	{
		// Clear flags
		f->mFlags |= flags;
		f->mFlags ^= flags;
	}
}


void Mesh::SetVertexFlags(index_t face, uint32 flags)
{
	Vertex *f = GetVertex(face);
		
	if (f)
	{
		// Set flags
		f->mFlags |= flags;
	}
}


void Mesh::ClearVertexFlags(index_t face, uint32 flags)
{
	Vertex *f = GetVertex(face);
		
	if (f)
	{
		// Clear flags
		f->mFlags |= flags;
		f->mFlags ^= flags;
	}
}


void Mesh::Rotate(vec_t x, vec_t y, vec_t z)
{
	BUG_ME("Not Implemented", __FILE__, __LINE__);
}


void Mesh::Scale(vec_t x, vec_t y, vec_t z)
{
	BUG_ME("Not Implemented", __FILE__, __LINE__);
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
