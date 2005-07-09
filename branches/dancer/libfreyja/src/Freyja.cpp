/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : mongoose@users.sourceforge.net
 * Object  : Egg
 * License : GPL, also (C) 2000 Mongoose
 * Comments: This is the data model agent
 *
 * 
 *-- History -----------------------------------------------
 *
 * 2000-12-09:
 * Mongoose - Move to new data structures
 *
 * 2000-07-01:
 * Mongoose - Rebuilt backend
 *
 * 2000-05-09:
 * Mongoose - Almost complete rewrite
 *
 * 1999-07-31:
 * Mongoose - Created ( After prototyping in C for a week )
 ==========================================================================*/

#include <sys/types.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

#include "FreyjaFileWriter.h"
#include "Freyja.h"



////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

FreyjaScene::FreyjaScene()
{
	mDebugLevel = 0;
}


FreyjaScene::~FreyjaScene()
{
	erase();
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

bool FreyjaScene::exportScene(const char *filename)
{
	FreyjaFileWriter w;

	// FIXME: not implemented

	return false;
}

unsigned int FreyjaScene::getMeshCount() { return meshes.end(); }
unsigned int FreyjaScene::getSkeletonCount() { return skeletons.end(); }
unsigned int FreyjaScene::getAnimationCount() { return animations.end(); }
unsigned int FreyjaScene::getModelCount() { return models.end(); }
unsigned int FreyjaScene::getMaterialCount() { return materials.end(); }
unsigned int FreyjaScene::getTextureCount() { return textures.end(); }
unsigned int FreyjaScene::getVertexGroupCount() { return vertexgroups.end(); }


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void FreyjaScene::setDebugLevel(unsigned int n)
{
	mDebugLevel = n;
}


void FreyjaScene::erase()
{
	models.erase();
	materials.erase();
	textures.erase();
	animations.erase();
	skeletons.erase();
	meshes.erase();
	metadata.erase();
	lights.erase();
	cameras.erase();
	vertexgroups.erase();
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////

bool FreyjaScene::isDebugLevel(unsigned int level)
{
	return (mDebugLevel >= level);
}


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test Code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_EGG
int main(int argc, char *argv[])
{
	FreyjaScene egg;


	printf("[FreyjaScene class test]\n");
	egg.setDebugLevel(4); // 5 for a lot of crap

	if (argc > 2)
	{
		if (strcmp(argv[1], "load") == 0)
		{
			if (!egg.loadFile(argv[2]))
				printf("main: Load reports success.\n");
		}
		else if (strcmp(argv[1], "save") == 0)
		{
			// FIXME: Add code here to generate a small egg

			if (!egg.saveFile(argv[2]))
				printf("main: Save reports success.\n");
		}
		else if (strcmp(argv[1], "test") == 0 && argc > 3)
		{
			if (!egg.loadFile(argv[2]))
				printf("main: Load reports success.\n");
			if (!egg.saveFile(argv[3]))
				printf("main: Save reports success.\n");
			if (!egg.loadFile(argv[3]))
				printf("main: Load reports success.\n");
		}
		else
		{
			printf("\n\n%s [save | load | test] filename.egg [testout.egg]\n", 
					 argv[0]);
		}
	}
	else
	{
		printf("\n\n%s [save | load | test] filename.egg [testout.egg]\n", 
				 argv[0]);
	}

	return 0;
}
#endif
