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

#include <stdlib.h>

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
	mTempBufferHackSz = 2047;
	mTempBufferHack = new char[mTempBufferHackSz+1];
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
		return true;
	}

	printf("<Md5> Not a valid md5 model file.\n");

	return false;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

bool Md5::loadModel(const char *filename)
{
	int i, j;  /* I hate everyone that uses signed indices in file formats */


	if (isMd5Model(filename) == false)
		return false;

	if (loadFile(filename) == false)
		return false;

	if (!matchSymbol("MD5Version"))
		return false;

	mVersion = getInteger();

	switch (mVersion)
	{
	case 10:
		break;
	default:
		printf("<Md5> Don't know this version %i.\n", mVersion);
		return false;
	}


	/* Some Id thingys */
	if (!matchSymbol("commandline"))
		return false;

	mCommandLine = getString();


	/* Joint setup */
	if (!matchSymbol("numJoints"))
		return false;

	mNumJoints = getInteger();

	if (mNumJoints < 0)
	{
		mNumJoints = 0;
	}
	else
	{
		mJoints = new Md5Joint[mNumJoints];
	}


	/* Mesh setup */
	if (!matchSymbol("numMeshes"))
		return false;

	mNumMeshes = getInteger();

	if (mNumMeshes < 0)
	{
		mNumMeshes = 0;
	}
	else
	{
		mMeshes = new Md5Mesh[mNumMeshes];
	}


	/* Joint data */
	if (mNumJoints && matchSymbol("joints"))
	{
		if (!matchSymbol("{"))
			return false;
			
		for (i = 0; i < mNumJoints; ++i)
		{
			mJoints[i].name = getString();
			mJoints[i].parent = getInteger();

			/* translate X Y Z */
			if (!matchSymbol("("))
				return false;

			mJoints[i].translate[0] = getFloat();
			mJoints[i].translate[1] = getFloat();
			mJoints[i].translate[2] = getFloat();

			if (!matchSymbol(")"))
				return false;


			/* rotate X Y Z */
			if (!matchSymbol("("))
				return false;
			
			mJoints[i].rotate[0] = getFloat();
			mJoints[i].rotate[1] = getFloat();
			mJoints[i].rotate[2] = getFloat();

			if (!matchSymbol(")"))
				return false;

			// FIXME: Save comment at end of line for something?
		}
		
		if (!matchSymbol("}"))
			return false;
	}
	else
	{
		return false;
	}

	
	for (i = 0; i < mNumMeshes; ++i)
	{
		if (matchSymbol("mesh"))
		{
			if (!matchSymbol("{"))
				return false;

			if (!matchSymbol("shader"))
				return false;

			mMeshes[i].shader = getString();

			if (!matchSymbol("numverts"))
				return false;

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
				if (!matchSymbol("vert"))
					return false;

				mMeshes[i].verts[j].index = getInteger();

				if (!matchSymbol("("))
					return false;

				mMeshes[i].verts[j].uv[0] = getFloat(); 
				mMeshes[i].verts[j].uv[1] = getFloat();

				if (!matchSymbol(")"))
					return false;
				
				mMeshes[i].verts[j].weight = getInteger();
				mMeshes[i].verts[j].numbones = getInteger();
			}

			if (!matchSymbol("numtris"))
				return false;

			mMeshes[i].numtriangles = getInteger();
			mMeshes[i].triangles = new Md5Triangle[mMeshes[i].numtriangles];

			for (j = 0; j < mMeshes[i].numtriangles; ++j)
			{
				if (!matchSymbol("tri"))
					return false;
				
				getInteger(); // integer == j
				mMeshes[i].triangles[j].vertex[0] = getInteger();
				mMeshes[i].triangles[j].vertex[1] = getInteger();
				mMeshes[i].triangles[j].vertex[2] = getInteger();
			}

			if (!matchSymbol("numweights"))
				return false;

			mMeshes[i].numweights = getInteger();
			mMeshes[i].weights = new Md5Weight[mMeshes[i].numweights];

			for (j = 0; j < mMeshes[i].numweights; ++j)
			{
				if (!matchSymbol("weight"))
					return false;

				mMeshes[i].weights[j].index = getInteger();
				mMeshes[i].weights[j].joint = getInteger();
				mMeshes[i].weights[j].weight = getFloat();

				if (!matchSymbol("("))
					return false;

				mMeshes[i].weights[j].pos[0] = getFloat();
				mMeshes[i].weights[j].pos[1] = getFloat();
				mMeshes[i].weights[j].pos[2] = getFloat();

				if (!matchSymbol(")"))
					return false;
			}
			
		}
		
		if (!matchSymbol("}"))
			return false;
	}

	closeFile();

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


