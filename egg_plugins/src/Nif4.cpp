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


	if (r.loadFile(filename) == false)
		return false;

	/* Get header and check to see if this is a Nifv4.0.0.2 */
	r.getCharString(mHeaderSz, mHeader);

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
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_NIF4
int runNif4UnitTest(int argc, char *argv[])
{
	Nif4 test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[Nif4 class test]\n");

	return runNif4UnitTest(argc, argv);
}
#endif
