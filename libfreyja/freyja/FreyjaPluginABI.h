/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : GPL, also (C) 2000 Mongoose
 * Comments: This is the Freyja plugin ABI/API.
 * 
 *           
 *-- History ------------------------------------------------ 
 *
 * 2005.01.03: (v0.9.1)
 * Mongoose - Using Freyja versioning numbers for ABI reporting.
 *
 *            Lots of pruning and documentation updates to the ABI.  Also
 *            some new features to go with the refactoring.
 *
 *
 * 2004.12.17: (v0.0.9)
 * Mongoose -  Created, FreyjaPlugin ABI interface defination refinement
 *             based on FreyjaPlugin and EggPlugin.
 *
 *             This common interface makes it possible to share ABI 
 *             compatible binary C/C++ plugins between FreyjaPlugin and
 *             EggPlugin.  ( Chimera and Umbra branches merged )
 *
 ==========================================================================*/

#ifndef GUARD__MONGOOSE_FREYJAPLUGINABI_H
#define GUARD__MONGOOSE_FREYJAPLUGINABI_H

#include <stdarg.h>
#include <hel/math.h>
#include "freyja.h"
#include "BoneABI.h"
#include "KeyFrameABI.h"
#include "MaterialABI.h"
#include "MeshABI.h"
#include "ModelABI.h"
#include "SkeletonABI.h"

#include "LegacyMeshABI.h"


