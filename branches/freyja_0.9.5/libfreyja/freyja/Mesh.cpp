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
	mTrack(),
	mVertexAnimTrack(),
	mUID(mNextUID++),
	mInitBoundingVol(false),
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


Mesh::Mesh(const Mesh &mesh) :
	mTrack(),
	mVertexAnimTrack(),
	mUID(mNextUID++),
	mInitBoundingVol(false),
	mFlags(mesh.mFlags),
	mMaterialIndex(mesh.mMaterialIndex),
	mPosition(mesh.mPosition),
	mRotation(mesh.mRotation),
	mScale(mesh.mScale),
	mBoundingVolume(mesh.mBoundingVolume),
	mVertexPool(mesh.mVertexPool),
	mFreedVertices(mesh.mFreedVertices),
	mNormalPool(mesh.mNormalPool),
	mFreedNormals(mesh.mFreedNormals),
	mColorPool(mesh.mColorPool),
	mFreedColors(mesh.mFreedColors),
	mTexCoordPool(mesh.mTexCoordPool),
	mFreedTexCoords(mesh.mFreedTexCoords),
	mFaces(),
	mVertices(),
	mWeights()
{
	uint32 i;

	foreach (((Mesh&)mesh).mVertices, i)
	{
		if (((Mesh&)mesh).mVertices[i])
		{
			mVertices.pushBack(new Vertex(*(((Mesh&)mesh).mVertices[i])));
		}
		else  // Need to NULL pad to exact match if needed
		{
			mVertices.pushBack(NULL);
		}

		DEBUG_MSG("\t%i - vertices copied\n", i);
	}


	foreach (((Mesh&)mesh).mFaces, i)
	{
		if (((Mesh&)mesh).mFaces[i])
		{
			mFaces.pushBack(new Face(*(((Mesh&)mesh).mFaces[i])));
		}
		else  // Need to NULL pad to exact match if needed
		{
			mFaces.pushBack(NULL);
		}

		DEBUG_MSG("\t%i - Facees copied\n", i);
	}

#if 0
	mesh.mWeights;
#endif
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


bool Mesh::Serialize(SystemIO::FileWriter &w)
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
	w.WriteLong(FREYJA_CHUNK_MESH);
	w.WriteLong(chunk.size);
	w.WriteLong(chunk.flags);
	w.WriteLong(chunk.version);

	/* Write chunk data to diskfile */
	w.WriteLong(mUID);
	w.WriteLong(mFlags);
	w.WriteLong(mMaterialIndex);
	w.WriteFloat32(mPosition.mVec[0]);
	w.WriteFloat32(mPosition.mVec[1]);
	w.WriteFloat32(mPosition.mVec[2]);
	w.WriteFloat32(mRotation.mVec[0]);
	w.WriteFloat32(mRotation.mVec[1]);
	w.WriteFloat32(mRotation.mVec[2]);
	w.WriteFloat32(mScale.mVec[0]);
	w.WriteFloat32(mScale.mVec[1]);
	w.WriteFloat32(mScale.mVec[2]);

	SerializePool(w, mVertexPool, mFreedVertices);
	SerializePool(w, mNormalPool, mFreedNormals);
	SerializePool(w, mColorPool, mFreedColors);
	SerializePool(w, mTexCoordPool, mFreedTexCoords);

	w.WriteLong(mFaces.size());
	for ( uint32 i = 0; i < mFaces.size(); ++i )
	{
		if ( mFaces[i] ) 
			mFaces[i]->Serialize(w);
	}

	w.WriteLong(mVertices.size());
	for ( uint32 i = 0; i < mVertices.size(); ++i )
	{
		if ( mVertices[i] ) 
			mVertices[i]->Serialize(w);
	}

	w.WriteLong(mWeights.size());
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

	if (idx < mVertices.size())
	{
		Vertex *vert = mVertices[idx];

		if ( vert ) 
		{
			GetVertexArrayPos(vert->mVertexIndex, v.mVec);
		}
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


bool Mesh::IntersectPerFace(Ray &r, vec_t &t)
{
	if (!Intersect(r, t))
		return false;

	vec_t bestDist = 99999.0f;
	r.mDir.normalize();
	int32 face0 = -1;
	
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

			if (intersect)
			{
				vec_t dist = tuv.mVec[0];

				if (face0 == -1 || dist < bestDist)
				{
					t = bestDist = dist;
					face0 = i;
				}
			}
		}
	}

	// Did we have a 'best hit'?
	return (face0 > -1);
}


