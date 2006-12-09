/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the mesh ABI.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 *
 *-- History ------------------------------------------------ 
 *
 * 2006.07.09:
 * Mongoose - Created, moved from other headers
 ==========================================================================*/

#ifndef GUARD__LIBFREYJA_MESHABI_H_
#define GUARD__LIBFREYJA_MESHABI_H_

#include "freyja.h"

extern "C" {

	///////////////////////////////////////////////////////////////////////
	// MESH mutators
	///////////////////////////////////////////////////////////////////////

	index_t freyjaMeshCreate();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns UID of mesh spawned
	 ------------------------------------------------------*/

	index_t freyjaMeshCopy(index_t mesh);
	/*------------------------------------------------------
	 * Pre  : <mesh> exists
	 * Post : Returns UID of clone mesh after cloning.
	 *        Returns INDEX_INVALID if failed. 
	 ------------------------------------------------------*/

	void freyjaMeshDelete(index_t mesh);
	/*------------------------------------------------------
	 * Pre  : Mesh <mesh> exists
	 * Post : Mesh is removed from mesh pool
	 ------------------------------------------------------*/

	void freyjaMeshMaterial(index_t mesh, index_t material);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets <mesh> material reference to <material>.
	 ------------------------------------------------------*/

	void freyjaMeshPosition(index_t meshIndex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets the mesh position attribute.
	 ------------------------------------------------------*/

	void freyjaMeshName1s(index_t meshIndex, const char *name);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set human readable name for mesh.
	 ------------------------------------------------------*/

	void freyjaMeshFlags1u(uint32 flags);
	/*------------------------------------------------------
	 * Pre  : Pass valid freyja_mesh_flags_t's bitmap
	 * Post : Sets flags for current mesh
	 ------------------------------------------------------*/

	void freyjaMeshTransform3fv(index_t mesh, 
								freyja_transform_action_t action, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Generic transform function.
	 ------------------------------------------------------*/

	index_t freyjaMeshVertexCreate3fv(index_t mesh, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns mesh local index of vertex created.
	 ------------------------------------------------------*/

	void freyjaMeshVertexPos3fv(index_t mesh, index_t vertex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets vertex position.  
	 ------------------------------------------------------*/

	void freyjaMeshVertexNormal3fv(index_t mesh, index_t vertex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Vertex based normal setting.
	 ------------------------------------------------------*/

	index_t freyjaMeshTexCoordCreate2fv(index_t mesh, vec2_t uv);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns mesh local index of texcoord created
	 ------------------------------------------------------*/

	index_t freyjaMeshTexCoordCreate2f(index_t mesh, vec_t u, vec_t v);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns mesh local index of texcoord created
	 ------------------------------------------------------*/

	index_t freyjaMeshTexCoordCreate3fv(index_t mesh, vec3_t uvw);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns mesh local index of texcoord created
	 ------------------------------------------------------*/

	index_t freyjaMeshTexCoordCreate3f(index_t mesh, 
									  vec_t u, vec_t v, vec_t w);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns mesh local index of texcoord created
	 ------------------------------------------------------*/

	void freyjaMeshTexCoord2fv(index_t mesh, index_t texcoord, vec2_t uv);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets <mesh> local index of <texcoord>'s value.
	 ------------------------------------------------------*/

	void freyjaMeshVertexTexCoord3fv(index_t mesh, index_t texcoord, 
									 vec3_t uvw);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Vertex based texcoord setting.
	 ------------------------------------------------------*/

	void freyjaMeshClampTexCoords(index_t mesh);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Makes sure all UVs clamp: 0.0f <= u,v <= 1.0f
	 ------------------------------------------------------*/

	//void freyjaMeshTransformTexCoord(index_t mesh, index_t texcoord,
	//								 freyja_transform_action_t action,
	//								 vec_t x, vec_t y);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : I can't believe I ever made an ABI wrapper for this either
	 ------------------------------------------------------*/

	index_t freyjaMeshPolygonCreate(index_t mesh);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Creates polygon and returns <mesh> local index.
	 ------------------------------------------------------*/

	void freyjaMeshPolygonDelete(index_t mesh, index_t polygon);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Deletes <polygon> in <mesh>.
	 ------------------------------------------------------*/

	void freyjaMeshPolygonAddVertex1i(index_t mesh, index_t polygon, 
									  index_t vertex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Append <mesh> <vertex> to <polygon>.
	 ------------------------------------------------------*/

	void freyjaMeshPolygonAddTexCoord1i(index_t mesh, index_t polygon, 
										index_t texcoord);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Append <mesh> <texcoord> to <polygon>.
	 ------------------------------------------------------*/

	void freyjaMeshPolygonMaterial(index_t mesh, index_t polygon, 
								   index_t material);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets <mesh> <face> <material>.
	 ------------------------------------------------------*/

	void freyjaMeshPolygonClearFlag1u(index_t mesh, index_t face, byte flag);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Clears <mesh> <face> flag.
	 ------------------------------------------------------*/

	void freyjaMeshPolygonSetFlag1u(index_t mesh, index_t face, byte flag);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets <mesh> <face> flag.
	 ------------------------------------------------------*/

	void freyjaMeshPolygonGroup1u(index_t mesh, index_t face, uint32 group);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Assigns <mesh> <face> to [smoothing] <group>.
	 ------------------------------------------------------*/

	void freyjaMeshUVMapPlanar(index_t mesh);
	/*------------------------------------------------------
	 * Pre  : mesh references a valid mesh
	 * Post : Texcoords computed by Planar algorithm
	 ------------------------------------------------------*/

	void freyjaMeshUVMapSpherical(index_t mesh);
	/*------------------------------------------------------
	 * Pre  : mesh references a valid mesh
	 * Post : Texcoords computed by Spherical algorithm
	 ------------------------------------------------------*/

	void freyjaMeshUVMapCylindrical(index_t mesh);
	/*------------------------------------------------------
	 * Pre  : mesh references a valid mesh
	 * Post : Texcoords computed by Cylindrical algorithm
	 ------------------------------------------------------*/

	void freyjaMeshTesselateTriangles(index_t mesh);
	/*------------------------------------------------------
	 * Pre  : mesh references a valid mesh
	 * Post : Divides all polygons in mesh into triangles
	 ------------------------------------------------------*/

	void freyjaMeshNormalFlip(index_t mesh);
	/*------------------------------------------------------
	 * Pre  : mesh references a valid mesh
	 * Post : Flips all vertex normals in mesh
	 ------------------------------------------------------*/

	void freyjaMeshGenerateVertexNormals(index_t mesh);
	/*------------------------------------------------------
	 * Pre  : mesh references a valid mesh
	 * Post : Recalculates all vertex normals in mesh as
	 *        a single smoothing group.
	 ------------------------------------------------------*/

	void freyjaMeshPolygonExtrudeQuad1f(index_t mesh, index_t quad,	vec3_t n);
	/*------------------------------------------------------
	 * Pre  : Polygon index <quad> is a quadrilateral.
	 * Post : Extrudes <quad> over normal <n> in <mesh>.
	 ------------------------------------------------------*/

	void freyjaMeshVertexWeight(index_t mesh, index_t vertex, 
								index_t bone, vec_t weight);
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

	index_t freyjaMeshTexCoordWeld(index_t mesh, index_t a, index_t b);
	/*------------------------------------------------------
	 * Pre  : Texcoords <a> and <b> exist in <mesh>
	 * Post : Replaces all references in <mesh> to <b> with <a>
	 ------------------------------------------------------*/

	index_t freyjaMeshVertexWeld(index_t mesh, index_t a, index_t b);
	/*------------------------------------------------------
	 * Pre  : Vertices <a> and <b> exist in <mesh>
	 * Post : Replaces all references in <mesh> to <b> with <a>
	 ------------------------------------------------------*/

	void freyjaMeshUpdateBlendVertices(index_t mesh, index_t track, vec_t time);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Updates <mesh> blend vertex array for
	 *        animation at <time> for <skeleton> and <track>
	 ------------------------------------------------------*/

	index_t freyjaMeshVertexTrackNew(index_t mesh, vec_t duration, vec_t rate);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Make a new vertex animation track for mesh
	 ------------------------------------------------------*/
	
	index_t freyjaMeshVertexKeyFrameNew(index_t mesh, index_t track, 
										vec_t time);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Make a new vertex animation keyframe for track
	 ------------------------------------------------------*/

	void freyjaMeshVertexKeyFrame3f(index_t mesh, index_t track, index_t key,
									uint32 vert, vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Update vertex position in keyframe
	 ------------------------------------------------------*/

	void freyjaMeshPolygonSplit(index_t mesh, index_t polygon);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Randomly splits face ( adding guided split later ).
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	// MESH accessors
	///////////////////////////////////////////////////////////////////////

	uint32 freyjaGetMeshCount();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns number of mesh UIDs used.
	 ------------------------------------------------------*/

	char freyjaIsMeshAllocated(index_t mesh);
	/*------------------------------------------------------
	 * Pre  : Given UID of <mesh>.
	 * Post : Returns 1 if <mesh> is allocated, or 0.
	 ------------------------------------------------------*/

	void freyjaGetMeshBoundingBox(index_t mesh, vec3_t min, vec3_t max);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns min and max of axis aligned bounding volume.
	 ------------------------------------------------------*/

	void freyjaGetMeshBoundingSphere(index_t mesh, 
									 vec3_t origin, vec_t &radius);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns min and max of spherical bounding volume.
	 ------------------------------------------------------*/

	uint32 freyjaGetMeshFlags(index_t mesh);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns option bitflags for <mesh>.
	 ------------------------------------------------------*/

	void freyjaGetMeshPosition(index_t mesh, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns <mesh> position attribute.
	 ------------------------------------------------------*/

	const char *freyjaGetMeshNameString(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns <mesh> human readable name.
	 ------------------------------------------------------*/

	void freyjaGetMeshName1s(index_t mesh, uint32 lenght, char *name);
	/*------------------------------------------------------
	 * Pre  : <name> is allocated with <lenght> bytes.
	 * Post : Returns <mesh> human readable inside buffer <name>.
	 ------------------------------------------------------*/

	byte freyjaGetMeshVertexFlags(index_t mesh, index_t vertex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns <mesh> <vertex> flags.
	 ------------------------------------------------------*/

	void freyjaGetMeshVertexPos3fv(index_t mesh, index_t vertex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns <mesh> <vertex> pos.
	 ------------------------------------------------------*/

	void freyjaGetMeshVertexNormal3fv(index_t mesh, index_t vertex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns <mesh> <vertex> normal.
	 ------------------------------------------------------*/

	void freyjaGetMeshVertexTexCoord3fv(index_t mesh, index_t vertex,
										vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns <mesh> <vertex> texcoord.
	 ------------------------------------------------------*/

	index_t freyjaGetMeshVertexTexCoord(index_t mesh, index_t vertex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns <mesh> <vertex> texcoord index.
	 ------------------------------------------------------*/

	uint32 freyjaGetMeshVertexPolygonRefCount(index_t mesh, index_t vertex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns <mesh> <vertex> face reference count.
	 ------------------------------------------------------*/

	index_t freyjaGetMeshVertexPolygonRefIndex(index_t mesh,
											   index_t vertex, uint32 element);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns <mesh> <vertex> face reference index for <element>.
	 ------------------------------------------------------*/

	void freyjaGetMeshTexCoord2fv(index_t mesh, index_t texcoord, vec2_t uv);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets <mesh> local index of <texcoord>'s value.
	 ------------------------------------------------------*/

	byte freyjaGetMeshPolygonFlags(index_t mesh, index_t polygon);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns <mesh> <polygon> flags.
	 ------------------------------------------------------*/

	uint32 freyjaGetMeshPolygonEdgeCount(index_t mesh, index_t polygon);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns vertex count for <polygon>.
	 ------------------------------------------------------*/

	uint32 freyjaGetMeshPolygonVertexCount(index_t mesh, index_t polygon);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns vertex count for <polygon>.
	 ------------------------------------------------------*/

	index_t freyjaGetMeshPolygonVertexIndex(index_t mesh, index_t polygon,
											uint32 element);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns vertex index for <element> of <polygon>.
	 ------------------------------------------------------*/

	uint32 freyjaGetMeshPolygonTexCoordCount(index_t mesh, index_t polygon);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns texcoord count for <polygon>.
	 ------------------------------------------------------*/

	index_t freyjaGetMeshPolygonTexCoordIndex(index_t mesh, index_t polygon,
											  uint32 element);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns texcoord index for <element> of <polygon>.
	 ------------------------------------------------------*/

	index_t freyjaGetMeshPolygonMaterial(index_t mesh, index_t polygon);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns material reference for <polygon>.
	 ------------------------------------------------------*/

	uint32 freyjaGetMeshTexCoordCount(index_t mesh);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns texcoord count for <mesh>.
	 ------------------------------------------------------*/

	uint32 freyjaGetMeshVertexCount(index_t mesh);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns vertex count for <mesh>.
	 ------------------------------------------------------*/

	uint32 freyjaGetMeshTexCoordCount(index_t mesh);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns texcoord count for <mesh>.
	 ------------------------------------------------------*/

	uint32 freyjaGetMeshPolygonCount(index_t mesh);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns polygon count for <mesh>.
	 ------------------------------------------------------*/

	vec_t *freyjaGetMeshBlendVertices(index_t mesh);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Retrieves <mesh> blend vertex array for rendering, etc
	 ------------------------------------------------------*/

	void freyjaGetMeshWeight(index_t mesh, index_t weight,
							 index_t &vertex, index_t &bone, vec_t &weightv);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets <weight> values from <mesh>
	 ------------------------------------------------------*/

	uint32 freyjaGetMeshWeightCount(index_t mesh);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets number of vertex:bone weights in mesh
	 ------------------------------------------------------*/

	index_t freyjaGetMeshVertexTrackCount(index_t mesh);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets number of tracks in <mesh>
	 ------------------------------------------------------*/

	index_t freyjaGetMeshVertexKeyFrameCount(index_t mesh, index_t track);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets number of keyframes in <track> of <mesh>
	 ------------------------------------------------------*/
}


#   if defined( __cplusplus ) && defined( USING_FREYJA_CPP_ABI )
#      include <mstl/SystemIO.h>
#      include <freyja/Mesh.h>

    freyja::Mesh *freyjaGetMeshClass(index_t meshUID); 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns pointer to Mesh with UID or NULL
	 ------------------------------------------------------*/

    freyja::Weight *freyjaGetMeshWeightClass(index_t meshUID, index_t weight);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns pointer to Weight with UID or NULL
	 ------------------------------------------------------*/

    freyja::Vertex *freyjaGetMeshVertexClass(index_t meshUID, index_t vertex); 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns pointer to Vertex with UID or NULL
	 ------------------------------------------------------*/

    freyja::Face *freyjaGetMeshFaceClass(index_t meshUID, index_t face); 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns pointer to Face with UID or NULL
	 ------------------------------------------------------*/

    int32 freyjaMeshLoadChunkJA(mstl::SystemIO::FileReader &r,
								freyja_file_chunk_t &chunk);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Serialiser for JA format mesh chunk
	 ------------------------------------------------------*/

    int32 freyjaMeshSaveChunkJA(mstl::SystemIO::FileWriter &w, index_t mesh);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Serialiser for JA format mesh chunk
	 ------------------------------------------------------*/

    bool freyjaMeshSaveChunkTextJA(SystemIO::TextFileWriter &w, index_t mesh);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Serialiser for JA format mesh chunk
	 ------------------------------------------------------*/

    bool freyjaMeshLoadChunkTextJA(SystemIO::TextFileReader &r);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Serialiser for JA format mesh chunk
	 ------------------------------------------------------*/

#   endif

#endif


