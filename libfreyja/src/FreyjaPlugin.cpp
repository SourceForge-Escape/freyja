/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : Plugin
 * License : GPL, also (C) 2000 Mongoose
 * Comments: This is the plugin system with C exported API.
 *
 * 
 *-- Test Defines -------------------------------------------
 *           
 * TEST_PLUGIN  Builds module test code as a console program
 *
 *-- History ------------------------------------------------ 
 *
 * 2001.02.24:
 * Mongoose - Created, based on python test code
 *
 ==========================================================================*/

#ifdef WIN32
#   undef FREYJA_PLUGINS
#endif

#ifdef FREYJA_PLUGINS
#   include <dlfcn.h> 
#endif

#include <sys/types.h>
#include <dirent.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <hel/math.h>
#include <hel/Quaternion.h>
#include <hel/Vector3d.h>
#include <hel/BoundingVolume.h>

#include "FreyjaFileReader.h"
#include "FreyjaPlugin.h"



////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_FREYJAPLUGIN

int runFreyjaPluginUnitTest(int argc, char *argv[])
{
	FreyjaPlugin ep;

	return 0;
}


int main(int argv, char *argc[])
{
 	printf("[FreyjaPlugin class test]\n");

	return runFreyjaPluginUnitTest(argc, argv); 
  
	return 0;
}

#endif
