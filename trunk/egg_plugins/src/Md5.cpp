/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : Md5
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: Doom3 model plugin
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.08.13:
 * Mongoose - Created
 ==========================================================================*/

#include "Md5.h"


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Md5::Md5()
{
	mVersion = 0;
	mCommandLine = 0x0;
	mNumJoints = 0;
	mNumMeshes = 0;
	mJoints = 0x0;
	mMeshes = 0x0;

	mFileHandle = 0x0;
	mTempBufferHackSz = 2048;
	mTempBufferHack = new char[mTempBufferHackSz];
}


Md5::~Md5()
{
	if (mCommandLine)
		delete [] mCommandLine;
	
	if (mNumJoints)
		delete [] mJoints;
	
	if (mMeshes)
		delete [] mMeshes;

	if (mTempBufferHack)
		delete [] mTempBufferHack;
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

bool Md5::isMd5Model(const char *filename)
{
	FILE *f = fopen(filename, "rb");
	char buffer[32];
	

	fread(buffer, 32, 1, f);
	
	if (!strncmp("MD5Version", buffer, 10))
	{
		printf("Not a valid md5 model file.\n");
		return true;
	}

	return false;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

bool Md5::loadModel(const char *filename)
{
	char *symbol;
	int i, j;  /* I hate everyone that uses signed indices in file formats */


	if (isMd5Model(filename) == false)
		return false;

	if (loadFile(filename) == false)
		return false;

	symbol = getSymbol();	
	if (strncmp("MD5Version", symbol, 10))
	{
		return false;	
	}

	mVersion = getInteger();

	switch (mVersion)
	{
	case 10:
		break;
	default:
		printf("Don't know version %i.\n", mVersion);
		return false;
	}

	symbol = getSymbol();	
	if (strncmp("commandline", symbol, 11))
	{
		return false;	
	}	

	/* Chunk/command style reads later instead of this order? */
	mCommandLine = getString();

	mNumJoints = getInteger();

	if (mNumJoints < 0)
		mNumJoints = 0;

	mNumMeshes = getInteger();

	if (mNumMeshes < 0)
		mNumMeshes = 0;

	symbol = getSymbol();	
	if (!strncmp("joints", symbol, 6))
	{
		symbol = getSymbol();
		
		if (strncmp("{", symbol, 1))
			return false;
			
		for (i = 0; i < mNumJoints; ++i)
		{
			mJoints[i].name = getString();
			mJoints[i].index = getInteger();

			/* translate X Y Z */
			getSymbol(); // '('
			mJoints[i].translate[0] = getFloat();
			mJoints[i].translate[1] = getFloat();
			mJoints[i].translate[2] = getFloat();
			getSymbol(); // ')'

			/* rotate X Y Z */
			getSymbol(); // '('
			mJoints[i].rotate[0] = getFloat();
			mJoints[i].rotate[1] = getFloat();
			mJoints[i].rotate[2] = getFloat();
			getSymbol(); // ')'

			// FIXME: Look for comment at end of line and save here?
		}

		symbol = getSymbol();
		
		if (strncmp("}", symbol, 1))
			return false;
	}
	else
	{
		return false;
	}

	
	for (i = 0; i < mNumMeshes; ++i)
	{
		symbol = getSymbol();	
		if (!strncmp("mesh", symbol, 4))
		{
			symbol = getSymbol();
		
			if (strncmp("{", symbol, 1))
				return false;

			mMeshes[i].shader = getString();
			mMeshes[i].numverts = getInteger();

			if (mMeshes[i].numverts < 0)
			{
				mMeshes[i].numverts = 0;
			}
			else
			{
				mMeshes[i].verts = new Md5Vertex[mMeshes[i].numverts];
			}

			for (j = 0; j < mMeshes[i].numverts; ++j)
			{
				getSymbol(); // 'vert'
				mMeshes[i].verts[j].index = getInteger();

				getSymbol(); // '('
				mMeshes[i].verts[j].uv[0] = getFloat(); 
				mMeshes[i].verts[j].uv[1] = getFloat();
				getSymbol(); // ')' 
				
				mMeshes[i].verts[j].weight = getInteger();
				mMeshes[i].verts[j].numbones = getInteger();
			}

			mMeshes[i].numtriangles = getInteger();
			mMeshes[i].triangles = new Md5Triangle[mMeshes[i].numtriangles];

			for (j = 0; j < mMeshes[i].numweights; ++j)
			{
				getSymbol(); // 'tri'
				getInteger(); // integer == j
				mMeshes[i].triangles[j].vertex[0] = getInteger();
				mMeshes[i].triangles[j].vertex[1] = getInteger();
				mMeshes[i].triangles[j].vertex[2] = getInteger();
			}

			mMeshes[i].numweights = getInteger();
			mMeshes[i].weights = new Md5Weight[mMeshes[i].numweights];

			for (j = 0; j < mMeshes[i].numweights; ++j)
			{
				mMeshes[i].weights[j].index = getInteger();
				mMeshes[i].weights[j].joint = getInteger();
				mMeshes[i].weights[j].weight = getFloat();

				getSymbol(); // '('
				mMeshes[i].weights[j].pos[0] = getFloat();
				mMeshes[i].weights[j].pos[1] = getFloat();
				mMeshes[i].weights[j].pos[2] = getFloat();
				getSymbol(); // ')'
			}
			
		}

		symbol = getSymbol();
		
		if (strncmp("}", symbol, 1))
			return false;
	}


	return true;
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////

void Md5::closeFile()
{
	if (mFileHandle)
		fclose(mFileHandle);
}


float Md5::getFloat()
{
	float r;


	// ugly fscanf use for temp hack
	fscanf(mFileHandle, "%f", &r);

	return r;
}


int Md5::getInteger()
{
	int i;


	// ugly fscanf use for temp hack
	fscanf(mFileHandle, "%i", &i);

	return i;
}


char *Md5::getString()
{
	unsigned int l;
	char *s;


	// ugly fscanf use for temp hack
	fscanf(mFileHandle, "%s", mTempBufferHack);
	
	l = strlen(mTempBufferHack);
	s = new char[l+1];
	
	return s;
}


char *Md5::getSymbol()
{
	// ugly fscanf use for temp hack
	fscanf(mFileHandle, "%s", mTempBufferHack);
	return mTempBufferHack;
}


bool Md5::loadFile(const char *filename)
{
	mFileHandle = fopen(filename, "rb");
	
	if (!mFileHandle)
	{
		perror(filename);
		return false;
	}

	return true;
}


////////////////////////////////////////////////////////////
// Special Interface code
////////////////////////////////////////////////////////////

#ifdef FREYJA_MODEL_PLUGINS
#include <freyja_model/EggPlugin.h>

extern "C" {

	int freyja_model__md5_check(char *filename);
	int freyja_model__md5_import(char *filename);
	int freyja_model__md5_export(char *filename);
}


int freyja_model__md5_check(char *filename)
{
	Md5 md5;

	return (!md5.isMd5Model(filename) == true); /* 0 is no error */
}


int freyja_model__md5_import(char *filename)
{
	Md5 md5;

	if (md5.loadModel(filename) == false)
		return -1;

	eggPrintMessage("FINISH ME!!!");

	return 0;
}


int freyja_model__md5_export(char *filename)
{
	eggPrintError("md5_export> ERROR: Not implemented.\n");
	return -1;
}
#endif


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_MD5
int runMd5UnitTest(int argc, char *argv[])
{
	Md5 test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[Md5 class test]\n");

	return runMd5UnitTest(argc, argv);
}
#endif
