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

#include "Egg9.h"


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Egg::Egg()
{
	mDebugLevel = 0;
}


Egg::~Egg()
{
	erase();
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void Egg::setDebugLevel(unsigned int n)
{
	mDebugLevel = n;
}


void Egg::erase()
{
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////

bool Egg::isDebugLevel(unsigned int level)
{
	return (mDebugLevel >= level);
}


void Egg::print(char *s, ...)
{
	va_list args;

	va_start(args, s);
	fprintf(stdout, "Egg::");
	vfprintf(stdout, s, args);
	fprintf(stdout, "\n");
	va_end(args);
}


void Egg::printError(char *s, ...)
{
	va_list args;
	
	va_start(args, s);
	fprintf(stderr, "Egg::");
	vfprintf(stderr, s, args);
	fprintf(stderr, "\n");
	va_end(args);
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
	Egg egg;


	printf("[Egg class test]\n");
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
