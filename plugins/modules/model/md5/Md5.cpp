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
#include <mstl/SystemIO.h>
#include <mstl/String.h>

#include "Md5.h"

using namespace mstl;
using namespace hel;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Md5Model::Md5Model() :
	mVersion(10),
	mCommandLine(),
	mNumJoints(0),
	mNumMeshes(0),
	mJoints(NULL),
	mMeshes(NULL)
{
}


Md5Model::~Md5Model()
{
	if (mJoints)
		delete [] mJoints;
	
	if (mMeshes)
		delete [] mMeshes;
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

void Md5Model::DecodeUnitQuaternion(hel::Quat &q)
{
	// Md5 stores unit quaternions without the W componet.
	float tmp = 1.0f - (q.mX*q.mX) - (q.mY*q.mY) - (q.mZ*q.mZ);

	q.mW = 0.0f;

	if (tmp >= 0.0f)
	{
		q.mW = -sqrtf( tmp );
	}
}


hel::Vec3 Md5Model::EncodeQuaternion(const hel::Quat &q)
{
	// Md5 stores unit quaternions without the W componet.
	Quat n;
	n = q;
	n.Norm();

	return Vec3(n.mX, n.mY, n.mZ);
}


bool Md5Model::IsMd5Model(const char *filename)
{
	SystemIO::TextFileReader r;

	if (r.Open(filename) == false)
		return false;

	if (r.ParseMatchingSymbol("MD5Version"))
		return true;

	r.Close();

	return false;
}


bool Md5Model::SaveModel(const char *filename)
{
	SystemIO::TextFileWriter w;

	if (w.Open(filename) == false)
		return false;

	/* Header */
	w.Print("MD5Version %u\n", mVersion);

	/* Some Id thingys */
	w.Print("commandline \"%s\"\n\n", mCommandLine.c_str() );

	/* Joint and mesh counts */
	w.Print("numJoints %i\n", mNumJoints);
	w.Print("numMeshes %i\n\n", mNumMeshes);

	/* Joint data */
	w.Print("joints {\n");

	for (unsigned int i = 0; i < mNumJoints; ++i)
	{
		w.Print("\t\"%s\"\t%i ", mJoints[i].name.c_str(), mJoints[i].parent);

		/* translate X Y Z */
		w.Print("( %f %f %f ) ",
			mJoints[i].translate[0],
			mJoints[i].translate[1],
			mJoints[i].translate[2]);

		Vec3 rot = EncodeQuaternion( mJoints[i].rotate );
		w.Print("( %f %f %f )\t\t", rot.mX, rot.mY, rot.mZ);

		/* Recover metadata of parent comment */
		w.Print("// ");

		if (mJoints[i].parent > 0)
		{
			w.Print("%s", mJoints[mJoints[i].parent].name.c_str() );
		}

		w.Print("\n");
	}

	w.Print("}\n");

	/* Write mesh data*/
	for (unsigned int i = 0; i < mNumMeshes; ++i)
	{
		w.Print("\n");
		w.Print("mesh {\n");
		w.Print("\t// meshes: %s\n", mMeshes[i].name.c_str() );
		w.Print("\tshader \"%s\"\n", mMeshes[i].shader.c_str() );

		w.Print("\n\tnumverts %i\n", mMeshes[i].numverts);

		for (unsigned int j = 0, 
			 jn = (mMeshes[i].numverts < 0) ? 0 : mMeshes[i].numverts; 
			 j < jn; ++j)
		{
			w.Print("\tvert %i ( %lf %lf ) %i %i\n", 
					j, // mMeshes[i].verts[j].index // Should == j
					mMeshes[i].verts[j].uv[0], 
					mMeshes[i].verts[j].uv[1],
					mMeshes[i].verts[j].weight,
					mMeshes[i].verts[j].numbones);
		}

		w.Print("\n\tnumtris %i\n", mMeshes[i].numtriangles);

		for (unsigned int j = 0,
			 jn = (mMeshes[i].numtriangles < 0) ? 0 : mMeshes[i].numtriangles; 
			 j < jn; ++j)
		{
			w.Print("\ttri %i %i %i %i\n",
				j,
				mMeshes[i].triangles[j].vertex[0],
				mMeshes[i].triangles[j].vertex[1],
				mMeshes[i].triangles[j].vertex[2]);
		}

		w.Print("\n\tnumweights %i\n", mMeshes[i].numweights);

		for (unsigned int j = 0,
			 jn = (mMeshes[i].numweights < 0) ? 0 : mMeshes[i].numweights; 
			 j < jn; ++j)
		{
			w.Print("\tweight %i %i %f ( %f %f %f )\n",
					j, // mMeshes[i].weights[j].index
					mMeshes[i].weights[j].joint,
					mMeshes[i].weights[j].weight,
					mMeshes[i].weights[j].pos[0],
					mMeshes[i].weights[j].pos[1],
					mMeshes[i].weights[j].pos[2]);			
		}
		
		w.Print("}\n");
	}

	w.Close();

	return true;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

bool Md5Model::LoadModel(const char *filename)
{
	SystemIO::TextFileReader r;

	if (IsMd5Model(filename) == false)
		return false;

	if (r.Open(filename) == false)
		return false;

	if (!r.ParseMatchingSymbol("MD5Version"))
		return false;

	mVersion = r.ParseInteger();

	switch (mVersion)
	{
	case 10:
		break;

	case 11:
		return LoadQuakeWarsModel( r );
		break;

	default:
		printf("<Md5> Don't know this version %i.\n", mVersion);
		return false;
	}


	/* Id model commandline. */
	if (!r.ParseMatchingSymbol("commandline"))
		return false; 

	mCommandLine = r.ParseStringLiteral();
	r.FindNextChar('\n'); // skip very long commandlines that don't fit limit.

	/* Joint setup */
	if (!r.ParseMatchingSymbol("numJoints"))
		return false;

	{
		int i = r.ParseInteger();

		if (i < 0)
		{
			mNumJoints = 0;
		}
		else
		{
			mNumJoints = i;
			mJoints = new Md5Joint[mNumJoints];
		}
	}


	/* Mesh setup */
	if (!r.ParseMatchingSymbol("numMeshes"))
		return false;

	{
		int i = r.ParseInteger();

		if (i < 0)
		{
			mNumMeshes = 0;
		}
		else
		{
			mNumMeshes = i;
			mMeshes = new Md5Mesh[mNumMeshes];
		}
	}


	/* Joint data */
	if (mNumJoints && r.ParseMatchingSymbol("joints"))
	{
		if (!r.ParseMatchingSymbol("{"))
			return false;
			
		for (unsigned int i = 0; i < mNumJoints; ++i)
		{
			Md5Joint &joint = mJoints[i];

			joint.name = r.ParseStringLiteral();
			joint.parent = r.ParseInteger();

			/* translate X Y Z */
			if (!r.ParseMatchingSymbol("("))
				return false;

			joint.translate.mX = r.ParseFloat();
			joint.translate.mY = r.ParseFloat();
			joint.translate.mZ = r.ParseFloat();

			if (!r.ParseMatchingSymbol(")"))
				return false;

			/* rotate X Y Z */
			if (!r.ParseMatchingSymbol("("))
				return false;
			
			joint.rotate.mX = r.ParseFloat();
			joint.rotate.mY = r.ParseFloat();
			joint.rotate.mZ = r.ParseFloat();

			DecodeUnitQuaternion(joint.rotate);

			if (!r.ParseMatchingSymbol(")"))
				return false;

			// FIXME: Save comment at end of line for something?
		}
		
		if (!r.ParseMatchingSymbol("}"))
			return false;
	}
	else
	{
		return false;
	}

	
	for (unsigned int i = 0; i < mNumMeshes; ++i)
	{
		if (r.ParseMatchingSymbol("mesh"))
		{
			if (!r.ParseMatchingSymbol("{"))
				return false;

			// FIXME: // meshes: MESHNAME?

			if (!r.ParseMatchingSymbol("shader"))
				return false;

			mMeshes[i].shader = r.ParseStringLiteral();

			// Get mesh name from shader
			{
				int j = mMeshes[i].shader.find_last_of('/');
				if (String::npos != j)
				{
					mMeshes[i].name = ( mMeshes[i].shader.c_str() )+j+1;
				}
				else
				{
					mMeshes[i].name.Set("mesh-%i", i);
				}
			}

			if (!r.ParseMatchingSymbol("numverts"))
				return false;

			{
				int j = r.ParseInteger();

				if (j < 0)
				{
					mMeshes[i].numverts = 0;
				}
				else
				{
					mMeshes[i].numverts = j;
					mMeshes[i].verts = new Md5Vertex[mMeshes[i].numverts];
				}
			}

			for (unsigned int j = 0; j < mMeshes[i].numverts; ++j)
			{
				if (!r.ParseMatchingSymbol("vert"))
					return false;

				//mMeshes[i].verts[j].index = 
				r.ParseInteger();

				if (!r.ParseMatchingSymbol("("))
					return false;

				mMeshes[i].verts[j].uv[0] = r.ParseFloat(); 
				mMeshes[i].verts[j].uv[1] = r.ParseFloat();

				if (!r.ParseMatchingSymbol(")"))
					return false;
				
				mMeshes[i].verts[j].weight = r.ParseInteger();
				mMeshes[i].verts[j].numbones = r.ParseInteger();
			}

			if (!r.ParseMatchingSymbol("numtris"))
				return false;

			mMeshes[i].numtriangles = r.ParseInteger();
			mMeshes[i].triangles = new Md5Triangle[mMeshes[i].numtriangles];

			for (unsigned int j = 0; j < mMeshes[i].numtriangles; ++j)
			{
				if (!r.ParseMatchingSymbol("tri"))
					return false;
				
				r.ParseInteger(); // integer == j
				mMeshes[i].triangles[j].vertex[0] = r.ParseInteger();
				mMeshes[i].triangles[j].vertex[1] = r.ParseInteger();
				mMeshes[i].triangles[j].vertex[2] = r.ParseInteger();
			}

			if (!r.ParseMatchingSymbol("numweights"))
				return false;

			mMeshes[i].numweights = r.ParseInteger();
			mMeshes[i].weights = new Md5Weight[mMeshes[i].numweights];

			for (unsigned int j = 0; j < mMeshes[i].numweights; ++j)
			{
				if (!r.ParseMatchingSymbol("weight"))
					return false;

				Md5Weight &w = mMeshes[i].weights[j];

				//w.index = 
				r.ParseInteger();
				w.joint = r.ParseInteger();
				w.weight = r.ParseFloat();

				if (!r.ParseMatchingSymbol("("))
					return false;

				w.pos.mX = r.ParseFloat();
				w.pos.mY = r.ParseFloat();
				w.pos.mZ = r.ParseFloat();

				if (!r.ParseMatchingSymbol(")"))
					return false;
			}
			
		}
		
		if (!r.ParseMatchingSymbol("}"))
			return false;
	}

	r.Close();

	return true;
}


bool Md5Model::LoadQuakeWarsModel(mstl::SystemIO::TextFileReader &r)
{
	/* Id model commandline. */
	if (!r.ParseMatchingSymbol("commandline"))
		return false; 

	mCommandLine = r.ParseStringLiteral();
	r.FindNextChar('\n'); // skip very long commandlines that don't fit limit.

	/* Joint setup */
	if (!r.ParseMatchingSymbol("numJoints"))
		return false;

	{
		int i = r.ParseInteger();

		if (i < 0)
		{
			mNumJoints = 0;
		}
		else
		{
			mNumJoints = i;
			mJoints = new Md5Joint[mNumJoints];
		}
	}


	/* Mesh setup */
	if (!r.ParseMatchingSymbol("numMeshes"))
		return false;

	{
		int i = r.ParseInteger();

		if (i < 0)
		{
			mNumMeshes = 0;
		}
		else
		{
			mNumMeshes = i;
			mMeshes = new Md5Mesh[mNumMeshes];
		}
	}


	/* Joint data */
	if (mNumJoints && r.ParseMatchingSymbol("joints"))
	{
		if (!r.ParseMatchingSymbol("{"))
			return false;
			
		for (unsigned int i = 0; i < mNumJoints; ++i)
		{
			Md5Joint &joint = mJoints[i];

			joint.name = r.ParseStringLiteral();
			joint.parent = r.ParseInteger();

			/* translate X Y Z */
			if (!r.ParseMatchingSymbol("("))
				return false;

			joint.translate.mX = r.ParseFloat();
			joint.translate.mY = r.ParseFloat();
			joint.translate.mZ = r.ParseFloat();

			if (!r.ParseMatchingSymbol(")"))
				return false;

			/* rotate X Y Z */
			if (!r.ParseMatchingSymbol("("))
				return false;
			
			joint.rotate.mX = r.ParseFloat();
			joint.rotate.mY = r.ParseFloat();
			joint.rotate.mZ = r.ParseFloat();

			DecodeUnitQuaternion(joint.rotate);

			if (!r.ParseMatchingSymbol(")"))
				return false;

			// FIXME: Save comment at end of line for something?
		}
		
		if (!r.ParseMatchingSymbol("}"))
			return false;
	}
	else
	{
		return false;
	}

	
	for (unsigned int i = 0; i < mNumMeshes; ++i)
	{
		if (r.ParseMatchingSymbol("mesh"))
		{
			if (!r.ParseMatchingSymbol("{"))
				return false;

			// FIXME: // meshes: MESHNAME?

			if (!r.ParseMatchingSymbol("name"))
				return false;

			mMeshes[i].name = r.ParseStringLiteral();

			if (!r.ParseMatchingSymbol("shader"))
				return false;

			mMeshes[i].shader = r.ParseStringLiteral();

			// Get mesh name from shader
			if ( mMeshes[i].name.c_str() == NULL )
			{
				int j = mMeshes[i].shader.find_last_of('/');
				if (String::npos != j)
				{
					mMeshes[i].name = ( mMeshes[i].shader.c_str() )+j+1;
				}
				else
				{
					mMeshes[i].name.Set("mesh-%i", i);
				}
			}

			if ( r.ParseMatchingSymbol("flags") )
			{
				r.ParseMatchingSymbol("{");

				while ( !r.ParseMatchingSymbol("}") )
					;  // FIXME: Store flags
			}

			if (!r.ParseMatchingSymbol("numverts"))
				return false;

			{
				int j = r.ParseInteger();

				if (j < 0)
				{
					mMeshes[i].numverts = 0;
				}
				else
				{
					mMeshes[i].numverts = j;
					mMeshes[i].verts = new Md5Vertex[mMeshes[i].numverts];
				}
			}

			for (unsigned int j = 0; j < mMeshes[i].numverts; ++j)
			{
				if (!r.ParseMatchingSymbol("vert"))
					return false;

				//mMeshes[i].verts[j].index = 
				r.ParseInteger();

				if (!r.ParseMatchingSymbol("("))
					return false;

				mMeshes[i].verts[j].uv[0] = r.ParseFloat(); 
				mMeshes[i].verts[j].uv[1] = r.ParseFloat();

				if (!r.ParseMatchingSymbol(")"))
					return false;
				
				mMeshes[i].verts[j].weight = r.ParseInteger();
				mMeshes[i].verts[j].numbones = r.ParseInteger();


				if (!r.ParseMatchingSymbol("("))
					return false;

				// FIXME: Unused for now.
				r.ParseInteger();
				r.ParseInteger();
				r.ParseInteger();
				r.ParseInteger();

				if (!r.ParseMatchingSymbol(")"))
					return false;
			}

			if (!r.ParseMatchingSymbol("numtris"))
				return false;

			mMeshes[i].numtriangles = r.ParseInteger();
			mMeshes[i].triangles = new Md5Triangle[mMeshes[i].numtriangles];

			for (unsigned int j = 0; j < mMeshes[i].numtriangles; ++j)
			{
				if (!r.ParseMatchingSymbol("tri"))
					return false;
				
				r.ParseInteger(); // integer == j
				mMeshes[i].triangles[j].vertex[0] = r.ParseInteger();
				mMeshes[i].triangles[j].vertex[1] = r.ParseInteger();
				mMeshes[i].triangles[j].vertex[2] = r.ParseInteger();
			}

			if (!r.ParseMatchingSymbol("numweights"))
				return false;

			mMeshes[i].numweights = r.ParseInteger();
			mMeshes[i].weights = new Md5Weight[mMeshes[i].numweights];

			for (unsigned int j = 0; j < mMeshes[i].numweights; ++j)
			{
				if (!r.ParseMatchingSymbol("weight"))
					return false;

				Md5Weight &w = mMeshes[i].weights[j];

				//w.index = 
				r.ParseInteger();
				w.joint = r.ParseInteger();
				w.weight = r.ParseFloat();

				if (!r.ParseMatchingSymbol("("))
					return false;

				w.pos.mX = r.ParseFloat();
				w.pos.mY = r.ParseFloat();
				w.pos.mZ = r.ParseFloat();

				if (!r.ParseMatchingSymbol(")"))
					return false;
			}
			
		}
		
		if (!r.ParseMatchingSymbol("}"))
			return false;
	}

	r.Close();

	return true;
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_MD5
int runMd5UnitTest(int argc, char *argv[])
{
	Md5Model test;

	if (argc > 2)
	{
		if (strcmp(argv[1], "load") == 0)
		{
			if (test.LoadModel(argv[2]) == false)
				printf("UNIT_TEST: Load reports error.\n");

			//test.print();
			return 0;
		}
		else if (strcmp(argv[1], "save") == 0 && argc > 3)
		{
			if (test.LoadModel(argv[2]) == false)
				printf("UNIT_TEST: Load reports error.\n");

			//test.print();

			if (test.SaveModel(argv[3]) == false)
				printf("UNIT_TEST: Save reports error.\n");

			//test.print();
			return 0;
		}
	}

	printf("\n\n");
	printf("%s load filenameLoad.md5mesh\n", argv[0]);
	printf("%s save filenameLoad.md5mesh filenameSave.md5mesh\n", argv[0]);

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[Md5 class test]\n");

	return runMd5UnitTest(argc, argv);
}
#endif
