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
	freyjaPluginExport1i(FREYJA_PLUGIN_MESH);
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
	Vector <vec_t> normals;
	//Vector <unsigned int> transN;
	char *symbol;
	vec_t x, y, z, u, v;
	int i, vertexIndex, index;
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
		else if (!strncmp(symbol, "mtllib", 6))
		{
			// Filename of the material
			symbol = r.parseSymbol();
			freyjaPrintMessage("FIXME: Material '%s' is not used\n", symbol);
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
			normals.pushBack(x);
			normals.pushBack(y);
			normals.pushBack(z);
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
				vertexIndex = transV[index-1];

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

					if (!normals.empty())
					{
						freyjaVertexNormal3f(vertexIndex,
											 normals[(index-1)*3],
											 normals[(index-1)*3+1],
											 normals[(index-1)*3+2]);
					}
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
	const vec_t scale = 1.0;
	FreyjaFileWriter w;
	long modelIndex = 0; // make plugin option
	long i, j, k, v;
	long meshCount, meshIndex;
	long vertexCount, vertexIndex;
	long polygonCount, polygonIndex, faceVertexCount, faceVertex;
	vec3_t xyz;
	vec2_t uv;


	if (!w.openFile(filename))
	{
		return -1;
	}

	/* Comment */
	w.print("# Exported from %s\n",	FREYJA_PLUGIN_VERSION);


	/* Meshes */
	meshCount = freyjaGetModelMeshCount(modelIndex);
	v = 0;

	for (i = 0; i < meshCount; ++i)
	{
		meshIndex = freyjaGetModelMeshIndex(modelIndex, i);

		vertexCount = freyjaGetMeshVertexCount(meshIndex);
		polygonCount = freyjaGetMeshPolygonCount(meshIndex);

		w.print("\ng mesh%03li\n", i);

		/* Vertices */
		w.print("\n# vertexCount %li\n", vertexCount);
		for (j = 0; j < vertexCount; ++j)
		{
			vertexIndex = freyjaGetMeshVertexIndex(meshIndex, j);
			freyjaGetVertexXYZ3fv(vertexIndex, xyz);

			//w.print("# vertex %li -> %li\n", j, vertexIndex);
			w.print("v %f %f %f\n",
					xyz[0]*scale, xyz[1]*scale, xyz[2]*scale);
		}

		/* Normals */
		w.print("\n# normalCount %li\n", vertexCount);
		for (j = 0; j < vertexCount; ++j)
		{
			vertexIndex = freyjaGetMeshVertexIndex(meshIndex, j);
			freyjaGetVertexNormalXYZ3fv(vertexIndex, xyz);

			w.print("vn %f %f %f\n",
					xyz[0], xyz[1], xyz[2]);
		}

		/* Texcoords */
		w.print("\n# texcoordCount %li\n", vertexCount);
		for (j = 0; j < vertexCount; ++j)
		{
			vertexIndex = freyjaGetMeshVertexIndex(meshIndex, j);
			freyjaGetVertexTexCoordUV2fv(vertexIndex, uv);

			w.print("vt %f %f\n", uv[0], uv[1]);
		}

		/* Faces */
		w.print("\n# faceCount %li\n", polygonCount);
		for (j = 0; j < polygonCount; ++j)
		{
			polygonIndex = freyjaGetMeshPolygonIndex(meshIndex, j);
			faceVertexCount = freyjaGetPolygonVertexCount(polygonIndex);

			w.print("f ");

			for (k = 0; k < faceVertexCount; ++k)
			{
				faceVertex = freyjaGetPolygonVertexIndex(polygonIndex, k);
				vertexIndex = freyjaGetMeshPolygonVertexIndex(meshIndex, faceVertex);

				//w.print("\n# %li. %li -> %li\n", j, faceVertex, vertexIndex);

				// Assumes UV mapped vertices exported
				// Remember obj indices start at 1
				w.print("%li/%li/%li ", 
						v+vertexIndex+1, v+vertexIndex+1, v+vertexIndex+1);
			}

			w.print("\n");			
		}

		v += vertexCount;
	}


#   ifdef OBJ_EXPORT_SKELETON_AS_COMMENT
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

				w.print("#bone %3i \"%s\" %i %f %f %f %f %f %f\n", i,
						name, freyjaGetBoneParent(index),
						translation[0], translation[2], translation[1], 
						rotation[0]*d2r, rotation[1]*d2r, rotation[2]*d2r);
			}
			else
			{
				w.print("#bone %3i \"%s\" %i %f %f %f %f %f %f\n", i,
						name, freyjaGetBoneParent(index),
						translation[0], translation[1], translation[2], 
						rotation[0]*d2r, rotation[1]*d2r, rotation[2]*d2r);
			}

			freyjaIterator(FREYJA_BONE, FREYJA_LIST_NEXT);
		}
	}
#   endif

	w.closeFile();

	return 0;
#else
	return -1;
#endif
}

