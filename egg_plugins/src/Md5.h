/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : Md5
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: Doom3 model plugin, was pretty easy to figure out the format
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_MD5 - Builds Md5 class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.08.13:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_MD5_H_
#define GUARD__FREYJA_MONGOOSE_MD5_H_

#include <stdio.h>
#include <string.h>

/* NOTES: Some indices and counts are unfortunately signed (negative) values */
class Md5Vertex
{
 public:
	int index;
	float uv[2];
	int weight;
	int numbones;
};


class Md5Triangle
{
 public:
	int vertex[3];
};


class Md5Weight
{
 public:
	int index;
	int joint;
	float weight;
	float pos[3];
};


class Md5Mesh
{
 public:
	char *shader;
	int numverts;
	Md5Vertex *verts;
	int numtriangles;
	Md5Triangle *triangles;
	int numweights;
	Md5Weight *weights;
};


class Md5Joint
{
 public:
	char *name;
	int index;
	double translate[3];
	double rotate[3];
};


class Md5
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Md5();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Md5
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~Md5();
	/*------------------------------------------------------
	 * Pre  : Md5 object is allocated
	 * Post : Deconstructs an object of Md5
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	bool isMd5Model(const char *filename);
	/*------------------------------------------------------
	 * Pre  : Checks to see if file is an Md5mesh model
	 * Post : Returns true if it is a Md5mesh
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	bool loadModel(const char *filename);
	/*------------------------------------------------------
	 * Pre  : Parses and stores a Md5mesh model from a file
	 * Post : Returns true if sucessful
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	void closeFile();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Closes attribute file if open
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	double getFloat();
	/*------------------------------------------------------
	 * Pre  : loadFile must have been sucessful, etc
	 * Post : Returns a parsed float
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int getInteger();
	/*------------------------------------------------------
	 * Pre  : loadFile must have been sucessful, etc
	 * Post : Returns a parsed signed integer
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	char *getString();
	/*------------------------------------------------------
	 * Pre  : loadFile must have been sucessful, etc
	 * Post : Returns an allocated C string
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	char *getSymbol();
	/*------------------------------------------------------
	 * Pre  : loadFile must have been sucessful, etc
	 * Post : Returns a parsed symbol 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	bool loadFile(const char *filename);
	/*------------------------------------------------------
	 * Pre  : Loads a file from filename
	 * Post : Returns true if sucessful
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	bool matchSymbol(const char *symbol);
	/*------------------------------------------------------
	 * Pre  : loadFile must have been sucessful, etc
	 * Post : Returns true if symbol found matches string
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	int mVersion;                     /* MD5 Version */

	char *mCommandLine;               /* Id cruft */

	int mNumJoints;                   /* Joint count */

	int mNumMeshes;                   /* Mesh count */

	Md5Joint *mJoints;                /* Model skeleton */

	Md5Mesh *mMeshes;                 /* Model geometery */

	FILE *mFileHandle;                /* File parsing cruft */
	char *mTempBufferHack;
	unsigned int mTempBufferHackSz;
};

#endif