bool Mesh::Intersect(Ray &r, vec_t &t)
{
	r.mDir.normalize();

	// Sphere test
	if (!r.IntersectSphere(mBoundingVolume.mSphere.mCenter, 
						   mBoundingVolume.mSphere.mRadius, t))
	{
		return false;
	}

	// Bbox test
	vec_t bestDist = 99999.0f;
	vec3_t min, max;

	GetBBox(min, max);

	const uint32 count = 6;
	Vec3 v[count];

	v[0] = Vec3(max[0], max[1], max[2]);
	v[1] = Vec3(min[0], min[1], min[2]);
	v[2] = Vec3(max[0], min[1], max[2]);
	v[3] = Vec3(min[0], max[1], max[2]);
	v[4] = Vec3(max[0], max[1], min[2]);
	v[5] = Vec3(min[0], min[1], max[2]);
	v[6] = Vec3(min[0], max[1], min[2]);
	v[7] = Vec3(max[0], min[1], min[2]);

	bool intersect = false;
	Vec3 tuv;

	// Quick and dirty hit test that assumes you can pusedo tesselate 
	// a quad here and always get as good results...
	for (uint32 i = 0, a, b, c, d; i < count; ++i)
	{
		switch (i)
		{
		case 0:    
			a = 1, b = 6, c = 4, d = 7;
			break;

		case 1:
			a = 6, b = 3, c = 0, d = 4;
			break;

		case 2:
			a = 1, b = 5, c = 2, d = 7;
			break;

		case 3:
			a = 1, b = 6, c = 3, d = 5;
			break;

		case 4:
			a = 4, b = 0, c = 2, d = 7;
			break;

		case 5:
			a = 3, b = 0, c = 2, d = 5;
			break;
		}

		if (r.IntersectTriangle(v[a].mVec, v[b].mVec, v[c].mVec, tuv.mVec))
		{
			intersect = true;
			
			if (tuv.mVec[0] < bestDist)
			{
				t = bestDist = tuv.mVec[0];
			}
		}
		else if (r.IntersectTriangle(v[c].mVec, v[d].mVec, v[a].mVec, tuv.mVec))
		{
			intersect = true;
			
			if (tuv.mVec[0] < bestDist)
			{
				t = bestDist = tuv.mVec[0];
			}
		}
	}

	return intersect;
}


