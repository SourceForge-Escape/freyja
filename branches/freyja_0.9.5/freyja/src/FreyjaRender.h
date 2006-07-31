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

#include "FreyjaModel.h"


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
void FreyjaRenderEventsAttach();



class FreyjaRender
{
public:

	typedef enum {                 /* Rendering modes */
		RENDER_WIREFRAME        = 1,
		RENDER_TEXTURE          = 2,
		RENDER_FACE             = 4,
		RENDER_EDIT_GRID        = 8,
		RENDER_LIGHTING         = 16,         /* Render with GL lighting */
		RENDER_PARTICLES        = 32,
		RENDER_BONES            = 64,
		RENDER_MATERIAL         = 128,
		RENDER_CAMERA           = 256,        /* Enable panning  */
		RENDER_BBOX             = 512,        /* Rendering bounding box */
		RENDER_NORMAL           = 1024, 
		RENDER_NORMALS          = 2048, 
		fHightlightPolygonWireframe = 4096, 
		RENDER_BBOX_SEL         = 8192,
		fSkeletalVertexBlending     = 16384,
		RENDER_POINTS           = 32768,
		fRenderBonesClearedZBuffer = 65536,
		fViewports              = 131072,
		fRenderGridClearedZBuffer = 262144
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

	unsigned int getFlags();
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Returns control flags for model
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.09: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void getRotation(vec3_t v);
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

	vec_t getWindowAspectRatio();
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

	vec_t getNearHeight() { return mScaleEnv; } 

	unsigned int getWindowWidth();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get GL context window width 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int getWindowHeight();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get GL context window height
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	float getZoom();
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

	void clearFlag(flags_t flag);
	/*------------------------------------------------------
	 * Pre  : Flag is valid
	 * Post : Clears control flag for model
	 *
	 ------------------------------------------------------*/

	void display();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renders a frame to GL context
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void initContext(unsigned int width, unsigned int height, bool fast_card);
	/*------------------------------------------------------
	 * Pre  : Width and Height are the GL context dim 
    *        Fast_card is true if GL hw accel is present
	 *
	 * Post : Sets up GL parms for GL context
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.02.02:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void resizeContext(unsigned int width, unsigned int height);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Resizes GL context
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setFlag(flags_t flag);
	/*------------------------------------------------------
	 * Pre  : Flag is valid
	 * Post : Sets control flag for model
	 *
	 ------------------------------------------------------*/

	void setNearHeight(vec_t scale) { mScaleEnv = scale; resizeContext(mWidth, mHeight); } 

	void setSceneData(FreyjaModel *model);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Hook into data model for the scene
	 *
	 ------------------------------------------------------*/

	void setViewMode(int mode);
	/*------------------------------------------------------
	 * Pre  : Mode are valid view_mode(s)
	 * Post : Sets viewing options
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setZoom(float zoom);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set current zoom factor for the scene
	 *
	 ------------------------------------------------------*/

	void toggleFlag(flags_t flag);
	/*------------------------------------------------------
	 * Pre  : Flag is valid
	 * Post : Toggles control flag for model
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

private:    

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	void renderBox(vec3_t min, vec3_t max);
	/*------------------------------------------------------
	 * Pre  : Group exists
	 * Post : Renders bounding box in default color, etc
	 *
	 ------------------------------------------------------*/

	void renderLights();
	/*------------------------------------------------------
	 * Pre  : Called from proper method
	 * Post : Renders light symbol and does lighting setup
	 *
	 ------------------------------------------------------*/

	void renderMesh(RenderMesh &mesh);
	/*------------------------------------------------------
	 * Pre  : Called from proper method
	 * Post : Renders mesh
	 *
	 ------------------------------------------------------*/

	void renderModel(RenderModel &model);
	/*------------------------------------------------------
	 * Pre  : Called from proper method
	 * Post : Renders model
	 *
	 ------------------------------------------------------*/

	void renderPolygon(RenderPolygon &face);
	/*------------------------------------------------------
	 * Pre  : Called from proper method
	 * Post : Renders polygon
	 *
	 ------------------------------------------------------*/

	void renderSkeleton(RenderSkeleton &skeleton, unsigned int currentBone,
							  vec_t scale);
	/*------------------------------------------------------
	 * Pre  : Called from proper method
	 * Post : Renders skeleton
	 *
	 ------------------------------------------------------*/

	void renderUVWindow();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renders texel editing quad
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/



	///////// Refactoring below here still...

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

	void drawFreeWindow();
	void drawWindow(freyja_plane_t plane);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renders 3d model/particles/etc editor in viewports
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void DrawMaterialEditWindow();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renders material editing sphere
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.24:
	 * Mongoose - Created
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

	FreyjaModel *mModel;                       /* Scene data model */

	unsigned int mWidth;                       /* Width of context */

	unsigned int mHeight;                      /* Height of context */

	vec_t mAspectRatio;                        /* Cached context aspect ratio */

	vec4_t mViewports[4];                      /* Size and offset of viewports
															  * if enabled */
	
	static unsigned int mRenderMode;                  /* Rendering mode */

	float mZoom;                               /* Used to cache zoom */

	unsigned int mTextureId;                   /* Currently bound texture */

	bool mInitContext;                         /* OpenGL context started? */

	int mViewMode;                             /* View mode */

	float mScroll[3];                          /* Used to cache scroll */

	float mAngles[3];                          /* Used to rotate the scene */

	float mScaleEnv;                           /* OpenGL context use */
	float mFar;	// zFar
	float mNear;	// zNear
	float mFovY;
	float mNearHeight;
};

#endif
