/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : FreyjaMesh
 * License : No use w/o permission (C) 2004-2006 Mongoose
 * Comments: This is the freyja::Mesh and classes that compose it.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_MESH - Builds Mesh class as a unit test 
 *
 *-- Todo ---------------------------------------------------
 *
 * o It might help to move some clutter out of this header  =p
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
#include <hel/Ray.h>
#include <mstl/Vector.h>
#include <mstl/stack.h>
#include <mstl/SystemIO.h>

#include "Track.h"

#include "freyja.h"


using namespace mstl;

namespace freyja {

class Weight
{
public:
	
	Weight(index_t vertex, index_t bone, vec_t weight) : 
		mVertexIndex(vertex),
		mBoneIndex(bone),
		mWeight(weight)
	{
		//freyjaPrintMessage("w %i %i %f", mVertexIndex, mBoneIndex, mWeight);
	}

	Weight() : mVertexIndex(INDEX_INVALID),
			   mBoneIndex(INDEX_INVALID),
			   mWeight(0.0f)
	{
		//freyjaPrintMessage("w %i %i %f", mVertexIndex, mBoneIndex, mWeight);
	}

	~Weight()
	{
	}


	static size_t SerializedSize() { return 4*2 + 4; }

	bool Serialize(SystemIO::FileReader &r) 
	{ 
		mVertexIndex = r.ReadLong();
		mBoneIndex = r.ReadLong();
		mWeight = r.ReadFloat32();
		return true; 
	}

	bool Serialize(SystemIO::FileWriter &w) 
	{ 
		w.WriteLong(mVertexIndex);
		w.WriteLong(mBoneIndex);
		w.WriteFloat32(mWeight);
		return true;
	}

	index_t mVertexIndex;
	index_t mBoneIndex;
	vec_t mWeight;
};


class Vertex
{
public:
	typedef enum {
		fNone        =  0,
		fHighlighted =  1,
		fMaterial    =  2,
		fSelected    =  4,
		fHidden      =  8,
		fRayHit      = 16
	} VertexFlags;

	Vertex(index_t vertex, index_t texcoord, index_t normal) :
		mFlags(fNone),
		mVertexIndex(vertex),
		mTexCoordIndex(texcoord),    
		mNormalIndex(normal),
		mMaterial(INDEX_INVALID),
		mFaceRefs(),
		mTmpRefs()
	{
	}

	Vertex() :
		mFlags(fNone),
		mVertexIndex(INDEX_INVALID),
		mTexCoordIndex(INDEX_INVALID),    
		mNormalIndex(INDEX_INVALID),
		mMaterial(INDEX_INVALID),
		mFaceRefs(),
		mTmpRefs()
	{
	}

	Vertex(const Vertex &v) :
		mFlags(v.mFlags),
		mVertexIndex(v.mVertexIndex),
		mTexCoordIndex(v.mTexCoordIndex),    
		mNormalIndex(v.mNormalIndex),
		mMaterial(v.mMaterial),
		mFaceRefs(v.mFaceRefs),
		mTmpRefs(v.mTmpRefs)
	{
	}

	static size_t SerializedSize() 
	{
		return ( 1 + 4 * 6); 
	}

	bool Serialize(SystemIO::FileWriter &w) 
	{ 
		freyja_file_chunk_t chunk;

		chunk.type = FREYJA_CHUNK_VERTEX;
		chunk.size = SerializedSize();
		chunk.flags = 0x0;
		chunk.version = 10;

		w.WriteInt8U(mFlags);
		w.WriteLong(mVertexIndex);
		w.WriteLong(mTexCoordIndex);
		w.WriteLong(mNormalIndex);
		w.WriteLong(mMaterial);

		// No use in storing face references to disk!

		return true; 
	}

	Vector<index_t> &GetFaceRefs() { return mFaceRefs; }

	Vector<index_t> &GetTmpRefs() { return mTmpRefs; }


	byte mFlags;                /* State flags */

	index_t mVertexIndex; 		/* Pool storage of XYZ position */

	index_t mTexCoordIndex;     /* Pool storage of UV[W] coordinate */

	index_t mNormalIndex; 		/* Pool storage of XYZ normal */

	index_t mMaterial;          /* Material index */

private:

	Vector<index_t> mFaceRefs;  /* Face references */

