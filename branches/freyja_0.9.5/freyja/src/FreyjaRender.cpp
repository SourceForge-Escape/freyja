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

#include "FreyjaOpenGL.h" // includes <windows.h>

#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>

#include <mgtk/ResourceEvent.h>
#include <freyja/SkeletonABI.h>
#include <freyja/Mesh.h>
#include <freyja/MeshABI.h>


#include "freyja_events.h"
#include "FreyjaControl.h"

#include "FreyjaRender.h"

#define TEST_NEW_BACKEND_FORMAT 1
#define COLORED_POLYGON -1

using namespace freyja;
using namespace freyja3d;

FreyjaRender *FreyjaRender::mSingleton = 0x0;

Ray FreyjaRender::mTestRay;

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

double gMatrix[16];


FreyjaRender::FreyjaRender() :
	mViewMode(VIEWMODE_MODEL_VIEW),
	mRenderMode(fBoundingVolSelection | 
				fBonesNoZbuffer | 
				fBoundingVolumes),
	mWidth(640),
	mHeight(480),
	mTextureId(0),
	mInitContext(false),
	mScaleEnv(40.0f), //20.0; // 40.0f for higher res
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

	mSingleton = this;
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
	RenderModel model;
	//unsigned int i;


#ifdef PLANE_NOTIFY_WITH_AXIS
	glPushMatrix();
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	glTranslatef(-mScaleEnv, -mScaleEnv + 2.5f, 10.0);

	glRotatef(mAngles[0], 1.0, 0.0, 0.0);
	glRotatef(mAngles[1], 0.0, 1.0, 0.0);
	glRotatef(mAngles[2], 0.0, 0.0, 1.0);

	mglDrawEditorAxis();

	glPopAttrib();
	glPopMatrix();
#endif

	//glPushMatrix();

	glRotatef(mAngles[0], 1.0, 0.0, 0.0);
	glRotatef(mAngles[1], 0.0, 1.0, 0.0);
	glRotatef(mAngles[2], 0.0, 0.0, 1.0);

	glTranslatef(mScroll[0], mScroll[1]+8.0f, mScroll[2]);

	if (mRenderMode & fGrid)
	{
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

		glLineWidth(1.25f);
		mglDrawGrid(mColorGridLine, 50.0f, 2.0f, 1.0f);

		glLineWidth(1.75f);
		glBegin(GL_LINES);
		glColor3fv(RED);
		glVertex3f(-50.0f, 0.0f, 0.0f);	
		glVertex3f(50.0f, 0.0f, 0.0f);
		glColor3fv(BLUE);
		glVertex3f(0.0f, 0.0f, -50.0f);	
		glVertex3f(0.0f, 0.0f, 50.0f);

		glColor3fv(mColorGridLine);

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

		glPopAttrib();
	}
	//glPopMatrix();

	//glPushMatrix();

	RenderLights();

	glScalef(mZoom, mZoom, mZoom);

	mglGetOpenGLModelviewMatrix(gMatrix);

	if (mRenderMode & fDrawPickRay)
	{
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

		Vec3 rayEnd = mTestRay.mOrigin + mTestRay.mDir * 1000;
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

	//glPopMatrix();

	for (uint32 i = 0; i < freyjaGetRenderModelCount(); ++i)
	{
		freyjaGetRenderModel(i, model);
		Render(model);
	}

	FreyjaControl::mInstance->GetCursor().Display();
}




///////// PRIVATE METHODS ////////////////////////////////

void FreyjaRender::BindTexture(unsigned int texture)
{
	static int hack = 0;


	if (mRenderMode & fTexture && texture != mTextureId)
	{
		if ((int)texture == COLORED_POLYGON)
		{
			hack = COLORED_POLYGON;
			glDisable(GL_TEXTURE_2D);
		}
		else
		{
			if (hack == COLORED_POLYGON)
			{
				hack = 0;
				glEnable(GL_TEXTURE_2D);
			}

			mTextureId = texture;  // First texture is white for colored_poly
			glBindTexture(GL_TEXTURE_2D, texture+1);
		}
	}
}


