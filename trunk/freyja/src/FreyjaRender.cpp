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
#   ifdef MACOSX
#      include <OpenGL/OpenGL.h>
#   else
#      include <GL/gl.h>
#      include <GL/glu.h>
#   endif
#endif

#include "freyja_events.h"
#include "MaterialManager.h"

#include "FreyjaRender.h"

vec4_t FreyjaRender::mColorBackground;
vec4_t FreyjaRender::mColorText;
vec4_t FreyjaRender::mColorWireframe;
vec4_t FreyjaRender::mColorWireframeHighlight;
vec4_t FreyjaRender::mColorBoundingBox;
vec4_t FreyjaRender::mColorGridSeperator;
vec4_t FreyjaRender::mColorGridLine;
vec4_t FreyjaRender::mColorVertex;
vec4_t FreyjaRender::mColorVertexHighlight;
vec4_t FreyjaRender::mColorAxisX;
vec4_t FreyjaRender::mColorAxisY;
vec4_t FreyjaRender::mColorAxisZ;
unsigned int FreyjaRender::mSelectedBone = 0;
unsigned int FreyjaRender::mBoneRenderType = 1;
unsigned char FreyjaRender::mJointRenderType = 1;
int FreyjaRender::mPatchDisplayList = -1;


#define SCALE_ENV   20.0
#define FAR       6000.0
#define NEAR         0.1
#define FOV_Y       40.0

#define GL_LIST_AXIS 1

#define AXIS_ICON_MIN 4.0
#define AXIS_ICON_MID 20.0
#define AXIS_ICON_MAX 15.0

const float RED[]          = {  1.0,  0.0,  0.0, 1.0 };
const float GREEN[]        = {  0.0,  1.0,  0.0, 1.0 };
const float BLUE[]         = {  0.0,  0.0,  1.0, 1.0 };
const float CYAN[]         = {  0.0,  1.0,  1.0, 1.0 };
const float ORANGE[]       = {  1.0,  0.0,  7.0, 1.0 };
const float YELLOW[]       = {  1.0,  1.0,  0.0, 1.0 };
const float BLACK[]        = {  0.0,  0.0,  0.0, 1.0 };
const float WHITE[]        = {  1.0,  1.0,  1.0, 1.0 };
const float NEXT_PURPLE[]  = {  0.3,  0.3,  0.5, 1.0 };
const float NEXT_PURPLE2[] = {  0.4,  0.4,  0.6, 1.0 };


void drawPatch(BezierPatch &patch)
{
	unsigned int i, j;


	glColor3f(1.0f,0.0f,0.0f);
	
	for (i = 0; i < 4; ++i)
	{
		glBegin(GL_LINE_STRIP);

		for (j = 0; j < 4; ++j)
			glVertex3fv(patch.control[i][j].mVec);
		glEnd();
	}

	for (i = 0; i < 4; ++i)
	{
		glBegin(GL_LINE_STRIP);

		for(j = 0; j < 4; ++j)
			glVertex3fv(patch.control[j][i].mVec);
		glEnd();
	}

	glPointSize(4.0);
	glColor3f(0.0f, 1.0f, 0.0f);

	for (i = 0; i < 4; ++i)
	{
		glBegin(GL_POINTS);

		for(j = 0; j < 4; ++j)
			glVertex3fv(patch.control[i][j].mVec);

		glEnd();
	}

	glPointSize(1.0);

	glColor3f(1.0f,1.0f,1.0f);
}

