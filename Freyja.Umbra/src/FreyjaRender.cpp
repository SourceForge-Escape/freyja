/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
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

#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <assert.h>

#ifdef HAVE_OPENGL
#   include <GL/gl.h>
#   include <GL/glu.h>
#endif

#include "freyja_events.h"
#include "MaterialManager.h"
#include "OpenGLFreyjaLight.h"
#include "FreyjaRender.h"


#define SCALE_ENV   20.0
#define FAR       6000.0
#define NEAR         0.1
#define FOV_Y       40.0

#define GL_LIST_AXIS 1

#define AXIS_ICON_MIN 4.0
#define AXIS_ICON_MID 20.0
#define AXIS_ICON_MAX 15.0


unsigned int gSelectedBone = 0;
unsigned int gBoneRenderType = 1;
unsigned char gJointRenderType = 3;


// Just for testing for now
bool gl_ext_check(char *ext)
{
	bool ret = false;
	
	
	if (strstr((char*)glGetString(GL_EXTENSIONS), ext))
	{
		ret = true;
	}
	
	return ret;
}


void gl_resize_2d(int width, int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();  
	glOrtho(0, width, 0, height, -99999, 99999);
	glTranslatef(0.0, 0.0, 99998.0);
	glMatrixMode(GL_MODELVIEW);
}


void mglDrawBbox(const vec3_t min, const vec3_t max, 
				 const vec4_t pointColor, const vec4_t lineColor)
{
	glPointSize(4.0);

	glColor4fv(pointColor);

	glBegin(GL_POINTS);
	glVertex3f(max[0], max[1], max[2]);
	glVertex3f(min[0], min[1], min[2]);
	glVertex3f(max[0], min[1], max[2]);
	glVertex3f(min[0], max[1], max[2]);
	glVertex3f(max[0], max[1], min[2]);
	glVertex3f(min[0], min[1], max[2]);
	glVertex3f(min[0], max[1], min[2]);
	glVertex3f(max[0], min[1], min[2]);
	glEnd();


	glColor4fv(lineColor);

	glBegin(GL_LINES);
	// max, top quad
	glVertex3f(max[0], max[1], max[2]);
	glVertex3f(max[0], min[1], max[2]);
	glVertex3f(max[0], max[1], max[2]);
	glVertex3f(min[0], max[1], max[2]);
	glVertex3f(max[0], max[1], max[2]);
	glVertex3f(max[0], max[1], min[2]);

	// max-min, vertical quads
	glVertex3f(min[0], max[1], max[2]);
	glVertex3f(min[0], max[1], min[2]);
	glVertex3f(max[0], min[1], max[2]);
	glVertex3f(max[0], min[1], min[2]);
	glVertex3f(max[0], min[1], max[2]);
	glVertex3f(min[0], min[1], max[2]);

	// min-max, vertical quads
	glVertex3f(max[0], max[1], min[2]);
	glVertex3f(max[0], min[1], min[2]);
	glVertex3f(max[0], max[1], min[2]);
	glVertex3f(min[0], max[1], min[2]);
	glVertex3f(min[0], max[1], max[2]);
	glVertex3f(min[0], min[1], max[2]);

	// min, bottom quad
	glVertex3f(min[0], min[1], min[2]);
	glVertex3f(min[0], max[1], min[2]);  
	glVertex3f(min[0], min[1], min[2]);
	glVertex3f(max[0], min[1], min[2]);
	glVertex3f(min[0], min[1], min[2]);
	glVertex3f(min[0], min[1], max[2]);
	glEnd();
}


void mglDrawAxis(const vec_t min, const vec_t mid, const vec_t max)
{
	glBegin(GL_LINES);
      
	// X Axis, red
	glColor3fv(OpenGLFreyjaScene::mRed);
	glVertex3f(-mid, 0.0, 0.0);
	glVertex3f(mid,  0.0, 0.0);
	glVertex3f(mid,  0.0, 0.0);
	glVertex3f(max,  min, 0.0);
	glVertex3f(mid,  0.0, 0.0);
	glVertex3f(max, -min, 0.0);
      
	// Y Axis, green
	glColor3fv(OpenGLFreyjaScene::mGreen);	
	glVertex3f(0.0,  mid, 0.0);
	glVertex3f(0.0, -mid, 0.0);			
	glVertex3f(0.0,  mid, 0.0);
	glVertex3f(min,  max, 0.0);
	glVertex3f(0.0,  mid, 0.0);
	glVertex3f(-min, max, 0.0);
      
	// Z Axis, blue
	glColor3fv(OpenGLFreyjaScene::mBlue);
	glVertex3f(0.0,  0.0,  mid);
	glVertex3f(0.0,  0.0, -mid);
	glVertex3f(0.0,  0.0,  mid);
	glVertex3f(0.0,  min,  max);
	glVertex3f(0.0,  0.0,  mid);
	glVertex3f(0.0, -min,  max);
	glEnd();
}


/* Based on draw_sphere (public domain), 1995, David G Yu */
void mglDrawSphere(int numMajor, int numMinor, float radius)
{
	double majorStep = (M_PI / numMajor);
	double minorStep = (2.0 * M_PI / numMinor);
	double a, b, c, r0, r1;
	float z0, z1, x, y;
	int i, j;
	
	
	for (i = 0; i < numMajor; ++i)
	{
		a = i * majorStep;
		b = a + majorStep;
		r0 = radius * sin(a);
		r1 = radius * sin(b);
		z0 = radius * cos(a);
		z1 = radius * cos(b);
		
		glBegin(GL_TRIANGLE_STRIP);
		
		for (j = 0; j <= numMinor; ++j) 
		{
			c = j * minorStep;
			x = cos(c);
			y = sin(c);
			
			glNormal3f((x * r0) / radius, (y * r0) / radius, z0 / radius);
			glTexCoord2f(j / (GLfloat) numMinor, i / (GLfloat) numMajor);
			glVertex3f(x * r0, y * r0, z0);
			
			glNormal3f((x * r1) / radius, (y * r1) / radius, z1 / radius);
			glTexCoord2f(j / (GLfloat) numMinor, (i + 1) / (GLfloat) numMajor);
			glVertex3f(x * r1, y * r1, z1);
		}
		
		glEnd();
	}
}