	Vector<index_t> mTmpRefs;   /* Face references used for special methods */
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

	typedef enum {
		fNone        =  0,
		fHighlighted =  1,
		fMaterial    =  2,
		fSelected    =  4,
		fHidden      =  8,
		fRayHit      = 16,
		fPolyMappedTexCoords = 32,
		fPolyMappedNormals = 64
	} Flags;


	Face() :
		mFlags(fNone),
		mSmoothingGroup(0),
		mColor(0),
		mMaterial(0), // Always have a valid material Id
		mIndices(),
		mTexCoordIndices(),
		mNormalsIndices()
	{
	}

	~Face()
	{
	}


	static const char *GetChunkType() { return "Face"; }

	static uint32 GetChunkVersion() { return 9500; /* 0.9.5-00 */ }


	void AppendNormal(index_t idx)
	{
		mFlags |= fPolyMappedNormals;
		mNormalsIndices.pushBack(idx);
	}


	void AppendTexCoord(index_t idx)
	{
		mFlags |= fPolyMappedTexCoords;
		mTexCoordIndices.pushBack(idx);
	}


	void AppendVertex(index_t idx)
	{
		// FIXME: Now you should append this Face's id to the vertex's polyref
		// FIXME: Might want to cache edge map too
		mIndices.pushBack(idx);
	}

	index_t FindCorrespondingTexCoord(index_t vertexIndex)
	{
		for (uint32 i = 0, n = mIndices.size(); i < n; ++i)
		{
			if (mIndices[i] == vertexIndex)
			{
				return mTexCoordIndices[i];
			}
		}
		
		return INDEX_INVALID;
	}


	void PurgePolyMappedTexCoords()
	{
		mFlags |= fPolyMappedTexCoords;
		mFlags ^= fPolyMappedTexCoords;
		mTexCoordIndices.clear();
	}

	void ClearSmoothingGroup() { mSmoothingGroup = 0; }
	void SetSmoothingGroup(uint32 g) { mSmoothingGroup = g; }
	/*------------------------------------------------------
	 * Pre  : -1 < <g> < 33 ( Relates to power of 2, bitmap ) 
	 *        Groups:  1-24,    Normal smoothing groups
	 *                25-32,    Reserved for special use
	 *
	 *        NOTE Groups/Colors may later use crease flags in other
	 *             'geometery types'.
	 *
	 * Post : 
	 *
	 ------------------------------------------------------*/

