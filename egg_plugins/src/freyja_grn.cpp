/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

#include <freyja_model/EggPlugin.h>

/* Export as C functions */
extern "C" {

	int freyja_model__grn_check(char *filename);
	int freyja_model__grn_import(char *filename);
	int freyja_model__grn_export(char *filename);
}

#include "GRN/grannyfile.h"

using namespace std;

#ifdef WIN32
#   define snprintf _snprintf
#endif

int freyja_model__grn_check(char *filename)
{
	cGrannyFile grn;

	grn.load(filename, "");

	if (!grn.initialized())
	{
		fprintf(stderr, "grn_import> Failed to load file\n");
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
	cGrannyFile grn;
	unsigned int i, j;
	string out;


	grn.load(filename, "");

	if (!grn.initialized())
	{
		fprintf(stderr, "grn_import> Failed to load file\n");
		return -1;
	}

	list<Mesh>::const_iterator iter;
	Meshes meshes = grn.getMeshes();
	Textures textures = grn.getTextures();
    Bones bones = grn.getBones();
	string texture = grn.getTextureName();

	// printf("%i meshes\n", meshes.meshes.size());

	for (iter = meshes.meshes.begin(); iter != meshes.meshes.end(); iter++)
	{
		Mesh mesh = *iter;
		Map <unsigned int, unsigned int> transUV, transXYZ;
		unsigned int textureIndex = 0;


		if (texture.c_str())
		{
			string texture2 = texture + ".png";
			textureIndex = eggTextureStoreFilename((char*)texture2.c_str());
		}

		eggBegin(FREYJA_MESH);
		eggBegin(FREYJA_GROUP);  

		for (j = 0; j < mesh.points.size(); ++j)
		{
			Point p = mesh.points[j];
			i = eggVertexStore3f(p.points[0], p.points[2], -p.points[1]);
			// XZ-Y

			// Generates id translator list
			transXYZ.Add(j, i);
		}
		eggEnd(); // GROUP

		for (j = 0; j < mesh.normals.size(); ++j)
		{
			Point p = mesh.normals[j];
			eggVertexNormalStore3f(j, p.points[0], p.points[1], p.points[2]);
		}

		for (j = 0; j < mesh.textureMap.size(); ++j)
		{
			Point p = mesh.textureMap[j];
			i = eggTexelStore2f(p.points[0], p.points[1]);

			// Generates id translator list
			transUV.Add(j, i);
		}

		for (j = 0; j < mesh.polygons.size(); ++j)
		{
			gPolygon p = mesh.polygons[j];

			eggBegin(FREYJA_POLYGON);

			eggVertex1i(transXYZ[p.nodes[0]]);
			eggVertex1i(transXYZ[p.nodes[1]]);
			eggVertex1i(transXYZ[p.nodes[2]]);

			eggTexel1i(transUV[textures.polygons[j].nodes[1]]);
			eggTexel1i(transUV[textures.polygons[j].nodes[2]]);
			eggTexel1i(transUV[textures.polygons[j].nodes[3]]);

			eggTexture1i(textureIndex);
			eggEnd(); // FREYJA_POLYGON
		}

		eggEnd(); // MESH


#ifdef OBSOLETE
		/* FIXME: Can't export weights and bones to ASE */
		filename[len-1] = 'l';
		filename[len-2] = 'e';
		filename[len-3] = 'k';
		filename[len-4] = 's';
		f = fopen(filename, "w");

		if (!f)
		{
			printf("Unable to save skeleton as %s\n", filename);
			perror("Couldn't open file for writing:");
			continue;
		}

		printf("Saving skeleton as: %s\n\n", filename);

		printf("\t%i weights\n", mesh.weights.size());
		for (i = 0, j = 0; i < mesh.weights.size(); ++i)
		{
			BoneWeight w = mesh.weights[i];
			j += w.numWeights;
		}

		printf("\t\t%i total weight connections\n", j);

		fprintf(f, "*SKELETON {\n");
		fprintf(f, "\t*NAME \"RestSkeleton\"\n");
		fprintf(f, "\t*BONE_COUNT %d\n", bones.bones.size());
		fprintf(f, "\t*WEIGHT_COUNT %d\n", mesh.weights.size());
		fprintf(f, "\t*BONE_WEIGHTS {\n");

		for (j = 0; j < mesh.weights.size(); ++j)
		{
			BoneWeight w = mesh.weights[j];

			if (f)
			{
				for (k = 0; k < w.numWeights && k < 10; ++k)
				{
					fprintf(f, "\t\t*WEIGHT %i BONE: %ld %f\n", 
							j, w.bones[k], w.weights[k]);
				}
			}
		}

		fprintf(f, "\t}\n");

		printf("\t%i bones\n", bones.bones.size());
		for (i = 0; i < bones.bones.size(); ++i)
		{
			Bone* bone = bones.bones[i];

			if (bone && f)
			{
				/* HELPEROBJECT won't cut it seems to make a mesh tree */
				fprintf(f, "\t*BONE {\n");
				fprintf(f, "\t\t*ID %ld\n", bone->id);
				fprintf(f, "\t\t*PARENT %ld\n", bone->parent);
				fprintf(f, "\t\t*CHILDREN_COUNT %d\n", bone->children.size());
				fprintf(f, "\t\t*TRANSLATE_XYZ %f %f %f\n", 
						bone->translate.points[0], 
						bone->translate.points[1], 
						bone->translate.points[2]);
				fprintf(f, "\t\t*ROTATE_XYZW %f %f %f %f\n", 
						bone->quaternion.points[0], 
						bone->quaternion.points[1], 
						bone->quaternion.points[2],
						bone->quaternion.points[3]);

				// GrnMatrix matrix,curMatrix;
				//
				//for (j = 0; j < bone->children.size(); ++j)
				//{
				//	Bone* child = bone->children[i];
				//
				//	if (child)
				//	{
				//		printf("*CHILD %u\n", child->id);
				//	}
				//}

				fprintf(f, "\t}\n");
			}
		}

		fprintf(f, "}\n");

		fclose(f);
#endif
	}

	return 0;
}

