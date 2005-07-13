/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the mesh data model class.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 *
 *-- History ------------------------------------------------ 
 *
 * 2005.07.12:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_FREYJAMESHABI_H_
#define GUARD__FREYJA_MONGOOSE_FREYJAMESHABI_H_

#include "freyja.h"

extern "C" {

	// FREYJA_MESH Accessors
	byte freyjaIsMeshAllocated(index_t meshIndex);

	byte freyjaGetMeshFlags(index_t meshIndex);

	void freyjaMeshFlags(index_t meshIndex, byte flags);

	void freyjaGetMeshPosition(index_t meshIndex, vec3_t xyz);

	char *freyjaGetMeshNameString(index_t meshIndex); // don't alter string

	void freyjaGetMeshName1s(index_t meshIndex, uint32 lenght, char *name);

	uint32 freyjaGetMeshTexCoordCount(index_t meshIndex);

	index_t freyjaGetMeshTexCoordIndex(index_t meshIndex, uint32 element);

	index_t freyjaGetMeshVertexIndex(index_t meshIndex, uint32 element);
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

	index_t freyjaGetMeshPolygonVertexIndex(index_t meshIndex, index_t faceVertexIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the index of local vertices
	 *        for mesh <meshIndex> with local
	 *        mapping from polygon to mesh ids or -1 on error
	 *
	 ------------------------------------------------------*/
	



	index_t freyjaGetMeshPolygonIndex(index_t meshIndex, uint32 element);

	uint32 freyjaGetMeshPolygonCount(index_t meshIndex);

	index_t freyjaGetMeshSmoohingGroupIndex(index_t meshIndex, uint32 element);

	uint32 freyjaGetMeshSmoothingGroupCount(index_t meshIndex);

	void freyjaMeshTransform(index_t meshIndex,
								freyja_transform_action_t action, 
								vec_t x, vec_t y, vec_t z);

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

	void freyjaMeshDelete(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : Mesh <meshIndex> exists
	 * Post : Mesh is removed from mesh pool
	 ------------------------------------------------------*/

	void freyjaMeshClampTexCoords(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Makes sure all UVs are inside 0,0 - 1,1
	 ------------------------------------------------------*/

	void freyjaMeshMaterial(index_t meshIndex, index_t materialIndex);

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

	void freyjaMeshUVMapPlanar(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Texcoords computed with Planar mapping algorithm
	 *
	 ------------------------------------------------------*/

	void freyjaMeshUVMapSpherical(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Texcoords computed with Spherical mapping algorithm
	 *
	 ------------------------------------------------------*/

	void freyjaMeshUVMapCylindrical(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Texcoords computed with Cylindrical mapping algorithm
	 *
	 ------------------------------------------------------*/

	void freyjaMeshTesselateTriangles(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Divides all polygons in mesh into triangles
	 *
	 ------------------------------------------------------*/

	void freyjaMeshNormalFlip(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Flips all vertex normals in mesh
	 *
	 ------------------------------------------------------*/

	void freyjaMeshGenerateVertexNormals(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Recalculates all vertex normals in mesh
	 *
	 ------------------------------------------------------*/

	void freyjaMeshPosition(index_t meshIndex, vec3_t xyz);

	void freyjaMeshName1s(index_t meshIndex, const char *name);

	void freyjaMeshFlags1u(unsigned int flags);
	/*------------------------------------------------------
	 * Pre  : Pass valid freyja_mesh_flags_t's bitmap
	 * Post : Sets flags for current mesh
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.05.16:
	 * Mongoose - Created
	 ------------------------------------------------------*/


}

#endif
