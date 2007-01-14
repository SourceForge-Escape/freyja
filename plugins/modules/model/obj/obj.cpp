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
#include <freyja/ModelABI.h>
#include <freyja/PluginABI.h>
#include <freyja/BoneABI.h>
#include <freyja/SkeletonABI.h>
#include <freyja/TextureABI.h>
#include <freyja/MeshABI.h>
#include <freyja/freyja.h>
#include <mstl/Vector.h>
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
	mstl::Vector<int> uvmap;

	if (!obj.Load(filename))
	{
		return -1;
	}

	index_t model = freyjaModelCreate();

	uint32 m, v, f, i;
	foreach (obj.mMeshes, m)
	{
		index_t mesh = freyjaMeshCreate();
		freyjaModelAddMesh(model, mesh);

		foreach (obj.mMeshes[m].mVertices, v)
		{
			i = freyjaMeshVertexCreate3fv(mesh, obj.mMeshes[m].mVertices[v].mVec);
			// FIXME Polymapped not supported this way
			if (obj.mMeshes[m].mHasNormals)
				freyjaMeshVertexNormal3fv(mesh, i, obj.mMeshes[m].mNormals[v].mVec);
		}

		if (obj.mMeshes[m].mHasTexCoords)
		{
			foreach (obj.mMeshes[m].mTexCoords, i)
			{
				uvmap.push_back(freyjaMeshTexCoordCreate2fv(mesh, obj.mMeshes[m].mTexCoords[i].mVec));
			}
		}

		foreach (obj.mMeshes[m].mFaces, f)
		{
			index_t face = freyjaMeshPolygonCreate(mesh);
			freyjaMeshPolygonMaterial(mesh, face, 0);
			freyjaMeshPolygonGroup1u(mesh, face, obj.mMeshes[m].mFaces[f].mSmoothingGroup);

			foreach (obj.mMeshes[m].mFaces[f].mVertexRefs, v)
			{
				freyjaMeshPolygonAddVertex1i(mesh, face, obj.mMeshes[m].mFaces[f].mVertexRefs[v]);
			}

			if (obj.mMeshes[m].mHasTexCoords)
			{
				foreach (obj.mMeshes[m].mFaces[f].mTexCoordRefs, v)
				{
					freyjaMeshPolygonAddTexCoord1i(mesh, face,  uvmap[obj.mMeshes[m].mFaces[f].mTexCoordRefs[v]]);
					//SystemIO::Print("uv '%i' %i %i\n", v, obj.mMeshes[m].mFaces[f].mTexCoordRefs[v], uvmap[obj.mMeshes[m].mFaces[f].mTexCoordRefs[v]]);
				}
			}

			
		}

		
	}

	

	return 0;
}


int freyja_model__obj_export(char *filename)
{
#ifdef OBJ_EXPORT_ENABLED
	const vec_t scale = 1.0;
	SystemIO::FileWriter w;
	long modelIndex = 0; // make plugin option
	long i, j, k, v;
	long meshCount, meshIndex;
	long vertexCount, vertexIndex;
	long polygonCount, polygonIndex, faceVertexCount, faceVertex;
	vec3_t xyz;
	vec2_t uv;


	if (!w.Open(filename))
	{
		return -1;
	}

	/* Comment */
	w.Print("# Exported from %s\n",	FREYJA_API_VERSION);


	/* Meshes */
	meshCount = freyjaGetModelMeshCount(modelIndex);
	v = 0;

	for (i = 0; i < meshCount; ++i)
	{
		meshIndex = freyjaGetModelMeshIndex(modelIndex, i);

		vertexCount = freyjaGetMeshVertexCount(meshIndex);
		polygonCount = freyjaGetMeshPolygonCount(meshIndex);

		w.Print("\ng mesh%03li\n", i);

		/* Vertices */
		w.Print("\n# vertexCount %li\n", vertexCount);
		for (j = 0; j < vertexCount; ++j)
		{
			vertexIndex = freyjaGetMeshVertexIndex(meshIndex, j);
			freyjaGetVertexXYZ3fv(vertexIndex, xyz);

			//w.Print("# vertex %li -> %li\n", j, vertexIndex);
			w.Print("v %f %f %f\n",
					xyz[0]*scale, xyz[1]*scale, xyz[2]*scale);
		}

		/* Normals */
		w.Print("\n# normalCount %li\n", vertexCount);
		for (j = 0; j < vertexCount; ++j)
		{
			vertexIndex = freyjaGetMeshVertexIndex(meshIndex, j);
			freyjaGetVertexNormalXYZ3fv(vertexIndex, xyz);

			w.Print("vn %f %f %f\n",
					xyz[0], xyz[1], xyz[2]);
		}

		/* Texcoords */
		w.Print("\n# texcoordCount %li\n", vertexCount);
		for (j = 0; j < vertexCount; ++j)
		{
			vertexIndex = freyjaGetMeshVertexIndex(meshIndex, j);
			freyjaGetVertexTexcoord2fv(vertexIndex, uv);

			w.Print("vt %f %f\n", uv[0], uv[1]);
		}

		/* Faces */
		w.Print("\n# faceCount %li\n", polygonCount);
		for (j = 0; j < polygonCount; ++j)
		{
			polygonIndex = freyjaGetMeshPolygonIndex(meshIndex, j);
			faceVertexCount = freyjaGetPolygonVertexCount(polygonIndex);

			w.Print("f ");

			for (k = 0; k < faceVertexCount; ++k)
			{
				faceVertex = freyjaGetPolygonVertexIndex(polygonIndex, k);
				vertexIndex = freyjaGetMeshPolygonVertexIndex(meshIndex, faceVertex);

				//w.Print("\n# %li. %li -> %li\n", j, faceVertex, vertexIndex);

				// Assumes UV mapped vertices exported
				// Remember obj indices start at 1
				w.Print("%li/%li/%li ", 
						v+vertexIndex+1, v+vertexIndex+1, v+vertexIndex+1);
			}

			w.Print("\n");			
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

				w.Print("#bone %3i \"%s\" %i %f %f %f %f %f %f\n", i,
						name, freyjaGetBoneParent(index),
						translation[0], translation[2], translation[1], 
						rotation[0]*d2r, rotation[1]*d2r, rotation[2]*d2r);
			}
			else
			{
				w.Print("#bone %3i \"%s\" %i %f %f %f %f %f %f\n", i,
						name, freyjaGetBoneParent(index),
						translation[0], translation[1], translation[2], 
						rotation[0]*d2r, rotation[1]*d2r, rotation[2]*d2r);
			}

			freyjaIterator(FREYJA_BONE, FREYJA_LIST_NEXT);
		}
	}
#   endif

	w.Close();

	return 0;
#else
	return -1;
#endif
}

