/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (C)2001-2006 Mongoose
 * Comments: GL context rendering backend for Freyja
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator scripopt.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2006.07.15:
 * Mongoose - Centralized various graphics util functions here
 *            Normalized the API to be more consistant
 ==========================================================================*/

#ifndef GUARD__FREYJA3D_OPENGL_H__
#define GUARD__FREYJA3D_OPENGL_H__

#include "config.h"
#include <freyja/PixelBuffer.h>


namespace freyja3d {

class OpenGL
{
public:

	////////////////////////////////////////////////////////////
	// Singleton methods.
	////////////////////////////////////////////////////////////

	static OpenGL* GetInstance( )
	{ return ( mInstance ? mInstance : ( mInstance = new OpenGL() ) ); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get instace of this singleton.
	 *
	 ------------------------------------------------------*/

	static void Destroy()
	{ 
		delete mInstance;
		mInstance = NULL; 
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public static methods.
	////////////////////////////////////////////////////////////

	static const char *BlendIntToString(int32 i);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Convert blend symbols to values.
	 *
	 ------------------------------------------------------*/

	static int32 BlendStringToInt(const char *str);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Convert blend values to symbols.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// OpenGL
	////////////////////////////////////////////////////////////

	void InitContext( uint32 width, uint32 height );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	int LoadTexture( const char* filename, int overwrite_id = -1 );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Reads texture from disk into memory.
	 *        Returns texture id on success.
	 *        Returns -1 on error.
	 *
	 ------------------------------------------------------*/

	int LoadTexture( byte *image, uint32 width, uint32 height, 
					 freyja::PixelFormat format, int overwrite_id = -1 );
	/*------------------------------------------------------
	 * Pre  : image must be a valid pixmap that agrees
	 *        with mode, width, and height
	 *
	 * Post : Returns texture id or < 0 error flag
	 *
	 ------------------------------------------------------*/

	int LoadTexture( freyja::Pixel& pixel, uint32 width, uint32 height );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Generates and loads a solid color texture,
	 *        returns texture Id or -1 if failed
	 *
	 ------------------------------------------------------*/

	void SetMaxTextureCount( uint32 max );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static void BindTexture( uint32 texture_unit, uint16 id );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Multitexture bind texture map to texel unit.
	 *
	 ------------------------------------------------------*/

	void ResizeContext( uint32 width, uint32 height );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void ResetTextures( );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool TakeScreenShot( const char* basename, const char* format );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Dump a TGA of the context if format isn't supported.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// ARB 
	////////////////////////////////////////////////////////////

	static void BindFragmentARB(int32 fragmentId);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : if <fragmentId> < 0 fragments are disabled.
	 *        Otherwise the referenced fragment is bound.
	 ------------------------------------------------------*/

	static bool LoadFragmentARB(const char *filename, uint32 &fragmentId);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// GLSL
	////////////////////////////////////////////////////////////

	// TODO: Might rename these to reflect program/object and no use 
	// in being parallel to ARB fragment methods above

	static void BindFragmentGLSL(int32 fragmentId);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : if <fragmentId> < 0 fragments are disabled.
	 *        Otherwise the referenced fragment is bound.
	 ------------------------------------------------------*/

	static void DeleteFragmentGLSL(int32 fragmentId);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Delete the given GLSL object.
	 *
	 ------------------------------------------------------*/

	static void DebugFragmentGLSL(const char *comment, int32 fragmentId);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Dump the GLSL log for this object to a dialog.
	 *
	 ------------------------------------------------------*/

	static bool LoadFragmentGLSL(const char *filename, uint32 &fragmentId);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Load a GLSL fragment program.
	 *
	 ------------------------------------------------------*/

	static void BindVertexGLSL(int32 vertexId);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : if <vertexId> < 0 fragments are disabled.
	 *        Otherwise the referenced fragment is bound.
	 ------------------------------------------------------*/

	static void DebugVertexGLSL(int32 vertexId);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Dump the GLSL log for this object to a dialog.
	 *
	 ------------------------------------------------------*/

	static bool LoadVertexGLSL(const char *filename, uint32 &vertexId);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Load a GLSL vertex program.
	 *
	 ------------------------------------------------------*/
	
	static void Uniform1i( int32 programId, const char* symbol, uint32 value );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Define a uniform for GLSL shader use.
	 *
	 ------------------------------------------------------*/


 protected:

	////////////////////////////////////////////////////////////
	// Protected methods.
	////////////////////////////////////////////////////////////

	OpenGL();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of OpenGL
	 *
	 ------------------------------------------------------*/

	~OpenGL( );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Destructor.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Protected attributes.
	////////////////////////////////////////////////////////////

	/* Extentions */
	static bool arb_multitexture;
	static bool arb_texture_env_combine;
	static bool arb_vertex_shader;
	static bool arb_fragment_shader;
	static bool arb_shader_objects;
	static bool arb_shadow;
	static bool arb_depth_texture;
	static bool arb_shading_language_100;
	static bool arb_vertex_buffer_object;
	static bool ext_cg_shader;

	static uint32 mObjects;     /* Count how many objects we load */

	/* Only support one context again. */
	bool mInit;
	uint32 mWidth;
	uint32 mHeight;
	vec_t mAspectRatio;
	vec_t mNearHeight;
	vec_t mNear;
	vec_t mFar;
	vec_t mScaleEnv;

	/* Hardware/driver limits. */
	uint32 mTextureUnitCount;
	uint32 mMaxLightsCount;

	/* Textures. */
	//uint32 mFlags;              /* Bitmap options */

	bool mUseMipmaps;

	static void* mTextureIds;   /* GL texture list */

	uint32 mTextureCount;	    /* Texture counter */
	
	uint32 mTextureLimit;	    /* The texture limit */
	
	int32 mTextureId;	    	/* Currently bound texture id */
	
	int32 mTextureId2;			/* Multitexture Texture Id */


 private:

	////////////////////////////////////////////////////////////
	// Private methods.
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private attributes.
	////////////////////////////////////////////////////////////

	static OpenGL* mInstance;
};

} /* namespace freyja3d */


////////////////////////////////////////////////////////////
// Gobal functions
////////////////////////////////////////////////////////////

bool mglHardwareExtTest(const char *ext);

void mgl2dProjection(const unsigned int width, const unsigned int height);

void mglDrawBbox(const vec3_t min, const vec3_t max, 
				 const vec4_t pointColor, const vec4_t lineColor);

void mglDrawSelectionBox(const vec3_t min, const vec3_t max, const vec4_t color);

void mglDrawSelectBox(const vec3_t min, const vec3_t max, const vec4_t color);

void mglDrawCone(vec3_t origin, vec_t height, uint32 count);

void mglDraw3dCircle(const vec3_t center, const vec_t radius, uint32 count, 
					 uint32 plane, bool solid);

void mglDrawControlPoint();

void mglDrawBoneSolid(const vec3_t pos);

void mglDrawEditorAxis();

void mglDrawAxisWithCones(const vec_t min, const vec_t mid, const vec_t max,
						  uint32 count);

void mglDrawAxisWithLines(const vec_t min, const vec_t mid, const vec_t max);

void mglDraw3dCursor(vec3_t pos, 
					 const vec_t min, const vec_t mid, const vec_t max);

void mglDrawSphere(int numMajor, int numMinor, float radius);

void mglDrawBone(unsigned char type, const vec3_t pos);

void mglDrawJoint(unsigned char type, const vec3_t pos);

void mglDrawPlane(vec_t size, vec_t step, vec_t scale);

void mglDrawGrid(vec3_t color, vec_t size, vec_t step, vec_t scale);

void mglGetOpenGLViewport(int *viewportXYWH); // int[4]

void mglGetOpenGLModelviewMatrix16fv(matrix_t modelview);

void mglGetOpenGLProjectionMatrix16fv(matrix_t projection);

#endif /* GUARD__FREYJA3D_OPENGL_H__ */
