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


#include <stdio.h>
#include <string.h>
#include <math.h>

#include <freyja/freyja.h>
#include <freyja/FreyjaPluginABI.h>
#include <hel/math.h>

#include "Cursor.h"
#include "Texture.h"

#include "FreyjaOpenGL.h"


#ifdef OPENGL_EXT_MULTITEXTURE
PFNGLMULTITEXCOORD1FARBPROC glMultiTexCoord1fARB = NULL;
PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB = NULL;
PFNGLMULTITEXCOORD3FARBPROC glMultiTexCoord3fARB = NULL;
PFNGLMULTITEXCOORD4FARBPROC glMultiTexCoord4fARB = NULL;
PFNGLACTIVETEXTUREARBPROC glActiveTextureARB = NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB = NULL;
#endif

using namespace freyja3d;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

OpenGL *OpenGL::mSingleton = NULL;

OpenGL *OpenGL::Instance()
{
	return mSingleton ? mSingleton : new OpenGL();
}

OpenGL::OpenGL() :
	mFlags(fNone),
	mTextureUnitCount(2),
	mMaxLightsCount(2)
{
	// Get hardware info
	glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &mTextureUnitCount);
	glGetIntegerv(GL_MAX_LIGHTS, &mMaxLightsCount);

	// Hook up functions
#ifdef OPENGL_EXT_MULTITEXTURE
	glMultiTexCoord1fARB = (PFNGLMULTITEXCOORD1FARBPROC)mglGetProcAddress("glMultiTexCoord1fARB");
	glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)mglGetProcAddress("glMultiTexCoord2fARB");
	glMultiTexCoord3fARB = (PFNGLMULTITEXCOORD3FARBPROC)mglGetProcAddress("glMultiTexCoord3fARB");
	glMultiTexCoord4fARB = (PFNGLMULTITEXCOORD4FARBPROC)mglGetProcAddress("glMultiTexCoord4fARB");
	glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)mglGetProcAddress("glActiveTextureARB");
	glClientActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC)mglGetProcAddress("glClientActiveTextureARB");
#endif

	mSingleton = this;
}


OpenGL::~OpenGL()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////

const char *gOpenGLExt[] =
{
	"GL_ARB_multitexture",
	"GL_EXT_texture_env_combine",
	"GL_EXT_Cg_shader",
	"GL_ARB_vertex_shader",
	"GL_ARB_shadow",
	"GL_ARB_fragment_shader",
};

void OpenGLContext::Init(uint32 width, uint32 height)
{
	bool arb_multitexture, ext_texture_env_combine;


	/* Log OpenGL driver support information */
	freyja_print("[GL Driver Info]");
	freyja_print("\tVendor     : %s", glGetString(GL_VENDOR));
	freyja_print("\tRenderer   : %s", glGetString(GL_RENDERER));
	freyja_print("\tVersion    : %s", glGetString(GL_VERSION));
	freyja_print("\tExtensions : %s", (char*)glGetString(GL_EXTENSIONS));

	/* Test for extentions */
	if (mglHardwareExtTest("GL_ARB_multitexture"))
		;



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

	// Use some hints
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_FOG_HINT, GL_NICEST);
	glEnable(GL_DITHER);
		
	// AA polygon edges
	//glEnable(GL_POLYGON_SMOOTH);
	//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

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

	mWidth = width;
	mHeight = height;
	mInitContext = true;
}

void OpenGLContext::Resize(uint32 width, uint32 height) 
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

	if (1) // pers
	{
		mNearHeight = 10.0f;
		mNear = 10.0f;
		mFar = 1000.0f;

		glFrustum( -mNearHeight * mAspectRatio, 
					mNearHeight * mAspectRatio,
					-mNearHeight, mNearHeight, 
					mNear,
					mFar );
	}
	else 
	{
		glOrtho(-mScaleEnv * mAspectRatio,
				mScaleEnv * mAspectRatio, 
				-mScaleEnv, mScaleEnv, 
				-400.0, // zNear
				400.0);
	}

	glMatrixMode(GL_MODELVIEW);
}



