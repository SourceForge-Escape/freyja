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

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <freyja/FreyjaPlugin.h>
#include <mstl/SystemIO.h>

#include "ObjModel.h"

using namespace mstl;

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
	if (SystemIO::File::CompareFilenameExtention(filename, ".obj") == 0 ||
		SystemIO::File::CompareFilenameExtention(filename, ".OBJ") == 0)
	{
		freyjaPrintMessage("obj.so: '%s' is an obj model", filename);
		return 0;
	}

	return -1;
}


int freyja_model__obj_import(char *filename)
{
	ObjModel obj;

	if (!obj.Load(filename))
	{
		return -1;
	}

	freyjaBegin(FREYJA_MODEL);

	uint32 m, v, f, i;
	foreach (obj.mMeshes, m)
	{
		freyjaBegin(FREYJA_MESH);
		//freyjaBegin(FREYJA_VERTEX_GROUP);

		foreach (obj.mMeshes[m].mVertices, v)
		{
			i = freyjaVertexCreate3fv(obj.mMeshes[m].mVertices[v].mVec);

			// FIXME Polymapped not supported this way
			if (obj.mMeshes[m].mHasNormals)
				freyjaVertexNormal3fv(i, obj.mMeshes[m].mNormals[v].mVec);
		}

		//freyjaEnd(); //FREYJA_VERTEX_GROUP

		if (obj.mMeshes[m].mHasTexCoords)
		{
			foreach (obj.mMeshes[m].mTexCoords, i)
			{
				freyjaTexCoordCreate2fv(obj.mMeshes[m].mTexCoords[i].mVec);
			}
		}

		foreach (obj.mMeshes[m].mFaces, f)
		{
			freyjaBegin(FREYJA_POLYGON);

			

			foreach (obj.mMeshes[m].mFaces[f].mVertexRefs, v)
			{
				freyjaPolygonVertex1i(obj.mMeshes[m].mFaces[f].mVertexRefs[v]);
			}

			if (obj.mMeshes[m].mHasTexCoords)
			{
				foreach (obj.mMeshes[m].mFaces[f].mTexCoordRefs, v)
				{
					freyjaPolygonTexCoord1i(obj.mMeshes[m].mFaces[f].mTexCoordRefs[v]);
				}
			}

			freyjaEnd(); // FREYJA_POLYGON
		}

		freyjaEnd(); // FREYJA_MESH
	}

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
	w.print("# Exported from %s\n",	FREYJA_API_VERSION);


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
			freyjaGetVertexTexcoord2fv(vertexIndex, uv);

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

