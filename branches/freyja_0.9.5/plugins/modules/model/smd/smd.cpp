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
#include <freyja/MaterialABI.h>
#include <freyja/MeshABI.h>
#include <freyja/freyja.h>
#include <mstl/SystemIO.h>
#include <mstl/String.h>
#include <mstl/Vector.h>
#include <hel/Vec3.h>
#include <hel/Mat44.h>
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
	freyjaPluginArg1f("scale", 1.0f);
	freyjaPluginArg1i("up", 1);
	freyjaPluginArg1i("weighted", 0);
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
	int idx;

} smd_bone_t;


int freyja_model__smd_import(char *filename)
{
	if (freyja_model__smd_check(filename) < 0)
	{
		return -1;
	}

	SystemIO::BufferedTextFileReader r;
	
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

	int up = 1;
	freyjaGetPluginArg1i(pluginId, "up", &up);

	int weighted = 0;
	freyjaGetPluginArg1i(pluginId, "weighted", &weighted);
	
	// If we're in an interactive program this will prompt for arg values.
	//freyjaPluginRequestUserInput();

	// FIXME: This file format prob shouldn't create new model instances,
	// since it's used for keyframes, skeletal data only by many
	index_t model = freyjaModelCreate();
	index_t skeleton = INDEX_INVALID;

	Vector<smd_bone_t *> bones;
	Vector<String> materials;
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
			skeleton = freyjaSkeletonCreate();
			freyjaModelAddSkeleton(model, skeleton);

			int time = 0;

			while ( ( symbol = r.ParseSymbol() )  && !r.IsEndOfFile() )
			{
				if (!symbol || !strncmp(symbol, "end", 3) )
					break;

				if ( !strncmp(symbol, "time", 4) )
				{
					time = r.ParseInteger();
					continue;
				}

				int idx = atoi(symbol);
				hel::Vec3 loc(r.ParseFloat(), r.ParseFloat(), r.ParseFloat());
				loc *= scale;
				hel::Vec3 rot(r.ParseFloat(), r.ParseFloat(), r.ParseFloat());

				// Convert coords
				switch (up)
				{
				case 0:
					{
						matrix_t m = {0,1,0,0, -1,0,0,0, 0,0,1,0, 0,0,0,1 };
						hel::Mat44 mat(m);
						loc = mat * loc;
					}
					break;

				case 2:
					{
						matrix_t m = {-1,0,0,0, 0,-1,0,0, 0,0,1,0, 0,0,0,1 };
						hel::Mat44 mat(m);
						loc = mat * loc;
					}
					break;
				}

				smd_bone_t *bone = bones[idx];

				// Skeleton
				if (time == 0)
				{
					index_t b = freyjaBoneCreate(skeleton);
#if 0
					// Found some kind of odd SMD -- not just coord system?
					hel::Mat44 m, x, y, z;
					x.SetRotationX(rot.mX);
					y.SetRotationY(rot.mY);
					z.SetRotationZ(rot.mZ);

					m = x * y * z;
					hel::Quat q = m.ToQuat();
					freyjaBoneRotateQuat4f(b, -q.mW, q.mX, q.mZ, q.mY);
#else
					freyjaBoneRotateEuler3f(b, rot.mX, rot.mY, rot.mZ);
#endif


					freyjaBoneTranslate3f(b, loc.mX, loc.mY, loc.mZ);
					
					
					bone->idx = b;

					if (bone && idx < (int)bones.size())
					{
						freyjaBoneParent(b, bone->parent);
						freyjaBoneName(b, bone->name.c_str());
						freyjaBoneAddChild(bone->parent, b);
					}
				}
				else if (bone) // Keyframes
				{
					index_t track = freyjaBoneTrackNew(bone->idx);
					index_t rkey = freyjaBoneKeyFrameNew(bone->idx, track, time);
					freyjaBoneRotKeyFrameEuler3f(bone->idx, track, rkey,
							rot.mX, rot.mY, rot.mZ);
					index_t tkey = freyjaBoneKeyFrameNew(bone->idx, track, time);
					freyjaBonePosKeyFrame3f(bone->idx, track, tkey, 
							loc.mX, loc.mY, loc.mZ);
				}
			}

			freyjaSkeletonUpdateBones(skeleton);
			bones.erase(); // calls delete [] 
			// End skeleton
		}
		else if ( !strncmp(symbol, "triangles", 9) )
		{
			// Start a new mesh
			index_t mesh = freyjaMeshCreate();
			freyjaModelAddMesh(model, mesh);
	
			while ( ( symbol = r.ParseSymbol() ) && !r.IsEndOfFile() )
			{
				if ( !strncmp(symbol, "end", 3)  )
					break;

				if (!symbol || !import_mesh)
					break;

				// Only way to be sure about materail requests
				{
					const char *material = symbol;
					unsigned int i;
					bool found  = false;

					foreach(materials, i)
					{
						if (materials[i] == material)
						{
							found = true;
							break;
						}
					}

					if (!found)
					{
						materials.push_back( String(material) );
						index_t mat = freyjaMaterialCreate();
						index_t texture = freyjaTextureCreateFilename(material);
						freyjaMaterialTexture(mat, texture);
						matId = mat;
					}

				}

				index_t face = freyjaMeshPolygonCreate(mesh);
				freyjaMeshPolygonMaterial(mesh, face, matId);
				freyjaMeshPolygonGroup1u(mesh, face, matId);

				hel::Vec3 pos, norm, uv;
				for (unsigned int i = 0; i < 3; ++i)
				{
					int parent = r.ParseInteger();

					pos.Set(r.ParseFloat(), r.ParseFloat(), r.ParseFloat());
					pos *= scale;
					norm.Set(r.ParseFloat(), r.ParseFloat(), r.ParseFloat());

					// Convert coords
					//

					switch (up)
					{
					case 0:
						pos.Set(pos.mY, pos.mX, pos.mZ);
						norm.Set(norm.mY, norm.mX, norm.mZ);

					case 2:
						pos.Set(pos.mX, pos.mZ, pos.mY);
						norm.Set(norm.mX, norm.mZ, norm.mY);
						break;
					}

					if (weighted)
					{
						// FIXME: No support for preweighted vertices!
					}

					index_t v = freyjaMeshVertexCreate3fv(mesh, pos.mVec);
					freyjaMeshPolygonAddVertex1i(mesh, face, v);	

					freyjaMeshVertexNormal3fv(mesh, v, norm.mVec);
				
					uv.mX = r.ParseFloat();
					uv.mY = r.ParseFloat();

					if (uv.mY < 0.0f) uv.mY = -uv.mY;
					if (uv.mX < 0.0f) uv.mX = -uv.mX;

					index_t t = freyjaMeshTexCoordCreate2f(mesh, uv.mX, uv.mY);
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
