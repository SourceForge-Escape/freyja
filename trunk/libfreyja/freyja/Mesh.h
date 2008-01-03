/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * License : No use w/o permission (C) 2004-2007 Mongoose
 * Object  : Mesh
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
#include "Weight.h"
#include "Vertex.h"
#include "Face.h"
#include "Edge.h"
#include "Plane.h"

#include "SceneNode.h"
#include "FloatArray.h"

namespace freyja {

class Mesh : 
		public SceneNode
{
public:	
	
	//typedef enum {
	//	fV3N3T2 = 0,
	//	fV4N3T3,
	//	
	//} VertexFormat;


	// This is a child of mesh in scenegraph -- you can't render Mesh directly in general.
	// Allows for material sorting, alpha pass, etc.
	class TriangleList :
		public Node
	{
	public:

		// bool mAlpha; // Move this to material, sort by material

		freyja::Material* mMaterial;    /* All faces in this list use this material. */
		mstl::Vector<uint32> mIndices;  /* Triangle list. */
	};



	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Mesh( const char* name );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	Mesh( const Mesh& mesh );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Copy constructor.
	 *
	 ------------------------------------------------------*/

	~Mesh( );
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

	freyja::Vertex* CreateVertex( const hel::Vec3 pos );
	/*------------------------------------------------------
	 * Pre  : <pos> is the XYZ coordinates of the vertex.
	 *
	 * Post : This creates a new 'wrapper' vertex that handles
	 *        a vertex format entry for face(s).
	 *
	 ------------------------------------------------------*/

	freyja::Edge* CreateEdge( freyja::Vertex* a, freyja::Vertex* b );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns a new edge or NULL on error.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Geometry methods
	////////////////////////////////////////////////////////////

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


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	bool CopyVertexArray( freyja::FloatArray& array ) const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Copies the vertex array to external array.
	 *        Returns true on success.
	 *
	 ------------------------------------------------------*/

	//virtual bool GetShadowVolume( mstl::Vector<vec_t>& shadowVolume,
	//							  vec3_t lightPos, vec3_t pos, vec_t cinf ) const;
	/*------------------------------------------------------
	 * Pre  : <cinf> > 0.0f - Then apply max occlusion distance.
	 * 
	 * Post : Rebuilds shadow volume for this mesh.
	 *        Uses currently past in shadow volume buffer.
	 *        Returns true on success.
	 *
	 ------------------------------------------------------*/

	const mstl::list<Face*>& GetSelectedFaces( ) const
	{ return mSelectedFaces; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Get a list of unique faces marked selected
	 *
	 ------------------------------------------------------*/

	void GetUniqueVerticesInFaces( const mstl::Vector<index_t>& faces,
								   mstl::Vector<index_t>& vertices) const;
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Get a list of unique vertices in faces list
	 *
	 ------------------------------------------------------*/

	freyja::Face* GetFace( index_t idx ) const;
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *
	 ------------------------------------------------------*/

	uint32 GetFaceCount() const 
	{ return mFaces.size(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	const mstl::list<Vertex*>& GetSelectedVertices( ) const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Return a list of vertices flagged fSelected 
	 *        ( by index )
	 ------------------------------------------------------*/

	freyja::Face* PickFace( hel::Ray& ray, hel::Vec3& tuv ) const;
	/*------------------------------------------------------
	 * Pre  : <ray> the pick ray.
	 *
	 * Post : Returns first face encountered or NULL if none.
	 *        <tuv> Contains t, and uv results from intersection.
	 *
	 *        Sets no flags on any face.
	 ------------------------------------------------------*/

	freyja::Edge* PickEdge( hel::Ray& ray, vec_t& t ) const;
	/*------------------------------------------------------
	 * Pre  : <r> the pick ray.
	 *
	 * Post : Returns first edge encountered or NULL if none.
	 *        <t> term where ray intersected edge bbox target.
	 *
	 ------------------------------------------------------*/

	const vec_t* GetVertexArray() const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns vertex array.
	 ------------------------------------------------------*/

	const vec_t* GetNormalArray() const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns normal array.
	 ------------------------------------------------------*/

	const vec_t* GetTexCoordArray() const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns texcoord array.
	 ------------------------------------------------------*/

	uint32 GetVertexArrayCount() const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

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

	void SelectVerticesOfSelectedFaces();
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

	uint32 GetVertexCount() const
	{ return mVertices.size(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	bool Intersect(hel::Ray& ray, vec_t& t);
	/*------------------------------------------------------
	 * Pre  : If mesh is fHidden intersects will always return false
	 *
	 * Post : Returns true if 'hit', and <t> the time along ray
	 *        This checks the bsphere, then bbox for collision.
	 ------------------------------------------------------*/

	bool IntersectHitBox(hel::Ray& ray, 
						 const hel::Vec3& min, const hel::Vec3& max, vec_t& t);
	/*------------------------------------------------------
	 * Pre  : <r> the pick ray.
	 *
	 * Post : Returns index of first edge encountered or -1 if missed.
	 *        <t> where ray intersected generic bbox target.
	 *
	 ------------------------------------------------------*/

	bool IntersectPerFace(hel::Ray &ray, vec_t &t);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if 'hit', and <t> the time along ray
	 *        This checks per face for the final validation on 
	 *        top of just bounding volume checks.
	 ------------------------------------------------------*/
	
	bool IntersectClosestFace(hel::Ray &ray, int &face0) 
	{ return IntersectFaces(ray, face0, false); }
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

	bool IntersectClosestVertex(hel::Ray &ray, int &vertex0, vec_t radius);
	/*------------------------------------------------------
	 * Pre  : <radius> Sets selected flag on all faces hit
	 *
	 * Post : <vertex0> First face encountered along ray, or -1 if DNE.
	 *        Always sets fRayHit flag on vertex0.
	 *        Clears old fRayHit results on all other vertices.
	 ------------------------------------------------------*/

	bool IntersectUVFaces(hel::Ray &ray, int &face0, bool markAll, 
						  index_t material);
	/*------------------------------------------------------
	 * Pre  : Current <material> is used as a filter.
	 *
	 * Post : <face0> First face encountered along ray, or -1 if DNE.
	 *        <markAll> If true sets fRayHit flag on all faces hit.
	 *        Always sets fRayHit flag on face0, clears old results. 
	 ------------------------------------------------------*/

	bool IntersectFaces(hel::Ray &ray, int &face0, bool markAll);
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

	//void ClampAllTexCoords()
	//{ ClampAllTexCoords(0.0f, 1.0f); }

	//void ClampAllTexCoords(vec_t min, vec_t max)
	//{ ClampVecValues(mTexCoordPool, min, max); }

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

	Mesh* PartialCopy() const;
	/*------------------------------------------------------
	 * Pre  : 
	 *
	 * Post : Not like the old school 2000.11.30 API, Read
	 *
	 *        A new mesh is made from this mesh based on the
	 *        currently fSelected flagged Faces.
	 *
	 *        This mesh exists outside of the scene.
	 *        If you want to add use the AddToPool() method.
	 ------------------------------------------------------*/

	Mesh* Split();
	/*------------------------------------------------------
	 * Pre  : Remove the selected faces
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

	void ExtrudeFace(index_t face, vec3_t displacement);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Extrudes <face> along vector <displacement>
	 ------------------------------------------------------*/


	bool WeldTexCoords(index_t replace, index_t texcoord);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Replace is replaced by texcoord.
	 *        
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

	//void MarkVerticesOfFacesWithFlag(Face::Flags flag, Vertex::Flags mark, bool clear);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Every face with <flag> set will set all its
	 *        vertices with flag <mark>.
	 *
	 *        If <clear> is true all vertices not marked will
	 *        have flag <mark> cleared.
	 ------------------------------------------------------*/

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
	// Transforms
	////////////////////////////////////////////////////////////

	void TransformFaceList( mstl::list<Face*>& faces, hel::Mat44& mat );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Applies matrix transform to faces in index list
	 ------------------------------------------------------*/

	void TransformVertexList( mstl::list<Vertex*>& vertices, hel::Mat44& mat );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Applies matrix transform to vertices in index list
	 ------------------------------------------------------*/

	void TransformTexcoordList( mstl::list<index_t>& texcoords, hel::Mat44& mat );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Interfaces
	////////////////////////////////////////////////////////////

	static freyja::Mesh* Cast( freyja_ptr ptr )
	{ return (freyja::Mesh*)ptr; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : FIXME Add RTTI checking.
	 *
	 ------------------------------------------------------*/

	virtual XMLSerializerNode CreateXMLSerializerNode( ) const;
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Creates serializer node for this object.
	 *
	 ------------------------------------------------------*/

	FREYJA_NODE_INTERFACE
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Node implementation.
	 *
	 ------------------------------------------------------*/

	FREYJA_XMLSERIALIZER_INTERFACE
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : XmlSerializer implementation.
	 *
	 ------------------------------------------------------*/


protected:

	////////////////////////////////////////////////////////////
	// Protected.
	////////////////////////////////////////////////////////////

	byte mMeshOptions;                     /* Options bitmap. */

	freyja::FloatArray mVertexArray;       /* Arrays to define geometry. */
	freyja::FloatArray mColorArray;
	freyja::FloatArray mNormalArray;
	freyja::FloatArray mTexcoordArray;

	mstl::list<uint32> mGaps;              /* Gaps in array usage. */

	mstl::list<Vertex*> mSelectedVertices; /* List of selected vertices. */

	mstl::list<Face*> mSelectedFaces;      /* List of selected faces. */

	mstl::list<Edge*> mSelectedEdges;      /* List of selected edges. */

	mstl::list<MeshRenderable*> mSubMeshes;

	mstl::Vector<freyja::Face*> mFaces;

	mstl::Vector<freyja::Vertex*> mVertices;

	mstl::Vector<freyja::Edge*> mEdges;
};


inline
const char* Mesh::GetType() const
{ return "Mesh"; }

 
inline
uint32 Mesh::GetVersion() const
{ return 0; }


inline
freyja::Node* Mesh::Duplicate() const
{ return new Mesh( *this ); }


} // namespace freyja

#endif // GUARD__FREYJA_MESH_H__
