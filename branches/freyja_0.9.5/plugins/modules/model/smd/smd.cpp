/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org
 * Email   : mongoose@icculus.org
 * Object  : 
 * Comments: This is the SMD plug-in module
 *
 *           See file COPYING for license details.
 *
 *
 *-- Test Defines -----------------------------------------------------
 *           
 *
 *-- History ---------------------------------------------------------- 
 *
 * 200?.??.??:
 * Mongoose - Created
 ==========================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <freyja/ModelABI.h>
#include <freyja/PluginABI.h>
#include <freyja/BoneABI.h>
#include <freyja/SkeletonABI.h>
#include <freyja/TextureABI.h>
#include <freyja/LegacyABI.h>
#include <freyja/MeshABI.h>
#include <freyja/freyja.h>
#include <mstl/SystemIO.h>
#include <mstl/String.h>
#include <mstl/Vector.h>

using namespace mstl;

extern "C" {

	int freyja_model__smd_check(char *filename);
	int freyja_model__smd_import(char *filename);
	int freyja_model__smd_export(char *filename);
	int import_model(char *filename);
	void freyja_init();
}

void freyja_init()
{
	freyjaPluginName1s("smd");
	freyjaPluginDescription1s("SMD model (*.smd)");
	freyjaPluginAddExtention1s("*.smd");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH | FREYJA_PLUGIN_SKELETON);
	freyjaPluginExport1i(FREYJA_PLUGIN_MESH | FREYJA_PLUGIN_SKELETON);
	freyjaPluginArg1f("scale", 0.15f);
	freyjaPluginArg1i("import_mesh", 1);
	freyjaPluginArg1i("export_mesh", 0);
}


int import_model(char *filename)
{
	if (!freyja_model__smd_check(filename))
		return freyja_model__smd_import(filename);

	return -1;
}


int freyja_model__smd_check(char *filename)
{
	if (SystemIO::File::CompareFilenameExtention(filename, ".smd") == 0)
		return 0;

	return -1;
}


typedef struct {

	int parent;
	String name;

} smd_bone_t;


int freyja_model__smd_import(char *filename)
{
	SystemIO::TextFileReader r;
	Vector <unsigned int> transV;
	Vector <smd_bone_t *> bones;
	const char *symbol;
	vec_t x, y, z, rx, ry, rz;
	int index;
	unsigned int i, n;
	vec_t scale;

	int pluginId = freyjaGetPluginId();
	freyjaGetPluginArg1f(pluginId, "scale", &scale);

	if (freyja_model__smd_check(filename) < 0 || !r.Open(filename))
	{
		return -1;
	}

	// FIXME: This file format prob shouldn't create new model instances,
	// since it's used for keyframes, skeletal data only by many
	index_t model = freyjaModelCreate();
	index_t skeleton = INDEX_INVALID;

	while ((symbol = r.ParseSymbol()) && !r.IsEndOfFile())
	{
		if (!strncmp(symbol, "version", 7))
		{
			index = r.ParseInteger();
		}
		else if (!strncmp(symbol, "nodes", 5))
		{
			while ((symbol = r.ParseSymbol()) && strncmp(symbol, "end", 3) != 0)
			{
				smd_bone_t *bone = new smd_bone_t;

				if (!symbol)
					break;

				index = atoi(symbol);
				bone->name = r.ParseStringLiteral();
				bone->parent = r.ParseInteger();
				bones.pushBack(bone);
			}
		}
		else if (!strncmp(symbol, "skeleton", 8))
		{
			symbol = r.ParseSymbol();
			int time = r.ParseInteger();
			printf("%s = %i\n", symbol, time);

			// FIXME: if time != 0 we might just want to force keyframes here
			skeleton = freyjaSkeletonCreate();
			freyjaModelAddSkeleton(model, skeleton);


			while ((symbol = r.ParseSymbol()) && strncmp(symbol, "end", 3) != 0 && !r.IsEndOfFile())
			{
				if (!symbol)
					break;

				index = atoi(symbol);
				smd_bone_t *bone = bones[index];

				if (bone && index < (int)bones.end())
				{
					x = r.ParseFloat();
					y = r.ParseFloat();
					z = r.ParseFloat();

					rx = r.ParseFloat();
					ry = r.ParseFloat();
					rz = r.ParseFloat();

					index_t b = freyjaBoneCreate(skeleton);
					freyjaBoneParent(b, bone->parent);
					freyjaBoneAddChild(bone->parent, b);
					freyjaSkeletonAddBone(skeleton, b); 
					freyjaBoneFlags(b, 0x0);
					freyjaBoneName(b, bone->name.GetCString());
					freyjaBoneTranslate3f(b, x*scale, y*scale, z*scale);
					freyjaBoneRotateEuler3f(b, rx, ry, rz);
				}
			}

			// End skeleton
		}
		else if (!strncmp(symbol, "triangles", 9))
		{
			n = 0;

			// Start a new mesh
			freyjaBegin(FREYJA_MESH);
			freyjaBegin(FREYJA_VERTEX_GROUP);

			while ((symbol = r.ParseSymbol()) && strncmp(symbol, "end", 3) != 0 && !r.IsEndOfFile())
			{
				if (!symbol)
					break;

				//printf("!!!%s %i\n", symbol, n);

				// symbol is material filename eg 'null.bmp'
				++n;

				freyjaBegin(FREYJA_POLYGON);

				index = r.ParseInteger();
				freyjaPolygonMaterial1i(index);

				freyjaPolygonVertex1i(i = freyjaVertexCreate3f(r.ParseFloat()*scale,
														 r.ParseFloat()*scale,
														 r.ParseFloat()*scale));
				freyjaVertexNormal3f(i, r.ParseFloat(),
									 r.ParseFloat(),
									 r.ParseFloat());
				freyjaVertexTexcoord2f(i, r.ParseFloat(), r.ParseFloat());


				index = r.ParseInteger();
				freyjaPolygonVertex1i(i = freyjaVertexCreate3f(r.ParseFloat()*scale,
														 r.ParseFloat()*scale,
														 r.ParseFloat()*scale));
				freyjaVertexNormal3f(i, r.ParseFloat(),
									 r.ParseFloat(),
									 r.ParseFloat());
				freyjaVertexTexcoord2f(i, r.ParseFloat(), r.ParseFloat());


				index = r.ParseInteger();
				freyjaPolygonVertex1i(i = freyjaVertexCreate3f(r.ParseFloat()*scale, 
														 r.ParseFloat()*scale,
														 r.ParseFloat()*scale));
				freyjaVertexNormal3f(i, r.ParseFloat(), 
									 r.ParseFloat(),
									 r.ParseFloat());
				freyjaVertexTexcoord2f(i, r.ParseFloat(), r.ParseFloat());
							  							  
				freyjaEnd(); // FREYJA_POLYGON
			}

			freyjaEnd(); // FREYJA_GROUP
			freyjaEnd(); // FREYJA_MESH
		}
	}

	r.Close();

	return 0;
}


