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


bool Mesh::IntersectFaces(Ray &r, int &face0, bool markAll)
{
	vec_t bestDist = 99999.0f;
	face0 = -1;
	

	for (uint32 i = 0, n = GetFaceCount(); i < n; ++i)
	{
		Face *f = GetFace(i);
		
		if (!f) 
			continue;
	
		// Quick and dirty hit test that pretends you can 
		// pusedo tesselate a face and get as good results
		bool intersect = false;
					
		if (f->mIndices.size() > 2)
		{
			Vec3 a, b, c, p;

			GetVertexPos(f->mIndices[0], a.mVec);
			GetVertexPos(f->mIndices[1], b.mVec);
		
			for (uint32 j = 2; j < f->mIndices.size(); ++j)
			{
				GetVertexPos(f->mIndices[j], c.mVec);
				intersect = r.IntersectTriangle(a.mVec, b.mVec, c.mVec, p);
			
				if (intersect)
				{
					break;
				}
			
				a = b;
				b = c;
			}
		

			if (intersect)
			{
				vec_t dist = helDist3v(r.mOrigin.mVec, p.mVec);

				if (dist < bestDist)
				{
					bestDist = dist;
					face0 = i;
				}

				if (markAll) f->mFlags |= Face::fRayHit;
			}
			else 
			{
				f->mFlags |= Face::fRayHit;
				f->mFlags ^= Face::fRayHit;
			}
		}
	}

	if (face0 > -1)
	{
		Face *f = GetFace(face0);
		
		if (f) 
			f->mFlags |= Face::fRayHit;
	}

	return (face0 > -1);
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
