/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (C)2001 Mongoose
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


#ifndef GUARD__FREYJA_MONGOOSE_FREYJAOPENGL_H_
#define GUARD__FREYJA_MONGOOSE_FREYJAOPENGL_H_

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

#   ifdef OPENGL_EXT_MULTITEXTURE
extern PFNGLMULTITEXCOORD1FARBPROC glMultiTexCoord1fARB;
extern PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB;
extern PFNGLMULTITEXCOORD3FARBPROC glMultiTexCoord3fARB;
extern PFNGLMULTITEXCOORD4FARBPROC glMultiTexCoord4fARB;
extern PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
extern PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB;
#   endif

#else
#   error "This module requires an OpenGL SDK"
#endif

#include <hel/math.h>

#include <mstl/Vector.h>

using namespace mstl;

const float RED[]          = {  1.0,  0.0,  0.0, 1.0 };
const float GREEN[]        = {  0.0,  1.0,  0.0, 1.0 };
const float BLUE[]         = {  0.0,  0.0,  1.0, 1.0 };
const float CYAN[]         = {  0.0,  1.0,  1.0, 1.0 };
const float PINK[]         = {  1.0,  0.75,  0.75, 1.0 };
const float ORANGE[]       = {  1.0,  7.0,  0.0, 1.0 };
const float YELLOW[]       = {  1.0,  1.0,  0.0, 1.0 };
const float DARK_YELLOW[]  = {  0.75, 0.75,  0.0, 1.0 };
const float BLACK[]        = {  0.0,  0.0,  0.0, 1.0 };
const float WHITE[]        = {  1.0,  1.0,  1.0, 1.0 };
const float NEXT_PURPLE[]  = {  0.3,  0.3,  0.5, 1.0 };
const float NEXT_PURPLE2[] = {  0.4,  0.4,  0.6, 1.0 };



////////////////////////////////////////////////////////////
// Gobal functions
////////////////////////////////////////////////////////////

bool mglHardwareExtTest(const char *ext);

void mgl2dProjection(const unsigned int width, const unsigned int height);

void mglDrawBbox(const vec3_t min, const vec3_t max, 
				 const vec4_t pointColor, const vec4_t lineColor);

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
 public:

	enum Flags {
		fNone = 0
	};


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	~OpenGL();
	/*------------------------------------------------------
	 * Pre  : OpenGL object is allocated
	 * Post : Deconstructs an object of OpenGL
	 *
	 *-- History ------------------------------------------
	 *
	 * 2006.07.22: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	static OpenGL *Instance();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of OpenGL if not already 
	 *        allocated
	 *
	 *-- History ------------------------------------------
	 *
	 * 2006.07.22: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	static OpenGL *mSingleton;

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

 protected:

	OpenGL();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of OpenGL
	 *
	 *-- History ------------------------------------------
	 *
	 * 2006.07.22: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	Vector<OpenGLContext> mContexts;

	uint32 mFlags;             /* */
};

} // namespace freyja3d



#endif /* GUARD__FREYJA_MONGOOSE_FREYJAOPENGL_H_ */