void mglDrawBone(unsigned char type, const vec3_t pos)
{
	const vec_t min = 0.625f;
	const vec_t max = 2.5f;


	switch (type)
	{
	case 1:
		glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(pos[0], pos[1], pos[2]);
		glEnd();
		break;
	case 2:
		glBegin(GL_LINE_STRIP);

		glVertex3f(0.0,   min,  0.0);     // 0
		glVertex3f(-max,  0.0, -max);     // 1
		glVertex3f( max,  0.0, -max);     // 2
		glVertex3fv(pos);                 // Base
		glVertex3f(-max,  0.0,-max);      // 1
		glVertex3f(-max,  0.0, max);      // 4
		glVertex3f( 0.0,  min, 0.0);      // 0
		glVertex3f( max,  0.0,-max);      // 2
		glVertex3f( max,  0.0, max);      // 3
		glVertex3f( 0.0,  min, 0.0);      // 0
		glVertex3f(-max,  0.0, max);      // 4
		glVertex3fv(pos);                 // Base
		glVertex3f( max,  0.0, max);      // 3
		glVertex3f(-max,  0.0, max);      // 4

		glEnd();
		break;
	}
}


void mglDrawJoint(unsigned char type, const vec3_t pos)
{
	switch (type)
	{
	case 1:
		glBegin(GL_POINTS);
		glVertex3fv(pos);
		glEnd();
		break;
	case 2:
		mglDrawSphere(12, 12, 0.5f);
		break;
	case 3:
		mglDrawAxis(0.25f, 1.2f, 0.872f);
		break;
	}
}


void mglDrawGround(vec3_t color, vec_t size, vec_t step, vec_t scale)
{
	vec_t x, z;


	/* Draw grid without using GL Scaling, which is 'teh bad' */
	glPushMatrix();
	glColor3fv(color);

	for (x = -size; x < size; x += step)
	{
		glBegin(GL_QUADS);
 
		for (z = -size; z < size; z += step)
		{
			glNormal3f(0, 1, 0);
			glTexCoord2f(0, 0);
			glVertex3f((x + step) * scale, 0.0f, z * scale);	
 
			glNormal3f(0, 1, 0);
			glTexCoord2f(0, 1);
			glVertex3f(x * scale, 0.0f, z * scale);	
 
			glNormal3f(0, 1, 0);
			glTexCoord2f(1, 1);
			glVertex3f(x * scale, 0.0f, (z + step) * scale);
 
			glNormal3f(0, 1, 0);
			glTexCoord2f(1, 0);
			glVertex3f((x + step) * scale, 0.0f, (z + step) * scale);
		}
		
		glEnd();
	}

	glPopMatrix();
}


void mglDrawGrid(vec3_t color, vec_t size, vec_t step, vec_t scale)
{
	vec_t x, z;


	/* Draw grid without using GL Scaling, which is 'teh bad' */
	glPushMatrix();
	glColor3fv(color);

	for (x = -size; x < size; x += step)
	{
		glBegin(GL_LINE_LOOP);

		for (z = -size; z < size; z += step)
		{
			glVertex3f((x + step) * scale, 0.0f, z * scale);	
			glVertex3f(x * scale, 0.0f, z * scale);	
			glVertex3f(x * scale, 0.0f, (z + step) * scale);
			glVertex3f((x + step) * scale, 0.0f, (z + step) * scale);
		}

		glEnd();
	}

	glPopMatrix();
}



void FreyjaRender::drawLights()
{
	unsigned int i;

	if (OpenGLFreyjaScene::mRenderMode & OpenGLFreyjaScene::fLighting)
	{
		for (i = mScene->lights.begin(); i < mScene->lights.end(); ++i)
		{
			OpenGLFreyjaLight::renderSymbol(mScene->lights[i]);
		}
	}
}	


void drawSkeleton(Vector<FreyjaBone *> *bonelist, 
				  unsigned int currentBone, vec_t scale)
{
	FreyjaBone *bone, *bone2;
	unsigned int i, j, index;


	if (bonelist->empty())
		return;

	for (i = bonelist->begin(); i < bonelist->end(); ++i)
	{
		bone = (*bonelist)[i];
		
		if (!bone)
			continue;

		if (i == currentBone)
		{
			glColor3fv(OpenGLFreyjaScene::mGreen);
		}
		else
		{
			glColor3fv(OpenGLFreyjaScene::mWhite);
		}
		
		mglDrawJoint(gJointRenderType, bone->position);
		
		if (bone->children.empty())
			continue;

		for (j = bone->children.begin(); j < bone->children.end(); ++j)
		{
			index = bone->children[j];
			bone2 = (*bonelist)[index];
			
			if (!bone2)
				continue; 
			glBegin(GL_LINES);

			glVertex3f(bone->position[0] * scale, 
					   bone->position[1] * scale,
					   bone->position[2] * scale);
			glVertex3f(bone2->position[0] * scale, 
					   bone2->position[1] * scale,
					   bone2->position[2] * scale);
			glEnd();
		}
	}
}


