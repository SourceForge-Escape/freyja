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
 * 2004.12.11:
 * Mongoose - Updated specs from
 *            http://netghost.narod.ru/gff/graphics/summary/waveobj.htm
 *
 * 2001.05.09:
 * Mongoose - Created
 ==========================================================================*/

#include <stdio.h>
#include <string.h>
#include <freyja/FreyjaPlugin.h>
#include <freyja/FreyjaFileReader.h>
#include <freyja/FreyjaFileWriter.h>


extern "C" {

	int freyja_model__obj_check(char *filename);
	int freyja_model__obj_import(char *filename);
	int freyja_model__obj_export(char *filename);
	int import_model(char *filename);
	void freyja_init();
}

void freyja_init()
{
	freyjaPluginDescription1s("Object Model (*.obj)");
	freyjaPluginAddExtention1s("*.obj");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH);
	freyjaPluginExport1i(FREYJA_PLUGIN_NONE);
}

int import_model(char *filename)
{
	if (!freyja_model__obj_check(filename))
		return freyja_model__obj_import(filename);

	return -1;
}


int freyja_model__obj_check(char *filename)
{
	if (FreyjaFileReader::compareFilenameExtention(filename, ".obj") == 0)
	{
		freyjaPrintMessage("obj.so: '%s' is an obj model", filename);
		return 0;
	}

	return -1;
}


int freyja_model__obj_import(char *filename)
{
	FreyjaFileReader r;
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

	freyjaBegin(FREYJA_MODEL);
	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

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
			freyjaPrintMessage("FIXME: Material '%s' is not used\n", symbol);
		}
		else if (!strncmp(symbol, "mg", 2))
		{
			freyjaPrintMessage("FIXME: Merging Groups is not used\n");
		}
		else if (!strncmp(symbol, "vp", 2))
		{
			freyjaPrintMessage("FIXME: Parameter space vertices are not used\n");
		}
		else if (!strncmp(symbol, "vt", 2))
		{
			u = r.parseFloat();
			v = r.parseFloat();
			hasUV = true;

			transT.pushBack(freyjaTexCoord2f(u, v));
		}
		else if (!strncmp(symbol, "vn", 2))  // Assumes normals come after v
		{
			x = r.parseFloat();
			y = r.parseFloat();
			z = r.parseFloat();
			hasNormal = true;

			// polymapped normals not in Freyja
			//transN.pushBack(freyjaNormal3f(x, y, z));
		}
		else if (!strncmp(symbol, "v", 1))
		{
			x = r.parseFloat();
			y = r.parseFloat();
			z = r.parseFloat();

			transV.pushBack(freyjaVertex3f(x, y, z));
		}
		else if (!strncmp(symbol, "o", 1))
		{
			// Name of the new group, but no used here
			symbol = r.parseSymbol();
			freyjaPrintMessage("FIXME: Object name '%s' is not used\n", symbol);
		}
		else if (!strncmp(symbol, "g", 1))
		{
			// Name of the new group, but no used here
			symbol = r.parseSymbol();
			freyjaPrintMessage("FIXME: Group name '%s' is not used\n", symbol);
		}
		else if (!strncmp(symbol, "s", 1))
		{
			index = r.parseInteger();
			freyjaPrintMessage("FIXME: Smoothing Group %i is not used\n", i);
		}
		else if (!strncmp(symbol, "f", 1))
		{
			// Start a new polygon
			freyjaBegin(FREYJA_POLYGON);

			for (i = 0; i < 3; ++i)
			{
				index = r.parseInteger();
				freyjaPolygonVertex1i(transV[index-1]); // index starts at 1 not 0

				if (hasUV)
				{
					r.readInt8(); // '/' no whitespace
					index = r.parseInteger();

					index = transT[index-1]; // index starts at 1 not 0
					freyjaPolygonTexCoord1i(index);
				}
				else
				{
					freyjaVertexTexCoord2f(index, 0.25*i, 0.25*(i%2));
				}

				if (hasNormal) // polymapped normals aren't in Freyja atm
				{
					r.readInt8(); // '/' no whitespace
					index = r.parseInteger();

					//index = transN[index-1]; // index starts at 1 not 0
					//freyjaNormal1i(index);
				}
			}

			// Handle quad faces
			i = r.getFileOffset();
			symbol = r.parseSymbol();
			r.setFileOffset(i);

			if (isdigit(symbol[0]))
			{
				index = r.parseInteger();
				freyjaPolygonVertex1i(transV[index-1]); // index starts at 1 not 0

				if (hasUV)
				{
					r.readInt8(); // '/' no whitespace
					index = r.parseInteger();

					index = transT[index-1]; // index starts at 1 not 0
					freyjaPolygonTexCoord1i(index);
				}
				else
				{
					freyjaVertexTexCoord2f(index, 0.25*i, 0.25*(i%2));
				}

				if (hasNormal) // polymapped normals aren't in Freyja atm
				{
					r.readInt8(); // '/' no whitespace
					index = r.parseInteger();

					//index = transN[index-1]; // index starts at 1 not 0
					//freyjaNormal1i(index);
				}
			}

			freyjaPolygonMaterial1i(0);

			freyjaEnd(); // FREYJA_POLYGON
		}
	}

	freyjaEnd(); // FREYJA_GROUP
	freyjaEnd(); // FREYJA_MESH
	freyjaEnd(); // FREYJA_MODEL

	return 0;
}


