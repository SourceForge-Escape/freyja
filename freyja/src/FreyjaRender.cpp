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

#ifdef HAVE_OPENGL
//#   ifdef MACOSX
//#      include <OpenGL/OpenGL.h>
//#   else
#      include <GL/gl.h>
#      include <GL/glu.h>
//#   endif
#endif


#include "freyja_events.h"
#include "FreyjaRender.h"

FreyjaRender *FreyjaRender::mSingleton = 0x0;
extern FreyjaModel *gFreyjaModel;

const float RED[]          = {  1.0,  0.0,  0.0, 1.0 };
const float GREEN[]        = {  0.0,  1.0,  0.0, 1.0 };
const float BLUE[]         = {  0.0,  0.0,  1.0, 1.0 };
const float CYAN[]         = {  0.0,  1.0,  1.0, 1.0 };
const float ORANGE[]       = {  1.0,  7.0,  0.0, 1.0 };
const float YELLOW[]       = {  1.0,  1.0,  0.0, 1.0 };
const float BLACK[]        = {  0.0,  0.0,  0.0, 1.0 };
const float WHITE[]        = {  1.0,  1.0,  1.0, 1.0 };
const float NEXT_PURPLE[]  = {  0.3,  0.3,  0.5, 1.0 };
const float NEXT_PURPLE2[] = {  0.4,  0.4,  0.6, 1.0 };


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
unsigned int  FreyjaRender::mRenderMode = 0;
unsigned int FreyjaRender::mSelectedBone = 0;
unsigned int FreyjaRender::mBoneRenderType = 1;
unsigned char FreyjaRender::mJointRenderType = 1;
int FreyjaRender::mPatchDisplayList = -1;

vec_t FreyjaRender::mBoneLineWidth = 3.0;        /* Custom artifact size */
vec_t FreyjaRender::mBonePointSize = 5.0;
vec_t FreyjaRender::mDefaultPointSize = 3.5;
vec_t FreyjaRender::mDefaultLineWidth = 1.0;
vec_t FreyjaRender::mVertexPointSize = 3.5;


////////////////////////////////////////////////////////////////
// Events
////////////////////////////////////////////////////////////////

void ePointJoint()
{
	FreyjaRender::mJointRenderType = 1;
}

void eSphereJoint()
{
	FreyjaRender::mJointRenderType = 2;
}

void eAxisJoint()
{
	FreyjaRender::mJointRenderType = 3;
}

void eLineBone()
{
	FreyjaRender::mBoneRenderType = 1;
}

void ePolyMeshBone()
{
		FreyjaRender::mBoneRenderType = 2;
}

void eSetNearHeight(vec_t f)
{
	FreyjaRender::mSingleton->setNearHeight(f);
	freyja_event_gl_refresh();
}

void eSetZoomLevel(vec_t f)
{
	FreyjaRender::mSingleton->setNearHeight(f*20.0f);
	freyja_event_gl_refresh();
}

void eRenderToggleBoneZClear()
{
	if (FreyjaRender::mSingleton->getFlags() & FreyjaRender::fRenderBonesClearedZBuffer)
		FreyjaRender::mSingleton->clearFlag(FreyjaRender::fRenderBonesClearedZBuffer);
	else
		FreyjaRender::mSingleton->setFlag(FreyjaRender::fRenderBonesClearedZBuffer);

	freyja_print("Bone rendering with cleared Z buffer [%s]",
				(FreyjaRender::mSingleton->getFlags() & FreyjaRender::fRenderBonesClearedZBuffer) ? "on" : "off");
}

void eRenderToggleGridZClear()
{
	if (FreyjaRender::mSingleton->getFlags() & FreyjaRender::fRenderGridClearedZBuffer)
		FreyjaRender::mSingleton->clearFlag(FreyjaRender::fRenderGridClearedZBuffer);
	else
		FreyjaRender::mSingleton->setFlag(FreyjaRender::fRenderGridClearedZBuffer);

	freyja_print("Grid rendering with cleared Z buffer [%s]",
				(FreyjaRender::mSingleton->getFlags() & FreyjaRender::fRenderGridClearedZBuffer) ? "on" : "off");
}