////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_OPENGL
int runOpenGLUnitTest(int argc, char *argv[])
{
	OpenGL test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[OpenGL class test]\n");

	return runOpenGLUnitTest(argc, argv);
}
#endif



////////////////////////////////////////////////////////////
// mgl utils
////////////////////////////////////////////////////////////

bool mglHardwareExtTest(const char *ext)
{
	bool ret = false;
	
	
	if (strstr((const char*)glGetString(GL_EXTENSIONS), ext))
	{
		ret = true;
	}
	
	return ret;
}


void mgl2dProjection(const unsigned int width, const unsigned int height)
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


void mglDrawSelectBox(const vec3_t min, const vec3_t max, const vec4_t lineColor)
{
	vec_t w = (max[0] - min[0]);
	vec_t h = (max[1] - min[1]);
	vec_t d = (max[2] - min[2]);
	vec_t sw = w * 0.33;
	vec_t sh = h * 0.33;
	vec_t sd = d * 0.33;


	glColor4fv(lineColor);

	//glBegin(GL_POINTS);
	//glVertex3f(max[0], max[1], max[2]);
	//glVertex3f(min[0], min[1], min[2]);
	//glVertex3f(max[0], min[1], max[2]);
	//glVertex3f(min[0], max[1], max[2]);
	//glVertex3f(max[0], max[1], min[2]);
	//glVertex3f(min[0], min[1], max[2]);
	//glVertex3f(min[0], max[1], min[2]);
	//glVertex3f(max[0], min[1], min[2]);
	//glEnd();


	glBegin(GL_LINES);

	// A
	glVertex3f(min[0], max[1], min[2]);
	glVertex3f(min[0] + sw, max[1], min[2]);
	glVertex3f(min[0], max[1], min[2]);
	glVertex3f(min[0], max[1] - sh, min[2]);
	glVertex3f(min[0], max[1], min[2]);
	glVertex3f(min[0], max[1], min[2] + sd);

	// B
	glVertex3f(min[0], max[1], max[2]);
	glVertex3f(min[0] + sw, max[1], max[2]);
	glVertex3f(min[0], max[1], max[2]);
	glVertex3f(min[0], max[1] - sh, max[2]);
	glVertex3f(min[0], max[1], max[2]);
	glVertex3f(min[0], max[1], max[2] -sd);

	// C ( max )
	glVertex3f(max[0], max[1], max[2]);
	glVertex3f(max[0] - sw, max[1], max[2]);
	glVertex3f(max[0], max[1], max[2]);
	glVertex3f(max[0], max[1] - sh, max[2]);
	glVertex3f(max[0], max[1], max[2]);
	glVertex3f(max[0], max[1], max[2] - sd);

	// D
	glVertex3f(max[0], max[1], min[2]);
	glVertex3f(max[0] - sw, max[1], min[2]);
	glVertex3f(max[0], max[1], min[2]);
	glVertex3f(max[0], max[1] - sh, min[2]);
	glVertex3f(max[0], max[1], min[2]);
	glVertex3f(max[0], max[1], min[2] + sd);

	// E ( min )
	glVertex3f(min[0], min[1], min[2]);
	glVertex3f(min[0] + sw, min[1], min[2]);
	glVertex3f(min[0], min[1], min[2]);
	glVertex3f(min[0], min[1] + sh, min[2]);
	glVertex3f(min[0], min[1], min[2]);
	glVertex3f(min[0], min[1], min[2] + sd);

	// F
	glVertex3f(min[0], min[1], max[2]);
	glVertex3f(min[0] + sw, min[1], max[2]);
	glVertex3f(min[0], min[1], max[2]);
	glVertex3f(min[0], min[1] + sh, max[2]);
	glVertex3f(min[0], min[1], max[2]);
	glVertex3f(min[0], min[1], max[2] - sd);

	// G
	glVertex3f(max[0], min[1], max[2]);
	glVertex3f(max[0] - sw, min[1], max[2]);
	glVertex3f(max[0], min[1], max[2]);
	glVertex3f(max[0], min[1] + sh, max[2]);
	glVertex3f(max[0], min[1], max[2]);
	glVertex3f(max[0], min[1], max[2] - sd);
	
	//H
	glVertex3f(max[0], min[1], min[2]);
	glVertex3f(max[0] - sw, min[1], min[2]);
	glVertex3f(max[0], min[1], min[2]);
	glVertex3f(max[0], min[1] + sh, min[2]);
	glVertex3f(max[0], min[1], min[2]);
	glVertex3f(max[0], min[1], min[2] + sd);

	glEnd();
}


