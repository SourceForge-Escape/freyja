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

#include "FreyjaRender.h"


#define SCALE_ENV            20.0
#define FAR                  6000.0
#define NEAR                 0.1
#define FOV_Y                40.0

#define GL_LIST_AXIS           1

#define LIGHT_ICON_H          16.0
#define CAMERA_ICON_H         10.0
#define CAMERA_ICON_W          2.0
#define AXIS_ICON_MIN          4.0
#define AXIS_ICON_MID         20.0
#define AXIS_ICON_MAX         15.0



const float RED[]          = {  1.0,  0.0,  0.0, 1.0 };
const float GREEN[]        = {  0.0,  1.0,  0.0, 1.0 };
const float BLUE[]         = {  0.0,  0.0,  1.0, 1.0 };
const float CYAN[]         = {  0.0,  1.0,  1.0, 1.0 };
const float ORANGE[]       = {  1.0,  0.0,  7.0, 1.0 };
const float YELLOW[]       = {  1.0,  1.0,  0.0, 1.0 };
const float BLACK[]        = {  0.0,  0.0,  0.0, 1.0 };
const float WHITE[]        = {  1.0,  1.0,  1.0, 1.0 };
const float NEXT_PURPLE[]  = {  0.3,  0.3,  0.5, 1.0 };


unsigned int gSelectedBone = 0;
unsigned int gBoneRenderType = 1;
unsigned char gJointRenderType = 3;


// Just for testing for now
void setup_lights_test()
{
	static bool init = false;
	
	
	if (init)
		return;
	
	init = true;
	glEnable(GL_LIGHT0);
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


// Based on draw_sphere (public domain), 1995, David G Yu 
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


void mglDrawGrid(vec_t size, vec_t step, vec_t scale)
{
	vec_t x, z;


	/* Draw grid without using GL Scaling, which is 'teh bad' */
	glPushMatrix();
	glColor3f(0.4f, 0.4f, 0.6f);

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
		float *test = _model->GetLight0Pos();
		
		glPushMatrix();
		glTranslated(test[0], test[1], test[2]);
		glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
		mglDrawSphere(32, 32, 2.5f);
		glPopMatrix();
		
		glEnable(GL_LIGHTING);
		glLightfv(GL_LIGHT0, GL_POSITION, _model->GetLight0Pos());
	}
}


void drawSkeleton(Vector<egg_tag_t *> *taglist, 
				  unsigned int currentBone, vec_t scale)
{
	egg_tag_t *tag, *tag2;
	unsigned int i, j, index;


	if (taglist->empty())
		return;

	for (i = taglist->begin(); i < taglist->end(); ++i)
	{
		tag = (*taglist)[i];
		
		if (!tag)
			continue;

		if (i == currentBone)
		{
			glColor3fv(GREEN);
		}
		else
		{
			glColor3fv(WHITE);
		}
		
		mglDrawJoint(gJointRenderType, tag->center);
		
		if (tag->slave.empty())
			continue;

		for (j = tag->slave.begin(); j < tag->slave.end(); ++j)
		{
			index = tag->slave[j];
			tag2 = (*taglist)[index];
			
			if (!tag2)
				continue;

			glBegin(GL_LINES);
			glVertex3f(tag->center[0] * scale, 
					   tag->center[1] * scale,
					   tag->center[2] * scale);
			glVertex3f(tag2->center[0] * scale, 
					   tag2->center[1] * scale,
					   tag2->center[2] * scale);
			glEnd();
		}
	}
}


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
	(gSelectedBone == currentBone) ? glColor3fv(RED) : glColor3fv(GREEN);
	mglDrawJoint(gJointRenderType, pos);

	(gSelectedBone == currentBone) ? glColor3fv(CYAN) : glColor3fv(WHITE);
	mglDrawBone(gBoneRenderType, pos);

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


////////////////////////////////////////////////////////////////