/* Crappy patch list using nehe tut for basis */
unsigned int generate_bezier_patch_list(BezierPatch &patch, int divs)
{
	int u = 0, v;
	vec_t py, px, pyold; 
	GLuint drawlist = glGenLists(1);
	Vector3d temp[4];
	Vector3d *last = new Vector3d[divs+1];  /* Array Of Points To Mark
											   The First Line Of Polygons */

	if (patch.displayList != 0)	   // Get Rid Of Any Old Display Lists
		glDeleteLists(patch.displayList, 1);

	temp[0] = patch.control[0][3];	   // The First Derived Curve (Along X-Axis)
	temp[1] = patch.control[1][3];
	temp[2] = patch.control[2][3];
	temp[3] = patch.control[3][3];

	for (v = 0; v <= divs; ++v)
	{			// Create The First Line Of Points
		px = ((vec_t)v)/((vec_t)divs);		// Percent Along Y-Axis
		/* Use The 4 Points From The Derived Curve To 
		 * Calculate The Points Along That Curve */
		last[v] = patch.solveBernstein(px, temp);
	}

	glNewList(drawlist, GL_COMPILE);				// Start A New Display List
	glBindTexture(GL_TEXTURE_2D, patch.texture);	// Bind The Texture

	for (u = 1; u <= divs; ++u) 
	{
		py    = ((float)u)/((float)divs);			// Percent Along Y-Axis
		pyold = ((float)u-1.0f)/((float)divs);		// Percent Along Old Y Axis

		// Calculate New Bezier Points
		temp[0] = patch.solveBernstein(py, patch.control[0]);
		temp[1] = patch.solveBernstein(py, patch.control[1]);
		temp[2] = patch.solveBernstein(py, patch.control[2]);
		temp[3] = patch.solveBernstein(py, patch.control[3]);

		glBegin(GL_TRIANGLE_STRIP);

		for (v = 0; v <= divs; ++v)
		{
			px = ((float)v)/((float)divs);		// Percent Along The X-Axis

			glTexCoord2f(pyold, px);			// Apply The Old Texture Coords
			glVertex3d(last[v].mVec[0], last[v].mVec[1], last[v].mVec[2]);	// Old Point

			last[v] = patch.solveBernstein(px, temp);	// Generate New Point
			glTexCoord2f(py, px);				// Apply The New Texture Coords
			glVertex3d(last[v].mVec[0], last[v].mVec[1], last[v].mVec[2]);	// New Point
		}

		glEnd();						// END The Triangle Strip
	}
	
	glEndList();						// END The List

	delete [] last;						// Free The Old Vertices Array

	patch.displayList = drawlist;

	return drawlist;					// Return The Display List
}


void test_patch()
{
	FreyjaRender::mPatchDisplayList = 1;
	generate_bezier_patch_list(FreyjaModel::gTestPatch, 7);
}


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
	glColor3fv(RED);
	glVertex3f(-mid, 0.0, 0.0);
	glVertex3f(mid,  0.0, 0.0);
	glVertex3f(mid,  0.0, 0.0);
	glVertex3f(max,  min, 0.0);
	glVertex3f(mid,  0.0, 0.0);
	glVertex3f(max, -min, 0.0);
      
	// Y Axis, green
	glColor3fv(GREEN);	
	glVertex3f(0.0,  mid, 0.0);
	glVertex3f(0.0, -mid, 0.0);			
	glVertex3f(0.0,  mid, 0.0);
	glVertex3f(min,  max, 0.0);
	glVertex3f(0.0,  mid, 0.0);
	glVertex3f(-min, max, 0.0);
      
	// Z Axis, blue
	glColor3fv(BLUE);
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
	if (mRenderMode & RENDER_LIGHTING)
	{
		float *test = _model->mLight0Pos;
		
		glPushMatrix();
		glTranslatef(test[0], test[1], test[2]);
		glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
		mglDrawSphere(16, 16, 0.75f);
		glPopMatrix();
		
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_POSITION, _model->mLight0Pos);
	}
}