// When you just don't want to do things the easy way... manually inlined
void mglDrawAxisWithCones(const vec_t min, const vec_t mid, const vec_t max,
						  uint32 count)
{  
	if (count < 3)
		count = 3;

	const float delta = HEL_DEG_TO_RAD(360.0f) * ( 1.0f / (float)count );
	Vec3 o, point, ith, last;
	vec_t s, t;


	// X Axis, Red
	glColor3fv(RED);
	glBegin(GL_LINES);
	glVertex3f(-mid, 0.0, 0.0);
	glVertex3f(mid,  0.0, 0.0);
	glEnd();

	o = Vec3(mid, 0.0f, 0.0f);
	point = o;
	point.mVec[0] += mid+min;
	glBegin(GL_TRIANGLES);
	helSinCosf((count-1) * delta, &s, &t);
	last = Vec3(o.mVec[0], o.mVec[1] + s, o.mVec[2] + t);

	for (uint32 i = 0; i < count; ++i)
	{ 
		helSinCosf((float)i * delta, &s, &t);

		ith = Vec3(o.mVec[0], o.mVec[1] + s, o.mVec[2] + t);

		/* Base */
		glVertex3fv(o.mVec);
		glVertex3fv(ith.mVec);
		glVertex3fv(last.mVec);

		/* Cone */
		glVertex3fv(point.mVec);
		glVertex3fv(ith.mVec);
		glVertex3fv(last.mVec);

		last = ith;
	}

	glEnd();


	// Y Axis, Green
	glColor3fv(GREEN);
	glBegin(GL_LINES);
	glVertex3f(0.0, -mid, 0.0);
	glVertex3f(0.0, mid, 0.0);
	glEnd();

	o = Vec3(0.0f, mid, 0.0f);
	point = o;
	point.mVec[1] += mid+min;
	glBegin(GL_TRIANGLES);
	helSinCosf((count-1) * delta, &s, &t);
	last = Vec3(o.mVec[0] + s, o.mVec[1], o.mVec[2] + t);

	for (uint32 i = 0; i < count; ++i)
	{ 
		helSinCosf((float)i * delta, &s, &t);

		ith = Vec3(o.mVec[0] + s, o.mVec[1], o.mVec[2] + t);

		/* Base */
		glVertex3fv(o.mVec);
		glVertex3fv(ith.mVec);
		glVertex3fv(last.mVec);

		/* Cone */
		glVertex3fv(point.mVec);
		glVertex3fv(ith.mVec);
		glVertex3fv(last.mVec);

		last = ith;
	}

	glEnd();


	// Z Axis, Blue
	glColor3fv(BLUE);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, -mid);
	glVertex3f(0.0, 0.0, mid);
	glEnd();

	o = Vec3(0.0f, 0.0f, mid);
	point = o;
	point.mVec[2] += mid+min;
	glBegin(GL_TRIANGLES);
	helSinCosf((count-1) * delta, &s, &t);
	last = Vec3(o.mVec[0] + s, o.mVec[1] + t, o.mVec[2]);

	for (uint32 i = 0; i < count; ++i)
	{ 
		helSinCosf((float)i * delta, &s, &t);

		ith = Vec3(o.mVec[0] + s, o.mVec[1] + t, o.mVec[2]);

		/* Base */
		glVertex3fv(o.mVec);
		glVertex3fv(ith.mVec);
		glVertex3fv(last.mVec);

		/* Cone */
		glVertex3fv(point.mVec);
		glVertex3fv(ith.mVec);
		glVertex3fv(last.mVec);

		last = ith;
	}

	glEnd();
}


