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
	long version;
	long flags;
	long reserved;
	char comment[64];

} freyja_file_header_t;


typedef struct {

	long type;
	long size;
	long flags;
	long version;

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

	long freyjaIterator(freyja_object_t type, long item);
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

	long freyjaCriticalSection(freyja_lock_t request);
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

	long freyjaGetCount(freyja_object_t type);
	/*------------------------------------------------------
	 * Pre  : <type> is valid
	 * Post : Returns total number of objects of type in 
	 *        parent frame type eg [model]'s meshes
	 *
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	long freyjaGetCurrent(freyja_object_t type);
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

	long freyjaGetBoneName(long boneIndex, unsigned int size, char *name);
	/*------------------------------------------------------
	 * Pre  : <name> must be allocated to <size> width
	 *        A <size> of 64 is recommended
	 *
	 * Post : Gets bone[index]'s name as '\0' terminated string
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	long freyjaGetBoneParent(long boneIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns bone[index]'s parent id
	 *        Returns -2 on error
	 ------------------------------------------------------*/

	long freyjaGetBoneRotationWXYZ4fv(long boneIndex, vec4_t wxyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets bone[index]'s orientation as a Quaternion
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	long freyjaGetBoneRotationXYZ3fv(long boneIndex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets bone[index]'s orientation in Euler angles
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	long freyjaGetBoneTranslation3fv(long index, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets bone[index]'s position
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/


	// FREYJA_TEXCOORD Accessors //////////////////////////////////////////

	void freyjaGetTexCoord2fv(long texcoordIndex, vec2_t uv);
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

	long freyjaGetVertexTexCoordUV2fv(long vertexIndex, vec2_t uv);
	long freyjaGetVertexNormalXYZ3fv(long vertexIndex, vec3_t nxyz);
	long freyjaGetVertexXYZ3fv(long vertexIndex, vec3_t xyz);
	long freyjaGetVertexFrame(long vertexIndex, long element,
							  long *frameIndex, vec3_t xyz);
	long freyjaGetVertexFrameCount(long vertexIndex);
	long freyjaGetVertexWeight(long vertexIndex, long element,
							   long *bone, vec_t *weight);
	long freyjaGetVertexWeightCount(long vertexIndex);
	long freyjaGetVertexFlags(long vertexIndex);

// FREYJA_MODEL Accessors
long freyjaGetModelFlags(long modelIndex);
long freyjaGetModelMeshIndex(long modelIndex, long element);
long freyjaGetModelMeshCount(long modelIndex);

// FREYJA_MESH Accessors
long freyjaGetMeshFlags(long meshIndex);
long freyjaGetMeshPosition(long meshIndex, vec3_t xyz);
long freyjaGetMeshVertexIndex(long meshIndex, long element); // Not Implemented
long freyjaGetMeshVertexCount(long meshIndex); // Not Implemented
long freyjaGetMeshPolygonIndex(long meshIndex, long element);
long freyjaGetMeshPolygonCount(long meshIndex);
long freyjaGetMeshVertexGroupIndex(long meshIndex, long element);
long freyjaGetMeshVertexGroupCount(long meshIndex);
long freyjaGetMeshVertexFrameIndex(long meshIndex, long element); // Not Implemented
long freyjaGetMeshVertexFrameCount(long meshIndex); // Not Implemented

// FREYJA_POLYGON Accessors
long freyjaGetPolygonMaterial(long polygonIndex);
long freyjaGetPolygonFlags(long polygonIndex);
long freyjaGetPolygonEdgeCount(long polygonIndex);
long freyjaGetPolygonVertexCount(long polygonIndex);
long freyjaGetPolygonTexCoordCount(long polygonIndex);
long freyjaGetPolygonVertexIndex(long polygonIndex, long element);
long freyjaGetPolygonTexCoordIndex(long polygonIndex, long element);


long freyjaGetPolygon1u(freyja_object_t type, long item, long *value);
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

long freyjaGetPolygon3f(freyja_object_t type, long item, vec_t *value);
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


long freyjaMeshPosition(long meshIndex, vec3_t xyz);

void freyjaSetNormal3f(unsigned int index, vec_t x, vec_t y, vec_t z);
void freyjaSetNormal3fv(unsigned int index, vec3_t xyz);

void freyjaSetTexCoord2f(unsigned int index, vec_t u, vec_t v);
void freyjaSetTexCoord2fv(unsigned int index, vec2_t uv);

void freyjaSetVertex3f(unsigned int index, vec_t x, vec_t y, vec_t z);
void freyjaSetVertex3fv(unsigned int index, vec3_t xyz);

long freyjaNormal3f(vec_t x, vec_t y, vec_t z);
long freyjaNormal3fv(vec3_t xyz);
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

long freyjaTexCoord2f(vec_t u, vec_t v);
long freyjaTexCoord2fv(vec2_t uv);
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

void freyjaVertexTexCoord2f(long index, vec_t u, vec_t v);
void freyjaVertexTexCoord2fv(long index, vec2_t uv);
void freyjaVertexNormal3f(long index, vec_t nx, vec_t ny, vec_t nz);
void freyjaVertexNormal3fv(long index, vec3_t nxyz);
long freyjaVertex3f(vec_t x, vec_t y, vec_t z);
long freyjaVertex3fv(vec3_t xyz);
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

void freyjaVertexWeight(long index, vec_t weight, long bone);
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

void freyjaMeshTreeFrameAddBone(long tag);
/*------------------------------------------------------
 * Pre  : eggBegin(FREYJA_MESHTREE_ANIM_FRAME);
 * Post : Appends tag to mesh tree frame
 *
 *-- History ------------------------------------------
 *
 * 2001.11.18: 
 * Mongoose - Created
 ------------------------------------------------------*/

long freyjaTextureFilename1s(const char *filename);

long freyjaTextureStoreBuffer(unsigned char *image, unsigned int depth,
							  unsigned int width, unsigned int height,
							  freyja_colormode_t type);

void freyjaBoneFlags1i(long boneIndex, long flags);
/*------------------------------------------------------
 * Pre  : freyjaBegin(FREYJA_BONE);
 * Post : Set bone flags
 *
 *-- History ------------------------------------------
 *
 * 2001.10.27: 
 * Mongoose - Created
 ------------------------------------------------------*/

void freyjaBoneParent1i(long boneIndex, long parentIndex);
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

void freyjaBoneName1s(long boneIndex, char *name);
/*------------------------------------------------------
 * Pre  : freyjaBegin(FREYJA_BONE);
 * Post : Set human readable bone name
 *
 *-- History ------------------------------------------
 *
 * 2001.10.27: 
 * Mongoose - Created
 ------------------------------------------------------*/

void freyjaBoneAddMesh1i(long boneIndex, long meshIndex);
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

void freyjaBoneAddChild1i(long boneIndex, long childIndex);
/*------------------------------------------------------
 * Pre  : freyjaBegin(FREYJA_BONE);
 * Post : Child is added to Bone's child list
 *
 *-- History ------------------------------------------
 *
 * 2001.10.27: 
 * Mongoose - Created
 ------------------------------------------------------*/

void freyjaBoneTranslate3f(long boneIndex, vec_t x, vec_t y, vec_t z);
void freyjaBoneTranslate3fv(long boneIndex, vec3_t xyz);
/*------------------------------------------------------
 * Pre  : freyjaBegin(FREYJA_BONE);
 * Post : Set bone relative position
 *
 *-- History ------------------------------------------
 *
 * 2001.10.27: 
 * Mongoose - Created
 ------------------------------------------------------*/

void freyjaBoneRotateEulerXYZ3f(long boneIndex, vec_t x, vec_t y, vec_t z);
void freyjaBoneRotateEulerXYZ3fv(long boneIndex, vec3_t xyz);
void freyjaBoneRotateQuatWXYZ4f(long boneIndex,vec_t w,vec_t x,vec_t y,vec_t z);
void freyjaBoneRotateQuatWXYZ4fv(long boneIndex, vec4_t wxyz);
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

void freyjaGenerateCircleMesh(vec3_t origin, long count); // radius

void freyjaGenerateConeMesh(vec3_t origin, vec_t height, long count); // radius

void freyjaGenerateCylinderMesh(vec3_t origin, vec_t height, 
								long count, long segments); // radius

void freyjaGenerateSphereMesh(vec3_t origin, vec_t radius, 
							  long count, long segments);

void freyjaGenerateTubeMesh(vec3_t origin, vec_t height, 
							long count, long segments); // radius

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


void freyjaVertexFrame3f(long index, vec_t x, vec_t y, vec_t z);

	long freyjaCheckModel(const char *filename);

	long freyjaLoadModel(const char *filename);

	long freyjaSaveModel(const char *filename);

	///////////////////////////////////////////////////////////////////////
	// Polygon
	///////////////////////////////////////////////////////////////////////

	void freyjaPolygonAddVertex1i(long polygonIndex, long vertexIndex);
	void freyjaPolygonVertex1i(long index);  // DEPRECATED
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 * Post : Adds a vertex to a polygon
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void freyjaPolygonAddTexCoord1i(long polygonIndex, long texcoordIndex);
	void freyjaPolygonTexCoord1i(long index);  // DEPRECATED
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 * Post : Adds a texcoord to a polygon
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void freyjaPolygonSetMaterial1i(long polygonIndex, long materialIndex);
	void freyjaPolygonMaterial1i(long id);  // DEPRECATED
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

	void freyjaPluginImport1i(long flags);

	void freyjaPluginExport1i(long flags);

	void freyjaPluginArgInt(const char *name, long defaults);

	void freyjaPluginArgFloat(const char *name, float defaults);

	void freyjaPluginArgString(const char *name, const char *defaults);

	void freyjaPluginEnd();


	///////////////////////////////////////////////////////////////////////
	//  Plugin import/export iteraction
	///////////////////////////////////////////////////////////////////////

	long freyjaGetPluginId();

	int freyjaGetPluginArg1f(long pluginId, const char *name, float &arg);

	int freyjaGetPluginArg1i(long pluginId, const char *name, long &arg);

	int freyjaGetPluginArg1s(long pluginId, const char *name, 
							 long len, const char *arg);


	///////////////////////////////////////////////////////////////////////
	//  Pak VFS 
	///////////////////////////////////////////////////////////////////////

	long freyjaPakBegin(const char *filename);
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

	long freyjaPakAddFullPathFile(long pakIndex,
								  const char *vfsFilename,
								  long offset, long size);
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

	void freyjaPakEnd(long pakIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Finalizes VFS for pakIndex
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.01.02:
	 * Mongoose - Created
	 ------------------------------------------------------*/
}


/* Mongoose 2004.12.19, 
 * C++ fun */


Vector<unsigned int> *freyjaFindVerticesByBox(vec3_t bbox[2]);


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
