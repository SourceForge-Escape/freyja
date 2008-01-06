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
 *           template generator script.  <mongoose@icculus.org>
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

#include "config.h"

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

#include <freyja/CameraABI.h>
#include <freyja/LightABI.h>
#include <freyja/SkeletonABI.h>
#include <freyja/Bone.h>
#include <freyja/Mesh.h>
#include <freyja/MeshABI.h>
#include <freyja/MaterialABI.h>
#include <freyja/Metadata.h>

#include "freyja_events.h"
#include "Plugins.h"
#include "Control.h"
#include "Cursor.h"
#include "MaterialControl.h"
#include "OpenGLRenderableStrategy.h"

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
vec4_t FreyjaRender::mColorJoint;
vec4_t FreyjaRender::mColorJointHighlight;
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
unsigned int FreyjaRender::mBoneRenderType = 2;
unsigned char FreyjaRender::mJointRenderType = 1;
int FreyjaRender::mPatchDisplayList = -1;

vec_t FreyjaRender::mBoneLineWidth = 3.0;        /* Custom artifact size */
vec_t FreyjaRender::mBonePointSize = 5.0;
vec_t FreyjaRender::mDefaultPointSize = 3.5;
vec_t FreyjaRender::mDefaultLineWidth = 1.0;
vec_t FreyjaRender::mVertexPointSize = 3.5f;

int FreyjaRender::EvModeAutoKeyframeId = -1;

matrix_t gModelViewMatrix;

freyja3d::Cursor gCursor;
freyja3d::Cursor& GetCursor()
{ return gCursor; }

index_t GetSelectedCamera()
{ return 0; }

freyja::Material* GetSelectedMaterial()
{ return MaterialControl::GetInstance()->GetCurrentMaterial(); }

 
FreyjaRender::FreyjaRender() :
	mTimer(),
	mViewMode( VIEWMODE_MODEL_EDIT ),
	mRenderMode(fBoundingVolSelection | 
				fBonesNoZbuffer | 
				fBoundingVolumes |
				fFPSCap |
				fGroupColors),
	mWidth(640),
	mHeight(480),
	mZoom(1.0f),
	mTextureId(0),
	mInitContext( false ),
	mScroll( 0.0f, -18.0f, 0.0f ),
	mScaleEnv(35.0f), // 40.0f is about too much, Use a larger number for higher res -- 35.0f is default
	mFar(6000.0f),
	mNear(0.1f),
	mFovY(40.0f),
	mNearHeight(20.0f),
	mSelectedView( PLANE_FREE ),
	mSelectedViewport( 0 )
{
	mAngles[0] = 18.0f;
	mAngles[1] = 42.0f;
	mAngles[2] = 0.0f;

	mUpperLeftText[0] = -mScaleEnv - 8;
	mUpperLeftText[1] = mScaleEnv - 1.5f;

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

	// To avoid branching in renderer by using a little more memory.
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
		mColors[ 9][i] = YELLOW[i]*0.25f;
		mColors[10][i] = YELLOW[i]*0.50f;
		mColors[11][i] = GREEN[i]*0.25f;
		mColors[12][i] = GREEN[i]*0.50f;
		mColors[13][i] = YELLOW[i]*0.75f;
		mColors[14][i] = GREEN[i]*0.75f;
		mColors[15][i] = PINK[i]*0.50f;
	}
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////


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


void FreyjaRender::DrawScene( freyja::Scene* scene )
{
	if ( scene )
	{
		glPushAttrib( GL_ENABLE_BIT );
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glPointSize(mVertexPointSize);

		glDisable(GL_TEXTURE_2D);
		glColor3fv( WHITE );

		freyja::Material* last_material = NULL;

		for ( RenderableIterator it = scene->GetRenderListIterator( ), end = it.end();
			  it != end; it++ )
		{
			glPushMatrix( );

			/* Translate. */
			{
				const hel::Vec3& pos = (*it)->GetWorldPosition( );
				glTranslatef( pos.mX, pos.mY, pos.mZ );
			}

			/* Rotate. */
			{
				const hel::Quat& q = (*it)->GetWorldOrientation( );
				vec_t theta;
				hel::Vec3 v;
				q.GetAxisAngles( theta, v );
				glRotatef( theta, v.mX, v.mY, v.mZ );
			}

			/* Scale. */
			{
				hel::Vec3 v = (*it)->GetScale( );
				glScalef( v.mX, v.mY, v.mZ );
			}

			if ( (*it)->GetMaterial( ) != last_material )
			{
				// FIXME: Set new material / shader here or use a renderable to set it inline for sorted renderlist.
				//ApplyMaterial( renderable->GetMaterial( ) );
				//last_material = renderable->GetMaterial( );
			}

			OpenGLRenderableStrategy strategeryizeification;
			(*it)->Draw( &strategeryizeification );

			glPopMatrix( );
		}

		glPopAttrib( );
	}
}