void FreyjaRenderEventsAttach()
{
	ResourceEventCallback::add("eRenderToggleGridZClear", &eRenderToggleGridZClear);
	ResourceEventCallback::add("eRenderToggleBoneZClear", &eRenderToggleBoneZClear);
	ResourceEventCallback::add("ePolyMeshBone", &ePolyMeshBone);
	ResourceEventCallback::add("eLineBone", &eLineBone);
	ResourceEventCallback::add("eAxisJoint", &eAxisJoint);
	ResourceEventCallback::add("eSphereJoint", &eSphereJoint);
	ResourceEventCallback::add("ePointJoint", &ePointJoint);
	ResourceEventCallbackVec::add("eSetNearHeight", &eSetNearHeight);
	ResourceEventCallbackVec::add("eSetZoomLevel", &eSetZoomLevel);
}


// TEMP ////////////////////////////////////////////////////////

void freyjaApplyMaterial(uint32 materialIndex)
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

	if (flags & fFreyjaMaterial_DetailTexture)
	{
		gFreyjaModel->mTexture.bindMultiTexture(texture, texture2);
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



////////////////////////////////////////////////////////////////


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


void mglDrawAxis(const vec_t min, const vec_t mid, const vec_t max)
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


void mglDraw3dCursorLoc(const vec_t min, const vec_t mid, const vec_t max)
{
	glBegin(GL_LINES);
      
	// X Axis, red
	glColor3fv(RED);
	glVertex3f(0.0,  0.0, 0.0);
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
	glVertex3f(0.0,  0.0, 0.0);	
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
	glVertex3f(0.0,  0.0,  0.0);
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

void mglDraw3dCursor(const vec_t min, const vec_t mid, const vec_t max)
{
	extern Freyja3dCursor gFreyjaCursor;

	glPushMatrix();
	glTranslatef(gFreyjaCursor.mPos.mVec[0], 
				 gFreyjaCursor.mPos.mVec[1], 
				 gFreyjaCursor.mPos.mVec[2]);

	switch (gFreyjaCursor.GetMode())
	{
	case Freyja3dCursor::Translation:
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glClear(GL_DEPTH_BUFFER_BIT);
		mglDraw3dCursorLoc(min, mid, max);
		glPopAttrib();
		break;

	default:
	case Freyja3dCursor::Invisible:
		break;
	}

	glPopMatrix();
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
	mModel = NULL;
	mRenderMode = RENDER_BBOX | fRenderBonesClearedZBuffer;
	mWidth = 640;
	mHeight = 480;
	mViewMode = VIEWMODE_MODEL_VIEW;
	mInitContext = false;
	mAngles[0] = 18.0f;
	mAngles[1] = 42.0f;
	mAngles[2] = 0.0f;
	mTextureId = 0;

	for (long i = 0; i < 3; ++i)
	{
		mColorAxisX[i] = RED[i] * 0.75f;
		mColorAxisY[i] = GREEN[i] * 0.75f;
		mColorAxisZ[i] = BLUE[i] * 0.75f;
	}

	mScaleEnv = 40.0f; //20.0; // 40.0f for higher res
	mFar = 6000.0;
	mNear = 0.1;
	mFovY = 40.0;

	mSingleton = this;
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
	v[0] = mAngles[0];
	v[1] = mAngles[1];
	v[2] = mAngles[2];
}


vec_t FreyjaRender::getWindowAspectRatio()
{
	return mAspectRatio;
}


unsigned int FreyjaRender::getWindowWidth()
{
	return mWidth;
}


unsigned int FreyjaRender::getWindowHeight()
{
	return mHeight;
}


float FreyjaRender::getZoom()
{
	return mZoom;
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


void FreyjaRender::drawFreeWindow()
{
	RenderModel model;
	unsigned int i;


	glRotatef(mAngles[0], 1.0, 0.0, 0.0);
	glRotatef(mAngles[1], 0.0, 1.0, 0.0);
	glRotatef(mAngles[2], 0.0, 0.0, 1.0);

	glTranslatef(mScroll[0], mScroll[1], mScroll[2]);

	//glPushMatrix();
	glLineWidth(2.0f);

	if (mRenderMode & RENDER_EDIT_GRID)
	{
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);

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

		glPopAttrib();
	}
	//glPopMatrix();

	//glPushMatrix();

	renderLights();

	glScalef(mZoom, mZoom, mZoom);

	for (i = 0; i < freyjaGetRenderModelCount(); ++i)
	{
		freyjaGetRenderModel(i, model);
		renderModel(model);
	}

	//glPopMatrix();

	mglDraw3dCursor(0.25f, 2.0f, 0.872f);
}




///////// PRIVATE METHODS ////////////////////////////////

#ifndef COLORED_POLYGON
#   define COLORED_POLYGON -1
#   warning COLORED_POLYGON is obsolete
#endif

void FreyjaRender::BindTexture(unsigned int texture)
{
	static int hack = 0;


	if (mRenderMode & RENDER_TEXTURE && texture != mTextureId)
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
		
		//BindTexture(mModel->getCurrentTextureIndex()+1);
		freyjaApplyMaterial(mModel->getCurrentTextureIndex());

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


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void FreyjaRender::clearFlag(flags_t flag)
{
	mRenderMode |= flag;
	mRenderMode ^= flag;

	if (flag & RENDER_CAMERA) /* Called for on and off */
	{
		resizeContext(mWidth, mHeight);
	}
}


void FreyjaRender::initContext(unsigned int width, unsigned int height, bool fast_card)
{
	bool arb_multitexture, ext_texture_env_combine;


	/* Log OpenGL driver support information */
	freyja_print("[GL Driver Info]");
	freyja_print("\tVendor     : %s", glGetString(GL_VENDOR));
	freyja_print("\tRenderer   : %s", glGetString(GL_RENDERER));
	freyja_print("\tVersion    : %s", glGetString(GL_VERSION));
	freyja_print("\tExtensions : %s", (char*)glGetString(GL_EXTENSIONS));

	/* Test for extentions */
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

	/* Was no longer used... Generate XYZ Axis symbol display list */
	//glNewList(1, GL_COMPILE);
	//mglDrawAxis(0.25f, 1.2f, 0.872f);
	//glEndList();


	mWidth = width;
	mHeight = height;
	mInitContext = true;
}


void FreyjaRender::display() 
{ 
	if (!mInitContext)
	{ 
		return;
	}
	
	// Mongoose 2002.02.02, Cache for use in calls from here
	mModel->getSceneTranslation(mScroll);
	
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
	
	switch (mViewMode)
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

			/* Viewport seperator */
			glColor3fv(BLACK);  // FIXME: hardcoded
			glBegin(GL_LINES);
			glVertex2f(-width*0.25, 19.85);
			glVertex2f(width*0.25, 19.85);
			glVertex2f(21.75, -height*0.25);
			glVertex2f(21.75, height*0.25);
			glEnd();

			drawWindow(PLANE_XY);
			glPopMatrix();

			mViewports[1][0] = width;
			mViewports[1][1] = 0;
			mViewports[1][2] = width;
			mViewports[1][3] = height/2;

			// Viewport ( side )
			glViewport(width, 0, width, height/2);
			glPushMatrix();			/* Viewport seperator */

			glColor3fv(BLACK);  // FIXME: hardcoded
			glBegin(GL_LINES);
			glVertex2f(-width*0.25, 19.85);
			glVertex2f(width*0.25, 19.85);
			glEnd();

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

			/* Viewport seperator */
			glColor3fv(BLACK);  // FIXME: hardcoded
			glBegin(GL_LINES);
			glVertex2f(21.75, -height*0.25);
			glVertex2f(21.75, height*0.25);
			glEnd();

			drawFreeWindow();
			//renderUVWindow();
			glPopMatrix();

			glViewport(vp[0], vp[1], vp[2], vp[3]);
		
			glBegin(GL_LINES);
			glColor3fv(BLACK);
			glVertex2f(width*mZoom, 0);
			glVertex2f(width*mZoom, height*mZoom);
			glVertex2f(0, height*mZoom/2);
			glVertex2f(width, height/2);
			glEnd();
		}
		else
		{
			glPushMatrix();
			drawWindow(mModel->getCurrentPlane());
			glPopMatrix();
		}
#else
		glPushMatrix();
		drawWindow(mModel->getCurrentPlane());
		glPopMatrix();
#endif
		break;
	case VIEWMODE_TEXTURE_EDIT:
		//DrawTextureEditWindow(getWindowWidth(), getWindowHeight());
		renderUVWindow();
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


void FreyjaRender::resizeContext(unsigned int width, unsigned int height) 
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

	if (mRenderMode & RENDER_CAMERA)
	{
		gluPerspective(mFovY, mAspectRatio, mNear * 100, mFar * 100);
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


void FreyjaRender::setFlag(flags_t flag)
{
	mRenderMode |= flag;

	if (flag & RENDER_CAMERA) /* Called for on and off */
	{
		resizeContext(mWidth, mHeight);
	}
}


void FreyjaRender::setSceneData(FreyjaModel *model)
{
	mModel = model;
}


void FreyjaRender::setViewMode(int mode)
{
	mViewMode = mode;
}


void FreyjaRender::setZoom(float zoom)
{
	mZoom = zoom;
}


void FreyjaRender::toggleFlag(flags_t flag)
{
	mRenderMode ^= flag;

	if (flag & RENDER_CAMERA) /* Called for on and off */
	{
		resizeContext(mWidth, mHeight);
	}
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////

void FreyjaRender::renderBox(vec3_t min, vec3_t max)
{
	mglDrawSelectBox(min, max, WHITE);
	// mglDrawBbox(min, max, RED, mColorWireframe);
}


void FreyjaRender::renderLights()
{
	vec4_t pos;

	if (mRenderMode & RENDER_LIGHTING)
	{
		freyjaGetLightPosition4v(0, pos);
		
		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2]);
		glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
		mglDrawSphere(16, 16, 0.75f);
		glPopMatrix();
		
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_POSITION, pos);
	}
}

#define TEST_NEW_BACKEND_FORMAT
#ifdef TEST_NEW_BACKEND_FORMAT
#include <freyja/FreyjaMesh.h>
using namespace freyja;
extern Mesh *freyjaModelGetMeshClass(index_t modelIndex, index_t meshIndex);
#endif
void FreyjaRender::renderMesh(RenderMesh &mesh)
{
	const vec_t scale = 1.0001f;
	static Vector3d u, v;
	static RenderPolygon face;
	vec_t *color;
	unsigned int i, j, n;


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
		glPointSize(mDefaultPointSize);

#ifdef TEST_NEW_BACKEND_FORMAT
		Mesh *m = freyjaModelGetMeshClass(0,0);

		if (m)
		{
			glPointSize(2.0);
			glBegin(GL_POINTS);
			
			for (i = 0; i < m->GetVertexCount(); ++i)
			{
				m->GetVertexPos(i, v.mVec);
				glColor3fv(GREEN);
				glVertex3fv(v.mVec);
			}

			glEnd();
			glPointSize(mDefaultPointSize);
		}
#endif
	}

#ifdef TEST_NEW_BACKEND_FORMAT
		Mesh *m = freyjaModelGetMeshClass(0,0);

		if (m && mRenderMode & RENDER_WIREFRAME)
		{
			for (i = 0, n = m->GetFaceCount(); i < n; ++i)
			{
				Face *f = m->GetFace(i);

				if (!f) continue;

				glBegin(GL_LINE_LOOP);
				glColor3fv(mColorWireframeHighlight);

				for (j = 0; j < f->mIndices.size(); ++j)
				{
					m->GetVertexPos(f->mIndices[j], v.mVec);
					glVertex3fv(v.mVec);
				}
				
				glEnd();
				glPointSize(mDefaultPointSize);
			}
		}
#endif

	if (mesh.getPolygonCount())
	{
		if (mRenderMode & RENDER_TEXTURE)
		{
			glEnable(GL_TEXTURE_2D);
		}
		
		if (mRenderMode & RENDER_MATERIAL)
		{
			// FIXME: mesh.material
			freyjaApplyMaterial(freyjaGetCurrentMaterial());
		}

		color = mColorWireframe;

		if ((int)mesh.id == (int)mModel->getCurrentMesh())
		{
			mRenderMode |= fHightlightPolygonWireframe;
			color = mColorWireframeHighlight;
		}

		// Setup GL state here for faces
		glLineWidth(mDefaultLineWidth*2.0f);

		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);


		// FIXME Cached array access instead of functional facade
		for (i = 0, n = mesh.getPolygonCount(); i < n; ++i)
		{
			if (mesh.getPolygon(i, mesh.frame, face))
			{
				/* Render wireframe */
				if (mRenderMode & RENDER_WIREFRAME)
				{
					glColor3fv(color);
					glBegin(GL_LINE_LOOP);

					for (j = 0; j < face.count; ++j)
					{
						u = face.vertices[j];
						u *= scale;

						glVertex3fv(u.mVec);
					}
	  
					glEnd();
				}

				// Render normals
				if (mRenderMode & RENDER_NORMALS)
				{
					glBegin(GL_LINES);
					glColor3f(0.2, 0.2, 0.8);

					for (j = 0; j < face.count; ++j)
					{
						u = face.vertices[j];
						v = face.normals[j];
						v *= (2 * (1 / mZoom));
						v += u;

						glVertex3fv(u.mVec);
						glVertex3fv(v.mVec);
					}
		
					glEnd();
				}
			}
		}

		glPopAttrib();

		/* Render face with material, color, or something */
		if (mRenderMode & RENDER_FACE)
		{
			if (mRenderMode & RENDER_TEXTURE) // Now implies material
			{
				glEnable(GL_TEXTURE_2D);

			for (i = 0, n = mesh.getPolygonCount(); i < n; ++i)
			{
				if (mesh.getPolygon(i, mesh.frame, face))
				{
					// FIXME - move to pass2
					if (face.flags & fPolygon_Alpha)  
						continue;

					freyjaApplyMaterial(face.material); // material!

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

					for (j = 0; j < face.count; ++j)
					{
						if (face.material == COLORED_POLYGON)
						{
							glColor4fv(face.colors[j]);
						}
						else
						{
							glColor3fv(WHITE);
						}

						glTexCoord2fv(face.texcoords[j].mVec);
						glNormal3fv(face.normals[j].mVec);
						glVertex3fv(face.vertices[j].mVec);
					}

					glEnd();
				}
			}
			}
			else // Fake color
			{
			for (i = 0, n = mesh.getPolygonCount(); i < n; ++i)
			{
				if (mesh.getPolygon(i, mesh.frame, face))
				{
					freyjaApplyMaterial(face.material); // material!

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

					for (j = 0; j < face.count; ++j)
					{
						glColor4f(face.texcoords[j].mVec[0], 
								  face.texcoords[j].mVec[1], 0.5, 1.0);
						glVertex3fv(face.vertices[j].mVec);
					}

					glEnd();
				}
			}				
			}
		}
		
		if ((int)mesh.id == (int)mModel->getCurrentMesh())
		{
			mRenderMode ^= fHightlightPolygonWireframe;
		}
		
		if (mRenderMode & RENDER_TEXTURE)
		{
			glDisable(GL_TEXTURE_2D);
		}
	}
}


