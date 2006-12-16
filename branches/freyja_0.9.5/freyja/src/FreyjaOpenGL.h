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

#ifndef GUARD__FREYJA_FREYJAOPENGL_H_
#define GUARD__FREYJA_FREYJAOPENGL_H_

#ifdef HAVE_OPENGL
#   ifdef MACOSX
#      include <OpenGL/OpenGL.h>
#      include <OpenGL/gl.h>
#      include <OpenGL/glu.h>
#   elif WIN32
#      include <GL/gl.h>
#      include <GL/glu.h>
#      include <GL/glext.h>
#      include <windows.h>
#      include <wingdi.h>
#      define mglGetProcAddress(string) wglGetProcAddress(string)
#   else
#      include <GL/gl.h>
#      include <GL/glu.h>
#      include <GL/glext.h>
#      define GLX_GLXEXT_PROTOTYPES
#      include <GL/glx.h>
#      include <GL/glxext.h>
#      define mglGetProcAddress(string) glXGetProcAddressARB((GLubyte *)string)
#   endif
#else
#   error "This module requires an OpenGL SDK"
#endif

#if defined(HAVE_OPENGL) && defined(USING_OPENGL_EXT)
extern PFNGLMULTITEXCOORD1FARBPROC h_glMultiTexCoord1fARB;
extern PFNGLMULTITEXCOORD2FARBPROC h_glMultiTexCoord2fARB;
extern PFNGLMULTITEXCOORD3FARBPROC h_glMultiTexCoord3fARB;
extern PFNGLMULTITEXCOORD4FARBPROC h_glMultiTexCoord4fARB;
extern PFNGLACTIVETEXTUREARBPROC h_glActiveTextureARB;
extern PFNGLCLIENTACTIVETEXTUREARBPROC h_glClientActiveTextureARB;

extern PFNGLGENPROGRAMSARBPROC h_glGenProgramsARB;
extern PFNGLBINDPROGRAMARBPROC h_glBindProgramARB;
extern PFNGLPROGRAMSTRINGARBPROC h_glProgramStringARB;
extern PFNGLGETPROGRAMIVARBPROC h_glGetProgramivARB;

extern	PFNGLCREATESHADEROBJECTARBPROC h_glCreateShaderObjectARB;
extern	PFNGLSHADERSOURCEARBPROC h_glShaderSourceARB;
extern	PFNGLCOMPILESHADERARBPROC h_glCompileShaderARB;
extern	PFNGLCREATEPROGRAMOBJECTARBPROC h_glCreateProgramObjectARB;
extern	PFNGLATTACHOBJECTARBPROC h_glAttachObjectARB;	
extern	PFNGLLINKPROGRAMARBPROC h_glLinkProgramARB;
extern	PFNGLUSEPROGRAMOBJECTARBPROC h_glUseProgramObjectARB;
extern	PFNGLGETINFOLOGARBPROC h_glGetInfoLogARB;
extern  PFNGLDELETEOBJECTARBPROC h_glDeleteObjectARB;
extern  PFNGLGETOBJECTPARAMETERIVARBPROC h_glGetObjectParameterivARB;

#else
extern void *h_glMultiTexCoord1fARB;
extern void *h_glMultiTexCoord2fARB;
extern void *h_glMultiTexCoord3fARB;
extern void *h_glMultiTexCoord4fARB;
extern void *h_glActiveTextureARB;
extern void *h_glClientActiveTextureARB;

extern void *h_glGenProgramsARB;
extern void *h_glBindProgramARB;
extern void *h_glProgramStringARB;
extern void *h_glGetProgramivARB;

extern	void *h_glCreateShaderObjectARB;
extern	void *h_glShaderSourceARB;
extern	void *h_glCompileShaderARB;
extern	void *h_glCreateProgramObjectARB;
extern	void *h_glAttachObjectARB;	
extern	void *h_glLinkProgramARB;
extern	void *h_glUseProgramObjectARB;
extern	void *h_glGetInfoLogARB;
extern  void *h_glDeleteObjectARB;
extern  void *h_glGetObjectParameterivARB;

#endif

#include <hel/math.h>
#include <mstl/Vector.h>

const float RED[]          = {  1.0,  0.0,  0.0, 1.0 };
const float GREEN[]        = {  0.0,  1.0,  0.0, 1.0 };
const float BLUE[]         = {  0.0,  0.0,  1.0, 1.0 };

const float DARK_RED[]     = {  0.75, 0.0,  0.0,  1.0 };
const float DARK_GREEN[]   = {  0.0,  0.75, 0.0,  1.0 };
const float DARK_BLUE[]    = {  0.0,  0.0,  0.75, 1.0 };