double Md5::getFloat()
{
	double r;


	// ugly fscanf use for temp hack
	fscanf(mFileHandle, "%lf", &r);
	//r = atof(getSymbol());

	//printf("** %f\n", r);
	return r;
}


int Md5::getInteger()
{
	int i;


	// ugly fscanf use for temp hack
	fscanf(mFileHandle, "%i", &i);
	//i = atoi(getSymbol());

	//printf("** %i\n", i);
	return i;
}


char *Md5::getString()
{
	unsigned int l, i = 0, state = 0;
	char *s;
	char c, lc = 0;
	
	// ugly fscanf use for temp hack
	while (i < mTempBufferHackSz && fscanf(mFileHandle, "%c", &c) != EOF)
	{
		switch (state)
		{
		case 0:
			if (c == '"')
				state = 1;
			break;
		case 1:
			if (c == '"' && lc != '\\')  // Allow quote escapes?
			{
				i = mTempBufferHackSz;
			}
			else
			{
				mTempBufferHack[i++] = c;
				mTempBufferHack[i] = 0;
			}
			break;
		}

		lc = c;
	}
	
	l = strlen(mTempBufferHack);
	s = new char[l+1];
	strncpy(s, mTempBufferHack, l);
	s[l] = 0;
	//printf("** \"%s\"\n", mTempBufferHack);

	return s;
}


