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

	int freyja_model__smd_check(char *filename);
	int freyja_model__smd_import(char *filename);
	int freyja_model__smd_export(char *filename);
}



int freyja_model__smd_check(char *filename)
{
	if (EggFileReader::compareFilenameExtention(filename, ".smd") == 0)
		return 0;

	return -1;
}


typedef struct {

	int parent;
	char *name;

} smd_bone_t;


int freyja_model__smd_import(char *filename)
{
	EggFileReader r;
	Vector <unsigned int> transV;
	Vector <smd_bone_t *> bones;
	char *symbol;
	vec_t x, y, z, rx, ry, rz;
	int index;
	unsigned int i, n;
	vec_t scale = 0.15;


	if (freyja_model__smd_check(filename) < 0 || !r.openFile(filename))
	{
		return -1;
	}

	while ((symbol = r.parseSymbol()) && !r.endOfFile())
	{
		if (!strncmp(symbol, "version", 7))
		{
			index = r.parseInteger();
		}
		else if (!strncmp(symbol, "nodes", 5))
		{
			while ((symbol = r.parseSymbol()) && strncmp(symbol, "end", 3) != 0)
			{
				smd_bone_t *bone = new smd_bone_t;

				if (!symbol)
					break;

				index = atoi(symbol);
				bone->name = r.parseString();
				bone->parent = r.parseInteger();
				bones.pushBack(bone);
			}
		}
		else if (!strncmp(symbol, "skeleton", 8))
		{
			eggBegin(FREYJA_SKELETON);

			symbol = r.parseSymbol();
			index = r.parseInteger();  // time

			printf("%s = %i\n", symbol, index);

			while ((symbol = r.parseSymbol()) && strncmp(symbol, "end", 3) != 0)
			{
				if (!symbol)
					break;

				index = atoi(symbol);
				//printf("%s <- %i?\n", symbol, index);

				smd_bone_t *bone = bones[index];
				//printf("bone[%i]\n", index);

				if (bone && index < (int)bones.end())
				{
					x = r.parseFloat();
					y = r.parseFloat();
					z = r.parseFloat();

					rx = r.parseFloat();
					ry = r.parseFloat();
					rz = r.parseFloat();

					//printf("{\n%s\n%i\n}", bone->name, bone->parent);
					eggBegin(FREYJA_BONE);
					eggTagFlags1u(0x0);
					eggSetBoneParent(bone->parent);
					eggTagName(bone->name);

					if (!index)
					{
						eggTagPos3f(x*scale, z*scale, y*scale);
						eggTagRotate3f(-90, -90, 0);
					}
					else
					{
						eggTagPos3f(x*scale, y*scale, z*scale);
						eggTagRotate3f(rx*54, ry*54, rz*54);
					}

					for (i = bones.begin(); i < bones.end(); ++i)
					{
						bone = bones[i];

						if (bone && bone->parent == index)
						{ 
							eggTagAddSlave1u(i);
						}
					}

					eggEnd(); // FREYJA_BONE
				}
			}

			eggEnd(); // FREYJA_SKELETON
		}
		else if (!strncmp(symbol, "triangles", 9))
		{
			n = 0;

			// Start a new mesh
			eggBegin(FREYJA_MESH);
			eggBegin(FREYJA_GROUP);

			while ((symbol = r.parseSymbol()) && strncmp(symbol, "end", 3) != 0)
			{
				if (!symbol)
					break;

				//printf("!!!%s %i\n", symbol, n);

				// symbol is material filename eg 'null.bmp'
				++n;

				eggBegin(FREYJA_POLYGON);

				index = r.parseInteger();
				eggTexture1i(index);

				eggVertex1i(i = eggVertexStore3f(r.parseFloat(), r.parseFloat(),
												 r.parseFloat()));
				eggVertexNormalStore3f(i, r.parseFloat(), r.parseFloat(),
									   r.parseFloat());
				eggVertexUVStore2f(i, r.parseFloat(), r.parseFloat());


				index = r.parseInteger();
				eggVertex1i(i = eggVertexStore3f(r.parseFloat(), r.parseFloat(),
												 r.parseFloat()));
				eggVertexNormalStore3f(i, r.parseFloat(), r.parseFloat(),
									   r.parseFloat());
				eggVertexUVStore2f(i, r.parseFloat(), r.parseFloat());


				index = r.parseInteger();
				eggVertex1i(i = eggVertexStore3f(r.parseFloat(), r.parseFloat(),
												 r.parseFloat()));
				eggVertexNormalStore3f(i, r.parseFloat(), r.parseFloat(),
									   r.parseFloat());
				eggVertexUVStore2f(i, r.parseFloat(), r.parseFloat());
							  							  
				eggEnd(); // FREYJA_POLYGON
			}

			eggEnd(); // FREYJA_GROUP
			eggEnd(); // FREYJA_MESH
		}
	}


	return 0;
}


int freyja_model__smd_export(char *filename)
{
	printf("freyja_model__smd_export> Not implemented, %s:%i\n", 
		   __FILE__, __LINE__);

	return -1;
}
