/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : Nif4
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: NetImmerse Model 4.0 class
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.08.16:
 * Mongoose - Created
 ==========================================================================*/

#include "Nif4.h"


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Nif4::Nif4()
{
	mHeader[0] = 0;
	mHeaderSz = 48; /* 38? This was 128 in 3.x */
}


Nif4::~Nif4()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

bool Nif4::loadModel(const char *filename)
{
	EggFileReader r;
	Vector<NiNode *> ninodes;
	Chunk chunk;
	bool done = false;


	if (r.openFile(filename) == false)
		return false;

	/* Get header and check to see if this is a Nifv4.0.0.2 */
	r.readCharString(mHeaderSz, mHeader);

	if (strncmp(NIF4_HEADER_START, mHeader, 34))
		return false;

	while (!done && chunk.readHeader(r))
	{
		if (!strncmp(chunk.name, "NiNode", 6))
		{
			NiNode *ninode = new NiNode();			
			ninodes.pushBack(ninode);
			ninode->readChunk(r);
		}
		else
		{
			return false;
		}
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
// Plugin hooks
////////////////////////////////////////////////////////////

#ifdef FREYJA_MODEL_PLUGINS
#include <freyja8/EggPlugin.h>

extern "C" {

	int freyja_model__nif4_check(char *filename);
	int freyja_model__nif4_import(char *filename);
	int freyja_model__nif4_export(char *filename);
}


int freyja_model__nif4_check(char *filename)
{	
	EggFileReader r;
	char header[48];
	unsigned int headerSz = 48;

	if (r.openFile(filename) == false)
		return -1;

	/* Get header and check to see if this is a Nifv4.0.0.2 */
	r.readCharString(headerSz, header);
	r.closeFile();

	if (strncmp(NIF4_HEADER_START, header, 34))
		return -1;

	return 0;
}


int freyja_model__nif4_import(char *filename)
{
	Nif4 nif4;
	//Vector<unsigned int> weights;
	//unsigned int vertex, texcoord;
	//int m, v, w, t, j;


	if (nif4.loadModel(filename) == false)
		return -1;

	return 0;
#ifdef FIXME
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
#endif

	return -1;
}


int freyja_model__nif4_export(char *filename)
{
	eggPrintError("nif4_export> ERROR: Not implemented.\n");
	return -1;
}
#endif


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_NIF4
int runNif4UnitTest(int argc, char *argv[])
{
	Nif4 nif4;

	if (nif4.loadModel(argv[1]) == false)
		return -1;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[Nif4 class test]\n");

	return runNif4UnitTest(argc, argv);
}
#endif