void drawSkeleton2(Vector<FreyjaBone *> *bonelist, 
				   unsigned int currentBone,
				   vec_t scale)
{
	const unsigned char x = 0, y = 1, z = 2;
	const unsigned char xr = 0, yr = 1, zr = 2;
	FreyjaBone *bone, *bone2;
	vec3_t pos;
	vec3_t eular;
	unsigned int i, index;


	if (bonelist->empty())
		return;

	bone = (*bonelist)[currentBone];
	
	if (!bone)
		return;
	
	/* Scale bones to match mesh scaling */
	pos[0] = bone->position[0] * scale; 
	pos[1] = bone->position[1] * scale;
	pos[2] = bone->position[2] * scale;

	/* Render bone and joint */
	(gSelectedBone == currentBone) ? glColor3fv(OpenGLFreyjaScene::mRed) : 
	glColor3fv(OpenGLFreyjaScene::mGreen);
	mglDrawJoint(gJointRenderType, pos);

	(gSelectedBone == currentBone) ? glColor3fv(OpenGLFreyjaScene::mCyan) : 
	glColor3fv(OpenGLFreyjaScene::mWhite);
	mglDrawBone(gBoneRenderType, pos);

	/* Transform children bones */
	glPushMatrix();
	glTranslatef(pos[x], pos[y], pos[z]);
	bone->rotation.getEulerAngles(eular, eular+1, eular+2);
	glRotatef(eular[zr], 0, 0, 1);
	glRotatef(eular[yr], 0, 1, 0);
	glRotatef(eular[xr], 1, 0, 0);

	if (bone->children.empty())
	{
		glPopMatrix();
		return;
	}

	for (i = bone->children.begin(); i < bone->children.end(); ++i)
	{
		index = bone->children[i];
		bone2 = (*bonelist)[index];
		
		if (!bone2)
			continue;

		drawSkeleton2(bonelist, index, scale);
	}

	glPopMatrix();
}


////////////////////////////////////////////////////////////////


FreyjaRender::FreyjaRender(OpenGLFreyjaScene *scene)
{
	mScene = scene;
	mRenderMode = 0;
	_view_mode = VIEWMODE_MODEL_VIEW;
	_init = false;

	// Mongoose 2002.03.07, Init these various things here in contructor now
	Width(640);
	Height(480);
	RotateAmount(1.0);


	Reset();
}


FreyjaRender::~FreyjaRender()
{
}


/*
void FreyjaRender::Flags(flags_t flag, int op)
{
	mRenderMode |= flag;

	if (!op)
	{
		mRenderMode ^= flag;
	}

	if (flag & RENDER_CAMERA && op)
	{
		Reshape(_width, _height);
	}
}


unsigned int  FreyjaRender::Flags()
{
	return mRenderMode;
}
*/


void FreyjaRender::Init(int width, int height, bool fast_card)
{
	bool arb_multitexture, ext_texture_env_combine;

	_width = width;
	_height = height;
	_fast_card = fast_card;

	// Print driver support information
	freyja_print("[GL Driver Info]");
	freyja_print("\tVendor     : %s", glGetString(GL_VENDOR));
	freyja_print("\tRenderer   : %s", glGetString(GL_RENDERER));
	freyja_print("\tVersion    : %s", glGetString(GL_VERSION));
	freyja_print("\tExtensions : %s", (char*)glGetString(GL_EXTENSIONS));

	// Testing for goodies
	arb_multitexture = gl_ext_check("GL_ARB_multitexture");
	ext_texture_env_combine = gl_ext_check("GL_EXT_texture_env_combine");

	freyja_print("\tGL_ARB_multitexture       \t\t[%s]",
			 arb_multitexture ? "YES" : "NO");

	freyja_print("\tGL_EXT_texture_env_combine\t\t[%s]",
			 ext_texture_env_combine ? "YES" : "NO");


	// Set up Z buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Set up culling
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);

	// Set background to black
	glClearColor(OpenGLFreyjaScene::mBlack[0], 
				 OpenGLFreyjaScene::mBlack[1], 
				 OpenGLFreyjaScene::mBlack[2],
				 OpenGLFreyjaScene::mBlack[3]);

	// Disable lighting 
	glDisable(GL_LIGHTING);

	// Setup shading
	glShadeModel(GL_SMOOTH);

	if (fast_card) 
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

	// Axis display list
	glNewList(GL_LIST_AXIS, GL_COMPILE);
	glBegin(GL_LINES);
      
	// X Axis, red
	glColor3fv(OpenGLFreyjaScene::mRed);
	glVertex3f(-AXIS_ICON_MID, 0.0, 0.0);
	glVertex3f(AXIS_ICON_MID,  0.0, 0.0);
	glVertex3f(AXIS_ICON_MID,  0.0, 0.0);
	glVertex3f(AXIS_ICON_MAX,  AXIS_ICON_MIN, 0.0);
	glVertex3f(AXIS_ICON_MID,  0.0, 0.0);
	glVertex3f(AXIS_ICON_MAX, -AXIS_ICON_MIN, 0.0);
      
	// Y Axis, green
	glColor3fv(OpenGLFreyjaScene::mGreen);	
	glVertex3f(0.0,  AXIS_ICON_MID, 0.0);
	glVertex3f(0.0, -AXIS_ICON_MID, 0.0);			
	glVertex3f(0.0,  AXIS_ICON_MID, 0.0);
	glVertex3f(AXIS_ICON_MIN,  AXIS_ICON_MAX, 0.0);
	glVertex3f(0.0,  AXIS_ICON_MID, 0.0);
	glVertex3f(-AXIS_ICON_MIN, AXIS_ICON_MAX, 0.0);
      
	// Z Axis, blue
	glColor3fv(OpenGLFreyjaScene::mBlue);
	glVertex3f(0.0,  0.0,  AXIS_ICON_MID);
	glVertex3f(0.0,  0.0, -AXIS_ICON_MID);
	glVertex3f(0.0,  0.0,  AXIS_ICON_MID);
	glVertex3f(0.0,  AXIS_ICON_MIN,  AXIS_ICON_MAX);
	glVertex3f(0.0,  0.0,  AXIS_ICON_MID);
	glVertex3f(0.0, -AXIS_ICON_MIN,  AXIS_ICON_MAX);
	glEnd();
	glEndList();

	_init = true;
}


void FreyjaRender::Reset()
{
	_angles[0] = 10.0f; _angles[1] = 45.0f; _angles[2] = 0.0f;
	_texture = 0;
}


