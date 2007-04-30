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
 *           template generator scripopt.  <mongoose@icculus.org>
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

#define USING_FREYJA_CPP_ABI

#include "FreyjaOpenGL.h" // Includes windows.h, so avoid header interaction

#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>

#include <freyja/LightABI.h>
#include <freyja/SkeletonABI.h>
#include <freyja/Bone.h>
#include <freyja/Mesh.h>
#include <freyja/MeshABI.h>
#include <freyja/MaterialABI.h>

#include "freyja_events.h"
#include "FreyjaControl.h"

#include "FreyjaRender.h"


using namespace freyja;
using namespace freyja3d;

FreyjaRender *FreyjaRender::mInstance = 0x0;

hel::Ray FreyjaRender::mTestRay;
OpenGLPrinter FreyjaRender::mPrinter;

vec4_t FreyjaRender::mColors[16];
vec4_t FreyjaRender::mColorBackground;
vec4_t FreyjaRender::mColorText;
vec4_t FreyjaRender::mColorWireframe;
vec4_t FreyjaRender::mColorWireframeHighlight;
vec4_t FreyjaRender::mColorBone;
vec4_t FreyjaRender::mColorBoneHighlight;
vec4_t FreyjaRender::mColorBoundingBox;
vec4_t FreyjaRender::mColorGridSeperator;
vec4_t FreyjaRender::mColorGridLine;
vec4_t FreyjaRender::mColorVertex;
vec4_t FreyjaRender::mColorVertexHighlight;
vec4_t FreyjaRender::mColorAxisX;
vec4_t FreyjaRender::mColorAxisY;
vec4_t FreyjaRender::mColorAxisZ;
//unsigned int  FreyjaRender::mRenderMode = 0;
unsigned int FreyjaRender::mSelectedBone = 0;
unsigned int FreyjaRender::mBoneRenderType = 1;
unsigned char FreyjaRender::mJointRenderType = 1;
int FreyjaRender::mPatchDisplayList = -1;

vec_t FreyjaRender::mBoneLineWidth = 3.0;        /* Custom artifact size */
vec_t FreyjaRender::mBonePointSize = 5.0;
vec_t FreyjaRender::mDefaultPointSize = 3.5;
vec_t FreyjaRender::mDefaultLineWidth = 1.0;
vec_t FreyjaRender::mVertexPointSize = 5.0; // 3.5;

matrix_t gModelViewMatrix;


FreyjaRender::FreyjaRender() :
	mTimer(),
	mViewMode(VIEWMODE_MODEL_VIEW),
	mRenderMode(fBoundingVolSelection | 
				fBonesNoZbuffer | 
				fBoundingVolumes |
				fFPSCap |
				fGroupColors),
	mWidth(640),
	mHeight(480),
	mTextureId(0),
	mInitContext(false),
	mScaleEnv(35.0f), // 40.0f is about too much, Use a larger number for higher res
	mFar(6000.0f),
	mNear(0.1f),
	mFovY(40.0f),
	mNearHeight(20.0f)
{
	mAngles[0] = 18.0f;
	mAngles[1] = 42.0f;
	mAngles[2] = 0.0f;

	for (long i = 0; i < 3; ++i)
	{
		mColorAxisX[i] = RED[i] * 0.75f;
		mColorAxisY[i] = GREEN[i] * 0.75f;
		mColorAxisZ[i] = BLUE[i] * 0.75f;
	}

	mViewports[0].plane = PLANE_FRONT;
	mViewports[1].plane = PLANE_LEFT;
	mViewports[2].plane = PLANE_TOP;
	mViewports[3].plane = PLANE_FREE;

	// To avoid branching by wasting a little memory 
	for (uint32 i = 0; i < 3; ++i)
	{
		mColors[ 0][i] = WHITE[i];
		mColors[ 1][i] = NEXT_PURPLE2[i];
		mColors[ 2][i] = PINK[i];
		mColors[ 3][i] = GREEN[i];
		mColors[ 4][i] = YELLOW[i];
		mColors[ 5][i] = CYAN[i];
		mColors[ 6][i] = DARK_YELLOW[i];
		mColors[ 7][i] = DARK_BLUE[i];
		mColors[ 8][i] = ORANGE[i];  // FIXME, needs more colors
		mColors[ 9][i] = YELLOW[i]*0.25;
		mColors[10][i] = YELLOW[i]*0.50;
		mColors[11][i] = GREEN[i]*0.25;
		mColors[12][i] = GREEN[i]*0.50;
		mColors[13][i] = YELLOW[i]*0.75;
		mColors[14][i] = GREEN[i]*0.75;
		mColors[15][i] = PINK[i]*0.50;
	}
}


FreyjaRender::~FreyjaRender()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

void FreyjaRender::GetRotation(vec3_t v)
{
	v[0] = mAngles[0];
	v[1] = mAngles[1];
	v[2] = mAngles[2];
}


/* Mongoose 2004.03.26, 
 * You have to be kidding me 
 * Replace with quarternion/matrix and change API  */
void FreyjaRender::Rotate(float n, int axis)
{
	mAngles[axis] += n;

	if (mAngles[axis] >= 360.0)
	{
		mAngles[axis] = 0.0;
	}
	else if (mAngles[axis] < 0.0)
	{
		mAngles[axis] = 360.0;
	}
}

void FreyjaRender::Rotate(int flags, float n)
{
	if (flags & X_F)
		Rotate(n, 0);

	if (flags & Y_F)
		Rotate(n, 1);

	if (flags & Z_F)
		Rotate(n, 2);
}


void FreyjaRender::DrawFreeWindow()
{
	glPushMatrix();

	// TODO: Replace with Mat44 transform
	glTranslatef(mScroll[0], mScroll[1]/*+8.0f*/, mScroll[2]);
	glRotatef(mAngles[0], 1.0, 0.0, 0.0);
	glRotatef(mAngles[1], 0.0, 1.0, 0.0);
	glRotatef(mAngles[2], 0.0, 0.0, 1.0);

	if (mRenderMode & fSolidPlane)
	{
		glPushAttrib(GL_ENABLE_BIT);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glColor3fv(WHITE);
		mglApplyMaterial(FreyjaControl::mInstance->GetSelectedMaterial());
		mglDrawPlane(50.0f, 2.0f, 1.0f);
		glPopAttrib();
	}
	else if (mRenderMode & fGrid)
	{
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

		glLineWidth(1.25f);
		mglDrawGrid(mColorGridLine, 50.0f, 2.0f, 1.0f);

		glColor3fv(mColorGridLine);
		glLineWidth(4.0f);
		glBegin(GL_LINES);
		//glColor3fv(RED);
		glVertex3f(-50.0f, 0.0f, 0.0f);	
		glVertex3f(50.0f, 0.0f, 0.0f);
		//glColor3fv(BLUE);
		glVertex3f(0.0f, 0.0f, -50.0f);	
		glVertex3f(0.0f, 0.0f, 50.0f);
		glEnd();

#if 1
		//glColor3fv(mColorGridLine);
		glLineWidth(1.75f);
		glBegin(GL_LINES);

		for (float x = -50.0f; x < 60.0f; x += 10.f)
		{
			if (x < 10.0f && x > -10.0f) 
				continue;

			glVertex3f(x, 0.0f, -50.0f);	
			glVertex3f(x, 0.0f, 50.0f);
			glVertex3f(-50.0f, 0.0f, x);	
			glVertex3f(50.0f, 0.0f, x);
		}

		glEnd();
#endif

		glPopAttrib();
	}


	//glPushMatrix();

	RenderLights();

	glScalef(mZoom, mZoom, mZoom);

	mglGetOpenGLModelviewMatrix16fv(gModelViewMatrix);

	if (mRenderMode & fDrawPickRay)
	{
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

		hel::Vec3 rayEnd = mTestRay.mOrigin + mTestRay.mDir * 1000;
		glPointSize(2.0);
		glBegin(GL_POINTS);	
		glColor3fv(GREEN);	
		glVertex3fv(mTestRay.mOrigin.mVec);
		glVertex3fv(rayEnd.mVec);
		glEnd();
		glBegin(GL_LINES);	
		glColor3fv(YELLOW);	
		glVertex3fv(mTestRay.mOrigin.mVec);
		glColor3fv(DARK_YELLOW);	
		glVertex3fv(rayEnd.mVec);
		glEnd();
		glPointSize(mDefaultPointSize);

		glPopAttrib();
	}

	for (uint32 i = 0; i < freyjaGetModelCount(); ++i)
	{
		RenderModel(i);
	}

	//glPopMatrix(); // Remove scaling

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	BindColorTexture();
	FreyjaControl::mInstance->GetCursor().Display();
	glPopAttrib();

	glPopMatrix();


#ifdef PLANE_NOTIFY_WITH_AXIS
	glPushMatrix();
	glTranslatef(-mScaleEnv, -mScaleEnv + 2.5f, 10.0);
	glRotatef(mAngles[0], 1.0, 0.0, 0.0);
	glRotatef(mAngles[1], 0.0, 1.0, 0.0);
	glRotatef(mAngles[2], 0.0, 0.0, 1.0);

	glClear(GL_DEPTH_BUFFER_BIT);
	mglDrawEditorAxis();
	glPopMatrix();
#endif

	// OpenGLPrinter test
	glPushAttrib(GL_ENABLE_BIT);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glColor3fv(WHITE);
	mPrinter.Print2d(-mScaleEnv - 1.0f, mScaleEnv - 1.5f, 0.06f, "ORBIT");
	//glColor3fv(GREEN);
	//mPrinter.Print2d(15.0f, mScaleEnv - 1.5f, 0.04f, 
	//				 "THIS IS A PREALPHA RELEASE.");
	glPopAttrib();
}