FreyjaRender::FreyjaRender()
{
	_model = NULL;
	mRenderMode = 0;
	_view_mode = VIEWMODE_MODEL_VIEW;
	_init = false;

	// Mongoose 2002.03.07, Init these various things here in contructor now
	Width(640);
	Height(480);
	RotateAmount(1.0);

	CustomColor(COLOR_VIEW_TEXT|COLOR_EDIT_TEXT, 1.0, 1.0, 1.0);
	CustomColor(COLOR_EDIT_POLYGON, 0.2, 0.2, 0.7);
	CustomColor(COLOR_EDIT_LINE, 0.1, 0.1, 0.9);
	CustomColor(COLOR_EDIT_LINE_HIGHLIGHT, 0.2, 0.2, 1.0);
	CustomColor(COLOR_EDIT_VERTEX, 0.2, 1.0, 1.0);
	CustomColor(COLOR_EDIT_VERTEX_HIGHLIGHT, 1.0, 1.0, 0.0);
	CustomColor(COLOR_EDIT_BG, 0.5, 0.5, 0.5);
	CustomColor(COLOR_EDIT_BBOX, 0.2, 0.5, 0.2);
	CustomColor(COLOR_VIEW_BG, 0.3, 0.3, 0.5);
	CustomColor(COLOR_EDIT_GRID_AXIS_X, 0.5, 0.0, 0.0);
	CustomColor(COLOR_EDIT_GRID_AXIS_Y, 0.0, 0.6, 0.0);
	CustomColor(COLOR_EDIT_GRID_AXIS_Z, 0.0, 0.0, 0.5);
	CustomColor(COLOR_EDIT_GRID, 0.3, 0.3, 0.3);

	Reset();
}


FreyjaRender::~FreyjaRender()
{
}


void FreyjaRender::Flags(flags_t flag, int op)
{
	mRenderMode |= flag;

	if (!op)
	{
		mRenderMode ^= flag;
	}
}


unsigned int  FreyjaRender::Flags()
{
	return mRenderMode;
}


void FreyjaRender::Register(FreyjaModel *model)
{
	_model = model;
	// printf("Render> Registered data model\n");
}


