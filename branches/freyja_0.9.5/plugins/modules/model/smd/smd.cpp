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
	int import_mesh = 0;
	freyjaGetPluginArg1i(pluginId, "import_mesh", &import_mesh);
	
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
				//loc.Set(loc.mX, loc.mZ, loc.mY);
				//rot.Set(rot.mX, rot.mZ, rot.mY);

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
				if (!symbol || !import_mesh)
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
					// Convert coords
					//u.Set(u.mX, u.mZ, u.mY);
					u *= scale;
					index_t v = freyjaMeshVertexCreate3fv(mesh, u.mVec);
					freyjaMeshPolygonAddVertex1i(mesh, face, v);	

					u.Set(r.ParseFloat(), r.ParseFloat(), r.ParseFloat());
					// Convert coords
					//u.Set(u.mX, u.mZ, u.mY);
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

	if (!w.Open(filename))
	{
		return -1;
	}

	int pluginId = freyjaGetPluginId();

	int skeleton = 0; // FIXME for multiskeleton
	int model = 0; // FIXME for multimodel

	// Model scale
	vec_t scale = 0.15f;
	freyjaGetPluginArg1f(pluginId, "scale", &scale);
	scale = 1.0f / scale;

	// Skeleton only? || Complete model? || Keyframe?
	int export_mesh = 0;
	freyjaGetPluginArg1i(pluginId, "export_mesh", &export_mesh);

	/* version */
	w.Print("version 1\n");

	/* nodes */
	w.Print("nodes\n");

	for (uint32 i = 0; i < freyjaGetSkeletonBoneCount(skeleton); ++i)
	{
		index_t bone = freyjaGetSkeletonBoneIndex(skeleton, i);
		index_t parent = freyjaGetBoneParent(bone);

		w.Print("%3i \"%s\" %i\n", bone, 
			freyjaGetBoneNameString(bone), 
			(parent == INDEX_INVALID) ? -1 : parent);
	}

	w.Print("end\n"); // nodes

	/* skeleton */
	w.Print("skeleton\n");
	w.Print("time 0\n");

	for (uint32 i = 0; i < freyjaGetSkeletonBoneCount(skeleton); ++i)
	{
		index_t bone = freyjaGetSkeletonBoneIndex(skeleton, i);
		hel::Vec3 loc, rot;

		freyjaGetBoneTranslation3fv(bone, loc.mVec);
		freyjaGetBoneRotationEuler3fv(bone, rot.mVec);

		loc *= scale;

		w.Print("%3i %f %f %f %f %f %f\n", i,
				loc[0], loc[1], loc[2], 
				rot[0], rot[1], rot[2]);
	}

	w.Print("end\n");


	/* triangles */
	w.Print("triangles\n");

	if (export_mesh)
	for (uint32 i = 0, in = freyjaGetModelMeshCount(model); i < in; ++i)
	{
		index_t mesh = freyjaGetModelMeshIndex(model, i);

		for (uint32 j = 0, jn = freyjaGetMeshPolygonCount(mesh); j < jn; ++j)
		{
			uint32 kn = freyjaGetMeshPolygonVertexCount(mesh, j);

			w.Print("null.png\n");
			
			for (uint32 k = 0; k < kn; ++k)
			{
				index_t v = freyjaGetMeshPolygonVertexIndex(mesh, j, k);
				index_t t = freyjaGetMeshPolygonTexCoordIndex(mesh, j, k);
				index_t weight = freyjaGetMeshVertexWeightIndex(mesh, v, 0);
				index_t b = freyjaGetMeshWeightBone(mesh, weight);

				hel::Vec3 vert, norm, uv;
				freyjaGetMeshVertexPos3fv(mesh, v, vert.mVec);
				freyjaGetMeshVertexNormal3fv(mesh, v, norm.mVec);
				freyjaGetMeshTexCoord2fv(mesh, t, uv.mVec);
				vert *= scale;

				w.Print("%3i  %f %f %f  %f %f %f  %f %f\n", 
						(b == INDEX_INVALID) ? 0 : b,
						vert[0], vert[1], vert[2], 
						norm[0], norm[1], norm[2],
						uv[0], uv[1]);

				// FIXME support optional SMD multibone here
			}
		}
	}

	w.Print("end\n");

	w.Close();

	return 0;
}
