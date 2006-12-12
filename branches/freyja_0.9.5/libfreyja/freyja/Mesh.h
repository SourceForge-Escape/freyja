/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : FreyjaMesh
 * License : No use w/o permission (C) 2004-2006 Mongoose
 * Comments: This is the freyja::Mesh and class.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_MESH - Builds Mesh class as a unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.10.22:
 * Mongoose - Created, expanded from Freyja class
 ==========================================================================*/

#ifndef GUARD__FREYJA_MESH_H_
#define GUARD__FREYJA_MESH_H_

#include "freyja.h"
#include <hel/math.h>
#include <hel/BoundingVolume.h>
#include <hel/Ray.h>
#include <mstl/Vector.h>
#include <mstl/stack.h>
#include <mstl/SystemIO.h>
#include "Track.h"
#include "Weight.h"
#include "Vertex.h"
#include "Face.h"

using namespace mstl;
using namespace freyja;

namespace freyja {

class Mesh
{
public:	
	typedef enum {
		fNone        =  0,
		fHighlighted =  1,
		fSelected    =  2,
		fHidden      =  4,
		fRayHit      =  8
	} Flags;


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Mesh();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Mesh
	 *
	 ------------------------------------------------------*/

	Mesh(const Mesh &mesh);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Mesh
	 *
	 ------------------------------------------------------*/

	~Mesh();
	/*------------------------------------------------------
	 * Pre  : Mesh object is allocated
	 * Post : Deconstructs an object of Mesh
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Properities
	////////////////////////////////////////////////////////////

	uint32 GetFlags() { return mFlags; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns option flag bitmap for this mesh
	 *
	 ------------------------------------------------------*/

