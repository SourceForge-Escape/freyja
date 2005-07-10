/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : FreyjaLight
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This is the light data model class.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.10.22:
 * Mongoose - Created, based on Freyja's Light class from 2002.01.27
 ==========================================================================*/

#include "Light.h"

using namespace freyja;

unsigned int Light::mCounter = 0;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Light::Light()
{
	mName[0] = 0;
	mOptions = 0;
	mId = mCounter++;
	mType = ePoint;

	mAmbient[0] = 0.1f; mAmbient[1] = 0.1f; mAmbient[2] = 0.1f;
	mAmbient[3] = 1.0f;

	mDiffuse[0] = 1.0f; mDiffuse[1] = 1.0f; mDiffuse[2] = 1.0f;
	mDiffuse[3] = 1.0f;

	mSpecular[0] = 1.0f; mSpecular[1] = 1.0f; mSpecular[2] = 1.0f;
	mSpecular[3] = 1.0f;

	mPos[0] = 0.0f; mPos[1] = 0.0f; mPos[2] = 0.0f; mPos[3] = 1.0f;
	mDir[0] = 0.0f; mDir[1] = 1.0f; mDir[2] = 0.0f; mDir[3] = 0.0f;

	mCutOff = 15.0f;
	mExponent = 0.0f;
	mAttenuation = 1.0f;
}


Light::~Light()
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


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_FREYJALIGHT
int runLightUnitTest(int argc, char *argv[])
{
	Light test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[Light class test]\n");

	return runLightUnitTest(argc, argv);
}
#endif