unsigned int FreyjaRender::Width()
{
	return _width;
}


unsigned int FreyjaRender::Height()
{
	return _height;
}


void FreyjaRender::Width(unsigned int i)
{
	_width = i;
}


void FreyjaRender::Height(unsigned int i)
{
	_height = i;
}


void FreyjaRender::RotateAmount(float n)
{
	_rotate_amount = n;
}


float FreyjaRender::RotateAmount()
{
	return _rotate_amount;
}


void FreyjaRender::ScreenShot()
{
	gMaterialManager->takeScreenshot("Freyja", Width(), Height());
}


void FreyjaRender::ViewMode(int mode)
{
	_view_mode = mode;
}


void FreyjaRender::Display() 
{ 
	if (!_init)
	{ 
		return;
	}
	
	glClearColor(OpenGLFreyjaScene::mColorBackground[0],
				 OpenGLFreyjaScene::mColorBackground[1],
				 OpenGLFreyjaScene::mColorBackground[2],
				 1.0);
	
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glLoadIdentity();

	// Mongoose 2002.02.02, Cache for use in calls from here
	mScene->getSceneTranslation(_scroll);
	
	switch (_view_mode)
	{
	case VIEWMODE_MODEL_VIEW:
		glPushMatrix();
		glRotatef(_angles[0], 1.0, 0.0, 0.0);
		glRotatef(_angles[1], 0.0, 1.0, 0.0);
		glRotatef(_angles[2], 0.0, 0.0, 1.0);

		glTranslatef(_scroll[0], _scroll[1], _scroll[2]);

		glPushMatrix();

		glLineWidth(2.0f); 

		if (OpenGLFreyjaScene::mRenderMode & OpenGLFreyjaScene::fGrid)
		{
			if (OpenGLFreyjaScene::mColorGridLine[3] > 0.9999f)
			{
				// FIXME: if (mEnabled & grid_light)
				//OpenGLFreyjaScene::mMaterialLight.enable();
				mglDrawGround(OpenGLFreyjaScene::mColorGridLine,
							  50.0f, 2.0f, 1.0f);
			}
			else
			{
				mglDrawGrid(OpenGLFreyjaScene::mColorGridLine,
							50.0f, 2.0f, 1.0f);
			}

			mglDrawAxis(0.25f, 1.2f, 0.872f);
		}

		if (OpenGLFreyjaScene::mRenderMode & OpenGLFreyjaScene::fBones)
		{
			//FIXME
			FreyjaSkeleton *skel = 0x0; //mScene->skeletons[mModel->getCurrentSkeleton()];
			if (skel)
			{
				glLineWidth(3.0f);
				gSelectedBone = 0;//mModel->getCurrentBone();
				drawSkeleton2(&(skel->mBones), 0, mZoom);
		  
				glLineWidth(OpenGLFreyjaScene::mLineWidthDefault);
			}
		}

		drawLights();
		glPopMatrix();

		glScalef(mZoom, mZoom, mZoom);

		mScene->render();

		glPopMatrix();
		break;
	case VIEWMODE_MODEL_EDIT:
		glPushMatrix();

		if (!(OpenGLFreyjaScene::mRenderMode & OpenGLFreyjaScene::fGrid))
		{
			//FIXME DrawGrid(Width(), Height(), 8);
		}

		DrawWindow(getCurrentPlane());
		glPopMatrix();
		break;
	case VIEWMODE_TEXTURE_EDIT:
		DrawTextureEditWindow(Width(), Height());
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


unsigned int FreyjaRender::getCurrentPlane()
{
	return mPlane;
}

void FreyjaRender::setCurrentPlane(unsigned int plane)
{
	mPlane = plane;
}


int FreyjaRender::getCurrentTextureIndex()
{
	return mTextureIndex;
}

void FreyjaRender::setCurrentTextureIndex(int index)
{
	mTextureIndex = index;
}


void FreyjaRender::Reshape(unsigned int w, unsigned int h) 
{ 
	if (!w || !h)
	{
		return;
	}

	_aspect_ratio = (float)w/(float)h;

	Width(w);
	Height(h);

	glViewport(0, 0, w, h); 
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 


	switch (_view_mode)
	{
	default:
		if (OpenGLFreyjaScene::mRenderMode & OpenGLFreyjaScene::fCamera)
		{
			gluPerspective(FOV_Y, 
						   ((GLdouble)w)/((GLdouble)h), 
						   NEAR*100, 
						   FAR*100);
		}
// 		else if (w <= h) 
// 		{
// 			glOrtho(-SCALE_ENV, 
// 					SCALE_ENV, -SCALE_ENV * (GLfloat) h / (GLfloat) w, 
// 					SCALE_ENV * (GLfloat) h / (GLfloat) w, 
// 					-400.0,
// 					400.0); 
// 		}
		else 
		{
			glOrtho(-SCALE_ENV * (GLfloat) w / (GLfloat) h, 
					SCALE_ENV * (GLfloat) w / (GLfloat) h, 
					-SCALE_ENV, SCALE_ENV, 
					-400.0, 
					400.0);
		}
	}

	glMatrixMode(GL_MODELVIEW);
}


///////// PRIVATE METHODS ////////////////////////////////


void FreyjaRender::BindTexture(unsigned int texture)
{
	static int hack = 0;

	
	if (OpenGLFreyjaScene::mRenderMode & OpenGLFreyjaScene::fTexture && texture != _texture)
	{
		if ((int)texture == -1)
		{
			hack = -1;
			glDisable(GL_TEXTURE_2D);
		}
		else
		{
			if (hack == -1)
			{
				hack = 0;
				glEnable(GL_TEXTURE_2D);
			}

			_texture = texture;  // First texture is white for colored_poly
			glBindTexture(GL_TEXTURE_2D, texture+1);
		}
	}
}


void FreyjaRender::DrawQuad(float x, float y, float w, float h)
{
	if (!(OpenGLFreyjaScene::mRenderMode & OpenGLFreyjaScene::fTexture))
	{
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_TEXTURE_2D);

		glColor3fv(OpenGLFreyjaScene::mWhite); // OpenGLFreyjaScene::mBlack
		
		glBegin(GL_QUADS);
		glColor3f(0.0, 1.0, 0.5);
		glVertex2f(x, y);
		
		glColor3f(0.0, 0.0, 0.5);
		glVertex2f(x, y+h);
		
		glColor3f(1.0, 0.0, 0.5);
		glVertex2f(x+w, y+h);
		
		glColor3f(1.0, 1.0, 0.5);
		glVertex2f(x+w, y);
		glEnd();

		glPopAttrib();
	}
	else  // if texture draw texture quad
	{
		glColor3fv(OpenGLFreyjaScene::mWhite);
		
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, getCurrentTextureIndex()+1);
		
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
	}
}