const float CYAN[]         = {  0.0,  1.0,  1.0,  1.0 };
const float PINK[]         = {  1.0,  0.75, 0.75, 1.0 };
const float ORANGE[]       = {  0.9,  0.7,  0.2,  1.0 };
const float YELLOW[]       = {  1.0,  1.0,  0.0,  1.0 };
const float DARK_YELLOW[]  = {  0.75, 0.75, 0.0,  1.0 };
const float BLACK[]        = {  0.0,  0.0,  0.0,  1.0 };
const float WHITE[]        = {  1.0,  1.0,  1.0,  1.0 };
const float NEXT_PURPLE[]  = {  0.3,  0.3,  0.5,  1.0 };
const float NEXT_PURPLE2[] = {  0.4,  0.4,  0.6,  1.0 };



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

void mglGetOpenGLModelviewMatrix(double *modelview); // double[16]

void mglGetOpenGLProjectionMatrix(double *projection); // double[16]

void mglApplyMaterial(uint32 materialIndex);


namespace freyja3d {


class OpenGLContext
{
public:
	
	
	void Init(uint32 width, uint32 height);

	void Resize(uint32 width, uint32 height);

	vec_t mAspectRatio;
	vec_t mNearHeight;
	vec_t mNear;
	vec_t mFar;
	vec_t mScaleEnv;
	uint32 mWidth;
	uint32 mHeight;
	bool mInitContext;
};


class OpenGL
{
	typedef enum {
		GREYSCALE = 1,
		INDEXED,
		RGB,
		RGBA,
		ARGB
	} ColorMode;

	typedef enum {
		fNone = 0,
		fUseMipmaps			= 1,
		fUseMultiTexture	= 2,
		fUseSDL_TTF			= 4
	} Flags;


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

 protected:

	OpenGL();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of OpenGL
	 *
	 ------------------------------------------------------*/

 public:

	static OpenGL *Instance();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of OpenGL if not already 
	 *        allocated
	 *
	 ------------------------------------------------------*/

	static const OpenGL *Singleton() { return mSingleton; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of OpenGL if not already 
	 *        allocated
	 *
	 ------------------------------------------------------*/

	~OpenGL();
	/*------------------------------------------------------
	 * Pre  : OpenGL object is allocated
	 * Post : Deconstructs an object of OpenGL
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// OpenGL
	////////////////////////////////////////////////////////////

	byte *GenerateColorTexture(byte rgba[4], uint32 width, uint32 height);
	/*------------------------------------------------------
	 * Pre  : <rgba> is 32bpp RGBA color
	 *        <width> and <height> are the dimensions of image
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.30:
	 * Mongoose - Created, taken from class Texture
	 ------------------------------------------------------*/

	// Need to 'update' this to be more OpenGL focused than generic now
	int LoadTexture(byte *image, uint32 width, uint32 height, 
					ColorMode mode, uint32 bpp);
	/*------------------------------------------------------
	 * Pre  : image must be a valid pixmap that agrees
	 *        with mode, width, and height
	 *
	 * Post : Returns texture id or < 0 error flag
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.05.29:
	 * Mongoose - Big code clean up, documentation
	 *
	 * 2000.10.05: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

   int loadBufferSlot(unsigned char *image, 
					  unsigned int width, unsigned int height, 
					  ColorMode mode, unsigned int bpp,
					  unsigned int slot);
	/*------------------------------------------------------
	 * Pre  : image must be a valid pixmap that agrees
	 *        with mode, width, and height, slot ( ID )
	 *
	 * Post : Returns texture id or < 0 error flag
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.09.05: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int loadColorTexture(unsigned char rgba[4],
						 unsigned int width, unsigned int height);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Generates and loads a solid color texture,
	 *        returns texture Id or -1 if failed
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.30:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void SetMaxTextureCount(uint32 max);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
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

	static void BindFragmentGLSL(int32 fragmentId);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : if <fragmentId> < 0 fragments are disabled.
	 *        Otherwise the referenced fragment is bound.
	 ------------------------------------------------------*/

	static void DebugFragmentGLSL(int32 fragmentId);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Dump the GLSL log for this object to a dialog.
	 *
	 ------------------------------------------------------*/

	static bool LoadFragmentGLSL(const char *filename, uint32 &fragmentId);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
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
	 * Post : 
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public attributes
	////////////////////////////////////////////////////////////

	/* Hardware/driver limits */
	int32 mTextureUnitCount;
	int32 mMaxLightsCount;

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

 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	static OpenGL *mSingleton;

	mstl::Vector<OpenGLContext> mContexts;

	uint32 mFlags;              /* Options */

	uint32 *mTextureIds;		/* GL texture list */

	uint32 mTextureCount;	    /* Texture counter */
	
	uint32 mTextureLimit;	    /* The texture limit */
	
	int32 mTextureId;	    	/* Currently bound texture id */
	
	int32 mTextureId2;			/* Multitexture Texture Id */
};

} // namespace freyja3d



#endif /* GUARD__FREYJA_MONGOOSE_FREYJAOPENGL_H_ */