void FreyjaRender::AttachMethodListeners()
{
	CreateListener("eViewports", &FreyjaRender::eViewports);
	CreateListener("eRenderBoneName", &FreyjaRender::eRenderBoneName);
	CreateListener("eRenderSkeleton", &FreyjaRender::eRenderSkeleton);
	CreateListener("eRenderSkeleton2", &FreyjaRender::eRenderSkeleton2);
	CreateListener("eRenderVertex", &FreyjaRender::eRenderVertex);
	CreateListener("eRenderWireframe", &FreyjaRender::eRenderWireframe);
	CreateListener("eRenderNormals", &FreyjaRender::eRenderNormals);
	CreateListener("eRenderLighting", &FreyjaRender::eRenderLighting);
	CreateListener("eRenderMaterial", &FreyjaRender::eRenderMaterial);
	CreateListener("eRenderGrid", &FreyjaRender::eRenderGrid);
	CreateListener("eRenderSolidGround", &FreyjaRender::eRenderSolidGround);
	CreateListener("eRenderFace", &FreyjaRender::eRenderFace);
	CreateListener("eRenderPickRay", &FreyjaRender::eRenderPickRay);
	CreateListener("eRenderBbox", &FreyjaRender::eRenderBbox);
	CreateListener("eGroupColors", &FreyjaRender::eGroupColors);
	CreateListener("eRenderBoneZClear", &FreyjaRender::eRenderBoneZClear);
	CreateListener("eRenderGridZClear", &FreyjaRender::eRenderGridZClear);
	CreateListener("eSkeletalDeform", &FreyjaRender::eRenderSkeletalDeform);
	CreateListener("eFPSCap", &FreyjaRender::eFPSCap);
	CreateListener("ePolyMeshBone", &FreyjaRender::ePolyMeshBone);
	CreateListener("eLineBone", &FreyjaRender::eLineBone);

	//CreateListener("", &FreyjaRender::);
}

////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////

void FreyjaRender::BindColorTexture()
{
	if (0 != mTextureId)
	{
		mTextureId = 1024; // FIXME: -1
		// First texture is reserved for 'white texture' for color
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}


void FreyjaRender::BindTexture(uint32 texture)
{
	if (texture != mTextureId)
	{
		mTextureId = texture;
		// First texture is reserved for 'white texture' for color
		glBindTexture(GL_TEXTURE_2D, mTextureId + 1);
	}
}


void FreyjaRender::DrawQuad(float x, float y, float w, float h)
{
	if (!(mRenderMode & fMaterial))
	{
		//glPushAttrib(GL_ENABLE_BIT);
		//glDisable(GL_TEXTURE_2D);

		glColor3fv(WHITE);
		BindColorTexture();

		glBegin(GL_QUADS);
		glColor3f(0.0f, 1.0f, 0.5f);
		glVertex2f(x, y);
		glColor3f(0.0f, 0.0f, 0.5f);
		glVertex2f(x, y + h);
		glColor3f(1.0f, 0.0f, 0.5f);
		glVertex2f(x + w, y + h);
		glColor3f(1.0f, 1.0f, 0.5f);
		glVertex2f(x + w, y);
		glEnd();

		//glPopAttrib();
	}
	else  // If fTexture draw textured quad
	{
		glPushAttrib(GL_ENABLE_BIT);
		glEnable(GL_TEXTURE_2D);
		glColor3fv(WHITE);
		mglApplyMaterial(FreyjaControl::mInstance->GetSelectedMaterial());

		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(x, y);		
		glTexCoord2f(0.0, 0.0);
		glVertex2f(x, y+h);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(x+w, y+h);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(x+w, y);
		glEnd();

		glPopAttrib();
	}
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void FreyjaRender::ClearFlag(flags_t flag)
{
	mRenderMode |= flag;
	mRenderMode ^= flag;
}


void FreyjaRender::InitContext(uint32 width, uint32 height, bool fastCard)
{
	OpenGL::Instance();

	// Due to whacky bullshit we're seriously going to have to use
	// GL_LESS depth and GL_FRONT culling or the interface will break

	// NOTE: The 'whacky bullshit' is due to various windings and
	// 'native' formats are allowed, so until that changes don't change this

	// Set up Z buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//glDepthFunc(GL_LEQUAL);

	// Set up culling
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);

	// Set background to black
	glClearColor(BLACK[0], BLACK[1], BLACK[2], BLACK[3]);

	// Setup shading
	glShadeModel(GL_SMOOTH);

	if (fastCard) 
	{
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glHint(GL_FOG_HINT, GL_NICEST);
		glEnable(GL_DITHER);
		
		// AA polygon edges
		//glEnable(GL_POLYGON_SMOOTH);
		//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	}
	else
	{
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
		glHint(GL_FOG_HINT, GL_FASTEST);
		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_DITHER);
		glDisable(GL_POLYGON_SMOOTH);
	}

	// Setup some general states
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_POINT_SMOOTH);
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_AUTO_NORMAL);
	glDisable(GL_LOGIC_OP);
	glDisable(GL_TEXTURE_1D);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_FOG);
	glDisable(GL_NORMALIZE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_EDGE_FLAG_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glPolygonMode(GL_FRONT, GL_FILL);
	glMatrixMode(GL_MODELVIEW);

	SetNearHeight(mScaleEnv);

	mWidth = width;
	mHeight = height;
	mInitContext = true;
}


#if 0
void FreyjaRender::DisplayByPolling()
{
	if (GetMode() & fFPSCap)
	{
		static unsigned int frames = 0;
		static mstl::Timer timer;
		
		float fps = (float)frames/((float)timer.GetTicks() / 1000.0f);
		
		freyja_print("! %f fps, %i ticks", fps, timer.GetTicks());
		
		if (fps > 60.0f)
			return;
		else
			++frames;
	}

	Display();
}
#endif