void FreyjaRender::DrawCamWindow()
{
	hel::Vec3 pos, target;
	hel::Vec3 up(0.0f, 1.0f, 0.0f);

	// FIXME: Use viewport's selected camera.
	index_t camera = GetSelectedCamera( );
	freyjaGetCameraPos3fv(camera, pos.mVec);
	freyjaGetCameraTarget3fv(camera, target.mVec);
	freyjaGetCameraUp3fv(camera, up.mVec);
	
	//glLoadIdentity();

	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	gluPerspective(90.0f, ((GLdouble)mWidth)/((GLdouble)mHeight), 4.0f, 9000.0f);

	gluLookAt(pos.mX, pos.mY, pos.mZ,
			  target.mX, target.mY,target.mZ,
			  up.mX, up.mY, up.mZ);
	
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	if (mRenderMode & fSolidPlane)
	{
		glPushAttrib(GL_ENABLE_BIT);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glColor3fv(WHITE);
		OpenGLRenderableStrategy::ApplyMaterial(GetSelectedMaterial());
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


	ApplyLights();

	glScalef(mZoom, mZoom, mZoom);

	mglGetOpenGLModelviewMatrix16fv(gModelViewMatrix);

	if (mRenderMode & fDrawPickRay)
	{
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

		hel::Vec3 rayEnd = mTestRay.GetOrigin() + mTestRay.GetDir() * 1000;
		glPointSize(2.0);
		glBegin(GL_POINTS);	
		glColor3fv(GREEN);	
		glVertex3fv(mTestRay.GetOrigin().mVec);
		glVertex3fv(rayEnd.mVec);
		glEnd();
		glBegin(GL_LINES);	
		glColor3fv(YELLOW);	
		glVertex3fv(mTestRay.GetOrigin().mVec);
		glColor3fv(DARK_YELLOW);	
		glVertex3fv(rayEnd.mVec);
		glEnd();



		glBegin(GL_POINTS);	
		glColor3fv(BLUE);	
		glVertex3fv(pos.mVec);
		//glVertex3fv(target.mVec);
		glEnd();
		glBegin(GL_LINES);	
		glColor3fv(RED);	
		glVertex3fv(pos.mVec);
		glColor3fv(DARK_RED);	
		glVertex3fv(target.mVec);
		glEnd();
		glPointSize(mDefaultPointSize);

		glPopAttrib();
	}

	DrawScene( gScene );

	DrawIcons();

	glPopMatrix();


	// FIXME: Use camera data to fix projections and status...

#if PLANE_NOTIFY_WITH_AXIS
	glPushMatrix();
	glTranslatef(-mScaleEnv + 2.5f, -mScaleEnv + 2.5f, 10.0);
	//glRotatef(mAngles[0], 1.0, 0.0, 0.0);
	//glRotatef(mAngles[1], 0.0, 1.0, 0.0);
	//glRotatef(mAngles[2], 0.0, 0.0, 1.0);

	glClear(GL_DEPTH_BUFFER_BIT);
	mglDrawEditorAxis();
	glPopMatrix();
#endif

	ResizeContext(mWidth, mHeight); // reset for other windows

	// OpenGLPrinter test
	glPushAttrib(GL_ENABLE_BIT);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glColor3fv(WHITE);
	//mPrinter.Print2d(-mScaleEnv, mScaleEnv - 1.5f, 0.06f, freyjaGetCameraNameString(camera) );
	glPopAttrib();
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
		OpenGLRenderableStrategy::ApplyMaterial(GetSelectedMaterial());
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

	ApplyLights();

	glScalef(mZoom, mZoom, mZoom);

	mglGetOpenGLModelviewMatrix16fv(gModelViewMatrix);

	if (mRenderMode & fDrawPickRay)
	{
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

		hel::Vec3 rayEnd = mTestRay.GetOrigin() + mTestRay.GetDir() * 1000;
		glPointSize(2.0);
		glBegin(GL_POINTS);	
		glColor3fv(GREEN);	
		glVertex3fv(mTestRay.GetOrigin().mVec);
		glVertex3fv(rayEnd.mVec);
		glEnd();
		glBegin(GL_LINES);	
		glColor3fv(YELLOW);	
		glVertex3fv(mTestRay.GetOrigin().mVec);
		glColor3fv(DARK_YELLOW);	
		glVertex3fv(rayEnd.mVec);
		glEnd();
		//glPointSize(mDefaultPointSize);

		{	
			hel::Vec3 pos, target;
			index_t camera = GetSelectedCamera();
			freyjaGetCameraPos3fv(camera, pos.mVec);
			freyjaGetCameraTarget3fv(camera, target.mVec);

			//glBegin(GL_POINTS);	
			glColor3fv(BLUE);	
			//glVertex3fv(pos.mVec);
			glPushMatrix();
			glTranslatef(pos.mX, pos.mY, pos.mZ);
			mglDrawControlPoint();
			glPopMatrix();

			//glVertex3fv(target.mVec);
			glEnd();
			glBegin(GL_LINES);	
			glColor3fv(RED);	
			glVertex3fv(pos.mVec);
			glColor3fv(DARK_RED);	
			glVertex3fv(target.mVec);
			glEnd();
			glPointSize(mDefaultPointSize);
		}

		glPopAttrib();
	}

	DrawScene ( gScene );

	//glPopMatrix(); // Remove scaling

	DrawIcons();

	glPopMatrix();


#if PLANE_NOTIFY_WITH_AXIS
	glPushMatrix();
	glTranslatef(-mScaleEnv + 2.5f, -mScaleEnv + 2.5f, 10.0);
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
	mPrinter.Print2d(mUpperLeftText[0], mUpperLeftText[1], 0.05f, "Orbit");
	glPopAttrib();
}


void FreyjaRender::DrawMetadataRenderables()
{
#if FIXME
	/* Metadata objects */
	glPushAttrib(GL_LIGHTING_BIT);  
	glDisable(GL_LIGHTING);

	BindColorTexture();
	glColor3fv(ORANGE);

	for (uint32 i = 0, n = Metadata::GetObjectCount(); i < n; ++i)
	{
		Metadata* metadata = Metadata::GetObjectByUid( i );
		
		// This method will encounter any 'gaps' ( NULL pointers ) in the container.
		if ( metadata )
		{
			glPushMatrix();

			const Vec3 &pos = metadata->GetPos();
			glTranslatef(pos.mX, pos.mY, pos.mZ);
			mglDrawControlPoint();

			glPopMatrix();
		}
	}

	glPopAttrib();



	glPushAttrib(GL_LIGHTING_BIT);  
	glDisable(GL_LIGHTING);

	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);//GL_CLIENT_VERTEX_ARRAY_BIT);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	for (uint32 i = 0, n = Metadata::GetObjectCount(); i < n; ++i)
	{
		Metadata* metadata = Metadata::GetObjectByUid( i );
		
		// This method will encounter any 'gaps' ( NULL pointers ) in the container.
		if ( metadata )
		{
			Renderable* renderable = metadata->GetRenderable(); 

			if ( renderable )
			{
				BindColorTexture();

				{
					glPushMatrix();
					const Vec3 &pos = metadata->GetPos();
					glTranslatef(pos.mX, pos.mY, pos.mZ);

					Vec3 rot;
					metadata->GetRot().GetEulerAngles(rot.mVec);
					rot *= HEL_180_OVER_PI;
					glRotatef(rot.mX, 1,0,0);
					glRotatef(rot.mY, 0,1,0);
					glRotatef(rot.mZ, 0,0,1);

					glPushMatrix();
					const Vec3& scale = metadata->GetScale();
					glScalef(scale.mX, scale.mY, scale.mZ);
				}

				#warning "FIXME: Add support for metadata:renderable material."
#if 1
		glPushAttrib(GL_POLYGON_BIT);
		glPushAttrib(GL_LINE_BIT | GL_DEPTH_BUFFER_BIT);

		glPolygonMode(GL_BACK, GL_LINE);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		glDisable(GL_DEPTH_TEST);

		glLineWidth(3.0f);

		glColor3fv(BLACK);
#endif

				glVertexPointer(3, GL_FLOAT, 0, renderable->mVertices);
				glNormalPointer(GL_FLOAT, 0, renderable->mNormals);
				glTexCoordPointer(3, GL_FLOAT, 0, renderable->mTexcoords);
				glDrawElements(GL_TRIANGLES, renderable->mFaceCount*3,
				               GL_UNSIGNED_SHORT, renderable->mIndices);

#if 1
		glPopAttrib(); // GL_LINE_BIT | GL_DEPTH_BUFFER_BIT

		glPolygonMode(GL_FRONT, GL_FILL);
		glCullFace(GL_BACK);

		glPopAttrib(); // GL_POLYGON_BIT

				glColor3fv(WHITE);

				glVertexPointer(3, GL_FLOAT, 0, renderable->mVertices);
				glNormalPointer(GL_FLOAT, 0, renderable->mNormals);
				glTexCoordPointer(3, GL_FLOAT, 0, renderable->mTexcoords);
				glDrawElements(GL_TRIANGLES, 
				               (renderable->mFaceCount<<1)+renderable->mFaceCount,
				               GL_UNSIGNED_SHORT, renderable->mIndices);
#endif

				glPopMatrix();
				glPopMatrix();
			}
		}
	}

	glPopAttrib();

	glPopClientAttrib();

	/* End metadata objects */

#endif // FIXME
}