void FreyjaRender::DrawQuad(float x, float y, float w, float h)
{
	if (!(mRenderMode & fTexture))
	{
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_TEXTURE_2D);

		glColor3fv(WHITE);

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

		glPopAttrib();
	}
	else  // If fTexture draw textured quad
	{
		glPushAttrib(GL_ENABLE_BIT);
		glEnable(GL_TEXTURE_2D);

		glColor3fv(WHITE);

		mglApplyMaterial(FreyjaControl::mInstance->GetSelectedTexture());

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
	bool arb_multitexture, ext_texture_env_combine;


	/* Log OpenGL driver support information */
	freyja_print("[GL Driver Info]");
	freyja_print("\tVendor     : %s", glGetString(GL_VENDOR));
	freyja_print("\tRenderer   : %s", glGetString(GL_RENDERER));
	freyja_print("\tVersion    : %s", glGetString(GL_VERSION));
	freyja_print("\tExtensions : %s", (char*)glGetString(GL_EXTENSIONS));

	/* Test for extentions */
	arb_multitexture = mglHardwareExtTest("GL_ARB_multitexture");
	ext_texture_env_combine = mglHardwareExtTest("GL_EXT_texture_env_combine");

	freyja_print("\tGL_ARB_multitexture       \t\t[%s]",
			 arb_multitexture ? "YES" : "NO");

	freyja_print("\tGL_EXT_texture_env_combine\t\t[%s]",
			 ext_texture_env_combine ? "YES" : "NO");

	// Set up Z buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_TEXTURE_2D);

	// Set up culling
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);

	// Set background to black
	glClearColor(BLACK[0], BLACK[1], BLACK[2], BLACK[3]);

	// Disable lighting 
	glDisable(GL_LIGHTING);

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

	glDisable(GL_POINT_SMOOTH);
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_AUTO_NORMAL);
	glDisable(GL_LOGIC_OP);
	glDisable(GL_TEXTURE_1D);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_FOG);

	glDisable(GL_NORMALIZE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_EDGE_FLAG_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glPolygonMode(GL_FRONT, GL_FILL);

	glMatrixMode(GL_MODELVIEW);

	// Generate XYZ Axis symbol display list
	//glNewList(1, GL_COMPILE);
	//mglDrawAxis(0.25f, 1.2f, 0.872f);
	//glEndList();

	SetNearHeight(20.0f);

	mWidth = width;
	mHeight = height;
	mInitContext = true;
}