void FreyjaRender::Display() 
{ 
	if (!mInitContext ||
		(GetMode() & fFPSCap) && mTimer.GetTicks() < 16) // ~60.0 fps cap
	{
		//freyja_print("%ims since last frame", mTimer.GetTicks());
		return;
	}

	// Mongoose 2002.02.02, Cache for use in calls from here
	hel::Vec3 v = FreyjaControl::mInstance->GetSceneTranslation();
	helCopyVec3(v.mVec, mScroll);
	
	glClearColor(mColorBackground[0], mColorBackground[1], mColorBackground[2], 
				 1.0);
	
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Disable lighting and texture here until the color visualizations
	// are updated
	//glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glLoadIdentity();
	
	switch (mViewMode)
	{
	case VIEWMODE_MODEL_VIEW:
		glPushMatrix();
		DrawFreeWindow();
		glPopMatrix();
		break;

	case VIEWMODE_MODEL_EDIT:
		if (mRenderMode & fViewports)
		{
			// Get viewport size info
			GLint vp[4];
			glGetIntegerv(GL_VIEWPORT, vp);    
			long width = vp[2] / 2;
			long height = vp[3];

			// Testing default viewports 0-3, these should move
			// to resize event later
			mViewportsCount = 4;

			// 0 ( Lower left here )
			mViewports[0].x = 0;
			mViewports[0].y = 0;
			mViewports[0].w = width;
			mViewports[0].h = height/2;
			//mViewports[0].plane = PLANE_FRONT;

			// 1 ( Lower right here )
			mViewports[1].x = width;
			mViewports[1].y = 0;
			mViewports[1].w = width;
			mViewports[1].h = height/2;
			//mViewports[1].plane = PLANE_LEFT;

			// 2 ( Upper right here )
			mViewports[2].x = width;
			mViewports[2].y = height/2;
			mViewports[2].w = width;
			mViewports[2].h = height/2;
			//mViewports[2].plane = PLANE_TOP;

			// 3 ( Upper left here )
			mViewports[3].x = 0;
			mViewports[3].y = height/2;
			mViewports[3].w = width;
			mViewports[3].h = height/2;
			//mViewports[3].plane = PLANE_FREE;

			for (uint32 i = 0; i < mViewportsCount; ++i)
			{
				glPushAttrib(GL_ENABLE_BIT);
				glDisable(GL_TEXTURE_2D);
				glDisable(GL_LIGHTING);
				glDisable(GL_BLEND);

				glColor3fv(BLACK);
				glBegin(GL_LINES);
				glVertex2f(mViewports[i].x, mViewports[i].y);
				glVertex2f(mViewports[i].x, mViewports[i].h);
				glVertex2f(mViewports[i].x, mViewports[i].y);
				glVertex2f(mViewports[i].w, mViewports[i].y);
				glEnd();

				glPopAttrib();

				glViewport(mViewports[i].x, mViewports[i].y, 
						   mViewports[i].w, mViewports[i].h);
				// Removed extra GL matrix stack frame around this call
				DrawWindow(mViewports[i].plane);

				// Clear depth after each viewport
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glClear(GL_DEPTH_BUFFER_BIT);


			}

			glViewport(vp[0], vp[1], vp[2], vp[3]);
		}
		else
		{
			glPushMatrix();
			DrawWindow(FreyjaControl::mInstance->GetSelectedView());
			glPopMatrix();
		}
		break;

	case VIEWMODE_TEXTURE_EDIT:
		DrawUVWindow();
		break;

	case VIEWMODE_MATERIAL_EDIT:
		DrawMaterialEditWindow();
		break;

	default:
		;
	}

	// OpenGLPrinter test
	glPushAttrib(GL_ENABLE_BIT);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glColor3fv(GREEN);
	mPrinter.Print2d(15.0f, mScaleEnv - 1.5f, 0.04f, 
					 "THIS IS A PREALPHA RELEASE.");
	glPopAttrib();

	glFlush();
	freyja_swap_buffers();

	mTimer.Reset();
}


void FreyjaRender::ResizeContext(uint32 width, uint32 height) 
{
	if (!width || !height)
	{
		return;
	}

	mWidth = width;
	mHeight = height;
	mAspectRatio = (float)width / (float)height;

	glViewport(0, 0, width, height); 
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 

	glOrtho(-mScaleEnv * mAspectRatio,
			mScaleEnv * mAspectRatio, 
			-mScaleEnv, mScaleEnv, 
			-400.0, // zNear
			400.0);

	glMatrixMode(GL_MODELVIEW);
}


// FIXME: This is obsolete now, since viewports now walk the earth
void CameraResizeContext(uint32 width, uint32 height) 
{
#if 0
	if (!width || !height)
	{
		return;
	}

	mWidth = width;
	mHeight = height;
	mAspectRatio = (float)width / (float)height;

	glViewport(0, 0, width, height); 
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 

	// Old method used gluPerspective()
	//gluPerspective(mFovY, mAspectRatio, mNear * 100, mFar * 100);

	mNearHeight = 10.0f;
	mNear = 10.0f;
	mFar = 1000.0f;
	
	glFrustum( -mNearHeight * mAspectRatio, 
			   mNearHeight * mAspectRatio,
			   -mNearHeight, mNearHeight, 
			   mNear,
			   mFar );

	glMatrixMode(GL_MODELVIEW);
#endif
}


void FreyjaRender::SetFlag(flags_t flag)
{
	mRenderMode |= flag;
}


void FreyjaRender::SetViewMode(int mode)
{
	mViewMode = mode;
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////

void FreyjaRender::RenderLights()
{
	vec4_t pos;

	if (mRenderMode & fLighting)
	{
		freyjaGetLightPosition4v(0, pos);
		
		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2]);
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
		mglDrawSphere(16, 16, 0.75f);
		glPopAttrib();
		glPopMatrix();
		
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_POSITION, pos);

		vec4_t color;
		freyjaGetLightAmbient(freyjaGetCurrentLight(), color);
		glLightfv(GL_LIGHT0, GL_AMBIENT, color);
		freyjaLightDiffuse(freyjaGetCurrentLight(), color);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
		freyjaGetLightSpecular(freyjaGetCurrentLight(), color);
		glLightfv(GL_LIGHT0, GL_SPECULAR, color);
	}
}


// This sure has a lot of branching...
void FreyjaRender::RenderMesh(index_t mesh)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (!m)
		return;

	glPushMatrix();

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glPointSize(mVertexPointSize);

	// 'World effects'


	// NOTE: Right here if you swap out the vertex array 'array', then
	// all the elements to be rendered will use that instead of the 
	// the current mesh vertex array.  Good for animation, skinning, etc.
	vec_t *array = m->GetVertexArray();
	vec_t *normalArray = m->GetNormalArray();
	vec_t *texcoordArray = m->GetTexCoordArray();

	// Keyframe animation
	if (mRenderMode & fKeyFrameAnimation)
	{
		uint32 a = FreyjaControl::mInstance->GetSelectedAnimation();
		uint32 k = FreyjaControl::mInstance->GetSelectedKeyFrame();	

		// Mesh animation
		TransformTrack &tt = m->GetTransformTrack(a);
		vec_t time = (vec_t)k / tt.GetRate();
		hel::Vec3 pos, rot, scale;
		tt.GetTransform(time, pos, rot, scale);

		glTranslatef(pos.mVec[0], pos.mVec[1], pos.mVec[2]);	
		glRotatef(rot.mVec[0], 1,0,0);
		glRotatef(rot.mVec[1], 0,1,0);
		glRotatef(rot.mVec[2], 0,0,1);
		glScalef(scale.mVec[0], scale.mVec[1], scale.mVec[2]);

		// FIXME: You'll have to store a cache for this array in anim/track
		//        really.  You have to be dynamic in an editor  =)
		VertexAnimTrack &vat = m->GetVertexAnimTrack(a);
		VertexAnimKeyFrame *kv = vat.GetKeyframe(k);

		if (kv && kv->GetVertexCount() == m->GetVertexCount())
		{
			array = kv->GetVertexArray();
		}

		if (mRenderMode & fSkeletalVertexBlending)
		{
			// FIXME: Only updating in render loop for testing only!
			freyjaMeshUpdateBlendVertices(mesh, a, time);

			if (freyjaGetMeshBlendVertices(mesh))
				array = freyjaGetMeshBlendVertices(mesh);
		}
	}

	// NOTE: Once we switch to more advanced arrays we have to
	//       start locking the dynamic reallocation.