#ifdef OBSOLETE
void FreyjaRender::DrawPolygon(FreyjaPolygon &polygon)
{
	egg_vertex_t *vertex;
	egg_texel_t *texel;
	egg_texel_t *texel2;
	float color[4];
	bool external_texel = false;
	unsigned int i, j;


	if (polygon.r_vertex.empty())
	{
		printf("FreyjaRender::DrawPolygon> Assertion failure, bad polygon\n");
		return;
	}

	// Render wireframe
	if (mRenderMode & RENDER_WIREFRAME)
	{
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_TEXTURE_2D);
	   
		// Update wireframe color
		if (mRenderMode & fHightlightPolygonWireframe)
			//polygon.id == mScene->getCurrentPolygon())
		{
			glColor3fv(_edit_line_highlight_color);    
		}
		else
		{
			glColor3fv(_edit_line_color);
		}

		glLineWidth(_default_line_width);

		glBegin(GL_LINE_LOOP);

		for(i = polygon.r_vertex.begin(); i < polygon.r_vertex.end(); ++i)
		{
			vertex = polygon.r_vertex[i];
		  
			if (vertex)
			{
				// Mongoose: 2001.10.28 Scaling replaced by mul ops
				glVertex3f(vertex->pos[0] * 1.0001, 
						   vertex->pos[1] * 1.0001, 
						   vertex->pos[2] * 1.0001);
			}
		}
	  
		glEnd();

		glPopAttrib();
	}


	if (mRenderMode & RENDER_NORMALS)
	{
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_TEXTURE_2D);

		glBegin(GL_LINES);
		glColor3f(0.2, 0.2, 0.8);
		
		for(i = polygon.r_vertex.begin(); i < polygon.r_vertex.end(); ++i)
		{
			vertex = polygon.r_vertex[i];

			glVertex3f(vertex->pos[0],
					   vertex->pos[1],
					   vertex->pos[2]);
			glVertex3f(vertex->pos[0] + vertex->norm[0] * 2 * 1/mZoom, 
					   vertex->pos[1] + vertex->norm[1] * 2 * 1/mZoom, 
					   vertex->pos[2] + vertex->norm[2] * 2 * 1/mZoom);
		}
		
		glEnd();
		glPopAttrib();
	}


	// Render points
	if (mRenderMode & RENDER_POINTS)
	{
		glColor3fv(_edit_vertex_color);

		glBegin(GL_POINTS);
	  
		for (i = polygon.r_vertex.begin(); i < polygon.r_vertex.end(); ++i)
		{
			vertex = polygon.r_vertex[i];
		  
			if (vertex)
			{
				glVertex3fv(vertex->pos);
			}
		}

		glEnd();   
	}


	/* Render face */
	if (mRenderMode & RENDER_FACE)
	{
		// Call shader/texture ( no shader support yet )
		if (mRenderMode & RENDER_TEXTURE && polygon.shader != COLORED_POLYGON)
		{
			if (mRenderMode & RENDER_MATERIAL)
			{
				//glPushAttrib(GL_ENABLE_BIT);
				gMaterialManager->applyEffectGL(polygon.shader);
			}

			BindTexture(polygon.shader+1);
		}
		else
		{
			BindTexture(0);//COLORED_POLYGON);
		}

		glColor3fv(WHITE);

		external_texel = !polygon.r_texel.empty();

		if (!external_texel && polygon.shader == COLORED_POLYGON)
		{
			printf("PolygonDraw> can't draw malformed COLORED_POLYGON\n");
			return;
		}

		switch (polygon.r_vertex.size())
		{
		case 3:
			glBegin(GL_TRIANGLES);
			break;
		case 4:
			glBegin(GL_QUADS);
			break;
		default:
			glBegin(GL_POLYGON);
		}

		if (external_texel)
		{
			for (i = polygon.r_texel.begin(), j = polygon.r_vertex.begin(); 
				  i < polygon.r_texel.end() && 
				  j < polygon.r_vertex.end();
				  ++i, ++j)
			{
				texel = polygon.r_texel[i];
				vertex = polygon.r_vertex[j];

				if (texel && vertex)
				{
					if (mRenderMode & RENDER_NORMAL)
					{
						glNormal3fv(vertex->norm);
					}
				 
					if (polygon.shader == COLORED_POLYGON)
					{
						glDisable(GL_TEXTURE_2D);
						glColor3fv(WHITE);
					 
						++i;
						texel2 = polygon.r_texel[i];
					 
						color[0] = texel->st[0];
						color[1] = texel->st[1];
	  
						if (texel2)
						{
							color[2] = texel2->st[0];
							color[3] = texel2->st[1];
						}
				 
						glColor4fv(color);
					}
					else if (mRenderMode & RENDER_TEXTURE)
					{
						glEnable(GL_TEXTURE_2D);
						glColor3f(1.0, 1.0, 1.0);
						glTexCoord2f(texel->st[0], texel->st[1]);
					}
					else
					{
						glColor3fv(WHITE);
						glColor4f(texel->st[0], texel->st[1], 0.5, 1.0);
					}

					glVertex3fv(vertex->pos);
				}
			}
		}
		else
		{
			for (i = polygon.r_vertex.begin(); i < polygon.r_vertex.end(); ++i)
			{
				vertex = polygon.r_vertex[i];
			 
				if (vertex)
				{
					if (mRenderMode & RENDER_NORMAL)
					{
						glNormal3fv(vertex->norm);
					}
				 
					if (mRenderMode & RENDER_TEXTURE)
					{
						glEnable(GL_TEXTURE_2D);
						glColor3f(1.0, 1.0, 1.0);
						glTexCoord2f(vertex->uv[0], vertex->uv[1]);
					}
					else
					{
						glColor3fv(WHITE);
						glColor4f(vertex->uv[0], vertex->uv[1], 0.5, 1.0);
					}

					glVertex3fv(vertex->pos);
				}
			}
		}
	 
		glEnd();

		if (mRenderMode & RENDER_MATERIAL)
		{
			//glPopAttrib();
		}
	}
}


