/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : stu7440@westga.edu
 * Object  : 
 * Comments: This is the Obj plug-in module
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
#include <freyja8/EggPlugin.h>
#include <freyja8/EggFileReader.h>


extern "C" {

	int freyja_model__obj_check(char *filename);
	int freyja_model__obj_import(char *filename);
	int freyja_model__obj_export(char *filename);
}



int freyja_model__obj_check(char *filename)
{
	if (EggFileReader::compareFilenameExtention(filename, ".smd") == 0)
		return 0;

	return -1;
}


int freyja_model__obj_import(char *filename)
{
	EggFileReader r;
	Vector <unsigned int> transV;
	char *symbol;
	vec_t x, y, z;
	int i, a, b, c;


	if (!r.openFile(filename))
	{
		return -1;
	}


	// Start a new mesh
	eggBegin(FREYJA_MESH);
	eggBegin(FREYJA_GROUP);

	while ((symbol = r.parseSymbol()))
	{
		if (!strncmp(symbol, "v", 1))
		{
			x = r.parseFloat();
			y = r.parseFloat();
			z = r.parseFloat();

			transV.pushBack(eggVertexStore3f(x, y, z));
		}
		else if (!strncmp(symbol, "f", 1))
		{
			// Start a new polygon
			eggBegin(FREYJA_POLYGON);

			// Format doesn't have UVWs
			i = eggTexCoordStore2f(0.5, 0.5);
			eggTexCoord1i(i);
			i = eggTexCoordStore2f(0.25, 0.25);
			eggTexCoord1i(i);
			i = eggTexCoordStore2f(0.5, 0.25);
			eggTexCoord1i(i);

			a = r.parseInteger();
			b = r.parseInteger();
			c = r.parseInteger();

			eggVertex1i(transV[a]);
			eggVertex1i(transV[b]);
			eggVertex1i(transV[c]);

			eggTexture1i(0);

			eggEnd(); // FREYJA_POLYGON
		}
	}

	eggEnd(); // FREYJA_GROUP
	eggEnd(); // FREYJA_MESH

	return 0;
}


int freyja_model__obj_export(char *filename)
{
  printf("freyja_model__obj_export> Not implemented, %s:%i\n", 
	 __FILE__, __LINE__);
  return -1;
}