#ifndef USE_IMM_VERTEX
	glPushClientAttrib(GL_NORMAL_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// Load vertex array
	glVertexPointer(3, GL_FLOAT, sizeof(vec_t)*3, array);
	glNormalPointer(GL_FLOAT, sizeof(vec_t)*3, normalArray);
	//glTexCoordPointer(3, GL_FLOAT, sizeof(vec_t)*3, texcoordArray);
	
#endif

	if (FreyjaControl::mInstance->GetSelectedMesh() == mesh)
	{
		if (FreyjaControl::mInstance->GetObjectMode() == FreyjaControl::tMesh &&
			FreyjaControl::mControlPoints.size() == 0)
		{
			Cursor &cur = FreyjaControl::mInstance->GetCursor();

			switch (cur.GetMode())
			{
			case Cursor::Rotation: // About mesh center ( matrix abuse )
				glTranslatef(m->GetBoundingVolumeCenter().mVec[0],
							 m->GetBoundingVolumeCenter().mVec[1],
							 m->GetBoundingVolumeCenter().mVec[2]);
				
				glRotatef(cur.mRotate.mVec[0], 1,0,0);
				glRotatef(cur.mRotate.mVec[1], 0,1,0);
				glRotatef(cur.mRotate.mVec[2], 0,0,1);
				
				glTranslatef(-m->GetBoundingVolumeCenter().mVec[0],
							 -m->GetBoundingVolumeCenter().mVec[1],
							 -m->GetBoundingVolumeCenter().mVec[2]);
				break;
				
			case Cursor::Scale:
				// Haven't got the backend / frontend ready for this yet
				if (FreyjaControl::mInstance->GetCursor().mSelected)
				{
					hel::Vec3 u = cur.mScale;
					glScalef(u.mVec[0], u.mVec[1], u.mVec[2]);
				}
				break;

			case Cursor::Translation:
				// Haven't got the backend / frontend ready for this yet
				if (cur.mSelected)
				{
					hel::Vec3 u = (cur.mPos - cur.mLastPos);
					glTranslatef(u.mVec[0], u.mVec[1], u.mVec[2]);
				}
				break;
				
			default:
				;
			}
		}

		if (GetFlags() & fBoundingVolumes)
		{
			vec3_t min, max;

			// Box
			m->GetBBox(min, max);
			mglDrawSelectBox(min, max, WHITE);

			// Sphere
			mglDraw3dCircle(m->GetBoundingVolumeCenter().mVec, 
							m->GetBoundingVolumeRadius(), 
							64, 0, false);
			mglDraw3dCircle(m->GetBoundingVolumeCenter().mVec, 
							m->GetBoundingVolumeRadius(), 
							64, 2, false);
		}

	
		if (GetFlags() & fBoundingVolumes && m->GetFlags() & Mesh::fSelected)
		{
			vec3_t min, max;
			m->GetBBox(min, max);
			mglDrawSelectBox(min, max, YELLOW);
		}	
	}
	else if (GetFlags() & fBoundingVolumes && m->GetFlags() & Mesh::fSelected)
	{
		vec3_t min, max;
		m->GetBBox(min, max);
		mglDrawSelectBox(min, max, RED);
	}

	if (mRenderMode & fPoints)
	{
		Vertex *vertex;

		glBegin(GL_POINTS);
			
		for (uint32 i = 0; i < m->GetVertexCount(); ++i)
		{
			vertex = m->GetVertex(i);

			if (vertex)
			{
				(vertex->mFlags & Vertex::fSelected) ?
				glColor3fv(mColorVertexHighlight) :
				glColor3fv(mColorVertex);
#if USE_IMM_VERTEX
				glVertex3fv(array+vertex->mVertexIndex*3);
#else
				glArrayElement(vertex->mVertexIndex);
#endif
			}
		}

		glEnd();
	}

	if (mRenderMode & fNormals)
	{
		Vertex *vertex;
		hel::Vec3 v, n;

		glBegin(GL_LINES);
		glColor3fv(mColorVertexHighlight);
			
		for (uint32 i = 0; i < m->GetVertexCount(); ++i)
		{
			vertex = m->GetVertex(i);

			if (vertex)
			{
				v.mVec[0] = array[vertex->mVertexIndex*3];
				v.mVec[1] = array[vertex->mVertexIndex*3+1];
				v.mVec[2] = array[vertex->mVertexIndex*3+2];
				glVertex3fv(v.mVec);

				m->GetNormal(i, n.mVec);
				n = v + (n*1.75f);
				glVertex3fv(n.mVec);
			}
		}

		glEnd();
	}


	/* Render face as wireframe */
	if ( mRenderMode & fWireframe )
	{
		const vec_t scale = 1.0001f;

		glPushMatrix();
		glScalef(scale, scale, scale);

		hel::Vec3 color(mColorWireframe);
		if (m->GetFlags() & Mesh::fSelected)
		{
			color = hel::Vec3(mColorWireframeHighlight);
		}

		for (uint32 i = 0, n = m->GetFaceCount(); i < n; ++i)
		{
			Face *f = m->GetFace(i);

			if (!f) 
				continue;

			// NOTE: This is a test rendering, since you can't
			//       guarantee a plugin won't fuck up Vertex:mVertexIndex
			//       mappings in the Mesh.   =)
			glColor3fv(color.mVec);
			glDrawElements(GL_LINE_LOOP, 
						   f->mIndices.size(), 
						   GL_UNSIGNED_INT,
						   f->mIndices.get_array());
		}

		glPopMatrix();
	}

	glPopAttrib();


	/* Render solid face with material, color, or whatever you got */
	if (mRenderMode & fFace)
	{
		uint32 material = 99999;

		glPushAttrib(GL_ENABLE_BIT);

		if (mRenderMode & fMaterial)
		{
			//glEnable(GL_TEXTURE_2D);
			mglApplyMaterial(material);
		}
		else
		{
			glDisable(GL_TEXTURE_2D);
		}

		for (uint32 i = 0, n = m->GetFaceCount(); i < n; ++i)
		{
			Face *f = m->GetFace(i);

			if (!f) 
				continue;

			// Replaces group branching on bitflags
			if (f->mFlags & Face::fSelected)
			{
				glColor3fv(RED);
			}
			else if (GetFlags() & fGroupColors)
			{
				glColor3fv(mColors[f->mSmoothingGroup]);
			}
			else
			{
				glColor3fv(WHITE);
			}

			if (!mRenderMode & fMaterial)
			{
				// NOTE: This is a test rendering, since you can't
				//       guarantee a plugin won't fuck up Vertex:mVertexIndex
				//       mappings in the Mesh  =)
				glDrawElements(GL_POLYGON, 
							   f->mIndices.size(), 
							   GL_UNSIGNED_INT,
							   f->mIndices.get_array());
				continue;
			}
			else if ( f->mMaterial != material )
			{
				mglApplyMaterial(f->mMaterial);
				material = f->mMaterial;
			}

			Vector<index_t> &texcoordIndices = ((f->mFlags & 
												 Face::fPolyMappedTexCoords) ? 
												f->mTexCoordIndices :
												f->mIndices);

			glBegin(GL_POLYGON);

			for (uint32 j = 0; j < f->mIndices.size(); ++j)
			{
				//m->GetTexCoord(texcoordIndices[j], v.mVec);
				//glTexCoord2fv(v.mVec);
				glTexCoord2fv(texcoordArray + texcoordIndices[j]*3);

				//m->GetNormal(f->mIndices[j], v.mVec);
				//glNormal3fv(v.mVec);
				glNormal3fv(normalArray + f->mIndices[j]*3);

				glArrayElement(f->mIndices[j]);
			}
			
			glEnd();
		}

		glPopAttrib();
	}

#ifndef USE_IMM_VERTEX
	glPopClientAttrib();
#endif

	glPopMatrix();
}


void FreyjaRender::Flag(flags_t flag, bool t)
{
	t ? SetFlag(flag) : ClearFlag(flag);
}