void FreyjaRender::DrawMesh(egg_mesh_t &mesh)
{
	Egg *egg = mScene->CurrentEgg();
	egg_polygon_t *polygon;
	egg_group_t *grp;
	unsigned int i;


	/* Mongoose 2004.03.26, 
	 * This was here for vertex morph frames, still used? */
	if (mesh.r_polygon.size() != mesh.polygon.size())
	{
		freyja_print("FreyjaRender::DrawMesh> %i polygons, %i cached...",
					mesh.polygon.size(), mesh.r_polygon.size());

		for (i = mesh.polygon.begin(); i < mesh.polygon.end(); ++i)
		{
			polygon = egg->getPolygon(mesh.polygon[i]);
			
			if (polygon)
			{
				mesh.r_polygon.pushBack(polygon);
			}
		}
	}

	if (mRenderMode & RENDER_POINTS)
	{
		glPointSize(5.0);
		glBegin(GL_POINTS);

		for (i = mesh.group.begin(); i < mesh.group.end(); ++i)
		{
			if ((grp = egg->getGroup(mesh.group[i])))
			{
				glColor3fv(RED);
				glVertex3fv(grp->position);
			}
		}

		glEnd();
		glPointSize(_default_point_size);
	}

	if (!mesh.r_polygon.empty())
	{
		if (mRenderMode & RENDER_TEXTURE)
		{
			glEnable(GL_TEXTURE_2D);
		}
		
		if (mRenderMode & RENDER_MATERIAL)
		{
			gMaterialManager->applyEffectGL();
		}
		
		if ((int)mesh.id == (int)mScene->getCurrentMesh())
		{
			mRenderMode |= fHightlightPolygonWireframe;
		}

		_default_line_width *= 2;

		for (i = mesh.r_polygon.begin(); i < mesh.r_polygon.end(); ++i)
		{
			polygon = mesh.r_polygon[i];
			
			if (!polygon)
			{
				freyja_print("FIXME: %s:%i\n", __FILE__, __LINE__);
				continue;
			}
			
			DrawPolygon(*polygon);    
		}
		
		if ((int)mesh.id == (int)mScene->getCurrentMesh())
		{
			mRenderMode ^= fHightlightPolygonWireframe;
		}

		_default_line_width /= 2;
		
		if (mRenderMode & RENDER_TEXTURE)
		{
			glDisable(GL_TEXTURE_2D);
		}
	}
}


void FreyjaRender::DrawModel(FreyjaModel *model)
{
	Vector<unsigned int> *list = NULL;
	Vector<FreyjaBone *> bone_list;
	FreyjaBone *bone = NULL;
	FreyjaSkeleton *skeleton = NULL;
	egg_vertex_t *vertex = NULL;
	vec3_t min, max;
	unsigned int i, j, current_bone;


	if (!egg)
		return;

	current_bone = mScene->getCurrentBone();

	glPushMatrix();


	// Selection bbox
	if (mRenderMode & RENDER_BBOX_SEL)
	{
		mScene->Bbox(min, max, &list);
		mglDrawBbox(min, max, RED, _edit_line_color);

		// Draw bbox selection list
		if (list && !list->empty())
		{
			glPointSize(_default_point_size);
			glBegin(GL_POINTS);
			glColor3fv(GREEN);
		 
			for (i = list->begin(); i < list->end(); ++i)
			{
				vertex = egg->getVertex((*list)[i]);
			 
				if (vertex)
					glVertex3fv(vertex->pos);
			}
		 
			glEnd();
		}
	}

	skeleton = egg->Skeleton(mScene->getCurrentSkeleton());

	if (skeleton)
	{
		// Make a copy of bone bone list
		for (i = skeleton->bone.begin(); i < skeleton->bone.end(); ++i)
		{
			bone = egg->getBone(skeleton->bone[i]);
		 
			if (!bone)
			{
				printf("Error: No bones to draw!\n");
				continue;
			}

			bone_list.pushBack(bone);
		}
	 
		// Draw skeletal model using bones as meshtree 
		for (i = skeleton->bone.begin(); i < skeleton->bone.end(); ++i)
		{
			bone = egg->getBone(skeleton->bone[i]);

			if (!bone)
			{
				printf("Error: No bones to draw!\n");
				continue;
			}

			if (i == 0)
			{
				glTranslatef(skeleton->position[0], 
							 skeleton->position[1], 
							 skeleton->position[2]);

				if (bone->rotation[0])
					glRotatef(bone->rotation[0], 1, 0, 0);

				if (bone->rotation[1])
					glRotatef(bone->rotation[1], 0, 1, 0);
	
				if (bone->rotation[2])
					glRotatef(bone->rotation[2], 0, 0, 1);
			
				glPushMatrix();
			}
			else
			{
				if (bone->flag & 0x01)
					glPopMatrix();
	
				if (bone->flag & 0x02)
					glPushMatrix();
			
				if (mRenderMode & RENDER_WIREFRAME)
				{
					glColor3fv(WHITE);
					mglDrawBone((bone->id == (int)current_bone) ? 2 : 1, 
								bone->position);
				}

				glTranslatef(bone->position[0], bone->position[1], bone->position[2]);

				/* Mongoose 2004.03.30, 
				 * Was 0 2 1 */
				if (bone->rotation[0])
					glRotatef(bone->rotation[0], 1, 0, 0);
	
				if (bone->rotation[1])
					glRotatef(bone->rotation[1], 0, 1, 0);
			
				if (bone->rotation[2])
					glRotatef(bone->rotation[2], 0, 0, 1);
			}
		
			for (j = bone->mesh.begin(); j < bone->mesh.end(); ++j)
			{
				egg_mesh_t *m = egg->getMesh(bone->mesh[j]);

				if (!m)
				{
					printf("FIXME: %s:%i\n", __FILE__, __LINE__);
					continue;
				}

				DrawMesh(*m);
			}
		}

		glPopMatrix();
	 
		glPopMatrix(); // model end
		return;
	}

	//////////////////////////////////////////////////////////////////////////

	egg_group_t *grp = NULL;
	Vector<egg_mesh_t *> *meshlist = NULL;
	egg_mesh_t *mesh = NULL;


	/* Highlight current vertex group */
	grp = egg->getGroup(mScene->getCurrentGroup());

	DrawBbox(grp);

	if (grp && !grp->vertex.empty() && (mRenderMode & RENDER_POINTS))
	{
		// DrawVertex //////////////////////////
		glPointSize(_default_point_size);
		glBegin(GL_POINTS);

		glColor3fv(_edit_vertex_highlight_color);

		for (i = grp->vertex.begin(); i < grp->vertex.end(); ++i)
		{
			vertex = egg->getVertex(grp->vertex[i]);

			if (vertex)
				glVertex3fv(vertex->pos);
		}
  
		glEnd();
		////////////////////////////////////////

		glPointSize(_default_point_size);
	}    


	/* Draw meshes */
	meshlist = egg->MeshList();

	if (meshlist && !meshlist->empty())
	{
		for (i = meshlist->begin(); i < meshlist->end(); ++i)
		{
			mesh = (*meshlist)[i]; 

			if (mesh)
				DrawMesh(*mesh);
		}
	}

	glPopMatrix();
}


