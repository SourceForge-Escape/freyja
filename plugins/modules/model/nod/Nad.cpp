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


#ifdef UNIT_TEST_NAD
int main(int argc, char *argv[])
{
	Nad nad;

	nad.Load(argv[1]);
	nad.Print();

	return 0;
}
#endif