void FreyjaRender::RenderModel(index_t model)
{
	glPushMatrix();

	/* Render meshes */
	glPushMatrix();
	
	if (FreyjaControl::mInstance->GetObjectMode() == FreyjaControl::tModel)
	{
		switch (FreyjaControl::mInstance->GetCursor().GetMode())
		{
		case freyja3d::Cursor::Rotation: // About model center ( matrix abuse )
			glRotatef(FreyjaControl::mInstance->GetCursor().mRotate.mVec[0],
					  1,0,0);
			glRotatef(FreyjaControl::mInstance->GetCursor().mRotate.mVec[1],
					  0,1,0);
			glRotatef(FreyjaControl::mInstance->GetCursor().mRotate.mVec[2],
					  0,0,1);
			break;
				
		case freyja3d::Cursor::Translation:
			{
				hel::Vec3 u = (FreyjaControl::mInstance->GetCursor().mPos -
						  FreyjaControl::mInstance->GetCursor().mLastPos);

				glTranslatef(u.mVec[0], u.mVec[1], u.mVec[2]);
			}
			break;

		default:
			;
		}
	}

	// Render each mesh of this model in turn
	for (uint32 i = 0, count = freyjaGetModelMeshCount(model); i < count; ++i)
	{
		index_t mesh = freyjaGetModelMeshIndex(model, i);

		if (freyjaIsMeshAllocated(mesh))
		{
			RenderMesh(mesh);
		}
	}

	glPopMatrix();


	freyja3d::OpenGL::BindFragmentGLSL(0); // Disable shaders

	{
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glClear(GL_DEPTH_BUFFER_BIT);

		Vector<hel::Vec3> ctrlpnts = FreyjaControl::GetControlPoints();
		uint32 i;
		foreach (ctrlpnts, i)
		{
			hel::Vec3 p = ctrlpnts[i];

			glPushMatrix();
			glTranslatef(p.mVec[0], p.mVec[1], p.mVec[2]);
			glColor3fv(RED);
			mglDrawSphere(8, 8, 0.5f);
			glPopMatrix();
		}

		if (mRenderMode & fBoundingVolSelection && ctrlpnts.size() == 2)
		{ 
			hel::Vec3 min = ctrlpnts[0];
			hel::Vec3 max = ctrlpnts[1];
			mglDrawSelectionBox(min.mVec, max.mVec, DARK_YELLOW);
		}

		glPopAttrib();
	}

	// FIXME: This renders all skeletons at once by design 
	/* Point type setting shows actual bind pose skeleton */
	if (mRenderMode & fBones2)
	{
		hel::Vec3 p;
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glClear(GL_DEPTH_BUFFER_BIT);

		/* Render spheres and bone geometery */
		for (uint32 i = 0; i < freyjaGetBoneCount(); ++i)
		{
			glPushMatrix();
#if 0
			freyjaGetBoneRotationEuler3fv(i, p.mVec);
			glRotatef(p.mX, 1, 0, 0);
			glRotatef(p.mY, 0, 1, 0);
			glRotatef(p.mZ, 0, 0, 1);

			freyjaGetBoneWorldPos3fv(i, p.mVec);
			glTranslatef(p.mX, p.mY, p.mZ);
#else
			glMultMatrixf(freyjaGetBoneBindPose16fv(i));
#endif
			(FreyjaControl::mInstance->GetSelectedBone() == i) ?
			glColor3fv(PINK) : glColor3fv(YELLOW);
			mglDrawSphere(12, 12, 0.5f);

			glPopMatrix();

			if (freyjaGetBoneParent(i) != INDEX_INVALID)
			{
				glPushMatrix();
				glMultMatrixf(freyjaGetBoneBindPose16fv(freyjaGetBoneParent(i)));

				(FreyjaControl::mInstance->GetSelectedBone() == i) ?
				glColor3fv(PINK) : 
				glColor3fv(FreyjaRender::mColorBoneHighlight);

				freyjaGetBoneTranslation3fv(i, p.mVec);
				mglDrawBone(2, p.mVec);

				glPopMatrix();
			}
		}


#define VIS_BONE_ANIM 1
#if VIS_BONE_ANIM
		hel::Mat44 combined;

		// Render spheres and lines
		for (uint32 i = 0; i < freyjaGetBoneCount(); ++i)
		{
			Bone *bone = Bone::GetBone(i);

			if (!bone)
				continue;

			glPushMatrix();
			//combined = bone->GetInverseBindPose() * bone->mTrack.mWorld;
			//glMultMatrixf( combined.mMatrix );
			
			freyjaGetBoneWorldPos3fv(i, p.mVec);
			glTranslatef(p.mX, p.mY, p.mZ);

			glColor3fv(DARK_RED);
			mglDrawSphere(10, 10, 0.4f);

			glPopMatrix();

			Bone *parent = Bone::GetBone(freyjaGetBoneParent(i));

			if (parent)
			{
				//combined = parent->GetInverseBindPose() * parent->mTrack.mWorld;
				combined = parent->mTrack.mWorld;

				glPushMatrix();
				glMultMatrixf( combined.mMatrix );

				glColor3fv(DARK_GREEN);
				freyjaGetBoneTranslation3fv(i, p.mVec);
				mglDrawBone(2, p.mVec);

				glPopMatrix();
			}
		}
#endif
		glPopAttrib();
	}

	/* Render bone names */
	if (mRenderMode & fBoneName)
	{
		glPushAttrib(GL_ENABLE_BIT);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);

		glColor3fv(WHITE);

		hel::Vec3 p;

		for (uint32 i = 0; i < freyjaGetBoneCount(); ++i)
		{
			// Only for names bind pose for now.
			freyjaBoneBindTransformVertex(i, p.mVec, 1.0f);
			//freyjaGetBoneWorldPos3fv(i, p.mVec);

			// Slightly broken b/c mAngles are not computed from quaternion.
			mPrinter.Print3d(p.mX, p.mY, p.mZ, 
							 -mAngles[2], -mAngles[1], -mAngles[0], 
							 0.05f, 
							 freyjaGetBoneNameString(i));
		}

		glPopAttrib();
	}
	
	/* Render skeleton */
	if (mRenderMode & fBones)
	{
		glPushAttrib(GL_ENABLE_BIT);

		/* Yay, let thou bones show through thine meshes 
		 * and it was good for editing */
		if (mRenderMode & fBonesNoZbuffer)
		{
			glClear(GL_DEPTH_BUFFER_BIT);
		}

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);

		FreyjaRender::mSelectedBone = FreyjaControl::mInstance->GetSelectedBone();
		RenderSkeleton(freyjaGetModelSkeleton(model), 0, 1.0f);

		glPopAttrib();
	}

	glPopMatrix();
}


