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

#include <freyja8/EggFileReader.h>

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
}


Md5::~Md5()
{
	if (mCommandLine)
		delete [] mCommandLine;
	
	if (mNumJoints)
		delete [] mJoints;
	
	if (mMeshes)
		delete [] mMeshes;
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
	EggFileReader r;
	int i, j;  /* I hate everyone that uses signed indices in file formats */


	if (isMd5Model(filename) == false)
		return false;

	if (r.openFile(filename) == false)
		return false;

	if (!r.parseMatchingSymbol("MD5Version"))
		return false;

	mVersion = r.parseInteger();

	switch (mVersion)
	{
	case 10:
		break;
	default:
		printf("<Md5> Don't know this version %i.\n", mVersion);
		return false;
	}


	/* Some Id thingys */
	if (!r.parseMatchingSymbol("commandline"))
		return false;

	mCommandLine = r.parseString();


	/* Joint setup */
	if (!r.parseMatchingSymbol("numJoints"))
		return false;

	mNumJoints = r.parseInteger();

	if (mNumJoints < 0)
	{
		mNumJoints = 0;
	}
	else
	{
		mJoints = new Md5Joint[mNumJoints];
	}


	/* Mesh setup */
	if (!r.parseMatchingSymbol("numMeshes"))
		return false;

	mNumMeshes = r.parseInteger();

	if (mNumMeshes < 0)
	{
		mNumMeshes = 0;
	}
	else
	{
		mMeshes = new Md5Mesh[mNumMeshes];
	}


	/* Joint data */
	if (mNumJoints && r.parseMatchingSymbol("joints"))
	{
		if (!r.parseMatchingSymbol("{"))
			return false;
			
		for (i = 0; i < mNumJoints; ++i)
		{
			mJoints[i].name = r.parseString();
			mJoints[i].parent = r.parseInteger();

			/* translate X Y Z */
			if (!r.parseMatchingSymbol("("))
				return false;

			mJoints[i].translate[0] = r.parseFloat();
			mJoints[i].translate[1] = r.parseFloat();
			mJoints[i].translate[2] = r.parseFloat();

			if (!r.parseMatchingSymbol(")"))
				return false;


			/* rotate X Y Z */
			if (!r.parseMatchingSymbol("("))
				return false;
			
			mJoints[i].rotate[0] = r.parseFloat();
			mJoints[i].rotate[1] = r.parseFloat();
			mJoints[i].rotate[2] = r.parseFloat();

			if (!r.parseMatchingSymbol(")"))
				return false;

			// FIXME: Save comment at end of line for something?
		}
		
		if (!r.parseMatchingSymbol("}"))
			return false;
	}
	else
	{
		return false;
	}

	
	for (i = 0; i < mNumMeshes; ++i)
	{
		if (r.parseMatchingSymbol("mesh"))
		{
			if (!r.parseMatchingSymbol("{"))
				return false;

			if (!r.parseMatchingSymbol("shader"))
				return false;

			mMeshes[i].shader = r.parseString();

			if (!r.parseMatchingSymbol("numverts"))
				return false;

			mMeshes[i].numverts = r.parseInteger();

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
				if (!r.parseMatchingSymbol("vert"))
					return false;

				mMeshes[i].verts[j].index = r.parseInteger();

				if (!r.parseMatchingSymbol("("))
					return false;

				mMeshes[i].verts[j].uv[0] = r.parseFloat(); 
				mMeshes[i].verts[j].uv[1] = r.parseFloat();

				if (!r.parseMatchingSymbol(")"))
					return false;
				
				mMeshes[i].verts[j].weight = r.parseInteger();
				mMeshes[i].verts[j].numbones =r. parseInteger();
			}

			if (!r.parseMatchingSymbol("numtris"))
				return false;

			mMeshes[i].numtriangles = r.parseInteger();
			mMeshes[i].triangles = new Md5Triangle[mMeshes[i].numtriangles];

			for (j = 0; j < mMeshes[i].numtriangles; ++j)
			{
				if (!r.parseMatchingSymbol("tri"))
					return false;
				
				r.parseInteger(); // integer == j
				mMeshes[i].triangles[j].vertex[0] = r.parseInteger();
				mMeshes[i].triangles[j].vertex[1] = r.parseInteger();
				mMeshes[i].triangles[j].vertex[2] = r.parseInteger();
			}

			if (!r.parseMatchingSymbol("numweights"))
				return false;

			mMeshes[i].numweights = r.parseInteger();
			mMeshes[i].weights = new Md5Weight[mMeshes[i].numweights];

			for (j = 0; j < mMeshes[i].numweights; ++j)
			{
				if (!r.parseMatchingSymbol("weight"))
					return false;

				mMeshes[i].weights[j].index = r.parseInteger();
				mMeshes[i].weights[j].joint = r.parseInteger();
				mMeshes[i].weights[j].weight = r.parseFloat();

				if (!r.parseMatchingSymbol("("))
					return false;

				mMeshes[i].weights[j].pos[0] = r.parseFloat();
				mMeshes[i].weights[j].pos[1] = r.parseFloat();
				mMeshes[i].weights[j].pos[2] = r.parseFloat();

				if (!r.parseMatchingSymbol(")"))
					return false;
			}
			
		}
		
		if (!r.parseMatchingSymbol("}"))
			return false;
	}

	r.closeFile();

	return true;
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Special Interface code
////////////////////////////////////////////////////////////

#ifdef FREYJA_MODEL_PLUGINS
#include <freyja8/EggPlugin.h>

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

		//eggPrintMessage("%i\t[%i] %f %f %f\t", vertex, v, 
		//			md5.mMeshes[m].weights[w].pos[0],
		//			md5.mMeshes[m].weights[w].pos[1],
		//			md5.mMeshes[m].weights[w].pos[2]);  

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
