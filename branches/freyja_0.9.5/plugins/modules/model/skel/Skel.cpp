/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : Skel
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This is an ASE like ASCII skeleton.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.12.31:
 * Mongoose - Created
 ==========================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mstl/SystemIO.h>

#include "Skel.h"

using namespace mstl;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Skel::Skel()
{
	mBoneCount = 0;
	mWeightCount = 0;
	
	mName[0] = 0;
	mBones = 0x0;
	mWeights = 0x0;
}


Skel::~Skel()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

int Skel::check(char *filename)
{
	if (SystemIO::File::CompareFilenameExtention(filename, "skel") == 0)
		return 0;

	return -1;
}


void Skel::print()
{
	unsigned int i, j;


	for (i = 0; i < mBoneCount; ++i)
	{
		printf("Bone[%d] { ", i);
	
		for (j = 0; j < mBones[i].childrenCount; ++j)
		{
			printf("%d ", mBones[i].children[j]);
		}

		printf("}\n");
	}
}


int Skel::save(char *filename)
{
	FILE *f;
	unsigned int a, b;
	Skel::weight_t *weight;


	if (!filename || !filename[0])
	{
		return -1;
	}

	f = fopen(filename, "w");

	if (!f)
	{
		perror(filename);
		return -2;
	}

	fprintf(f, "*SKELETON {\n");
	fprintf(f, "\t*NAME \"%s\"\n", "RestSkeleton");
	fprintf(f, "\t*BONE_COUNT %lu\n", mBoneCount);
	fprintf(f, "\t*WEIGHT_COUNT %lu\n", mWeightCount);

	fprintf(f, "\t*BONE_WEIGHTS {\n");

	for (a = 0; a < mWeightCount; ++a)
	{
		weight = &mWeights[a];

		while (weight)
		{
			fprintf(f, "\t\t*WEIGHT %d BONE: %d %f\n", 
					a, mWeights[a].boneId, mWeights[a].weight);

			weight = weight->next;
		}
	}
	
	fprintf(f, "\t}\n");

	for (b = 0; b < mBoneCount; ++b)
	{
		fprintf(f, "\t*BONE {\n");
		fprintf(f, "\t\t*ID %d\n", mBones[b].id);
		fprintf(f, "\t\t*PARENT %d\n", mBones[b].parent);
		fprintf(f, "\t\t*CHILDREN_COUNT %d\n", mBones[b].childrenCount);
		fprintf(f, "\t\t*TRANSLATE_XYZ %f %f %f\n",
				mBones[b].translation[1],
				mBones[b].translation[0],
				mBones[b].translation[2]);
		fprintf(f, "\t\t*ROTATE_XYZW %f %f %f %f\n",
				mBones[b].rotation[0],
				mBones[b].rotation[1],
				mBones[b].rotation[2],
				mBones[b].rotation[3]);
		fprintf(f, "\t}\n");
	}

	fprintf(f, "}\n");

	fclose(f);

	return 0;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

int Skel::load(char *filename)
{
	FILE *f;
	char buffer[256];
	unsigned int i, j, k, a, b, state, last;
	float n;
	char c;
	Skel::weight_t *weight;


	if (!filename || !filename[0])
		return -1;
	
	f = fopen(filename, "r");

	if (!f)
	{
		perror(filename);
		return -2;
	}

	printf("Loading SKEL\n"); //  %s...\n", filename);

	i = 0;
	state = 0;
	last = 100;

	while (fscanf(f, "%c", &c) != EOF && state != 666)
	{
		// printf("%c\n", c);

		switch (c)
		{
		case '\"':
		case '\t':
			break;
		case '\n':
			// printf("LINE  %s\n", buffer);

			switch (state)
			{
			case 0:
				if (!strncmp(buffer, "*SKELETON {", 11)) 
					state = 1;
				break;
			case 1:
				if (!strncmp(buffer, "*BONE_WEIGHTS {", 14))
				{
					state = 2;
				}
				else if (!strncmp(buffer, "*NAME ", 6))
				{
					strncpy(mName, (*(&buffer)+6), 64);
				}
				else if (!strncmp(buffer, "*BONE_COUNT ", 12))
				{
					mBoneCount = atoi(*(&buffer)+12);
					mBones = new Skel::bone_t[mBoneCount];
				}
				else if (!strncmp(buffer, "*WEIGHT_COUNT ", 14))
				{
					mWeightCount = atoi(*(&buffer)+14);
					mWeights = new Skel::weight_t[mWeightCount];
				}
				break;
			case 2:
				if (!strncmp(buffer, "*WEIGHT", 7)) 
				{
					sscanf(buffer, "*WEIGHT %d BONE: %d %f", &a, &b, &n);

					if (last != a)
					{
						mWeights[a].weight = n;
						mWeights[a].boneId = b;
						mWeights[a].next = 0x0;

						weight = &mWeights[a];
					}
					else
					{
						weight = weight->next = new Skel::weight_t;

						weight->weight = n;
						weight->boneId = b;
						weight->next = 0x0;
					}

					last = a;
				}				
				else if (!strncmp(buffer, "}", 1)) 
				{
					state = 3;
				}
				break;
			case 3:
				if (!strncmp(buffer, "*BONE {", 7)) 
				{
					b = 0;
				}
				else if (!strncmp(buffer, "*ID ", 4))
				{
					sscanf(buffer, "*ID %d", &b);
					mBones[b].id = b;
					mBones[b].name[0] = 0;
				}
				else if (!strncmp(buffer, "*PARENT ", 7))
				{
					sscanf(buffer, "*PARENT %d", &mBones[b].parent);
				}
				else if (!strncmp(buffer, "*NAME ", 6))
				{
					sscanf(buffer, "*NAME %s", mBones[b].name);
				}
				else if (!strncmp(buffer, "*CHILDREN_COUNT ", 16))
				{
					sscanf(buffer, "*CHILDREN_COUNT %d\n", &mBones[b].childrenCount);
				}
				else if (!strncmp(buffer, "*TRANSLATE_XYZ ", 15))
				{
					sscanf(buffer, "*TRANSLATE_XYZ %f %f %f",
						   &mBones[b].translation[0],
						   &mBones[b].translation[1],
						   &mBones[b].translation[2]);
				}
				else if (!strncmp(buffer, "*ROTATE_XYZW ", 12))
				{
					sscanf(buffer, "*ROTATE_XYZW %f %f %f %f",
						   &mBones[b].rotation[0],
						   &mBones[b].rotation[1],
						   &mBones[b].rotation[2],
						   &mBones[b].rotation[3]);
				}
				if (!strncmp(buffer, "}", 1)) 
				{
				}
				break;
			}

			buffer[0] = i = 0;
			break;
		default:
			buffer[i++] = c;
			buffer[i] = 0;
		}
	}


	/* Setup bone's children feild */
	for (i = 0; i < mBoneCount; ++i)
	{
		printf("++ Bone[%d] :: ", i);

		if (mBones[i].childrenCount > 0)
		{
			mBones[i].children = new unsigned int[mBones[i].childrenCount];

			for (j = 0, k = 0; j < mBoneCount; ++j)
			{
				if (mBones[j].parent == (int)i)// && k < mBones[i].childrenCount)
				{
					if (j == i) // Don't self link, jesus
					{
						//mBones[i].childrenCount--;
					}
					else
					{
						printf(" %d ", j);
						mBones[i].children[k++] = j; 
					}
				}
			}
		}

		printf("\n");
	}

	fclose(f);

	return 0;
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_SKEL
int runSkelUnitTest(int argc, char *argv[])
{
	Skel test;

	if (argc > 2)
		test.load(argv[1]);

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[Skel class test]\n");

	return runSkelUnitTest(argc, argv);
}
#endif


////////////////////////////////////////////////////////////
// Special Interface code
////////////////////////////////////////////////////////////

#ifdef FREYJA_PLUGINS

#include <freyja/ModelABI.h>
#include <freyja/PluginABI.h>
#include <freyja/BoneABI.h>
#include <freyja/SkeletonABI.h>
#include <freyja/TextureABI.h>
#include <freyja/LegacyABI.h>
#include <freyja/MeshABI.h>
#include <freyja/freyja.h>
#include <mstl/Vector.h>

using namespace mstl;

extern "C" {

	int freyja_model__skel_check(char *filename);
	int freyja_model__skel_import(char *filename);
	int freyja_model__skel_export(char *filename);
	int import_model(char *filename);
	void freyja_init();
}

void freyja_init()
{
	freyjaPluginDescription1s("SKEL Reference (*.skel)");
	freyjaPluginAddExtention1s("*.skel");
	freyjaPluginImport1i(FREYJA_PLUGIN_SKELETON);
	freyjaPluginExport1i(FREYJA_PLUGIN_SKELETON);
}

int freyja_model__skel_check(char *filename)
{
	Skel skel;

	return skel.check(filename);
}


int import_model(char *filename)
{
	if (freyja_model__skel_check(filename) == 0)
		return freyja_model__skel_import(filename);

	return -1;
}


int freyja_model__skel_import(char *filename)
{
	Skel skel;
	unsigned long i, j;
	long idx;


	if (skel.check(filename) != 0)
		return -1;

	if (skel.load(filename) != 0)
		return -2;

	/* Skeleton */
	if (skel.mBoneCount)
	{
		Skel::weight_t *weight;
		
		for (i = 0; i < skel.mWeightCount; ++i)
		{
			weight = &(skel.mWeights[i]);
			
			while (weight)
			{
				freyjaVertexWeight(i, 
								   skel.mWeights[i].weight,
								   skel.mWeights[i].boneId);
				weight = weight->next;
			}
		}

		freyjaBegin(FREYJA_SKELETON);

		for (i = 0; i < skel.mBoneCount; ++i)
		{
			/* Start a new tag */
			freyjaBegin(FREYJA_BONE);
			idx = freyjaGetCurrent(FREYJA_BONE);
			freyjaBoneFlags(idx, 0x0);
			freyjaBoneName(idx, skel.mBones[i].name);
			freyjaBoneTranslate3f(idx, 
								  skel.mBones[i].translation[0],
								  skel.mBones[i].translation[1],
								  skel.mBones[i].translation[2]);
			freyjaBoneRotateQuat4f(idx, 
									skel.mBones[i].rotation[0], // w?
									skel.mBones[i].rotation[1],
									skel.mBones[i].rotation[2],
									skel.mBones[i].rotation[3]);
			
			for (j = 0; j <  skel.mBones[i].childrenCount; ++j)
			{
				printf("%d ", skel.mBones[i].children[j]);
				freyjaBoneAddChild(idx, skel.mBones[i].children[j]);
			}
			
			printf("\n");
			
			freyjaEnd(); // FREYJA_TAG
		}

		freyjaEnd(); // FREYJA_BONE_FRAME
	}

	return 0;
}


int freyja_model__skel_export(char *filename)
{
	Skel skel;
	unsigned long i, j, b;
	long bone;


	/* SKELETON */
	freyjaIterator(FREYJA_SKELETON, FREYJA_RESET);

	skel.mBoneCount = freyjaGetCount(FREYJA_BONE);
	skel.mBones = new Skel::bone_t[skel.mBoneCount];
	freyjaIterator(FREYJA_BONE, FREYJA_RESET);

	for (b = 0; b < skel.mBoneCount; ++b)
	{
		skel.mBones[b].childrenCount = 0;
	}

	for (b = 0; b < skel.mBoneCount; ++b)
	{
		bone = freyjaIterator(FREYJA_BONE, FREYJA_CURRENT);
		bone = freyjaGetCurrent(FREYJA_BONE);

		skel.mBones[b].parent = freyjaGetBoneParent(bone);
		freyjaGetBoneRotationQuat4fv(bone, skel.mBones[b].rotation);
		freyjaGetBoneTranslation3fv(bone, skel.mBones[b].translation);
		freyjaGetBoneName(bone, 64, skel.mBones[b].name);

		skel.mBones[skel.mBones[b].parent].childrenCount++;

		freyjaPrintMessage("so: bone[%i].parent = %i\n", 
			   b, skel.mBones[b].parent);

		// Use translator list
		//trans.Add(bone, b);
		//printf("trans.Add(%i, %i)\n", bone, b);

		freyjaIterator(FREYJA_BONE, FREYJA_NEXT);
	}

	// Loop to populate children lists here
	for (b = 0; b < skel.mBoneCount; ++b)
	{
		if (skel.mBones[b].childrenCount == 0)
			continue;

		skel.mBones[b].children = new unsigned int[skel.mBones[b].childrenCount];

		for (i = 0, j  = 0; i < skel.mBoneCount; ++i)
		{
			if (skel.mBones[i].parent == (int)b)
				skel.mBones[b].children[j++] = i;
		}
	}

	if (skel.save(filename) != 0)
		return -1;

	return 0;
}
#endif