void FreyjaRender::RenderSkeleton(index_t skeleton, uint32 bone, vec_t scale)
{
	const unsigned char x = 0, y = 1, z = 2;
	const unsigned char xr = 0, yr = 1, zr = 2;

	if (!freyjaGetSkeletonBoneCount(skeleton))
		return;

	index_t boneIndex = freyjaGetSkeletonBoneIndex(skeleton, bone);

	if (!freyjaIsBoneAllocated(boneIndex))
		return;

	hel::Vec3 pos, rot;

	/* Scale bones to match mesh scaling */
	freyjaGetBoneTranslation3fv(boneIndex, pos.mVec);

	/* Get orientation */
	freyjaGetBoneRotationEuler3fv(boneIndex, rot.mVec);

	/* Animation, if any... */
	if (FreyjaControl::mInstance->GetControlScheme() == 
		FreyjaControl::eScheme_Animation)
	{
		Bone *b = Bone::GetBone(boneIndex);

		if (b)
		{
			hel::Vec3 p, o;
			uint32 k = FreyjaControl::mInstance->GetSelectedKeyFrame();
			uint32 a = FreyjaControl::mInstance->GetSelectedAnimation();
			BoneTrack &track = b->GetTrack(a);

			if (track.GetRot((vec_t)k / track.GetRate(), o))
			{
				rot += o;
			}

			if (track.GetLoc((vec_t)k / track.GetRate(), p))
			{
				pos += p;
			}
		}
	}

	/* Radians to degrees */
	rot *= 57.295779513082323f;

	/* Scale bones to match mesh scaling */
	pos *= scale;

	/* Render bone joint */
	((FreyjaRender::mSelectedBone == boneIndex) ? 
	 glColor3fv(RED) : glColor3fv(GREEN));
	mglDrawJoint(FreyjaRender::mJointRenderType, pos.mVec);

	/* Render bone */
	((FreyjaRender::mSelectedBone == boneIndex) ? 
	 glColor3fv(FreyjaRender::mColorBoneHighlight) : glColor3fv(FreyjaRender::mColorBone));
	mglDrawBone(FreyjaRender::mBoneRenderType, pos.mVec);


	/* Transform child bones */
	glPushMatrix();

	glTranslatef(pos.mVec[x], pos.mVec[y], pos.mVec[z]);

#if ALLOW_BONE1_NAMES
	if (mRenderMode & fBoneName)
	{
		// OpenGLPrinter test
		glPushAttrib(GL_ENABLE_BIT);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		glColor3fv(WHITE);
		mPrinter.Print3d(0,0,0,  0,0,0, 0.05f, freyjaGetBoneNameString(boneIndex));
		glPopAttrib();
		// End OpenGLPrinter test
	}
#endif

	glRotatef(rot.mVec[zr], 0, 0, 1);
	glRotatef(rot.mVec[yr], 0, 1, 0);
	glRotatef(rot.mVec[xr], 1, 0, 0);

	for (uint32 i = 0, n = freyjaGetBoneChildCount(boneIndex); i < n; ++i)
	{
		index_t idx = freyjaGetBoneChild(boneIndex, i);

		if (idx != boneIndex)
		{
			RenderSkeleton(skeleton, idx, scale);
		}
	}

	glPopMatrix();
}


void FreyjaRender::DrawCurveWindow()
{
	const vec_t s = 6.5f;
	unsigned int width = GetWindowWidth();
	unsigned int height = GetWindowHeight();
	uint32 curKey = FreyjaControl::mInstance->GetSelectedKeyFrame();
	vec_t x = 0.0f, y = 0.0f;
	vec3_t yT = { 80.0f, 160.0f, 240.0f };

	Mesh *m = Mesh::GetMesh(FreyjaControl::mInstance->GetSelectedMesh());

	if (!m)
		return;

	vec2_t p;

	glPushMatrix();
	mgl2dProjection(width, height);
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glLineWidth(mDefaultLineWidth);
	glPointSize(mDefaultPointSize);

	// FIXME: This is temp test constant - replace with track switching later
	const uint32 track = 0;

	// FIXME: Need to handle DNE Tracks when sparse tracks are brought online.

	if (curKey < m->GetTransformTrack(track).mKeyFrames.size())
	{
		p[0] = x + curKey*s;		
		p[1] = 0.0f;
		glColor3fv(PINK); // gSweepColor
		glBegin(GL_LINES);
		glVertex2fv(p);
		p[1] = height;
		glVertex2fv(p);
		glEnd();
	}

	if (curKey < m->GetTransformTrack(track).mKeyFrames.size())
	{
		p[0] = x + curKey*s;		
		p[1] = 0.0f;
		glColor3fv(PINK); // gSweepColor
		glBegin(GL_LINES);
		glVertex2fv(p);
		p[1] = height;
		glVertex2fv(p);
		glEnd();
	}

	for ( uint32 ii = 0; ii < 3; ++ii )
	{
		glColor3fv(YELLOW);
		glBegin(GL_POINTS);
		unsigned int idx = 0;
		foreach (m->GetTransformTrack(track).mKeyFrames, idx)
		{
			Vec3x3KeyFrame *key = m->GetTransformTrack(track).GetKeyframe(idx);
			
			if (key)
			{
				p[0] = x + idx*s;
				
				p[1] = yT[ii] + key->mData[ii].mVec[0];
				glVertex2fv(p);
				
				p[1] = yT[ii] + key->mData[ii].mVec[1];
				glVertex2fv(p);
				
				p[1] = yT[ii] + key->mData[ii].mVec[2];
				glVertex2fv(p);
			}
		}
		glEnd();
	}

	// FIXME: Use cursor mode to determine which to 'modify and render here'
	if (m && FreyjaControl::mInstance->GetObjectMode() == FreyjaControl::tMesh)
	{
		vec_t time;
		hel::Vec3 v, pos, rot, scale;
		vec_t rateInverse = 1.0f / m->GetTransformTrack(track).GetRate();
		uint32 count = m->GetTransformTrack(track).GetKeyframeCount();

		for (uint32 j = 0; j < 3; ++j)
		{
			switch (j)
			{
			case 0:
				glColor3fv(RED);
				break;

			case 1:
				glColor3fv(GREEN);
				break;

			default:
				glColor3fv(BLUE);
				break;
			}

			glBegin(GL_LINES);
			glVertex2f(x, yT[2]);

			for (uint32 i = 0; i < count; ++i)
			{
				time = (vec_t)i * rateInverse;
				m->GetTransformTrack(track).GetTransform(time, pos, rot, scale);
				v.mVec[0] = x + i*s;
				v.mVec[1] = yT[2] + pos.mVec[j];
				glVertex2fv(v.mVec);
				glVertex2fv(v.mVec);
			}

			glVertex2f(width, yT[2]);
			glEnd();

			glBegin(GL_LINES);
			glVertex2f(x, yT[2]+30.0f);

			for (uint32 i = 0; i < count; ++i)
			{
				time = (vec_t)i * rateInverse;
				m->GetTransformTrack(track).GetTransform(time, pos, rot, scale);
				v.mVec[0] = x + i*s;
				v.mVec[1] = yT[2]+30.0f + rot.mVec[j];
				glVertex2fv(v.mVec);
				glVertex2fv(v.mVec);
			}

			glVertex2f(width, yT[2]+30.0f);
			glEnd();
		}
	}


	/* Skeletal animation, if any... */
	uint32 bone = FreyjaControl::mInstance->GetSelectedBone();
	Bone *b = Bone::GetBone(bone);

	// FIXME: Use cursor mode to determine which to 'modify and render here'
	if (b && FreyjaControl::mInstance->GetObjectMode() == FreyjaControl::tBone)
	{
		const vec_t shift = 60.0f;
		vec_t time;
		uint32 a = FreyjaControl::mInstance->GetSelectedAnimation();
		BoneTrack &track = b->GetTrack(a);
		vec_t rateInverse = 1.0f / track.GetRate();
		hel::Vec3 v, p;

		yT[2] += 20.0f;

		glLineWidth(1.5);

		for (uint32 j = 0; j < 3; ++j)
		{
			switch (j)
			{
			case 0:
				glColor3fv(DARK_RED);
				break;

			case 1:
				glColor3fv(DARK_GREEN);
				break;

			default:
				glColor3fv(DARK_BLUE);
				break;
			}

			glBegin(GL_LINES);
			glVertex2f(x, yT[2]);

			for (uint32 i = 0; i < track.GetRotKeyframeCount(); ++i)
			{
				time = (vec_t)i * rateInverse;
				p = track.GetRot(time) * 57.295779513082323f;
				v.mVec[0] = x + i*s;
				v.mVec[1] = yT[2] + p.mVec[j];
				glVertex2fv(v.mVec);
				glVertex2fv(v.mVec);
			}

#if 1
			glVertex2f(width, yT[2]);
			glEnd();

			switch (j)
			{
			case 0:
				glColor3fv(RED);
				break;

			case 1:
				glColor3fv(GREEN);
				break;

			default:
				glColor3fv(BLUE);
				break;
			}

			glBegin(GL_LINES);
			glVertex2f(x, yT[2]+shift);

			for (uint32 i = 0; i < track.GetRotKeyframeCount(); ++i)
			{
				time = (vec_t)i * rateInverse;
				p = track.GetLoc(time) * 2.0f;;
				v.mVec[0] = x + i*s;
				v.mVec[1] = yT[2]+shift + p.mVec[j];
				glVertex2fv(v.mVec);
				glVertex2fv(v.mVec);
			}

			glVertex2f(width, yT[2]+shift);
			glEnd();
		}
#endif
	}


	if (mRenderMode & fGrid)
	{
		const int szA = 20, szB = 80;
		int xi = 0;
		int yi = 0;
		int offset_x = (int)xi;//(xi % size) - width;
		int offset_y = (int)yi;//(yi % size) - height;

		glLineWidth(1.0);
		glColor3fv(mColorGridLine);
		glBegin(GL_LINES);
		for (yi = offset_y; yi < (int)width; yi+=szA)
		{
			glVertex2i(-width, yi);
			glVertex2i(width, yi);
		}

		for (xi = offset_x; xi < (int)height; xi+=szA)
		{
			glVertex2i(xi, -height);
			glVertex2i(xi, height);
		}
		glEnd();

		glLineWidth(1.75);
		glColor3fv(mColorGridSeperator);
		glBegin(GL_LINES);
		for (yi = offset_y; yi < (int)width; yi+=szB)
		{
			glVertex2i(-width, yi);
			glVertex2i(width, yi);
		}

		for (xi = offset_x; xi < (int)height; xi+=szB)
		{
			glVertex2i(xi, -height);
			glVertex2i(xi, height);
		}
		glEnd();
	}

	glColor3fv(mColorBackground);
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x, y+height);
	glVertex2f(x+width, y+height);
	glVertex2f(x+width, y);
	glEnd();

	glPopAttrib();

	ResizeContext(width, height);

	glPopMatrix();

	// OpenGLPrinter test
	glPushAttrib(GL_ENABLE_BIT);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glColor3fv(WHITE);
	mPrinter.Print2d(-mScaleEnv - 1.0f, mScaleEnv - 1.5f, 0.06f, "CURVE");
	glPopAttrib();
	// End OpenGLPrinter test
}


