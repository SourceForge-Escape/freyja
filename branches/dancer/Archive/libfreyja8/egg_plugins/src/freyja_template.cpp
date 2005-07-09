/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : stu7440@westga.edu
 * Object  : 
 * Comments: This is the Template plug-in module
 *
 *           See file COPYING for license details.
 *
 *
 *-- Test Defines -----------------------------------------------------
 *           
 *
 *-- History ---------------------------------------------------------- 
 *
 * 2001-05-09:
 * Mongoose - Created
 ==========================================================================*/

#include <stdio.h>
#include <string.h>
#include <freyja_egg/EggPlugin.h>
// #include "Template.h"


extern "C" {

	int freyja_model__template_check(char *filename);
	int freyja_model__template_import(char *filename);
	int freyja_model__template_export(char *filename);
}



int freyja_model__template_check(char *filename)
{
	printf("freyja_model__template_export> Not implemented, %s:%i\n", 
			 __FILE__, __LINE__);
	return -1;
}


int freyja_model__template_import(char *filename)
{
	printf("freyja_model__template_import> Not implemented, %s:%i\n", 
			 __FILE__, __LINE__);
	return -1;
}


int freyja_model__template_export(char *filename)
{
	printf("freyja_model__template_export> Not implemented, %s:%i\n", 
			 __FILE__, __LINE__);
	return -1;
}
