/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : Lwo
 * License : GPL
 * Comments: NAD module for freyja
 *
 * 
 *-- History ------------------------------------------------ 
 *
 * 2006.01.21:
 * Mongoose - Created, Nad format supplied by documentation. Wow, docs.
 * Some Nad comments are right from the doc.  No special types in header by
 * request.
 ==========================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <mstl/SystemIO.h>
#include <mstl/String.h>

#include "Nad.h"

bool Nad::Load(const char *filename)
{
	mstl::SystemIO::BufferedFileReader r;

	if (!r.Open(filename))
		return false;

	mVersion = r.ReadLong();

	if (mVersion != 3)
		return false;

	mNumBoneTracks = r.ReadLong();

	mFlags = r.ReadLong();

	mDuration = r.ReadFloat32();

	mBoneTracks = new BoneTrack[mNumBoneTracks];

	for (unsigned int i = 0; i < mNumBoneTracks; ++i)
	{
		mBoneTracks[i].mNumKeys = r.ReadLong();
		mBoneTracks[i].mBoneNum = r.ReadLong();
		mBoneTracks[i].mTrackType = r.ReadLong();

		mBoneTracks[i].mKeys = new Keyframe[mBoneTracks[i].mNumKeys];

		for (unsigned int j = 0; j < mBoneTracks[i].mNumKeys; ++j)
		{
			mBoneTracks[i].mKeys[j].mFrame = r.ReadFloat32();
			mBoneTracks[i].mKeys[j].mFrameScale = r.ReadFloat32();
			mBoneTracks[i].mKeys[j].mValue[0] = r.ReadFloat32();
			mBoneTracks[i].mKeys[j].mValue[1] = r.ReadFloat32();
			mBoneTracks[i].mKeys[j].mValue[2] = r.ReadFloat32();
			mBoneTracks[i].mKeys[j].mCFactor[0] = r.ReadFloat32();
			mBoneTracks[i].mKeys[j].mCFactor[1] = r.ReadFloat32();
			mBoneTracks[i].mKeys[j].mCFactor[2] = r.ReadFloat32();
			mBoneTracks[i].mKeys[j].mBFactor[0] = r.ReadFloat32();
			mBoneTracks[i].mKeys[j].mBFactor[1] = r.ReadFloat32();
			mBoneTracks[i].mKeys[j].mBFactor[2] = r.ReadFloat32();
			mBoneTracks[i].mKeys[j].mAFactor[0] = r.ReadFloat32();
			mBoneTracks[i].mKeys[j].mAFactor[1] = r.ReadFloat32();
			mBoneTracks[i].mKeys[j].mAFactor[2] = r.ReadFloat32();
		}
	}

	mNumTags = r.ReadLong();

	mTags = new KeyframeTag[mNumTags]; 

	for (unsigned int i = 0; i < mNumTags; ++i)
	{
		mTags[i].mFrameNum = r.ReadFloat32();
		mTags[i].mTagType = r.ReadLong();
	}

	r.Close();

	return true;
}

	
bool Nad::Save(const char *filename)
{
	return false;
}


void Nad::Print()
{
	printf("mVersion = %i\n", mVersion);
	printf("mNumBoneTracks = %i\n", mNumBoneTracks);
	printf("mFlags = %i\n", mFlags);
	printf("mDuration = %f\n", mDuration);

	for (unsigned int i = 0; i < mNumBoneTracks; ++i)
	{
		printf("mBoneTracks[%i].mNumKeys = %i\n", i, mBoneTracks[i].mNumKeys);
		printf("mBoneTracks[%i].mBoneNum = %i\n", i, mBoneTracks[i].mBoneNum);
		printf("mBoneTracks[%i].mTrackType = %i\n", i, mBoneTracks[i].mTrackType);

		for (unsigned int j = 0; j < mBoneTracks[i].mNumKeys; ++j)
		{
			printf("mBoneTracks[%i].mKeys[%i].mFrame = %f\n", 
					i, j, mBoneTracks[i].mKeys[j].mFrame);

			printf("mBoneTracks[%i].mKeys[%i].mFrameScale = %f\n", 
					i, j, mBoneTracks[i].mKeys[j].mFrameScale);

			printf("mBoneTracks[%i].mKeys[%i].mValue = { %f, %f, %f }\n", i, j, 
					mBoneTracks[i].mKeys[j].mValue[0], 
					mBoneTracks[i].mKeys[j].mValue[1], 
					mBoneTracks[i].mKeys[j].mValue[2]);

			printf("mBoneTracks[%i].mKeys[%i].mCFactor = { %f, %f, %f }\n", i, j, 
					mBoneTracks[i].mKeys[j].mCFactor[0], 
					mBoneTracks[i].mKeys[j].mCFactor[1], 
					mBoneTracks[i].mKeys[j].mCFactor[2]);

			printf("mBoneTracks[%i].mKeys[%i].mBFactor = { %f, %f, %f }\n", i, j, 
					mBoneTracks[i].mKeys[j].mBFactor[0], 
					mBoneTracks[i].mKeys[j].mBFactor[1], 
					mBoneTracks[i].mKeys[j].mBFactor[2]);

			printf("mBoneTracks[%i].mKeys[%i].mAFactor = { %f, %f, %f }\n", i, j, 
					mBoneTracks[i].mKeys[j].mAFactor[0], 
					mBoneTracks[i].mKeys[j].mAFactor[1], 
					mBoneTracks[i].mKeys[j].mAFactor[2]);
		}
	}

	printf("mNumTags = %i\n", mNumTags);

	for (unsigned int i = 0; i < mNumTags; ++i)
	{
		printf("mTags[%i].mFrameNum = %f\n", i, mTags[i].mFrameNum);
		printf("mTags[%i].mTagType = %i\n", i, mTags[i].mTagType);
	}
}