void mglDrawEditorAxis()
{
	static int drawList = -1;

	if (drawList == -1)
	{
		const vec_t min = 0.5f, mid = 2.4f, max = 1.744f;
		//const vec_t min = 0.5f, mid = 1.2f, max = 0.872f;
		vec_t scale = 0.5f;
		drawList = glGenLists(1);
		glNewList(drawList, GL_COMPILE);
		glPushMatrix();
		glScalef(scale, scale, scale);
		mglDrawAxisWithCones(min, mid, max, 8);
		glPopMatrix();
		glEndList();
	}
	else
	{
		glCallList(drawList);
	}
}


void mglDrawAxisWithLines(const vec_t min, const vec_t mid, const vec_t max)
{
	glBegin(GL_LINES);
      
	// X Axis, red
	glColor3fv(RED);
	glVertex3f(-mid, 0.0, 0.0);
	glVertex3f(mid,  0.0, 0.0);
	//  Y arrowhead
	glVertex3f(mid,  0.0, 0.0);
	glVertex3f(max,  min, 0.0);
	glVertex3f(mid,  0.0, 0.0);
	glVertex3f(max, -min, 0.0);
	//  Z arrowhead
	glVertex3f(mid,  0.0, 0.0);
	glVertex3f(max,  0.0, min);
	glVertex3f(mid,  0.0, 0.0);
	glVertex3f(max,  0.0, -min);


	// Y Axis, green
	glColor3fv(GREEN);	
	glVertex3f(0.0,  mid, 0.0);
	glVertex3f(0.0, -mid, 0.0);	
	//  X arrowhead		
	glVertex3f(0.0,  mid, 0.0);
	glVertex3f(min,  max, 0.0);
	glVertex3f(0.0,  mid, 0.0);
	glVertex3f(-min, max, 0.0);
	//  Z arrowhead
	glVertex3f(0.0,  mid, 0.0);
	glVertex3f(0.0,  max, min);
	glVertex3f(0.0,  mid, 0.0);
	glVertex3f(0.0,  max, -min);

      
	// Z Axis, blue
	glColor3fv(BLUE);
	glVertex3f(0.0,  0.0,  mid);
	glVertex3f(0.0,  0.0, -mid);
	//  Y arrowhead
	glVertex3f(0.0,  0.0,  mid);
	glVertex3f(0.0,  min,  max);
	glVertex3f(0.0,  0.0,  mid);
	glVertex3f(0.0, -min,  max);
	//  X arrowhead
	glVertex3f(0.0,  0.0,  mid);
	glVertex3f(min,  0.0,  max);
	glVertex3f(0.0,  0.0,  mid);
	glVertex3f(-min, 0.0,  max);
	glEnd();
}


void mglDrawCone(vec3_t origin, vec_t height, uint32 count)
{
	Vec3 point(origin[0], origin[1] + height, origin[2]);
	Vec3 center(origin[0], origin[1], origin[2]);
	vec_t x, z;
	const float delta = HEL_DEG_TO_RAD(360.0f) * ( 1.0f / (float)count );

	
	if (count < 3)
		count = 3;

	glBegin(GL_TRIANGLES);
	
	helSinCosf((count-1) * delta, &x, &z);
	Vec3 ith;
	Vec3 last(origin[0] + x, origin[1], origin[2] + z);

	for (uint32 i = 0; i < count; ++i)
	{ 
		helSinCosf((float)i * delta, &x, &z);

		ith = Vec3(origin[0] + x, origin[1], origin[2] + z);

		/* Base */
		glVertex3fv(center.mVec);
		glVertex3fv(ith.mVec);
		glVertex3fv(last.mVec);

		/* Cone */
		glVertex3fv(point.mVec);
		glVertex3fv(ith.mVec);
		glVertex3fv(last.mVec);

		last = ith;
	}

	glEnd();
}