void FreyjaRender::DrawIcons()
{
	DrawMetadataRenderables();

	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	//glDisable(GL_TEXTURE_2D);
	//glDisable(GL_BLEND);

	BindColorTexture();

	if (mRenderMode & fLighting)
	{
		vec4_t pos;
		freyjaGetLightPosition4v(0, pos);
		
		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2]);

		glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
		mglDrawControlPoint();

		glPopMatrix();
	}

	GetCursor().Display();

#if FIXME
	/* Special edit modes. */
	switch ( GetObjectMode() )
	{
	case FreyjaControl::tCamera:
		for (uint32 i = 0, n = freyjaGetCameraCount(); i < n; ++i)
		{
			hel::Vec3 target, pos;
			freyjaGetCameraTarget3fv(i, target.mVec);
			freyjaGetCameraPos3fv(i, pos.mVec);
			
			glBegin(GL_LINES);
			glColor3fv(RED);
			glVertex3fv(pos.mVec);
			glColor3fv(DARK_RED);
			glVertex3fv(target.mVec);
			glEnd();

			glPushMatrix();
			glTranslatef(pos.mX, pos.mY, pos.mZ);
			glColor3fv(RED);
			mglDrawControlPoint();
			glPopMatrix();
			
			glPushMatrix();
			glTranslatef(target.mX, target.mY, target.mZ);
			glColor3fv(DARK_RED);
			mglDrawControlPoint();
			glPopMatrix();
		}
		break;

	default:
		;
	}
#endif

	// Draw currently enabled plugin 'icons'.
	// PluginManager.Draw();
	freyja3d_plugin_draw_loop();

	glPopAttrib();
}


void FreyjaRender::AttachMethodListeners()
{
	CreateListener("eScreenShot", &FreyjaRender::EvScreenShot);

	CreateListener("eShadowVolume", &FreyjaRender::EvRenderShadowVolume);
	CreateListener("eViewports", &FreyjaRender::eViewports);
	CreateListener("eRenderBoneName", &FreyjaRender::eRenderBoneName);
	CreateListener("eRenderSkeleton", &FreyjaRender::eRenderSkeleton);
	CreateListener("eRenderSkeleton2", &FreyjaRender::eRenderSkeleton2);
	CreateListener("eRenderSkeleton3", &FreyjaRender::eRenderSkeleton3);
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


	CreateListener("eViewportBack", &FreyjaRender::EvViewportBack);
	CreateListener("eViewportBottom", &FreyjaRender::EvViewportBottom);
	CreateListener("eViewportRight", &FreyjaRender::EvViewportRight);
	CreateListener("eViewportFront", &FreyjaRender::EvViewportFront);
	CreateListener("eViewportTop", &FreyjaRender::EvViewportTop);
	CreateListener("eViewportLeft", &FreyjaRender::EvViewportLeft);
	CreateListener("eViewportOrbit", &FreyjaRender::EvViewportOrbit);
	CreateListener("eViewportUV", &FreyjaRender::EvViewportUV);
	CreateListener("eViewportCurve", &FreyjaRender::EvViewportCurve);
	CreateListener("eViewportCamera", &FreyjaRender::EvViewportCamera);
	CreateListener("eViewportMaterial", &FreyjaRender::EvViewportMaterial);
	//CreateListener("", &FreyjaRender::);

	// Mode events
	EvModeAutoKeyframeId = CreateListener("eModeAnim", &FreyjaRender::EvModeAutoKeyframe);
	CreateListener("eModeUV", &FreyjaRender::EvModeUV);
	CreateListener("eModeModel", &FreyjaRender::EvModeModel);
	CreateListener("eModeMaterial", &FreyjaRender::EvModeMaterial);
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
		OpenGLRenderableStrategy::ApplyMaterial(GetSelectedMaterial());

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
	mRenderMode &= ~flag;
}


void FreyjaRender::InitContext(uint32 width, uint32 height, bool fastCard)
{
	if ( width > 0 && height > 0 )
	{
		OpenGL::Instance( );

		// Due to whacky bullshit we're seriously going to have to use
		// GL_LESS depth and GL_FRONT culling or the interface will break

		// NOTE: The 'whacky bullshit' is due to various windings and
		// 'native' formats are allowed, so until that changes don't change this

		// Set up Z buffer
		glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LESS);
		glDepthFunc(GL_LEQUAL);
		//glClearDepth(1.0f);

		// Stencil
		//glClearStencil(0);

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
			//glEnable(GL_LINE_SMOOTH);
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

		ResizeContext( width, height );
	}
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
	if ( !mInitContext ||
		 (GetMode() & fFPSCap) && mTimer.GetTicks() < 16 ) // ~60.0 fps cap
	{
		//freyja_print("%ims since last frame", mTimer.GetTicks());
		return;
	}

	glClearColor(mColorBackground[0], mColorBackground[1], mColorBackground[2], 1.0);
	
	// 2007.05.20 - Removed depth test toggle
	//glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);

	/* Disable lighting here until the color materials are updated. */
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
		if ( mRenderMode & fViewports )
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
			DrawWindow( GetSelectedView( ) );
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
	mPrinter.Print2d(15.0f, mScaleEnv - 1.5f, 0.04f, FREYJA_WATERMARK );
	glPopAttrib();

	//CHECK_OPENGL_ERROR( glFlush() );
	glFlush();

	mTimer.Reset();
}


