/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : 
 * Comments: This is the NWN ASCII plug-in module
 *
 *           See file COPYING for license details.
 *
 *
 *-- Test Defines -----------------------------------------------------
 *           
 *
 *-- History ---------------------------------------------------------- 
 *
 * 2005.01.06:
 * Mongoose - Created
 ==========================================================================*/

#include <stdio.h>
#include <string.h>
#include <freyja/FreyjaPlugin.h>
#include <freyja/FreyjaFileReader.h>
#include <freyja/FreyjaFileWriter.h>


extern "C" {
	int freyja_model__nwnascii_check(char *filename);
	int freyja_model__nwnascii_import(char *filename);
	int freyja_model__nwnascii_export(char *filename);
	int import_model(char *filename);
	void freyja_init();
}

void freyja_init()
{
	freyjaPluginDescription1s("Neverwinter ASCII Model (*.mdl)");
	freyjaPluginAddExtention1s("*.mdl");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH);
	freyjaPluginExport1i(FREYJA_PLUGIN_NONE);
}

int import_model(char *filename)
{
	if (!freyja_model__nwnascii_check(filename))
		return freyja_model__nwnascii_import(filename);

	return -1;
}


int freyja_model__nwnascii_check(char *filename)
{
	FreyjaFileReader r;
	char *symbol;


	if (FreyjaFileReader::compareFilenameExtention(filename, ".mdl") == 0)
	{
		if (!r.openFile(filename))
		{
			return -1;
		}

		symbol = r.parseSymbol();

		if (!strncmp("#MAXMODEL", symbol, 9))
		{
			freyjaPrintMessage("nwnascii.so: '%s' is an nwnascii model",
							   filename);
			return 0;
		}
	}

	return -1;
}


int freyja_model__nwnascii_import(char *filename)
{
	const vec_t scale = 20.0f;
	FreyjaFileReader r;
	Vector <unsigned int> transV;
	Vector <unsigned int> transT;
	Vector <long> faces;
	char *symbol;
	vec_t x, y, z, u, v;
	vec4_t orientation;
	vec3_t position;
	long i, a, b, c, ta, tb, tc, count, faceCount;
	long meshIndex;


	if (freyja_model__nwnascii_check(filename))
	{
		return -1;
	}

	if (!r.openFile(filename))
	{
		return -1;
	}


	freyjaBegin(FREYJA_MODEL);

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
		else if (!strncmp(symbol, "node", 4))
		{
			symbol = r.parseSymbol();

			if (!strncmp(symbol, "trimesh", 7))
			{

			symbol = r.parseSymbol();

			freyjaPrintMessage("New mesh '%s'\n", symbol);
		
			freyjaBegin(FREYJA_MESH);
			meshIndex = freyjaGetCurrent(FREYJA_MESH);
			freyjaMeshName1s(meshIndex, symbol);

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
				else if (!strncmp(symbol, "orientation", 8))
				{
					// Euler encoding or axis?
					orientation[0] = r.parseFloat(); // x
					orientation[1] = r.parseFloat(); // y
					orientation[2] = r.parseFloat(); // z
					orientation[3] = r.parseFloat(); // angle
				}
				else if (!strncmp(symbol, "position", 8))
				{
					position[0] = r.parseFloat();
					position[1] = r.parseFloat();
					position[2] = r.parseFloat();
				}
				else if (!strncmp(symbol, "endnode", 7))
				{
					break;
				}
				else if (!strncmp(symbol, "tverts", 7))
				{
					count = r.parseInteger();

					//printf("\n# %li texcoords\n", count);

					for (i = 0; i < count && !r.endOfFile(); ++i)
					{
						u = r.parseFloat();
						v = r.parseFloat();
						r.parseFloat(); // w

						//printf("vt %f %f\n", u, v);
						transT.pushBack(freyjaTexCoord2f(u, 1.0 - v));
					}
				}
				else if (!strncmp(symbol, "verts", 6))
				{
					freyjaBegin(FREYJA_VERTEX_GROUP);

					count = r.parseInteger();

					transV.reserve(count+1);

					//printf("\n# %li vertices\n", count);

					for (i = 0; i < count && !r.endOfFile(); ++i)
					{
						x = r.parseFloat() + position[0];
						y = r.parseFloat() + position[1];
						z = r.parseFloat() + position[2];

						//printf("v %f %f %f\n", x, z, y);
						transV.pushBack(freyjaVertex3f(x*scale,
													   z*scale,
													   y*scale));
					}

					freyjaEnd(); // FREYJA_GROUP
				}
				else if (!strncmp(symbol, "faces", 5))
				{
					faceCount = count = r.parseInteger();

					for (i = 0; i < count && !r.endOfFile(); ++i)
					{
						a = r.parseInteger();
						b = r.parseInteger();
						c = r.parseInteger();
						r.parseInteger(); // Smoothing group?
						ta = r.parseInteger();
						tb = r.parseInteger();
						tc = r.parseInteger();
						r.parseInteger(); // Unknown

						//printf("f %li/%li %li/%li %li/%li\n",  a, b, c, ta, tb, tc);

						faces.pushBack(a);
						faces.pushBack(b);
						faces.pushBack(c);
						faces.pushBack(ta);
						faces.pushBack(tb);
						faces.pushBack(tc);
					}
				}
			}

			
			for (i = 0; i < faceCount; ++i)
			{
				a = faces[i*6];
				b = faces[i*6+1];
				c = faces[i*6+2];
				ta = faces[i*6+3];
				tb = faces[i*6+4];
				tc = faces[i*6+5];

				freyjaBegin(FREYJA_POLYGON);
				freyjaPolygonMaterial1i(meshIndex); // good idea?
				freyjaPolygonVertex1i(transV[a]);
				freyjaPolygonVertex1i(transV[b]);
				freyjaPolygonVertex1i(transV[c]);
				freyjaPolygonTexCoord1i(transT[ta]);
				freyjaPolygonTexCoord1i(transT[tb]);
				freyjaPolygonTexCoord1i(transT[tc]);
				freyjaEnd(); // FREYJA_POLYGON
			}

			transV.clear();
			transT.clear();
			faces.clear();

			freyjaEnd(); // FREYJA_MESH
			}
		}
		else
		{
			//printf("# '%s'\n", symbol);
		}

	}

	freyjaEnd(); // FREYJA_MODEL

	r.closeFile();

	return 0;
}


int freyja_model__nwnascii_export(char *filename)
{
	freyjaPrintMessage("freyja_model__nwnascii_export> No Neverwinter Nights ASCII export");
	return -1;
}

