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
#include <freyja/ModelABI.h>
#include <freyja/PluginABI.h>
#include <freyja/BoneABI.h>
#include <freyja/SkeletonABI.h>
#include <freyja/TextureABI.h>
#include <freyja/LegacyABI.h>
#include <freyja/MeshABI.h>
#include <freyja/freyja.h>
#include <mstl/Vector.h>
#include <mstl/SystemIO.h>

using namespace mstl;

extern "C" {
	int freyja_model__nwnascii_check(char *filename);
	int freyja_model__nwnascii_import(char *filename);
	int freyja_model__nwnascii_export(char *filename);
	int import_model(char *filename);
	void freyja_init();
}

void freyja_init()
{
	freyjaPluginName1s("nwnascii");
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
	SystemIO::TextFileReader r;
	const char *symbol;


	if (SystemIO::File::CompareFilenameExtention(filename, ".mdl") == 0)
	{
		if (!r.Open(filename))
		{
			return -1;
		}

		symbol = r.ParseSymbol();

		if (strncmp("#MAXMODEL", symbol, 9) == 0)
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
	SystemIO::TextFileReader r;
	Vector <unsigned int> transV;
	Vector <unsigned int> transT;
	Vector <long> faces;
	const char *symbol;
	vec_t x, y, z, u, v;
	vec4_t orientation;
	vec3_t position;
	long i, a, b, c, ta, tb, tc, count, faceCount;
	long meshIndex;


	if (freyja_model__nwnascii_check(filename))
	{
		return -1;
	}

	if (!r.Open(filename))
	{
		return -1;
	}


	freyjaBegin(FREYJA_MODEL);

	while ((symbol = r.ParseSymbol()) && !r.IsEndOfFile())
	{
		if (!strncmp(symbol, "#", 1))
		{
			char c;

			while ((c = r.NextChar()) && !r.IsEndOfFile())
			{
				if (c == '\n')
					break;
			}
		}
		else if (!strncmp(symbol, "node", 4))
		{
			symbol = r.ParseSymbol();

			if (!strncmp(symbol, "trimesh", 7))
			{

			symbol = r.ParseSymbol();

			freyjaPrintMessage("New mesh '%s'\n", symbol);
		
			freyjaBegin(FREYJA_MESH);
			meshIndex = freyjaGetCurrent(FREYJA_MESH);
			freyjaMeshName1s(meshIndex, symbol);

			while ((symbol = r.ParseSymbol()) && !r.IsEndOfFile())
			{
				if (!strncmp(symbol, "#", 1))
				{
					char c;

					while ((c = r.NextChar()) && !r.IsEndOfFile())
					{
						if (c == '\n')
							break;
					}
				}
				else if (!strncmp(symbol, "orientation", 8))
				{
					// Euler encoding or axis?
					orientation[0] = r.ParseFloat(); // x
					orientation[1] = r.ParseFloat(); // y
					orientation[2] = r.ParseFloat(); // z
					orientation[3] = r.ParseFloat(); // angle
				}
				else if (!strncmp(symbol, "position", 8))
				{
					position[0] = r.ParseFloat();
					position[1] = r.ParseFloat();
					position[2] = r.ParseFloat();
				}
				else if (!strncmp(symbol, "endnode", 7))
				{
					break;
				}
				else if (!strncmp(symbol, "tverts", 7))
				{
					count = r.ParseInteger();

					//printf("\n# %li texcoords\n", count);

					for (i = 0; i < count && !r.IsEndOfFile(); ++i)
					{
						u = r.ParseFloat();
						v = r.ParseFloat();
						r.ParseFloat(); // w

						//printf("vt %f %f\n", u, v);
						transT.push_back(freyjaTexCoordCreate2f(u, 1.0 - v));
					}
				}
				else if (!strncmp(symbol, "verts", 6))
				{
					freyjaBegin(FREYJA_VERTEX_GROUP);

					count = r.ParseInteger();

					transV.reserve(count+1);

					//printf("\n# %li vertices\n", count);

					for (i = 0; i < count && !r.IsEndOfFile(); ++i)
					{
						x = r.ParseFloat() + position[0];
						y = r.ParseFloat() + position[1];
						z = r.ParseFloat() + position[2];

						//printf("v %f %f %f\n", x, z, y);
						transV.push_back(freyjaVertexCreate3f(x*scale,
													   z*scale,
													   y*scale));
					}

					freyjaEnd(); // FREYJA_GROUP
				}
				else if (!strncmp(symbol, "faces", 5))
				{
					faceCount = count = r.ParseInteger();

					for (i = 0; i < count && !r.IsEndOfFile(); ++i)
					{
						a = r.ParseInteger();
						b = r.ParseInteger();
						c = r.ParseInteger();
						r.ParseInteger(); // Smoothing group bitflag?
						ta = r.ParseInteger();
						tb = r.ParseInteger();
						tc = r.ParseInteger();
						r.ParseInteger(); // Unknown

						//printf("f %li/%li %li/%li %li/%li\n",  a, b, c, ta, tb, tc);

						faces.push_back(a);
						faces.push_back(b);
						faces.push_back(c);
						faces.push_back(ta);
						faces.push_back(tb);
						faces.push_back(tc);
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

				if (!transT.empty())
				{
					freyjaPolygonTexCoord1i(transT[ta]);
					freyjaPolygonTexCoord1i(transT[tb]);
					freyjaPolygonTexCoord1i(transT[tc]);
				}
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

	r.Close();

	return 0;
}


int freyja_model__nwnascii_export(char *filename)
{
	freyjaPrintMessage("freyja_model__nwnascii_export> No Neverwinter Nights ASCII export");
	return -1;
}