void FreyjaRender::Display() 
{ 
	if (!mInitContext)
	{ 
		return;
	}
	
	// Mongoose 2002.02.02, Cache for use in calls from here
	Vec3 v = FreyjaControl::mInstance->GetSceneTranslation();
	HEL_VEC3_COPY(v.mVec, mScroll);
	
	glClearColor(mColorBackground[0], mColorBackground[1], mColorBackground[2], 
				 1.0);
	
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Disable lighting and texture here until the color visualizations
	// are updated
	glDisable(GL_TEXTURE_2D);
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

			for (long i = 0; i < mViewportsCount; ++i)
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
	
	glFlush();
	freyja_swap_buffers();
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


void FreyjaRender::Render(RenderMesh &mesh)
{
	const vec_t scale = 1.0001f;
	static Vector3d u, v;
	static RenderPolygon face;
	Mesh *m = freyjaModelGetMeshClass(0, mesh.id);

	if (!m)
		return;

	vec_t *array = m->GetVertexArray();

	glPushMatrix();

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glPointSize(mVertexPointSize);

	// 'World effects' 

	if (FreyjaControl::mInstance->GetSelectedMesh() == mesh.id)
	{
		if (FreyjaControl::mInstance->GetObjectMode() == FreyjaControl::tMesh)
		{
			switch (FreyjaControl::mInstance->GetCursor().GetMode())
			{
			case freyja3d::Cursor::Rotation: // About mesh center ( matrix abuse )
				glTranslatef(m->GetBoundingVolumeCenter().mVec[0],
							 m->GetBoundingVolumeCenter().mVec[1],
							 m->GetBoundingVolumeCenter().mVec[2]);
				
				glRotatef(FreyjaControl::mInstance->GetCursor().mRotate.mVec[0],
						  1,0,0);
				glRotatef(FreyjaControl::mInstance->GetCursor().mRotate.mVec[1],
						  0,1,0);
				glRotatef(FreyjaControl::mInstance->GetCursor().mRotate.mVec[2],
						  0,0,1);
				
				glTranslatef(-m->GetBoundingVolumeCenter().mVec[0],
							 -m->GetBoundingVolumeCenter().mVec[1],
							 -m->GetBoundingVolumeCenter().mVec[2]);
				break;
				
			case freyja3d::Cursor::Translation:
				// Haven't got the backend / frontend ready for this yet
				if (FreyjaControl::mInstance->GetCursor().mSelected)
				{
					Vec3 u = (FreyjaControl::mInstance->GetCursor().mPos -
							  FreyjaControl::mInstance->GetCursor().mLastPos);

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
				glVertex3fv(array+vertex->mVertexIndex*3);
			}
		}

		glEnd();
	}

	if (mRenderMode & fNormals)
	{
		Vertex *vertex;
		Vec3 v, n;

		glBegin(GL_LINES);
		glColor3fv(mColorVertexHighlight);
			
		for (uint32 i = 0; i < m->GetVertexCount(); ++i)
		{
			vertex = m->GetVertex(i);

			if (vertex)
			{
				// FIXME: later this will use vertex/face remapped index
				m->GetVertexPos(i, v.mVec);
				m->GetNormal(i, n.mVec);
				n = v + (n*1.75f);
				glVertex3fv(v.mVec);
				glVertex3fv(n.mVec);
			}
		}

		glEnd();
	}

	// Render wireframe faces	
	for (uint32 i = 0, n = m->GetFaceCount(); i < n; ++i)
	{
		Face *f = m->GetFace(i);

		if (!f) 
			continue;

		/* Render face as wireframe */
		if ( mRenderMode & fWireframe )
		{
			glBegin(GL_LINE_LOOP);
			glColor3fv(mColorWireframeHighlight);

			for (uint32 j = 0; j < f->mIndices.size(); ++j)
			{
				m->GetVertexPos(f->mIndices[j], v.mVec);
				v *= scale; // Scale out a little to avoid z-fighting
				glVertex3fv(v.mVec);
			}
				
			glEnd();
		}
	}

	glPopAttrib();


	glPushAttrib(GL_ENABLE_BIT);

	/* Render solid face with material, color, or whatever you got */
	if (mRenderMode & fFace)
	{
		uint32 material = 99999;

		if (mRenderMode & fTexture)
		{
			glEnable(GL_TEXTURE_2D);
		}

		for (uint32 i = 0, n = m->GetFaceCount(); i < n; ++i)
		{
			Face *f = m->GetFace(i);

			if (!f) 
				continue;

			if (f->mFlags & Face::fSelected)
			{
				glColor3fv(RED);
			}
			else
			{
				glColor3fv(WHITE);
			}

			if ( f->mMaterial != material )
			{
				mglApplyMaterial(f->mMaterial);
				material = f->mMaterial;
			}

			glBegin(GL_POLYGON);

			if (f->mFlags & Face::fPolyMappedTexCoords)
			{
				for (uint32 j = 0; j < f->mIndices.size(); ++j)
				{
					m->GetTexCoord(f->mTexCoordIndices[j], v.mVec);
					glTexCoord2fv(v.mVec);
					m->GetNormal(f->mIndices[j], v.mVec);
					glNormal3fv(v.mVec);
					m->GetVertexPos(f->mIndices[j], v.mVec);
					glVertex3fv(v.mVec);
				}
			}
			else
			{
				for (uint32 j = 0; j < f->mIndices.size(); ++j)
				{
					m->GetTexCoord(f->mIndices[j], v.mVec);
					glTexCoord2fv(v.mVec);
					m->GetNormal(f->mIndices[j], v.mVec);
					glNormal3fv(v.mVec);
					m->GetVertexPos(f->mIndices[j], v.mVec);
					glVertex3fv(v.mVec);
				}
			}
			
			glEnd();
		}
		
	}

	glPopAttrib();

	glPopMatrix();
}


void FreyjaRender::Render(RenderModel &model)
{
	glPushMatrix();

	// This is the old vertex bbox selection highlighting 
#if 0
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);

	//Vector<unsigned int> *list;
	//vec3_t min, max;
	//vec3_t *xyz;
	//int32 meshIndex = mModel->getCurrentMesh();

	/* Highlight current vertex group
	 * -- this should be model specific later:
	 * eg mModel->getCurrentGroup() -> model.index */
	if (mRenderMode & fBoundingVolSelection && model.getMeshCount() > 0)
	{
		/* Render bounding box */
		freyjaGetMeshFrameBoundingBox(mModel->getCurrentMesh(),
									  mModel->getCurrentGroup(), min, max);
		mglDrawSelectBox(min, max, WHITE);
	}

	/* Render bounding box selection */
	mModel->getVertexSelection(min, max, &list);

	if (mRenderMode & fBoundingVolSelection)
	{
		mglDrawBbox(min, max, RED, mColorWireframe);
	}

	/* Render selected vertices */
	if (list && !list->empty())
	{
		glPointSize(mDefaultPointSize + 2.0f);
		glColor3fv(RED);
		glBegin(GL_POINTS);
		 
		for (i = list->begin(), count = list->end(); i < count; ++i)
		{
			xyz = freyjaGetVertexXYZ((*list)[i]);

			if (xyz)
			{
				glVertex3fv(*xyz);
			}
		}
		 
		glEnd();
		glPointSize(mDefaultPointSize);
	}

	glPopAttrib();
#endif

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
				Vec3 u = (FreyjaControl::mInstance->GetCursor().mPos -
						  FreyjaControl::mInstance->GetCursor().mLastPos);

				glTranslatef(u.mVec[0], u.mVec[1], u.mVec[2]);
			}
			break;

		default:
			;
		}
	}

	RenderMesh rmesh;

	for (uint32 i = 0, count = model.getMeshCount(); i < count; ++i)
	{
		if (model.getMesh(i, rmesh, 0))
			Render(rmesh);
	}

	glPopMatrix();


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
		Render(model.getSkeleton(), 0, 1.0f);

		glPopAttrib();
	}

	glPopMatrix();
}