void FreyjaRender::renderModel(RenderModel &model)
{
	Vector<unsigned int> *list;
	RenderMesh rmesh;
	vec3_t min, max;
	vec3_t *xyz;
	int32 meshIndex = mModel->getCurrentMesh();
	uint32 count, i;


	glPushMatrix();

	/* Render patch test -- this should be model specific later */ 
	if (FreyjaRender::mPatchDisplayList > 0)
	{
		glCallList(FreyjaModel::gTestPatch.displayList);
		drawPatch(FreyjaModel::gTestPatch);
	}

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);

	/* Highlight current vertex group
	 * -- this should be model specific later:
	 * eg mModel->getCurrentGroup() -> model.index */
	if (mRenderMode & RENDER_BBOX && model.getMeshCount() > 0)
	{
		/* Render bounding box */
		freyjaGetMeshFrameBoundingBox(mModel->getCurrentMesh(),
									  mModel->getCurrentGroup(), min, max);
		renderBox(min, max);
	}

	if (mRenderMode & RENDER_POINTS)
	{
		/* Render bounding box */
		//mModel->getMeshBoundingBox(mModel->getCurrentGroup(), min, max);
		//renderBox(min, max);

		/* Render actual vertices */
		count = freyjaGetMeshVertexGroupVertexCount(meshIndex, 0);

		if (count > 0)
		{
			glPointSize(mDefaultPointSize);
			glColor3fv(mColorVertexHighlight);
			glBegin(GL_POINTS);

			for (i = 0; i < count; ++i)
			{
				xyz = freyjaGetVertexXYZ(freyjaGetMeshVertexGroupVertexIndex(meshIndex, 0, i));

				if (xyz)
				{
					glVertex3fv(*xyz);
				}
			}
  
			glEnd();
		}
	}    


	/* Render bounding box selection */
	mModel->getVertexSelection(min, max, &list);

	if (mRenderMode & RENDER_BBOX_SEL)
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


	/* Render meshes */
	for (i = 0, count = model.getMeshCount(); i < count; ++i)
	{
		if (model.getMesh(i, rmesh, mModel->getCurrentGroup()))
			renderMesh(rmesh);
	}


	/* Render skeleton */
	if (mRenderMode & RENDER_BONES)
	{
		glPushAttrib(GL_ENABLE_BIT);

		/* Yay, let thou bones show through thine meshes 
		 * and it was good for editing */
		if (mRenderMode & fRenderBonesClearedZBuffer)
		{
			glClear(GL_DEPTH_BUFFER_BIT);
		}

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);

		FreyjaRender::mSelectedBone = mModel->getCurrentBone();
		renderSkeleton(model.getSkeleton(), 0, mZoom);

		glPopAttrib();
	}

	glPopMatrix();
}