bool Mesh::IntersectFaces(Ray &r, int &face0, bool markAll)
{
	//no 'editor side' scale support vec_t t; if (!Intersect(r, t)) return false;

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
	vec_t t;

	//no 'editor side' scale support if (!Intersect(r, t)) return false;

	Vec3 center, normal;
	vec_t bestDist = 99999.0f;
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

void Mesh::CollapseEdge(index_t faceIndex, 
						uint32 a, uint32 b, uint32 c, uint32 d)
{
	Face *face = GetFace(faceIndex);

	if (!face || face->mIndices.size() == 0)
		return;

	for (uint32 i = 0, count = face->mIndices.size(); i < count; ++i)
	{
		if (face->mIndices[i] == a) face->mIndices[i] = c;
		if (face->mIndices[i] == d) face->mIndices[i] = d;
	}


	BUG_ME("Can't be implemented properly until vertex refs are checked in.");

	// Edge #1 AB
	// Edge #2 CD
}


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


void Mesh::SelectedFacesMarkSmoothingGroup(uint32 group, bool t)
{
	/* Compute face normals */
	for (uint32 f = 0, fn = GetFaceCount(); f < fn; ++f)
	{
		Face *face = GetFace(f);

		if (!face || !(face->mFlags & Face::fSelected))
			continue;

		if (t)
		{
			face->SetSmoothingGroup(group);
		}
		else
		{
			face->ClearSmoothingGroup(group);
		}
	}
}


void Mesh::GroupedFacesGenerateVertexNormals(uint32 group)
{
	// FIXME: Cached face refs would be smart for all the connectivty use
	BUG_ME("Function local vertex refs implementation used.");
	Vector<Vec3> faceNormals;
	Vec3 a, b, c, normal;

    for (uint32 v = 0, vn = GetVertexCount(); v < vn; ++v)
    {
		Vertex *vertex = GetVertex(v);

		if (vertex)
			vertex->mPolyRefIndices.clear();
	}

	/* Compute face normals */
	for (uint32 f = 0, fn = GetFaceCount(); f < fn; ++f)
	{
		Face *face = GetFace(f);

		// We only consider selected facets here... you filter updates here
		if (!face || !(face->mSmoothingGroups & (1<<group)))
		{
			faceNormals.pushBack(normal);  // For alignment purposes push a 'V0'
			continue;
		}

		for (uint32 v = 0, vn = face->mIndices.size(); v < vn; ++v)
		{
			Vertex *vertex = GetVertex(face->mIndices[v]);
			
			if (vertex)
				vertex->mPolyRefIndices.pushBack(f);
		}

		GetVertexPos(face->mIndices[0], a.mVec);
		GetVertexPos(face->mIndices[1], b.mVec);
		GetVertexPos(face->mIndices[2], c.mVec);

		/* Compute normal for the face, and store it */
		normal = Vector3d::cross(a - b, c - b);
		normal.normalize();
		faceNormals.pushBack(normal);
	}


	/* Compute vertex normals */
    for (uint32 v = 0, vn = GetVertexCount(); v < vn; ++v)
    {
		Vertex *vertex = GetVertex(v);

		if (!vertex)
			continue;

		normal.zero();

		if (vertex->mPolyRefIndices.size() == 0)
		{
			// This means it's not selected for update, or
			// has no faces containing this vertex
			continue;
		}

		for (uint32 j = 0, jn = vertex->mPolyRefIndices.size(); j < jn; ++j)
		{
			if (vertex->mPolyRefIndices[j] == INDEX_INVALID)
			{
				freyjaPrintError("%s> ERROR Bad face reference\n", __func__);
				continue;
			}

			normal += faceNormals[vertex->mPolyRefIndices[j]];
		}

		normal.normalize();

		// FIXME: Doesn't use vertex normal remap ( which isn't used yet )
		SetNormal(v, normal.mVec);
    }
}


void Mesh::SelectedFacesGenerateVertexNormals()
{
	// FIXME: Cached face refs would be smart for all the connectivty use
	BUG_ME("Function local vertex refs implementation used.");
	Vector<Vec3> faceNormals;
	Vec3 a, b, c, normal;

    for (uint32 v = 0, vn = GetVertexCount(); v < vn; ++v)
    {
		Vertex *vertex = GetVertex(v);

		if (vertex)
			vertex->mPolyRefIndices.clear();
	}

	/* Compute face normals */
	for (uint32 f = 0, fn = GetFaceCount(); f < fn; ++f)
	{
		Face *face = GetFace(f);

		// We only consider selected facets here... you filter updates here
		if (!face || !(face->mFlags & Face::fSelected))
		{
			faceNormals.pushBack(normal);  // For alignment purposes push a 'V0'
			continue;
		}

		for (uint32 v = 0, vn = face->mIndices.size(); v < vn; ++v)
		{
			Vertex *vertex = GetVertex(face->mIndices[v]);
			
			if (vertex)
				vertex->mPolyRefIndices.pushBack(f);
		}

		GetVertexPos(face->mIndices[0], a.mVec);
		GetVertexPos(face->mIndices[1], b.mVec);
		GetVertexPos(face->mIndices[2], c.mVec);

		/* Compute normal for the face, and store it */
		normal = Vector3d::cross(a - b, c - b);
		normal.normalize();
		faceNormals.pushBack(normal);
	}


	/* Compute vertex normals */
    for (uint32 v = 0, vn = GetVertexCount(); v < vn; ++v)
    {
		Vertex *vertex = GetVertex(v);

		if (!vertex)
			continue;

		normal.zero();

		if (vertex->mPolyRefIndices.size() == 0)
		{
			// This means it's not selected for update, or
			// has no faces containing this vertex
			continue;
		}

		for (uint32 j = 0, jn = vertex->mPolyRefIndices.size(); j < jn; ++j)
		{
			if (vertex->mPolyRefIndices[j] == INDEX_INVALID)
			{
				freyjaPrintError("%s> ERROR Bad face reference\n", __func__);
				continue;
			}

			normal += faceNormals[vertex->mPolyRefIndices[j]];
		}

		normal.normalize();

		// FIXME: Doesn't use vertex normal remap ( which isn't used yet )
		SetNormal(v, normal.mVec);
    }
}


void Mesh::SplitFace(index_t faceIndex)
{
	Face *face = GetFace(faceIndex);

	if (!face || face->mIndices.size() == 0)
		return;

	Face *genFace;
	int32 A, B, C, D, M1, M2;
	Vec3 m1, m2, a, b, c, d, uv, norm;
	bool gap1 = false, gap2 = false;

 
	switch (face->mIndices.size())
	{
	case 4:

		// 1. Generate midpoint vertices
		A = face->mIndices[0];
		GetVertexPos(A, a.mVec);
		B = face->mIndices[1];
		GetVertexPos(B, b.mVec);
		C = face->mIndices[2];
		GetVertexPos(C, c.mVec);
		D = face->mIndices[3];
		GetVertexPos(D, d.mVec);

		// M1
		helMidpoint3v(a.mVec, b.mVec, m1.mVec);
		uv = GetVertexTexCoord(A) + GetVertexTexCoord(A);
		uv *= 0.5f;
		norm = GetVertexNormal(A) + GetVertexNormal(B);
		norm.normalize();
		M1 = CreateVertex(m1.mVec, uv.mVec, norm.mVec);

		// M2
		helMidpoint3v(c.mVec, d.mVec, m2.mVec);
		uv = GetVertexTexCoord(C) + GetVertexTexCoord(D);
		uv *= 0.5f;
		norm = GetVertexNormal(C) + GetVertexNormal(D);
		norm.normalize();
		M2 = CreateVertex(m2.mVec, uv.mVec, norm.mVec);

		// 2. Generate a new quad face, and reuse the old one with new vertices
		genFace = GetFace(CreateFace());
		genFace->mMaterial = face->mMaterial;
		genFace->mIndices.pushBack(A);
		genFace->mIndices.pushBack(M1);
		genFace->mIndices.pushBack(M2);
		genFace->mIndices.pushBack(D);
		
		// This won't be correct
		if (face->mFlags & Face::fPolyMappedTexCoords)
		{
			// This will have to change when polymapping changes
			genFace->mTexCoordIndices.pushBack(A);
			genFace->mTexCoordIndices.pushBack(M1);
			genFace->mTexCoordIndices.pushBack(M2);
			genFace->mTexCoordIndices.pushBack(D);
		}

		// Reuse old face here
		face->mIndices[0] = B;
		face->mIndices[1] = M1;
		face->mIndices[2] = M2;
		face->mIndices[3] = C;

		// This won't be correct
		if (face->mFlags & Face::fPolyMappedTexCoords)
		{
			// This will have to change when polymapping changes
			genFace->mTexCoordIndices.pushBack(B);
			genFace->mTexCoordIndices.pushBack(M1);
			genFace->mTexCoordIndices.pushBack(M2);
			genFace->mTexCoordIndices.pushBack(C);
		}

		// 3. Use vertex references to check for gaps
		BUG_ME("No vertex refs to check for gaps, assuming closed topology...");
		gap1 = gap2 = true;

		// 4. Fill in the two gaps with triangles
		if (gap1)
		{
			genFace = GetFace(CreateFace());
			genFace->mMaterial = face->mMaterial;
			genFace->mIndices.pushBack(A);
			genFace->mIndices.pushBack(M1);
			genFace->mIndices.pushBack(B);
		}

		if (gap2)
		{
			genFace = GetFace(CreateFace());
			genFace->mMaterial = face->mMaterial;
			genFace->mIndices.pushBack(D);
			genFace->mIndices.pushBack(M2);
			genFace->mIndices.pushBack(C);
		}
		break;

	default:
		;
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

bool Mesh::SerializePool(SystemIO::FileWriter &w, 
						 Vector<vec_t> &v, mstl::stack<index_t> &s)
{
	mstl::stack<index_t> copy; // We don't really care about order

	w.WriteLong(v.size());

	for ( uint32 i = 0; i < v.size(); ++i )
	{
		w.WriteFloat32(v[i]);
	}
		
	w.WriteLong(s.size());

	for ( uint32 i = 0; i < s.size(); ++i )
	{
		index_t item = s.pop();
		copy.push(item);
		w.WriteLong(item);
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