void FreyjaRender::Init(int width, int height, bool fast_card)
{
	bool arb_multitexture, ext_texture_env_combine;

	_width = width;
	_height = height;
	_fast_card = fast_card;

	// Print driver support information
	event_print("[GL Driver Info]");
	event_print("\tVendor     : %s", glGetString(GL_VENDOR));
	event_print("\tRenderer   : %s", glGetString(GL_RENDERER));
	event_print("\tVersion    : %s", glGetString(GL_VERSION));
	event_print("\tExtensions : %s", (char*)glGetString(GL_EXTENSIONS));

	// Testing for goodies
	arb_multitexture = gl_ext_check("GL_ARB_multitexture");
	ext_texture_env_combine = gl_ext_check("GL_EXT_texture_env_combine");

	event_print("\tGL_ARB_multitexture       \t\t[%s]",
			 arb_multitexture ? "YES" : "NO");

	event_print("\tGL_EXT_texture_env_combine\t\t[%s]",
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

	_angles[0] = _angles[1] = _angles[2] = 0.0;

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


void FreyjaRender::EditTexture(int n)
{
	if (n < 0)
	{
		return;
	}

	_model->setCurrentTextureIndex(n);
	event_print("Texture[%i] selected", _model->getCurrentTextureIndex());
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


void colorCopyHelper(vec3_t colorSource, vec3_t colorDest)
{
	colorDest[0] = colorSource[0];
	colorDest[1] = colorSource[1];
	colorDest[2] = colorSource[2];
	colorDest[3] = 1.0f;
}


void FreyjaRender::CustomColor(int flags, float r, float g, float b)
{
	vec3_t color;

	color[0] = r;
	color[1] = g;
	color[2] = b;

	if (flags & COLOR_EDIT_POLYGON)
		colorCopyHelper(color, _edit_polygon_color);

	if (flags & COLOR_EDIT_LINE)
		colorCopyHelper(color, _edit_line_color);

	if (flags & COLOR_EDIT_LINE_HIGHLIGHT)
		colorCopyHelper(color, _edit_line_highlight_color);

	if (flags & COLOR_EDIT_GRID_8)
		colorCopyHelper(color,_edit_grid_8_color);

	if (flags & COLOR_EDIT_VERTEX)
		colorCopyHelper(color,_edit_vertex_color);
   
	if (flags & COLOR_EDIT_VERTEX_HIGHLIGHT)
		colorCopyHelper(color,_edit_vertex_highlight_color);

	if (flags & COLOR_EDIT_BBOX)
		colorCopyHelper(color,_edit_bbox_color);
      
	if (flags & COLOR_EDIT_BG)
		colorCopyHelper(color,_edit_bg_color);
       
	if (flags & COLOR_EDIT_GRID)
		colorCopyHelper(color,_edit_grid_color);

	if (flags & COLOR_EDIT_GRID_AXIS_X)
		colorCopyHelper(color,_edit_grid_x_axis_color);

	if (flags & COLOR_EDIT_GRID_AXIS_Y)
		colorCopyHelper(color,_edit_grid_y_axis_color);

	if (flags & COLOR_EDIT_GRID_AXIS_Z)
		colorCopyHelper(color,_edit_grid_z_axis_color);

	if (flags & COLOR_EDIT_TEXT)
		colorCopyHelper(color,_edit_text_color);
       
	if (flags & COLOR_VIEW_BG)
		colorCopyHelper(color,_view_bg_color);
          
	if (flags & COLOR_VIEW_TEXT)
		colorCopyHelper(color,_view_text_color);
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
	
	// Mongoose 2002.02.02, Cache for use in calls from here
	_model->getSceneTranslation(_scroll);
	_zoom = _model->getZoom();
	
	switch (_view_mode)
	{
	case VIEWMODE_MODEL_EDIT:
		glClearColor(_edit_bg_color[0], _edit_bg_color[1], _edit_bg_color[2], 
					 1.0);
		break;
	default:
		glClearColor(_view_bg_color[0], _view_bg_color[1], _view_bg_color[2], 
					 1.0);
	}
	
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glLoadIdentity();
	
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

		if (!(mRenderMode & RENDER_EDIT_GRID))
		{
			mglDrawGrid(50.0f, 2.0f, 1.0f);
			mglDrawAxis(0.25f, 1.2f, 0.872f);
		}

		if (mRenderMode & RENDER_BONES)
		{
			glLineWidth(3.0f);
			gSelectedBone = _model->getCurrentBone();
			drawSkeleton2((_model->CurrentEgg())->TagList(), 0, _zoom);

			glLineWidth(_default_line_width);
		}

		drawLights();
		glPopMatrix();

		glScalef(_zoom, _zoom, _zoom);

		DrawModel(_model->CurrentEgg());
		glPopMatrix();
		break;
	case VIEWMODE_MODEL_EDIT:
		glPushMatrix();
		DrawGrid(Width(), Height(), 8);
		DrawWindow(_model->CurrentPlane());
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
	event_swap_buffers();
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
		if (mRenderMode & RENDER_CAMERA)
		{
			gluPerspective(FOV_Y, ((GLdouble)w)/((GLdouble)h), NEAR*100, FAR*100);
		}
		else if (w <= h) 
		{
			glOrtho(-SCALE_ENV, 
					  SCALE_ENV, -SCALE_ENV * (GLfloat) h / (GLfloat) w, 
					  SCALE_ENV * (GLfloat) h / (GLfloat) w, 
					  -400.0,
					  400.0); 
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


void FreyjaRender::DrawPolygon(egg_polygon_t &polygon)
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
		// Update wireframe color
		if (polygon.id == _model->getCurrentPolygon())
		{
			glColor3fv(RED);    
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
	}


	if (mRenderMode & RENDER_NORMALS)
	{
		glBegin(GL_LINES);
		glColor3f(0.2, 0.2, 0.8);
		
		for(i = polygon.r_vertex.begin(); i < polygon.r_vertex.end(); ++i)
		{
			vertex = polygon.r_vertex[i];

			glVertex3f(vertex->pos[0],
					   vertex->pos[1],
					   vertex->pos[2]);
			glVertex3f(vertex->pos[0] + vertex->norm[0] * 2 * 1/_zoom, 
					   vertex->pos[1] + vertex->norm[1] * 2 * 1/_zoom, 
					   vertex->pos[2] + vertex->norm[2] * 2 * 1/_zoom);
		}
		
		glEnd();
	}


	// Render points
	if (mRenderMode & RENDER_POINTS)
	{
		glColor3fv(CYAN);

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
	Egg *egg = _model->CurrentEgg();
	egg_polygon_t *polygon;
	egg_group_t *grp;
	unsigned int i;


	/* Mongoose 2004.03.26, 
	 * This was here for vertex morph frames, still used? */
	if (mesh.r_polygon.size() != mesh.polygon.size())
	{
		event_print("FreyjaRender::DrawMesh> caching %i polygons, %i cached...",
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
				glVertex3fv(grp->center);
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
		
		if ((int)mesh.id == (int)_model->getCurrentMesh())
			_default_line_width *= 2;
		
		for (i = mesh.r_polygon.begin(); i < mesh.r_polygon.end(); ++i)
		{
			polygon = mesh.r_polygon[i];
			
			if (!polygon)
			{
				printf("FIXME: %s:%i\n", __FILE__, __LINE__);
				continue;
			}
			
			DrawPolygon(*polygon);    
		}
		
		if ((int)mesh.id == (int)_model->getCurrentMesh())
			_default_line_width /= 2;
		
		if (mRenderMode & RENDER_TEXTURE)
		{
			glDisable(GL_TEXTURE_2D);
		}
	}
}


void FreyjaRender::DrawModel(Egg *egg)
{
	Vector<unsigned int> *list = NULL;
	Vector<egg_tag_t *> tag_list;
	egg_tag_t *tag = NULL;
	egg_boneframe_t *boneframe = NULL;
	egg_vertex_t *vertex = NULL;
	vec3_t min, max;
	unsigned int i, j, current_tag;


	if (!egg)
		return;

	current_tag = _model->getCurrentBone();

	glPushMatrix();


	// Selection bbox
	if (mRenderMode & RENDER_BBOX_SEL)
	{
		_model->Bbox(min, max, &list);
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

	boneframe = egg->BoneFrame(_model->getCurrentSkeleton());

	if (boneframe)
	{
		// Make a copy of bone tag list
		for (i = boneframe->tag.begin(); i < boneframe->tag.end(); ++i)
		{
			tag = egg->getTag(boneframe->tag[i]);
		 
			if (!tag)
			{
				printf("Error: No tags to draw!\n");
				continue;
			}

			tag_list.pushBack(tag);
		}
	 
		// Draw skeletal model using tags as meshtree 
		for (i = boneframe->tag.begin(); i < boneframe->tag.end(); ++i)
		{
			tag = egg->getTag(boneframe->tag[i]);

			if (!tag)
			{
				printf("Error: No tags to draw!\n");
				continue;
			}

			if (i == 0)
			{
				glTranslatef(boneframe->center[0], 
								 boneframe->center[1], 
								 boneframe->center[2]);

				if (tag->rot[0])
					glRotatef(tag->rot[0], 1, 0, 0);

				if (tag->rot[1])
					glRotatef(tag->rot[1], 0, 1, 0);
	
				if (tag->rot[2])
					glRotatef(tag->rot[2], 0, 0, 1);
			
				glPushMatrix();
			}
			else
			{
				if (tag->flag & 0x01)
					glPopMatrix();
	
				if (tag->flag & 0x02)
					glPushMatrix();
			
				if (mRenderMode & RENDER_WIREFRAME)
				{
					glColor3fv(WHITE);
					mglDrawBone((tag->id == (int)current_tag) ? 2 : 1, 
								tag->center);
				}

				glTranslatef(tag->center[0], tag->center[1], tag->center[2]);

				/* Mongoose 2004.03.30, 
				 * Was 0 2 1 */
				if (tag->rot[0])
					glRotatef(tag->rot[0], 1, 0, 0);
	
				if (tag->rot[1])
					glRotatef(tag->rot[1], 0, 1, 0);
			
				if (tag->rot[2])
					glRotatef(tag->rot[2], 0, 0, 1);
			}
		
			for (j = tag->mesh.begin(); j < tag->mesh.end(); ++j)
			{
				egg_mesh_t *m = egg->getMesh(tag->mesh[j]);

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
	grp = egg->getGroup(_model->getCurrentGroup());

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

   switch (_model->CurrentPlane())
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

   if (!(mRenderMode & RENDER_EDIT_GRID))
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


void FreyjaRender::DrawWindow(int plane)
{
#ifdef PLANE_NOTIFY_WITH_AXIS
	glPushMatrix();
	glTranslatef(-20.0, -17.0, 10.0);

	switch (plane)
	{
	case Egg::PLANE_XY:
		break;
	case Egg::PLANE_XZ:
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		break;
	case Egg::PLANE_YZ:
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
	case Egg::PLANE_XY:
		glTranslatef(_scroll[0], _scroll[1], 0.0);
		break;
	case Egg::PLANE_XZ:
		glTranslatef(_scroll[0], _scroll[2], 0.0);
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		break;
	case Egg::PLANE_YZ:
		glTranslatef(_scroll[1], _scroll[2], 0.0);
		glRotatef(-90.0, 0.0, 1.0, 0.0);
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		break;
	}

	if (mRenderMode & RENDER_BONES)
	{
		gSelectedBone = _model->getCurrentBone();
		drawSkeleton2((_model->CurrentEgg())->TagList(), 0, _zoom);
	}

	drawLights();

	glScalef(_zoom, _zoom, _zoom);

	DrawModel(_model->CurrentEgg());
}


void FreyjaRender::DrawMaterialEditWindow()
{
	float pos[4] = {128.0, 128.0, 128.0, 1.0};


	/* Setup lighting */
	glEnable(GL_LIGHTING);
	setup_lights_test();	
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
void FreyjaRender::DrawTextureEditWindow(unsigned int width, 
										 unsigned int height)
{
	Egg *egg = _model->CurrentEgg();
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
		
		if (poly->id == _model->getCurrentPolygon())
		{
			glColor3fv(RED);    
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
					glColor3fv(_edit_vertex_highlight_color);
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

void FreyjaRender::getRotation(vec3_t v)
{
	v[0] = _angles[0];
	v[1] = _angles[1];
	v[2] = _angles[2];
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
