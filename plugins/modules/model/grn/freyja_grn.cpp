/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : stu7440@westga.edu
 * Object  : 
 * Comments: This is the Template plug-in module
 *
 *           See file COPYING for license details.
 *
 *
 *-- Test Defines -----------------------------------------------------
 *           
 *
 *-- History ---------------------------------------------------------- 
 *
 * 2004.03.02:
 * Mongoose - Created, using my grn2ase code as a base which uses
 *            the grn system from a UO clone I patched up.
 *
 *            See GRN files for details.  This is to be replaced with
 *            my own GRN implementation later, so it'll be cleaner for
 *            library use.
 ==========================================================================*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

#include "GRN/grannyfile.h"

#include <hel/math.h>
#include <hel/Quaternion.h>
#include <freyja/PluginABI.h>
#include <freyja/BoneABI.h>
#include <freyja/SkeletonABI.h>
#include <freyja/MeshABI.h>
#include <freyja/TextureABI.h>
#include <freyja/LegacyABI.h>
#include <mstl/Map.h>
#include <mstl/Vector.h>

using namespace mstl;
using namespace std;


#ifdef WIN32
#   define snprintf _snprintf
#endif

/* Export as C functions */
extern "C" {

	int freyja_model__grn_check(char *filename);
	int freyja_model__grn_import(char *filename);
	int freyja_model__grn_export(char *filename);
	int import_model(char *filename);
	void freyja_init();
}

void freyja_init()
{
	freyjaPluginDescription1s("Granny Model (*.grn)");
	freyjaPluginAddExtention1s("*.grn");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH | FREYJA_PLUGIN_SKELETON);
	freyjaPluginExport1i(FREYJA_PLUGIN_NONE);
}

int import_model(char *filename)
{
	if (!freyja_model__grn_check(filename))
		return freyja_model__grn_import(filename);

	return -1;
}


unsigned int freyja_model_grn_info()
{
	return (1 | 2 | 4);
}


int freyja_model__grn_check(char *filename)
{
	cGrannyFile grn;

	grn.load(filename, "");

	if (!grn.initialized())
	{
		//fprintf(stderr, "grn_import> Failed to load file\n");
		return -1;
	}	

	return 0;
}


int freyja_model__grn_export(char *filename)
{
	return -1;
}


