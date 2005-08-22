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


#define FREYJA_API_VERSION   "Freyja 0.9.3"

/* index_t invalid state is equal to UINT_MAX 32bit */
#define INDEX_INVALID      4294967295U

/* FSM uses index_t with extentions by these error/states */
#define FREYJA_NEXT     (INDEX_INVALID - 1)
#define FREYJA_RESET    (INDEX_INVALID - 2)
#define FREYJA_CURRENT  (INDEX_INVALID - 3)
#define FREYJA_SIZE     (INDEX_INVALID - 4)
#define FREYJA_ERROR    (INDEX_INVALID - 5)

/* Flags used to determine polygon texcoord data paking or special flags */
#define fPolygon_VertexUV      2
#define fPolygon_PolyMapped    8
#define fPolygon_PolyMappedUV  8
#define fPolygon_ColorMapped   16
#define fPolygon_Alpha         32

extern "C" {

	typedef uint32 index_t;

	typedef unsigned char byte;

	typedef enum {
		INDEXED_8 = 1, 
		RGB_24, 
		RGBA_32

	} freyja_colormode_t;


	typedef enum {
		FREYJA_MODEL = 1,
		FREYJA_MESH,
		FREYJA_POLYGON,
		FREYJA_BONE,
		FREYJA_SKELETON,
		FREYJA_VERTEX_FRAME,
		FREYJA_VERTEX_GROUP,
		FREYJA_VERTEX,
		FREYJA_TEXCOORD,
		FREYJA_MATERIAL,
		FREYJA_TEXTURE,
		FREYJA_SKEL_ANIMATION,
		FREYJA_SKEL_KEYFRAME
	
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


	enum freyja_material_flags {
		fFreyjaMaterial_Blending = 1,
		fFreyjaMaterial_Texture = 2,
		fFreyjaMaterial_DetailTexture = 4,
		fFreyjaMaterial_Normalize = 8
	};


	typedef enum {
		
		FREYJA_PLUGIN_NONE            = 0,
		FREYJA_PLUGIN_MESH            = 1,
		FREYJA_PLUGIN_SKELETON        = 2,
		FREYJA_PLUGIN_VERTEX_MORPHING = 4,
		FREYJA_PLUGIN_VERTEX_BLENDING = 8

	} freyja_plugin_options_t;


	///////////////////////////////////////////////////////////////////////
	//  Freyja library util functions
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

	void *freyjaModuleImportFunction(void *handle, const char *name);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : System facade for shared object function import
	 ------------------------------------------------------*/

	void *freyjaModuleLoad(const char *module);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : System facade for shared object loading
	 ------------------------------------------------------*/

	void freyjaModuleUnload(void *handle);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : System facade for shared object unloading
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	//  File I/O
	///////////////////////////////////////////////////////////////////////

	int32 freyjaCheckModel(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Checks if model is in native format
	 *        Returns 0 on sucess
	 ------------------------------------------------------*/

	int32 freyjaLoadModel(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Loads model in native format
	 *        Returns 0 on sucess
	 ------------------------------------------------------*/

	int32 freyjaSaveModel(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Saves model in native format
	 *        Returns 0 on sucess
	 ------------------------------------------------------*/

	int32 freyjaExportModel(const char *filename, const char *type);
	/*------------------------------------------------------
	 * Pre  : type is module basename eg 'md5.so' -> 'md5'
	 * Post : Exports model using plugins
	 *        Returns 0 on sucess
	 ------------------------------------------------------*/

	int32 freyjaImportModel(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Imports model using plugins
	 *        Returns 0 on sucess
	 ------------------------------------------------------*/


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

	index_t freyjaCriticalSectionLock();
	void freyjaCriticalSectionUnlock(); // for null locks ( fake )
	void freyjaCriticalSectionUnLock(index_t lockId);
	/*------------------------------------------------------
	 * Pre  : FreyjaPlugin singleton exists
	 *
	 * Post : If FreyjaPlugin is locked this will block
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

	void freyjaGroupCenter3f(vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : freyjaIterator has initialized a vertexgroup
	 * Post : Sets vertexgroups[FREYJA_LIST_CURRENT]'s
	 *        pivot / center
	 ------------------------------------------------------*/

	void freyjaPolygonVertex1i(index_t index);
	void freyjaPolygonTexCoord1i(index_t index);
	void freyjaPolygonMaterial1i(index_t id);
	/*------------------------------------------------------
	 * Pre  : freyjaIterator has initialized a polygon
	 *
	 * Post : Appends/sets polygon[FREYJA_LIST_CURRENT]'s
	 *        vertices, texcoord, or material
	 ------------------------------------------------------*/

	void freyjaGetVertexTexCoord2fv(vec2_t uv);
	void freyjaGetVertexNormal3fv(vec3_t xyz);
	void freyjaGetVertex3fv(vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : freyjaIterator has initialized a vertex
	 *
	 * Post : Gets vertex[FREYJA_LIST_CURRENT]'s
	 *        position, normal, or texcoord
	 ------------------------------------------------------*/

	index_t freyjaVertexCreate3f(vec_t x, vec_t y, vec_t z);
	index_t freyjaVertexCreate3fv(vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_GROUP);
	 *        x,y,z are valid 3space coors
	 * Post : A new vertex created in the model
	 *        Returns the native index of that vertex
	 ------------------------------------------------------*/

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

	void freyjaPluginImport1i(int32 flags);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginExport1i(int32 flags);
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

}

#endif