void FreyjaRender::renderPolygon(RenderPolygon &face)
{
	static Vector3d u, v;
	static unsigned int i;
	const vec_t scale = 1.0001f;

	/* Render wireframe */
	if (mRenderMode & RENDER_WIREFRAME)
	{
		glLineWidth(mDefaultLineWidth);

		// Update wireframe color
		if (face.id == (int)mModel->getCurrentPolygon())
		{
			glColor3fv(RED);
		}
		else if (mRenderMode & fHightlightPolygonWireframe)
		{
			glColor3fv(mColorWireframeHighlight);
		}
		else
		{
			glColor3fv(mColorWireframe);
		}


		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);

		glBegin(GL_LINE_LOOP);

		for (i = 0; i < face.count; ++i)
		{
			u = face.vertices[i];
			u *= scale;

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
		if (mRenderMode & RENDER_TEXTURE && 
			face.material != COLORED_POLYGON)
		{
			if (mRenderMode & RENDER_MATERIAL)
			{
				//glPushAttrib(GL_ENABLE_BIT);
				freyjaApplyMaterial(face.material);
			}
			else
			{
				BindTexture(face.material + 1);
			}
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


void FreyjaRender::renderSkeleton(RenderSkeleton &skeleton, 
								  unsigned int currentBone,
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
	 glColor3fv(CYAN) : glColor3fv(WHITE));
	mglDrawBone(FreyjaRender::mBoneRenderType, pos.mVec);

	/* Transform child bones */
	glPushMatrix();
	glTranslatef(pos.mVec[x], pos.mVec[y], pos.mVec[z]);
	glRotatef(bone.rotate.mVec[zr], 0, 0, 1);
	glRotatef(bone.rotate.mVec[yr], 0, 1, 0);
	glRotatef(bone.rotate.mVec[xr], 1, 0, 0);
	n = bone.getChildrenCount();

	for (i = 0; i < n; ++i)
	{
		index = bone.getBoneIndex(i);

		if (index != currentBone)
		{
			renderSkeleton(skeleton, index, scale);
		}
	}

	glPopMatrix();
}


/* Mongoose 2004.03.30, 
 * Dependences of this method:
 * x GL context width and height
 * - Current mesh to skin
 * - OpenGL 2d view helper function
 * - Quad to render skin helper function
 */
void FreyjaRender::renderUVWindow()
{
	RenderMesh mesh;
	RenderPolygon face;
	float x = 0.0f, y = 0.0f;
	unsigned int i, j, n, width, height;


	width = getWindowWidth();
	height = getWindowHeight();

	glPushMatrix();

	gl_resize_2d(width, height);
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
		if (model.getMesh(mModel->getCurrentMesh(), mesh, 
						  mModel->getCurrentGroup()))
		{
			valid = true;
		}
	}
	

	if (!valid)
	{
		DrawQuad(0.0, 0.0, width, height);
		resizeContext(width, height);
		glPopAttrib();
		glPopMatrix();
		return;
	}

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


	for (i = 0, n = mesh.getPolygonCount(); i < n; ++i)
	{
		if (!mesh.getPolygon(i, face))
			continue;

		if (mesh.id == (int)mModel->getCurrentMesh())
		{
			if (face.id == (int)mModel->getCurrentPolygon())
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
		
		if (face.material != (int)mModel->getCurrentTextureIndex())
		{
			if (mesh.id == (int)mModel->getCurrentMesh())
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
			if ((int)mModel->getCurrentTextureIndex() == face.material)
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

	DrawQuad(0.0, 0.0, width, height);
	glPopAttrib();
	resizeContext(width, height);
	glPopMatrix();
}


void FreyjaRender::DrawGrid(freyja_plane_t plane, int w, int h, int size)
{
	static int x, y, offset_x, offset_y;


	glPushMatrix();

	if (mRenderMode & fRenderGridClearedZBuffer)
		glClear(GL_DEPTH_BUFFER_BIT);

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glLineWidth(2.0);

	switch (plane)
	{
	case PLANE_XY:
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


	case PLANE_XZ:
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


	case PLANE_ZY:
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

   glPopAttrib();
   glPopMatrix();
}

double gMatrix[16];

void FreyjaRender::drawWindow(freyja_plane_t plane)
{
	RenderModel model;
	unsigned int i;


	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);

	if (mRenderMode & RENDER_EDIT_GRID)
		DrawGrid(plane, getWindowWidth(), getWindowHeight(), 10);

#ifdef PLANE_NOTIFY_WITH_AXIS
	glPushMatrix();
	glTranslatef(/*-20.0*/-mScaleEnv, /*-17.0*/-mScaleEnv + 2.5f, 10.0);

	switch (plane)
	{
	case PLANE_XY:
		break;
	case PLANE_XZ:
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		break;
	case PLANE_ZY:
		glRotatef(90.0, 0.0, 1.0, 0.0);
		break;
	}

	glLineWidth(2.0);

	mglDrawAxis(0.5f, 2.4f, 1.744f);
	glPopMatrix();
#endif

	glPopAttrib();

	switch (plane)
	{
	case PLANE_XY: // front
		glTranslatef(mScroll[0], mScroll[1], 0.0);
		break;
	case PLANE_XZ: // top
		glTranslatef(mScroll[0], mScroll[2], 0.0);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		break;
	case PLANE_ZY: // side
		glTranslatef(mScroll[2], mScroll[1], 0.0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		break;
	}

	glPushMatrix();

	renderLights();

	glScalef(mZoom, mZoom, mZoom);

	getOpenGLModelviewMatrix(gMatrix);

	for (i = 0; i < freyjaGetRenderModelCount(); ++i)
	{
		freyjaGetRenderModel(i, model);
		renderModel(model);
	}

	mglDraw3dCursor(0.5f, 2.4f, 1.744f);

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

#ifdef USE_TORUS_TEST
	glPushMatrix();
	glRotatef(45.0f, 1, 0, 0);
	glRotatef(45.0f, 0, 0, 1);
	drawTorus(3.0, 10.0);
	glPopMatrix();
#else
	/* Cast light on sphere colored/detailed by material */
	glPushMatrix();
	glRotatef(90.0f, 1, 0, 0);
	glRotatef(180.0f, 0, 1, 0);
	freyjaApplyMaterial(freyjaGetCurrentMaterial());
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

	glColor3fv(WHITE);
	freyjaApplyMaterial(freyjaGetCurrentMaterial());
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(x, y, z);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(x, y+h, z);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(x+w, y+h, z);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(x+w, y, z);
	glEnd();	
	glPopMatrix();
	glPopMatrix();
}