int freyja_model__grn_import(char *filename)
{
	const float scale = 15.0f;
	cGrannyFile grn;
	unsigned int i, j;
	string out;
	long index;
	//vec4_t wxyz;



	grn.load(filename, "");

	if (!grn.initialized())
	{
		//fprintf(stderr, "grn_import> Failed to load file\n");
		return -1;
	}

	list<Mesh>::const_iterator iter;
	Meshes meshes = grn.getMeshes();
	Textures textures = grn.getTextures();
    Bones bones = grn.getBones();
	string texture = grn.getTextureName();


	// printf("%i meshes\n", meshes.meshes.size());


	freyjaBegin(FREYJA_MODEL);

#ifdef EGGV9
	freyjaMetaData1String(NAME, filename);
	freyjaMetaData3u(WORLD_TO_FREYJA, X, Z, NY);
	freyjaMetaData1f(SCALE_VERTEX, 15.0f);
	freyjaMetaData1f(SCALE_BONE, 15.0f);
#endif

	for (iter = meshes.meshes.begin(); iter != meshes.meshes.end(); iter++)
	{
		Mesh mesh = *iter;
		Map <unsigned int, unsigned int> transUV, transXYZ;
		unsigned int textureIndex = 0;

		freyjaPrintMessage("Importing mesh\n");

		if (texture.c_str())
		{
			string texture2 = texture + ".png";
			textureIndex = freyjaTextureCreateFilename((char*)texture2.c_str());
			if (textureIndex == INDEX_INVALID)
				textureIndex = 0;
		}

		freyjaBegin(FREYJA_MESH);
		freyjaBegin(FREYJA_VERTEX_GROUP);  

		for (j = 0; j < mesh.points.size(); ++j)
		{
			Point p = mesh.points[j];
			i = freyjaVertexCreate3f(p.points[0]*scale, p.points[2]*scale, -p.points[1]*scale);
			// XZ-Y

			// Generates id translator list
			transXYZ.Add(j, i);
		}
		freyjaEnd(); // GROUP

		for (j = 0; j < mesh.normals.size(); ++j)
		{
			Point p = mesh.normals[j];
			freyjaVertexNormal3f(transXYZ[j], 
								 p.points[0], p.points[2], -p.points[1]);
		}

		for (j = 0; j < mesh.textureMap.size(); ++j)
		{
			Point p = mesh.textureMap[j];
			i = freyjaTexCoordCreate2f(p.points[0], p.points[1]);

			// Generates id translator list
			transUV.Add(j, i);
		}

		for (j = 0; j < mesh.polygons.size(); ++j)
		{
			gPolygon p = mesh.polygons[j];

			freyjaBegin(FREYJA_POLYGON);

			freyjaPolygonVertex1i(transXYZ[p.nodes[0]]);
			freyjaPolygonVertex1i(transXYZ[p.nodes[1]]);
			freyjaPolygonVertex1i(transXYZ[p.nodes[2]]);

			freyjaPolygonTexCoord1i(transUV[textures.polygons[j].nodes[1]]);
			freyjaPolygonTexCoord1i(transUV[textures.polygons[j].nodes[2]]);
			freyjaPolygonTexCoord1i(transUV[textures.polygons[j].nodes[3]]);

			freyjaPolygonMaterial1i(textureIndex);
			freyjaEnd(); // FREYJA_POLYGON
		}

		freyjaEnd(); // MESH

		transXYZ.Clear();
		transUV.Clear();


		/* Skeleton */
		freyjaBegin(FREYJA_SKELETON);
		unsigned int skeletonIndex = freyjaGetCurrent(FREYJA_SKELETON);

		for (i = 0; i < bones.bones.size(); ++i)
		{
			Bone* bone = bones.bones[i];
			char boneName[64];

			if (!bone) // depends on bones[] type if this ever happens
			{
				freyjaPrintMessage("ERROR: NULL GRN bone!\n");
				freyjaBegin(FREYJA_BONE);
				index = freyjaGetCurrent(FREYJA_BONE);
				freyjaSkeletonAddBone(skeletonIndex, index);
				freyjaBoneFlags(index, 0x0);
				freyjaBoneName(index, "ERROR: NULL!");
				freyjaEnd();

				continue;
			}

			BoneTies &boneTies = grn.getTies();
			dword id = boneTies.boneObjects[bone->id];
			dword anmNm = grn.findString("__ObjectName");
			dword textId = grn.getValue(id, anmNm);
			std::string boneStr = grn.findID(textId);
			snprintf(boneName, 64, "%s", boneStr.data());

			freyjaPrintMessage("bone[%i] = '%s', parent = %u\n",
							   i, boneName, bone->parent);

			freyjaBegin(FREYJA_BONE);
			index = freyjaGetCurrent(FREYJA_BONE);
			freyjaSkeletonAddBone(skeletonIndex, index);
			freyjaBoneFlags(index, 0x0);
			freyjaBoneName(index, boneName);
			freyjaBoneParent(index, bone->parent);

			freyjaBoneTranslate3f(index,
								  bone->translate.points[0]*scale, // 0 2 -(1)
								  bone->translate.points[2]*scale, 
								  -bone->translate.points[1]*scale);

#define GRN_QUAT_STACK
#ifdef GRN_QUAT_STACK
			Quaternion r, r2, q;
			long nxt;

			q = Quaternion(bone->quaternion.points[3], // wxyz 
						   bone->quaternion.points[0], 
						   bone->quaternion.points[1],
						   bone->quaternion.points[2]);

			nxt = i;
			r.setIdentity();
			while (nxt > -1)
			{
				Bone *child = bones.bones[nxt];
				nxt = child->parent;

				if (nxt == 0 || nxt == (int)child->parent)
					nxt = -1;

				r2 = Quaternion(child->quaternion.points[3], // wxyz 
								child->quaternion.points[0], 
								child->quaternion.points[1],
								child->quaternion.points[2]);
				//r2.normalize();
				r = r * r2;
			}
			
			q = r * q;
#   if 0

			q.getQuaternion4fv(wxyz);
			freyjaBoneRotateQuat4fv(index, wxyz);
#   else

			{
				vec_t x, y, z;

				q.getEulerAngles(&x, &y, &z);
				x = helRadToDeg(x);
				y = helRadToDeg(y);
				z = helRadToDeg(z);
				freyjaBoneRotateEuler3f(index, x, y, z);
			}
#   endif

			// Experimental
			//q.getEulerAngles(wxyz);
			//freyjaBoneRotateEuler3f(index, wxyz[2], wxyz[0], wxyz[1]);
#else

			freyjaBoneRotateQuat4f(index,
										bone->quaternion.points[3], // wxyz
										bone->quaternion.points[0], 
										bone->quaternion.points[1],
										bone->quaternion.points[2]);


#endif

			if (bone->parent == bone->id)
				freyjaBoneParent(index, INDEX_INVALID);

			for (j = 0; j < bones.bones.size(); ++j)
			{
				Bone *child = bones.bones[j];

				if (child->parent == bone->id && child->id != bone->id)
				{
					printf("+child %li\n", child->id);
					freyjaBoneAddChild(index, child->id);
				}
			}

			freyjaEnd(); // FREYJA_BONE
		}

		freyjaEnd(); // FREYJA_SKELETON

		/* Weights */
		for (i = 0; i < mesh.weights.size(); ++i)
		{
			BoneWeight w = mesh.weights[i];

			for (j = 0; j < w.numWeights; ++j)
			{
				freyjaVertexWeight(i, w.weights[j], w.bones[j]);
			}
		}
	}

	freyjaEnd(); // FREYJA_MODEL

	return 0;
}

