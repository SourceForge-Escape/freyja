/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (C) 2004-2006 Mongoose
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

#ifndef GUARD__FREYJA_FREYJA__H_
#define GUARD__FREYJA_FREYJA__H_

#include <hel/math.h>

#define FREYJA_API_VERSION   "Freyja 0.9.5"

/* index_t invalid state is equal to UINT_MAX 32bit */
#define INDEX_INVALID      4294967295U

/* Special messages visible in all builds */
#define BUG_ME freyjaPrintMessage("\n[%s] %s:%i, %s() file bug with %s", FREYJA_API_VERSION, __FILE__, __LINE__, __func__, EMAIL_ADDRESS); freyjaPrintMessage   

#define MARK_MSG(msg) freyjaPrintMessage("[%s] %s:%i, %s() %s", FREYJA_API_VERSION, __FILE__, __LINE__, __func__, msg)

#define MARK_MSGF freyjaPrintMessage("\n[%s] %s:%i, %s() ", FREYJA_API_VERSION, __FILE__, __LINE__, __func__); freyjaPrintMessage

#define OBS_CALL(...) freyjaPrintMessage("[%s] %s:%i, %s() Obsolete ABI call", FREYJA_API_VERSION, __FILE__, __LINE__, __func__)


#if ENABLE_FREYJA_ASSERTS
#   define FREYJA_ASSERTMSG(expr, format, ...) \
if (!(expr)) freyjaAssertMessage(__FILE__, __LINE__, __func__, #expr, false, format, ##__VA_ARGS__)
#   define FREYJA_INFOMSG(expr, format, ...) \
if (!(expr)) freyjaDebugInfoMessage(__FILE__, __LINE__, __func__, #expr, false, format, ##__VA_ARGS__)
#else
#   define FREYJA_ASSERTMSG(...)
#   define FREYJA_INFOMSG(...)
#endif


extern "C" {

	typedef uint32 index_t;

	typedef unsigned char byte;

	typedef void* ptr_t;

	typedef void* freyja_ptr;  /* New for 0.10, 
								* Replaces index_t use to aid parallel code from C ABI. */

	typedef uint16 freyja_id;  /* New for 0.10 smaller range for allocated types. */

	typedef int (*FreyjaAssertCallback)(const char* file, unsigned int line, 
										const char* function,
										const char* expression,
										const char* message);

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
		fUnion = 1,
		fDifference,
		fIntersection
	
	} freyja_csg_t;


	typedef enum {
		fTransformActionNone = 0,
		fTranslate = 1,
		fRotate,
		fScale,
		fRotateAboutOrigin,
		fScaleAboutOrigin
	
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
		FREYJA_PLUGIN_VERTEX_BLENDING = 8,
		FREYJA_PLUGIN_PAK_VFS         = 16,
		FREYJA_PLUGIN_THREAD_SAFE     = 32

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

	void freyjaAssertHandler(FreyjaAssertCallback func);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Assertion event handler is assigned.
	 ------------------------------------------------------*/

	byte freyjaAssertMessage(const char *file, unsigned int line, 
							 const char *function, const char *exprString,
							 bool expr, const char *format, ...);
	/*------------------------------------------------------
	 * Pre  : Format string and args are valid
	 * Post : Report messages to stdout or gPrinter
	 ------------------------------------------------------*/

	void freyjaDebugInfoHandler(FreyjaAssertCallback func);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Assertion event handler is assigned.
	 ------------------------------------------------------*/

	byte freyjaDebugInfoMessage(const char *file, unsigned int line, 
								const char *function, const char *exprString,
								bool expr, const char *format, ...);
	/*------------------------------------------------------
	 * Pre  : Format string and args are valid
	 * Post : Report messages to stdout or gPrinter
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

	void freyjaGetGenericTransform3fv(freyja_transform_t transform,
									  freyja_transform_action_t action,
									  index_t id, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets a transform
	 ------------------------------------------------------*/
		
	void freyjaGenericTransform3fv(freyja_transform_t transform,
								   freyja_transform_action_t action,
								   index_t id, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets a transform generically 
	 ------------------------------------------------------*/

	const char *freyjaObjectToString(freyja_object_t obj);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	const char *freyjaTransformToString(freyja_transform_t transform);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	const char *freyjaActionToString(freyja_transform_action_t action);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
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

	int32 freyjaExportModelByModule(const char *filename, const char *module);
	/*------------------------------------------------------
	 * Pre  : <filename> - Filename of the model to be exported
	 *        <module> - Filename of the module to use
	 * Post : Exports model using the exact module passed in...
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

	int32 freyjaImportModelByModule(const char *filename, const char *module);
	/*------------------------------------------------------
	 * Pre  : <filename> - Filename of the model to be imported
	 *        <module> - Filename of the module to use
	 * Post : Imports model using the exact module passed in...
	 *        Returns 0 on sucess
	 ------------------------------------------------------*/

	const char* libfreyjaVersion();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : String version info.
	 *        
	 ------------------------------------------------------*/

}


/* Internal memory management and logging system ala mtk_memeory */
#   if defined ( __cplusplus )
#      include <stddef.h>
#      include "MemTrack.h"

#      if USING_FREYJA_MEMORY
void freyjaRemoveTrack(void *ptr, const char *file, int line, const char *func, uint32 flags);
#         define FREYJA_NEW new( __FILE__, __LINE__, __func__)
#         define FREYJA_DELETE freyjaRemoveTrack(NULL, __FILE__, __LINE__, __func__, 0x2); delete
#         define freyjaNew FREYJA_NEW
#         define freyjaDelete FREYJA_DELETE

void *operator new(size_t size, const char *file, int line, const char *func);
void *operator new [](size_t size, const char *file, int line, const char *func);

// Default parms to match new makes doing this unreasonable unless
// I use the mtk_memeory tricks, which overrides *ALL deletes 
//void operator delete(void *p);
//void operator delete [](void *p); 

#      else
#         define FREYJA_NEW new
#         define FREYJA_DELETE delete
#      endif // USING_FREYJA_MEMORY
#   endif // __cplusplus


#endif // GUARD__FREYJA_FREYJA__H_
