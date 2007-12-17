/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : FreyjaMesh
 * License : No use w/o permission (C) 2004-2007 Mongoose
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
 *-- Todo ---------------------------------------------------
 *
 * Alpha faces 'presort'
 * Enable depth sorting for fAlpha faces and put them at the end of the
 * list instead.  This way on export renderers can just move a pointer
 * on the buffers to get alpha pass.
 *
 *
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.06.10:
 * Mongoose - XML serializer, doc update, new clean methods to 
 *            shrink sparse buffers, etc.
 *
 * 2004.10.22:
 * Mongoose - Created, expanded from Freyja class
 ==========================================================================*/

#ifndef GUARD__FREYJA_MESH_H__
#define GUARD__FREYJA_MESH_H__

#include <hel/Ray.h>
#include "Track.h"
#include "Weight.h"
#include "Vertex.h"
#include "Face.h"
#include "Edge.h"
#include "Plane.h"

#include "SceneNode.h"
#include "VertexArray.h"

namespace freyja {

class Mesh : public SceneNode
{
public:	
	
	//typedef enum {
	//	fV3N3T2 = 0,
	//	fV4N3T3,
	//	
	//} Format;


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Mesh( const char* name );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Mesh
	 *
	 ------------------------------------------------------*/

	Mesh( const Mesh& mesh );
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

	freyja::Face* CreateFace( );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : This also creates a coresponding index array entry.
	 *
	 ------------------------------------------------------*/

	void AddVertexToFace( freyja::Face* face, freyja::Vertex* vertex );
	/*------------------------------------------------------
	 * Pre  : <face> and <vertex> are allocated, and don't already 
	 *        have an association. 
	 *
	 * Post : This also reserves space in the vertex arrays for a
	 *        new face vertex entry.  Faces can share a Vertex, 
	 *        but must keep separate array entries for the new
	 *        rendering support design.
	 *
	 ------------------------------------------------------*/

	freyja::Vertex* CreateVertex( const vec3_t pos );
	/*------------------------------------------------------
	 * Pre  : <pos> is the XYZ coordinates of the vertex.
	 *
	 * Post : This creates a new 'wrapper' vertex that handles
	 *        a vertex format entry for face(s).
	 *
	 ------------------------------------------------------*/

	freyja::Edge* CreateEdge( freyja::Vertex* vertex0, freyja::Vertex* vertex1 );


	////////////////////////////////////////////////////////////
	// Weight methods
	////////////////////////////////////////////////////////////

	freyja::Weight* CreateWeight( freyja::Bone* bone, freyja::Vertex* vertex );
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

	void RemoveWeight( freyja::Weight* weight );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	bool CopyVertexBuffer(mstl::Vector<vec_t> &buffer);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Copies the vertex buffer to external Vector.
	 *        Returns true on success.
	 *
	 ------------------------------------------------------*/

	bool CopyVertexBlendBuffer(mstl::Vector<vec_t> &buffer);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Copies vertex blend buffer to external Vector.
	 *        Returns true on success.
	 *
	 ------------------------------------------------------*/

	bool GetBlendShadowVolume(mstl::Vector<vec_t> &shadowVolume,
							  vec3_t lightPos, vec3_t pos, vec_t cinf);
	/*------------------------------------------------------
	 * Pre  : <cinf> > 0.0f - Then apply max occlusion distance.
	 * 
	 * Post : Rebuilds shadow volume for this mesh.
	 *        Uses currently past in shadow volume buffer.
	 *        Returns true on success.
	 *
	 ------------------------------------------------------*/

	bool GetShadowVolume(mstl::Vector<vec_t> &shadowVolume,
						 vec3_t lightPos, vec3_t pos, vec_t cinf);
	/*------------------------------------------------------
	 * Pre  : <cinf> > 0.0f - Then apply max occlusion distance.
	 * 
	 * Post : Rebuilds shadow volume for this mesh.
	 *        Uses currently past in shadow volume buffer.
	 *        Returns true on success.
	 *
	 ------------------------------------------------------*/