#warning FIXME Uses Egg to render skeleton
void drawSkeleton2(Vector<egg_tag_t *> *taglist, 
				   unsigned int currentBone,
				   vec_t scale)
{
	const unsigned char x = 0, y = 1, z = 2;
	const unsigned char xr = 0, yr = 1, zr = 2;
	egg_tag_t *tag, *tag2;
	vec3_t pos;
	unsigned int i, index;


	if (taglist->empty())
		return;

	tag = (*taglist)[currentBone];
	
	if (!tag)
		return;
	
	/* Scale bones to match mesh scaling */
	pos[0] = tag->center[0] * scale; 
	pos[1] = tag->center[1] * scale;
	pos[2] = tag->center[2] * scale;

	/* Render bone and joint */
	(FreyjaRender::mSelectedBone == currentBone) ? glColor3fv(RED) : glColor3fv(GREEN);
	mglDrawJoint(FreyjaRender::mJointRenderType, pos);

	(FreyjaRender::mSelectedBone == currentBone) ? glColor3fv(CYAN) : glColor3fv(WHITE);
	mglDrawBone(FreyjaRender::mBoneRenderType, pos);

	/* Transform children bones */
	glPushMatrix();
	glTranslatef(pos[x], pos[y], pos[z]);
	glRotatef(tag->rot[zr], 0, 0, 1);
	glRotatef(tag->rot[yr], 0, 1, 0);
	glRotatef(tag->rot[xr], 1, 0, 0);

	if (tag->slave.empty())
	{
		glPopMatrix();
		return;
	}

	for (i = tag->slave.begin(); i < tag->slave.end(); ++i)
	{
		index = tag->slave[i];
		tag2 = (*taglist)[index];
		
		if (!tag2)
			continue;

		drawSkeleton2(taglist, index, scale);
	}

	glPopMatrix();
}


void getOpenGLViewport(int *viewportXYWH) // int[4]
{
	glGetIntegerv(GL_VIEWPORT, viewportXYWH);
}

void getOpenGLModelviewMatrix(double *modelview) // double[16]
{
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
}

void getOpenGLProjectionMatrix(double *projection) // double[16]
{
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
}


////////////////////////////////////////////////////////////////


FreyjaRender::FreyjaRender()
{
	_model = NULL;
	mRenderMode = 0 | fRenderBonesClearedZBuffer;
	_view_mode = VIEWMODE_MODEL_VIEW;
	_init = false;

	// Mongoose 2002.03.07, Init these various things here in contructor now
	_width = 640;
	_height = 480;
	RotateAmount(1.0);

	for (long i = 0; i < 3; ++i)
	{
		mColorAxisX[i] = RED[i] * 0.75f;
		mColorAxisY[i] = GREEN[i] * 0.75f;
		mColorAxisZ[i] = BLUE[i] * 0.75f;
	}

	Reset();
}


FreyjaRender::~FreyjaRender()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

unsigned int FreyjaRender::getFlags()
{
	return mRenderMode;
}


void FreyjaRender::getRotation(vec3_t v)
{
	v[0] = _angles[0];
	v[1] = _angles[1];
	v[2] = _angles[2];
}


vec_t FreyjaRender::getWindowAspectRatio()
{
	return mAspectRatio;
}


unsigned int FreyjaRender::getWindowWidth()
{
	return _width;
}


unsigned int FreyjaRender::getWindowHeight()
{
	return _height;
}


float FreyjaRender::getZoom()
{
	return mZoom;
}




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


