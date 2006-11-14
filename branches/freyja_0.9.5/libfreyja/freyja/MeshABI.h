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


#ifndef GUARD__FREYJA_MONGOOSE_FREYJAMESHABI_H_
#define GUARD__FREYJA_MONGOOSE_FREYJAMESHABI_H_

#include "freyja.h"

extern "C" {

	// TMP model stuff
	uint32 freyjaModelGetMeshCount(index_t modelIndex);

	void freyjaModelMeshTransform3fv(index_t modelIndex, index_t meshIndex, 
									freyja_transform_action_t action, vec3_t xyz);


	///////////////////////////////////////////////////////////////////////
	// MESH
	///////////////////////////////////////////////////////////////////////


	index_t freyjaMeshCreate();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns index of mesh spawned
	 ------------------------------------------------------*/

	index_t freyjaGetCurrentMesh();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns index using meshIndex state 
	 ------------------------------------------------------*/

	void freyjaCurrentMesh(index_t idx);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Sets index, which is useful for 0.9.3 usage 
	 ------------------------------------------------------*/

	void freyjaMeshDelete(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : Mesh <meshIndex> exists
	 * Post : Mesh is removed from mesh pool
	 ------------------------------------------------------*/

	index_t freyjaMeshVertexCreate3f(index_t meshIndex, 
									 index_t groupIndex,
									 vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : Note in 0.9.3 API group isn't smoothing group
	 *        After 0.10 merge it will be
	 *
	 * Post : Returns index of vertex created
	 ------------------------------------------------------*/

	void freyjaMeshVertexTranslate3fv(index_t meshIndex, 
									  index_t vertexIndex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Alter a single vertex position
	 ------------------------------------------------------*/


	void freyjaMeshTransform(index_t meshIndex, uint32 frame,
								freyja_transform_action_t action, 
								vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaMeshFrameTransform(index_t meshIndex, uint32 frame,
									freyja_transform_action_t action, 
									vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaMeshClampTexCoords(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Makes sure all UVs are inside 0,0 - 1,1
	 ------------------------------------------------------*/

	void freyjaMeshMaterial(index_t meshIndex, index_t materialIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaMeshFrameCenter(index_t meshIndex, uint32 frame, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaGetMeshFrameCenter(index_t meshIndex, uint32 frame, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaMeshPromoteTexcoordsToPloymapping(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Takes vertex UVs and promotes them to polymapped
	 *        texcoords ( stored in polygon ala texture polygons )
	 ------------------------------------------------------*/

	void freyjaMeshAddPolygon(index_t meshIndex, index_t polygonIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaMeshRemovePolygon(index_t meshIndex, index_t polygonIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Removes references to polygonIndex in mesh, but
	 *        it doesn't free the allocated polygon
	 ------------------------------------------------------*/

	void freyjaMeshPolygonSplitTexCoords(index_t meshIndex,
                                         index_t polygonIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : All TexCoords are duplicated, then references to old
	 *        TexCoords are removed -- this is useful for making a single
	 *        polymapped texture polygon when all others will remain
	 *        in a UV Mesh
	 ------------------------------------------------------*/

	void freyjaMeshUVMapPlanar(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Texcoords computed with Planar mapping algorithm
	 ------------------------------------------------------*/

	void freyjaMeshUVMapSpherical(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Texcoords computed with Spherical mapping algorithm
	 ------------------------------------------------------*/

	void freyjaMeshUVMapCylindrical(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Texcoords computed with Cylindrical mapping algorithm
	 ------------------------------------------------------*/

	void freyjaMeshTesselateTriangles(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Divides all polygons in mesh into triangles
	 ------------------------------------------------------*/

	void freyjaMeshNormalFlip(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Flips all vertex normals in mesh
	 ------------------------------------------------------*/

	void freyjaMeshGenerateVertexNormals(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Recalculates all vertex normals in mesh
	 ------------------------------------------------------*/

	void freyjaMeshPosition(index_t meshIndex, vec3_t xyz);

	void freyjaMeshName1s(index_t meshIndex, const char *name);

	void freyjaMeshFlags1u(uint32 flags);
	/*------------------------------------------------------
	 * Pre  : Pass valid freyja_mesh_flags_t's bitmap
	 * Post : Sets flags for current mesh
	 ------------------------------------------------------*/

	void freyjaMeshPolygonExtrudeQuad1f(index_t meshIndex, index_t polygonIndex,
 vec3_t v);


	// Mesh accessors //////////////

	char freyjaIsMeshAllocated(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaGetMeshFrameBoundingBox(index_t meshIndex, 
										uint32 frame, vec3_t min, vec3_t max);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	index_t freyjaModelMeshPolygonCreate(index_t modelIndex, index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	uint32 freyjaGetMeshFlags(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaGetMeshPosition(index_t meshIndex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	const char *freyjaGetMeshNameString(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaGetMeshName1s(index_t meshIndex, uint32 lenght, char *name);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	uint32 freyjaGetMeshTexCoordCount(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	index_t freyjaGetMeshTexCoordIndex(index_t meshIndex, index_t element);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	index_t freyjaGetMeshVertexIndex(index_t meshIndex, index_t element);
	/*------------------------------------------------------
	 * Pre  : freyjaGetMeshVertexCount must be called before
	 *        this is valid while using the Egg backend
	 *
	 * Post : Returns the index of the local vertex <element>
	 *        for mesh <meshIndex> or -1 on error
	 ------------------------------------------------------*/

	uint32 freyjaGetMeshVertexCount(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the number of local vertices
	 *        for mesh <meshIndex> or 0 on error
	 *
	 ------------------------------------------------------*/

	index_t freyjaGetMeshPolygonVertexIndex(index_t meshIndex,
                                            index_t faceVertexIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the index of local vertices
	 *        for mesh <meshIndex> with local
	 *        mapping from polygon to mesh ids or -1 on error
	 *
	 ------------------------------------------------------*/

	// 'free vertices' outside of polygons as well as 'grouped' vertices
	void freyjaMeshVertexGroupAppendGobalVertex(index_t meshIndex, uint32 element, 
												index_t vertexIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	uint32 freyjaGetMeshVertexGroupVertexCount(index_t meshIndex, uint32 element);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	index_t freyjaGetMeshVertexGroupVertexIndex(index_t meshIndex, uint32 element,
											  uint32 vertexElement);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	index_t freyjaGetMeshPolygonIndex(index_t meshIndex, uint32 element);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	uint32 freyjaGetMeshPolygonCount(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	index_t freyjaGetMeshVertexGroupIndex(index_t meshIndex, uint32 element);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	uint32 freyjaGetMeshVertexGroupCount(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	index_t freyjaGetMeshVertexFrameIndex(index_t meshIndex, uint32 element); // Not Implemented
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	uint32 freyjaGetMeshVertexFrameCount(index_t meshIndex); // Not Implemented
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	// 0.9.5 EXT

	void freyjaMeshPolygonDelete(index_t meshIndex, index_t polygonIndex);

	index_t freyjaGetFSMMeshIndex();
}


#   ifdef __cplusplus
#      include <freyja/Mesh.h>
freyja::Mesh *freyjaModelGetMeshClass(index_t modelIndex, index_t meshIndex);
#   endif

#endif