////////////////////////////////////////////////////////////
// Special Interface code
////////////////////////////////////////////////////////////

#ifdef FREYJA_PLUGINS
#include <stdio.h>
#include <string.h>
#include <freyja/freyja.h>
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

	int freyja_model__nad_check(char *filename);
	int freyja_model__nad_import(char *filename);
	int freyja_model__nad_export(char *filename);
	int import_model(char *filename);
	void freyja_init();
}

void freyja_init()
{
	freyjaPluginDescription1s("Vampire Animation (*.nad)");
	freyjaPluginAddExtention1s("*.nad");
	freyjaPluginImport1i(FREYJA_SKEL_ANIMATION);
	freyjaPluginExport1i(FREYJA_PLUGIN_NONE);
}


int import_model(char *filename)
{
	if (!freyja_model__nad_check(filename))
		return freyja_model__nad_import(filename);

	return -1;
}


int freyja_model__nad_check(char *filename)
{
	if (!mstl::SystemIO::File::CompareFilenameExtention(filename, ".nad") ||
		!mstl::SystemIO::File::CompareFilenameExtention(filename, ".NAD"))
	{
		mstl::SystemIO::BufferedFileReader r;

		if (!r.Open(filename))
			return -1;

		if (r.ReadLong() != 3)
			return -1;

		r.Close();

		return 0;
	}

	return -2;
}


int freyja_model__nad_import(char *filename)
{
	Nad nad;

	if (!nad.Load(filename))
		return -1;

	for (unsigned int i = 0; i < nad.mNumBoneTracks; ++i)
	{
		index_t bone = nad.mBoneTracks[i].mBoneNum;
		index_t track = freyjaBoneTrackNew(bone);
		freyjaBoneTrackRate(bone, track, 30.0f);  

		for (unsigned int j = 0; j < nad.mBoneTracks[i].mNumKeys; ++j)
		{
			vec_t t = nad.mBoneTracks[i].mKeys[j].mFrame;
			vec_t x = nad.mBoneTracks[i].mKeys[j].mValue[0];
			vec_t y = nad.mBoneTracks[i].mKeys[j].mValue[1];
			vec_t z = nad.mBoneTracks[i].mKeys[j].mValue[2];

			index_t key = freyjaBoneKeyFrameNew(bone, track, t);

			switch (nad.mBoneTracks[i].mTrackType)
			{
			case 0:
				freyjaBoneRotKeyFrameEuler3f(bone, track, key, x, y, z);
				break;

			case 1:
				freyjaBonePosKeyFrame3f(bone, track, key, x, y, z);
				break;

			case 2:
				//freyjaBoneSizeKeyFrame3f(bone, track, key, x, y, z);
				break;

			default:
				;
			}
		}
	}

	return 0;
}


int freyja_model__nad_export(char *filename)
{
	freyjaPrintMessage("Nad Export: Not implemented, %s:%i\n", 
	                   __FILE__, __LINE__);
	return -1;
}
#endif


#ifdef UNIT_TEST_NAD
int main(int argc, char *argv[])
{
	Nad nad;

	nad.Load(argv[1]);
	nad.Print();

	return 0;
}
#endif