void FreyjaRender::Render(RenderSkeleton &skeleton, uint32 currentBone,
						  vec_t scale)
{
	const unsigned char x = 0, y = 1, z = 2;
	const unsigned char xr = 0, yr = 1, zr = 2;
	RenderBone bone, child;
	Vector3d pos;
	unsigned int i, n, index;


	if (!skeleton.getBoneCount())
		return;

	if (!skeleton.getBone(currentBone, bone))
		return;

	/* Scale bones to match mesh scaling */
	pos = bone.translate * scale;

	/* Render bone joint */
	((FreyjaRender::mSelectedBone == currentBone) ? 
	 glColor3fv(RED) : glColor3fv(GREEN));
	mglDrawJoint(FreyjaRender::mJointRenderType, pos.mVec);

	/* Render bone */
	((FreyjaRender::mSelectedBone == currentBone) ? 
	 glColor3fv(FreyjaRender::mColorBoneHighlight) : glColor3fv(FreyjaRender::mColorBone));
	mglDrawBone(FreyjaRender::mBoneRenderType, pos.mVec);

	/* Transform child bones */
	glPushMatrix();
	glTranslatef(pos.mVec[x], pos.mVec[y], pos.mVec[z]);

	freyjaGetBoneRotationEuler3fv(bone.mBoneIndex, bone.rotate.mVec);
	bone.rotate *= 57.295779513082323f;
	glRotatef(bone.rotate.mVec[zr], 0, 0, 1);
	glRotatef(bone.rotate.mVec[yr], 0, 1, 0);
	glRotatef(bone.rotate.mVec[xr], 1, 0, 0);
	n = bone.getChildrenCount();

	for (i = 0; i < n; ++i)
	{
		index = bone.getBoneIndex(i);

		if (index != currentBone)
		{
			Render(skeleton, index, scale);
		}
	}

	glPopMatrix();
}

Vector<Vec3> gCurveTest;


