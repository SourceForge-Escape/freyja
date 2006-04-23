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
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_FREYJAMESH - Builds FreyjaMesh class as a unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.10.22:
 * Mongoose - Created, expanded from Freyja class
 ==========================================================================*/

#ifndef GUARD__FREYJA_MONGOOSE_FREYJAMESH_H_
#define GUARD__FREYJA_MONGOOSE_FREYJAMESH_H_

#include <hel/math.h>
#include <hel/BoundingVolume.h>
#include <mstl/Vector.h>
#include <mstl/stack.h>
#include "freyja.h"
#include "FreyjaFileWriter.h"
#include "FreyjaFileReader.h"


namespace freyja {

class Weight
{
public:

	static size_t SerializedSize() { return 0; }
	bool Serialize(FreyjaFileWriter &w) { return true; }
	
	index_t mVertexIndex;
	index_t mBoneIndex;
	vec_t mWeight;
};


// This class allows polymapping Normals, UVs, etc
class Vertex
{
public:
	typedef enum {
		fNone       = 0,
		fRBGAColor  = 1,
		fMaterial   = 2
	} VertexFlags;

	static size_t SerializedSize() { return 0; }
	bool Serialize(FreyjaFileWriter &w) { return true; }

	unsigned char mFlags;

	index_t mVertexIndex; // Pool storage of XYZ position

	index_t mUVIndex;     // Pool storage of UVW coordinate

	index_t mNormalIndex; // Pool storage of XYZ normal

	index_t mColor;       // Pool storage of color

	index_t mMaterial;
	
	index_t mReserved1;
};


class UVMap
{
public:
	void rotate(vec_t x, vec_t y);

	void scale(vec_t x, vec_t y);

	void translate(vec_t x, vec_t y);

	static size_t SerializedSize() { return 0; }

	Vector<index_t> mFaceIndices;   /* Contains UVs composing group
									 * either polymapped or by vertex */
};


class Face
{
public:
	void rotate(vec_t x, vec_t y, vec_t z);

	void scale(vec_t x, vec_t y, vec_t z);

	void translate(vec_t x, vec_t y, vec_t z);

	static size_t SerializedSize() { return 0; }
	bool Serialize(FreyjaFileWriter &w) { return true; }

	index_t mMaterial;
	uint32 mFlags;
	uint32 mSmoothingGroups;
};

class Triangle : public Face
{
public:
	index_t mIndices[3];
};

class Quad : public Face
{
public:
	index_t mIndices[4];
};

class Polygon : public Face
{
public:
	Vector<index_t> mIndices;
};


class Mesh
{
public:	
	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Mesh();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Mesh
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.10.22: 
	 * Mongoose - Created, from FreyjaMesh in Freyja
	 ------------------------------------------------------*/

	~Mesh();
	/*------------------------------------------------------
	 * Pre  : Mesh object is allocated
	 * Post : Deconstructs an object of Mesh
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.10.22: 
	 * Mongoose - Created, from FreyjaMesh in Freyja
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	bool Serialize(FreyjaFileWriter &w)
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
		chunk.size += mVertexPool.size() * 4;
		chunk.size += mFreedVertices.size() * 4;
		chunk.size += mNormalPool.size() * 4;
		chunk.size += mFreedNormals.size() * 4;
		chunk.size += mColorPool.size() * 4;
		chunk.size += mFreedColors.size() * 4;
		chunk.size += mTexCoordPool.size() * 4;
		chunk.size += mFreedTexCoords.size() * 4;
		chunk.size += mFaces.size() * Face::SerializedSize();
		chunk.size += mVertices.size() * Vertex::SerializedSize();
		chunk.size += mWeights.size() * Weight::SerializedSize();

		/* Write chunk header to diskfile */
		w.writeLong(FREYJA_CHUNK_MESH);
		w.writeLong(chunk.size);
		w.writeLong(chunk.flags);
		w.writeLong(chunk.version);

		/* Write chunk data to diskfile */
		w.writeLong(mUID);
		w.writeLong(mFlags);
		w.writeLong(mMaterialIndex);
		w.writeFloat32(mPosition[0]);
		w.writeFloat32(mPosition[1]);
		w.writeFloat32(mPosition[2]);
		w.writeFloat32(mRotation[0]);
		w.writeFloat32(mRotation[1]);
		w.writeFloat32(mRotation[2]);
		w.writeFloat32(mScale[0]);
		w.writeFloat32(mScale[1]);
		w.writeFloat32(mScale[2]);

		SerializePool(w, mVertexPool, mFreedVertices);
		SerializePool(w, mNormalPool, mFreedNormals);
		SerializePool(w, mColorPool, mFreedColors);
		SerializePool(w, mTexCoordPool, mFreedTexCoords);

		for ( uint32 i = 0; i < mFaces.size(); ++i )
		{
			if ( mFaces[i] ) 
				mFaces[i]->Serialize(w);
		}

		for ( uint32 i = 0; i < mVertices.size(); ++i )
		{
			if ( mVertices[i] ) 
				mVertices[i]->Serialize(w);
		}