void mglDraw3dCircle(const vec3_t center, const vec_t radius, uint32 count, 
					 uint32 plane, bool solid)
{
	vec_t fCount = (float)((count < 8) ? 8 : count);
	vec_t x, z, i;

	glBegin(solid ? GL_LINE_LOOP : GL_LINES);

	for (i = 0.0f; i < fCount; ++i)
	{
		helSinCosf(helDegToRad(360.0f * (i / fCount)), &x, &z);

		x *= radius;
		z *= radius;

		switch (plane)
		{
		case 0:
			glVertex3f(center[0] + x, center[1] + z, center[2] + 0.0f);
			break;

		case 1:
			glVertex3f(center[0] + 0.0f, center[1] + x, center[2] + z);
			break;

		default:
			glVertex3f(center[0] + x, center[1] + 0.0f, center[2] + z);
			break;
		}
	}

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
			
			glNormal3f((x * r0) / radius, z0 / radius, (y * r0) / radius);
			glTexCoord2f(j / (GLfloat) numMinor, i / (GLfloat) numMajor);
			glVertex3f(x * r0, z0, y * r0);
			
			glNormal3f((x * r1) / radius, z1 / radius, (y * r1) / radius);
			glTexCoord2f(j / (GLfloat) numMinor, (i + 1) / (GLfloat) numMajor);
			glVertex3f(x * r1, z1, y * r1);
		}
		
		glEnd();
	}
}


void mglDrawBone(unsigned char type, const vec3_t pos)
{
	const vec_t min = 0.05f;
	const vec_t max = 0.50f;


	switch (type)
	{
	case 1:
		glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(pos[0], pos[1], pos[2]);
		glEnd();
		break;

	case 2:
		glBegin(GL_LINE_STRIP);
		glVertex3f(0.0f,   min,  0.0f);    // 0
		glVertex3f(-max,  0.0f, -max);     // 1
		glVertex3f( max,  0.0f, -max);     // 2
		glVertex3fv(pos);                        // Base
		glVertex3f(-max,  0.0f,-max);      // 1
		glVertex3f(-max,  0.0f, max);      // 4
		glVertex3f( 0.0f,  min, 0.0f);     // 0
		glVertex3f( max,  0.0f,-max);      // 2
		glVertex3f( max,  0.0f, max);      // 3
		glVertex3f( 0.0f,  min, 0.0f);     // 0
		glVertex3f(-max,  0.0f, max);      // 4
		glVertex3fv(pos);                        // Base
		glVertex3f( max,  0.0f, max);      // 3
		glVertex3f(-max,  0.0f, max);      // 4
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
		mglDrawEditorAxis();
		break;
	}
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


void mglGetOpenGLViewport(int *viewportXYWH) // int[4]
{
	glGetIntegerv(GL_VIEWPORT, viewportXYWH);
}

void mglGetOpenGLModelviewMatrix(double *modelview) // double[16]
{
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
}


void mglGetOpenGLProjectionMatrix(double *projection) // double[16]
{
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
}



void mglApplyMaterial(uint32 materialIndex)
{
	vec4_t ambient, diffuse, specular, emissive;
	vec_t shininess;
	uint32 flags, texture, texture2, blendSrc, blendDest;


	if (materialIndex > freyjaGetMaterialCount())
	{
		materialIndex = 0;
	}

	flags = freyjaGetMaterialFlags(materialIndex);
	texture = freyjaGetMaterialTexture(materialIndex);
	blendSrc = freyjaGetMaterialBlendSource(materialIndex);
	blendDest = freyjaGetMaterialBlendDestination(materialIndex);
	freyjaGetMaterialAmbient(materialIndex, ambient);
	freyjaGetMaterialDiffuse(materialIndex, diffuse);
	freyjaGetMaterialSpecular(materialIndex, specular);
	freyjaGetMaterialEmissive(materialIndex, emissive);
	shininess = freyjaGetMaterialShininess(materialIndex);

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, emissive);
	glMaterialfv(GL_FRONT, GL_SHININESS, &(shininess));

	// 2006.07.15 - Hey, these should use Texture class binding to
	// make sure the array ids match in the gl texture binding
	if (flags & fFreyjaMaterial_DetailTexture)
	{
		Texture::mSingleton->bindMultiTexture(texture, texture2);
	}
	else if (flags & fFreyjaMaterial_Texture) // Non-colored is ( id + 1)
	{
		glBindTexture(GL_TEXTURE_2D, texture+1);
	}
	else // Colored, first texture is a generated WHITE 32x32
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	if (flags & fFreyjaMaterial_Blending)
	{
		glBlendFunc(blendSrc, blendDest);
		glEnable(GL_BLEND);
	}
	else
	{
		glDisable(GL_BLEND);
	}
}
