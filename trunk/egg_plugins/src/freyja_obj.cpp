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
	if (EggFileReader::compareFilenameExtention(filename, ".obj") == 0)
	{
		eggPrintMessage("obj.so: '%s' is an obj model", filename);
		return 0;
	}

	return -1;
}


int freyja_model__obj_import(char *filename)
{
	EggFileReader r;
	Vector <unsigned int> transV;
	Vector <unsigned int> transT;
	//Vector <unsigned int> transN;
	char *symbol;
	vec_t x, y, z, u, v;
	int i, index;
	bool hasUV = false;
	bool hasNormal = false;


	if (!r.openFile(filename))
	{
		return -1;
	}

	// Start a new mesh
	eggBegin(FREYJA_MESH);
	eggBegin(FREYJA_GROUP);

	while ((symbol = r.parseSymbol()) && !r.endOfFile())
	{
		if (!strncmp(symbol, "#", 1))
		{
			char c;

			while ((c = r.readInt8()) && !r.endOfFile())
			{
				if (c == '\n')
					break;
			}
		}
		else if (!strncmp(symbol, "usemtl", 6))
		{
			// Filename of the material
			symbol = r.parseSymbol();
			eggPrintMessage("FIXME: Material '%s' is not used\n", symbol);
		}
		else if (!strncmp(symbol, "vt", 2))
		{
			u = r.parseFloat();
			v = r.parseFloat();
			hasUV = true;

			transT.pushBack(eggTexCoordStore2f(u, v));
		}
		else if (!strncmp(symbol, "vn", 2))  // Assumes normals come after v
		{
			x = r.parseFloat();
			y = r.parseFloat();
			z = r.parseFloat();
			hasNormal = true;

			// polymapped normals not in Egg
			//transN.pushBack(eggNormal3f(x, y, z));
		}
		else if (!strncmp(symbol, "v", 1))
		{
			x = r.parseFloat();
			y = r.parseFloat();
			z = r.parseFloat();

			transV.pushBack(eggVertexStore3f(x, y, z));
		}
		else if (!strncmp(symbol, "g", 1))
		{
			// Name of the new group, but no used here
			symbol = r.parseSymbol();
			eggPrintMessage("FIXME: Group '%s' is not used\n", symbol);
		}
		else if (!strncmp(symbol, "s", 1))
		{
			index = r.parseInteger();  // Don't know what this is
		}
		else if (!strncmp(symbol, "f", 1))
		{
			// Start a new polygon
			eggBegin(FREYJA_POLYGON);

			for (i = 0; i < 3; ++i)
			{
				index = r.parseInteger();
				eggVertex1i(transV[index-1]); // index starts at 1 not 0

				if (hasUV)
				{
					r.readInt8(); // '/' no whitespace
					index = r.parseInteger();

					index = transT[index-1]; // index starts at 1 not 0
					eggTexCoord1i(index);
				}
				else
				{
					eggVertexUVStore2f(index, 0.25*i, 0.25*(i%2));
				}

				if (hasNormal) // polymapped normals aren't in Egg atm
				{
					r.readInt8(); // '/' no whitespace
					index = r.parseInteger();

					//index = transN[index-1]; // index starts at 1 not 0
					//eggNormal1i(index);
				}
			}

			// Handle quad faces
			i = r.getFileOffset();
			symbol = r.parseSymbol();
			r.setFileOffset(i);

			if (isdigit(symbol[0]))
			{
				index = r.parseInteger();
				eggVertex1i(transV[index-1]); // index starts at 1 not 0

				if (hasUV)
				{
					r.readInt8(); // '/' no whitespace
					index = r.parseInteger();

					index = transT[index-1]; // index starts at 1 not 0
					eggTexCoord1i(index);
				}
				else
				{
					eggVertexUVStore2f(index, 0.25*i, 0.25*(i%2));
				}

				if (hasNormal) // polymapped normals aren't in Egg atm
				{
					r.readInt8(); // '/' no whitespace
					index = r.parseInteger();

					//index = transN[index-1]; // index starts at 1 not 0
					//eggNormal1i(index);
				}
			}

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