	void ClearFlag(Flags flag) { mFlags &= ~flag; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Clears option flag for this mesh
	 *
	 ------------------------------------------------------*/

	void SetFlag(Flags flag) { mFlags |= flag; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets option flag for this mesh
	 *
	 ------------------------------------------------------*/

	void SetMaterial(index_t idx) { mMaterialIndex = idx; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	const char *GetName() { return mName; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get human readable name of this mesh
	 *
	 ------------------------------------------------------*/

	void SetName(const char *name) { strncpy(mName, name, 31); mName[31] = 0; }
	/*------------------------------------------------------
	 * Pre  : <name> != NULL && <name>[0] != 0
	 * Post : Sets human readable name of this mesh
	 *
	 ------------------------------------------------------*/

	const Vec3 &GetPosition() { return mPosition; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets mesh position attribute
	 *
	 ------------------------------------------------------*/

	void SetPosition(const Vec3 &v) { mPosition = v; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets mesh position attribute
	 *
	 ------------------------------------------------------*/

	const Vec3 &GetRotation() { return mRotation; }
	/*------------------------------------------------------
	 * Pre  : Euler angles in radians
	 * Post : Gets mesh position attribute
	 *
	 ------------------------------------------------------*/

	void SetRotation(const Vec3 &v) { mRotation = v; }
	/*------------------------------------------------------
	 * Pre  : Euler angles in radians
	 * Post : Sets mesh rotation attribute
	 *
	 ------------------------------------------------------*/

	const Vec3 &GetScale() { return mScale; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets mesh scale attribute
	 *
	 ------------------------------------------------------*/

	void SetScale(const Vec3 &v) { mScale = v; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets mesh scale attribute
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	static const char *GetChunkType() { return "Mesh"; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	static uint32 GetChunkVersion() { return 2; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void CollapseEdge(index_t face, uint32 a, uint32 b, uint32 c, uint32 d);
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

	Vec3 GetVertexNormal(index_t idx);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	Vec3 GetVertexPosition(index_t idx);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	Vec3 GetVertexTexCoord(index_t idx);
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
	 * Pre  : If mesh is fHidden intersects will always return false
	 *
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

	void SelectFacesByBox(Vec3 &min, Vec3 &max);
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

	bool Serialize(SystemIO::TextFileWriter &w);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Serializes the mesh to diskfile as a chunk
	 ------------------------------------------------------*/

	bool Serialize(SystemIO::TextFileReader &r);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Serializes the mesh to diskfile as a chunk
	 ------------------------------------------------------*/

	bool Serialize(SystemIO::FileWriter &w);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Serializes the mesh to diskfile as a chunk
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void UVMapSelectedFaces_Cylindrical();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Makes UV cylindrical projection for selected faces
	 ------------------------------------------------------*/

	void UVMapSelectedFaces_Plane();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Makes UV planar projection for selected faces
	 ------------------------------------------------------*/

	void UVMapSelectedFaces_Spherical();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Makes UV spherical projection for selected faces
	 ------------------------------------------------------*/

	void RebuildVertexPolygonReferences();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Completely rebuilds each Vertices' FaceReferences,
	 *        and you don't want to do that very often.
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
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Clears option flag for <face>
	 ------------------------------------------------------*/

	void SetFaceFlags(index_t face, uint32 flags);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets option flag for <face>
	 ------------------------------------------------------*/

	void ClearVertexFlags(index_t vertex, uint32 flags);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Clears option flag for <vertex>
	 ------------------------------------------------------*/

	void SetVertexFlags(index_t vertex, uint32 flags);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets option flag for <vertex>
	 ------------------------------------------------------*/

	bool Serialize(SystemIO::FileReader &r);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void Merge(Mesh *mesh);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Not like the old school 2000.11.30 API, Read
	 *
	 *        The passed <mesh> will be duplicated entirely
	 *        into this mesh.  There is no 'partial merge'
	 *        anymore.  If you want partial do a Split() first.
	 *
	 *        Also this doesn't make a 3rd mesh, but adds
	 *        to this existing one.  Copy() if you want that.
	 *
	 *        If you want <mesh> to not exist after merge
	 *        then you'll have to delete it yourself.
	 ------------------------------------------------------*/

	Mesh *Split(bool trim);
	/*------------------------------------------------------
	 * Pre  : If <trim> is true remove the selected faces
	 *        from this mesh after splitting off to another mesh.
	 * 
	 * Post : Not like the old school 2000.11.30 API, Read
	 *
	 *        A new mesh is made from this mesh based on the
	 *        currently fSelected flagged Faces.
	 *
	 *        This mesh exists outside of the gobal pool.
	 *        If you want to add use the AddToPool() method.
	 ------------------------------------------------------*/

	Mesh *CsgUnion(Mesh *a) { return NULL; }
	Mesh *CsgIntersection(Mesh *a) { return NULL; }
	Mesh *CsgDifference(Mesh *a) { return NULL; }
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

	index_t CreateTexCoord(const vec3_t uvw)
	{ return AddTripleVec(mTexCoordPool, mFreedTexCoords, (vec_t*)uvw); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns local index of new texcoord array element
	 ------------------------------------------------------*/

	index_t CreateVertex(const Vec3 &v) { return CreateVertex(v.mVec); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns local index of new Vertex
	 ------------------------------------------------------*/

	index_t CreateVertex(const vec3_t xyz)
	{
		Vec3 n(0.0f, 1.0f, 0.0f); 
		Vec3 t(0.5f, 0.5f, 0.0f);
		return CreateVertex(xyz, t.mVec, n.mVec);
	}
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns local index of new Vertex
	 ------------------------------------------------------*/

	index_t CreateVertex(const vec3_t xyz, const vec3_t uvw, const vec3_t nxyz);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns local index of new Vertex
	 ------------------------------------------------------*/

	void ExtrudeFace(index_t face, vec3_t displacement);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Extrudes <face> along vector <displacement>
	 ------------------------------------------------------*/
	
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

	void MarkVerticesOfFacesWithFlag(Face::Flags flag, 
									 Vertex::Flags mark, bool clear);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Every face with <flag> set will set all its
	 *        vertices with flag <mark>.
	 *
	 *        If <clear> is true all vertices not marked will
	 *        have flag <mark> cleared.
	 ------------------------------------------------------*/


	void ConvertAllFacesToTexCoordPloymapping();

	void ConvertFaceToTexCoordPloymapping(index_t face);

	Face *GetFace(index_t idx)
	{
		Face **array = mFaces.getVectorArray();
		
		if ( idx < mFaces.size() )
		{
			return array[idx];
		}

		return NULL;
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


	void DeleteUnSelectedFaces()
	{
		Face **array = mFaces.getVectorArray();

		for (uint32 i = 0, n = mFaces.size(); i < n; ++i)
		{
			Face *face = array[i];

			if (face && !(face->mFlags & Face::fSelected))
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


	void UpdateBoundingVolume();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Rotates mesh about bounding volume center
	 *
	 ------------------------------------------------------*/

	void DeleteVertex(index_t vertexIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool WeldVertices(index_t a, index_t b);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void ClampAllTexCoords()
	{ ClampAllTexCoords(0.0f, 1.0f); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void ClampAllTexCoords(vec_t min, vec_t max)
	{ ClampVecValues(mTexCoordPool, min, max); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

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

	void SelectedFacesFlipVertexNormals();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Array exposure
	////////////////////////////////////////////////////////////

	void GetVertexArrayPos(index_t vertexIndex, vec3_t xyz);
	void SetVertexArrayPos(index_t vertexIndex, const vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : NOTE This is not the same as GetVertexClassPos
	 *        This is the ith point stored in the point array
	 * Post : 
	 ------------------------------------------------------*/

	vec_t *GetVertexArray()
	{ return mVertexPool.getVectorArray(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns vertex array ( great for rendering )
	 ------------------------------------------------------*/

	vec_t *GetNormalArray()
	{ return mNormalPool.getVectorArray(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns normal array ( great for rendering )
	 ------------------------------------------------------*/

	vec_t *GetTexCoordArray()
	{ return mTexCoordPool.getVectorArray(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns texcoord array ( great for rendering )
	 ------------------------------------------------------*/


	//////////////////////////////////////////////////////////////////////////
	// Animation tracks interface
	// Testing embedded keyframing, so bare with me...
	// these methods only support one 'animation' atm, but enforcing interface
	// API usage makes it a transparent fix to API users later.
	//////////////////////////////////////////////////////////////////////////

	uint32 GetTransformTrackCount() {return 1;}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	index_t NewTransformTrack() {return 0;}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	TransformTrack &GetTransformTrack(uint32 track) { return mTrack; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	uint32 GetVertexAnimTrackCount() {return 1;}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	index_t NewVertexAnimTrack() {return 0;}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	VertexAnimTrack &GetVertexAnimTrack(uint32 track) {return mVertexAnimTrack;}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	vec_t *GetBlendVerticesArray() { return mBlendVertices.get_array(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	uint32 GetBlendVerticesCount() { return mBlendVertices.size(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void ResetBlendVertices()
	{
		vec_t *array = mBlendVertices.get_array();
		if (array)
			memset(array, 0, mBlendVertices.size()*sizeof(vec_t));
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void SyncBlendVertices()
	{
		if (mBlendVertices.size() < mVertexPool.size())
		{
			mBlendVertices.resize(mVertexPool.size());
		}
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/


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


	////////////////////////////////////////////////////////////
	// Transforms
	////////////////////////////////////////////////////////////

	void TransformTexCoords(Matrix &mat)
	{ TripleVec_Transform(mTexCoordPool, mat); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Applies matrix transform to texcoord pool
	 ------------------------------------------------------*/

	void TransformNormals(Matrix &mat)
	{ TripleVec_Transform(mNormalPool, mat); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Applies matrix transform to normal pool
	 ------------------------------------------------------*/

	void TransformVertices(Matrix &mat)
	{
		TripleVec_Transform(mVertexPool, mat);
		mInitBoundingVol = false;
		UpdateBoundingVolume(); // Handles rotations correctly ( keeps AABB )
	}
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Applies matrix transform to vertices pool
	 ------------------------------------------------------*/

	void TransformFacesWithFlag(Face::Flags flag, Matrix &mat);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Applies matrix transform to face with flag set
	 ------------------------------------------------------*/

	void TransformVerticesWithFlag(Vertex::Flags flag, Matrix &mat);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Applies matrix transform to vertices with flag set
	 ------------------------------------------------------*/

	void Rotate(const Vec3 &v);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Rotates mesh about bounding volume center
	 ------------------------------------------------------*/

	void RotateAboutPoint(const Vec3 &point, const Vec3 &v);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Rotates mesh about <point>
	 ------------------------------------------------------*/

	void Scale(const Vec3 &v);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Scales mesh about Position()
	 ------------------------------------------------------*/

	void ScaleAboutPoint(const Vec3 &point, const Vec3 &v);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Scales mesh about <point>
	 ------------------------------------------------------*/

	void Translate(const Vec3 &v);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Translates mesh based on Position()
	 ------------------------------------------------------*/

	void SelectedFacesRotateUVMap(vec_t z);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Translates mesh based on Position()
	 ------------------------------------------------------*/

	void SelectedFacesScaleUVMap(vec_t x, vec_t y);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Translates mesh based on Position()
	 ------------------------------------------------------*/

	void SelectedFacesTranslateUVMap(vec_t x, vec_t y);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Translates mesh based on Position()
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Gobal pool API
	////////////////////////////////////////////////////////////

	static uint32 GetCount() { return mGobalPool.size(); } 
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns number of meshes in gobal store
	 ------------------------------------------------------*/

	static Mesh *GetMesh(index_t uid) 
	{ return (uid < mGobalPool.size()) ?  mGobalPool[uid] : NULL; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns mesh matching gobal store UID
	 ------------------------------------------------------*/

	static void ResetPool() { mGobalPool.erase(); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Erase the gobal mesh store
	 ------------------------------------------------------*/

	index_t AddToPool()
	{
		uint32 i, count;
		bool found = false;

		if (mUID == INDEX_INVALID)
		{
			/* Setup UID and class container reference */
			mUID = count = mGobalPool.size();

			for (i = 0; i < count; ++i)
			{
				if (mGobalPool[i] == 0x0)
				{
					mUID = i;
					mGobalPool.assign(mUID, this);
					found = true;
				}	
			}

			if (!found)
			{
				mGobalPool.pushBack(this);
			}
		}

		return mUID;
	}
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Add this mesh to the gobal store
	 *        Return UID
	 ------------------------------------------------------*/

	index_t GetUID() { return mUID; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns this mesh's gobal store UID
	 ------------------------------------------------------*/

	void RemoveFromPool()
	{
		if (mUID < mGobalPool.size())
			mGobalPool.assign(mUID, 0x0);

		mUID = INDEX_INVALID;
	}

	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Remove this mesh from gobal store 
	 *        Does not delete mesh ( obviously )
	 ------------------------------------------------------*/


private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////

	bool SerializePool(SystemIO::FileWriter &w, 
					   Vector<vec_t> &v, mstl::stack<index_t> &s);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Serializes the given pool to diskfile as a chunk
	 ------------------------------------------------------*/

	bool SerializePool(SystemIO::TextFileWriter &w, const char *name,
					   Vector<vec_t> &array, mstl::stack<index_t> &stack);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Serializes the given pool to diskfile as a chunk
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	bool SerializePool(SystemIO::FileReader &r, 
					   Vector<vec_t> &v, mstl::stack<index_t> &s);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Serializes the given pool to diskfile as a chunk
	 ------------------------------------------------------*/

	bool SerializePool(SystemIO::TextFileReader &r, const char *name,
					   Vector<vec_t> &array, mstl::stack<index_t> &stack);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Serializes the given pool to diskfile as a chunk
	 ------------------------------------------------------*/

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

	static Vector<Mesh *> mGobalPool; /* Storage for gobal access */

	const static uint32 mNameSize = 32;

	//static index_t mNextUID;          /* UID creation outside of gobal pool */

	TransformTrack mTrack;            /* Mesh transform animation track */

	VertexAnimTrack mVertexAnimTrack; /* Mesh vertex animation track  */

	vec_t mBlendVerticesTime;         /* Last time this was updated */

	Vector<vec_t> mBlendVertices;     /* Skeletal vertex blending use  */

	char mName[mNameSize];            /* Human readable name of mesh */

	index_t mUID;                     /* Gobal pool UID */

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


} // End namespace freyja


#endif