void FreyjaRender::DrawBbox(egg_group_t *group)
{
	if (!group)// && mRenderMode & RENDER_BBOX))
		return;

	mglDrawBbox(group->bbox_min, group->bbox_max, RED, _edit_line_color);
}


void FreyjaRender::DrawGrid(int w, int h, int size)
{
	static int x, y, offset_x, offset_y;


   glPushMatrix();
   glLineWidth(2.0);

   switch (mScene->CurrentPlane())
   {
   case Egg::PLANE_XY:
		x = (int)_scroll[0];
		y = (int)_scroll[1];

		glBegin(GL_LINES); 
      glColor3fv(_edit_grid_x_axis_color);
      glVertex2i(-w, y);
      glVertex2i(w, y);
    
      glColor3fv(_edit_grid_y_axis_color);
      glVertex2i(x, -h);
      glVertex2i(x, h);
		glEnd();
		break;
   case Egg::PLANE_XZ:
		x = (int)_scroll[0];
		y = (int)_scroll[2];

		glBegin(GL_LINES); 
      glColor3fv(_edit_grid_x_axis_color);
      glVertex2i(-w, y);
      glVertex2i(w, y);
    
      glColor3fv(_edit_grid_z_axis_color);
      glVertex2i(x, -h);
      glVertex2i(x, h);
		glEnd();
		break;
   case Egg::PLANE_YZ:
		x = (int)_scroll[1];
		y = (int)_scroll[2];

		glBegin(GL_LINES); 
      glColor3fv(_edit_grid_y_axis_color);
      glVertex2i(-w, y);
      glVertex2i(w, y);
    
      glColor3fv(_edit_grid_z_axis_color);
      glVertex2i(x, -h);
      glVertex2i(x, h);
		glEnd(); 
		break;
	default:
		return;
   }

   if (!(OpenGLFreyjaScene::mRenderMode & fGrid))
   {
		glLineWidth(1.0);

		offset_x = (x % size) - w;
		offset_y = (y % size) - h;

		glColor3fv(_edit_grid_color);

		glBegin(GL_LINES);
		for (y = offset_y; y < w; y++)
		{
			if ((y % size) == 0)
			{
				glColor3fv(_edit_grid_8_color);
				glVertex2i(-w, y);
				glVertex2i(w, y);
				glColor3fv(_edit_grid_color);
			}
			else
			{
				glVertex2i(-w, y);
				glVertex2i(w, y);
			}
		}

		for (x = offset_x; x < h; x++)
		{
			glVertex2i(x, -h);
			glVertex2i(x, h);
		}
		glEnd();
   }

   glPopMatrix();
}
#endif


void FreyjaRender::setZoom(float zoom)
{
	mZoom = zoom;
}

float FreyjaRender::getZoom()
{
	return mZoom;
}

void FreyjaRender::DrawWindow(int plane)
{
#ifdef PLANE_NOTIFY_WITH_AXIS
	glPushMatrix();
	glTranslatef(-20.0, -17.0, 10.0);

	switch (plane)
	{
	case PLANE_XY:
		break;
	case PLANE_XZ:
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		break;
	case PLANE_YZ:
		glRotatef(-90.0, 0.0, 1.0, 0.0);
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		break;
	}

	glLineWidth(2.0);

	mglDrawAxis(0.5f, 2.4f, 1.744f);
	glPopMatrix();
#endif

	switch (plane)
	{
	case PLANE_XY:
		glTranslatef(_scroll[0], _scroll[1], 0.0);
		break;
	case PLANE_XZ:
		glTranslatef(_scroll[0], _scroll[2], 0.0);
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		break;
	case PLANE_YZ:
		glTranslatef(_scroll[1], _scroll[2], 0.0);
		glRotatef(-90.0, 0.0, 1.0, 0.0);
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		break;
	}

	if (OpenGLFreyjaScene::mRenderMode & OpenGLFreyjaScene::fBones)
	{
		//FIXME
		FreyjaSkeleton *skel = 0x0;//mScene->skeletons[mScene->getCurrentSkeleton()];

		if (skel)
		{
			gSelectedBone = 0;//mScene->getCurrentBone();
			drawSkeleton2(&(skel->mBones), 0, mZoom);
		}
	}

	drawLights();

	glScalef(mZoom, mZoom, mZoom);
	mScene->renderCurrentModel();
}