void FreyjaRender::Register(FreyjaModel *model)
{
	_model = model;
	// freyja_print("Render> Registered data model");
}


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
	glClearColor(BLACK[0], BLACK[1], BLACK[2], BLACK[3]);

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
	glColor3fv(RED);
	glVertex3f(-AXIS_ICON_MID, 0.0, 0.0);
	glVertex3f(AXIS_ICON_MID,  0.0, 0.0);
	glVertex3f(AXIS_ICON_MID,  0.0, 0.0);
	glVertex3f(AXIS_ICON_MAX,  AXIS_ICON_MIN, 0.0);
	glVertex3f(AXIS_ICON_MID,  0.0, 0.0);
	glVertex3f(AXIS_ICON_MAX, -AXIS_ICON_MIN, 0.0);
      
	// Y Axis, green
	glColor3fv(GREEN);	
	glVertex3f(0.0,  AXIS_ICON_MID, 0.0);
	glVertex3f(0.0, -AXIS_ICON_MID, 0.0);			
	glVertex3f(0.0,  AXIS_ICON_MID, 0.0);
	glVertex3f(AXIS_ICON_MIN,  AXIS_ICON_MAX, 0.0);
	glVertex3f(0.0,  AXIS_ICON_MID, 0.0);
	glVertex3f(-AXIS_ICON_MIN, AXIS_ICON_MAX, 0.0);
      
	// Z Axis, blue
	glColor3fv(BLUE);
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
	_bone_line_width = 3.0;
	_bone_point_size = 5.0;
	_default_point_size = 3.5;
	_default_line_width = 1.0;
	_vertex_point_size = 3.5;

	_angles[0] = 18.0f;
	_angles[1] = 42.0f;
	_angles[2] = 0.0f;

	_texture = 0;
}


