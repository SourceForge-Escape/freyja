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
#include <math.h>
#include <freyja/FreyjaFileReader.h>

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

float Md5::decodeIdQuaternion(float qx, float qy, float qz)
{
	float qw, tmp;

	tmp = 1.0f - ((qx*qx) + (qy*qy) + (qz*qz));

	if (tmp >= 0)
	{
		qw = -(float)sqrt( tmp );
	}
	else
	{
		qw = 0;
	}

	return qw;
}


bool Md5::isMd5Model(const char *filename)
{
	FILE *f = fopen(filename, "rb");
	char buffer[32];
	

	fread(buffer, 32, 1, f);
	
	if (!strncmp("MD5Version", buffer, 10))
	{
		return true;
	}

	//printf("<Md5> Not a valid md5 model file.\n");

	return false;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

bool Md5::loadModel(const char *filename)
{
	FreyjaFileReader r;
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
		//printf("<Md5> Don't know this version %i.\n", mVersion);
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

#ifdef FREYJA_PLUGINS
#include <freyja/FreyjaPlugin.h>
#include <hel/Quaternion.h>
#include <hel/Matrix.h>


extern "C" {

	int freyja_model__md5_check(char *filename);
	int freyja_model__md5_import(char *filename);
	int freyja_model__md5_export(char *filename);
	int import_model(char *filename);
	void freyja_init();
}


void freyja_init()
{
	freyjaPluginDescription1s("Doom3 Mesh (*.md5mesh)");
	freyjaPluginAddExtention1s("*.md5mesh");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH | FREYJA_PLUGIN_SKELETON);
	freyjaPluginExport1i(FREYJA_PLUGIN_NONE);
}


int import_model(char *filename)
{
	if (!freyja_model__md5_check(filename))
		return freyja_model__md5_import(filename);

	return -1;
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
	const vec_t scale = 0.3;
	Md5 md5;
	Vector<unsigned int> weights;
	unsigned int vertex, texcoord;
	int i, m, v, w, t, j, bindex, index;
	float qw;
	vec4_t wxyz;
	Quaternion q, q2;
	Matrix mat, mat2;
	Vector3d vec3, tmp;


	if (md5.loadModel(filename) == false)
		return -1;

	/* Start a new model */
	freyjaBegin(FREYJA_MODEL);

	for (m = 0; m < md5.mNumMeshes; ++m)
	{
		Vector<unsigned int> vertices;
		Vector<unsigned int> texcoords;


		/* Start a new mesh */
		freyjaBegin(FREYJA_MESH);
	
		/* Start a new vertex group */
		freyjaBegin(FREYJA_VERTEX_GROUP);

		for (v = 0; v < md5.mMeshes[m].numverts; ++v)
		{
			vec3.zero();

			for (i = 0; i < md5.mMeshes[m].verts[v].numbones; ++i)
			{
				w = md5.mMeshes[m].verts[v].weight;
				w = md5.mMeshes[m].weights[w].index + i;
				j = md5.mMeshes[m].weights[w].joint;

				qw = md5.decodeIdQuaternion(md5.mJoints[j].rotate[0],
											md5.mJoints[j].rotate[1],
											md5.mJoints[j].rotate[2]);
				q = Quaternion(qw, 
							   md5.mJoints[j].rotate[0],
							   md5.mJoints[j].rotate[1],
							   md5.mJoints[j].rotate[2]);

				tmp = q.rotate(md5.mMeshes[m].weights[w].pos);		
				tmp += Vector3d(md5.mJoints[j].translate[0],
								md5.mJoints[j].translate[1],
								md5.mJoints[j].translate[2]);
				vec3 += tmp * md5.mMeshes[m].weights[w].weight;

			}

			vec3 *= scale;

			vertex = freyjaVertex3f(vec3.mVec[0], vec3.mVec[2], vec3.mVec[1]);

			/* Store texels */
			texcoord = freyjaTexCoord2f(md5.mMeshes[m].verts[v].uv[0],
										md5.mMeshes[m].verts[v].uv[1]);
			
			/* Generates id translator list */
			vertices.pushBack(vertex);
			texcoords.pushBack(texcoord);
		}

		freyjaEnd(); // FREYJA_GROUP

		for (t = 0; t < md5.mMeshes[m].numtriangles; ++t)
		{
			/* Start a new polygon */
			freyjaBegin(FREYJA_POLYGON);
			
			/* Store vertices and texels by true id, using translator lists */
			freyjaPolygonVertex1i(vertices[md5.mMeshes[m].triangles[t].vertex[0]]);
			freyjaPolygonVertex1i(vertices[md5.mMeshes[m].triangles[t].vertex[1]]);
			freyjaPolygonVertex1i(vertices[md5.mMeshes[m].triangles[t].vertex[2]]);
			freyjaPolygonTexCoord1i(texcoords[md5.mMeshes[m].triangles[t].vertex[0]]);
			freyjaPolygonTexCoord1i(texcoords[md5.mMeshes[m].triangles[t].vertex[1]]);
			freyjaPolygonTexCoord1i(texcoords[md5.mMeshes[m].triangles[t].vertex[2]]);

			freyjaPolygonMaterial1i(m); // mMeshes[m].shader
			
			freyjaEnd(); // FREYJA_POLYGON
		}

		freyjaEnd(); // FREYJA_MESH
	}


	/* Load skeleton */
	for (m = 0; m < md5.mNumMeshes; ++m)
	{
		for (v = 0; v < md5.mMeshes[m].numverts; ++v)
		{
			w = md5.mMeshes[m].verts[v].weight;

			freyjaVertexWeight(v, 
							   md5.mMeshes[m].weights[w].weight,
							   md5.mMeshes[m].weights[w].joint);

			/* Generates id translator list */
			weights.pushBack(md5.mMeshes[m].weights[w].index);
		}
	}

	freyjaEnd(); // FREYJA_MODEL

	freyjaBegin(FREYJA_SKELETON);

	Vector3d transforms[md5.mNumJoints];
	Vector3d origin;

	for (j = 0; j < md5.mNumJoints; ++j)
	{
		transforms[j].zero();
	}

	/* Mongoose 2004.12.21, 
	 * Md5 stores absolution bone pos -- make them offsets from parent */
	for (j = 0; j < md5.mNumJoints; ++j)
	{
		vec3 = Vector3d(md5.mJoints[j].translate[0],
						md5.mJoints[j].translate[1],
						md5.mJoints[j].translate[2]);
		//printf("%3i   %.3f %.3f %.3f\n", j,
		//	   vec3.mVec[0], vec3.mVec[1], vec3.mVec[2]);
		index = j;

		while (index > -1)
		{
			vec3 -= transforms[index];
			index = md5.mJoints[index].parent;
		}

		transforms[j] = vec3;
		//printf("->    %.3f %.3f %.3f\n",
		//	   vec3.mVec[0], vec3.mVec[1], vec3.mVec[2]);
	}

	for (j = 0; j < md5.mNumJoints; ++j)
	{
		/* Start a new tag */
		freyjaBegin(FREYJA_BONE);
		bindex = freyjaGetCurrent(FREYJA_BONE);
		freyjaBoneFlags1i(bindex, 0x0);
		freyjaBoneParent1i(bindex, md5.mJoints[j].parent);
		freyjaBoneName1s(bindex, md5.mJoints[j].name);

		/* Translate */
		vec3 = transforms[j];
		vec3 *= scale;

		if (!j || 1)
		{
			freyjaBoneTranslate3f(bindex,
								  vec3.mVec[0], vec3.mVec[2], vec3.mVec[1]);
		}
		else
		{
			freyjaBoneTranslate3f(bindex, 
								  vec3.mVec[0], vec3.mVec[1], vec3.mVec[2]);
		}

		/* Scale */
		index = j;
		mat.setIdentity();

		while (index > -1)
		{
			index = md5.mJoints[index].parent;

			q2 = Quaternion(qw, 
							md5.mJoints[index].rotate[0],
							md5.mJoints[index].rotate[1],
							md5.mJoints[index].rotate[2]);
			mat2 = q2;
			mat = mat * mat2;
		}

		mat.invert();
		mat2 = Quaternion(qw, 
						  md5.mJoints[j].rotate[0],
						  md5.mJoints[j].rotate[1],
						  md5.mJoints[j].rotate[2]);
		mat = mat * mat2;
		q.setByMatrix(mat.mMatrix);
		q.getQuaternion4fv(wxyz);

		//freyjaBoneRotateQuatWXYZ4fv(bindex, wxyz); // argh

		for (int j2 = 0; j2 < md5.mNumJoints; ++j2)
		{
			if (md5.mJoints[j2].parent == j)
			{ 
				freyjaBoneAddChild1i(bindex, j2);
			}
		}

		freyjaEnd(); // FREYJA_TAG
	}

	freyjaEnd(); // FREYJA_SKELETON


	freyjaPrintMessage("The Doom3 plugin sez: FINISH ME!!!");

	return 0;
}


int freyja_model__md5_export(char *filename)
{
	freyjaPrintError("md5_export> ERROR: Not implemented.\n");
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