	size_t SerializedSize() { return 0; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool Serialize(SystemIO::FileWriter &w) { return false; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/


	byte mFlags;                      /* Options flags */

	byte mSmoothingGroup;             /* Group bit index */

	byte mColor;                      /* Reserved use for future algorithms */

	index_t mMaterial;                /* Material index */

	Vector<index_t> mIndices;         /* Indices to the Vertex *class */

	Vector<index_t> mTexCoordIndices; /* Only used with fPolyMappedTexCoords */

	Vector<index_t> mNormalsIndices;  /* Only used with fPolyMappedNormals */
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

	Mesh(const Mesh &mesh);
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

	// Testing embedded keyframing as public attribute, so bare with us...
	// these methods only support one 'animation' atm, but forcing interface
	// use makes it easy to fix that later =p
	TransformTrack &GetTransformTrack(uint32 track) { return mTrack; }
	TransformTrack mTrack;

	index_t NewVertexAnimTrack() {return 0;}
	VertexAnimTrack &GetVertexAnimTrack(uint32 track) {return mVertexAnimTrack;}
	VertexAnimTrack mVertexAnimTrack;

	vec_t mBlendVerticesTime;       /* Last time this was updated */
	Vector<vec_t> mBlendVertices;   /* Skeletal vertex blending use  */

	vec_t *GetBlendVerticesArray() { return mBlendVertices.get_array(); }

	uint32 GetBlendVerticesCount() { return mBlendVertices.size(); }

	void ResetBlendVertices()
	{
		vec_t *array = mBlendVertices.get_array();

		if (array)
			memset(array, 0, mBlendVertices.size()*sizeof(vec_t));
	}

	void SyncBlendVertices()
	{
		if (mBlendVertices.size() < mVertexPool.size())
		{
			mBlendVertices.resize(mVertexPool.size());
		}
	}

	// Getting ready to test arrays / VBO with dynamic backend, which
	// requires caching here in the backend to be effective
	//Vector<index_t> mTriangleList;
	//Vector<index_t> mQuadList;

	////////////////////////////////////////////////////////////
	// Bounding Volume properties
	////////////////////////////////////////////////////////////

	void SetBBox(const vec3_t min, const vec3_t max)
	{
		// Update bbox
		HEL_VEC3_COPY(min, mBoundingVolume.mBox.mMin);
		HEL_VEC3_COPY(max, mBoundingVolume.mBox.mMax);

		// Update sphere
		Vector3d vMin(min), vMax(max);
		Vec3 origin = ( vMin + vMax ) * 0.5; // midpoint
		vec_t radius = Vec3(origin - vMax).magnitude();

		origin.Get(mBoundingVolume.mSphere.mCenter);
		mBoundingVolume.mSphere.mRadius = radius;
	}

	void GetBBox(vec3_t min, vec3_t max)
	{
		HEL_VEC3_COPY(mBoundingVolume.mBox.mMin, min);
		HEL_VEC3_COPY(mBoundingVolume.mBox.mMax, max);
	}


	Vec3 GetBoundingVolumeCenter()
	{
		return Vec3(mBoundingVolume.mSphere.mCenter);
	}


	vec_t GetBoundingVolumeRadius()
	{
		return mBoundingVolume.mSphere.mRadius;
	}


	Vec3 GetBBoxCenter()
	{
		return GetBoundingVolumeCenter();
	}

	void SetFaceSmoothingGroup(index_t face, uint32 group);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void SelectedFacesMarkSmoothingGroup(uint32 group, bool t);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void GroupedFacesGenerateVertexNormals(uint32 group);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void SelectedFacesGenerateVertexNormals();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	static const char *GetChunkType() { return "Mesh"; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	static uint32 GetChunkVersion() { return 9500; /* 0.9.5-00 */ }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void CollapseEdge(index_t faceIndex,uint32 a, uint32 b, uint32 c, uint32 d);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void SplitFace(index_t faceIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void AssignGroupToSelectedFaces(uint32 groups);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : All faces mark selected are assigned to group 
	 ------------------------------------------------------*/

	void SetGroupsFaceSelected(uint32 groups);
	/*------------------------------------------------------
	 * Pre  : <groups> is a valid bitmap of groups
	 * Post : All faces in group(s) are marked selected
	 *
	 ------------------------------------------------------*/

	void ClearGroupsFaceSelected(uint32 groups);
	/*------------------------------------------------------
	 * Pre  : <groups> is a valid bitmap of groups
	 * Post : All faces in group(s) are marked unselected
	 *
	 ------------------------------------------------------*/

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


	void GetVertexPos(index_t vertexIndex, vec3_t xyz)
	{
		Vertex *vert;
		if (vertexIndex < mVertices.size() && (vert = mVertices[vertexIndex]))
			GetVertexArrayPos(vert->mVertexIndex, xyz);
	}
	void SetVertexPos(index_t vertexIndex, vec3_t xyz)
	{
		Vertex *vert;
		if (vertexIndex < mVertices.size() && (vert = mVertices[vertexIndex]))
			SetVertexArrayPos(vert->mVertexIndex, xyz);
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void GetVertexArrayPos(index_t vertexIndex, vec3_t xyz);
	void SetVertexArrayPos(index_t vertexIndex, const vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : NOTE This is not the same as GetVertexClassPos
	 *        This is the ith point stored in the point array
	 * Post : 
	 ------------------------------------------------------*/

	Weight *GetWeight(index_t w) 
	{ return (w < mWeights.size()) ? mWeights[w] : NULL; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	uint32 GetWeightCount() { return mWeights.size(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/


	bool Intersect(Ray &r, vec_t &t);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if 'hit', and <t> the time along ray
	 *        This checks the bsphere, then bbox for collision.
	 ------------------------------------------------------*/

	bool IntersectPerFace(Ray &r, vec_t &t);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if 'hit', and <t> the time along ray
	 *        This checks per face for the final validation on 
	 *        top of just bounding volume checks.
	 ------------------------------------------------------*/
	
	bool IntersectClosestFace(Ray &r, int &face0) 
	{ return IntersectFaces(r, face0, false); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : <face0> First face encountered along ray, or -1 if DNE.
	 *        Always sets fRayHit flag on face0.
	 *        Clears old fRayHit results on all other faces.
	 ------------------------------------------------------*/

	void SelectVerticesByBox(Vec3 &min, Vec3 &max);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	bool IntersectClosestVertex(Ray &r, int &vertex0, vec_t radius);
	/*------------------------------------------------------
	 * Pre  : <radius> Sets selected flag on all faces hit
	 *
	 * Post : <vertex0> First face encountered along ray, or -1 if DNE.
	 *        Always sets fRayHit flag on vertex0.
	 *        Clears old fRayHit results on all other vertices.
	 ------------------------------------------------------*/


	bool IntersectFaces(Ray &r, int &face0, bool markAll);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : <face0> First face encountered along ray, or -1 if DNE.
	 *        <markAll> If true sets fRayHit flag on all faces hit.
	 *        Always sets fRayHit flag on face0, clears old results. 
	 ------------------------------------------------------*/

	bool Serialize(SystemIO::FileWriter &w);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Serializes the mesh to diskfile as a chunk
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	// FIXME: This should use UV groups when they get checked in...
	void UVMapSelectedFaces_Spherical();
	void UVMapSelectedFaces_Cylindrical();
	void UVMapSelectedFaces_Plane();
	void UVMapSpherical(uint32 groups);
	/*------------------------------------------------------
	 * Pre  : <groupFilter> is valid smoothing group(s) as bitflag
	 * Post : Generates a UV map with spherical projection for groups
	 ------------------------------------------------------*/

	void UpdateVertexReferenceWithSelectedBias();
	void UpdateVertexReferenceWithColorBias(uint32 uvmap);
	void UpdateVertexReferenceWithSmoothingGroupBias(uint32 groupFilter);
	/*------------------------------------------------------
	 * Pre  : <groupFilter> is valid smoothing group or none
	 * Post : Will mark all vertex references for faces of group(s).
	 *
	 * NOTE: This means some vertices will be 'dropped' from reverse lookup, so
	 *       only use this if you're doing group connectivity filtering.
	 *
	 *       If you want a 'full' graph, then pass in 0xFFFF as groups bitmap.
	 *       All facets will be added to their vertex references.
	 ------------------------------------------------------*/

	void ClearFaceFlags(index_t face, uint32 flags);

	void SetFaceFlags(index_t face, uint32 flags);


	void ClearVertexFlags(index_t vertex, uint32 flags);

	void SetVertexFlags(index_t vertex, uint32 flags);


	void SetName(const char *name)
	{
		strncpy(mName, name, 31);
		mName[31] = 0;
	}

	// 'Location' interface
	const Vec3 &GetPosition() { return mPosition; }
	void GetPosition(vec3_t xyz) { mPosition.Set(xyz); }
	void SetPosition(const vec3_t xyz) { HEL_VEC3_COPY(xyz, mPosition.mVec); }
	void SetPosition(const Vec3 &v) { mPosition = v; }
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


	bool Serialize(SystemIO::FileReader &r)
	{
		BUG_ME("Serialize disabled for incomplete class");
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

	void ExtrudeFace(index_t faceIndex, vec3_t displacement);

	Mesh *Cut();

	void Paste(Mesh *model);

	void Rotate(vec_t x, vec_t y, vec_t z);

	void RotateAboutPoint(vec3_t point, vec_t x, vec_t y, vec_t z);

	void Scale(vec_t x, vec_t y, vec_t z);

	void Translate(vec_t x, vec_t y, vec_t z) { Translate(Vec3(x,y,z)); }

	void Translate(Vec3 v) 
	{
		// Position (world)
		SetPosition(v+GetPosition());
		
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

	
	void RemoveWeightSelectedVertices(index_t bone)
	{
		for (int32 i = mVertices.size()-1; i > -1; --i)
		{
			Vertex *v = mVertices[i];

			if (v && v->mFlags & Vertex::fSelected)
			{
				RemoveWeight(i, bone);
			}
		}
	}


	void SetWeightSelectedVertices(index_t bone, vec_t weight)
	{
		for (int32 i = mVertices.size()-1; i > -1; --i)
		{
			Vertex *v = mVertices[i];

			if (v && v->mFlags & Vertex::fSelected)
			{
				SetWeight(i, bone, weight);
			}
		}
	}


	// FIXME: Will make weights maped off actual Vertex later to speed lookup,
	//        and ensure weighting limits
	// NOTE: This doesn't do 'balancing' of the 1.0f limit
	void SetWeight(index_t vertex, index_t bone, vec_t weight)
	{
		for (int32 i = mWeights.size()-1; i > -1; --i)
		{
			Weight *w = mWeights[i];

			if (w && w->mVertexIndex == vertex && w->mBoneIndex == bone)
			{
				w->mWeight = weight;
				return;
			}
		}

		mWeights.pushBack(new Weight(vertex, bone, weight));
	}

	void RemoveWeight(index_t vertex, index_t bone)
	{
		for (int32 i = mWeights.size()-1; i > -1; --i)
		{
			Weight *w = mWeights[i];

			if (w && w->mVertexIndex == vertex && w->mBoneIndex == bone)
			{
				delete w;
				mWeights[i] = NULL;
				return;
			}
		}
	}

	void RemoveWeight(index_t vertex)
	{
		for (int32 i = mWeights.size()-1; i > -1; --i)
		{
			Weight *w = mWeights[i];

			if (w && w->mVertexIndex == vertex)
			{
				delete w;
				mWeights[i] = NULL;
				return;
			}
		}
	}


	void AddWeight(index_t vertexIndex, vec_t weight, index_t bone) 
	{
		SetWeight(vertexIndex, bone, weight);
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


	void FaceRemovalCleanup(Face *face)
	{
		BUG_ME("This implementation doesn't clean up deps, which it prob shouldn't until vertex reference design is finalized");		
	}


	void DeleteSelectedFaces()
	{
		Face **array = mFaces.getVectorArray();

		for (uint32 i = 0, n = mFaces.size(); i < n; ++i)
		{
			Face *face = array[i];

			if (face && face->mFlags & Face::fSelected)
			{
				FaceRemovalCleanup(face);
				delete face;
				array[i] = NULL;
			}
		}
	}


	void DeleteFace(index_t idx)
	{
		Face **array = mFaces.getVectorArray();
		Face *face = GetFace(idx);

		if ( face )
		{
			FaceRemovalCleanup(face);
			delete face;
			array[idx] = NULL;
		}
	}

	vec_t *GetVertexArray()
	{
		return mVertexPool.getVectorArray();
	}


	vec_t *GetNormalArray()
	{
		return mNormalPool.getVectorArray();
	}


	vec_t *GetTexCoordArray()
	{
		return mTexCoordPool.getVectorArray();
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


	index_t CreateTexCoord(const vec3_t uvw)
	{
		return AddTripleVec(mTexCoordPool, mFreedTexCoords, (vec_t*)uvw);
	}


	void TransformTexCoords(Matrix &mat)
	{
		TripleVec_Transform(mTexCoordPool, mat);
	}

	void TransformNormals(Matrix &mat)
	{
		TripleVec_Transform(mNormalPool, mat);
	}

	void TransformVertices(Matrix &mat)
	{
		TripleVec_Transform(mVertexPool, mat);

		// Transform BV too, now
		//vec3_t min, max;
		//GetBBox(min, max);
		//mat.Multiply3v(min);
		//mat.Multiply3v(max);
		//SetBBox(min, max);
		mInitBoundingVol = false;
		UpdateBoundingVolume(); // Handles rotations correctly ( keeps AABB )
	}


	void UpdateBoundingVolume()
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


	void DeleteVertex(index_t vertexIndex)
	{
		BUG_ME("Not Implemented");
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




private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////

	bool SerializePool(SystemIO::FileWriter &w, 
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
		uint32 i, size = v.size();
		vec_t *array = v.getVectorArray();

		for ( i = 0; i < size; i += 3 )
		{
			array[i  ] += xyz[0];  
			array[i+1] += xyz[1];  
			array[i+2] += xyz[2];  
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

	static index_t mNextUID;  /* UIDs outside of model (owner's) array index */

	char mName[mNameSize];    /* Human readable name of mesh */

	index_t mUID;

	bool mInitBoundingVol;

	uint32 mFlags;

	index_t mMaterialIndex;

	Vector3d mPosition;

	Vector3d mRotation;        /* Store as Euler Angles for 'Size' interface */

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