void FreyjaRender::ResizeContext(uint32 width, uint32 height) 
{
	if ( !mInitContext || !width || !height )
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



////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////

void FreyjaRender::ApplyLights()
{
	if (mRenderMode & fLighting)
	{
		vec4_t pos = { 0.0f, 0.0f, 0.0f, 1.0f};
		freyjaGetLightPosition4v(0, pos);

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
#if 0
	else 
	{
		glEnable(GL_LIGHTING);

		vec4_t pos = { 0.0f, 0.0f, 1.0f, 0.0f };
		vec4_t color = { 0.5f, 0.5f, 0.5f, 1.0f };

		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_POSITION, pos);
		glLightfv(GL_LIGHT0, GL_AMBIENT, color);
		//glLightfv(GL_LIGHT0, GL_DIFFUSE, color);


		pos[3] = -1.0f;
		glEnable(GL_LIGHT1);
		glLightfv(GL_LIGHT1, GL_POSITION, pos);
		glLightfv(GL_LIGHT1, GL_AMBIENT, color);
		//glLightfv(GL_LIGHT1, GL_DIFFUSE, color);
	}
#endif
}


// This sure has a lot of branching...
void FreyjaRender::RenderMesh(index_t mesh)
{
#if FIXME
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
		uint32 a = GetSelectedAnimation();
		uint32 k = GetSelectedKeyFrame();	

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
#warning "FIXME: Still using update in render request for blending!"
			// FIXME: Only updating in render loop for testing only!
			freyjaMeshUpdateBlendVertices(mesh, a, time);

			if (freyjaGetMeshBlendVertices(mesh))
				array = freyjaGetMeshBlendVertices(mesh);
		}
	}

	// NOTE: Once we switch to more advanced arrays we have to
	//       start locking the dynamic reallocation.
	glPushClientAttrib(GL_NORMAL_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// Load vertex array
	glVertexPointer(3, GL_FLOAT, sizeof(vec_t)*3, array);
	glNormalPointer(GL_FLOAT, sizeof(vec_t)*3, normalArray);
	//glTexCoordPointer(3, GL_FLOAT, sizeof(vec_t)*3, texcoordArray);

	if (GetSelectedMesh() == mesh)
	{
#if FIXME
		if (GetObjectMode() == FreyjaControl::tMesh && gControlPoints.size() == 0)
		{
			Cursor &cur = FreyjaControl::GetInstance()->GetCursor();

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
				if (FreyjaControl::GetInstance()->GetCursor().mSelected)
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
#endif

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
	 


	/* Render outline */
	if ( !(mRenderMode & fWireframe) && 
		 (mRenderMode & fFace) &&
		 ( m->GetFlags() & Mesh::fSelected ||
		   mesh == FreyjaControl::GetInstance()->GetSelectedMesh() ) )
	{
		const vec_t scale = 1.0002f;

		glPushAttrib(GL_POLYGON_BIT);
		glPushAttrib(GL_LINE_BIT | GL_DEPTH_BUFFER_BIT);

		glPolygonMode(GL_BACK, GL_LINE);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		glDisable(GL_DEPTH_TEST);

		glLineWidth(3.0f);

		glPushMatrix();
		glScalef(scale, scale, scale);

		for (uint32 i = 0, n = m->GetFaceCount(); i < n; ++i)
		{
			Face *f = m->GetFace(i);

			if (!f) 
				continue;

			// NOTE: This is a test rendering, since you can't
			//       guarantee a plugin won't fuck up Vertex:mVertexIndex
			//       mappings in the Mesh.   =)
			glColor3fv(mColorWireframeHighlight);
			glDrawElements(GL_POLYGON, //GL_LINE_LOOP
						   f->mIndices.size(), 
						   GL_UNSIGNED_INT,
						   f->mIndices.get_array());
		}

		glPopMatrix();

		glPopAttrib(); // GL_LINE_BIT | GL_DEPTH_BUFFER_BIT

		glPolygonMode(GL_FRONT, GL_FILL);
		glCullFace(GL_BACK);

		glPopAttrib(); // GL_POLYGON_BIT
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
				glColor3fv(	(vertex->mFlags & Vertex::fSelected) ?
							mColorVertexHighlight : mColorVertex );
				glArrayElement(vertex->mVertexIndex);
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
		glColor3fv(mColorWireframe);

		for (uint32 i = 0, n = m->GetFaceCount(); i < n; ++i)
		{
			Face *f = m->GetFace(i);

			if (!f) 
				continue;

			// NOTE: Can't guarantee Vertex:mVertexIndex after plugin 
			//       operation if they fail to Repack() the Mesh.
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

	glPopClientAttrib();

	glPopMatrix();

#endif
}


// TEST
//mstl::Vector<vec_t> gVertexBufferTranslated;
mstl::Vector<vec_t> gShadowVolume;

void FreyjaRender::RenderMeshShadowVolume(index_t mesh)
{
	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
				 GL_POLYGON_BIT | GL_STENCIL_BUFFER_BIT);

	glDisable(GL_LIGHTING);

#if TEST_SHADOW_VOLUME_SURFACES
	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glColor4f(0.0f, 0.0f, 0.0f, 0.4f);

	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);

	RenderMeshShadowVolumeSurfaces(mesh);
#else
	/* Disable color, disable depth, and enable stencil */
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_FALSE);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 0, ~0); // 1's comp of 0 should fill for size

	glFrontFace(GL_CCW);

	// Front zfail pass
	glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);
	glCullFace(GL_FRONT);
	RenderMeshShadowVolumeSurfaces(mesh);

	// Back zfail pass
	glStencilOp(GL_KEEP, GL_DECR, GL_KEEP);
	glCullFace(GL_BACK);
	//RenderMeshShadowVolumeSurfaces(mesh);

	// Shadow color pass
	{
		glDisable(GL_CULL_FACE);

		/* Enable color, blending, and setup shadow color value with alpha. */
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glStencilFunc(GL_NOTEQUAL, 0, ~0);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

		glDepthMask(GL_TRUE);

		glPushMatrix();
		glLoadIdentity();
		glBegin(GL_QUADS);
		glVertex3f(-40.0f, 40.0f, 100);
		glVertex3f(-40.0f, -40.0f, 100);
		glVertex3f(40.0f, -40.0f, 100);
		glVertex3f(40.0f, 40.0f, 100);
		glEnd();
		glPopMatrix();
	}
#endif

	glPopAttrib();
}