extern "C" {

	////////////////////////////////////////////////////////////////
	// Texcoords
	//
	////////////////////////////////////////////////////////////////

	char freyjaIsTexCoordAllocated(index_t texcoordIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns 1 if texcoord exists
	 ------------------------------------------------------*/

	index_t freyjaTexCoordCreate2f(vec_t u, vec_t v);
	index_t freyjaTexCoordCreate2fv(const vec2_t uv);
	/*------------------------------------------------------
	 * Pre  : u, v are clamped/scaled to 0.0f to 1.0f
	 * Post : A new texcoord is created
	 *        Returns the native index of that texcoord
	 ------------------------------------------------------*/

	vec2_t *freyjaGetTexCoordUV(index_t texcoordIndex);
	/*------------------------------------------------------
	 * Pre  : texcoordIndex exists
	 * Post : Sets passed float array to texcoord u, v
	 ------------------------------------------------------*/

	void freyjaTexCoordCombine(uint32 A, uint32 B);
	/*------------------------------------------------------
	 * Pre  : texcoord indices A and B exist
	 * Post : replaces all references to B with A
	 ------------------------------------------------------*/

	void freyjaTexCoord2f(index_t texcoordIndex, vec_t u, vec_t v);
	/*------------------------------------------------------
	 * Pre  : texcoordIndex exists
	 * Post : Alters the texcoord's values
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////////
	// 0.9.3 Bone utils for 10.0 bones
	//
	////////////////////////////////////////////////////////////////

	void freyjaBoneRemoveMesh(index_t boneIndex, index_t meshIndex);
	void freyjaBoneAddMesh(index_t boneIndex, index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_BONE);
	 * Post : Mesh is added to Bone's child list
	 *
	 *        Either makes mesh tree connection or
	 *        simulates by vertex weights and pivots
	 ------------------------------------------------------*/

	index_t freyjaGetBoneSkeletalBoneIndex(index_t boneIndex);

	void freyjaBoneAddVertex(index_t boneIndex, index_t vertexIndex);

	void freyjaBoneRemoveVertex(index_t boneIndex, index_t vertexIndex);


	////////////////////////////////////////////////////////////////
	// Vertices
	//
	////////////////////////////////////////////////////////////////

	index_t freyjaVertexCombine(index_t vertexIndexA, index_t vertexIndexB);

	void freyjaVertexDelete(index_t vertexIndex);

	index_t freyjaGetCurrentVertex();

	void freyjaCurrentVertex(index_t vertexIndex);

	char freyjaIsVertexAllocated(index_t vertexIndex);

	void freyjaVertexFrame3f(index_t vertexIndex, vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Appends a new mesh animation frame to this vertex
	 ------------------------------------------------------*/

	void freyjaVertexNormalFlip(index_t vertexIndex);

	void freyjaVertexTexcoord2f(index_t vertexIndex, vec_t u, vec_t v);

	void freyjaVertexTexCoord2fv(index_t vertexIndex, vec2_t uv);

	void freyjaVertexNormal3f(index_t vertexIndex, vec_t nx, vec_t ny, vec_t nz);

	void freyjaVertexNormal3fv(index_t vertexIndex, vec3_t nxyz);

	void freyjaVertexPosition3fv(index_t vertexIndex, vec3_t xyz);



	index_t freyjaGetVertexPolygonRefIndex(index_t vertexIndex, uint32 element);

	uint32 freyjaGetVertexPolygonRefCount(index_t vertexIndex);

	void freyjaGetVertexTexcoord2fv(index_t vertexIndex, vec2_t uv);

	void freyjaGetVertexNormalXYZ3fv(index_t vertexIndex, vec3_t nxyz);

	void freyjaGetVertexXYZ3fv(index_t vertexIndex, vec3_t xyz);

	void freyjaGetVertexFrame(index_t vertexIndex, uint32 element,
							  index_t *frameIndex, vec3_t xyz);

	uint32 freyjaGetVertexFrameCount(index_t vertexIndex);

	uint32 freyjaGetVertexFlags(index_t vertexIndex);

	vec3_t *freyjaGetVertexXYZ(index_t vertexIndex);

	vec2_t *freyjaGetVertexUV(index_t vertexIndex);



	///////////////////////////////////////////////////////////////////////
	// Polygon
	///////////////////////////////////////////////////////////////////////

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

	char freyjaIsPolygonAllocated(index_t polygonIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns 1 if exists
	 ------------------------------------------------------*/

	void freyjaPolygonFlagAlpha(index_t polygonIndex, char on);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPolygonSplit(index_t meshIndex, index_t polygonIndex);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 *
	 * Post : Polygon is split into 2 smaller polygons 
	 ------------------------------------------------------*/

	void freyjaPolygonTexCoordPurge(index_t polygonIndex);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 *
	 * Post : All polymapped texcoords are dereferenced
	 ------------------------------------------------------*/

	void freyjaPolygonExtrudeQuad1f(index_t polygonIndex, vec_t dist);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 *        the 'normal' is the vector you wish to follow with extrude
	 *
	 * Post : Adds a quad where every edge is on this face by
	 *        extruding by face normal scaled by dist
	 ------------------------------------------------------*/

	void freyjaPolygonExtrudeQuad(index_t polygonIndex, vec3_t v);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 *        the 'normal' is the vector you wish to follow with extrude
	 *
	 * Post : Adds a quad where every edge is on this face by
	 *        extruding by 'normal' v ( or partial spline or ray )
	 ------------------------------------------------------*/

	void freyjaPolygonAddVertex1i(index_t polygonIndex, index_t vertexIndex);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 * Post : Adds a vertex to a polygon
	 ------------------------------------------------------*/

	void freyjaPolygonAddTexCoord1i(index_t polygonIndex, index_t texcoordIndex);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 * Post : Adds a texcoord to a polygon
	 ------------------------------------------------------*/

	void freyjaPolygonSetMaterial1i(index_t polygonIndex, index_t materialIndex);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 * Post : Sets material for a polygon
	 ------------------------------------------------------*/

	void freyjaPolygonSmoothingGroup(index_t smoothingGroupIndex); // NOT IMPLEMENTED
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Assigns face to smoothing group
	 ------------------------------------------------------*/

	index_t freyjaGetPolygonMaterial(index_t polygonIndex);

	uint32 freyjaGetPolygonFlags(index_t polygonIndex);

	uint32 freyjaGetPolygonEdgeCount(index_t polygonIndex);

	uint32 freyjaGetPolygonVertexCount(index_t polygonIndex);

	uint32 freyjaGetPolygonTexCoordCount(index_t polygonIndex);

	index_t freyjaGetPolygonVertexIndex(index_t polygonIndex, uint32 element);

	index_t freyjaGetPolygonTexCoordIndex(index_t polygonIndex, uint32 element);


	///////////////////////////////////////////////////////////////////////
	// Texture ( 0.9.3 ABI, Can't be used with freyjaIterators )
	///////////////////////////////////////////////////////////////////////

	index_t freyjaTextureCreateFilename(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns new index or INDEX_INVALID
	 ------------------------------------------------------*/

	index_t freyjaTextureCreateBuffer(byte *image, uint32 byteDepth,
                                      uint32 width, uint32 height,
                                      freyja_colormode_t type);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns new index or INDEX_INVALID
	 ------------------------------------------------------*/

	void freyjaTextureDelete(index_t textureIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Deletes texture given any valid index 
	 ------------------------------------------------------*/

	void freyjaGetTextureImage(index_t textureIndex,
                               uint32 *w, uint32 *h, uint32 *bitDepth,  
                               uint32 *type, byte **image);
	/*------------------------------------------------------
	 * Pre  : Do not attempt to alter <image> on return
	 *
	 * Post : Gets pointers to texture[index]'s data members
	 *        Returns 0 on success.
	 ------------------------------------------------------*/

	uint32 freyjaGetTexturePoolCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the number of texture slots
	 ------------------------------------------------------*/

	uint32 freyjaGetTextureCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the number of texture slots used
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	// Freyja plugin subsystem
	//
	///////////////////////////////////////////////////////////////////////

	void freyjaPluginDirectoriesInit();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets default plugin directories.
	 ------------------------------------------------------*/

	void freyjaPluginsInit();
	/*------------------------------------------------------
	 * Pre  : freyjaPluginDirectoriesInit() is called, or you've
	 *        set some custom paths with freyjaPluginAddDirectory(...)
	 * Post : Scans plugin directories and sets up newly found plugins.
	 ------------------------------------------------------*/

	void freyjaPluginAddDirectory(const char *dir);
	/*------------------------------------------------------
	 * Pre  : <dir> is valid string representing valid directory
	 * Post : Directory is added to runtime linked library
	 *        search path list.
	 ------------------------------------------------------*/

	void freyjaPluginDescription(uint32 pluginIndex, const char *info_line);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginImportFlags(uint32 pluginIndex, int32 flags);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginExportFlags(uint32 pluginIndex, int32 flags);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginExtention(uint32 pluginIndex, const char *ext);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	uint32 freyjaGetPluginCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/
	

	///////////////////////////////////////////////////////////////////////
	//  Plugin import/export iteraction
	///////////////////////////////////////////////////////////////////////

	int32 freyjaGetPluginId();

	int freyjaGetPluginArg1f(int32 pluginId, const char *name, float *arg);

	int freyjaGetPluginArg1i(int32 pluginId, const char *name, int32 *arg);

	int freyjaGetPluginArg1s(int32 pluginId, const char *name, char **arg);

	int freyjaGetPluginArgString(int32 pluginId, const char *name, 
								 int32 len, char *arg);


	///////////////////////////////////////////////////////////////////////
	//  Pak VFS 
	///////////////////////////////////////////////////////////////////////

	index_t freyjaPakBegin(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Starts a new VFS from a 'pak file'
	 *        Returns vfs index
	 ------------------------------------------------------*/

	void freyjaPakAddDecoderFunction2s(const char *module, const char *symbol);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Used to decrypt or uncompress files in a pak
	 *        using an external module and one of it's
	 *        C accessable functions using freyjaPak ABI.
	 *
	 *        NOT IMPLEMENTED!
	 ------------------------------------------------------*/

	void freyjaPakAddFullPathFile(index_t pakIndex,
								  const char *vfsFilename,
								  int32 offset, int32 size);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Adds a new entry to VFS mapping a chunk from
	 *        offset to offset+size as a file named vfsFilename
	 *
	 *        Returns 1 if it is successfully 'loaded'
	 ------------------------------------------------------*/

	void freyjaPakEnd(index_t pakIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Finalizes VFS for pakIndex
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////////
	// Util
	//
	////////////////////////////////////////////////////////////////

	void freyjaGenerateQuadPlaneMesh(vec3_t origin, vec_t side);

	void freyjaGenerateQuadCubeMesh(vec3_t origin, vec_t side);

	void freyjaGenerateCircleMesh(vec3_t origin, int32 count); // radius

	void freyjaGenerateConeMesh(vec3_t origin, vec_t height, int32 count); // radius

	void freyjaGenerateCylinderMesh(vec3_t origin, vec_t height, 
								int32 count, int32 segments); // radius

	void freyjaGenerateSphereMesh(vec3_t origin, vec_t radius, 
							  int32 count, int32 segments);

	void freyjaGenerateTubeMesh(vec3_t origin, vec_t height, 
							int32 count, int32 segments); // radius


	///////////////////////////////////////////////////////////////////////
	// Internal ABI calls
	//
	//   If used externally you'll likely get a lot of breakage or
	//   slower and possibly incorrect object states.
	//
	//   Only use this if you're a core developer writing
	//   special test plugins, or internal code.
	///////////////////////////////////////////////////////////////////////


}

#endif