int freyja_model__smd_export(char *filename)
{
	SystemIO::TextFileWriter w;
	char name[64];
	int index;
	unsigned int i, n;
	vec_t scale = 1.0 / 0.15;
	vec_t d2r = 1.0f;//0.017453292519943295;
	vec3_t translation, rotation;


	if (!w.Open(filename))
	{
		return -1;
	}

	/* version */
	w.Print("version 1\n");

	/* nodes */
	w.Print("nodes\n");

	if (freyjaGetCount(FREYJA_BONE))
	{
		freyjaIterator(FREYJA_SKELETON, FREYJA_RESET);
		freyjaIterator(FREYJA_BONE, FREYJA_RESET);
		n = freyjaGetCount(FREYJA_BONE);

		for (i = 0; i < n; ++i)
		{
			index = freyjaIterator(FREYJA_BONE, FREYJA_CURRENT);
			index = freyjaGetCurrent(FREYJA_BONE);

			freyjaGetBoneName(index, 64, name);
			w.Print("%3i \"%s\" %i\n", i, name, freyjaGetBoneParent(index));

			freyjaIterator(FREYJA_BONE, FREYJA_NEXT);
		}
	}

	w.Print("end\n"); // nodes

	/* skeleton */
	w.Print("skeleton\n");
	w.Print("time 0\n");

	if (freyjaGetCount(FREYJA_BONE))
	{
		freyjaIterator(FREYJA_SKELETON, FREYJA_RESET);
		freyjaIterator(FREYJA_BONE, FREYJA_RESET);
		n = freyjaGetCount(FREYJA_BONE);

		for (i = 0; i < n; ++i)
		{
			index = freyjaIterator(FREYJA_BONE, FREYJA_CURRENT);
			index = freyjaGetCurrent(FREYJA_BONE);

			freyjaGetBoneTranslation3fv(index, translation);
			freyjaGetBoneRotationEuler3fv(index, rotation);

			translation[0] *= scale; 
			translation[2] *= scale; 
			translation[1] *= scale; 

			if (!i)
			{
				rotation[1] += HEL_DEG_TO_RAD(90.0f);

				w.Print("%3i %f %f %f %f %f %f\n", i,
						translation[0], translation[2], translation[1], 
						rotation[0]*d2r, rotation[1]*d2r, rotation[2]*d2r);
			}
			else
			{
				w.Print("%3i %f %f %f %f %f %f\n", i,
						translation[0], translation[1], translation[2], 
						rotation[0]*d2r, rotation[1]*d2r, rotation[2]*d2r);
			}

			freyjaIterator(FREYJA_BONE, FREYJA_NEXT);
		}
	}
	w.Print("end\n");

	/* triangles */
	w.Print("triangles\n");
#ifdef SMD_MESH_EXPORT
	unsigned int j, k, group;
	vec3_t vert;
	vec2_t uv;


	if (freyjaGetNum(FREYJA_POLYGON))
	{
		freyjaIterator(FREYJA_MESH, FREYJA_RESET);
		freyjaIterator(FREYJA_POLYGON, FREYJA_RESET);
		n = freyjaGetNum(FREYJA_POLYGON);

		group = 0;

#ifdef SMD_MESH_EXPORT_ALL
		unsigned int meshCount = freyjaGetNum(FREYJA_MESH);
		for (i = 0; i < meshCount; ++i, group = i) // send all meshes
#endif
		for (j = 0; j < n; ++j)
		{
			index = freyjaIterator(FREYJA_POLYGON, FREYJA_CURRENT);
			index = freyjaGetCurrent(FREYJA_POLYGON);

			for (j = 0; j < n; ++j)
			{
				freyjaGetPolygon3f(FREYJA_VERTEX, j, &vert);
				freyjaGetPolygon3f(FREYJA_TEXCOORD, j, &uv);

				w.Print("null.png");
				w.Print("%3i  %f %f %f  %f %f %f  %f %f\n", group,
						vert[0]*scale, vert[2]*scale, vert[1]*scale, 
						norm[0], norm[2], norm[1],
						uv[0], uv[1]);
			}

			freyjaIterator(FREYJA_POLYGON, FREYJA_NEXT);
		}
	}
#endif
	w.Print("end\n");

	w.Close();

	return 0;
}
