/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : Skeleton
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the skeletal animation subsystem
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2005.06.29:
 * Mongoose - Created
 ==========================================================================*/

#include "SkeletonABI.h"
#include "Skeleton.h"

using namespace freyja;

Vector<Skeleton *> Skeleton::mGobalPool;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Skeleton::Skeleton() :
	mBones(),
	mRoot(0),
	mAnimations(),
	mUID(INDEX_INVALID)
{
	mName[0] = '\0';
}


Skeleton::~Skeleton()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

SkeletalAnimation *Skeleton::getAnimation(index_t animationIndex)
{
	if (animationIndex < mAnimations.size())
		return mAnimations[animationIndex];

	return 0x0;
}


index_t Skeleton::getUID()
{
	return mUID;
}


uint32 Skeleton::getCount()
{
	return mGobalPool.size();
}


Skeleton *Skeleton::getSkeleton(index_t uid)
{
	if (uid < mGobalPool.size())
		return mGobalPool[uid];

	return 0x0;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void Skeleton::addBone(index_t bone)
{
	mBones.pushBack(bone);
}


index_t Skeleton::addToPool()
{
	uint32 i, count;
	bool found = false;


	if (mUID == INDEX_INVALID)
	{

		/* Setup UID and class container reference */
		mUID = count = mGobalPool.size();

		for (i = 0; i < count; ++i)
		{
			if (mGobalPool[i] == 0x0)
			{
				mUID = i;
				mGobalPool.assign(mUID, this);

				found = true;
			}	
		}

		if (!found)
		{
			mGobalPool.pushBack(this);
		}
	}

	return mUID;
}


index_t Skeleton::createAnimation()
{
	SkeletalAnimation *anim = new SkeletalAnimation();
	uint32 i, count;
	bool found = false;

	anim->mSkeleton = mUID;

	/* Setup Animation ID and class container reference */
	anim->mId = count = mAnimations.size();

	for (i = 0; i < count; ++i)
	{
		if (mAnimations[i] == 0x0)
		{
			anim->mId = i;
			mAnimations.assign(mUID, anim);
			found = true;
		}	
	}

	if (!found)
	{
		mAnimations.pushBack(anim);
	}

	return anim->mId;
}


void Skeleton::deleteAnimation(index_t animationIndex)
{
	if (animationIndex < mAnimations.size())
	{
		if (mAnimations[animationIndex])
			delete mAnimations[animationIndex];

		mAnimations.assign(animationIndex, 0x0);
	}
}


void Skeleton::removeBone(index_t bone)
{
	Vector<index_t> bones;
	uint32 i, n;

	// Yeah, it's crap -- but it's the only real way to repack w/o errors
	// and adjusting bounds -- also this isn't time sensitive
	for (i = 0, n = mBones.size(); i < n; ++i)
	{
		if (mBones[i] != bone)
		{
			bones.pushBack(mBones[i]);
		}
	}

	mBones.clear();

	for (i = 0, n = bones.size(); i < n; ++i)
	{
		mBones.pushBack(bones[i]);
	}
}


void Skeleton::removeFromPool()
{
	if (mUID < mGobalPool.size())
		mGobalPool.assign(mUID, 0x0);

	mUID = INDEX_INVALID;
}


void Skeleton::resetPool()
{
	mGobalPool.clear();
}


void Skeleton::setName(const char *name)
{
	uint32 i;

	if (name != 0x0 && name[0] != 0)
	{
		for (i = 0; i < 62 && name[i] != 0; ++i)
		{
			mName[i] = name[i];
		}

		mName[i] = 0;
		
		mName[63] = 0;
	}
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

#ifdef UNIT_TEST_SKELETON
int runSkeletonUnitTest(int argc, char *argv[])
{
	Skeleton test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[Skeleton class test]\n");

	return runSkeletonUnitTest(argc, argv);
}
#endif


////////////////////////////////////////////////////////////
// ABI
////////////////////////////////////////////////////////////

#include "BoneABI.h"
#include "Bone.h"


index_t gFreyjaCurrentSkeleton = INDEX_INVALID;

void freyjaSkeletonPoolClear()
{
	gFreyjaCurrentSkeleton = INDEX_INVALID;
	Skeleton::resetPool();
}


index_t freyjaGetCurrentSkeleton()
{
	return gFreyjaCurrentSkeleton;
}


void freyjaCurrentSkeleton(index_t skeletonIndex)
{
	if (freyjaIsBoneAllocated(skeletonIndex))
		gFreyjaCurrentSkeleton = skeletonIndex;
}


index_t freyjaSkeletonCreate()
{
	Skeleton *skel = new Skeleton();

	return skel->addToPool();
}


void freyjaSkeletonDelete(index_t skeletonIndex)
{
	Skeleton *skel = Skeleton::getSkeleton(skeletonIndex);

	if (skel)
	{
		skel->removeFromPool();
		delete skel;
	}
}


void freyjaSkeletonUpdateBones(index_t skeletonIndex)
{
	// FIXME
}


void freyjaSkeletonRootIndex(index_t skeletonIndex, index_t boneIndex)
{
	Skeleton *skel = Skeleton::getSkeleton(skeletonIndex);
	Bone *bone = Bone::GetBone(boneIndex);

	if (skel && bone)
		skel->mRoot = boneIndex;
}


void freyjaSkeletonAddBone(index_t skeletonIndex, index_t boneIndex)
{
	Skeleton *skel = Skeleton::getSkeleton(skeletonIndex);
	Bone *bone = Bone::GetBone(boneIndex);

	if (skel && bone)
	{
		skel->addBone(boneIndex);
		bone->mSkeleton = skel->getUID();
	}
}

// ABI Accessors ///////////////////////

uint32 freyjaGetSkeletonCount()
{
	return Skeleton::getCount();
}


uint32 freyjaGetSkeletonBoneCount(index_t skeletonIndex)
{
	Skeleton *skel = Skeleton::getSkeleton(skeletonIndex);

	if (skel)
		return skel->mBones.size();

	return 0;
}


index_t freyjaGetSkeletonBoneIndex(index_t skeletonIndex, uint32 element)
{
	Skeleton *skel = Skeleton::getSkeleton(skeletonIndex);

	if (skel && element < skel->mBones.size())
		return skel->mBones[element];

	return INDEX_INVALID;
}


index_t freyjaGetSkeletonRootIndex(index_t skeletonIndex)
{
	Skeleton *skel = Skeleton::getSkeleton(skeletonIndex);

	if (skel)
		return skel->mRoot;

	return INDEX_INVALID;
}


void freyjaSkeletonTransform(index_t boneIndex, 
                             freyja_transform_action_t action, 
                             vec_t x, vec_t y, vec_t z)
{
	//Matrix m;


	switch (action)
	{
	case fTranslate:
		break;

	case fRotate:
		break;

	case fScale:
		break;

	case fScaleAboutPoint:
		break;

	case fRotateAboutPoint:
		break;
	}

	freyjaPrintError("FIXME %s:%i", __FILE__, __LINE__);
}

