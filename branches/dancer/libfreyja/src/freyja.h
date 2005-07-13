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

#ifndef GUARD__FREYJA_MONGOOSE_FREYJA__H_
#define GUARD__FREYJA_MONGOOSE_FREYJA__H_

#include <hel/math.h>
//#include <hel/Vector3d.h>
//#include <mstl/Vector.h>

#define FREYJA_API_VERSION   "Freyja 0.10.0"

/* FSM uses index_t with extentions by these error/states */
#define FREYJA_NEXT        (UINT_MAX - 1)
#define FREYJA_RESET       (UINT_MAX - 2)
#define FREYJA_CURRENT     (UINT_MAX - 3)
#define FREYJA_SIZE        (UINT_MAX - 4)
#define FREYJA_ERROR       (UINT_MAX - 5)

/* UINT_MAX 32bit */
#define INDEX_INVALID      4294967295U

extern "C" {

	typedef uint32 index_t;

	typedef unsigned char byte;

	typedef enum {
		fTransformScene = 1,
		fTransformModel,
		fTransformMesh,
		fTransformVertexFrame,
		fTransformSkeleton,
		fTransformBone,
		fTransformUVMap,
		fTransformVertexGroup,
		fTransformVertex,
		fTransformTexCoord
	
	} freyja_transform_t;


	typedef enum {
		fTranslate = 1,
		fRotate,
		fScale,
		fRotateAboutPoint,
		fScaleAboutPoint
	
	} freyja_transform_action_t;


	void freyjaPrintError(const char *format, ...);
	/*------------------------------------------------------
	 * Pre  : Format string and args are valid
	 * Post : Report messages to stderr or gPrinter
	 ------------------------------------------------------*/

	void freyjaPrintMessage(const char *format, ...);
	/*------------------------------------------------------
	 * Pre  : Format string and args are valid
	 * Post : Report messages to stdout or gPrinter
	 ------------------------------------------------------*/



	/*** TEMP ABI location for stuff in transition ****/

	///////////////////////////////////////////////////////////////////////
	// Bone
	///////////////////////////////////////////////////////////////////////

	byte freyjaIsBoneAllocated(index_t boneIndex);


	///////////////////////////////////////////////////////////////////////
	// Model
	///////////////////////////////////////////////////////////////////////

	byte freyjaGetModelFlags(index_t modelIndex);

	index_t freyjaGetModelMeshIndex(index_t modelIndex, uint32 element);

	uint32 freyjaGetModelMeshCount(index_t modelIndex);


	///////////////////////////////////////////////////////////////////////
	// TexCoord
	///////////////////////////////////////////////////////////////////////

	byte freyjaIsTexCoordAllocated(index_t texcoordIndex);

	index_t freyjaGetTexCoordPolygonRefIndex(index_t texcoordIndex, uint32 element);
	uint32 freyjaGetTexCoordPolygonRefCount(index_t texcoordIndex);

	void freyjaPolygonTexCoordPurge(index_t polygonIndex);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 * Post : All polymapped texcoords are dereferenced
	 ------------------------------------------------------*/

	void freyjaTexCoord2fv(index_t texcoordIndex, const vec2_t uv);

	void freyjaGetTexCoord2fv(index_t texcoordIndex, vec2_t uv);
	/*------------------------------------------------------
	 * Pre  : texcoord[index] exists
	 * Post : Sets passed float array to texcoord u, v
	 ------------------------------------------------------*/

	index_t freyjaTexCoordCreate2f(vec_t u, vec_t v);
	index_t freyjaTexCoordCreate2fv(vec2_t uv);
	/*------------------------------------------------------
	 * Pre  : u, v are 0.0 to 1.0
	 * Post : A new texture coordinate is created
	 *        Returns the UID of that texcoord
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	// Polygon
	///////////////////////////////////////////////////////////////////////

	byte freyjaIsPolygonAllocated(index_t polygonIndex);

	index_t freyjaPolygonCreate();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Makes invalid/empty polygon to be filled, returns index
	 ------------------------------------------------------*/

	void freyjaPolygonDelete(index_t polygonIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Removes polygon 
	 ------------------------------------------------------*/

	void freyjaPolygonSplit(index_t meshIndex, index_t polygonIndex);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 * Post : Polygon is split into 2 smaller polygons 
	 ------------------------------------------------------*/

	void freyjaPolygonExtrudeQuad1f(index_t polygonIndex, vec_t dist);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 *        the 'normal' is the vector you wish to follow with extrude
	 *
	 * Post : Adds a quad where every edge is on this face by
	 *        extruding by face normal scaled by dist
	 ------------------------------------------------------*/

	void freyjaPolygonExtrudeQuad(index_t polygonIndex, vec3_t normal);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 *        the 'normal' is the vector you wish to follow with extrude
	 *
	 * Post : Adds a quad where every edge is on this face by
	 *        extruding by 'normal'
	 ------------------------------------------------------*/

	index_t freyjaGetPolygonMaterial(index_t polygonIndex);

	byte freyjaGetPolygonFlags(index_t polygonIndex);

	uint32 freyjaGetPolygonEdgeCount(index_t polygonIndex);

	uint32 freyjaGetPolygonVertexCount(index_t polygonIndex);

	uint32 freyjaGetPolygonTexCoordCount(index_t polygonIndex);

	index_t freyjaGetPolygonVertexIndex(index_t polygonIndex, uint32 element);

	index_t freyjaGetPolygonTexCoordIndex(index_t polygonIndex, uint32 element);

	void freyjaPolygonMaterial(index_t polygonIndex, index_t materialIndex);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 * Post : Sets material for a polygon
	 ------------------------------------------------------*/

	void freyjaPolygonAddVertex(index_t polygonIndex, index_t vertexIndex);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 * Post : Adds a vertex to a polygon
	 ------------------------------------------------------*/

	void freyjaPolygonAddTexCoord(index_t polygonIndex, index_t texcoordIndex);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 * Post : Adds a texcoord to a polygon
	 ------------------------------------------------------*/
}

#endif