void FreyjaRender::DrawUVWindow()
{
	unsigned int width = GetWindowWidth();
	unsigned int height = GetWindowHeight();

	glPushMatrix();

	mgl2dProjection(width, height);

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glLineWidth(mDefaultLineWidth);
	glPointSize(mVertexPointSize);

	Mesh *m = freyjaGetMeshClass(FreyjaControl::mInstance->GetSelectedMesh());

	if (m)
	{
		/* Render UVs as points */
		// FIXME: This will do many points for each point, opt later
		//        when we can't have mixed polymaps
		glBegin(GL_POINTS);
		glColor3fv(mColorVertexHighlight);	

		for (uint32 i = 0, n = m->GetFaceCount(); i < n; ++i)
		{
			Face *f = m->GetFace(i);
			hel::Vec3 v;

			if (!f || 
				f->mMaterial != FreyjaControl::mInstance->GetSelectedMaterial())
				continue;

			if (f->mFlags & Face::fPolyMappedTexCoords)
			{
				for (uint32 j = 0, jn = f->mTexCoordIndices.size(); j < jn; ++j)
				{
					m->GetTexCoord(f->mTexCoordIndices[j], v.mVec);
					v[0] *= width;
					//v[1] *= height;
					v[1] = height - v[1]*height;
					glVertex2fv(v.mVec);
				}
			}
			else
			{
				for (uint32 j = 0, jn = f->mIndices.size(); j < jn; ++j)
				{
					m->GetTexCoord(f->mIndices[j], v.mVec);
					v[0] *= width;
					//v[1] *= height;
					v[1] = height - v[1]*height;
					glVertex2fv(v.mVec);
				}
			}
		}

		glEnd();


		// Render wireframe faces	
		for (uint32 i = 0, n = m->GetFaceCount(); i < n; ++i)
		{
			Face *f = m->GetFace(i);
			hel::Vec3 v;
			
			if (!f) 
				continue;
			
			/* Render face as wireframe */
			glBegin(GL_LINE_LOOP);

			if (f->mFlags & Face::fSelected)
				glColor3fv(mColorWireframeHighlight);
			else
				glColor3fv(mColorWireframe);
			
			if (f->mFlags & Face::fPolyMappedTexCoords)
			{
				for (uint32 j = 0, jn = f->mTexCoordIndices.size(); j < jn; ++j)
				{
					m->GetTexCoord(f->mTexCoordIndices[j], v.mVec);
					v.mVec[0] *= width;
					v.mVec[1] = height - v.mVec[1]*height;
					glVertex2fv(v.mVec);
				}
			}
			else
			{
				for (uint32 j = 0, jn = f->mIndices.size(); j < jn; ++j)
				{
					m->GetTexCoord(f->mIndices[j], v.mVec);
					v.mVec[0] *= width;
					v.mVec[1] = height - v.mVec[1]*height;
					glVertex2fv(v.mVec);
				}
			}
			
			glEnd();
		}
	}

	DrawQuad(0.0, 0.0, width, height);
	glPopAttrib();
	ResizeContext(width, height);
	glPopMatrix();
}


void FreyjaRender::DrawGrid(freyja_plane_t plane, int w, int h, int size)
{
	int x, y;

	//glPushMatrix();

	if (mRenderMode & fRenderGridClearedZBuffer)
		glClear(GL_DEPTH_BUFFER_BIT);

	glLineWidth(2.0);

	switch (plane)
	{
	case PLANE_FRONT:
	case PLANE_BACK:
		x = (int)mScroll[0];
		y = (int)mScroll[1];

		glBegin(GL_LINES); 
		glColor3fv(FreyjaRender::mColorAxisX);
		glVertex2i(-w, y);
		glVertex2i(w, y);
    
		glColor3fv(FreyjaRender::mColorAxisY);
		glVertex2i(x, -h);
		glVertex2i(x, h);
		glEnd();
		break;


	case PLANE_TOP:
	case PLANE_BOTTOM:
		x = (int)mScroll[0];
		y = (int)mScroll[2];

		glBegin(GL_LINES); 
		glColor3fv(FreyjaRender::mColorAxisX);
		glVertex2i(-w, y);
		glVertex2i(w, y);
    
		glColor3fv(FreyjaRender::mColorAxisZ);
		glVertex2i(x, -h);
		glVertex2i(x, h);
		glEnd();
		break;


	case PLANE_LEFT:
	case PLANE_RIGHT:
	   x = (int)mScroll[2];
	   y = (int)mScroll[1];

		glBegin(GL_LINES); 
		glColor3fv(FreyjaRender::mColorAxisZ);
		glVertex2i(-w, y);
		glVertex2i(w, y);
    
		glColor3fv(FreyjaRender::mColorAxisY);
		glVertex2i(x, -h);
		glVertex2i(x, h);
		glEnd(); 
		break;

	default:
		return;
   }

	//if (mRenderMode & fGrid)
   {
		int offset_x = (x % size) - w;
		int offset_y = (y % size) - h;

		glLineWidth(1.0);
		glColor3fv(mColorGridLine);

		glBegin(GL_LINES);
		for (y = offset_y; y < w; y++)
		{
			glVertex2i(-w, y);
			glVertex2i(w, y);
		}

		for (x = offset_x; x < h; x++)
		{
			glVertex2i(x, -h);
			glVertex2i(x, h);
		}
		glEnd();

		glLineWidth(1.75);
		glColor3fv(mColorGridSeperator);
		glBegin(GL_LINES);
		for (y = offset_y; y < w; y+=10)
		{
			glVertex2i(-w, y);
			glVertex2i(w, y);
		}

		for (x = offset_x; x < h; x+=10)
		{
			glVertex2i(x, -h);
			glVertex2i(x, h);
		}
		glEnd();
   }

   //glPopAttrib();
   //glPopMatrix();
}