int freyja_model__obj_export(char *filename)
{
#ifdef OBJ_EXPORT_ENABLED
	FreyjaFileWriter w;
	int index;
	unsigned int j, k, n, group, count;
	vec3_t vert;
	vec2_t uv;
	vec_t scale = 0.5;


	if (!w.openFile(filename))
	{
		return -1;
	}

	/* Comment */
	w.print("# Exported from Freyja, not even tested code\n");

#ifdef BONE_EXT_BREAK_COMPATIBILITY
	char name[64];
	vec_t d2r = 0.017453292519943295;
	vec3_t translation, rotation;


	if (freyjaGetNum(FREYJA_BONE))
	{
		freyjaIterator(FREYJA_SKELETON, FREYJA_LIST_RESET);
		freyjaIterator(FREYJA_BONE, FREYJA_LIST_RESET);
		n = freyjaGetNum(FREYJA_BONE);

		for (i = 0; i < n; ++i)
		{
			index = freyjaIterator(FREYJA_BONE, FREYJA_LIST_CURRENT);
			index = freyjaGetCurrent(FREYJA_BONE);

			freyjaGetBoneName(index, 64, name);
			freyjaGetBoneTranslation3fv(index, translation);
			freyjaGetBoneRotationXYZ3fv(index, rotation);

			translation[0] *= scale; 
			translation[2] *= scale; 
			translation[1] *= scale; 

			if (!i)
			{
				rotation[1] += 90.0;

				w.print("b %3i \"%s\" %i %f %f %f %f %f %f\n", i,
						name, freyjaGetBoneParent(index),
						translation[0], translation[2], translation[1], 
						rotation[0]*d2r, rotation[1]*d2r, rotation[2]*d2r);
			}
			else
			{
				w.print("b %3i \"%s\" %i %f %f %f %f %f %f\n", i,
						name, freyjaGetBoneParent(index),
						translation[0], translation[1], translation[2], 
						rotation[0]*d2r, rotation[1]*d2r, rotation[2]*d2r);
			}

			freyjaIterator(FREYJA_BONE, FREYJA_LIST_NEXT);
		}
	}
#endif


	/* Meshes */
	if (freyjaGetCount(FREYJA_POLYGON))
	{
		freyjaIterator(FREYJA_MESH, FREYJA_LIST_RESET);
		freyjaIterator(FREYJA_POLYGON, FREYJA_LIST_RESET);
		n = freyjaGetCount(FREYJA_POLYGON);

		group = 0;

		//unsigned int meshCount = freyjaGetNum(FREYJA_MESH);
		//for (i = 0; i < meshCount; ++i, group = i) // send all meshes
		//{
			//w.print("g %3i\n", group);

			for (j = 0; j < n; ++j)
			{
				index = freyjaIterator(FREYJA_POLYGON, FREYJA_LIST_CURRENT);
				index = freyjaGetCurrent(FREYJA_POLYGON);

				count = freyjaGetPolygonVertexCount(index);

				for (k = 0; k < count; ++k)
				{
					freyjaGetPolygon3f(FREYJA_VERTEX, j, vert);
					
					w.print("v %f %f %f\n",
							vert[0]*scale, vert[2]*scale, vert[1]*scale);
				}
#ifdef FIXME
				for (k = 0; k < count; ++k)
				{
					freyjaGetPolygon3f(FREYJA_TEXCOORD, j, uv);
					
					w.print("vt %f %f\n",
							uv[0], uv[1]);
				}

				w.print("f %i/%i %i/%i %i/%i", 
						j*3+1, j*3+1, j*3+2, j*3+2, j*3+3, j*3+3);
#else
				w.print("f %i %i %i", 
						j*3+1, j*3+2, j*3+3);				
#endif
				freyjaIterator(FREYJA_POLYGON, FREYJA_LIST_NEXT);
			}
		//}
	}

	w.closeFile();

	return 0;
#else
	return -1;
#endif
}