void FreyjaRender::RenderMeshShadowVolumeSurfaces(index_t mesh)
{
#if FIXME
	Mesh *m = freyjaGetMeshClass(mesh);
	vec_t *shadow = NULL, *array = NULL;

	if (m)
	{
		vec3_t pos = {0.0f, 0.0f, 0.0f};
		const vec_t oco = 100.0f;  //0.0f;

		// Only allow light #0 for now
		vec4_t lightPos;
		freyjaGetLightPosition4v(0, lightPos);

		if (mRenderMode & fSkeletalVertexBlending)
		{
			if (freyjaGetMeshBlendVertices(mesh))
			{
				array = freyjaGetMeshBlendVertices(mesh);
			}

			if ( m->GetBlendShadowVolume(gShadowVolume, lightPos, pos, oco) )
			{
				shadow = gShadowVolume.get_array();
			}
		}
		else
		{
			// There is no need to translate as long as mesh: world = local...
			//m->CopyVertexBuffer(gVertexBufferTranslated);
			// translate gVertexBufferTranslated by pos
			//vec_t *array = gVertexBufferTranslated.get_array();
			array = m->GetVertexArray();
			
			if ( m->GetShadowVolume(gShadowVolume, lightPos, pos, oco) )
			{
				shadow = gShadowVolume.get_array();
			}
		}
	}


	if ( !shadow || !array )
		return;

	mstl::Vector<index_t> indices;
	indices.reserve(4);

	// Render caps for any polygon edge count
	for (uint32 i = 0, n = m->GetFaceCount(); i < n; ++i)
	{
		Face *f = m->GetFace(i);

		if (!f) 
			continue;

		/* Just go ahead and store this for edges here, and use for cap too. */
		for (int k = f->mIndices.size() - 1; k > -1; --k)
		{
			indices.push_back(f->mIndices[k]);
		}

#if SHADOW_VOLUME_WITH_ARRAYS
		/* Back cap, which is the reverse order of front. */
		glVertexPointer(3, GL_FLOAT, sizeof(vec_t)*3, array);
		glDrawElements(GL_POLYGON, 
					   f->mIndices.size(), 
					   GL_UNSIGNED_INT,
					   f->mIndices.get_array());

		/* Front cap */
		glVertexPointer(3, GL_FLOAT, sizeof(vec_t)*3, shadow);
		glDrawElements(GL_POLYGON, 
					   indices.size(), 
					   GL_UNSIGNED_INT,
					   indices.get_array());

#else // Don't have OpenGL 1.1?  Should I care?

		/* Back cap, which is the reverse order of front. */
		glBegin(GL_POLYGON);
			
		{
			uint32 j;
			foreach (f->mIndices, j)
			{
				glVertex3fv(array + f->mIndices[j]*3);
			}
		}

		glEnd();

		/* Front cap */
		glBegin(GL_POLYGON);
			
		{
			uint32 j;
			foreach (indices, j)
			{
				glVertex3fv(shadow + indices[j]*3);
			}
		}

		glEnd();
#endif

#if 1
		/* Edges */
		glBegin(GL_QUAD_STRIP);
		glVertex3fv(array + f->mIndices[0]*3);
		glVertex3fv(shadow + indices[0]*3);

		for (uint32 k = 1, kn = indices.size(); k < kn; ++k)
		{
			uint32 idx = k % kn;
			glVertex3fv(array + f->mIndices[idx]*3);
			glVertex3fv(shadow + indices[idx]*3);
		}
			
		glEnd();
#endif

		/* Just reset counter don't reallocate. */
		indices.clear();
	}
#endif // FIXME
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
	
#if FIXME
	if ( GetObjectMode() == FreyjaControl::tModel )
	{
		switch ( GetCursor().GetMode() )
		{
		case freyja3d::Cursor::Rotation: // About model center ( matrix abuse )
			glRotatef( GetCursor().mRotate.mVec[0], 1,0,0);
			glRotatef( GetCursor().mRotate.mVec[1], 0,1,0);
			glRotatef( GetCursor().mRotate.mVec[2], 0,0,1);
			break;
				
		case freyja3d::Cursor::Translation:
			{
				hel::Vec3 u = (GetCursor().mPos - GetCursor().mLastPos);
				glTranslatef(u.mVec[0], u.mVec[1], u.mVec[2]);
			}
			break;

		default:
			;
		}
	}
#endif

	if (mRenderMode & fShadowVolume)
	{
#if FIXME
		// Render each mesh of this model in turn
		for (uint32 i = 0, count = freyjaGetModelMeshCount(model); i < count; ++i)
		{
			index_t mesh = freyjaGetModelMeshIndex(model, i);
			
			if (freyjaIsMeshAllocated(mesh))
			{
				RenderMesh(mesh);
			}
		}

		for (uint32 i = 0, count = freyjaGetModelMeshCount(model); i < count; ++i)
		{
			index_t mesh = freyjaGetModelMeshIndex(model, i);
			
			if (freyjaIsMeshAllocated(mesh))
			{
				RenderMeshShadowVolume(mesh);
			}
		}
	}
	else
	{
		// Render each mesh of this model in turn
		for (uint32 i = 0, count = freyjaGetModelMeshCount(model); i < count; ++i)
		{
			index_t mesh = freyjaGetModelMeshIndex(model, i);
			
			if (freyjaIsMeshAllocated(mesh))
			{
				RenderMesh(mesh);
			}
		}
#endif
	}


	glPopMatrix();

	/* Disable GLSL shaders */
	freyja3d::OpenGL::BindFragmentGLSL(0);

	/* This handles all skeleton/ctrlpnt setup to avoid repeated 
	 * state changes and clears.
	 */
	//if (mRenderMode & fBones || mRenderMode & fBones2 || 
	// mRenderMode & fBones3 || mRenderMode & fBoneName)
	{
		glPushAttrib(GL_ENABLE_BIT); //GL_LIGHTING_BIT | GL_TEXTURE_BIT
		glDisable(GL_BLEND);

		if (mRenderMode & fBonesNoZbuffer)
		{
			glClear(GL_DEPTH_BUFFER_BIT);
		}

		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		
		glBindTexture(GL_TEXTURE_2D, 0); // 'Color' TextureId
	}

#if FIXME
	{
		Vector<hel::Vec3> ctrlpnts = GetControlPoints();

		glColor3fv(RED);

		uint32 i;
		foreach (ctrlpnts, i)
		{
			hel::Vec3 p = ctrlpnts[i];

			glPushMatrix();
			glTranslatef(p.mVec[0], p.mVec[1], p.mVec[2]);
			mglDrawControlPoint();
			glPopMatrix();
		}

		if (mRenderMode & fBoundingVolSelection && ctrlpnts.size() == 2)
		{ 
			hel::Vec3 min = ctrlpnts[0];
			hel::Vec3 max = ctrlpnts[1];
			mglDrawSelectionBox(min.mVec, max.mVec, DARK_YELLOW);
		}
	}
#endif

#if FIXME
	/* Render 'old' skeleton, which is comprised of z,y,x rotations. */
	if (mRenderMode & fBones)
	{
		FreyjaRender::mSelectedBone = FreyjaControl::GetInstance()->GetSelectedBone();
		RenderSkeleton(freyjaGetModelSkeleton(model), 0, 1.0f);
	}

	/* Point type setting shows actual bind pose skeleton */
	if ( mRenderMode & fBones2 && !(mRenderMode & fKeyFrameAnimation) )
	{
		hel::Vec3 p, r;

		/* Render spheres and bone geometery */
		for (uint32 i = 0; i < freyjaGetBoneCount(); ++i)
		{
			glPushMatrix();
			glMultMatrixf(freyjaGetBoneBindPose16fv(i));

			(FreyjaControl::GetInstance()->GetSelectedBone() == i) ?
			glColor3fv(FreyjaRender::mColorJointHighlight) : 
			glColor3fv(FreyjaRender::mColorJoint);

			mglDrawControlPoint();

			glPopMatrix();

			index_t parent = freyjaGetBoneParent(i);
			if (parent != INDEX_INVALID)
			{
				glPushMatrix();
				glMultMatrixf( freyjaGetBoneBindPose16fv(parent) );

				(FreyjaControl::GetInstance()->GetSelectedBone() == i) ?
				glColor3fv(FreyjaRender::mColorBoneHighlight) : 
				glColor3fv(FreyjaRender::mColorBone);

				freyjaGetBoneTranslation3fv(i, p.mVec);

				if (FreyjaRender::mBoneRenderType == 2)
				{
					mglDrawBoneSolid(p.mVec);
				}
				else
				{
					mglDrawBone(2, p.mVec);
				}

				glPopMatrix();
			}
		}

		//glPopAttrib();
	}


	/* Render transformed bones */
	if (mRenderMode & fBones2 && mRenderMode & fKeyFrameAnimation ||
		mRenderMode & fBones3 )
	{
		hel::Vec3 p;
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

#if 0
			(FreyjaControl::GetInstance()->GetSelectedBone() == i) ?
			glColor3fv(WHITE) : glColor3fv(DARK_RED);
			mglDrawControlPoint();
#else
			(FreyjaControl::GetInstance()->GetSelectedBone() == i) ?
			glColor3fv(FreyjaRender::mColorJointHighlight) : 
			glColor3fv(FreyjaRender::mColorJoint);
#endif

			glPopMatrix();

			Bone *parent = Bone::GetBone(freyjaGetBoneParent(i));

			if (parent)
			{
				combined = parent->GetWorldPose();

				glPushMatrix();
				glMultMatrixf( combined.mMatrix );

#if 0
				(FreyjaControl::GetInstance()->GetSelectedBone() == i) ?
				glColor3fv(WHITE) : glColor3fv(DARK_GREEN);
#else
				(FreyjaControl::GetInstance()->GetSelectedBone() == i) ?
				glColor3fv(FreyjaRender::mColorBoneHighlight) : 
				glColor3fv(FreyjaRender::mColorBone);
#endif

				freyjaGetBoneTranslation3fv(i, p.mVec);

				if (FreyjaRender::mBoneRenderType == 2)
				{
					mglDrawBoneSolid(p.mVec);
				}
				else
				{
					mglDrawBone(2, p.mVec);
				}

				glPopMatrix();
			}
		}
	}


	/* Render bone names */
	if (mRenderMode & fBoneName)
	{
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
		glDisable(GL_LIGHTING);

		glColor3fv(WHITE);

		hel::Vec3 p;

		for (uint32 i = 0; i < freyjaGetBoneCount(); ++i)
		{
			if (mRenderMode & fBones3)
			{
				freyjaGetBoneWorldPos3fv(i, p.mVec);
			}
			else
			{
				freyjaBoneBindTransformVertex(i, p.mVec, 1.0f);
			}

			// Slightly broken b/c mAngles are not computed from quaternion.
			mPrinter.Print3d(p.mX, p.mY, p.mZ, 
							 -mAngles[2], -mAngles[1], -mAngles[0], 
							 0.05f, 
							 freyjaGetBoneNameString(i));
		}

		
		glBindTexture(GL_TEXTURE_2D, 0);
	}