	void CollapseEdge(index_t face, uint32 a, uint32 b, uint32 c, uint32 d);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void SplitFace(index_t faceIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void GetSelectedFaces(Vector<index_t>& faces);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Get a list of unique faces marked selected
	 *
	 ------------------------------------------------------*/

	void GetUniqueVerticesInFaces(const Vector<index_t>& faces,
								  Vector<index_t>& vertices);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Get a list of unique vertices in faces list
	 *
	 ------------------------------------------------------*/

	void SelectVerticesOfSelectedFaces();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void ClearFlagForSelectedFaces(Face::Flags flag);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void SetFlagForSelectedFaces(Face::Flags flag);
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

	//void GetColor(index_t colorIndex, vec4_t rgba)
	//{ GetVec(mColorPool, 4, colorIndex, rgba); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	Face *GetFace(index_t idx);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *
	 ------------------------------------------------------*/

	uint32 GetFaceCount() { return mFaces.size(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	//void GetNormal(index_t normalIndex, vec3_t xyz)
	//{ GetTripleVec(mNormalPool, normalIndex, xyz); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	//uint32 GetNormalCount() { return mNormalPool.size(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	const mstl::list<Vertex*>& GetSelectedVertices( );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Return a list of vertices flagged fSelected 
	 *        ( by index )
	 ------------------------------------------------------*/

	//void GetTexCoord(index_t texCoordIndex, vec3_t uvw)
	//{	GetTripleVec(mTexCoordPool, texCoordIndex, uvw);	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	//uint32 GetVertexArrayCount() { return mVertexPool.size(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	//uint32 GetTexCoordCount() { return mTexCoordPool.size(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	Vertex *GetVertex(index_t vertexIndex);
	/*------------------------------------------------------
	 * Pre  : vertexIndex is valid for this mesh
	 * Post : Return vertex if it exists or NULL
	 ------------------------------------------------------*/

	hel::Vec3 GetVertexNormal(index_t idx);
	void SetVertexNormal(index_t idx, hel::Vec3 n);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	hel::Vec3 GetVertexPosition(index_t idx);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	hel::Vec3 GetVertexTexCoord(index_t idx);
	void SetVertexTexCoord(index_t idx, hel::Vec3 uv);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	uint32 GetVertexCount() { return mVertices.size(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void GetVertexPos(index_t vertex, vec3_t xyz)
	{
		if (vertex < mVertices.size() && mVertices[vertex])
			GetVertexArrayPos(mVertices[vertex]->mVertexIndex, xyz);
	}
	/*------------------------------------------------------
	 * Pre  : <vertex> is an mVertices[] index.
	 * Post : Correct Get method for class Vertex position
	 *        with bounds checking.
	 ------------------------------------------------------*/

	void SetVertexPos(index_t vertex, vec3_t xyz)
	{
		if (vertex < mVertices.size() && mVertices[vertex])
			SetVertexArrayPos(mVertices[vertex]->mVertexIndex, xyz);
	}
	/*------------------------------------------------------
	 * Pre  : <vertex> is an mVertices[] index.
	 * Post : Correct Set method for class Vertex position
	 *        with bounds checking.
	 ------------------------------------------------------*/



	bool Intersect(hel::Ray &r, vec_t &t);
	/*------------------------------------------------------
	 * Pre  : If mesh is fHidden intersects will always return false
	 *
	 * Post : Returns true if 'hit', and <t> the time along ray
	 *        This checks the bsphere, then bbox for collision.
	 ------------------------------------------------------*/

	int PickFace(Face::Flags flag, hel::Ray &r, hel::Vec3 &tuv);
	/*------------------------------------------------------
	 * Pre  : <flag> ignore faces with this flag.
	 *        <r> the pick ray.
	 *
	 * Post : Returns index of first face encountered or -1 if DNE.
	 *        <tuv> Contains t, and uv results from intersection.
	 *
	 *        Sets no flags on any face.
	 ------------------------------------------------------*/

	int PickEdge(hel::Ray &r, vec_t &t);
	/*------------------------------------------------------
	 * Pre  : <r> the pick ray.
	 *
	 * Post : Returns index of first edge encountered or -1 if none.
	 *        <t> term where ray intersected edge bbox target.
	 *
	 ------------------------------------------------------*/

	bool IntersectHitBox(hel::Ray &r, 
						 const hel::Vec3 &min, const hel::Vec3 &max, vec_t &t);
	/*------------------------------------------------------
	 * Pre  : <r> the pick ray.
	 *
	 * Post : Returns index of first edge encountered or -1 if missed.
	 *        <t> where ray intersected generic bbox target.
	 *
	 ------------------------------------------------------*/

	bool IntersectPerFace(hel::Ray &r, vec_t &t);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if 'hit', and <t> the time along ray
	 *        This checks per face for the final validation on 
	 *        top of just bounding volume checks.
	 ------------------------------------------------------*/
	
	bool IntersectClosestFace(hel::Ray &r, int &face0) 
	{ return IntersectFaces(r, face0, false); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : <face0> First face encountered along ray, or -1 if DNE.
	 *        Always sets fRayHit flag on face0.
	 *        Clears old fRayHit results on all other faces.
	 ------------------------------------------------------*/

	void SelectVerticesByBox(hel::Vec3 &min, hel::Vec3 &max);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void SelectFacesByBox(hel::Vec3 &min, hel::Vec3 &max);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	bool IntersectClosestVertex(hel::Ray &r, int &vertex0, vec_t radius);
	/*------------------------------------------------------
	 * Pre  : <radius> Sets selected flag on all faces hit
	 *
	 * Post : <vertex0> First face encountered along ray, or -1 if DNE.
	 *        Always sets fRayHit flag on vertex0.
	 *        Clears old fRayHit results on all other vertices.
	 ------------------------------------------------------*/

	bool IntersectUVFaces(hel::Ray &r, int &face0, bool markAll, 
						  index_t material);
	/*------------------------------------------------------
	 * Pre  : Current <material> is used as a filter.
	 *
	 * Post : <face0> First face encountered along ray, or -1 if DNE.
	 *        <markAll> If true sets fRayHit flag on all faces hit.
	 *        Always sets fRayHit flag on face0, clears old results. 
	 ------------------------------------------------------*/

	bool IntersectFaces(hel::Ray &r, int &face0, bool markAll);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : <face0> First face encountered along ray, or -1 if DNE.
	 *        <markAll> If true sets fRayHit flag on all faces hit.
	 *        Always sets fRayHit flag on face0, clears old results. 
	 ------------------------------------------------------*/

	bool Repack();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Repack state is cached to avoid needless work.
	 *        Other mesh object operations affect state.
	 *
	 *        Remove gaps in sparse buffers and objects.
	 *        Updates all references to these elements.
	 *
	 *        Returns true if repack was needed.
	 *
	 ------------------------------------------------------*/

	void UpdateBoundingVolume();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Rebuilds bounding volumes.
	 *
	 ------------------------------------------------------*/

	void UpdateEdgeGraph();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Rebuilds edge graph by checking every face.
	 *
	 ------------------------------------------------------*/

	void UpdatePlaneGeometry();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Rebuilds list of planes from facet list.
	 *
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

	FREYJA_XMLSERIALIZER_INTERFACE
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : XmlSerializer interface macro.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Move to external algorithms.
	////////////////////////////////////////////////////////////

	void ApplyLoopSubDiv();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Loop subdivision is performed on the mesh.
	 *        This calls UpdateEdgeGraph() before subdiv.
	 *
	 ------------------------------------------------------*/

	void ApplyTrianglarTesselation();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Make all facets triangles.  
	 *        This calls Repack() before tesselation.
	 *
	 ------------------------------------------------------*/

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

	void RecomputeFaceNormal(index_t face, bool full);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Recomputes face normal.
	 *
	 *        If <full> all vertices are also updated, and
	 *        so are the face's neighbour cache.
	 *
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

	Mesh* CopyWithBlendedVertices();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Copies this mesh, except the blend vertices
	 *        replace the unweighted vertices.
	 *
	 *        Returns mesh outside of pool control. 
	 *
	 ------------------------------------------------------*/

	index_t CreateVertexKeyframeFromBlended(index_t track, vec_t time);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : The blend vertices are converted to a 
	 *        vertex morph keyframe.
	 *
	 *        Returns valid index on success. 
	 *
	 ------------------------------------------------------*/	

	index_t CreateVertexKeyframeFromImport(index_t track, vec_t time, Vector<vec_t>& vertices);
	/*------------------------------------------------------
	 * Pre  : List must have the same number of vertices as mesh.
	 *
	 * Post : The imported vertices are converted to a 
	 *        vertex morph keyframe.
	 *
	 *        Returns valid index on success. 
	 *
	 ------------------------------------------------------*/	

	void SetVertexFlags(index_t vertex, uint32 flags);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets option flag for <vertex>
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

	// In case you can't tell these are no longer implemented
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
	 ------------------------------------------------------*/
	
	void AppendVertexToFace(index_t face, index_t vertex);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/



	/*
	index_t CreateVertex(const vec3_t xyz)
	{
		hel::Vec3 n(0.0f, 1.0f, 0.0f); 
		hel::Vec3 t(0.5f, 0.5f, 0.0f);
		return CreateVertex(xyz, t.mVec, n.mVec);
	}
	*/
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

	void CheckArrayRefs(index_t vertex, bool &v, 
						index_t normal, bool &n,
						index_t texcoord, bool &t);	
	bool CheckNormalArrayRefs(index_t arrayIndex);
	bool CheckTexCoordArrayRefs(index_t arrayIndex);
	bool CheckVertexArrayRefs(index_t arrayIndex);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns true if array index is being referenced.
	 *
	 ------------------------------------------------------*/

	void RemoveWeightSelectedVertices(index_t bone);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void SetWeightSelectedVertices(index_t bone, vec_t weight);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *
	 ------------------------------------------------------*/







	bool WeldTexCoords(index_t replace, index_t texcoord);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Replace is replaced by texcoord.
	 *        
	 ------------------------------------------------------*/

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

	void VertexCleanup();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Any 'loose' vertices that are unreferenced 
	 *        will be deleted.
	 *
	 ------------------------------------------------------*/

	void FaceRemovalCleanup(Face *face);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Any 'loose' vertices referenced by this face
	 *        will be deleted.
	 *
	 ------------------------------------------------------*/

	void DeleteSelectedFaces();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : All faces marked selected are removed from mesh.
	 *
	 ------------------------------------------------------*/

	void DeleteUnSelectedFaces();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : All faces not marked selected are removed from mesh.
	 *
	 ------------------------------------------------------*/

	void DeleteFace(index_t face);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : face is removed from mesh.
	 *
	 ------------------------------------------------------*/

	bool DeleteVertex(index_t vertex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : If vertex polygon reference is empty it will 
	 *        be deleted and this will return true.
	 *
	 ------------------------------------------------------*/

	void MeldVertices(index_t a, index_t b);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Vertex <a> clones all of <b>'s array indices.
	 *        Both vertices will continue to exist while 
	 *        maintaining separate reference lists and flags.
	 *
	 *        Operations done on either will operate on the
	 *        same pos/texcoord/normal array elements.
	 *        This allows for undo operations for example.  
	 *
	 ------------------------------------------------------*/

	bool WeldVertices(index_t replace, index_t vertex);
	/*------------------------------------------------------
	 * Pre  : <vertex> must index a valid Vertex.
	 *
	 * Post : Vertex <replace> is destroyed and all references 
	 *        to <replace> are replaced with <vertex>.
	 *
	 *        Returns true if <replace> is sucessfully purged.
	 *
	 ------------------------------------------------------*/

	void WeldVerticesByDistance(vec_t tolerance);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : All vertices are tested to be within a distance
	 *        of tolerance to each other vertex to be welded.
	 *
	 ------------------------------------------------------*/

	//void ClampAllTexCoords()
	//{ ClampAllTexCoords(0.0f, 1.0f); }

	//void ClampAllTexCoords(vec_t min, vec_t max)
	//{ ClampVecValues(mTexCoordPool, min, max); }

	void SetFaceSmoothingGroup(index_t face, uint32 group);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void SelectedFacesMarkSmoothingGroup(uint32 group, bool t);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void GroupedFacesGenerateVertexNormals(uint32 group);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
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
	 *
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
	{ return mVertexPool.get_array(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns vertex array ( great for rendering )
	 ------------------------------------------------------*/

	vec_t *GetNormalArray()
	{ return mNormalPool.get_array(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns normal array ( great for rendering )
	 ------------------------------------------------------*/

	vec_t *GetTexCoordArray()
	{ return mTexCoordPool.get_array(); }
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

	//uint32 GetTransformTrackCount() {return 1;}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	//index_t NewTransformTrack() {return 0;}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	//TransformTrack &GetTransformTrack(uint32 track) 
	//{ return mTrack; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	//uint32 GetVertexAnimTrackCount() {return 1;}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	//uint32 GetVertexAnimKeyframeCount(uint32 track) 
	//{ return mVertexAnimTrack.GetKeyframeCount();}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	//index_t NewVertexAnimTrack() {return 0;}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	//VertexAnimTrack &GetVertexAnimTrack(uint32 track) {return mVertexAnimTrack;}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	//vec_t *GetBlendVerticesArray() { return mBlendVertices.get_array(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	//uint32 GetBlendVerticesCount() { return mBlendVertices.size(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	//void ResetBlendVertices()
	//{
	//	vec_t *array = mBlendVertices.get_array();
	//	if (array)
	//		memset(array, 0, mBlendVertices.size()*sizeof(vec_t));
	//}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	//void SyncBlendVertices()
	//{
	//	if (mBlendVertices.size() < mVertexPool.size())
	//	{
	//		mBlendVertices.resize(mVertexPool.size());
	//	}
	//}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/





	////////////////////////////////////////////////////////////
	// Transforms
	////////////////////////////////////////////////////////////

	//void TransformTexCoords(hel::Mat44 &mat)
	//{ TripleVec_Transform(mTexCoordPool, mat); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Applies matrix transform to texcoord pool
	 ------------------------------------------------------*/

	//void TransformNormals(hel::Mat44 &mat)
	//{ TripleVec_Transform(mNormalPool, mat); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Applies matrix transform to normal pool
	 ------------------------------------------------------*/

	//void TransformVertices(hel::Mat44 &mat)
	//{
	//	TripleVec_Transform(mVertexPool, mat);
	//	mInitBoundingVol = false;
	//	UpdateBoundingVolume(); // Handles rotations correctly ( keeps AABB )
	//}
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Applies matrix transform to vertices pool
	 ------------------------------------------------------*/

	void TransformFacesWithFlag(Face::Flags flag, hel::Mat44 &mat);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Applies matrix transform to face with flag set
	 ------------------------------------------------------*/

	void TransformFacesInList(Vector<index_t> &faces, hel::Mat44 &mat);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Applies matrix transform to faces in index list
	 ------------------------------------------------------*/

	void TransformVerticesWithFlag(Vertex::Flags flag, hel::Mat44 &mat);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Applies matrix transform to vertices with flag set
	 ------------------------------------------------------*/

	void TransformVerticesInList(Vector<index_t> &vertices, hel::Mat44 &mat);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Applies matrix transform to vertices in index list
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


protected:

	////////////////////////////////////////////////////////////
	// Protected Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Protected Mutators
	////////////////////////////////////////////////////////////

	void DeleteVertexHelper(Vertex **array, index_t vertex);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : This is called by methods that also do ref fixing.
	 *
	 ------------------------------------------------------*/

	//TransformTrack mTrack;            /* Mesh transform animation track */

	//VertexAnimTrack mVertexAnimTrack; /* Mesh vertex animation track  */

	//vec_t mBlendVerticesTime;         /* Last time this was updated */

	//Vector<vec_t> mBlendVertices;     /* Skeletal vertex blending use  */

	bool mPacked;

	Vector<vec_t> mVertexPool;
	mstl::stack<index_t> mFreedVertices;

	Vector<vec_t> mNormalPool;
	mstl::stack<index_t> mFreedNormals;

	Vector<vec_t> mColorPool;
	mstl::stack<index_t> mFreedColors;

	Vector<vec_t> mTexCoordPool;
	mstl::stack<index_t> mFreedTexCoords;

	mstl::list<Vertex*> mSelectedVertices; /* List of selected vertices. */

	mstl::list<Face*> mSelectedFaces;      /* List of selected faces. */

	mstl::list<Edge*> mSelectedEdges;      /* List of selected edges. */
	

	Vector<Face *> mFaces;

	Vector<Vertex *> mVertices;

	Vector<Weight *> mWeights;

	Vector<Plane *> mPlanes;

	Vector<Edge *> mEdges;
};

} // namespace freyja

#endif // GUARD__FREYJA_MESH_H__
