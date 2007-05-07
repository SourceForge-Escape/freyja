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
#include <hel/Vec3.h>
#include <hel/Quat.h>

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

	int id;
	int parent;
	String name;

} smd_bone_t;


int freyja_model__smd_import(char *filename)
{
	if (freyja_model__smd_check(filename) < 0)
	{
		return -1;
	}

	SystemIO::TextFileReader r;
	
	if ( !r.Open(filename) )
	{
		perror(filename);
		return -2;
	}

	int pluginId = freyjaGetPluginId();

	// Model scale
	vec_t scale = 0.15f;
	freyjaGetPluginArg1f(pluginId, "scale", &scale);

	// Skeleton only? || Complete model? || Keyframe?
	int type = 0;
	freyjaGetPluginArg1i(pluginId, "type", &type);
	
	// If we're in an interactive program this will prompt for arg values.
	//freyjaPluginRequestUserInput();

	// FIXME: This file format prob shouldn't create new model instances,
	// since it's used for keyframes, skeletal data only by many
	index_t model = freyjaModelCreate();
	index_t skeleton = INDEX_INVALID;

	Vector <smd_bone_t *> bones;
	const char *symbol;
	mstl::String mat = "none.mat";
	index_t matId = 0;

	while ((symbol = r.ParseSymbol()) && !r.IsEndOfFile())
	{
		if (!strncmp(symbol, "version", 7))
		{
			/*int idx =*/ r.ParseInteger();
		}
		else if (!strncmp(symbol, "nodes", 5))
		{
			while ((symbol = r.ParseSymbol()) && strncmp(symbol, "end", 3) != 0)
			{
				smd_bone_t *bone = new smd_bone_t;

				if (!symbol)
					break;

				bone->id = atoi(symbol);
				bone->name = r.ParseStringLiteral();
				bone->parent = r.ParseInteger();
				bones.push_back(bone);
			}
		}
		else if (!strncmp(symbol, "skeleton", 8))
		{
			symbol = r.ParseSymbol();
			int time = r.ParseInteger();
			//printf("%s = %i\n", symbol, time);

			// FIXME: if time != 0 we might just want to force keyframes here

			skeleton = freyjaSkeletonCreate();
			freyjaModelAddSkeleton(model, skeleton);

			while ((symbol = r.ParseSymbol()) && 
					strncmp(symbol, "end", 3) != 0 && !r.IsEndOfFile())
			{
				if (!symbol)
					break;

				int idx = atoi(symbol);
				hel::Vec3 loc(r.ParseFloat(), r.ParseFloat(), r.ParseFloat());
				loc *= scale;
				hel::Vec3 rot(r.ParseFloat(), r.ParseFloat(), r.ParseFloat());

				// Convert coords
				loc.Set(loc.mY, loc.mZ, loc.mX);
				rot.Set(rot.mY, rot.mZ, rot.mX);

				smd_bone_t *bone = bones[idx];

				index_t b = freyjaBoneCreate(skeleton);
				freyjaBoneTranslate3f(b, loc.mX, loc.mY, loc.mZ);
				freyjaBoneRotateEuler3f(b, rot.mX, rot.mY, rot.mZ);

				if (bone && idx < (int)bones.size())
				{
					freyjaBoneParent(b, bone->parent);
					freyjaBoneName(b, bone->name.c_str());
					freyjaBoneAddChild(bone->parent, b);
				}
			}

			freyjaSkeletonUpdateBones(skeleton);

			bones.erase(); // calls delete [] 

			// End skeleton
		}
		else if (!strncmp(symbol, "triangles", 9))
		{
			// Start a new mesh
			index_t mesh = freyjaMeshCreate();
			freyjaModelAddMesh(model, mesh);
	
			while ((symbol = r.ParseSymbol()) && strncmp(symbol, "end", 3) != 0 && !r.IsEndOfFile())
			{
				if (!symbol)
					break;

#if 0
				if (mat != symbol)
				{
					matId = freyjaMaterialCreate();
					freyjaMaterialTexture( idx, freyjaTextureCreateFilename(symbol) );
					mat = symbol;
				}
#endif
				index_t face = freyjaMeshPolygonCreate(mesh);
				freyjaMeshPolygonMaterial(mesh, face, matId);
				freyjaMeshPolygonGroup1u(mesh, face, matId);

				hel::Vec3 u;
				for (unsigned int i = 0; i < 3; ++i)
				{
					int parent = r.ParseInteger();

					u.Set(r.ParseFloat(), r.ParseFloat(), r.ParseFloat());
					//u.Set(u.mX, u.mZ, u.mY);
					u *= scale;
					index_t v = freyjaMeshVertexCreate3fv(mesh, u.mVec);
					freyjaMeshPolygonAddVertex1i(mesh, face, v);	

					u.Set(r.ParseFloat(), r.ParseFloat(), r.ParseFloat());
					freyjaMeshVertexNormal3fv(mesh, v, u.mVec);
				
					index_t t = freyjaMeshTexCoordCreate2f(mesh, r.ParseFloat(), r.ParseFloat());
					freyjaMeshPolygonAddTexCoord1i(mesh, face, t);


					// FIXME: Handle optional weights here!
					// int num_weights;
					// for num_weights : { int bone_id, vec weight }
					//
					// Remaining of 1.0 weight assigned to parent, but if
					// there is no num_weights set parent to 1.0 automatically.

					freyjaMeshVertexWeight(mesh, v, parent, 1.0f);
				}

						  					
			}
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
				rotation[1] += helDegToRad(90.0f);

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