void FreyjaRender::DrawMaterialEditWindow()
{
	/* Setup lighting */
	OpenGLFreyjaScene::mMaterialLight.enable();

	/* Apply material */
	gMaterialManager->applyEffectGL();

	/* Cast light on sphere colored/detailed by material */
	mglDrawSphere(128, 128, 10.0);
}


/* Mongoose 2004.03.30, 
 * Dependences of this method:
 * x GL context width and height
 * - Current mesh to skin
 * - Vertex UVs of mesh
 * - External texels of mesh if applicable
 * - OpenGL 2d view helper function
 * - Quad to render skin helper function
 */
void FreyjaRender::DrawTextureEditWindow(unsigned int width,
										 unsigned int height)
{
	//FreyjaMesh *mesh = mScene->meshes[mScene->getCurrentMesh()];
	//unsigned int uvIndex = 0;
	//FreyjaPolygon *polygon = 0x0;
	//FreyjaUVMap *uvMap = 0x0;
	vec_t x, y;
	//unsigned int i, j;

	glPushMatrix();

	gl_resize_2d(width, height);

	x = 0.0;
	y = 0.0;

#ifdef FIXME
	glDisable(GL_TEXTURE_2D);

	egg_mesh_t *meshPtr = NULL;

	meshPtr = egg->getMesh(mScene->getCurrentMesh());

	if (!meshPtr)
	{
		DrawQuad(0.0, 0.0, width, height);
		Reshape(width, height);
		glPopMatrix();
		return;
	}

	egg_mesh_t mesh = *meshPtr;
	
	for (i = mesh.polygon.begin(); i < mesh.polygon.end(); ++i)
	{
		poly = egg->getPolygon(mesh.polygon[i]);
		
		if (!poly)
			continue;
		
		if (poly->shader != (int)mScene->getCurrentTextureIndex())
			continue;
		
		// Mongoose: Draw texel polygons
		glLineWidth(_default_line_width);
		glBegin(GL_LINE_LOOP);
		
		if (mesh.id == (int)mScene->getCurrentMesh())
		{
			if (poly->id == mScene->getCurrentPolygon())
				glColor3fv(RED);    
			else
				glColor3fv(CYAN);
		}
		else
		{
			glColor3fv(_edit_line_color);
		}
		
		/* Mongoose 2004.03.26, 
		 * Use Vertex UV if no external texels */
		if (poly->texel.empty())
		{
			egg_vertex_t *vertex;
			
			for (j = poly->vertex.begin(); j < poly->vertex.end(); ++j)
			{
				vertex = egg->getVertex(poly->vertex[j]);
				
					if (!vertex)
					{
						continue;
					}
					
					x = vertex->uv[0] * width;
					y = vertex->uv[1] * height;
					
					glVertex2f(x, height - y);
			}
			
			glEnd();
		}
		else
		{
			for (j = poly->texel.begin(); j < poly->texel.end(); ++j)
			{
				texel = egg->getTexel(poly->texel[j]);
				
				if (!texel)
				{
					continue;
				}
				
				x = texel->st[0] * width;
				y = texel->st[1] * height;
				
				glVertex2f(x, height - y);
			}
			
			glEnd();
		}
		
		// Mongoose: Draw vertices
		glPointSize(_default_point_size);
		glBegin(GL_POINTS);
		
		if (poly->texel.empty())
		{
			egg_vertex_t *vertex;
			
			for (j = poly->vertex.begin(); j < poly->vertex.end(); ++j)
			{
				vertex = egg->getVertex(poly->vertex[j]);
				
				if (!vertex)
				{
					continue;
				}

				if ((int)mScene->getCurrentTextureIndex() == poly->shader)
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
					glColor3fv(_edit_vertex_highlight_color);
				}
				
				x = vertex->uv[0] * width;
				y = vertex->uv[1] * height;
				
				glVertex2f(x, height - y);
			}
			
			glEnd();
		}
		else
		{
			for (j = poly->texel.begin(); j < poly->texel.end(); ++j)
			{
				texel = egg->getTexel(poly->texel[j]);
					
				if (!texel)
					continue;
				
				x = texel->st[0] * width;
				y = texel->st[1] * height;
				
				if ((int)mScene->getCurrentTextureIndex() == poly->shader)
				{
					switch (j)
					{
					case 0:
						glColor3fv(RED);
						break;
					case 1:
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
					glColor3fv(_edit_vertex_highlight_color);
				}
				
				glVertex2f(x, height - y);
			}
			
			glEnd();
		}
	}
#endif

	DrawQuad(0.0, 0.0, width, height);
	Reshape(width, height);
	glPopMatrix();
}

void FreyjaRender::getRotation(vec3_t v)
{
	v[0] = _angles[0];
	v[1] = _angles[1];
	v[2] = _angles[2];
}

/* Mongoose 2004.03.26, 
 * You have to be kidding me 
 * Replace with quarternion arcball and change API  */
void FreyjaRender::Rotate(int axis, float n)
{
	_angles[axis] += n;
	
	if (_angles[axis] >= 360.0)
	{
		_angles[axis] = 0.0;
	}
	else if (_angles[axis] < 0.0)
	{
		_angles[axis] = 360.0;
	}
}
