/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : FreyjaRender
 * License : No use w/o permission (C)2001 Mongoose
 * Comments: GL context rendering backend for Freyja
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2001.07.28:
 * Mongoose - Update documentation and prune obsoloete methods
 *
 * 2000.08.25:
 * Mongoose - Created, based on GooseEgg view prototype
 *
 ==========================================================================*/

#ifndef GUARD__MONGOOSE_FREYJA_FREYJARENDER_H_
#define GUARD__MONGOOSE_FREYJA_FREYJARENDER_H_


#include <hel/ViewVolume.h>
#include <hel/Ray.h>
#include <freyja/RenderModel.h>


enum view_mode {                   /* View|Edit mode */     
	VIEWMODE_MODEL_EDIT       = 1,
	VIEWMODE_TEXTURE_EDIT     = 2,
	VIEWMODE_MAP_VIEW         = 3,
	VIEWMODE_MODEL_VIEW       = 4,
	VIEWMODE_MATERIAL_EDIT    = 5
};

enum rotate_flags {
	X_F = 1,
	Y_F = 2,
	Z_F = 4
};

void getOpenGLViewport(int *viewportXYWH); // int[4]
void getOpenGLModelviewMatrix(double *modelview); // double[16]
void getOpenGLProjectionMatrix(double *projection); // double[16]
void FreyjaViewEventsAttach();


namespace freyja3d {

typedef enum { 
	
	PLANE_FRONT = 0, // XY
	PLANE_LEFT  = 1, // ZY
	PLANE_TOP   = 2, // XZ
	PLANE_FREE,
	PLANE_BACK, 
	PLANE_BOTTOM, 
	PLANE_RIGHT,

	DRAW_CAMERA,
	DRAW_UV,
	DRAW_MATERIAL,
	DRAW_CURVE
	
} freyja_plane_t;


class Viewport
{
public:
	Viewport()
	{
	}

	~Viewport() 
	{
	}

	long x, y, w, h;

	freyja_plane_t plane;

	long mode;
};


class FreyjaRender
{
public:

	typedef enum {                 /* Rendering modes */
		fWireframe        = 1,
		fTexture          = 2,
		fFace             = 4,
		fGrid             = 8,
		fLighting         = 16,    /* Render with GL lighting */
		fParticles        = 32,
		fBones            = 64,
		fMaterial         = 128,
		fPoints           = 256,   
		fDrawPickRay      = 512,   /* Render the pick ray */
		fBoundingVolumes  = 1024,  /* Render geometry bounding volumes */ 
		fNormals          = 2048,  /* Render vectors for normals */
		fViewports        = 4096, 
		fBoundingVolSelection      = 8192,
		fSkeletalVertexBlending    = 16384,
		fBonesNoZbuffer            = 32768,
		fRenderGridClearedZBuffer  = 65536,
		fReserved131072            = 131072,
		fReserved262144            = 262144,
		fReserved524288            = 524288
	} flags_t;


	FreyjaRender();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : FreyjaRender is constructed
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.02.02:
	 * Mongoose - No args
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~FreyjaRender();
	/*------------------------------------------------------
	 * Pre  : FreyjaRender exists
	 * Post : FreyjaRender is deconstructed
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	unsigned int GetFlags() { return mRenderMode; }
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Returns options flags for view
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.09: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void GetRotation(vec3_t v);
	/*------------------------------------------------------
	 * Pre  : 
	 *
	 * Post : Return present rotation angles in Euler 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	const vec_t &GetWindowAspectRatio() { return mAspectRatio; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get GL context window aspect ratio
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created, was public data member
	 ------------------------------------------------------*/