void FreyjaRender::DrawCurveWindow()
{
	unsigned int width = GetWindowWidth();
	unsigned int height = GetWindowHeight();
	float x = 0.0f, y = 0.0f;


	if (gCurveTest.size() == 0)
	{
		vec_t xx = 0, yy = 0;
		for (int i = 0; i < 64; ++i)
		{
			xx = helRandomNum(xx+1.0f, xx+11.0f);
			yy = helRandomNum(0.0f, height/2);

			Vec3 v(xx, yy, 0.0f);
			gCurveTest.pushBack(v);
		}
	}

	glPushMatrix();
	mgl2dProjection(width, height);
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glLineWidth(mDefaultLineWidth);
	glPointSize(mDefaultPointSize);

	// Fake 'data' to test the rendering look and feel
	glColor3fv(GREEN);
	glBegin(GL_POINTS);
	unsigned int idx;
	foreach (gCurveTest, idx)
	{
		Vec3 v = gCurveTest[idx] + Vec3(x, y, 0.0f);
		glVertex2fv(v.mVec);
	}
	glEnd();

	glColor3fv(YELLOW);
	glBegin(GL_LINES);
	glVertex2f(x, y);
	foreach (gCurveTest, idx)
	{
		Vec3 v = gCurveTest[idx] + Vec3(x, y, 0.0f);
		glVertex2fv(v.mVec);
		glVertex2fv(v.mVec);
	}
	glVertex2f(width, y);
	glEnd();


	if (mRenderMode & fGrid)
	{
		const int szA = 20, szB = 80;
		int xi = (int)x;
		int yi = (int)y;
		int offset_x = (int)x;//(xi % size) - width;
		int offset_y = (int)y;//(yi % size) - height;

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

	ResizeContext(width, height);
	glPopAttrib();
	glPopMatrix();
}


/* Mongoose 2004.03.30, 
 * Dependences of this method:
 * x GL context width and height
 * - Current mesh to skin
 * - OpenGL 2d view helper function
 * - Quad to render skin helper function
 */
void FreyjaRender::DrawUVWindow()
{
	RenderMesh mesh;
	RenderPolygon face;
	unsigned int width, height;


	width = GetWindowWidth();
	height = GetWindowHeight();

	glPushMatrix();

	mgl2dProjection(width, height);
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glLineWidth(mDefaultLineWidth);
	glPointSize(mDefaultPointSize);


	// Until multi model editing comes just use model[0]
	RenderModel model;
	bool valid = false;
	if (freyjaGetRenderModel(0, model))
	{
		if (model.getMesh(FreyjaControl::mInstance->GetSelectedMesh(), mesh, 0))
		{
			valid = true;
		}
	}
	

	if (!valid)
	{
		DrawQuad(0.0, 0.0, width, height);
		ResizeContext(width, height);
		glPopAttrib();
		glPopMatrix();
		return;
	}

#if 0
	float x = 0.0f, y = 0.0f;

	for (i = mModel->mUVMap.begin(), n = mModel->mUVMap.end();  i < n; ++i)
	{
		// Until multi model editing comes just use model[0]
		//if (!freyjaGetRenderModelPolygon(0, mModel->mUVMap[i], face))
		//	continue;
		
		if (!mesh.getPolygon(mModel->mUVMap[i], face))
			continue;

		if (face.material != (int)mModel->getCurrentTextureIndex())
			continue;

		glBegin(GL_LINE_LOOP);
		glColor3fv(BLUE);

		for (j = 0; j < face.count; ++j)
		{
			x = face.texcoords[j].mVec[0] * width;
			y = face.texcoords[j].mVec[1] * height;
					
			glVertex2f(x, height - y);
		}
		
		glEnd();
	}
#endif



#if 0
	for (i = 0, n = mesh.getPolygonCount(); i < n; ++i)
	{
		if (!mesh.getPolygon(i, face))
			continue;

		if (mesh.id == FreyjaControl::mInstance->GetSelectedMesh())
		{
			if (face.id == FreyjaControl::mInstance->GetSelectedFace())
				glColor3fv(RED);
			else if (face.flags & fPolygon_Alpha)  
				glColor3fv(ORANGE);
			else
				glColor3fv(CYAN);
		}
		else
		{
			glColor3fv(mColorWireframe);
		}
		
		if (face.material != (int)FreyjaControl::mInstance->GetSelectedTexture())
		{
			if (mesh.id == FreyjaControl::mInstance->GetSelectedMesh())
			{
				glColor3fv(GREEN);
			}
			else
			{
				continue;
			}
		}		

		// Mongoose: Draw texel polygons
		glBegin(GL_LINE_LOOP);
		
		for (j = 0; j < face.count; ++j)
		{
			x = face.texcoords[j].mVec[0] * width;
			y = face.texcoords[j].mVec[1] * height;
					
			glVertex2f(x, height - y);
		}
			
		glEnd();


		// Mongoose: Draw vertices
		glBegin(GL_POINTS);
		
		for (j = 0; j < face.count; ++j)
		{
			if ((int)FreyjaControl::mInstance->GetSelectedTexture() == face.material)
			{
				switch (j)
				{
				case 0:
					glColor3fv(GREEN);
					break;
				case 2:
					glColor3fv(BLUE);
					break;
				case 3:
					glColor3fv(ORANGE);
					break;
				case 4:
					glColor3fv(WHITE);
					break;
				default:
					glColor3fv(YELLOW);
				}
			}
			else
			{
				glColor3fv(mColorVertexHighlight);
			}
				
			x = face.texcoords[j].mVec[0] * width;
			y = face.texcoords[j].mVec[1] * height;
				
			glVertex2f(x, height - y);
		}
			
		glEnd();
	}
#else
	Mesh *m = freyjaModelGetMeshClass(0, FreyjaControl::mInstance->GetSelectedMesh());


	if (!m)
		return;

	glPushMatrix();

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glPointSize(mVertexPointSize);

	/* Render UVs as points */
	// FIXME: This will do many points for each point, opt later
	//        when we can't have mixed polymaps
	glBegin(GL_POINTS);
	glColor3fv(mColorVertexHighlight);	

	for (uint32 i = 0, n = m->GetFaceCount(); i < n; ++i)
	{
		Face *f = m->GetFace(i);
		Vec3 v;

		if (!f ||f->mMaterial != FreyjaControl::mInstance->GetSelectedTexture())
			continue;

		if (f->mFlags & Face::fPolyMappedTexCoords)
		{
			for (uint32 j = 0, jn = f->mTexCoordIndices.size(); j < jn; ++j)
			{
				m->GetTexCoord(f->mTexCoordIndices[j], v.mVec);
				v.mVec[0] *= width;
				//v.mVec[1] *= height;
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
				//v.mVec[1] *= height;
				v.mVec[1] = height - v.mVec[1]*height;
				glVertex2fv(v.mVec);
			}
		}
	}

	glEnd();


	// Render wireframe faces	
	for (uint32 i = 0, n = m->GetFaceCount(); i < n; ++i)
	{
		Face *f = m->GetFace(i);
		Vec3 v;

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
				//v.mVec[1] *= height;
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
				//v.mVec[1] *= height;
				v.mVec[1] = height - v.mVec[1]*height;
				glVertex2fv(v.mVec);
			}
		}
				
		glEnd();
	}

	//glPopAttrib();


#endif

	DrawQuad(0.0, 0.0, width, height);
	glPopAttrib();
	ResizeContext(width, height);
	glPopMatrix();
}


void FreyjaRender::DrawGrid(freyja_plane_t plane, int w, int h, int size)
{
	int x, y;

	glPushMatrix();

	if (mRenderMode & fRenderGridClearedZBuffer)
		glClear(GL_DEPTH_BUFFER_BIT);

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
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

   if (mRenderMode & fGrid)
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

   glPopAttrib();
   glPopMatrix();
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

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);

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

	mglGetOpenGLModelviewMatrix(gMatrix);

	if (mRenderMode & fDrawPickRay)
	{
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

		Vec3 rayEnd = mTestRay.mOrigin + mTestRay.mDir * 1000;
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
	RenderModel model;

	for (unsigned int i = 0; i < freyjaGetRenderModelCount(); ++i)
	{
		freyjaGetRenderModel(i, model);
		Render(model);
	}

	FreyjaControl::mInstance->GetCursor().Display();

	glPopMatrix();
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
}


