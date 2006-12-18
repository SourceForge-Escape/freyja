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

Vector<Mesh *> Mesh::mGobalPool;
//index_t Mesh::mNextUID = 0;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Mesh::Mesh() :
	mTrack(),
	mVertexAnimTrack(),
	mBlendVerticesTime(-1.0f),
	mBlendVertices(),
	mUID(INDEX_INVALID),
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
	mTrack(mesh.mTrack),
	mVertexAnimTrack(mesh.mVertexAnimTrack),
	mBlendVerticesTime(-1.0f),
	mBlendVertices(),
	mUID(INDEX_INVALID),
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
	snprintf(mName, mNameSize-1, "Mesh%i", mUID);
	mName[mNameSize-1] = 0;

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


	foreach (((Mesh&)mesh).mWeights, i)
	{
		if (((Mesh&)mesh).mWeights[i])
		{
			mWeights.pushBack(new Weight(*(((Mesh&)mesh).mWeights[i])));
		}
		else  // Need to NULL pad to exact match if needed
		{
			mWeights.pushBack(NULL);
		}

		DEBUG_MSG("\t%i - weights copied\n", i);
	}

	//VertexCleanup();
}


Mesh::~Mesh()
{
	// Force removal in case of bad delete calls
	if (GetMesh(mUID) == this)
		RemoveFromPool();

	uint32 i;

	foreach (mWeights, i)
	{
		if (mWeights[i])
		{
			delete mWeights[i];
			mWeights[i] = NULL;
		}
	}

	foreach (mVertices, i)
	{
		if (mVertices[i])
		{
			delete mVertices[i];
			mVertices[i] = NULL;
		}
	}

	foreach (mFaces, i)
	{
		if (mFaces[i])
		{
			delete mFaces[i];
			mFaces[i] = NULL;
		}
	}
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

void Mesh::GetSelectedVertices(Vector<index_t> &list) 
{
	list.clear();

	for (uint32 i = 0, icount = GetVertexCount(); i < icount; ++i)
	{
		Vertex *v = GetVertex(i);

		if (!v) 
			continue;

		if (v->mFlags & Vertex::fSelected)
		{
			list.push_back(i);
		}
	}	
}


bool Mesh::SerializePool(SystemIO::TextFileWriter &w, const char *name,
						 Vector<vec_t> &array, mstl::stack<index_t> &stack)
{
	if (!array.size())
	{
		w.Print("\t%sStack %u\n", name, 0);
		w.Print("\t%sArray %u\n", name, 0);	
		return true;
	}

	{
		w.Print("\t%sStack %u\n", name, stack.size());
		mstl::StackNode<index_t> *cur = stack.top();

		// FIXME: This will cause it to read in reverse, but it doesn't matter 
		while (cur)
		{
			index_t data = cur->Data();
			w.Print("%u ", data);
			cur = cur->Prev();
		}

		w.Print("\n");
	}

	w.Print("\t%sArray %u\n", name, array.size());
	for (uint32 i = 0, n = array.size(); i < n; ++i)
	{
		if (i % 3 == 0) w.Print("\n\t"); // Makes it easier on text editors
		w.Print("%f ", array[i]);
	}
	w.Print("\n");

	return true;
}


bool Mesh::SerializePool(SystemIO::TextFileReader &r, const char *name,
						 Vector<vec_t> &array, mstl::stack<index_t> &stack)
{
	freyjaPrintMessage("Loading %s pool...\n", name);

	{
		r.ParseSymbol(); // name + "Stack"
		int32 count = r.ParseInteger();

		while (count > 0)
		{
			stack.push(r.ParseInteger());
			--count;
		}
	}

	r.ParseSymbol(); // name + "Array"
	int32 count = r.ParseInteger();

	while (count > 0)
	{
		array.push_back(r.ParseFloat());
		--count;
	}

	return true;
}


bool Mesh::Serialize(SystemIO::TextFileWriter &w)
{
	w.Print("Mesh\n");
	w.Print("\t mVersion 2\n");
	w.Print("\t mName \"%s\"\n", mName);
	w.Print("\t mUID %u\n", mUID);
	w.Print("\t mFlags %u\n", mFlags);
	w.Print("\t mMaterial %u\n", mMaterialIndex);
	w.Print("\t mPosition %f %f %f\n", mPosition[0], mPosition[1], mPosition[2]);
	w.Print("\t mRotation %f %f %f\n", mRotation[0], mRotation[1], mRotation[2]);
	w.Print("\t mScale %f %f %f\n", mScale[0], mScale[1], mScale[2]);
	w.Print("\t mBoundingVolume.mBox %f %f %f %f %f %f\n", 
			mBoundingVolume.mBox.mMin[0],
			mBoundingVolume.mBox.mMin[1],
			mBoundingVolume.mBox.mMin[2],
			mBoundingVolume.mBox.mMax[0],
			mBoundingVolume.mBox.mMax[1],
			mBoundingVolume.mBox.mMax[2]);
	w.Print("\t mBoundingVolume.mSphere %f %f %f %f\n", 
			mBoundingVolume.mSphere.mRadius,
			mBoundingVolume.mSphere.mCenter[0], 
			mBoundingVolume.mSphere.mCenter[1], 
			mBoundingVolume.mSphere.mCenter[2]);

	SerializePool(w, "mVertex", mVertexPool, mFreedVertices);
	SerializePool(w, "mTexCoord", mTexCoordPool, mFreedTexCoords);
	SerializePool(w, "mNormal", mNormalPool, mFreedNormals);

	w.Print("\t mWeights %u\n", mWeights.size());
	for (uint32 i = 0, n = mWeights.size(); i < n; ++i)
	{
		Weight *ww = GetWeight(i);
		if (ww) ww->Serialize(w); else w.Print("\t\tNULL\n");
	}

	w.Print("\t mVertices %u\n", mVertices.size());
	for (uint32 i = 0, n = mVertices.size(); i < n; ++i)
	{
		Vertex *v = GetVertex(i);
		if (v) v->Serialize(w); else w.Print("\t\tNULL\n");
	}

	for (uint32 i = 0, n = mFaces.size(), count = 0; i < n; ++i)
	{
		if (GetFace(i)) ++count;
	}

	w.Print("\t mFaces %u\n", mFaces.size());
	for (uint32 i = 0, n = mFaces.size(); i < n; ++i)
	{
		Face *f = GetFace(i);
		if (f) f->Serialize(w); else w.Print("\t\tNULL\n");
	}

	w.Print("\t mTracks %u\n", 1); // only have one in test =p
	mTrack.Serialize(w);

	w.Print("\t mVertexAnimTracks %u\n", 1); // only have one in test =p
	mVertexAnimTrack.Serialize(w);

	return true;
}


bool Mesh::Serialize(SystemIO::TextFileReader &r)
{
	// Currently no parsing/checking done as this is mostly for debugging

	r.ParseSymbol(); // Mesh

	r.ParseSymbol(); // version
	uint32 version = r.ParseInteger();

	r.ParseSymbol(); // name
	const char *s = r.ParseStringLiteral();
	SetName(s);
	if (s) delete [] s;

	r.ParseSymbol(); // uid
	/*mUID =*/ r.ParseInteger();
	
	r.ParseSymbol(); // flags
	mFlags = r.ParseInteger();

	r.ParseSymbol(); // material
	mMaterialIndex = r.ParseInteger();

	r.ParseSymbol(); // loc 
	mPosition[0] = r.ParseFloat();
	mPosition[1] = r.ParseFloat();
	mPosition[2] = r.ParseFloat();

	r.ParseSymbol(); // rot 
	mRotation[0] = r.ParseFloat();
	mRotation[1] = r.ParseFloat();
	mRotation[2] = r.ParseFloat();

	r.ParseSymbol(); // siz
	mScale[0] = r.ParseFloat();
	mScale[1] = r.ParseFloat();
	mScale[2] = r.ParseFloat();

	r.ParseSymbol(); // bbox
	mBoundingVolume.mBox.mMin[0] = r.ParseFloat();
	mBoundingVolume.mBox.mMin[1] = r.ParseFloat();
	mBoundingVolume.mBox.mMin[2] = r.ParseFloat();
	mBoundingVolume.mBox.mMax[0] = r.ParseFloat();
	mBoundingVolume.mBox.mMax[1] = r.ParseFloat();
	mBoundingVolume.mBox.mMax[2] = r.ParseFloat();

	r.ParseSymbol(); // bsphere
	mBoundingVolume.mSphere.mRadius = r.ParseFloat();
	mBoundingVolume.mSphere.mCenter[0] = r.ParseFloat();
	mBoundingVolume.mSphere.mCenter[1] = r.ParseFloat();
	mBoundingVolume.mSphere.mCenter[2] = r.ParseFloat();

	SerializePool(r, "vertex", mVertexPool, mFreedVertices);
	SerializePool(r, "texcoord", mTexCoordPool, mFreedTexCoords);
	SerializePool(r, "normal", mNormalPool, mFreedNormals);

	freyjaPrintMessage("Loading weights...\n");
	r.ParseSymbol(); // Weight
	{
		int32 count = r.ParseInteger();
		while (count > 0)
		{
			Weight *w = new Weight();
			if (w->Serialize(r))
			{
				mWeights.push_back(w);
			}
			else
			{
				delete w;
				mWeights.push_back(NULL);
			}
			--count;
		}
	}

	freyjaPrintMessage("Loading vertices...\n");
	r.ParseSymbol(); // Vertex
	{
		int32 count = r.ParseInteger();
		while (count > 0)
		{
			Vertex *v = new Vertex();
			if (v->Serialize(r))
			{
				mVertices.push_back(v);
			}
			else
			{
				delete v;
				mVertices.push_back(NULL);
			}
			--count;
		}
	}

	freyjaPrintMessage("Loading faces...\n");
	r.ParseSymbol(); // Face
	{
		int32 count = r.ParseInteger();
		while (count > 0)
		{
			Face *f = new Face();
			if (f->Serialize(r))
			{
				mFaces.push_back(f);
			}
			else
			{
				delete f;
				mFaces.push_back(NULL);
			}
			--count;
		}
	}

	if (version == 1)
		return true;

	r.ParseSymbol(); // mTracks
	{
		int32 count = r.ParseInteger();
		while (count > 0)
		{
			// only have one in test =p
			mTrack.Serialize(r);
			--count;
		}
	}

	r.ParseSymbol(); // mVertexAnimTracks
	{
		int32 count = r.ParseInteger();
		while (count > 0)
		{
			// only have one in test =p
			mVertexAnimTrack.Serialize(r);
			--count;
		}
	}

	return true;
}


bool Mesh::Serialize(SystemIO::FileWriter &w)
{
	freyja_file_chunk_t chunk;

	chunk.type = FREYJA_CHUNK_MESH;
	chunk.size = 0;
	chunk.flags = 0x0;
	chunk.version = 10;

	// FIXME: Screw this -- 
	// just use file seek to come back and write the value =P
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
	chunk.size += 4 + mFaces.size() * 0;//Face::SerializedSize();
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
	if (GetFlags() & fHidden)
		return false;

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


void Mesh::SelectFacesByBox(Vec3 &min, Vec3 &max)
{
	Vec3 mina(min), maxa(max);

	// Reorder as needed, since we don't trust input
	for (int32 i = 2; i >= 0; --i)
	{
		if (min[i] > max[i])
		{
			mina[i] = max[i];
			maxa[i] = min[i];
		}
	}

	for (uint32 i = 0, iCount = GetVertexCount(); i < iCount; ++i)
	{
		Vertex *v = GetVertex(i);

		if (!v) 
			continue;

		Vec3 p;
		bool inside = true;
		v->mFlags &= ~Vertex::fSelected2;
		GetVertexArrayPos(v->mVertexIndex, p.mVec);
		
		for (int32 j = 2; inside && j >= 0; --j)
		{
			if (p[j] > maxa[j] || p[j] < mina[j])
			{
				inside = false;
			}
		}

		if (inside)
		{
			v->mFlags |= Vertex::fSelected2;
		}
	}

	
	for (uint32 i = 0, iCount = GetFaceCount(); i < iCount; ++i)
	{
		Face *f = GetFace(i);

		if (f)
		{
			bool select = true;
			uint32 j;
			foreach (f->mIndices, j)
			{
				Vertex *v = GetVertex(f->mIndices[j]);
				if (!(v->mFlags & Vertex::fSelected2))
				{
					select = false;
				}
			}

			if (select)
				f->mFlags |= Face::fSelected;
		}
	}
}


void Mesh::SelectVerticesByBox(Vec3 &min, Vec3 &max)
{
	Vec3 mina(min), maxa(max);

	// Reorder as needed, since we don't trust input
	for (int32 i = 2; i >= 0; --i)
	{
		if (min[i] > max[i])
		{
			mina[i] = max[i];
			maxa[i] = min[i];
		}
	}

	for (uint32 i = 0, iCount = GetVertexCount(); i < iCount; ++i)
	{
		Vertex *v = GetVertex(i);

		if (!v) 
			continue;

		Vec3 p;
		bool inside = true;
		GetVertexArrayPos(v->mVertexIndex, p.mVec);
		
		for (int32 j = 2; inside && j >= 0; --j)
		{
			if (p[j] > maxa[j] || p[j] < mina[j])
			{
				inside = false;
			}
		}

		if (inside)
		{
			v->mFlags |= Vertex::fSelected;
		}
	}	
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

index_t Mesh::CreateVertex(const vec3_t xyz, const vec3_t uvw, const vec3_t nxyz)
{
	Vertex **array = mVertices.getVectorArray();
	index_t vertex = AddTripleVec(mVertexPool, mFreedVertices, (vec_t*)xyz);
	index_t texcoord = AddTripleVec(mTexCoordPool, mFreedTexCoords, (vec_t*)uvw);
	index_t normal = AddTripleVec(mNormalPool, mFreedNormals, (vec_t*)nxyz);
	Vertex *vert = new Vertex(vertex, texcoord, normal);
	
	for ( uint32 i = 0, count = mVertices.size(); i < count; ++i )
	{
		if (array[i] == NULL)
		{
			array[i] = vert;
			return i;
		}
	}
	
	// Adjust bounding volume
	if (!mInitBoundingVol)
	{
		mInitBoundingVol = true;
		SetBBox(xyz, xyz);
	}
	else
	{
		vec3_t min;
		vec3_t max;
		bool update = false;

		GetBBox(min, max);
		
		for (uint32 i = 0; i < 3; ++i)
		{
			if (xyz[i] < min[i])
			{
				min[i] = xyz[i];
				update = true;
			}
			else if (xyz[i] > max[i])
			{
				max[i] = xyz[i];
				update = true;
			}
		}

		if (update)
		{
			SetBBox(min, max);
		}
	}

	mVertices.pushBack(vert);
	return mVertices.size() - 1;
}


bool Mesh::DeleteVertex(index_t vertex)
{
	bool result = false;

	// We don't trust anyone calling this method...
	RebuildVertexPolygonReferences(vertex);

	Vertex **array = mVertices.get_array();
	Vertex *v = GetVertex(vertex);

	if ( v && v->GetFaceRefs().size() == 0)
	{
		array[vertex] = NULL;
		mFreedVertices.push(v->mVertexIndex);
		mFreedTexCoords.push(v->mTexCoordIndex);
		mFreedNormals.push(v->mNormalIndex);
		delete v;

		result = true;
	}

	return result;
}


void Mesh::MeldVertices(index_t a, index_t b)
{
	Vertex *va = GetVertex(a);
	Vertex *vb = GetVertex(b);

	if (va && vb)
		va->Meld(*vb);
}


bool Mesh::WeldVertices(index_t a, index_t b)
{
	Vertex *va = GetVertex(a);
	Vertex *vb = GetVertex(b);

	if (!va || !vb)
		return false;

	// Make all polygons referencing A point to B
	for (uint32 i = mFaces.begin(); i < mFaces.end(); ++i)
	{
		if (mFaces[i])
		{
			mFaces[i]->WeldVertices(a, b);
		}
	}

	// Delete A from the vertex pool
	va->GetFaceRefs().clear();
	return DeleteVertex(a);
}


void Mesh::WeldVerticesByDistance(vec_t tolerance)
{
	for (uint32 i = 0, iCount = GetVertexCount(), jCount = GetVertexCount();
		 i < iCount; ++i)
	{
		for (uint32 j = 0; j < jCount; ++j)
		{
			vec_t dist = helDist3v(GetVertexPosition(i).mVec, 
								   GetVertexPosition(j).mVec);
			
			if (dist < tolerance)
				WeldVertices(i, j);
		}
	}
}


void Mesh::Merge(Mesh *mesh)
{
	if (mesh == NULL)
		return;

	// FIXME: Merge doesn't merge animations...
	//TransformTrack mTrack;
	//VertexAnimTrack mVertexAnimTrack;

	Vector<index_t> transV; // need to translate ids to 'this' mesh ids

	uint32 i;

	foreach (mesh->mVertices, i)
	{
		if (mesh->mVertices[i])
		{
			Vec3 n = mesh->GetVertexNormal(i);
			Vec3 p = mesh->GetVertexPosition(i);
			Vec3 t = mesh->GetVertexTexCoord(i);

			// pos, uvw, normal order
			index_t idx = CreateVertex(p.mVec, t.mVec, n.mVec);

			// transV[i] <= idx
			transV.push_back(idx);

			Vertex *a =	mesh->GetVertex(i);
			Vertex *b = GetVertex(idx);
			b->mFlags = a->mFlags;
		}
		else  // Need to NULL pad to exact match if needed
		{
			transV.push_back(INDEX_INVALID);
		}
	}


	foreach (mesh->mFaces, i)
	{
		Face *face = mesh->GetFace(i);
		if (face)
		{
			Face *newFace = GetFace(CreateFace());

			newFace->mFlags = face->mFlags;
			newFace->mColor = face->mColor;
			newFace->mMaterial = face->mMaterial;
			newFace->mSmoothingGroup = face->mSmoothingGroup;

#if 0 // atm only vertex normals allowed
			if (face->mFlags & Face::fPolyMappedNormals)
			{
				uint32 j;
				foreach (face->mNormalIndices, j)
				{
					vec3_t uvw;
					index_t idx = face->mNormalIndices[j];
					mesh->GetNormal(idx, uvw);
					newFace->AppendNormal(CreateNormal(uvw));
				}
			}
#endif

			if (face->mFlags & Face::fPolyMappedTexCoords)
			{
				uint32 j;
				foreach (face->mTexCoordIndices, j)
				{
					vec3_t uvw;
					index_t idx = face->mTexCoordIndices[j];
					mesh->GetTexCoord(idx, uvw);
					newFace->AppendTexCoord(CreateTexCoord(uvw));
				}
			}

			uint32 j = 0;
			foreach (face->mIndices, j)
			{
				newFace->AppendVertex(transV[face->mIndices[j]]);
			}
		}
	}

	foreach (mesh->mWeights, i)
	{
		Weight *w = mesh->mWeights[i];

		if (w)
		{
			// vertex, weight, bone order
			AddWeight(transV[w->mVertexIndex], w->mWeight, w->mBoneIndex);
		}
	}

	UpdateBoundingVolume();
}


void Mesh::DeleteFace(index_t face)
{
	Face **array = mFaces.get_array();
	Face *f = GetFace(face);

	if ( f )
	{
		array[face] = NULL; // this has to marked NULL before cleanup is called
		FaceRemovalCleanup(f);
		delete f;
	}
}


void Mesh::DeleteSelectedFaces()
{
	Face **array = mFaces.getVectorArray();

	for (uint32 i = 0, n = mFaces.size(); i < n; ++i)
	{
		Face *face = array[i];

		if (face && face->mFlags & Face::fSelected)
		{
			array[i] = NULL;
			//FaceRemovalCleanup(face);
			delete face;
		}
	}

	VertexCleanup();
	UpdateBoundingVolume();
}


void Mesh::DeleteUnSelectedFaces()
{
	Face **array = mFaces.getVectorArray();

	for (uint32 i = 0, n = mFaces.size(); i < n; ++i)
	{
		Face *face = array[i];

		if (face && !(face->mFlags & Face::fSelected))
		{
			array[i] = NULL;
			//FaceRemovalCleanup(face);
			delete face;
		}
	}

	VertexCleanup();
	UpdateBoundingVolume();
}


Mesh *Mesh::Split(bool trim)
{
	Mesh *mesh = new Mesh(*this);
	mesh->DeleteUnSelectedFaces();

	if (trim)
	{
		DeleteSelectedFaces();
	}

	return mesh;
}


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


void Mesh::SetFaceSmoothingGroup(index_t face, uint32 group)
{
	Face *f = GetFace(face);

	if (f)
	{
		f->SetSmoothingGroup(group);
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
			face->ClearSmoothingGroup();
		}
	}
}


void Mesh::AssignGroupToSelectedFaces(uint32 group)
{
	for (uint32 f = 0, fn = GetFaceCount(); f < fn; ++f)
	{
		Face *face = GetFace(f);

		// We only consider facets in given group(s)...
		if (face && face->mFlags & Face::fSelected)
		{
			face->mSmoothingGroup = group; 
		}
	}
}


void Mesh::ClearGroupsFaceSelected(uint32 groups)
{
	for (uint32 f = 0, fn = GetFaceCount(); f < fn; ++f)
	{
		Face *face = GetFace(f);

		// We only consider facets in given group(s)...
		if (face && groups & face->mSmoothingGroup)
		{
			face->mFlags ^= Face::fSelected;
		}
	}
}


void Mesh::SetGroupsFaceSelected(uint32 groups)
{
	for (uint32 f = 0, fn = GetFaceCount(); f < fn; ++f)
	{
		Face *face = GetFace(f);

		// We only consider facets in given group(s)...
		if (face && groups & face->mSmoothingGroup)
		{
			face->mFlags |= Face::fSelected;
		}
	}
}


void Mesh::VertexCleanup()
{
	RebuildVertexPolygonReferences();
	Vertex **array = mVertices.get_array();
	Vertex *vert;

    for (uint32 v = 0, vn = GetVertexCount(); v < vn; ++v)
	{
		vert = array[v];

		if ( vert && vert->GetFaceRefs().size() == 0)
		{
			array[v] = NULL;			
			mFreedVertices.push(vert->mVertexIndex);
			mFreedTexCoords.push(vert->mTexCoordIndex);
			mFreedNormals.push(vert->mNormalIndex);
			delete vert;
		}
	}
}


void Mesh::FaceRemovalCleanup(Face *face)
{
	if (face && face->mIndices.size())
	{
		uint32 i;
		foreach (face->mIndices, i)
		{
			DeleteVertex(face->mIndices[i]);
		}
	}
}


void Mesh::RebuildVertexPolygonReferences(index_t vertex)
{
	Vertex *v = GetVertex(vertex);

	if (!v)
		return;

	v->GetFaceRefs().clear();

	for (uint32 f = 0, fn = GetFaceCount(); f < fn; ++f)
	{
		Face *face = GetFace(f);

		if (!face)
			continue;
		
		uint32 i;
		foreach (face->mIndices, i)
		{
			if (face->mIndices[i] == vertex)
			{
				v->GetFaceRefs().push_back(f);	
			}
		}
	}	
}


void Mesh::RebuildVertexPolygonReferences()
{
    for (uint32 v = 0, vn = GetVertexCount(); v < vn; ++v)
    {
		Vertex *vertex = GetVertex(v);

		if (vertex)
		{
			vertex->GetFaceRefs().clear();
		}
	}	

	for (uint32 f = 0, fn = GetFaceCount(); f < fn; ++f)
	{
		Face *face = GetFace(f);

		if (face)
		{
			uint32 i;
			foreach (face->mIndices, i)
			{
				Vertex *vertex = GetVertex(face->mIndices[i]);
				if (vertex)
				{
					vertex->GetFaceRefs().push_back(f);
				}	
			}
		}
	}	
}


void Mesh::UpdateVertexReferenceWithSelectedBias()
{
	// FIXME: Add some kind of conditional flag here to avoid
	//        uneeded updates.  This isn't so important *yet, since
	//        only 'one-offs' call this for now.
	//
	//        You need one flag for 'mesh not modified' and one for prev filter.

    for (uint32 v = 0, vn = GetVertexCount(); v < vn; ++v)
    {
		Vertex *vertex = GetVertex(v);

		if (vertex)
		{
			vertex->GetTmpRefs().clear();
		}
	}

	for (uint32 f = 0, fn = GetFaceCount(); f < fn; ++f)
	{
		Face *face = GetFace(f);

		// We only consider facets in given group(s)...
		if (!face || !(face->mFlags & Face::fSelected))
		{
			continue;
		}

		// Very expensive
		for (uint32 v = 0, vn = face->mIndices.size(); v < vn; ++v)
		{
			Vertex *vertex = GetVertex(face->mIndices[v]);
			
			if (vertex)
			{
				vertex->GetTmpRefs().pushBack(f);
			}
		}
	}	
}


void Mesh::UpdateVertexReferenceWithColorBias(uint32 color)
{
	// FIXME: Add some kind of conditional flag here to avoid
	//        uneeded updates.  This isn't so important *yet, since
	//        only 'one-offs' call this for now.
	//
	//        You need one flag for 'mesh not modified' and one for prev filter.

    for (uint32 v = 0, vn = GetVertexCount(); v < vn; ++v)
    {
		Vertex *vertex = GetVertex(v);

		if (vertex)
		{
			vertex->GetTmpRefs().clear();
		}
	}

	for (uint32 f = 0, fn = GetFaceCount(); f < fn; ++f)
	{
		Face *face = GetFace(f);

		// We only consider facets in given group(s)...
		if (!face || !(color == face->mColor))
		{
			continue;
		}

		// Very expensive
		for (uint32 v = 0, vn = face->mIndices.size(); v < vn; ++v)
		{
			Vertex *vertex = GetVertex(face->mIndices[v]);
			
			if (vertex)
			{
				vertex->GetTmpRefs().pushBack(f);
			}
		}
	}
}


void Mesh::UpdateVertexReferenceWithSmoothingGroupBias(uint32 groupFilter)
{
	// FIXME: Add some kind of conditional flag here to avoid
	//        uneeded updates.  This isn't so important *yet, since
	//        only 'one-offs' call this for now.
	//
	//        You need one flag for 'mesh not modified' and one for prev filter.

    for (uint32 v = 0, vn = GetVertexCount(); v < vn; ++v)
    {
		Vertex *vertex = GetVertex(v);

		if (vertex)
		{
			vertex->GetTmpRefs().clear();
		}
	}

	for (uint32 f = 0, fn = GetFaceCount(); f < fn; ++f)
	{
		Face *face = GetFace(f);

		// We only consider facets in given group(s)...
		if (!face || !(groupFilter & (1 << face->mSmoothingGroup)))
		{
			continue;
		}

		// Very expensive
		for (uint32 v = 0, vn = face->mIndices.size(); v < vn; ++v)
		{
			Vertex *vertex = GetVertex(face->mIndices[v]);
			
			if (vertex)
			{
				vertex->GetTmpRefs().pushBack(f);
			}
		}
	}
}

 
void Mesh::UVMapSelectedFaces_Spherical()
{
	vec_t longitude, latitude;
	vec3_t xyz;
	vec3_t uvw;

	UpdateVertexReferenceWithSelectedBias();

    for (uint32 v = 0, vn = GetVertexCount(); v < vn; ++v)
    {
		Vertex *vertex = GetVertex(v);

		// Was this allocated and also marked by group bias?
		if (vertex && vertex->GetTmpRefs().size() > 0)
		{
			GetVertexArrayPos(vertex->mVertexIndex, xyz);

			longitude = atan2((float)-xyz[0], xyz[2]);
			latitude = atan(xyz[1] / sqrt(xyz[0]*xyz[0] + xyz[2]*xyz[2]));
			
			longitude = 1.0 - longitude / (HEL_2_PI);
			latitude = fabs(0.5 - latitude / HEL_PI);
			
			uvw[0] = longitude - floor(longitude);
			uvw[1] = latitude;
			uvw[2] = 0.0f;

			SetTexCoord(vertex->mTexCoordIndex, uvw);
		}
	}
}


void Mesh::UVMapSelectedFaces_Cylindrical()
{
	UpdateVertexReferenceWithSelectedBias();
	vec_t longitude, latitude, ysize;
	vec3_t min = {999999.0f, 999999.0f, 999999.0f};
	vec3_t max = {-999999.0f, -999999.0f, -999999.0f};

    for (uint32 v = 0, vn = GetVertexCount(); v < vn; ++v)
    {
		Vertex *vertex = GetVertex(v);

		// Was this allocated and also marked by group bias?
		if (vertex && vertex->GetTmpRefs().size() > 0)
		{
			vec3_t xyz;

			GetVertexArrayPos(vertex->mVertexIndex, xyz);

			for (uint32 j = 0; j < 3; ++j)
			{
				if (xyz[j] < min[j])
					min[j] = xyz[j];

				if (xyz[j] > max[j])
					max[j] = xyz[j];
			}
		}
	}

	if (max[1] >= 0)
	{
		if (min[1] >= 0)
		{
			ysize = max[1] - min[1];
		}
		else
		{
			ysize = max[1] + -min[1];
		}
	}
	else
	{
		ysize = -max[1] + min[1];
	}
	
	if (ysize < 0.0001 && ysize > -0.0001)
		ysize = 1.0f;

    for (uint32 v = 0, vn = GetVertexCount(); v < vn; ++v)
    {
		Vertex *vertex = GetVertex(v);

		// Was this allocated and also marked by group bias?
		if (vertex && vertex->GetTmpRefs().size() > 0)
		{
			vec3_t xyz;
			vec3_t uvw;

			GetVertexArrayPos(vertex->mVertexIndex, xyz);

			longitude = atan2((float)-xyz[0], xyz[2]);
			latitude = atan(xyz[1] / sqrt(xyz[0]*xyz[0] + xyz[2]*xyz[2]));

			longitude = 1.0 - longitude / (HEL_2_PI);
			latitude = fabs(0.5 - latitude / HEL_PI);

			uvw[0] = longitude - floor(longitude);
			uvw[1] = xyz[1] / ysize;
			uvw[2] = 0.0f;

			SetTexCoord(vertex->mTexCoordIndex, uvw);

			// Force face to drop polymapping
			uint32 i;
			foreach (vertex->GetTmpRefs(), i)
			{
				Face *f = GetFace(vertex->GetTmpRefs()[i]);
				if (f)
				{
					SetTexCoord(f->FindCorrespondingTexCoord(v), uvw);
					//f->PurgePolyMappedTexCoords();
				}
			}
		}
	}
}


void Mesh::UVMapSelectedFaces_Plane()
{
	UpdateVertexReferenceWithSelectedBias();

    for (uint32 v = 0, vn = GetVertexCount(); v < vn; ++v)
    {
		Vertex *vertex = GetVertex(v);

		// Was this allocated and also marked by group bias?
		if (vertex && vertex->GetTmpRefs().size() > 0)
		{
			vec3_t xyz;
			vec3_t uvw;

			GetVertexArrayPos(vertex->mVertexIndex, xyz);
	
			uvw[0] = (xyz[0] > 0) ? xyz[0] : -xyz[0];	
			vec_t s = 0.025;
  
			while (uvw[0] > 1.0)
			{
				uvw[0] *= s;
				s *= 0.01;
			}
  
			uvw[1] = (xyz[1] > 0) ? xyz[1] : -xyz[1];
			s = 0.025;
  
			while (uvw[1] > 1.0)
			{
				uvw[1] *= s;
				s *= 0.01;
			}
  
			uvw[0] = 1.0 - uvw[0];
			uvw[1] = 1.0 - uvw[1];
			uvw[2] = 0.0f;	

			SetTexCoord(vertex->mTexCoordIndex, uvw);
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
			vertex->GetTmpRefs().clear();
	}

	/* Compute face normals */
	for (uint32 f = 0, fn = GetFaceCount(); f < fn; ++f)
	{
		Face *face = GetFace(f);

		// We only consider selected facets here... you filter updates here
		if (!face || !(group & (1<<face->mSmoothingGroup)))
		{
			faceNormals.pushBack(normal);  // For alignment purposes push a 'V0'
			continue;
		}

		for (uint32 v = 0, vn = face->mIndices.size(); v < vn; ++v)
		{
			Vertex *vertex = GetVertex(face->mIndices[v]);
			
			if (vertex)
				vertex->GetTmpRefs().pushBack(f);
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

		if (vertex->GetTmpRefs().size() == 0)
		{
			// This means it's not selected for update, or
			// has no faces containing this vertex
			continue;
		}

		for (uint32 j = 0, jn = vertex->GetTmpRefs().size(); j < jn; ++j)
		{
			if (vertex->GetTmpRefs()[j] == INDEX_INVALID)
			{
				freyjaPrintError("%s> ERROR Bad face reference\n", __func__);
				continue;
			}

			normal += faceNormals[vertex->GetTmpRefs()[j]];
		}

		normal.normalize();

		// FIXME: Doesn't use vertex normal remap ( which isn't used yet )
		SetNormal(v, normal.mVec);
    }
}


void Mesh::SelectedFacesFlipVertexNormals()
{
	UpdateVertexReferenceWithSelectedBias();  

	for (uint32 v = 0, vn = GetVertexCount(); v < vn; ++v)
    {
		Vertex *vertex = GetVertex(v);

		if (!vertex)
			continue;

		if (vertex->GetTmpRefs().size())
		{
			Vec3 normal;
			GetNormal(vertex->mNormalIndex, normal.mVec);
			normal = -normal;
			SetNormal(vertex->mNormalIndex, normal.mVec);
		}
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
			vertex->GetTmpRefs().clear();
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
				vertex->GetTmpRefs().pushBack(f);
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

		if (vertex->GetTmpRefs().size() == 0)
		{
			// This means it's not selected for update, or
			// has no faces containing this vertex
			continue;
		}

		for (uint32 j = 0, jn = vertex->GetTmpRefs().size(); j < jn; ++j)
		{
			if (vertex->GetTmpRefs()[j] == INDEX_INVALID)
			{
				freyjaPrintError("%s> ERROR Bad face reference\n", __func__);
				continue;
			}

			normal += faceNormals[vertex->GetTmpRefs()[j]];
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


void Mesh::UpdateBoundingVolume()
{
	vec3_t min;
	vec3_t max;
	vec3_t xyz;
	bool update = false;


	for ( uint32 i = 0, count = mVertices.size(); i < count; ++i )
	{
		if (!mVertices[i])
			continue;

		GetVertexArrayPos(mVertices[i]->mVertexIndex, xyz);

		// Adjust bounding volume in loop to handle gaps
		if (!mInitBoundingVol)
		{
			mInitBoundingVol = true;
			SetBBox(xyz, xyz);
			continue;
		}

		GetBBox(min, max);

		for (uint32 i = 0; i < 3; ++i)
		{
			if (xyz[i] < min[i])
			{
				min[i] = xyz[i];
				update = true;
			}
			else if (xyz[i] > max[i])
			{
				max[i] = xyz[i];
				update = true;
			}
		}

		if (update)
		{
			update = false;
			SetBBox(min, max);
		}
	}

	SetPosition(GetBoundingVolumeCenter());
}


////////////////////////////////////////////////////////////
// Transforms
////////////////////////////////////////////////////////////

void Mesh::Rotate(const Vec3 &v)
{
	RotateAboutPoint(GetBoundingVolumeCenter(), v);
}


void Mesh::RotateAboutPoint(const Vec3 &point, const Vec3 &v)
{
	Matrix t, r, t2, mat;
	// Rotate about bounding volume center instead of origin
	t.translate(point.mVec);
	r.rotate(v.mVec);
	t2.translate((-Vec3(point)).mVec);
	
	// FIXME: Fix the damn matrix backend to avoid such expensive
	//        processing here ( only want to transform once )
	TransformVertices(t2);
	TransformVertices(r);
	TransformVertices(t);
	
	// Transform normals by inverted rotation to stay correct
	Matrix nr;
	nr.rotate(v.mVec);
	nr.invert();
	TransformNormals(nr);
}


void Mesh::Scale(const Vec3 &v)
{
	ScaleAboutPoint(GetPosition(), v);
}


void Mesh::ScaleAboutPoint(const Vec3 &point, const Vec3 &v)
{
	Matrix t, s, t2, mat;

	// Scale about bounding volume center instead of origin
	t.translate(point.mVec);
	s.scale(v.mVec);
	t2.translate((-Vec3(point)).mVec);

	// FIXME: Fix the damn matrix backend to avoid such expensive
	//        processing here ( only want to transform once )
	TransformVertices(t2);
	TransformVertices(s);
	TransformVertices(t);
}


void Mesh::Translate(const Vec3 &v) 
{
	// Position (world)
	SetPosition(Vec3(v)+GetPosition());
	
	// Vertices
	TripleVec_Addition(mVertexPool, v.mVec);
	
	// Bounding Vol
	mBoundingVolume.mSphere.mCenter[0] += v.mVec[0];
	mBoundingVolume.mSphere.mCenter[1] += v.mVec[1];
	mBoundingVolume.mSphere.mCenter[2] += v.mVec[2];
	mBoundingVolume.mBox.mMax[0] += v.mVec[0];
	mBoundingVolume.mBox.mMax[1] += v.mVec[1];
	mBoundingVolume.mBox.mMax[2] += v.mVec[2];
	mBoundingVolume.mBox.mMin[0] += v.mVec[0];
	mBoundingVolume.mBox.mMin[1] += v.mVec[1];
	mBoundingVolume.mBox.mMin[2] += v.mVec[2];
}


void Mesh::SelectedFacesRotateUVMap(vec_t z)
{
	MSTL_MSG("FIXME");
}

void Mesh::SelectedFacesScaleUVMap(vec_t x, vec_t y)
{
	MSTL_MSG("FIXME");
}


void Mesh::SelectedFacesTranslateUVMap(vec_t x, vec_t y)
{
	MSTL_MSG("FIXME");
}


void Mesh::TransformFacesWithFlag(Face::Flags flag, Matrix &mat)
{
	MarkVerticesOfFacesWithFlag(flag, Vertex::fSelected2, true);
	TransformVerticesWithFlag(Vertex::fSelected2, mat);
}


void Mesh::TransformVerticesWithFlag(Vertex::Flags flag, Matrix &mat)
{
	vec_t *array = mVertexPool.get_array();

	for (uint32 i = 0, n = GetVertexCount(); i < n; ++i)
	{
		Vertex *v = GetVertex(i);
		
		if (v && v->mFlags & flag)
		{			
			mat.Multiply3v(array + v->mVertexIndex);
		}
	}
}


void Mesh::MarkVerticesOfFacesWithFlag(Face::Flags flag, 
									   Vertex::Flags mark, bool clear)
{
	for (uint32 i = 0, iCount = GetVertexCount(); i < iCount; ++i)
	{
		Vertex *v = GetVertex(i);

		if (!v) 
			continue;

		if (clear)
		{
			v->mFlags &= ~mark;
		}

		for (int32 j = 0, jCount = v->GetFaceRefs().size(); j < jCount; ++j)
		{
			Face *f = GetFace(v->GetFaceRefs()[j]);
			
			if (f && (f->mFlags & flag))
			{
				v->mFlags |= mark;
				break;
			}
		}
	}
}


void Mesh::ConvertAllFacesToTexCoordPloymapping()
{
	for (uint32 i = 0, n = GetFaceCount(); i < n; ++i)
	{
		ConvertFaceToTexCoordPloymapping(i);
	}
}


void Mesh::ConvertFaceToTexCoordPloymapping(index_t face)
{
	Face *f = GetFace(face);

	if (f)
	{
		if (f->mFlags & Face::fPolyMappedTexCoords &&
			f->mIndices.size() == f->mTexCoordIndices.size())
		{
		}
		else
		{
			f->mTexCoordIndices.clear();

			for (uint32 i = 0, n = f->mIndices.size(); i < n; ++i)
			{
				Vec3 t = GetVertexTexCoord(f->mIndices[i]);
				f->mTexCoordIndices.push_back(CreateTexCoord(t.mVec));
			}
		}
	}
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