	unsigned int GetMode() { return mRenderMode; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	unsigned int GetViewMode() { return mRenderMode; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	const vec_t &getNearHeight() { return mScaleEnv; } 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get GL context window height
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	const unsigned int &GetWindowWidth() { return mWidth; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get GL context window width 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	const unsigned int &GetWindowHeight() { return mHeight; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get GL context window height
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	const vec_t &GetZoom() { return mZoom; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get current zoom factor for the scene
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void ClearFlag(flags_t flag);
	/*------------------------------------------------------
	 * Pre  : Flag is valid
	 * Post : Clears control flag for model
	 *
	 ------------------------------------------------------*/

	void Display();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renders a frame to GL context
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void InitContext(uint32 width, uint32 height, bool fastCard);
	/*------------------------------------------------------
	 * Pre  : <width> and <height> are the GL context dimensions 
	 *        <fastCard> is true if GL hw accel is present
	 *
	 * Post : Sets up GL parms for GL context
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.02.02:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void ResizeContext(unsigned int width, unsigned int height);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Resizes GL context
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void SetFlag(flags_t flag);
	/*------------------------------------------------------
	 * Pre  : Flag is valid
	 * Post : Sets control flag for model
	 *
	 ------------------------------------------------------*/

	void SetNearHeight(vec_t scale) 
	{ mScaleEnv = scale; ResizeContext(mWidth, mHeight); } 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void SetViewMode(int mode);
	/*------------------------------------------------------
	 * Pre  : Mode are valid view_mode(s)
	 * Post : Sets viewing options
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void SetViewportWindow(long idx, freyja_plane_t win)
	{ if (idx < mViewportsCount) mViewports[idx].plane = win; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void SetZoom(vec_t zoom) { mZoom = zoom; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set current zoom factor for the scene
	 *
	 ------------------------------------------------------*/

	// FIXME to be replaced by Arcball most likely
	void Rotate(int flags, float n);
	/*------------------------------------------------------
	 * Pre  : Flags are valid XYZ flags
	 * Post : Rotate on axis n degrees
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	static vec4_t mColorBackground;
	static vec4_t mColorGridLine;
	static vec4_t mColorVertex;
	static vec4_t mColorVertexHighlight;
	static vec4_t mColorWireframeHighlight;
	static vec4_t mColorWireframe;
	static vec4_t mColorBoneHighlight;
	static vec4_t mColorBone;
	static vec4_t mColorText;
	static vec4_t mColorBoundingBox;
	static vec4_t mColorGridSeperator;
	static vec4_t mColorAxisX;
	static vec4_t mColorAxisY;
	static vec4_t mColorAxisZ;
	static unsigned int mSelectedBone;
	static unsigned int mBoneRenderType;
	static unsigned char mJointRenderType;
	static int mPatchDisplayList;

	static vec_t mBoneLineWidth;                    /* Custom artifact size */
	static vec_t mBonePointSize;
	static vec_t mDefaultPointSize;
	static vec_t mDefaultLineWidth;
	static vec_t mVertexPointSize;

	static FreyjaRender *mSingleton;  // Not really, but will be later

	static Ray mTestRay;

	uint32 mViewportsCount;                    /* How many viewports are used */

	Viewport mViewports[4];                    /* Viewports information */


private:    

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	void RenderLights();
	/*------------------------------------------------------
	 * Pre  : Called from viewport interface renderer
	 * Post : Renders light symbol and does lighting setup
	 *
	 ------------------------------------------------------*/

	void Render(RenderMesh &mesh);
	/*------------------------------------------------------
	 * Pre  : Called from proper method
	 * Post : Renders mesh
	 *
	 ------------------------------------------------------*/

	void Render(RenderModel &model);
	/*------------------------------------------------------
	 * Pre  : Called from proper method
	 * Post : Renders model
	 *
	 ------------------------------------------------------*/

	void Render(RenderSkeleton &skeleton, uint32 currentBone, vec_t scale);
	/*------------------------------------------------------
	 * Pre  : Called from proper method
	 * Post : Renders skeleton
	 *
	 ------------------------------------------------------*/

	void DrawQuad(float x, float y, float w, float h);
	/*------------------------------------------------------
	 * Pre  : X, Y, W, H define a 2d quad
	 * Post : Draws a quad orthogonal to GL context
	 *        as a solid or with current texture
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void DrawGrid(freyja_plane_t plane, int w, int h, int size);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renders a grid orthogal to gl context
	 *        WxH at size spacing and adjusts for scroll
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void DrawCurveWindow();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renders curve editing interface in viewport
	 *
	 ------------------------------------------------------*/

	void DrawFreeWindow();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renders oribital model editor interface in viewport
	 *
	 ------------------------------------------------------*/

	void DrawMaterialEditWindow();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renders material editing interface in viewport
	 *
	 ------------------------------------------------------*/

	void DrawUVWindow();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renders UV editing interface in viewport
	 *
	 ------------------------------------------------------*/

	void DrawWindow(freyja_plane_t plane);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renders given editor interface in viewport
	 *
	 ------------------------------------------------------*/

	void Rotate(float n, int axis);
	/*------------------------------------------------------
	 * Pre  : Axis is 0 = x, 1 = y, 2 = z
	 * Post : Rotate on axis n degrees
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void BindTexture(unsigned int texture);
	/*------------------------------------------------------
	 * Pre  : texture is valid texture id
	 * Post : Binds GL texture 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.02.28:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	uint32 mViewMode;                          /* View mode */

	uint32 mRenderMode;                        /* Rendering mode */

	uint32 mWidth;                             /* Width of context */

	uint32 mHeight;                            /* Height of context */

	vec_t mAspectRatio;                        /* Cached context aspect ratio */

	vec_t mZoom;                               /* Used to cache zoom */

	uint32 mTextureId;                         /* Currently bound texture */

	bool mInitContext;                         /* OpenGL context started? */

	vec3_t mScroll;                            /* Used to cache scroll */

	vec3_t mAngles;                            /* Used to rotate the scene */

	vec_t mScaleEnv;                           /* OpenGL context use */
	vec_t mFar;
	vec_t mNear;
	vec_t mFovY;
	vec_t mNearHeight;

	bool mScrollingUV_X;
};

} /* End namespace freyja3d */

#endif