char *Md5::getSymbol()
{
	unsigned int i = 0, state = 0;
	char c;


	mTempBufferHack[0] = 'F';
	mTempBufferHack[1] = 'U';
	mTempBufferHack[2] = 0;

	// ugly fscanf use for temp hack
	while (i < mTempBufferHackSz && fscanf(mFileHandle, "%c", &c) != EOF)
	{
		switch (state)
		{
		case 0:
			if (c == '/')
			{
				state = 1;
				mTempBufferHack[i++] = c;
				mTempBufferHack[i] = 0;	
			}
			else if (c == ' ' || c == '\r' || c == '\n' || c == '\t')
			{
				if (i > 0)
					i = mTempBufferHackSz;
			}
			else
			{
				mTempBufferHack[i++] = c;
				mTempBufferHack[i] = 0;
			}
			break;
		case 1:
			if (c == '/')
			{
				state = 2;
				--i;
				mTempBufferHack[i] = 0;
			}
			else
			{
				state = 0;
				mTempBufferHack[i++] = c;
				mTempBufferHack[i] = 0;
			}
			break;
		case 2:
			if (c == '\n')
			{
				/* Only wrap lines when given a only comment line(s) */
				if (i > 0)
					i = mTempBufferHackSz;
				else
					state = 0;
			}
			break;
		}
	}

	// ugly fscanf use for temp hack
	//	fscanf(mFileHandle, "%s", mTempBufferHack);
   	//printf("\n** <%s>\n", mTempBufferHack);
	
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


bool Md5::matchSymbol(const char *symbol)
{
	unsigned int l = strlen(symbol); /* Assumes !(!symbol) && !(!symbol[0]) */
	char *sym = getSymbol();
	bool test = (!strncmp(symbol, sym, l));

	
	if (!test)
	{
		printf("Not matched: '%s' != '%s'\n", symbol, sym);
	}

	return test;
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

	if (md5.isMd5Model(filename) == true)
		return 0; /* 0 is no error */

	return -1;
}


int freyja_model__md5_import(char *filename)
{
	Md5 md5;
	Vector<unsigned int> weights;
	unsigned int vertex, texcoord;
	int m, v, w, t, j;


	if (md5.loadModel(filename) == false)
		return -1;

	for (m = 0; m < md5.mNumMeshes; ++m)
	{
		Vector<unsigned int> vertices;
		Vector<unsigned int> texcoords;


		/* Start a new mesh */
		eggBegin(FREYJA_MESH);
	
		/* Start a new vertex group */
		eggBegin(FREYJA_GROUP);

		for (v = 0; v < md5.mMeshes[m].numverts; ++v)
		{
			w = md5.mMeshes[m].verts[v].weight;

			/* Store vertices in group */
			vertex = eggVertexStore3f(md5.mMeshes[m].weights[w].pos[0], 
									  -md5.mMeshes[m].weights[w].pos[1], 
									  md5.mMeshes[m].weights[w].pos[2]);

			/* Store texels */
			texcoord = eggTexCoordStore2f(md5.mMeshes[m].verts[v].uv[0],
										  md5.mMeshes[m].verts[v].uv[1]);
			
			/* Generates id translator list */
			vertices.pushBack(vertex);
			texcoords.pushBack(texcoord);
		}

		for (t = 0; t < md5.mMeshes[m].numtriangles; ++t)
		{
			/* Start a new polygon */
			eggBegin(FREYJA_POLYGON);
			
			/* Store vertices and texels by true id, using translator lists */
			eggVertex1i(vertices[md5.mMeshes[m].triangles[t].vertex[0]]);
			eggVertex1i(vertices[md5.mMeshes[m].triangles[t].vertex[1]]);
			eggVertex1i(vertices[md5.mMeshes[m].triangles[t].vertex[2]]);
			eggTexCoord1i(texcoords[md5.mMeshes[m].triangles[t].vertex[0]]);
			eggTexCoord1i(texcoords[md5.mMeshes[m].triangles[t].vertex[1]]);
			eggTexCoord1i(texcoords[md5.mMeshes[m].triangles[t].vertex[2]]);

			eggTexture1i(m); // mMeshes[m].shader
			
			eggEnd(); // FREYJA_POLYGON
		}

		eggEnd(); // FREYJA_GROUP
		eggEnd(); // FREYJA_MESH
	}


	/* Load skeleton */
	for (m = 0; m < md5.mNumMeshes; ++m)
	{
		for (v = 0; v < md5.mMeshes[m].numverts; ++v)
		{
			w = md5.mMeshes[m].verts[v].weight;

			eggVertexWeightStore(v, 
								 md5.mMeshes[m].weights[w].weight,
								 md5.mMeshes[m].weights[w].joint);

			/* Generates id translator list */
			weights.pushBack(md5.mMeshes[m].weights[w].index);
		}
	}

	eggBegin(FREYJA_SKELETON);

	for (j = 0; j < md5.mNumJoints; ++j)
	{
		/* Start a new tag */
		eggBegin(FREYJA_BONE);
		eggTagFlags1u(0x0);
		eggTagName(md5.mJoints[j].name);
		eggTagPos3f(md5.mJoints[j].translate[0],
					-md5.mJoints[j].translate[1],
					md5.mJoints[j].translate[2]);
		eggTagRotate3f(md5.mJoints[j].rotate[0],
					  	-md5.mJoints[j].rotate[1],
						md5.mJoints[j].rotate[2]);

		eggPrintMessage("[%i]", j);   

		for (int j2 = 0; j2 < md5.mNumJoints; ++j2)
		{
			if (md5.mJoints[j2].parent == j)
			{ 
				eggTagAddSlave1u(j2);
			}
		}

		eggEnd(); // FREYJA_TAG
	}

	eggEnd(); // FREYJA_SKELETON


	eggPrintMessage("The Doom3 plugin sez: FINISH ME!!!");

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