void FreyjaRender::RotateAmount(float n)
{
	_rotate_amount = n;
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


float FreyjaRender::RotateAmount()
{
	return _rotate_amount;
}


void FreyjaRender::ViewMode(int mode)
{
	_view_mode = mode;
}


void FreyjaRender::drawFreeWindow()
{
	RenderModel model;
	unsigned int i;


	glRotatef(_angles[0], 1.0, 0.0, 0.0);
	glRotatef(_angles[1], 0.0, 1.0, 0.0);
	glRotatef(_angles[2], 0.0, 0.0, 1.0);

	glTranslatef(_scroll[0], _scroll[1], _scroll[2]);

	glPushMatrix();

	glLineWidth(2.0f); 

	if (mRenderMode & RENDER_EDIT_GRID)
	{
		glLineWidth(1.25f);
		mglDrawGrid(mColorGridLine, 50.0f, 2.0f, 1.0f);

		glLineWidth(2.0f);
		mglDrawAxis(0.25f, 1.2f, 0.872f);

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
	}

	glPopMatrix();

	drawLights();
	glScalef(mZoom, mZoom, mZoom);

	for (i = 0; i < _model->getModelCount(); ++i)
	{
		_model->getModel(model, i);
		renderModel(model);
	}
}


void FreyjaRender::Display() 
{ 
	if (!_init)
	{ 
		return;
	}
	
	// Mongoose 2002.02.02, Cache for use in calls from here
	_model->getSceneTranslation(_scroll);
	
	glClearColor(mColorBackground[0], mColorBackground[1], mColorBackground[2], 
				 1.0);
	
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

#define VIEWPORT_TEST
#ifdef VIEWPORT_TEST
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);    
    long width = vp[2] / 2;
    long height = vp[3];
#endif

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glLoadIdentity();
	
	switch (_view_mode)
	{
	case VIEWMODE_MODEL_VIEW:
		glPushMatrix();
		drawFreeWindow();
		glPopMatrix();
		break;

	case VIEWMODE_MODEL_EDIT:
#ifdef VIEWPORT_TEST
		if (mRenderMode & fViewports)
		{
			mViewports[0][0] = 0;
			mViewports[0][1] = 0;
			mViewports[0][2] = width;
			mViewports[0][3] = height/2;

			// Viewport ( front )
			glViewport(0, 0, width, height/2);
			glPushMatrix();
			drawWindow(PLANE_XY);
			glPopMatrix();

			mViewports[1][0] = width;
			mViewports[1][1] = 0;
			mViewports[1][2] = width;
			mViewports[1][3] = height/2;

			// Viewport ( side )
			glViewport(width, 0, width, height/2);
			glPushMatrix();
			drawWindow(PLANE_ZY);
			glPopMatrix();

			mViewports[2][0] = width;
			mViewports[2][1] = height/2;
			mViewports[2][2] = width;
			mViewports[2][3] = height/2;

			// Viewport ( top )
			glViewport(width, height/2, width, height/2);
			glPushMatrix();
			drawWindow(PLANE_XZ);
			glPopMatrix();

			mViewports[3][0] = 0;
			mViewports[3][1] = height/2;
			mViewports[3][2] = width;
			mViewports[3][3] = height/2;

			// Viewport ( free )
			glViewport(0, height/2, width, height/2);
			glPushMatrix();
			drawFreeWindow();
			glPopMatrix();

			glViewport(vp[0], vp[1], vp[2], vp[3]);
		
			glBegin(GL_LINES);
			glColor3fv(BLACK);
			glVertex2f(width, 0);
			glVertex2f(width, height);
			glVertex2f(0, height/2);
			glVertex2f(width, height/2);
			glEnd();
		}
		else
		{
			glPushMatrix();
			drawWindow(_model->CurrentPlane());
			glPopMatrix();
		}
#else
		glPushMatrix();
		drawWindow(_model->CurrentPlane());
		glPopMatrix();
#endif
		break;
	case VIEWMODE_TEXTURE_EDIT:
		DrawTextureEditWindow(getWindowWidth(), getWindowHeight());
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


void FreyjaRender::Reshape(unsigned int w, unsigned int h) 
{ 
	if (!w || !h)
	{
		return;
	}

	_width = w;
	_height = h;
	mAspectRatio = (float)w/(float)h;

	glViewport(0, 0, w, h); 
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 


	switch (_view_mode)
	{
	default:
		if (mRenderMode & RENDER_CAMERA)
		{
			gluPerspective(FOV_Y, 
						   ((GLdouble)w)/((GLdouble)h), 
						   NEAR*100, 
						   FAR*100);
		}
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


	if (mRenderMode & RENDER_TEXTURE && texture != _texture)
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

			_texture = texture;  // First texture is white for colored_poly
			glBindTexture(GL_TEXTURE_2D, texture+1);
		}
	}
}


void FreyjaRender::DrawQuad(float x, float y, float w, float h)
{
	if (!(mRenderMode & RENDER_TEXTURE))
	{
		glDisable(GL_TEXTURE_2D);
		glColor3fv(WHITE); // BLACK
		

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
	}
	else  // if texture draw texture quad
	{
		glColor3fv(WHITE);
		
		glEnable(GL_TEXTURE_2D);
		
		BindTexture(_model->getCurrentTextureIndex()+1);
		
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


void FreyjaRender::renderPolygon(RenderPolygon &face)
{
	static Vector3d u, v;
	static unsigned int i;


	/* Render wireframe */
	if (mRenderMode & RENDER_WIREFRAME)
	{
		// Update wireframe color
		if (mRenderMode & fHightlightPolygonWireframe)
			//polygon.id == _model->getCurrentPolygon())
		{
			glColor3fv(mColorWireframeHighlight);    
		}
		else
		{
			glColor3fv(mColorWireframe);
		}

		glLineWidth(_default_line_width);


		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);

		glBegin(GL_LINE_LOOP);

		for (i = 0; i < face.count; ++i)
		{
			u = face.vertices[i];
			u *= 1.0001;
			glVertex3fv(u.mVec);
		}
	  
		glEnd();

		glPopAttrib();
	}

	// Render normals, remove this for speed -- move up to model or mesh
	if (mRenderMode & RENDER_NORMALS)
	{
		glBegin(GL_LINES);
		glColor3f(0.2, 0.2, 0.8);
	
		for (i = 0; i < face.count; ++i)
		{
			u = face.vertices[i];
			v = face.normals[i];
			v *= (2 * (1 / mZoom));
			v += u;

			glVertex3fv(u.mVec);
			glVertex3fv(v.mVec);
		}
		
		glEnd();
	}

	/* Render face with material, color, or something */
	if (mRenderMode & RENDER_FACE)
	{
		if (mRenderMode & RENDER_TEXTURE)
		{
			glEnable(GL_TEXTURE_2D);
		}

		// Call shader/texture ( no shader support yet )
		if (mRenderMode & RENDER_TEXTURE && face.material != COLORED_POLYGON)
		{
			if (mRenderMode & RENDER_MATERIAL)
			{
				//glPushAttrib(GL_ENABLE_BIT);
				gMaterialManager->applyEffectGL(face.material);
			}

			BindTexture(face.material + 1);
		}
		else
		{
			BindTexture(0);//COLORED_POLYGON);
			glDisable(GL_TEXTURE_2D);
		}

		glColor3fv(WHITE);

		switch (face.count)
		{
		case 1:
			glBegin(GL_POINTS); // error
			break;
		case 2:
			glBegin(GL_LINES); // error
			break;
		case 3:
			glBegin(GL_TRIANGLES);
			break;
		case 4:
			glBegin(GL_QUADS);
			break;
		default:
			glBegin(GL_POLYGON);
		}

		for (i = 0; i < face.count; ++i)
		{
			if (mRenderMode & RENDER_NORMAL)
			{
				glNormal3fv(face.normals[i].mVec);
			}

			if (face.material == COLORED_POLYGON)
			{
				glColor4fv(face.colors[i]);
			}
			else if (mRenderMode & RENDER_TEXTURE)
			{
				glColor3f(1.0, 1.0, 1.0);
				glTexCoord2fv(face.texcoords[i].mVec);
			}
			else
			{
				glColor3fv(WHITE);
				glColor4f(face.texcoords[i].mVec[0], 
						  face.texcoords[i].mVec[1], 0.5, 1.0);
			}

			glVertex3fv(face.vertices[i].mVec);
		}

		glEnd();

		if (mRenderMode & RENDER_MATERIAL)
		{
			//glPopAttrib();
		}
	}
}


void FreyjaRender::renderMesh(RenderMesh &mesh)
{
	RenderPolygon face;
	Vector3d v;
	unsigned int i;


	if (mRenderMode & RENDER_POINTS)
	{
		glPointSize(5.0);
		glBegin(GL_POINTS);

		for (i = mesh.gbegin; i < mesh.gend; ++i)
		{
			v = mesh.getGroupCenter(i);
			glColor3fv(RED);
			glVertex3fv(v.mVec);
		}

		glEnd();
		glPointSize(_default_point_size);
	}

	if (!mesh.polygon->empty())
	{
		if (mRenderMode & RENDER_TEXTURE)
		{
			glEnable(GL_TEXTURE_2D);
		}
		
		if (mRenderMode & RENDER_MATERIAL)
		{
			gMaterialManager->applyEffectGL();
		}
		
		if ((int)mesh.id == (int)_model->getCurrentMesh())
		{
			mRenderMode |= fHightlightPolygonWireframe;
		}

		_default_line_width *= 2;

		// FIXME: Move this back into a method in RenderMesh to avoid
		//        exposing old polygon type ( like getGroupCenter() )
		for (i = mesh.polygon->begin(); i < mesh.polygon->end(); ++i)
		{
			createRenderPolygon(face, *(*mesh.polygon)[i], mesh.frame);
			renderPolygon(face);
		}
		
		if ((int)mesh.id == (int)_model->getCurrentMesh())
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


void FreyjaRender::renderModel(RenderModel &model)
{
	Vector<unsigned int> *list = 0x0;
	RenderMesh rmesh;
	vec3_t min, max;
	vec3_t *xyz;
	unsigned int i, n;


	/* Render patch test -- this should be model specific later */ 
	if (FreyjaRender::mPatchDisplayList > 0)
	{
		glCallList(FreyjaModel::gTestPatch.displayList);
		drawPatch(FreyjaModel::gTestPatch);
	}


	/* Highlight current vertex group
	 * -- this should be model specific later:
	 * eg _model->getCurrentGroup() -> model.index */
	if (mRenderMode & RENDER_POINTS)
	{
		/* Render bounding box */
		_model->getMeshBoundingBox(_model->getCurrentGroup(), min, max);
		renderBox(min, max);

		/* Render actual vertices */
		_model->getMeshVertices(_model->getCurrentGroup(), &list);

		if (list && !list->empty())
		{
			glPointSize(_default_point_size);
			glColor3fv(mColorVertexHighlight);
			glBegin(GL_POINTS);

			for (i = list->begin(), n = list->end(); i < n; ++i)
			{
				xyz = _model->getVertexXYZ((*list)[i]);

				if (xyz)
				{
					glVertex3fv(*xyz);
				}
			}
  
			glEnd();
		}
	}    


	/* Render selected vertices by bounding box
	 *  -- this should be model specific later */
	if (mRenderMode & RENDER_BBOX_SEL)
	{
		_model->getVertexSelection(min, max, &list);

		/* Render bounding box */
		mglDrawBbox(min, max, RED, mColorWireframe);

		/* Render selected vertices */
		if (list && !list->empty())
		{
			glPointSize(_default_point_size);
			glColor3fv(GREEN);
			glBegin(GL_POINTS);
		 
			for (i = list->begin(), n = list->end(); i < n; ++i)
			{
				xyz = _model->getVertexXYZ((*list)[i]);

				if (xyz)
				{
					glVertex3fv(*xyz);
				}
			}
		 
			glEnd();
		}
	}


	/* Render meshes */
	for (i = 0, n = model.getMeshCount(); i < n; ++i)
	{
		model.getMesh(i, rmesh);
		renderMesh(rmesh);
	}


	/* Render skeleton */
	if (mRenderMode & RENDER_BONES)
	{
		/* Yay, let thou bones show through thine meshes 
		 * and it was good for editing */
		if (mRenderMode & fRenderBonesClearedZBuffer)
		{
			glClear(GL_DEPTH_BUFFER_BIT);
		}

		FreyjaRender::mSelectedBone = _model->getCurrentBone();
		drawSkeleton2(model.getSkeleton(), 0, mZoom);
	}

	glPopMatrix();
}


void FreyjaRender::renderBox(vec3_t min, vec3_t max)
{
	//if (mRenderMode & RENDER_BBOX)
	mglDrawBbox(min, max, RED, mColorWireframe);
}


void FreyjaRender::DrawGrid(freyja_plane_t plane, int w, int h, int size)
{
	static int x, y, offset_x, offset_y;


   glPushMatrix();
   glLineWidth(2.0);

   switch (plane)
   {
   case PLANE_XY:
		x = (int)_scroll[0];
		y = (int)_scroll[1];

		glBegin(GL_LINES); 
		glColor3fv(FreyjaRender::mColorAxisX);
		glVertex2i(-w, y);
		glVertex2i(w, y);
    
		glColor3fv(FreyjaRender::mColorAxisY);
		glVertex2i(x, -h);
		glVertex2i(x, h);
		glEnd();
		break;


   case PLANE_XZ:
		x = (int)_scroll[0];
		y = (int)_scroll[2];

		glBegin(GL_LINES); 
		glColor3fv(FreyjaRender::mColorAxisX);
		glVertex2i(-w, y);
		glVertex2i(w, y);
    
		glColor3fv(FreyjaRender::mColorAxisZ);
		glVertex2i(x, -h);
		glVertex2i(x, h);
		glEnd();
		break;


   case PLANE_ZY:
	   x = (int)_scroll[2];
	   y = (int)_scroll[1];

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

   if (mRenderMode & RENDER_EDIT_GRID)
   {
		offset_x = (x % size) - w;
		offset_y = (y % size) - h;

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

   glPopMatrix();
}

void FreyjaRender::setZoom(float zoom)
{
	mZoom = zoom;
}


void FreyjaRender::drawWindow(freyja_plane_t plane)
{
	RenderModel model;
	unsigned int i;


	if (mRenderMode & RENDER_EDIT_GRID)
		DrawGrid(plane, getWindowWidth(), getWindowHeight(), 10);

#ifdef PLANE_NOTIFY_WITH_AXIS
	glPushMatrix();
	glTranslatef(-20.0, -17.0, 10.0);

	switch (plane)
	{
	case PLANE_XY:
		break;
	case PLANE_XZ:
		glRotatef(90.0, 1.0, 0.0, 0.0);
		break;
	case PLANE_ZY:
		glRotatef(90.0, 0.0, 1.0, 0.0);
		break;
	}

	glLineWidth(2.0);

	mglDrawAxis(0.5f, 2.4f, 1.744f);
	glPopMatrix();
#endif

	switch (plane)
	{
	case PLANE_XY: // front
		glTranslatef(_scroll[0], _scroll[1], 0.0);
		break;
	case PLANE_XZ: // top
		glTranslatef(_scroll[0], _scroll[2], 0.0);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		break;
	case PLANE_ZY: // side
		glTranslatef(_scroll[2], _scroll[1], 0.0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		break;
	}

	drawLights();

	glScalef(mZoom, mZoom, mZoom);

	for (i = 0; i < _model->getModelCount(); ++i)
	{
		_model->getModel(model, i);
		renderModel(model);
	}
}


void FreyjaRender::DrawMaterialEditWindow()
{
	float pos[4] = {128.0, 128.0, 128.0, 1.0};


	/* Setup lighting */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	/* Setup material */
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
#warning FIXME Uses Egg to generate texture polygons on the fly
void FreyjaRender::DrawTextureEditWindow(unsigned int width, 
										 unsigned int height)
{
	Egg *egg = _model->getCurrentEgg();
	egg_texel_t *texel = NULL;
	egg_polygon_t *poly = NULL;
	float x, y;
	unsigned int i, j;

	glPushMatrix();

	gl_resize_2d(width, height);

	x = 0.0;
	y = 0.0;

	glDisable(GL_TEXTURE_2D);

	egg_mesh_t *meshPtr = NULL;

	meshPtr = egg->getMesh(_model->getCurrentMesh());

	if (!meshPtr)
	{
		DrawQuad(0.0, 0.0, width, height);
		Reshape(width, height);
		glPopMatrix();
		return;
	}

	for (i = _model->mUVMap.begin();  i < _model->mUVMap.end(); ++i)
	{
		poly = egg->getPolygon(_model->mUVMap[i]);
		
		if (!poly)
			continue;

		if (poly->shader != (int)_model->getCurrentTextureIndex())
			continue;

		glLineWidth(_default_line_width);
		glBegin(GL_LINE_LOOP);
		glColor3fv(BLUE);    
		
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
	}

	egg_mesh_t mesh = *meshPtr;
	
	for (i = mesh.polygon.begin(); i < mesh.polygon.end(); ++i)
	{
		poly = egg->getPolygon(mesh.polygon[i]);
		
		if (!poly)
			continue;
		
		if (poly->shader != (int)_model->getCurrentTextureIndex())
			continue;
		
		// Mongoose: Draw texel polygons
		glLineWidth(_default_line_width);
		glBegin(GL_LINE_LOOP);
		
		if (mesh.id == (int)_model->getCurrentMesh())
		{
			if (poly->id == _model->getCurrentPolygon())
				glColor3fv(RED);    
			else
				glColor3fv(CYAN);
		}
		else
		{
			glColor3fv(mColorWireframe);
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

				if ((int)_model->getCurrentTextureIndex() == poly->shader)
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
				
				if ((int)_model->getCurrentTextureIndex() == poly->shader)
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
					glColor3fv(mColorVertexHighlight);
				}
				
				glVertex2f(x, height - y);
			}
			
			glEnd();
		}
	}


	DrawQuad(0.0, 0.0, width, height);
	Reshape(width, height);
	glPopMatrix();
}




/* Mongoose 2004.03.26, 
 * You have to be kidding me 
 * Replace with quarternion/matrix and change API  */
void FreyjaRender::Rotate(float n, int axis)
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
