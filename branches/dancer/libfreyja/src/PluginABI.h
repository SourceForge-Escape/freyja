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
#include <mstl/Vector.h>

#include "freyja.h"
#include "VertexABI.h"
#include "LightABI.h"

extern "C" {




typedef enum {
	INDEXED_8 = 1, 
	RGB_24, 
	RGBA_32

} freyja_colormode_t;


typedef enum {
	FREYJA_WRITE_LOCK = 1,
	FREYJA_WRITE_UNLOCK
	
} freyja_lock_t;


typedef enum {
	FREYJA_MODEL = 1,
	FREYJA_MESH,
	FREYJA_POLYGON,
	FREYJA_BONE,
	FREYJA_SKELETON,
	FREYJA_VERTEX_FRAME,
	FREYJA_VERTEX_GROUP,           /* Was FREYJA_GROUP in EggPlugin */
	FREYJA_VERTEX,
	FREYJA_TEXCOORD,
	FREYJA_MATERIAL,
	FREYJA_TEXTURE,
	FREYJA_ANIMATION,
	FREYJA_MESHTREE_FRAME,         /* Was FREYJA_SKEL_ANIM in EggPlugin */
	FREYJA_MESHTREE_TAG
	
} freyja_object_t;


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


typedef struct {

	char magic[16];
	int32 version;
	int32 flags;
	int32 reserved;
	char comment[64];

} freyja_file_header_t;


typedef struct {

	int32 type;
	int32 size;
	int32 flags;
	int32 version;

} freyja_file_chunk_t;


enum freyja_material_flags {
	fFreyjaMaterial_Blending = 1,
	fFreyjaMaterial_Texture = 2,
	fFreyjaMaterial_DetailTexture = 4,
	fFreyjaMaterial_Normalize = 8
};


#define fPolygon_VertexUV      2
#define fPolygon_PolyMapped    8
#define fPolygon_PolyMappedUV  8
#define fPolygon_ColorMapped   16


typedef enum {

	FREYJA_CHUNK_MODEL    = 0x204C444D,
	FREYJA_CHUNK_MESH     = 0x4853454D,
	FREYJA_CHUNK_TEXCOORDS= 0x524F4F43,
	FREYJA_CHUNK_VERTICES = 0x54524556,
	FREYJA_CHUNK_POLYGONS = 0x594C4F50,
	FREYJA_CHUNK_SKELETON = 0x4C454B53,
	FREYJA_CHUNK_BONE     = 0x454E4F42,
	FREYJA_CHUNK_MATERIAL = 0x5454414D,
	FREYJA_CHUNK_TEXTURE  = 0x54584554,
	FREYJA_CHUNK_METADATA = 0x4154454D

} freyja_file_chunk_type_t;


	///////////////////////////////////////////////////////////////////////
	// Freyja Iterator/FSM functions
	//
	///////////////////////////////////////////////////////////////////////

	void freyjaBegin(freyja_object_t type);
	/*------------------------------------------------------
	 * Pre  : <type> is valid
	 * Post : A new object of type is constructed.
	 ------------------------------------------------------*/

	void freyjaEnd();
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(...) is called to start the state
	 * Post : The object is finialized and construction ends
	 ------------------------------------------------------*/

	index_t freyjaIterator(freyja_object_t type, index_t item);
	/*------------------------------------------------------
	 * Pre  : <type> is valid
	 *        <item> is a FREYJA_LIST_... command or item index
	 *
	 * Post : Sets current item in internal iterator to
	 *        do operations on util a new construction or
	 *        iterator operation of the same type is done.
	 *
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	index_t freyjaCriticalSection(freyja_lock_t request);
	/*------------------------------------------------------
	 * Pre  : FreyjaPlugin singleton exists
	 *
	 * Post : Returns 0 on sucess
	 *        Returns FREYJA_PLUGIN_ERROR on fail
	 *
	 *        If FreyjaPlugin is locked this will block
	 *        in the future. For now it's just to test if
	 *        the singleton is up and running.
	 ------------------------------------------------------*/

	uint32 freyjaGetCount(freyja_object_t type);
	/*------------------------------------------------------
	 * Pre  : <type> is valid
	 * Post : Returns total number of objects of type in 
	 *        parent frame type eg [model]'s meshes
	 *
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	index_t freyjaGetCurrent(freyja_object_t type);
	/*------------------------------------------------------
	 * Pre  : <type> is valid
	 * Post : Returns index of current internal complex type
	 *        ( Last constructed or iterated )
	 *
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	index_t freyjaFSMVertex3fv(vec3_t xyz); // FIXME?
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_GROUP);
	 *        x,y,z are valid 3space coors
	 * Post : A new vertex created in the model
	 *        Returns the native index of that vertex
	 ------------------------------------------------------*/

	char freyjaIsTexCoordAllocated(index_t texcoordIndex);

	char freyjaIsBoneAllocated(index_t boneIndex);

	char freyjaIsPolygonAllocated(index_t polygonIndex);

	void freyjaModelTransform(index_t modelIndex,
								freyja_transform_action_t action, 
								vec_t x, vec_t y, vec_t z);

	void freyjaMeshTransform(index_t meshIndex, index_t frame,
								freyja_transform_action_t action, 
								vec_t x, vec_t y, vec_t z);

	void freyjaBoneTransform(index_t boneIndex, 
								freyja_transform_action_t action, 
								vec_t x, vec_t y, vec_t z);

	void freyjaMeshFrameTransform(index_t meshIndex, index_t frame,
									freyja_transform_action_t action, 
									vec_t x, vec_t y, vec_t z);


	///////////////////////////////////////////////////////////////////////
	// libfreyja plugin ABI 0.9.1 object accessors
	///////////////////////////////////////////////////////////////////////




	// FREYJA_TEXTURE Accessors ///////////////////////////////////////////

	int32 freyjaGetTextureFilename(index_t textureIndex, char **filename);
	/*------------------------------------------------------
	 * Pre  : Do not attempt to alter <filename> on return
	 *
	 * Post : Gets a pointer to the filename of texture[index]
	 *        Returns 0 on success.
	 ------------------------------------------------------*/

	int32 freyjaGetTextureImage(index_t textureIndex,
								  uint32 *w, uint32 *h, 
								  uint32 *depth, uint32 *type, byte **image);
	/*------------------------------------------------------
	 * Pre  : Do not attempt to alter <image> on return
	 *
	 * Post : Gets pointers to texture[index]'s data members
	 *        Returns 0 on success.
	 ------------------------------------------------------*/


	// FREYJA_BONE Accessors //////////////////////////////////////////////

	uint32 freyjaGetSkeletonBoneCount(index_t skeletonIndex);

	index_t freyjaGetSkeletonBoneIndex(index_t skeletonIndex, uint32 element);

	index_t freyjaGetSkeletonRootIndex(index_t skeletonIndex);

	const char *freyjaGetBoneName1s(index_t boneIndex);

	int32 freyjaGetBoneName(index_t boneIndex, uint32 size, char *name);
	/*------------------------------------------------------
	 * Pre  : <name> must be allocated to <size> width
	 *        A <size> of 64 is recommended
	 *
	 * Post : Gets bone[index]'s name as '\0' terminated string
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	int32 freyjaGetBoneParent(index_t boneIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns bone[index]'s parent id
	 *        Returns -2 on error
	 ------------------------------------------------------*/

	index_t freyjaGetBoneRotationWXYZ4fv(index_t boneIndex, vec4_t wxyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets bone[index]'s orientation as a Quaternion
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	index_t freyjaGetBoneRotationXYZ3fv(index_t boneIndex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets bone[index]'s orientation in Euler angles
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	index_t freyjaGetBoneTranslation3fv(index_t boneIndex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets bone[index]'s position
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	index_t freyjaGetBoneSkeletalBoneIndex(index_t boneIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns local skeletal index of this bone
	 ------------------------------------------------------*/

	index_t freyjaGetBoneChild(index_t boneIndex, uint32 element);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Iterator for bone children
	 ------------------------------------------------------*/

	uint32 freyjaGetBoneChildCount(index_t boneIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Count of bone children
	 ------------------------------------------------------*/


	// FREYJA_TEXCOORD Accessors //////////////////////////////////////////

	index_t freyjaGetTexCoordPolygonRefIndex(index_t texcoordIndex, uint32 element);

	uint32 freyjaGetTexCoordPolygonRefCount(index_t texcoordIndex);

	void freyjaGetTexCoord2fv(index_t texcoordIndex, vec2_t uv);
	/*------------------------------------------------------
	 * Pre  : texcoord[index] exists
	 * Post : Sets passed float array to texcoord u, v
	 ------------------------------------------------------*/


	// FREYJA_MODEL Accessors
	uint32 freyjaGetModelFlags(index_t modelIndex);
	index_t freyjaGetModelMeshIndex(index_t modelIndex, uint32 element);
	uint32 freyjaGetModelMeshCount(index_t modelIndex);

	// FREYJA_MESH Accessors
	char freyjaIsMeshAllocated(index_t meshIndex);

	int32 freyjaGetMeshFrameBoundingBox(index_t meshIndex, 
										index_t frame, vec3_t min, vec3_t max);

	uint32 freyjaGetMeshFlags(index_t meshIndex);

	int32 freyjaGetMeshPosition(index_t meshIndex, vec3_t xyz);

	char *freyjaGetMeshNameString(index_t meshIndex); // don't alter string

	int32 freyjaGetMeshName1s(index_t meshIndex, uint32 lenght, char *name);

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
	

	// 'free vertices' outside of polygons as well as other 'grouped' vertices
	void freyjaMeshVertexGroupAppendGobalVertex(index_t meshIndex, uint32 element, 
												index_t vertexIndex);
	uint32 freyjaGetMeshVertexGroupVertexCount(index_t meshIndex, uint32 element);
	index_t freyjaGetMeshVertexGroupVertexIndex(index_t meshIndex, uint32 element,
											  index_t vertexElement);


	index_t freyjaGetMeshPolygonIndex(index_t meshIndex, uint32 element);
	uint32 freyjaGetMeshPolygonCount(index_t meshIndex);
	index_t freyjaGetMeshVertexGroupIndex(index_t meshIndex, uint32 element);
	uint32 freyjaGetMeshVertexGroupCount(index_t meshIndex);
	index_t freyjaGetMeshVertexFrameIndex(index_t meshIndex, uint32 element); // Not Implemented
	uint32 freyjaGetMeshVertexFrameCount(index_t meshIndex); // Not Implemented

	// FREYJA_POLYGON Accessors
	index_t freyjaGetPolygonMaterial(int32 polygonIndex);
	uint32 freyjaGetPolygonFlags(int32 polygonIndex);
	uint32 freyjaGetPolygonEdgeCount(int32 polygonIndex);
	uint32 freyjaGetPolygonVertexCount(int32 polygonIndex);
	uint32 freyjaGetPolygonTexCoordCount(int32 polygonIndex);
	index_t freyjaGetPolygonVertexIndex(int32 polygonIndex, int32 element);
	index_t freyjaGetPolygonTexCoordIndex(int32 polygonIndex, int32 element);


///////////////////////////////////////////////////////////////////////
// Mutator functions to operate on Scene
///////////////////////////////////////////////////////////////////////

	uint32 freyjaGetModelDebugLevel(index_t modelIndex);

	void freyjaModelDebugLevel(index_t modelIndex, uint32 debugLevel);

	/* Polymapped texcoords */
	void freyjaPolyMapTexCoordCombine(index_t a, index_t b);
	void freyjaPolyMapTexCoord2f(index_t index, vec_t u, vec_t v);
	void freyjaPolyMapTexCoord2fv(index_t index, vec2_t uv);
	void freyjaPolyMapTexCoord3f(index_t index, vec_t u, vec_t v, vec_t w);
	void freyjaPolyMapTexCoord3fv(index_t index, vec3_t uv);

	/* Polymapped normals */
	void freyjaPolyMapNormal3f(vec_t x, vec_t y, vec_t z);
	void freyjaPolyMapNormal3fv(vec3_t xyz);

	index_t freyjaTexCoord2f(vec_t u, vec_t v);
	index_t freyjaTexCoord2fv(vec2_t uv);
	/*------------------------------------------------------
	 * Pre  : s, t are 0.0 to 1.0 texels
	 * Post : A new texel is created in the model
	 *        Returns the native index of that texel
	 ------------------------------------------------------*/



	////////////////////////////////////////////////////////////////
	// Bone
	//
	////////////////////////////////////////////////////////////////

	index_t freyjaBoneCreate(index_t skeletonIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Creates a new rest bone for skeleton
	 *        Returns valid Index or -1 on Error
	 ------------------------------------------------------*/

	void freyjaBoneAddVertex(index_t boneIndex, index_t vertexIndex);

	void freyjaBoneRemoveVertex(index_t boneIndex, index_t vertexIndex);

	void freyjaBoneFlags1i(index_t boneIndex, index_t flags);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_BONE);
	 * Post : Set bone flags
	 ------------------------------------------------------*/

	void freyjaBoneParent1i(index_t boneIndex, index_t parentIndex);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_BONE);
	 * Post : Set bone parent
	 *
	 *        This doesn't affect skeleton, a follow up
	 *        call to freyjaBoneAddChild1i is needed after
	 *        all bones in skeleton are allocated
	 ------------------------------------------------------*/

	void freyjaBoneName1s(index_t boneIndex, const char *name);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_BONE);
	 * Post : Set human readable bone name
	 ------------------------------------------------------*/

	void freyjaBoneRemoveMesh1i(index_t boneIndex, index_t meshIndex);
	void freyjaBoneAddMesh1i(index_t boneIndex, index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_BONE);
	 * Post : Mesh is added to Bone's child list
	 *
	 *        Either makes mesh tree connection or
	 *        simulates by vertex weights and pivots
	 ------------------------------------------------------*/

	void freyjaBoneRemoveChild1i(index_t boneIndex, index_t childIndex);
	void freyjaBoneAddChild1i(index_t boneIndex, index_t childIndex);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_BONE);
	 * Post : Child is added to Bone's child list
	 ------------------------------------------------------*/

	void freyjaBoneTranslate3f(index_t boneIndex, vec_t x, vec_t y, vec_t z);
	void freyjaBoneTranslate3fv(index_t boneIndex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_BONE);
	 * Post : Set bone relative position
	 ------------------------------------------------------*/

	void freyjaBoneRotateEulerXYZ3f(index_t boneIndex, vec_t x, vec_t y, vec_t z);
	void freyjaBoneRotateEulerXYZ3fv(index_t boneIndex, vec3_t xyz);
	void freyjaBoneRotateQuatWXYZ4f(index_t boneIndex,vec_t w,vec_t x,vec_t y,vec_t z);
	void freyjaBoneRotateQuatWXYZ4fv(index_t boneIndex, vec4_t wxyz);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_BONE);
	 * Post : Set bone orientation
	 ------------------------------------------------------*/

	void freyjaGroupCenter3f(vec_t x, vec_t y, vec_t z);

	void freyjaGenerateQuadPlaneMesh(vec3_t origin, vec_t side);

	void freyjaGenerateQuadCubeMesh(vec3_t origin, vec_t side);

	void freyjaGenerateCircleMesh(vec3_t origin, uint32 count);

	void freyjaGenerateConeMesh(vec3_t origin, vec_t height, uint32 count);

	void freyjaGenerateCylinderMesh(vec3_t origin, vec_t height, 
									uint32 count, uint32 segments); // radius

	void freyjaGenerateSphereMesh(vec3_t origin, vec_t radius, 
								  uint32 count, uint32 segments);

	void freyjaGenerateTubeMesh(vec3_t origin, vec_t height, 
								uint32 count, uint32 segments); // radius



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

	void freyjaMeshFrameCenter(index_t meshIndex, index_t frame, vec3_t xyz);

	void freyjaGetMeshFrameCenter(index_t meshIndex, index_t frame, vec3_t xyz);

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

	void freyjaGenerateVertexNormals();

	void freyjaGenerateUVFromXYZ(vec3_t xyz, vec_t *u, vec_t *v);

	void freyjaVertexFrame3f(index_t index, vec_t x, vec_t y, vec_t z);


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
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.03.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void freyjaPolygonExtrudeQuad(index_t polygonIndex, vec3_t normal);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 *        the 'normal' is the vector you wish to follow with extrude
	 *
	 * Post : Adds a quad where every edge is on this face by
	 *        extruding by 'normal'
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.03.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void freyjaPolygonAddVertex1i(index_t polygonIndex, index_t vertexIndex);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 * Post : Adds a vertex to a polygon
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void freyjaPolygonAddTexCoord1i(index_t polygonIndex, index_t texcoordIndex);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 * Post : Adds a texcoord to a polygon
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void freyjaPolygonSetMaterial1i(index_t polygonIndex, index_t materialIndex);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 * Post : Sets material for a polygon
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	// Animation ( 0.9.3 ABI, Can't be used with freyjaIterators )
	///////////////////////////////////////////////////////////////////////

	index_t freyjaAnimationCreate();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Creates a new Freyja Animation object
	 *        Returns the new Animation's index or -1 on error
	 *
	 * NOTES: The new skeletal animation system allows for
	 *        sharing (subset) animations across multiple
	 *        skeletons using name matching to assure the
	 *        skeletons match. 
	 ------------------------------------------------------*/

	index_t freyjaAnimationBoneCreate(index_t animationIndex,
								   const char *name, index_t boneIndex);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 * Post : Creates a new Freyja Animation Bone object
	 *        Returns the new Bone's index or -1 on error
	 ------------------------------------------------------*/

	index_t freyjaAnimationBoneKeyFrameCreate(index_t animationIndex,
										   index_t boneIndex,
										   vec_t time, vec3_t xyz, vec4_t wxyz);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 *        Animation Bone <boneIndex> exists
	 *
	 * Post : Returns the keyframe's local Animation's Bone's
	 *        Keyframe element index or -1 on error
	 ------------------------------------------------------*/


	/* Animation Accessors */

	uint32 freyjaGetAnimationCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the number of Animations being managed
	 ------------------------------------------------------*/

	int32 freyjaGetAnimationFrameCount(int32 animationIndex);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 * Post : Returns the number of frames or -1 on error
	 ------------------------------------------------------*/

	// FIXME: This is still using fused Animation/Skeleton
	int32 freyjaGetAnimationBoneCount(int32 skeletonIndex);
	/*------------------------------------------------------
	 * Pre  : Skeleton <skeletonIndex> exists
	 * Post : Returns the number of bones or -1 on error
	 ------------------------------------------------------*/

	int32 freyjaGetAnimationBoneKeyFrameCount(int32 animationIndex, 
											 int32 boneIndex);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 *        Animation Bone <boneIndex> exists
	 *
	 * Post : Returns the number of keyframes or -1 on error
	 ------------------------------------------------------*/

	// Finish me


	/* Animation Mutators */

	void freyjaAnimationName(int32 animationIndex, const char *name);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 * Post : Sets human readable animation name
	 ------------------------------------------------------*/

	void freyjaAnimationBoneName(int32 animationIndex, int32 boneIndex,
								 const char *name);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 *        Animation Bone <boneIndex> exists
	 * Post : Sets human readable animation name
	 ------------------------------------------------------*/

	void freyjaAnimationFrameRate(int32 animationIndex, vec_t frameRate);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaAnimationTime(int32 animationIndex, vec_t time);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaAnimationSubsetRoot(int32 animationIndex, int32 startBone);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaAnimationKeyFrameTime(int32 animationIndex, int32 boneIndex,
									 int32 keyFrameIndex, vec_t time);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> and <keyFrameindex> exist
	 * Post : Sets time for <keyFrameIndex> keyframe in animation
	 ------------------------------------------------------*/

	void freyjaAnimationKeyFramePosition(int32 animationIndex, int32 boneIndex, 
										 int32 keyFrameIndex, vec3_t position);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> and <keyFrameindex> exist
	 * Post : Sets <keyFrameIndex> keyframe's position in animation
	 ------------------------------------------------------*/

	void freyjaAnimationKeyFrameOrientationXYZ(int32 animationIndex,
											   int32 boneIndex, 
											   int32 keyFrameIndex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> and <keyFrameindex> exist
	 * Post : Sets <keyFrameIndex> keyframe's orienation in animation
	 *        by Euler angles
	 ------------------------------------------------------*/

	void freyjaAnimationKeyFrameOrientationWXYZ(int32 animationIndex,
												int32 boneIndex,
												int32 keyFrameIndex,vec4_t wxyz);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> and <keyFrameindex> exist
	 * Post : Sets <keyFrameIndex> keyframe's orienation in animation
	 *        by Quaternion ( note the w, xyz order )
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	// Texture ( 0.9.3 ABI, Can't be used with freyjaIterators )
	///////////////////////////////////////////////////////////////////////

	index_t freyjaShaderCreate();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaShaderProgram(index_t shaderIndex, const char *program);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	const char *freyjaGetShaderProgram(index_t shaderIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	// Texture ( 0.9.3 ABI, Can't be used with freyjaIterators )
	///////////////////////////////////////////////////////////////////////

	index_t freyjaTextureCreate();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	index_t freyjaTextureCreateFilename(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	index_t freyjaTextureCreateBuffer(byte *image, uint32 depth,
								 	uint32 width, uint32 height,
									freyja_colormode_t type);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/





	///////////////////////////////////////////////////////////////////////
	// Faster access API for modeler use
	///////////////////////////////////////////////////////////////////////

	vec3_t *freyjaGetVertexXYZ(index_t vertexIndex);
	vec2_t *freyjaGetVertexUV(index_t vertexIndex);
	vec2_t *freyjaGetTexCoordUV(index_t texcoordIndex);

	char freyjaGetModelAppendMeshMode(index_t modelIndex);
	/*------------------------------------------------------
	 * Pre  : Model <modelIndex> exists and has an active copy buffer
	 * Post : Model's mesh buffer mode is returned or -1 on error
	 *         enabled when returning  1
	 *         disabled when returning  0 
	 ------------------------------------------------------*/

	void freyjaModelClear(index_t modelIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : All data in model is reset/cleared
	 ------------------------------------------------------*/

	void freyjaPolygonSplitTexCoords(index_t meshIndex, index_t polygonIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : All TexCoords are duplicated, then references to old
	 *        TexCoords are removed -- this is useful for making a single
	 *        polymapped texture polygon when all others will remain
	 *        in a UV Mesh
	 ------------------------------------------------------*/

	void freyjaModelTransformTexCoord(index_t modelIndex,
										index_t texCoordIndex,
										freyja_transform_action_t action,
										vec_t x, vec_t y);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaModelClampTexCoords(index_t modelIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Clamps UVs and TexCoords to 0.0 to 1.0
	 ------------------------------------------------------*/

	void freyjaModelAppendMeshMode(index_t modelIndex, char on);
	/*------------------------------------------------------
	 * Pre  : Model <modelIndex> exists and has an active copy buffer
	 * Post : Model's mesh buffer mode is 
	 *         enabled when <on> = 1
	 *         disabled when <on> = 0 
	 ------------------------------------------------------*/

	char freyjaModelCopyMesh(index_t modelIndex, index_t mesh, index_t frame);
	/*------------------------------------------------------
	 * Pre  : Model <modelIndex> exists and has an active copy buffer
	 * Post : Model's copy buffer is modified to include new mesh duplicate
	 *        using the *local <mesh> index and vertex morph <frame>
	 ------------------------------------------------------*/

	char freyjaModelPasteMesh(index_t modelIndex);
	/*------------------------------------------------------
	 * Pre  : Model <modelIndex> exists and has an active copy buffer
	 * Post : Model's copy buffer is added to freyja as world object
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

	int32 freyjaExportModel(const char *filename, const char *type);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Exports model and returns 0, or
	 *        fails to import returns error -N
	 ------------------------------------------------------*/

	int32 freyjaImportModel(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Imports model and returns 0, or
	 *        fails to import returns error -N
	 ------------------------------------------------------*/

	void freyjaPluginAddDirectory(const char *dir);
	/*------------------------------------------------------
	 * Pre  : <dir> is valid string representing valid directory
	 * Post : Directory is added to runtime linked library
	 *        search path list.
	 ------------------------------------------------------*/

	void freyjaPluginDescription(index_t pluginIndex, const char *info_line);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginImportFlags(index_t pluginIndex, uint32 flags);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginExportFlags(index_t pluginIndex, uint32 flags);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginExtention(index_t pluginIndex, const char *ext);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	int32 freyjaGetPluginCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	// Old iterator style plugin interface

	typedef enum {
		
		FREYJA_PLUGIN_NONE            = 0,
		FREYJA_PLUGIN_MESH            = 1,
		FREYJA_PLUGIN_SKELETON        = 2,
		FREYJA_PLUGIN_VERTEX_MORPHING = 4,
		FREYJA_PLUGIN_VERTEX_BLENDING = 8

	} freyja_plugin_options_t;


	void freyjaPluginBegin();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginDescription1s(const char *info_line);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginAddExtention1s(const char *ext);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginImport1i(uint32 flags);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginExport1i(uint32 flags);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginArg1i(const char *name, int32 defaults);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginArg1f(const char *name, float defaults);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginArg1s(const char *name, const char *defaults);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginEnd();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginRegisterMenuCallback(int (*)(int menuId, const char *label, int (*)(int event)));
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/
	

	///////////////////////////////////////////////////////////////////////
	//  Plugin import/export iteraction
	///////////////////////////////////////////////////////////////////////

	int32 freyjaGetPluginId();

	int freyjaGetPluginArg1f(index_t pluginId, const char *name, float *arg);

	int freyjaGetPluginArg1i(index_t pluginId, const char *name, int32 *arg);

	int freyjaGetPluginArg1s(index_t pluginId, const char *name, char **arg);

	int freyjaGetPluginArgString(index_t pluginId, const char *name, 
								 uint32 len, char *arg);


	///////////////////////////////////////////////////////////////////////
	//  Pak VFS 
	///////////////////////////////////////////////////////////////////////

	index_t freyjaPakBegin(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Starts a new VFS from a 'pak file'
	 *
	 *        Returns vfs index
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.01.02:
	 * Mongoose - Created
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

	int32 freyjaPakAddFullPathFile(index_t pakIndex,
								  const char *vfsFilename,
								  uint32 offset, uint32 size);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Adds a new entry to VFS mapping a chunk from
	 *        offset to offset+size as a file named vfsFilename
	 *
	 *        Returns vfs file entry index
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.01.02:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void freyjaPakEnd(index_t pakIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Finalizes VFS for pakIndex
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.01.02:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	//  Pak VFS 
	///////////////////////////////////////////////////////////////////////

	int32 freyjaCheckModel(const char *filename);

	int32 freyjaLoadModel(const char *filename);

	int32 freyjaSaveModel(const char *filename);


	///////////////////////////////////////////////////////////////////////
	// Internal ABI calls
	//
	//   If used externally you'll likely get a lot of breakage or
	//   slower and possibly incorrect object states.
	///////////////////////////////////////////////////////////////////////

	void freyja__MeshUpdateMappings(int32 meshIndex);
	/*------------------------------------------------------
	 * Pre  : Only use this if you're a core developer writing
	 *        special test plugins, or internal code.
	 *
	 * Post : Updates Egg backend egg_mesh_t to simulate
	 *        FreyjaMesh local vertex mappings
	 ------------------------------------------------------*/



	///////////////////////////////////////////////////////////////////////
	// Managed ABI 
	//
	//   For use by external systems to bring up/down Backend outside C++
	///////////////////////////////////////////////////////////////////////

	void freyjaSpawn();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Starts freyja backend, also does needed allocations
	 ------------------------------------------------------*/

	void freyjaFree();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Stops freyja backend, also frees memory used
	 ------------------------------------------------------*/
}


// Use freyjaVertexGroupCreate/Delete to make tmp lists that are C exportable?
#ifdef REPLACE_ME_FIXME_NOW
/* Mongoose 2004.12.19, 
 * C++ fun */
void freyjaVertexListTransform(Vector<index_t> &list,
								freyja_transform_action_t action, 
								vec_t x, vec_t y, vec_t z);

void freyjaModelMirrorTexCoord(index_t modelIndex, index_t texCoordIndex,
								Vector<index_t> uvMap, bool x, bool y);

char freyjaModelCopyVertexList(index_t modelIndex, 
							   Vector<index_t> &list,
							   index_t mesh, index_t frame);
/*------------------------------------------------------
 * Pre  : Model <modelIndex> exists and has an active copy buffer
 * Post : Model's copy buffer is modified to include new mesh duplicate
 *        using the *local <mesh> index and vertex morph <frame>
 ------------------------------------------------------*/

index_t freyjaFindPolygonByVertices(Vector<index_t> vertices);


Vector<index_t> *freyjaFindVerticesByBox(vec3_t bbox[2]);

void freyjaGetVertexPolygonRef1i(index_t vertexIndex, Vector<index_t> &polygons);

void freyjaGetVertexPolygonRef(Vector<index_t> &polygons);
/*------------------------------------------------------
 * Pre  : 
 * Post : Returns a list of indices of polygons that
 *        use current vertex in iterator
 *
 *        List can be empty
 ------------------------------------------------------*/

#endif /* REPLACE_ME_FIXME_NOW */

#endif