void FreyjaRender::DrawWindow(freyja_plane_t plane)
{
	switch (plane)
	{
	case DRAW_CAMERA:
		// FIXME: Add camera support back to this build
		DrawFreeWindow();
		return;
		
	case PLANE_FREE:
		DrawFreeWindow();
		return;
	   
	case DRAW_UV:
		DrawUVWindow();
		return;
		
	case DRAW_MATERIAL:
		DrawMaterialEditWindow();
		return;

	case DRAW_CURVE:
		DrawCurveWindow();
		return;

	default:
		;
	}

	glPushMatrix(); // New for 2d caption, etc

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE);
	glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, 0); // color

	if (mRenderMode & fGrid)
		DrawGrid(plane, GetWindowWidth(), GetWindowHeight(), 10);

#ifdef PLANE_NOTIFY_WITH_AXIS
	glPushMatrix();
	glTranslatef(-mScaleEnv, -mScaleEnv + 2.5f, 10.0);

	switch (plane)
	{
	case PLANE_FRONT: // XY
		break;

	case PLANE_BACK: // XY
		glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
		break;

	case PLANE_TOP: // XZ
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		break;

	case PLANE_BOTTOM: // XZ
		glRotatef(-90.0f, 1.0f, 0.0, 0.0);
		break;

	case PLANE_LEFT: // ZY
		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		break;

	case PLANE_RIGHT: // ZY
		glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
		break;

	default:
		;
	}

	glLineWidth(2.0);
	mglDrawEditorAxis();

	glPopMatrix();
#endif

	glPopAttrib();

	switch (plane)
	{
	case PLANE_FRONT: // XY
		glTranslatef(mScroll[0], mScroll[1], 0.0);
		break;

	case PLANE_BACK: // XY
		glTranslatef(mScroll[0], mScroll[1], 0.0);
		glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
		break;

	case PLANE_TOP: // XZ
		glTranslatef(mScroll[0], mScroll[2], 0.0);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		break;

	case PLANE_BOTTOM: // XZ
		glTranslatef(mScroll[0], mScroll[2], 0.0);
		glRotatef(-90.0f, 1.0f, 0.0, 0.0);
		break;

	case PLANE_LEFT: // ZY
		glTranslatef(mScroll[2], mScroll[1], 0.0);
		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		break;

	case PLANE_RIGHT: // ZY
		glTranslatef(mScroll[2], mScroll[1], 0.0);
		glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
		break;

	default:
		;
	}

	glPushMatrix();

	RenderLights();

	glScalef(mZoom, mZoom, mZoom);

	mglGetOpenGLModelviewMatrix16fv(gModelViewMatrix);

	if (mRenderMode & fDrawPickRay)
	{
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

		hel::Vec3 rayEnd = mTestRay.mOrigin + mTestRay.mDir * 1000;
		glPointSize(2.0);
		glBegin(GL_POINTS);	
		glColor3fv(GREEN);	
		glVertex3fv(mTestRay.mOrigin.mVec);
		glVertex3fv(rayEnd.mVec);
		glEnd();
		glBegin(GL_LINES);	
		glColor3fv(YELLOW);	
		glVertex3fv(mTestRay.mOrigin.mVec);
		glColor3fv(DARK_YELLOW);	
		glVertex3fv(rayEnd.mVec);
		glEnd();
		glPointSize(mDefaultPointSize);

		glPopAttrib();
	}

	// Draw model geometry, metadata visualizations, and all that good stuff
	for (unsigned int i = 0; i < freyjaGetModelCount(); ++i)
	{
		RenderModel(i);
	}

	FreyjaControl::mInstance->GetCursor().Display();

	glPopMatrix();

	glPopMatrix(); // New for 2d caption, etc

	// OpenGLPrinter test
	glPushAttrib(GL_ENABLE_BIT);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glColor3fv(WHITE);	

	switch (plane)
	{
	case PLANE_FRONT:
		mPrinter.Print2d(-mScaleEnv - 1.0f, mScaleEnv - 1.5f, 0.05f, "FRONT");
		break;

	case PLANE_BACK:
		mPrinter.Print2d(-mScaleEnv - 1.0f, mScaleEnv - 1.5f, 0.05f, "BACK");
		break;

	case PLANE_TOP:
		mPrinter.Print2d(-mScaleEnv - 1.0f, mScaleEnv - 1.5f, 0.05f, "TOP");
		break;

	case PLANE_BOTTOM:
		mPrinter.Print2d(-mScaleEnv - 1.0f, mScaleEnv - 1.5f, 0.05f, "BOTTOM");
		break;

	case PLANE_LEFT:
		mPrinter.Print2d(-mScaleEnv - 1.0f, mScaleEnv - 1.5f, 0.05f, "LEFT");
		break;

	case PLANE_RIGHT:
		mPrinter.Print2d(-mScaleEnv - 1.0f, mScaleEnv - 1.5f, 0.05f, "RIGHT");
		break;

	default:
		mPrinter.Print2d(-mScaleEnv - 1.0f, mScaleEnv - 1.5f, 0.05f, "????");
	}

	glPopAttrib();
}


void FreyjaRender::DrawMaterialEditWindow()
{
	float pos[4] = {128.0, 128.0, 128.0, 1.0};


	glPushMatrix();
	glTranslatef(-8.0f, 0.0f, 0.0f);

	/* Setup lighting */	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	vec4_t color;
	freyjaGetLightAmbient(freyjaGetCurrentLight(), color);
	glLightfv(GL_LIGHT0, GL_AMBIENT, color);
	freyjaLightDiffuse(freyjaGetCurrentLight(), color);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
	freyjaGetLightSpecular(freyjaGetCurrentLight(), color);
	glLightfv(GL_LIGHT0, GL_SPECULAR, color);

#ifdef USE_TORUS_TEST
	glPushMatrix();
	glRotatef(45.0f, 1, 0, 0);
	glRotatef(45.0f, 0, 0, 1);
	mglDrawTorus(3.0, 10.0);
	glPopMatrix();
#else
	/* Cast light on sphere colored/detailed by material */
	static vec_t dy = 0.0f;
	dy += 0.5f;
	if (dy > 360.0f) dy = 0.0f;
	glPushMatrix();
	glRotatef(dy, 0, 1, 0);
	mglApplyMaterial(freyjaGetCurrentMaterial());
	mglDrawSphere(128, 128, 10.0);
	glPopMatrix();
#endif

	const float x = 12.0f, y = -8.0f, z = 0.0f, w = 16.0f, h = 16.0f, s = 1.001f;

	glPushMatrix();
	glDisable(GL_LIGHTING);

	glColor3fv(BLACK);
	glBegin(GL_LINE_LOOP);
	glVertex3f(x*s, y*s, z);
	glVertex3f(x*s, (y+h)*s, z);
	glVertex3f((x+w)*s, (y+h)*s, z);
	glVertex3f((x+w)*s, y*s, z);
	glEnd();


	static vec_t xd = 0.0f;

	if ( mScrollingUV_X )
	{
		xd += 0.001f;
		if (xd > 1.0f) xd = 0.0f;
	}

	vec_t xAB = 1.0f * xd;
	vec_t xCD = xAB + 1.0f;

	glColor3fv(WHITE);
	mglApplyMaterial(freyjaGetCurrentMaterial());
	glBegin(GL_QUADS);
	//glTexCoord2f(0.0, 1.0);
	glTexCoord2f(xAB, 1.0f);
	glVertex3f(x, y, z);
	//glTexCoord2f(0.0, 0.0);
	glTexCoord2f(xAB, 0.0f);
	glVertex3f(x, y+h, z);
	//glTexCoord2f(1.0, 0.0);
	glTexCoord2f(xCD, 0.0f);
	glVertex3f(x+w, y+h, z);
	//glTexCoord2f(1.0, 1.0);
	glTexCoord2f(xCD, 1.0f);
	glVertex3f(x+w, y, z);
	glEnd();	
	glPopMatrix();
	glPopMatrix();

	// OpenGLPrinter test
	glPushAttrib(GL_ENABLE_BIT);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
	glEnable(GL_BLEND);
	glColor3fv(WHITE);
	mPrinter.Print2d(-mScaleEnv - 1.0f, mScaleEnv - 1.5f, 0.06f, "MATERIAL");
	glPopAttrib();
	// End OpenGLPrinter test
}


