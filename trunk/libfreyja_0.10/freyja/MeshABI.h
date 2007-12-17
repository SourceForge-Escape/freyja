/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2005-2007 Mongoose
 * Comments: This is the mesh ABI.
 *
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.12.12:
 * Mongoose - Refactored.
 *
 * 2006.07.09:
 * Mongoose - Created, moved from other headers
 ==========================================================================*/

#ifndef GUARD__FREYJA_MESHABI_H_
#define GUARD__FREYJA_MESHABI_H_

#include "freyja.h"


extern "C" {

	///////////////////////////////////////////////////////////////////////
	// Mesh 0.10.0 ABI
	//
	// Implements: Node, SceneNode, XMLSerializer
	///////////////////////////////////////////////////////////////////////

	freyja_ptr freyjaMeshCreate( const char* name );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns pointer of mesh spawned.
	 *        The mesh won't be part of any scene by default.
	 ------------------------------------------------------*/

	void freyjaMeshDelete( freyja_ptr mesh );
	/*------------------------------------------------------
	 * Pre  : Mesh exists.
	 * Post : Mesh is deleted and removed from any graphs.
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	// freyjaMeshGet
	///////////////////////////////////////////////////////////////////////

    freyja_ptr freyjaMeshGetVertex( freyja_ptr mesh, freyja_id vertex ); 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns pointer to Vertex with UID or NULL.
	 ------------------------------------------------------*/

    freyja_ptr freyjaMeshGetFace( freyja_ptr mesh, freyja_id face ); 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns pointer to Face with UID or NULL.
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	// freyjaMeshGetWeight
	///////////////////////////////////////////////////////////////////////

    freyja_ptr freyjaMeshGetWeight( freyja_ptr mesh, freyja_id weight );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns pointer to Weight with UID or NULL.
	 ------------------------------------------------------*/

	uint32 freyjaMeshGetWeightCount(freyja_ptr mesh);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets number of vertex:bone weights in mesh
	 ------------------------------------------------------*/




	///////////////////////////////////////////////////////////////////////
	// freyjaMeshSet
	///////////////////////////////////////////////////////////////////////

	void freyjaMeshSetMaterial( freyja_ptr mesh, freyja_ptr material );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets <mesh> material reference to <material>.
	 ------------------------------------------------------*/

	void freyjaMeshSetPosition( freyja_ptr meshIndex, vec3_t xyz );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets the mesh position attribute.
	 ------------------------------------------------------*/



	////////////////////////////////////////////////////////////////
	// Mesh utilities ABI
	////////////////////////////////////////////////////////////////

	freyja_ptr freyjaMeshCreateSheet(vec3_t origin, vec_t size, 
									 uint32 rows, uint32 columns);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Quadrilateral mesh sheet is created.
	 *
	 ------------------------------------------------------*/

	freyja_ptr freyjaMeshCreateLattice(vec3_t origin, vec_t size, 
									   uint32 rows, uint32 columns, uint32 layers);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Quadrilateral lattice mesh is created.
	 *        <layers> tall with <rows> x <cols> of quads div per layer.
	 *
	 *        Useful for quickier extruding.
	 *
	 ------------------------------------------------------*/

	freyja_ptr freyjaMeshCreateCube(vec3_t origin, vec_t size);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Quadrilateral mesh cube is created.
	 *
	 ------------------------------------------------------*/

	freyja_ptr freyjaMeshCreateRing(vec3_t origin, vec_t radius, uint32 count,
									uint32 rings);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Triangular center point circle mesh is created.
	 *
	 ------------------------------------------------------*/

	freyja_ptr freyjaMeshCreateCircle(vec3_t origin, vec_t radius, uint32 count);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Triangular center point circle mesh is created.
	 *
	 ------------------------------------------------------*/

	freyja_ptr freyjaMeshCreateCone(vec3_t origin, vec_t height, vec_t radius,
									uint32 wedges);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Triangular center point cone mesh is created.
	 *
	 ------------------------------------------------------*/

	freyja_ptr freyjaMeshCreateCylinder(vec3_t origin, vec_t height, vec_t radius, 
										uint32 sides, uint32 rings); 
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Triangular center point capped, 
	 *        quadrilateral cylinder mesh is created.
	 *
	 ------------------------------------------------------*/

	freyja_ptr freyjaMeshCreateSphere(vec3_t origin, vec_t radius, 
									  uint32 sides, uint32 rings);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	freyja_ptr freyjaMeshCreateTube(vec3_t origin, vec_t height, vec_t radius, 
									uint32 sides, uint32 rings);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Quadrilateral cylinder mesh is created.
	 *
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	// Vertex
	///////////////////////////////////////////////////////////////////////

	void freyjaVertexGetTexCoord2fv( freyja_ptr vertex, vec2_t uv );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns <mesh> <vertex> texcoord index.
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	// Weight
	///////////////////////////////////////////////////////////////////////

	void freyjaWeightCreate( freyja_ptr mesh, 
							 freyja_ptr vertex, freyja_ptr bone, vec_t weight );
	/*------------------------------------------------------------
	 * Pre  : All <weight>s for <vertex> combined must be 1.0
	 *        Each weight must fit 0.0f < w <= 1.0f
	 *
	 * Post : Sets <weight> of influence of <bone> on <vertex> in <mesh>
	 *
	 *        If there is already a weight for the corresponding
	 *        bone it is replaced.
	 *
	 -----------------------------------------------------------*/

	freyja_ptr freyjaWeightGetBone( freyja_ptr weight );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets <weight> attribute bone.
	 ------------------------------------------------------*/

	vec_t freyjaWeightGetValue( freyja_ptr weight );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets <weight> attribute weight value.
	 ------------------------------------------------------*/

	freyja_ptr freyjaWeightGetVertex( freyja_ptr weight );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets <weight> attribute vertex.
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	// MESH mutators
	///////////////////////////////////////////////////////////////////////

	// FIXME: Make this functional instead of based on action value.
	void freyjaTransform3fv(freyja_ptr mesh, 
								freyja_transform_action_t action, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Generic transform function.
	 ------------------------------------------------------*/

	freyja_ptr freyjaMeshVertexCreate3fv(freyja_ptr mesh, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns mesh local index of vertex created.
	 ------------------------------------------------------*/

	void freyjaMeshVertexPos3fv(freyja_ptr mesh, freyja_ptr vertex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets vertex position.  
	 ------------------------------------------------------*/

	void freyjaMeshVertexNormal3fv(freyja_ptr mesh, freyja_ptr vertex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Vertex based normal setting.
	 ------------------------------------------------------*/

	freyja_ptr freyjaMeshTexCoordCreate2fv(freyja_ptr mesh, vec2_t uv);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns mesh local index of texcoord created
	 ------------------------------------------------------*/

	freyja_ptr freyjaMeshTexCoordCreate2f(freyja_ptr mesh, vec_t u, vec_t v);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns mesh local index of texcoord created
	 ------------------------------------------------------*/

	void freyjaMeshClampTexCoords(freyja_ptr mesh);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Makes sure all UVs clamp: 0.0f <= u,v <= 1.0f
	 ------------------------------------------------------*/

	freyja_ptr freyjaMeshPolygonCreate(freyja_ptr mesh);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Creates polygon and returns <mesh> local index.
	 ------------------------------------------------------*/

	void freyjaMeshPolygonDelete(freyja_ptr mesh, freyja_ptr polygon);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Deletes <polygon> in <mesh>.
	 ------------------------------------------------------*/

	void freyjaMeshPolygonAddVertex1i(freyja_ptr mesh, freyja_ptr polygon, 
									  freyja_ptr vertex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Append <mesh> <vertex> to <polygon>.
	 ------------------------------------------------------*/

	void freyjaMeshPolygonAddTexCoord1i(freyja_ptr mesh, freyja_ptr polygon, 
										freyja_ptr texcoord);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Append <mesh> <texcoord> to <polygon>.
	 ------------------------------------------------------*/

	void freyjaMeshPolygonMaterial(freyja_ptr mesh, freyja_ptr polygon, 
								   freyja_ptr material);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets <mesh> <face> <material>.
	 ------------------------------------------------------*/

	void freyjaMeshPolygonFlipNormal(freyja_ptr mesh, freyja_ptr face);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Flips <mesh> <face> normal.
	 ------------------------------------------------------*/

	void freyjaMeshPolygonComputeNormal(freyja_ptr mesh, freyja_ptr face);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets <mesh> <face> normal from vertices.
	 ------------------------------------------------------*/

	void freyjaMeshPolygonNormal(freyja_ptr mesh, freyja_ptr face, vec3_t normal);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets <mesh> <face> <normal>.
	 ------------------------------------------------------*/

	void freyjaMeshPolygonClearFlag1u(freyja_ptr mesh, freyja_ptr face, byte flag);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Clears <mesh> <face> flag.
	 ------------------------------------------------------*/

	void freyjaMeshPolygonSetFlag1u(freyja_ptr mesh, freyja_ptr face, byte flag);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets <mesh> <face> flag.
	 ------------------------------------------------------*/

	void freyjaMeshPolygonGroup1u(freyja_ptr mesh, freyja_ptr face, uint32 group);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Assigns <mesh> <face> to [smoothing] <group>.
	 ------------------------------------------------------*/

	void freyjaMeshUVMapPlanar(freyja_ptr mesh);
	/*------------------------------------------------------
	 * Pre  : mesh references a valid mesh
	 * Post : Texcoords computed by Planar algorithm
	 ------------------------------------------------------*/

	void freyjaMeshUVMapSpherical(freyja_ptr mesh);
	/*------------------------------------------------------
	 * Pre  : mesh references a valid mesh
	 * Post : Texcoords computed by Spherical algorithm
	 ------------------------------------------------------*/

	void freyjaMeshUVMapCylindrical(freyja_ptr mesh);
	/*------------------------------------------------------
	 * Pre  : mesh references a valid mesh
	 * Post : Texcoords computed by Cylindrical algorithm
	 ------------------------------------------------------*/

	void freyjaMeshTesselateTriangles(freyja_ptr mesh);
	/*------------------------------------------------------
	 * Pre  : mesh references a valid mesh
	 * Post : Divides all polygons in mesh into triangles
	 ------------------------------------------------------*/

	void freyjaMeshNormalFlip(freyja_ptr mesh);
	/*------------------------------------------------------
	 * Pre  : mesh references a valid mesh
	 * Post : Flips all vertex normals in mesh
	 ------------------------------------------------------*/

	void freyjaMeshGenerateVertexNormals(freyja_ptr mesh);
	/*------------------------------------------------------
	 * Pre  : mesh references a valid mesh
	 * Post : Recalculates all vertex normals in mesh as
	 *        a single smoothing group.
	 ------------------------------------------------------*/

	void freyjaMeshPolygonExtrudeQuad1f(freyja_ptr mesh, freyja_ptr quad,	vec3_t n);
	/*------------------------------------------------------
	 * Pre  : Polygon index <quad> is a quadrilateral.
	 * Post : Extrudes <quad> over normal <n> in <mesh>.
	 ------------------------------------------------------*/

	freyja_ptr freyjaMeshTexCoordWeld(freyja_ptr mesh, freyja_ptr a, freyja_ptr b);
	/*------------------------------------------------------
	 * Pre  : Texcoords <a> and <b> exist in <mesh>
	 * Post : Replaces all references in <mesh> to <b> with <a>
	 ------------------------------------------------------*/

	freyja_ptr freyjaMeshVertexWeld(freyja_ptr mesh, freyja_ptr a, freyja_ptr b);
	/*------------------------------------------------------
	 * Pre  : Vertices <a> and <b> exist in <mesh>
	 * Post : Replaces all references in <mesh> to <b> with <a>
	 ------------------------------------------------------*/

	void freyjaMeshUpdateBlendVertices(freyja_ptr mesh, freyja_ptr track, vec_t time);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Updates <mesh> blend vertex array for
	 *        animation at <time> for <skeleton> and <track>
	 ------------------------------------------------------*/

	freyja_ptr freyjaMeshVertexTrackNew(freyja_ptr mesh, vec_t duration, vec_t rate);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Make a new vertex animation track for mesh
	 ------------------------------------------------------*/
	
	freyja_ptr freyjaMeshVertexKeyFrameNew(freyja_ptr mesh, freyja_ptr track, 
										vec_t time);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Make a new vertex animation keyframe for track
	 ------------------------------------------------------*/

	void freyjaMeshVertexKeyFrame3f(freyja_ptr mesh, freyja_ptr track, freyja_ptr key,
									uint32 vert, vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Update vertex position in keyframe
	 ------------------------------------------------------*/

	void freyjaMeshPolygonSplit(freyja_ptr mesh, freyja_ptr polygon);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Randomly splits face ( adding guided split later ).
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	// MESH accessors
	///////////////////////////////////////////////////////////////////////

	byte freyjaGetMeshVertexFlags(freyja_ptr mesh, freyja_ptr vertex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns <mesh> <vertex> flags.
	 ------------------------------------------------------*/

	void freyjaGetMeshVertexPos3fv(freyja_ptr mesh, freyja_ptr vertex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns <mesh> <vertex> pos.
	 ------------------------------------------------------*/

	void freyjaGetMeshVertexNormal3fv(freyja_ptr mesh, freyja_ptr vertex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns <mesh> <vertex> normal.
	 ------------------------------------------------------*/

	void freyjaGetMeshVertexTexCoord3fv(freyja_ptr mesh, freyja_ptr vert,	vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns <mesh> <vertex> texcoord.
	 ------------------------------------------------------*/


	uint32 freyjaGetMeshVertexPolygonRefCount(freyja_ptr mesh, freyja_ptr vertex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns <mesh> <vertex> face reference count.
	 ------------------------------------------------------*/

	freyja_ptr freyjaGetMeshVertexPolygonRefIndex(freyja_ptr mesh,
											   freyja_ptr vertex, uint32 element);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns <mesh> <vertex> face reference index for <element>.
	 ------------------------------------------------------*/

	void freyjaGetMeshTexCoord2fv(freyja_ptr mesh, freyja_ptr texcoord, vec2_t uv);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets <mesh> local index of <texcoord>'s value.
	 ------------------------------------------------------*/

	byte freyjaGetMeshPolygonFlags(freyja_ptr mesh, freyja_ptr polygon);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns <mesh> <polygon> flags.
	 ------------------------------------------------------*/

	byte freyjaGetMeshPolygonGroup(freyja_ptr mesh, freyja_ptr polygon);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns <mesh> <polygon> group.
	 ------------------------------------------------------*/

	uint32 freyjaGetMeshPolygonEdgeCount(freyja_ptr mesh, freyja_ptr polygon);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns vertex count for <polygon>.
	 ------------------------------------------------------*/

	uint32 freyjaGetMeshPolygonVertexCount(freyja_ptr mesh, freyja_ptr polygon);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns vertex count for <polygon>.
	 ------------------------------------------------------*/

	freyja_ptr freyjaGetMeshPolygonVertexIndex(freyja_ptr mesh, freyja_ptr polygon,
											uint32 element);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns vertex index for <element> of <polygon>.
	 ------------------------------------------------------*/

	uint32 freyjaGetMeshPolygonTexCoordCount(freyja_ptr mesh, freyja_ptr polygon);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns texcoord count for <polygon>.
	 ------------------------------------------------------*/

	freyja_ptr freyjaGetMeshPolygonTexCoordIndex(freyja_ptr mesh, freyja_ptr polygon,
											  uint32 element);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns texcoord index for <element> of <polygon>.
	 ------------------------------------------------------*/

	freyja_ptr freyjaGetMeshPolygonMaterial(freyja_ptr mesh, freyja_ptr polygon);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns material reference for <polygon>.
	 ------------------------------------------------------*/

	uint32 freyjaGetMeshTexCoordCount(freyja_ptr mesh);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns texcoord count for <mesh>.
	 ------------------------------------------------------*/

	uint32 freyjaGetMeshVertexCount(freyja_ptr mesh);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns vertex count for <mesh>.
	 ------------------------------------------------------*/

	uint32 freyjaGetMeshNormalCount(freyja_ptr mesh);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns normal count for <mesh>.
	 ------------------------------------------------------*/

	uint32 freyjaGetMeshPolygonCount(freyja_ptr mesh);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns polygon count for <mesh>.
	 ------------------------------------------------------*/

	vec_t *freyjaGetMeshBlendVertices(freyja_ptr mesh);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Retrieves <mesh> blend vertex array for rendering, etc
	 ------------------------------------------------------*/

	uint32 freyjaGetMeshVertexTrackCount(freyja_ptr mesh);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets number of tracks in <mesh>
	 ------------------------------------------------------*/

	uint32 freyjaGetMeshVertexKeyFrameCount(freyja_ptr mesh, freyja_ptr track);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets number of keyframes in <track> of <mesh>
	 ------------------------------------------------------*/


} // extern "C"

#endif // GUARD__FREYJA_MESHABI_H_