		for ( uint32 i = 0; i < mWeights.size(); ++i )
		{
			if ( mWeights[i] ) 
				mWeights[i]->Serialize(w);
		}

		return true;
	}

	bool SerializePool(FreyjaFileWriter &w, 
					   Vector<vec_t> &v, mstl::stack<index_t> &s)
	{
		mstl::stack<index_t> copy; // We don't really care about order

		for ( uint32 i = 0; i < v.size(); ++i )
		{
			w.writeFloat32(v[i]);
		}
		
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

	Mesh *Copy();


	uint32 GetVertexCount() { return 0; }


	uint32 GetTexCoordCount() { return 0; }


	uint32 GetNormalCount() { return 0; }


	uint32 GetVertexWeightCount() { return 0; }


	uint32 GetFaceCount() { return 0; }


	void GetColor(index_t colorIndex, vec4_t rgba)
	{
		// FIXME: Check flags for rgba or rgb
		GetVec(mColorPool, 4, colorIndex, rgba);
	}


	void GetNormal(index_t normalIndex, vec3_t xyz)
	{
		GetTripleVec(mNormalPool, normalIndex, xyz);
	}


	void GetTexCoord(index_t texCoordIndex, vec3_t uvw)
	{
		// FIXME: Check flags for uv or uvw
		GetTripleVec(mTexCoordPool, texCoordIndex, uvw);
	}


	void GetVertex(index_t vertexIndex, vec3_t xyz)
	{
		GetTripleVec(mVertexPool, vertexIndex, xyz);
	}


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	// 'Location' interface
	void SetPosition(vec3_t xyz) { Translate(xyz[0], xyz[1], xyz[2]); }
	void SetPositionX(vec_t x) { Translate(x, mPosition[1], mPosition[2]); }
	void SetPositionY(vec_t y) { Translate(mPosition[0], y, mPosition[2]); }
	void SetPositionZ(vec_t z) { Translate(mPosition[0], mPosition[1], z); }
	void SetDeltaPosition(vec3_t xyz) 
	{Translate(mPosition[0]+xyz[0], mPosition[1]+xyz[1], mPosition[2]+xyz[2]);}
	void SetDeltaPositionX(vec_t x) 
	{ Translate(mPosition[0]+x, mPosition[1], mPosition[2]); }
	void SetDeltaPositionY(vec_t y)
	{ Translate(mPosition[0], mPosition[1]+y, mPosition[2]); }
	void SetDeltaPositionZ(vec_t z)
	{ Translate(mPosition[0], mPosition[1], mPosition[2]+z); }

	// 'Rotation' Euler angle interface 
	void SetRotationQuat(vec4_t wxyz) {}
	void SetDeltaRotationQuat(vec4_t wxyz) {}

	// 'Rotation' Quaternion interface 
	void SetRotation(vec3_t xyz) {}
	void SetRotationX(vec_t x) {}
	void SetRotationY(vec_t y) {}
	void SetRotationZ(vec_t z) {}
	void SetDeltaRotation(vec3_t xyz) {}	
	void SetDeltaRotationX(vec_t x) {}
	void SetDeltaRotationY(vec_t y) {}
	void SetDeltaRotationZ(vec_t z) {}

	// 'Size' interface
	void SetScale(vec3_t xyz) {}	
	void SetScaleX(vec_t x) {}
	void SetScaleY(vec_t y) {}
	void SetScaleZ(vec_t z) {}
	void SetDeltaScale(vec3_t xyz) {}	
	void SetDeltaScaleX(vec_t x) {}
	void SetDeltaScaleY(vec_t y) {}
	void SetDeltaScaleZ(vec_t z) {}


	bool Serialize(FreyjaFileReader &r)
	{
		// FIXME
		return false;
	}


	Mesh *CsgUnion(Mesh *a, Mesh *b);
	Mesh *CsgIntersection(Mesh *a, Mesh *b);
	Mesh *CsgDifference(Mesh *a, Mesh *b);
	/*------------------------------------------------------
	 * Pre  : Don't count on more than simple vertex culling now
	 *
	 * Post : OPERATION on TYPE object A and B in model
	 *
	 *        A and B are perserved, creates new object C 
	 *        where: C = A OPERATION B
	 *
	 *        UNION        : C = A u B
	 *                       if A eq B then copy of A is made
	 *
	 *        INTERSECTION : C = A n B
	 *                       if A eq B then copy of A is made
	 *
	 *        DIFFERENCE   : C = A - B
	 *                       B culls A, order matters
	 *                       if A eq B then undefined behavior
	 *
	 *        Returns C's index
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.04.08:
	 * Mongoose - Created with new generic API based on mtk
	 ------------------------------------------------------*/


	Mesh *Cut();

	void Paste(Mesh *model);

	void Rotate(vec_t x, vec_t y, vec_t z);

	void RotateAboutPoint(vec3_t point, vec_t x, vec_t y, vec_t z);

	void Scale(vec_t x, vec_t y, vec_t z);

	void Translate(vec_t x, vec_t y, vec_t z);



	void SetColor(index_t colorIndex, const vec4_t rgba)
	{
		// FIXME: Check flags for rgba or rgb
		SetVec(mColorPool, 4, colorIndex, rgba);
	}


	void SetNormal(index_t normalIndex, const vec3_t xyz)
	{
		SetTripleVec(mNormalPool, normalIndex, xyz);
	}


	void SetTexCoord(index_t texCoordIndex, const vec3_t uvw)
	{
		// FIXME: Check flags for uv or uvw
		SetTripleVec(mTexCoordPool, texCoordIndex, uvw);
	}


	void SetVertex(index_t vertexIndex, const vec3_t xyz)
	{
		SetTripleVec(mVertexPool, vertexIndex, xyz);
	}


	bool WeldTexCoords(index_t a, index_t b)
	{
		Face *face;


		// Make all polygons referencing A point to B
		for (uint32 i = mFaces.begin(); i < mFaces.end(); ++i)
		{
			face = mFaces[i];

			if (face)
			{
				//FIXME face->WeldTexCoords(a, b);
			}
		}

		// Mark A as unused in the texcoord pool
		mFreedTexCoords.push(a);

		return true;
	}


	bool WeldVertices(index_t a, index_t b)
	{
		Face *face;
		unsigned int i;


		// Make all polygons referencing A point to B
		for (i = mFaces.begin(); i < mFaces.end(); ++i)
		{
			face = mFaces[i];

			if (face)
			{
				//FIXME face->WeldVertices(a, b);
			}
		}

		// Mark A as unused in the vertex pool
		mFreedVertices.push(a);

		return true;
	}


private:

	index_t AddVec(Vector<vec_t> &v, mstl::stack<index_t>&f, uint32 n, vec_t *u)
	{
		if (f.empty())
		{
			return AppendVec(v, n, u);
		}

		index_t tIndex = f.pop();

		SetVec(v, n, tIndex, u);

		return tIndex;
	}


	index_t AppendVec(Vector<vec_t> &v, uint32 n, vec_t *u)
	{
		for ( uint32 i = 0; i < n; ++i)
		{
			v.pushBack(u[i]);
		}

		return (( v.end() / n ) - 1);
	}


	void GetVec(Vector<vec_t> &v, uint32 n, index_t tIndex, vec_t *u)
	{
		tIndex *= n;

		if (tIndex > v.end())
			return;

		vec_t *array = v.getVectorArray();

		for ( uint32 i = 0; i < n; ++i)
		{
			u[i] = array[tIndex + i];
		}
	}


	void SetVec(Vector<vec_t> &v, uint32 n, index_t tIndex, const vec_t *u)
	{
		tIndex *= n;

		if (tIndex > v.end())
			return;

		vec_t *array = v.getVectorArray();

		for ( uint32 i = 0; i < n; ++i)
		{
			array[tIndex + i] = u[i];
		}
	}


	index_t AddTripleVec(Vector<vec_t> &v, mstl::stack<index_t> &f, vec3_t xyz)
	{
		if (f.empty())
		{
			return AppendTripleVec(v, xyz);
		}

		index_t tIndex = f.pop();

		SetTripleVec(v, tIndex, xyz);

		return tIndex;
	}


	index_t AppendTripleVec(Vector<vec_t> &v, vec3_t xyz)
	{
		v.pushBack(xyz[0]);
		v.pushBack(xyz[1]);
		v.pushBack(xyz[2]);

		return (( v.end() / 3 ) - 1);
	}


	void GetTripleVec(Vector<vec_t> &v, index_t tIndex, vec3_t xyz)
	{
		tIndex *= 3;

		if (tIndex > v.end())
			return;

		vec_t *array = v.getVectorArray();

		xyz[0] = array[tIndex    ];
		xyz[1] = array[tIndex + 1];
		xyz[2] = array[tIndex + 2];
	}


	void SetTripleVec(Vector<vec_t> &v, index_t tIndex, const vec3_t xyz)
	{
		tIndex *= 3;

		if (tIndex > v.end())
			return;

		vec_t *array = v.getVectorArray();

		array[tIndex    ] = xyz[0];
		array[tIndex + 1] = xyz[1];
		array[tIndex + 2] = xyz[2];
	}

	static index_t mNextUID;

	index_t mUID;

	uint32 mFlags;

	index_t mMaterialIndex;

	vec3_t mPosition;

	vec3_t mRotation;  // Set by quaternion, store as euler for Size interface

	vec3_t mScale;

	BoundingBoxCombo mBoundingVolume;

	Vector<vec_t> mVertexPool;
	mstl::stack<index_t> mFreedVertices;

	Vector<vec_t> mNormalPool;
	mstl::stack<index_t> mFreedNormals;

	Vector<vec_t> mColorPool;
	mstl::stack<index_t> mFreedColors;

	Vector<vec_t> mTexCoordPool;
	mstl::stack<index_t> mFreedTexCoords;

	Vector<Face *> mFaces;

	Vector<Vertex *> mVertices;

	Vector<Weight *> mWeights;
};


class ObjectManager
{
public:

	//mDeletedObjectCount;
};

}


#endif
