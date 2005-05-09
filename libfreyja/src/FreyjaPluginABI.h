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

extern "C" {

#define FREYJA_PLUGIN_VERSION   "Freyja 0.9.1"
#define FREYJA_LIST_RESET       -2
#define FREYJA_LIST_NEXT        -1
#define FREYJA_LIST_CURRENT     -3
#define FREYJA_LIST_SIZE        -4
#define FREYJA_PLUGIN_ERROR     -1


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
	// Freyja Iterator functions
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

	int32 freyjaIterator(freyja_object_t type, int32 item);
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

	int32 freyjaCriticalSection(freyja_lock_t request);
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

	int32 freyjaGetCount(freyja_object_t type);
	/*------------------------------------------------------
	 * Pre  : <type> is valid
	 * Post : Returns total number of objects of type in 
	 *        parent frame type eg [model]'s meshes
	 *
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	int32 freyjaGetCurrent(freyja_object_t type);
	/*------------------------------------------------------
	 * Pre  : <type> is valid
	 * Post : Returns index of current internal complex type
	 *        ( Last constructed or iterated )
	 *
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	// libfreyja plugin ABI 0.9.1 object accessors
	///////////////////////////////////////////////////////////////////////

	int32 freyjaLightCreate();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns index of light spawned
	 ------------------------------------------------------*/

	void freyjaLightDelete(uint32 lightIndex);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Light is removed from light pool
	 ------------------------------------------------------*/

	// Light mutators /////////////////

	void freyjaLightPosition4v(uint32 lightIndex, vec4_t position);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Sets <position> 
	 ------------------------------------------------------*/

	void freyjaLightAmbient(uint32 lightIndex, vec4_t ambient);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Sets <ambient> color
	 ------------------------------------------------------*/

	void freyjaLightDiffuse(uint32 lightIndex, vec4_t diffuse);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Sets <diffuse> color
	 ------------------------------------------------------*/

	void freyjaLightSpecular(uint32 lightIndex, vec4_t specular);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Sets <specular> color
	 ------------------------------------------------------*/

	// Light accessors /////////////////

	void freyjaGetLightPosition4v(uint32 lightIndex, vec4_t position);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Returns <position> 
	 ------------------------------------------------------*/

	void freyjaGetLightAmbient(uint32 lightIndex, vec4_t ambient);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Returns <ambient> color
	 ------------------------------------------------------*/

	void freyjaGetLightDiffuse(uint32 lightIndex, vec4_t diffuse);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Returns <diffuse> color
	 ------------------------------------------------------*/

	void freyjaGetLightSpecular(uint32 lightIndex, vec4_t specular);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Returns <specular> color
	 ------------------------------------------------------*/
	


	// FREYJA_TEXTURE Accessors ///////////////////////////////////////////

	int freyjaGetTextureFilename(unsigned int index, char **filename);
	/*------------------------------------------------------
	 * Pre  : Do not attempt to alter <filename> on return
	 *
	 * Post : Gets a pointer to the filename of texture[index]
	 *        Returns 0 on success.
	 ------------------------------------------------------*/

	int freyjaGetTextureImage(unsigned int index,
							  unsigned int *w, unsigned int *h, 
							  unsigned int *depth,  unsigned int *type,
							  unsigned char **image);
	/*------------------------------------------------------
	 * Pre  : Do not attempt to alter <image> on return
	 *
	 * Post : Gets pointers to texture[index]'s data members
	 *        Returns 0 on success.
	 ------------------------------------------------------*/


	// FREYJA_BONE Accessors //////////////////////////////////////////////

	int32 freyjaGetSkeletonBoneCount(int32 skeletonIndex);
	int32 freyjaGetSkeletonBoneIndex(int32 skeletonIndex, int32 element);

	int32 freyjaGetBoneName(int32 boneIndex, unsigned int size, char *name);
	/*------------------------------------------------------
	 * Pre  : <name> must be allocated to <size> width
	 *        A <size> of 64 is recommended
	 *
	 * Post : Gets bone[index]'s name as '\0' terminated string
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	int32 freyjaGetBoneParent(int32 boneIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns bone[index]'s parent id
	 *        Returns -2 on error
	 ------------------------------------------------------*/

	int32 freyjaGetBoneRotationWXYZ4fv(int32 boneIndex, vec4_t wxyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets bone[index]'s orientation as a Quaternion
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	int32 freyjaGetBoneRotationXYZ3fv(int32 boneIndex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets bone[index]'s orientation in Euler angles
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	int32 freyjaGetBoneTranslation3fv(int32 index, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets bone[index]'s position
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/


	// FREYJA_TEXCOORD Accessors //////////////////////////////////////////

	void freyjaGetTexCoord2fv(int32 texcoordIndex, vec2_t uv);
	/*------------------------------------------------------
	 * Pre  : texcoord[index] exists
	 * Post : Sets passed float array to texcoord u, v
	 ------------------------------------------------------*/


	// FREYJA_VERTEX Accessors ////////////////////////////////////////////

	void freyjaGetVertexTexCoord2fv(vec2_t uv);
	void freyjaGetVertexNormal3fv(vec3_t xyz);
	void freyjaGetVertex3fv(vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : freyjaIterator has initialized a vertex
	 *
	 * Post : Sets vertex[FREYJA_LIST_CURRENT]'s
	 *        position, normal, or texcoord
	 *
	 *        This is deprecated
	 ------------------------------------------------------*/

	int32 freyjaGetVertexTexCoordUV2fv(int32 vertexIndex, vec2_t uv);
	int32 freyjaGetVertexNormalXYZ3fv(int32 vertexIndex, vec3_t nxyz);
	int32 freyjaGetVertexXYZ3fv(int32 vertexIndex, vec3_t xyz);
	int32 freyjaGetVertexFrame(int32 vertexIndex, int32 element,
							  int32 *frameIndex, vec3_t xyz);
	int32 freyjaGetVertexFrameCount(int32 vertexIndex);
	int32 freyjaGetVertexWeight(int32 vertexIndex, int32 element,
							   int32 *bone, vec_t *weight);
	int32 freyjaGetVertexWeightCount(int32 vertexIndex);
	int32 freyjaGetVertexFlags(int32 vertexIndex);

	// FREYJA_MODEL Accessors
	int32 freyjaGetModelFlags(int32 modelIndex);
	int32 freyjaGetModelMeshIndex(int32 modelIndex, int32 element);
	int32 freyjaGetModelMeshCount(int32 modelIndex);

	// FREYJA_MESH Accessors
	//int freyjaGetMeshBoundingBox(int32 meshIndex, vec3_t min, vec3_t max);
	int32 freyjaGetMeshFlags(int32 meshIndex);
	int32 freyjaGetMeshPosition(int32 meshIndex, vec3_t xyz);
	char *freyjaGetMeshNameString(int32 meshIndex); // don't alter string
	int32 freyjaGetMeshName1s(int32 meshIndex, int32 lenght, char *name);

	int32 freyjaGetMeshVertexIndex(int32 meshIndex, int32 element);
	/*------------------------------------------------------
	 * Pre  : freyjaGetMeshVertexCount must be called before
	 *        this is valid while using the Egg backend
	 *
	 * Post : Returns the index of the local vertex <element>
	 *        for mesh <meshIndex> or -1 on error
	 ------------------------------------------------------*/

	int32 freyjaGetMeshVertexCount(int32 meshIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the number of local vertices
	 *        for mesh <meshIndex> or 0 on error
	 *
	 ------------------------------------------------------*/

	int32 freyjaGetMeshPolygonVertexIndex(int32 meshIndex, int32 faceVertexIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the index of local vertices
	 *        for mesh <meshIndex> with local
	 *        mapping from polygon to mesh ids or -1 on error
	 *
	 ------------------------------------------------------*/
	

	int32 freyjaGetMeshPolygonIndex(int32 meshIndex, int32 element);
	int32 freyjaGetMeshPolygonCount(int32 meshIndex);
	int32 freyjaGetMeshVertexGroupIndex(int32 meshIndex, int32 element);
	int32 freyjaGetMeshVertexGroupCount(int32 meshIndex);
	int32 freyjaGetMeshVertexFrameIndex(int32 meshIndex, int32 element); // Not Implemented
int32 freyjaGetMeshVertexFrameCount(int32 meshIndex); // Not Implemented

// FREYJA_POLYGON Accessors
int32 freyjaGetPolygonMaterial(int32 polygonIndex);
int32 freyjaGetPolygonFlags(int32 polygonIndex);
int32 freyjaGetPolygonEdgeCount(int32 polygonIndex);
int32 freyjaGetPolygonVertexCount(int32 polygonIndex);
int32 freyjaGetPolygonTexCoordCount(int32 polygonIndex);
int32 freyjaGetPolygonVertexIndex(int32 polygonIndex, int32 element);
int32 freyjaGetPolygonTexCoordIndex(int32 polygonIndex, int32 element);


int32 freyjaGetPolygon1u(freyja_object_t type, int32 item, int32 *value);
/*------------------------------------------------------
 * Pre  : Type is either vertex or texel
 *        Item is index into polygon's type list 
 *        Value is valid pointer
 *
 * Post : Sets Value to the internal id value used by item
 *
 *-- History ------------------------------------------
 *
 * 2001.11.03: 
 * Mongoose - Created
 ------------------------------------------------------*/

int32 freyjaGetPolygon3f(freyja_object_t type, int32 item, vec_t *value);
/*------------------------------------------------------
 * Pre  : Type is either vertex or texel
 *        Item is index into polygon's type list 
 *        Value is valid pointer
 *
 * Post : Sets Value to the value used by item:
 *
 *           Texel  - float st[2]
 *           Vertex - float xyz[3]
 *
 *-- History ------------------------------------------
 *
 * 2001.11.03: 
 * Mongoose - Created
 ------------------------------------------------------*/

void freyjaPrintError(const char *format, ...);
/*------------------------------------------------------
 * Pre  : Format string and args are valid
 * Post : Report messages to stdout
 *
 *-- History ------------------------------------------
 *
 * 2004.05.18:
 * Mongoose - Created, split from experimental branch
 ------------------------------------------------------*/

void freyjaPrintMessage(const char *format, ...);
/*------------------------------------------------------
 * Pre  : Format string and args are valid
 * Post : Report messages to stdout
 *
 *-- History ------------------------------------------
 *
 * 2004.05.18:
 * Mongoose - Created, split from experimental branch
 ------------------------------------------------------*/


///////////////////////////////////////////////////////////////////////
// Mutator functions to operate on Scene
///////////////////////////////////////////////////////////////////////

void freyjaSetNormal3f(unsigned int index, vec_t x, vec_t y, vec_t z);
void freyjaSetNormal3fv(unsigned int index, vec3_t xyz);

void freyjaSetTexCoord2f(unsigned int index, vec_t u, vec_t v);
void freyjaSetTexCoord2fv(unsigned int index, vec2_t uv);

void freyjaSetVertex3f(unsigned int index, vec_t x, vec_t y, vec_t z);
	//void freyjaSetVertex3fv(unsigned int index, vec3_t xyz);

int32 freyjaNormal3f(vec_t x, vec_t y, vec_t z);
int32 freyjaNormal3fv(vec3_t xyz);
/*------------------------------------------------------
 * Pre  : (x,y,z) is a valid normal vector
 * Post : A new normal is created in the mesh
 *        Returns the native index of that normal
 *
 *-- History ------------------------------------------
 *
 * 1999.07.31:
 * Mongoose - Created
 ------------------------------------------------------*/

int32 freyjaTexCoord2f(vec_t u, vec_t v);
int32 freyjaTexCoord2fv(vec2_t uv);
/*------------------------------------------------------
 * Pre  : s, t are 0.0 to 1.0 texels
 * Post : A new texel is created in the model
 *        Returns the native index of that texel
 *
 *-- History ------------------------------------------
 *
 * 1999.07.31:
 * Mongoose - Created
 ------------------------------------------------------*/

void freyjaVertexNormalFlip(int32 index);
void freyjaVertexTexCoord2f(int32 index, vec_t u, vec_t v);
void freyjaVertexTexCoord2fv(int32 index, vec2_t uv);
void freyjaVertexNormal3f(int32 index, vec_t nx, vec_t ny, vec_t nz);
void freyjaVertexNormal3fv(int32 index, vec3_t nxyz);
int32 freyjaVertex3f(vec_t x, vec_t y, vec_t z);
int32 freyjaVertexXYZ3fv(int32 vertexIndex, vec3_t xyz);
int32 freyjaVertex3fv(vec3_t xyz);
/*------------------------------------------------------
 * Pre  : freyjaBegin(FREYJA_GROUP);
 *        x,y,z are valid 3space coors
 * Post : A new vertex created in the model
 *        Returns the native index of that vertex
 *
 *-- History ------------------------------------------
 *
 * 1999.07.31:
 * Mongoose - Created
 ------------------------------------------------------*/

void freyjaVertexWeight(int32 index, vec_t weight, int32 bone);
/*------------------------------------------------------
 * Pre  : <weight> of influence of <bone> on vertex[<index>]
 *
 * Post : Vertex <index> in the model gets weight added
 *        to influence list, if there is a weight for the
 *        corresponding bone it is replaced
 *
 *        <weight> <= 0.0 removes weight
 *
 *        All weights for the vertex combined must be 1.0
 *-- History ------------------------------------------
 *
 * 2004.03.25:
 * Mongoose - Created
 ------------------------------------------------------*/

void freyjaMeshTreeAddFrame(vec_t x, vec_t y, vec_t z);
/*------------------------------------------------------
 * Pre  : freyjaBegin(FREYJA_MESHTREE_ANIM);
 * Post : Adds a new meshtree frame to meshtree
 *
 *-- History ------------------------------------------
 *
 * 2001.11.18: 
 * Mongoose - Created
 ------------------------------------------------------*/

void freyjaMeshTreeFrameAddBone(int32 tag);
/*------------------------------------------------------
 * Pre  : eggBegin(FREYJA_MESHTREE_ANIM_FRAME);
 * Post : Appends tag to mesh tree frame
 *
 *-- History ------------------------------------------
 *
 * 2001.11.18: 
 * Mongoose - Created
 ------------------------------------------------------*/

int32 freyjaTextureFilename1s(const char *filename);

int32 freyjaTextureStoreBuffer(unsigned char *image, unsigned int depth,
							  unsigned int width, unsigned int height,
							  freyja_colormode_t type);

void freyjaBoneFlags1i(int32 boneIndex, int32 flags);
/*------------------------------------------------------
 * Pre  : freyjaBegin(FREYJA_BONE);
 * Post : Set bone flags
 *
 *-- History ------------------------------------------
 *
 * 2001.10.27: 
 * Mongoose - Created
 ------------------------------------------------------*/

void freyjaBoneParent1i(int32 boneIndex, int32 parentIndex);
/*------------------------------------------------------
 * Pre  : freyjaBegin(FREYJA_BONE);
 * Post : Set bone parent
 *
 *        This doesn't affect skeleton, a follow up
 *        call to freyjaBoneAddChild1i is needed after
 *        all bones in skeleton are allocated
 *
 *-- History ------------------------------------------
 *
 * 2001.10.27: 
 * Mongoose - Created
 ------------------------------------------------------*/

void freyjaBoneName1s(int32 boneIndex, char *name);
/*------------------------------------------------------
 * Pre  : freyjaBegin(FREYJA_BONE);
 * Post : Set human readable bone name
 *
 *-- History ------------------------------------------
 *
 * 2001.10.27: 
 * Mongoose - Created
 ------------------------------------------------------*/

void freyjaBoneAddMesh1i(int32 boneIndex, int32 meshIndex);
/*------------------------------------------------------
 * Pre  : freyjaBegin(FREYJA_BONE);
 * Post : Mesh is added to Bone's child list
 *
 *        Either makes mesh tree connection or
 *        simulates by vertex weights and pivots
 *
 *-- History ------------------------------------------
 *
 * 2001.10.27: 
 * Mongoose - Created
 ------------------------------------------------------*/

void freyjaBoneAddChild1i(int32 boneIndex, int32 childIndex);
/*------------------------------------------------------
 * Pre  : freyjaBegin(FREYJA_BONE);
 * Post : Child is added to Bone's child list
 *
 *-- History ------------------------------------------
 *
 * 2001.10.27: 
 * Mongoose - Created
 ------------------------------------------------------*/

void freyjaBoneTranslate3f(int32 boneIndex, vec_t x, vec_t y, vec_t z);
void freyjaBoneTranslate3fv(int32 boneIndex, vec3_t xyz);
/*------------------------------------------------------
 * Pre  : freyjaBegin(FREYJA_BONE);
 * Post : Set bone relative position
 *
 *-- History ------------------------------------------
 *
 * 2001.10.27: 
 * Mongoose - Created
 ------------------------------------------------------*/

void freyjaBoneRotateEulerXYZ3f(int32 boneIndex, vec_t x, vec_t y, vec_t z);
void freyjaBoneRotateEulerXYZ3fv(int32 boneIndex, vec3_t xyz);
void freyjaBoneRotateQuatWXYZ4f(int32 boneIndex,vec_t w,vec_t x,vec_t y,vec_t z);
void freyjaBoneRotateQuatWXYZ4fv(int32 boneIndex, vec4_t wxyz);
/*------------------------------------------------------
 * Pre  : freyjaBegin(FREYJA_BONE);
 * Post : Set bone orientation
 *
 *-- History ------------------------------------------
 *
 * 2001.11.18: 
 * Mongoose - Created
 ------------------------------------------------------*/


void freyjaGroupCenter3f(vec_t x, vec_t y, vec_t z);

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

	void freyjaMeshPromoteTexcoordsToPloymapping(int32 meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Takes vertex UVs and promotes them to polymapped
	 *        texcoords ( stored in polygon ala texture polygons )
	 ------------------------------------------------------*/

	int freyjaMeshRemovePolygon(int32 meshIndex, int32 polygonIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Removes references to polygonIndex in mesh, but
	 *        it doesn't free the allocated polygon
	 ------------------------------------------------------*/

	void freyjaMeshUVMapSpherical(int32 meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Texcoords computed with Spherical mapping algorithm
	 *
	 ------------------------------------------------------*/

	void freyjaMeshUVMapCylindrical(int32 meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Texcoords computed with Cylindrical mapping algorithm
	 *
	 ------------------------------------------------------*/

	void freyjaMeshTesselateTriangles(int32 meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Divides all polygons in mesh into triangles
	 *
	 ------------------------------------------------------*/

	void freyjaMeshNormalFlip(int32 meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Flips all vertex normals in mesh
	 *
	 ------------------------------------------------------*/

	void freyjaMeshGenerateVertexNormals(int32 meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Recalculates all vertex normals in mesh
	 *
	 ------------------------------------------------------*/

int32 freyjaMeshPosition(int32 meshIndex, vec3_t xyz);

void freyjaMeshName1s(int32 meshIndex, const char *name);

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


void freyjaVertexFrame3f(int32 index, vec_t x, vec_t y, vec_t z);


	///////////////////////////////////////////////////////////////////////
	// Polygon
	///////////////////////////////////////////////////////////////////////

	void freyjaPolygonSplit(int32 meshIndex, int32 polygonIndex);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 *
	 * Post : Polygon is split into 2 smaller polygons 
	 ------------------------------------------------------*/

	void freyjaPolygonTexCoordPurge(int32 polygonIndex);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 *
	 * Post : All polymapped texcoords are dereferenced
	 ------------------------------------------------------*/

	int freyjaPolygonExtrudeQuad1f(int32 polygonIndex, vec_t dist);
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

	int freyjaPolygonExtrudeQuad(int32 polygonIndex, vec3_t normal);
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

	void freyjaPolygonAddVertex1i(int32 polygonIndex, int32 vertexIndex);
	void freyjaPolygonVertex1i(int32 index);  // DEPRECATED
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 * Post : Adds a vertex to a polygon
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void freyjaPolygonAddTexCoord1i(int32 polygonIndex, int32 texcoordIndex);
	void freyjaPolygonTexCoord1i(int32 index);  // DEPRECATED
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 * Post : Adds a texcoord to a polygon
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void freyjaPolygonSetMaterial1i(int32 polygonIndex, int32 materialIndex);
	void freyjaPolygonMaterial1i(int32 id);  // DEPRECATED
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

	int32 freyjaAnimationCreate();
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

	int32 freyjaAnimationBoneCreate(int32 animationIndex,
								   const char *name, int32 boneIndex);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 * Post : Creates a new Freyja Animation Bone object
	 *        Returns the new Bone's index or -1 on error
	 ------------------------------------------------------*/

	int32 freyjaAnimationBoneKeyFrameCreate(int32 animationIndex,
										   int32 boneIndex,
										   vec_t time, vec3_t xyz, vec4_t wxyz);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 *        Animation Bone <boneIndex> exists
	 *
	 * Post : Returns the keyframe's local Animation's Bone's
	 *        Keyframe element index or -1 on error
	 ------------------------------------------------------*/


	/* Animation Accessors */

	int32 freyjaGetAnimationCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the number of Animations being managed
	 ------------------------------------------------------*/

	int32 freyjaGetAnimationBoneCount(int32 animationIndex);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
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
	// Material ( 0.9.3 ABI, Can't be used with freyjaIterators )
	///////////////////////////////////////////////////////////////////////

	int32 freyjaMaterialCreate();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Creates a new Freyja Material object
	 *        Returns the new Material's index or -1 on error
	 ------------------------------------------------------*/


	/* Material Accessors */

	int32 freyjaGetMaterialCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the number of Materials being managed
	 ------------------------------------------------------*/

	int32 freyjaGetMaterialIndex(int32 materialIndex, int32 element);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns the gobal object index ( from the local
	 *        Material element index ) or -1 on error
	 ------------------------------------------------------*/

	char *freyjaGetMaterialName(int32 materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 *        Don't alter the returned string
	 *
	 * Post : Returns a pointer to NULL terminated name string
	 *        Returns 0x0 on error
	 ------------------------------------------------------*/

	int32 freyjaGetMaterialFlags(int32 materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns flags or -1 on error
	 ------------------------------------------------------*/

	int32 freyjaGetMaterialTexture(int32 materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns texture index or -1 for error or no texture
	 ------------------------------------------------------*/

	void freyjaGetMaterialAmbient(int32 materialIndex, vec4_t ambient);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns <ambient> color
	 ------------------------------------------------------*/

	void freyjaGetMaterialDiffuse(int32 materialIndex, vec4_t diffuse);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns <diffuse> color
	 ------------------------------------------------------*/

	void freyjaGetMaterialSpecular(int32 materialIndex, vec4_t specular);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns <specular> color
	 ------------------------------------------------------*/

	void freyjaGetMaterialEmissive(int32 materialIndex, vec4_t emissive);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns <emissive> color
	 ------------------------------------------------------*/

	vec_t freyjaGetMaterialShininess(int32 materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns specular exponent or -1.0 on error
	 ------------------------------------------------------*/

	vec_t freyjaGetMaterialTransparency(int32 materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns transparency or -1.0 on error
	 ------------------------------------------------------*/

	int32 freyjaGetMaterialBlendSource(int32 materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns blend source factor or -1 on error
	 ------------------------------------------------------*/

	vec_t freyjaGetMaterialBlendDestination(int32 materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns blend destination factor or -1 on error
	 ------------------------------------------------------*/


	/* Material Mutators */

	void freyjaMaterialName(int32 materialIndex, const char *name);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's <name> id is set
	 ------------------------------------------------------*/

	void freyjaMaterialFlags(int32 materialIndex, int32 flags);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's bit <flags> are set
	 ------------------------------------------------------*/

	void freyjaMaterialTextureFilename(int32 materialIndex, const char *filename);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material textures's <filename> id is set
	 ------------------------------------------------------*/

	void freyjaMaterialTexture(int32 materialIndex, int32 textureIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's <texture> index is set
	 ------------------------------------------------------*/

	void freyjaMaterialAmbient(int32 materialIndex, const vec4_t ambient);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's <ambient> color is set
	 ------------------------------------------------------*/

	void freyjaMaterialDiffuse(int32 materialIndex, const vec4_t diffuse);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's <diffuse> color is set
	 ------------------------------------------------------*/

	void freyjaMaterialSpecular(int32 materialIndex, const vec4_t specular);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's <specular> color is set
	 ------------------------------------------------------*/

	void freyjaMaterialEmissive(int32 materialIndex, const vec4_t emissive);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's <emissive> color is set
	 ------------------------------------------------------*/

	void freyjaMaterialShininess(int32 materialIndex, vec_t exponent);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's Specular <exponent> is set
	 ------------------------------------------------------*/

	void freyjaMaterialTransparency(int32 materialIndex, vec_t transparency);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 *        <transparency> is a value from 0.0 to 1.0
	 *
	 * Post : Material's <transparency> is set
	 ------------------------------------------------------*/

	void freyjaMaterialBlendSource(int32 materialIndex, uint32 factor);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's blend source <factor> is set
	 ------------------------------------------------------*/

	void freyjaMaterialBlendDestination(int32 materialIndex,
										uint32 factor);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's blend destination <factor> is set
	 ------------------------------------------------------*/



	///////////////////////////////////////////////////////////////////////
	// Faster access API for modeler use
	///////////////////////////////////////////////////////////////////////

	vec3_t *freyjaGetVertexXYZ(int32 vertexIndex);
	vec2_t *freyjaGetVertexUV(int32 vertexIndex);
	vec2_t *freyjaGetTexCoordUV(int32 texcoordIndex);

	char freyjaGetModelAppendMeshMode(int32 modelIndex);
	/*------------------------------------------------------
	 * Pre  : Model <modelIndex> exists and has an active copy buffer
	 * Post : Model's mesh buffer mode is returned or -1 on error
	 *         enabled when returning  1
	 *         disabled when returning  0 
	 ------------------------------------------------------*/

	void freyjaModelAppendMeshMode(int32 modelIndex, char on);
	/*------------------------------------------------------
	 * Pre  : Model <modelIndex> exists and has an active copy buffer
	 * Post : Model's mesh buffer mode is 
	 *         enabled when <on> = 1
	 *         disabled when <on> = 0 
	 ------------------------------------------------------*/

	char freyjaModelCopyMesh(int32 modelIndex, int mesh, int frame);
	/*------------------------------------------------------
	 * Pre  : Model <modelIndex> exists and has an active copy buffer
	 * Post : Model's copy buffer is modified to include new mesh duplicate
	 *        using the *local <mesh> index and vertex morph <frame>
	 ------------------------------------------------------*/

	char freyjaModelPasteMesh(int32 modelIndex);
	/*------------------------------------------------------
	 * Pre  : Model <modelIndex> exists and has an active copy buffer
	 * Post : Model's copy buffer is added to freyja as world object
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	// Plugin import/export iteraction setup
	///////////////////////////////////////////////////////////////////////

	typedef enum {
		
		FREYJA_PLUGIN_NONE            = 0,
		FREYJA_PLUGIN_MESH            = 1,
		FREYJA_PLUGIN_SKELETON        = 2,
		FREYJA_PLUGIN_VERTEX_MORPHING = 4,
		FREYJA_PLUGIN_VERTEX_BLENDING = 8

	} freyja_plugin_options_t;


	void freyjaPluginBegin();

	void freyjaPluginDescription1s(const char *info_line);

	void freyjaPluginAddExtention1s(const char *ext);

	void freyjaPluginImport1i(int32 flags);

	void freyjaPluginExport1i(int32 flags);

	void freyjaPluginArg1i(const char *name, int32 defaults);

	void freyjaPluginArg1f(const char *name, float defaults);

	void freyjaPluginArg1s(const char *name, const char *defaults);

	void freyjaPluginEnd();


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

	int32 freyjaPakBegin(const char *filename);
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

	int32 freyjaPakAddFullPathFile(int32 pakIndex,
								  const char *vfsFilename,
								  int32 offset, int32 size);
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

	void freyjaPakEnd(int32 pakIndex);
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

	void freyjaKill();
}


/* Mongoose 2004.12.19, 
 * C++ fun */

char freyjaModelCopyVertexList(int32 modelIndex, 
							   Vector<unsigned int> &list,
							   int mesh, int frame);
/*------------------------------------------------------
 * Pre  : Model <modelIndex> exists and has an active copy buffer
 * Post : Model's copy buffer is modified to include new mesh duplicate
 *        using the *local <mesh> index and vertex morph <frame>
 ------------------------------------------------------*/

// FreyjaSkeletalAnimation *freyjaGetAnimation(int32 animationIndex);

Vector<unsigned int> *freyjaFindVerticesByBox(vec3_t bbox[2]);

void freyjaGetVertexPolygonRef1i(int32 vertexIndex, Vector<long> &polygons);
void freyjaGetVertexPolygonRef(Vector<long> &polygons);
/*------------------------------------------------------
 * Pre  : 
 * Post : Returns a list of indices of polygons that
 *        use current vertex in iterator
 *
 *        List can be empty
 *
 *-- History ------------------------------------------
 *
 * 2004.12.17:
 * Mongoose - Created, wrapper for old Egg style
 *            reverse reference system ( very handy )
 ------------------------------------------------------*/


#endif
