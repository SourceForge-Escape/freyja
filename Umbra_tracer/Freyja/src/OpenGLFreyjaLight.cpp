/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : Light
 * License : No use w/o permission (C) 2002 Mongoose
 * Comments: This is the GL light class
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2002.01.27:
 * Mongoose - Created
 ==========================================================================*/

#ifdef HAVE_OPENGL
#   include <GL/gl.h>
#   include <GL/glu.h>
#endif

#include "OpenGLFreyjaLight.h"


vec4_t OpenGLFreyjaLight::mGobalAmbient = {0.1f, 0.1f, 0.1f, 1.0f};


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

OpenGLFreyjaLight::OpenGLFreyjaLight() : FreyjaLight()
{
	mEnabled = false;
}


OpenGLFreyjaLight::~OpenGLFreyjaLight()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

void OpenGLFreyjaLight::renderSymbol(FreyjaLight *light)
{
	glPushMatrix();
	glTranslated(light->mPos[0], light->mPos[1], light->mPos[2]);
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	//FIXME mglDrawSphere(16, 16, 0.75f);
	glPopMatrix();
}


/* FIXME: make a round robin for > 8 sources first */
void OpenGLFreyjaLight::enable()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, mPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, mAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, mDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, mSpecular);
	mEnabled = true;
}


void OpenGLFreyjaLight::enableGobalAmbient()
{
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, mGobalAmbient);
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef LIGHT_UNIT_TEST
int runLightUnitTest(int argc, char *argv[])
{
	return 0;
}


int main(int argc, char *argv[])
{
	printf("[Light class test]\n");

	return runLightUnitTest(argc, argv);
}
#endif