#endif // FIXME	

	//if (mRenderMode & fBones || mRenderMode & fBones2
	//	|| mRenderMode & fBones3 || mRenderMode & fBoneName)
	{
		glPopAttrib();
	}

	glPopMatrix();
}


void FreyjaRender::RenderSkeleton(index_t skeleton, uint32 bone, vec_t scale)
{
#if FIXME
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
	if (FreyjaControl::GetInstance()->GetControlScheme() == 
		FreyjaControl::eScheme_Animation)
	{
		Bone *b = Bone::GetBone(boneIndex);

		if (b)
		{
			hel::Vec3 p, o;
			uint32 k = FreyjaControl::GetInstance()->GetSelectedKeyFrame();
			uint32 a = FreyjaControl::GetInstance()->GetSelectedAnimation();
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
	 glColor3fv(FreyjaRender::mColorBoneHighlight) : 
	 glColor3fv(FreyjaRender::mColorBone));

	if (FreyjaRender::mBoneRenderType == 2)
	{
		mglDrawBone(3, pos.mVec);		
	}
	else
	{
		mglDrawBone(FreyjaRender::mBoneRenderType, pos.mVec);
	}


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
#endif // FIXME
}


void FreyjaRender::DrawCurveWindow()
{
#if FIXME
	const unsigned int width = GetWindowWidth();
	const unsigned int height = GetWindowHeight();
	const unsigned int end = 175;
	const vec_t end_v = 1.0f / end;
	const vec_t inc_v = 5;

	glPushMatrix();

	mgl2dProjection( width, height );

	/* Background Quad */
	{
		const vec_t x = 0.0f;
		const vec_t y = 0.0f;

		glColor3fv( mColorBackground );
		BindColorTexture();
		glColor3fv( mColorBackground );

		glBegin(GL_QUADS);
		glVertex2f(x, y);
		glVertex2f(x, y + height);
		glVertex2f(x + width, y + height);
		glVertex2f(x + width, y);
		glEnd();
	}


	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glLineWidth(mDefaultLineWidth);
	glPointSize(mVertexPointSize);



#if 0
#else
	/* Skeletal animation, if any... */
	uint32 bone = FreyjaControl::GetInstance()->GetSelectedBone();
	Bone *b = Bone::GetBone(bone);

	// FIXME: Use cursor mode to determine which to 'modify and render here'
	if ( b )
	{
		const vec_t shift = 0.0f;
		uint32 a = FreyjaControl::GetInstance()->GetSelectedAnimation();
		BoneTrack &track = b->GetTrack(a);
		vec_t rateInverse = 1.0f / track.GetRate();
		hel::Vec3 v;

		vec_t yT2 = 20.0f, x = 0.0f, s = 1.0f;

		//glLineWidth(1.5);

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
			glVertex2f(x, yT2);

			for (uint32 i = 0; i < track.GetRotKeyframeCount(); ++i)
			{
				vec_t time = (vec_t)i * rateInverse;
				hel::Vec3 p = track.GetRot(time) * 57.295779513082323f;
				v.mVec[0] = x + i*s;
				v.mVec[1] = yT2 + p.mVec[j];
				glVertex2fv(v.mVec);
				glVertex2fv(v.mVec);
			}

#if 1
			glVertex2f(width, yT2);
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
			glVertex2f(x, yT2+shift);

			for (uint32 i = 0; i < track.GetRotKeyframeCount(); ++i)
			{
				vec_t time = (vec_t)i * rateInverse;
				hel::Vec3 p = track.GetLoc(time) * 2.0f;;
				v.mVec[0] = x + i*s;
				v.mVec[1] = yT2+shift + p.mVec[j];
				glVertex2fv(v.mVec);
				glVertex2fv(v.mVec);
			}

			glVertex2f(width, yT2+shift);
			glEnd();
		}

		/* Display keyframes by time markers. */
		glColor3fv( YELLOW );
		glBegin(GL_LINES);

		for (uint32 i = 0; i < track.GetRotKeyframeCount(); ++i)
		{
			vec_t time = track.GetRotKeyframeTime( i );
			vec_t x = (float)i * end_v * width;
			
			//y = height - y * height;
			glVertex2f( x, 0.0f );
			glVertex2f( x, height );
		}

		glEnd();
#endif
	}
#endif

	/* Lines */
	glColor3fv(mColorWireframe);
	glBegin(GL_LINES);

	for (unsigned int i = 0; i < end; i+=inc_v)
	{
		vec_t x = (float)i * end_v;
		x *= width;

		//y = height - y * height;
		glVertex2f( x, 0.0f );
		glVertex2f( x, height );
	}

	glEnd();


	/* Time marker. */
	{
		glColor3fv(mColorWireframeHighlight);
		glBegin(GL_LINES);
		vec_t x = FreyjaControl::GetInstance()->GetSelectedKeyFrame();

		// 'full coverage' but this is not used for events.
		// x = (x/500)*width;

		// 'matching' which ensures 1:1 mapping of the same value.
		x = x * end_v * width;

		glVertex2f( x, 0.0f );
		glVertex2f( x, height );
		glEnd();
	}

	glPopAttrib();


	/* Text */
	{
		glPushAttrib(GL_ENABLE_BIT);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);

		glColor3fv(WHITE);

		const float text_height = height - height * 0.5f;
		String s;
		vec_t x;

		for (unsigned int i = inc_v; i < end; i+=inc_v)
		{			
			x = (float)i * end_v;
			x *= width;	
			s.Set("%i", i);
			mPrinter.Print2d( x, text_height, 0.33f, s.c_str() );
		}

		glPopAttrib();
	}

	ResizeContext(width, height);
	glPopMatrix();
