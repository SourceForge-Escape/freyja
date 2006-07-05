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


using namespace mstl;

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
		fNone        =  0,
		fHighlighted =  1,
		fMaterial    =  2,
		fSelected    =  4,
		fHidden      =  8
	} VertexFlags;

	Vertex(index_t vertex, index_t texcoord, index_t normal) :
		mFlags(fNone),
		mVertexIndex(vertex),
		mTexCoordIndex(texcoord),    
		mNormalIndex(normal),
		mColor(INDEX_INVALID),      
		mMaterial(INDEX_INVALID),
		mReserved1(0)
	{
	}

	Vertex() :
		mFlags(fNone),
		mVertexIndex(0),
		mTexCoordIndex(0),    
		mNormalIndex(0),
		mColor(INDEX_INVALID),      
		mMaterial(INDEX_INVALID),
		mReserved1(0)
	{
	}

	static size_t SerializedSize() 
	{
		return ( 1 + 4 * 6); 
	}

	bool Serialize(FreyjaFileWriter &w) 
	{ 
		freyja_file_chunk_t chunk;

		chunk.type = FREYJA_CHUNK_VERTEX;
		chunk.size = 0;
		chunk.flags = 0x0;
		chunk.version = 10;

		w.writeInt8U(mFlags);
		w.writeLong(mVertexIndex);
		w.writeLong(mTexCoordIndex);
		w.writeLong(mNormalIndex);
		w.writeLong(mColor);
		w.writeLong(mMaterial);
		w.writeLong(mReserved1);

		return true; 
	}

	byte mFlags;

	index_t mVertexIndex; // Pool storage of XYZ position

	index_t mTexCoordIndex;     // Pool storage of UV[W] coordinate

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
	Face() :
		mMaterial(0), // Always have a valid material
		mFlags(0x0),
		mSmoothingGroups(0), // Bitmap of groups 
		mIndices()
	{
	}

	~Face()
	{
	}

	void rotate(vec_t x, vec_t y, vec_t z);

	void scale(vec_t x, vec_t y, vec_t z);

	void translate(vec_t x, vec_t y, vec_t z);

	static size_t SerializedSize() { return 0; }
	bool Serialize(FreyjaFileWriter &w) { return true; }

	index_t mMaterial;
	uint32 mFlags;
	uint32 mSmoothingGroups; // bitmap
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

	Mesh *Copy();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Clone this mesh
	 ------------------------------------------------------*/


	void GetBBox(vec3_t min, vec3_t max)
	{
		HEL_VEC3_COPY(mBoundingVolume.mBox.mMin, min);
		HEL_VEC3_COPY(mBoundingVolume.mBox.mMax, max);
	}


	Vector3d GetBBoxCenter()
	{
		Vector3d u, v;
		u.Set(mBoundingVolume.mBox.mMax);
		v.Set(mBoundingVolume.mBox.mMin);
		return (u - v);
	}


	void GetColor(index_t colorIndex, vec4_t rgba)
	{	GetVec(mColorPool, 4, colorIndex, rgba);	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/


	uint32 GetFaceCount() { return mFaces.size(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/


	const char *GetName() { return mName; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get human readable name of mesh
	 ------------------------------------------------------*/


	void GetNormal(index_t normalIndex, vec3_t xyz)
	{ GetTripleVec(mNormalPool, normalIndex, xyz); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/


	uint32 GetNormalCount() { return mNormalPool.size(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/


	void GetSelectedVertices(Vector<index_t> &list);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Return a list of vertices flagged fSelected 
	 *        ( by index )
	 ------------------------------------------------------*/


	void GetTexCoord(index_t texCoordIndex, vec3_t uvw)
	{	GetTripleVec(mTexCoordPool, texCoordIndex, uvw);	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/


	uint32 GetTexCoordCount() { return mTexCoordPool.size(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/


	Vertex *GetVertex(index_t vertexIndex);
	/*------------------------------------------------------
	 * Pre  : vertexIndex is valid for this mesh
	 * Post : Return vertex if it exists or NULL
	 ------------------------------------------------------*/


	Vector3d GetVertexNormal(index_t idx);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/


	Vector3d GetVertexPosition(index_t idx);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/


	Vector3d GetVertexTexCoord(index_t idx);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/


	uint32 GetVertexCount() { return mVertices.size(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/


	void GetVertexPos(index_t vertexIndex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void GetVertexArrayPos(index_t vertexIndex, vec3_t xyz)
	{	GetTripleVec(mVertexPool, vertexIndex, xyz);	}
	/*------------------------------------------------------
	 * Pre  : NOTE This is not the same as GetVertexClassPos
	 *        This is the ith point stored in the point array
	 * Post : 
	 ------------------------------------------------------*/


	uint32 GetWeightCount() { return mWeights.size(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/


	bool Serialize(FreyjaFileWriter &w);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Serializes the mesh to diskfile as a chunk
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void SetName(const char *name)
	{
		strncpy(mName, name, 31);
		mName[31] = 0;
	}

	// 'Location' interface
	void GetPosition(vec3_t xyz) { mPosition.Set(xyz); }
	void SetPosition(const vec3_t xyz) { HEL_VEC3_COPY(xyz, mPosition.mVec); }
	void SetPositionX(vec_t x) { mPosition.mVec[0] = x; }
	void SetPositionY(vec_t y) { mPosition.mVec[1] = y; }
	void SetPositionZ(vec_t z) { mPosition.mVec[2] = z; }

	void SetDeltaPosition(const vec3_t xyz) 
	{
		mPosition.mVec[0] += xyz[0]; 
		mPosition.mVec[1] += xyz[1];
		mPosition.mVec[2] += xyz[2];
	}

	void SetDeltaPositionX(vec_t x) { mPosition.mVec[0]+=x; }
	void SetDeltaPositionY(vec_t y)	{ mPosition.mVec[1]+=y; }
	void SetDeltaPositionZ(vec_t z) { mPosition.mVec[2]+=z; }

	// 'Rotation' Euler angle interface 
	void GetRotation(vec3_t xyz) { mRotation.Set(xyz); }
	void SetRotationQuat(const vec4_t wxyz) {}
	void SetDeltaRotationQuat(const vec4_t wxyz) {}

	// 'Rotation' Quaternion interface 
	void SetRotation(const vec3_t xyz) {}
	void SetRotationX(vec_t x) {}
	void SetRotationY(vec_t y) {}
	void SetRotationZ(vec_t z) {}
	void SetDeltaRotation(const vec3_t xyz) {}	
	void SetDeltaRotationX(vec_t x) {}
	void SetDeltaRotationY(vec_t y) {}
	void SetDeltaRotationZ(vec_t z) {}

	// 'Size' interface
	void SetScale(const vec3_t xyz) {}	
	void SetScaleX(vec_t x) {}
	void SetScaleY(vec_t y) {}
	void SetScaleZ(vec_t z) {}
	void SetDeltaScale(const vec3_t xyz) {}	
	void SetDeltaScaleX(vec_t x) {}
	void SetDeltaScaleY(vec_t y) {}
	void SetDeltaScaleZ(vec_t z) {}


	bool Serialize(FreyjaFileReader &r)
	{
		BUG_ME("AddWeight Not Implemented", __FILE__, __LINE__);
		return false;
	}


	Mesh *CopyPartial(Vector<index_t> &list);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 *
	 * Pre  : mesh is a valid mesh
	 *        list is a valid vertex list of mesh
	 * Post : Returns newly allocated mesh
	 *        that is a copy of mesh 
	 *        determined by the list
	 -----------------------------------------*/

	void Merge(Mesh *meshA, Vector<index_t> &list);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 *
	 * Pre  : meshA is a valid mesh
	 *        list is a valid vertex list of meshA
	 *  
	 * Post : Makes one mesh of of two, by
	 *        merging vertices in list with this
	 *        and removing from A 
	 *
	 *        Passing all vertices makes a 
	 *        full merge
	 -----------------------------------------*/

	Mesh *Split(Vector<index_t> &list);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 *
	 * Pre  : mesh is a valid mesh
	 *        list is a valid vertex list of mesh
	 * Post : Removes the vertices in list
	 *        from mesh, then generates a new
	 *        mesh ( splits one mesh into two )
	 *        The newly made mesh is returned
	 -----------------------------------------*/    


	Mesh *CsgUnion(Mesh *a);
	Mesh *CsgIntersection(Mesh *a);
	Mesh *CsgDifference(Mesh *a);
	/*------------------------------------------------------
	 * Pre  : Don't count on more than simple vertex culling now
	 *
	 * Post : OPERATION on TYPE object A and B (this) in model
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


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	Mesh *Cut();

	void Paste(Mesh *model);

	void Rotate(vec_t x, vec_t y, vec_t z);

	void RotateAboutPoint(vec3_t point, vec_t x, vec_t y, vec_t z);

	void Scale(vec_t x, vec_t y, vec_t z);

	void Translate(vec_t x, vec_t y, vec_t z);

	void AddWeight(index_t vertexIndex, vec_t weight, index_t bone) 
	{
		BUG_ME("AddWeight Not Implemented", __FILE__, __LINE__);
	}


	void SetColor(index_t colorIndex, const vec4_t rgba)
	{
		// FIXME: Check flags for rgba or rgb
		SetVec(mColorPool, 4, colorIndex, (vec_t*)rgba);
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


	Face *GetFace(index_t idx)
	{
		Face **array = mFaces.getVectorArray();
		
		if ( idx < mFaces.size() )
		{
			return array[idx];
		}

		return NULL;
	}


	index_t CreateFace()
	{
		Face **array = mFaces.getVectorArray();
		Face *face = new Face();

		for ( uint32 i = 0, count = mFaces.size(); i < count; ++i )
		{
			if (array[i] == NULL)
			{
				array[i] = face;
				return i;
			}
		}

		mFaces.pushBack(face);
		return mFaces.size() - 1;
	}


	vec_t *GetVertexArray()
	{
		return mVertexPool.getVectorArray();
	}

	index_t CreateVertex(const vec3_t xyz)
	{
		vec3_t nxyz = {0.0f, 1.0f, 0.0f};
		vec3_t uvw = {0.5f, 0.5f, 0.0f};
		return CreateVertex(xyz, uvw, nxyz);
	}

	void SetMaterial(index_t idx)
	{
		mMaterialIndex = idx;
	}

	index_t CreateVertex(const vec3_t xyz, const vec3_t uvw, const vec3_t nxyz)
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

		mVertices.pushBack(vert);
		return mVertices.size() - 1;
	}


	void DeleteVertex(index_t vertexIndex)
	{
		BUG_ME("DeleteVertex Not Implemented", __FILE__, __LINE__);
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

	void ClampAllTexCoords()
	{
		ClampAllTexCoords(0.0f, 1.0f);
	}

	void ClampAllTexCoords(vec_t min, vec_t max)
	{
		ClampVecValues(mTexCoordPool, min, max);
	}

	void SetBBox(const vec3_t min, const vec3_t max)
	{
		HEL_VEC3_COPY(min, mBoundingVolume.mBox.mMin);
		HEL_VEC3_COPY(max, mBoundingVolume.mBox.mMax);
	}


private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////

	bool SerializePool(FreyjaFileWriter &w, 
					   Vector<vec_t> &v, mstl::stack<index_t> &s);


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	void ClampVecValues(Vector<vec_t> &v, vec_t min, vec_t max)
	{
		vec_t *array = v.getVectorArray();
		vec_t r;

		for ( uint32 i = 0, n = v.size(); i < n; ++i )
		{
			r = array[i];

			if (r < min)
				array[i] = min;
			else if (r > max)
				array[i] = max;
		}
	}

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


	void SetVec(Vector<vec_t> &v, uint32 n, index_t tIndex, vec_t *u)
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

	// NOTE: If this is a sparse array ( most of these are ) you operate
	// on more than actually used data, however it doesn't use the class
	// index system ( so it likely still faster in general case )
	void TripleVec_Addition(Vector<vec_t> &v, const vec3_t xyz)
	{
		uint32 i, idx, size = v.size();
		vec_t *array = v.getVectorArray();

		for ( i = 0; i < size; ++i )
		{
			idx = i * 3;
			array[idx  ] += xyz[0];  
			array[idx+1] += xyz[1];  
			array[idx+2] += xyz[2];  
		}
	}

	void TripleVec_Transform(Vector<vec_t> &v, Matrix &mat)
	{
		uint32 i, size = v.size();
		vec_t *array = v.getVectorArray();

		for ( i = 0; i < size; i += 3 )
		{
			mat.Multiply3v(array + i);
		}
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


	const static uint32 mNameSize = 32;

	static index_t mNextUID;  // UIDs outside of owner's array index

	char mName[mNameSize];  // Human readable name of mesh

	index_t mUID;

	uint32 mFlags;

	index_t mMaterialIndex;

	Vector3d mPosition;

	Vector3d mRotation;  // Store as Euler Angles for 'Size' interface

	Vector3d mScale;

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