#endif
}


void FreyjaRender::DrawUVWindow()
{
	unsigned int width = GetWindowWidth();
	unsigned int height = GetWindowHeight();

	glPushMatrix();

	mgl2dProjection(width, height);

	DrawQuad(0.0f, 0.0f, width, height);

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glLineWidth(mDefaultLineWidth);
	glPointSize(mVertexPointSize);

#if FIXME
	Mesh *m = freyjaGetMeshClass(FreyjaControl::GetInstance()->GetSelectedMesh());

	if (m)
	{
		/* Render UVs as points */
		// FIXME: This will do many points for each point, opt later
		//        when we can't have mixed polymaps
		glBegin(GL_POINTS);
		glColor3fv(mColorVertexHighlight);
		index_t selected = freyjaGetCurrentMaterial();//FreyjaControl::GetInstance()->GetSelectedTexture();

		for (uint32 i = 0, n = m->GetFaceCount(); i < n; ++i)
		{
			Face *f = m->GetFace(i);

			if (!f || f->mMaterial != selected)
				continue;

			mstl::Vector<index_t> &indices = 
				(f->mFlags & Face::fPolyMappedTexCoords && f->mTexCoordIndices.size()) ? 
					f->mTexCoordIndices : f->mIndices;

			for (uint32 j = 0, jn = indices.size(); j < jn; ++j)
			{
				hel::Vec3 v;
				m->GetTexCoord(indices[j], v.mVec);

				v.mX *= width;
				v.mY = height - v.mY * height;

				glVertex2fv(v.mVec);
			}
		}

		glEnd();


		// Render wireframe faces	
		for (uint32 i = 0, n = m->GetFaceCount(); i < n; ++i)
		{
			Face *f = m->GetFace(i);
			hel::Vec3 v;
			
			// 0.9.5.13 - We want to always see the wireframe faces for selection!
			if (!f ) //|| f->mMaterial != selected) 
				continue;
			
			/* Render face as wireframe */
			glBegin(GL_LINE_LOOP);

			if (f->mFlags & Face::fSelected)
				glColor3fv(mColorWireframeHighlight);
			else
				glColor3fv(mColorWireframe);
			
			mstl::Vector<index_t> &indices = 
				(f->mFlags & Face::fPolyMappedTexCoords && f->mTexCoordIndices.size()) ? 
					f->mTexCoordIndices : f->mIndices;

			for (uint32 j = 0, jn = indices.size(); j < jn; ++j)
			{
				hel::Vec3 v;
				m->GetTexCoord(indices[j], v.mVec);

				v.mX *= width;
				v.mY = height - v.mY * height;

				glVertex2fv(v.mVec);
			}
			
			glEnd();
		}
	}

#endif // FIXME

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


void FreyjaRender::DrawWindow( freyja_plane_t plane )
{
	switch (plane)
	{
	case DRAW_CAMERA:
		DrawCamWindow();
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
	//glDisable(GL_TEXTURE);
	glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, 0); // color

	if (mRenderMode & fGrid)
		DrawGrid(plane, GetWindowWidth(), GetWindowHeight(), 10);

#if PLANE_NOTIFY_WITH_AXIS
	glPushMatrix();
	glTranslatef(-mScaleEnv + 2.5f, -mScaleEnv + 2.5f, 10.0);

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

	ApplyLights();

	glScalef(mZoom, mZoom, mZoom);

	mglGetOpenGLModelviewMatrix16fv(gModelViewMatrix);

	if (mRenderMode & fDrawPickRay)
	{
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

		hel::Vec3 rayEnd = mTestRay.GetOrigin() + mTestRay.GetDir() * 1000;
		glPointSize(2.0);
		glBegin(GL_POINTS);	
		glColor3fv(GREEN);	
		glVertex3fv(mTestRay.GetOrigin().mVec);
		glVertex3fv(rayEnd.mVec);
		glEnd();
		glBegin(GL_LINES);	
		glColor3fv(YELLOW);	
		glVertex3fv(mTestRay.GetOrigin().mVec);
		glColor3fv(DARK_YELLOW);	
		glVertex3fv(rayEnd.mVec);
		glEnd();
		glPointSize(mDefaultPointSize);

		glPopAttrib();
	}

	/* Draw model geometry, metadata visualizations, and all that good stuff. */
	DrawScene ( gScene );

	DrawIcons();

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
		mPrinter.Print2d(mUpperLeftText[0], mUpperLeftText[1], 0.05f, "Front");
		break;

	case PLANE_BACK:
		mPrinter.Print2d(mUpperLeftText[0], mUpperLeftText[1], 0.05f, "Back");
		break;

	case PLANE_TOP:
		mPrinter.Print2d(mUpperLeftText[0], mUpperLeftText[1], 0.05f, "Top");
		break;

	case PLANE_BOTTOM:
		mPrinter.Print2d(mUpperLeftText[0], mUpperLeftText[1], 0.05f, "Bottom");
		break;

	case PLANE_LEFT:
		mPrinter.Print2d(mUpperLeftText[0], mUpperLeftText[1], 0.05f, "Left");
		break;

	case PLANE_RIGHT:
		mPrinter.Print2d(mUpperLeftText[0], mUpperLeftText[1], 0.05f, "Right");
		break;

	default:
		mPrinter.Print2d(mUpperLeftText[0], mUpperLeftText[1], 0.05f, "Unknown");
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

	{
		vec4_t ambient = { 0.2, 0.2, 0.2, 1.0f };
		vec4_t diffuse = { 0.8, 0.8, 0.8, 1.0f };
		vec4_t specular = { 0.2, 0.2, 0.2, 1.0f };
		glLightfv( GL_LIGHT0, GL_AMBIENT, ambient );
		glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
		glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
	}

	/* Cast light on sphere colored/detailed by material */
	static vec_t dy = 0.0f;
	dy += 0.5f;
	if (dy > 360.0f) dy = 0.0f;
	glPushMatrix();
	glRotatef(dy, 0, 1, 0);
	OpenGLRenderableStrategy::ApplyMaterial( GetSelectedMaterial() );

	// FIXME: Add support for swapping out material mesh.
#if USE_TORUS_TEST
	mglDrawTorus(3.0, 10.0);
#else
	mglDrawSphere(128, 128, 13.0f);
#endif

	glPopMatrix();

	const float x = 21.0f, y = -8.0f, z = 0.0f, w = 16.0f, h = 16.0f, s = 1.001f;

	glPushMatrix();
	glDisable(GL_LIGHTING);

	glColor3fv(BLACK);
	glBegin(GL_LINE_LOOP);
	glVertex3f(x*s, y*s, z);
	glVertex3f(x*s, (y+h)*s, z);
	glVertex3f((x+w)*s, (y+h)*s, z);
	glVertex3f((x+w)*s, y*s, z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(x*s, y*s+h/2, z);
	glVertex3f(8, y*s+h/2, z);
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
	OpenGLRenderableStrategy::ApplyMaterial( MaterialControl::GetInstance()->GetCurrentMaterial() ); //freyjaGetCurrentMaterial());
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
	mPrinter.Print2d(mUpperLeftText[0], mUpperLeftText[1], 0.05f, "Material");
	mPrinter.Print2d(x*s-8, y*s+h+1, 0.04f, "Texture");
	glPopAttrib();
	// End OpenGLPrinter test
}


void FreyjaRender::EvScreenShot()
{
	OpenGL::TakeScreenShot("Freyja", GetWindowWidth(), GetWindowHeight());
}


void FreyjaRender::HandleMotion( mgtk_mouse_event_t* event )
{
	switch (event->button)
	{
	case MOUSE_BTN_MIDDLE:
		{
			const float treshold = 1.0f;
			hel::Vec3 xyz;
			bool xd = true;
			bool yd = true;

			if ( event->x_delta > treshold )
			{
				xyz[0] = 1.0f;
			}
			else if ( event->x_delta < -treshold )
			{
				xyz[0] = -1.0f;
			}
			else
			{
				xd = false;
			}

			if ( event->y_delta > treshold )
			{
				xyz[1] = -1.0f;
			}
			else if ( event->y_delta < -treshold )
			{
				xyz[1] = 1.0f;
			}
			else
			{
				yd = false;
			}

			if ( !xd && !yd )
			{
				return;
			}

			switch ( GetSelectedView( ) )
			{
			case PLANE_BACK:
				mScroll += hel::Vec3(xyz[0], xyz[1], xyz[2]);
				break;
				
			case PLANE_BOTTOM:
				mScroll += hel::Vec3(xyz[0], xyz[2], xyz[1]);
				break;
				
			case PLANE_RIGHT:
				mScroll += hel::Vec3(xyz[2], xyz[1], xyz[0]);
				break;
				
			case PLANE_FRONT: // front, xy
				mScroll += hel::Vec3(xyz[0], xyz[1], xyz[2]);
				break;
				
			case PLANE_TOP: // top, xz
				mScroll += hel::Vec3(xyz[0], xyz[2], xyz[1]);
				break;
				
			case PLANE_LEFT: // left, zy
				mScroll += hel::Vec3(xyz[2], xyz[1], xyz[0]);
				break;
				
			default:
				;
			}
		}
		break;


	case MOUSE_BTN_RIGHT: 
		if ( GetSelectedView() == PLANE_FREE )
		{
			const float treshold = 1.0f;

			if (event->x_delta > treshold)
				Rotate(Y_F, -1.0f);
			else if (event->x_delta < -treshold)
				Rotate(Y_F, 1.0f);
			
			if ( event->y_delta > treshold)
				Rotate(X_F, -1.0f);
			else if (event->y_delta < -treshold)
				Rotate(X_F, 1.0f);
		}
		break;

	default:
		;
	}
}
