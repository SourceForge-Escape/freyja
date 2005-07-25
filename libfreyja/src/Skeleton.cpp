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

#include "Skeleton.h"
#include "SkeletonABI.h"

using namespace freyja;


Vector<Bone *> Bone::mGobalPool; 


Bone::Bone()
{
	uint32 i, count;
	bool found = false;

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

	mSkeleton = INDEX_INVALID;
	mParent = INDEX_INVALID;
	mName[0] = '\0';
	mFlags = 0;
	mTranslation.zero();
	mRotation = Quaternion();
	updateBoneToWorld();
}


Bone::~Bone()
{
}


Bone *Bone::getBone(index_t uid)
{
	if (uid < mGobalPool.size())
		return mGobalPool[uid];

	return 0x0;
}


index_t Bone::getUID()
{
	return mUID;
}


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Skeleton::Skeleton()
{
}


Skeleton::~Skeleton()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// C ABI
////////////////////////////////////////////////////////////

index_t gFreyjaCurrentBone = INDEX_INVALID;

index_t freyjaGetCurrentBone()
{
	return gFreyjaCurrentBone;
}


void freyjaCurrentBone(index_t boneIndex)
{
	if (freyjaIsBoneAllocated(boneIndex))
		gFreyjaCurrentBone = boneIndex;
}

	///////////////////////////////////////////////////////////////////////
	// SKELETON
	///////////////////////////////////////////////////////////////////////

uint32 freyjaGetSkeletonBoneCount(index_t skeletonIndex)
{
	return 0;
}



index_t freyjaGetSkeletonBoneIndex(index_t skeletonIndex, uint32 element)
{
	return 0;
}


index_t freyjaGetSkeletonRootIndex(index_t skeletonIndex)
{
	return 0;
}



	///////////////////////////////////////////////////////////////////////
	// BONE
	///////////////////////////////////////////////////////////////////////

byte freyjaIsBoneAllocated(index_t boneIndex)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
	{
		return 1;
	}

	return 0;
}


index_t freyjaBoneCreate(index_t skeletonIndex)
{
	Bone *b = new Bone();

	b->mSkeleton = skeletonIndex;
	return b->getUID();
}


void freyjaBoneDelete(index_t boneIndex)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
		delete b;
}


void freyjaBoneFlags(index_t boneIndex, byte flags)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
		b->mFlags = flags;
}


void freyjaBoneParent(index_t boneIndex, index_t parentIndex)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
		b->mParent = parentIndex;
}


void freyjaBoneName(index_t boneIndex, const char *name)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
		b->setName(name);
}


void freyjaBoneRemoveChild(index_t boneIndex, index_t childIndex)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
		b->removeChild(childIndex);
}


void freyjaBoneAddChild(index_t boneIndex, index_t childIndex)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
		b->addChild(childIndex);
}


void freyjaBoneTranslate3f(index_t boneIndex, vec_t x, vec_t y, vec_t z)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
		b->mTranslation = Vector3d(x, y, z);
}


void freyjaGetBoneTranslation3fv(index_t boneIndex, vec3_t xyz)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
	{
		xyz[0] = b->mTranslation.mVec[0];
		xyz[1] = b->mTranslation.mVec[1];
		xyz[2] = b->mTranslation.mVec[2];
	}
}


void freyjaBoneTranslate3fv(index_t boneIndex, vec3_t xyz)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
		b->mTranslation = Vector3d(xyz[0], xyz[1], xyz[2]);
}


void freyjaBoneRotateEuler3f(index_t boneIndex, vec_t p, vec_t h, vec_t r)
{
	Bone *b = Bone::getBone(boneIndex);
	Quaternion q = Quaternion(p, h, r);

	if (b)
	{
		b->mRotation = q;
	}
}


void freyjaBoneRotateEuler3fv(index_t boneIndex, vec3_t phr)
{
	Bone *b = Bone::getBone(boneIndex);
	Quaternion q = Quaternion(phr[0], phr[1], phr[2]);

	if (b)
	{
		b->mRotation.getEulerAngles(phr);
	}
}



void freyjaBoneRotateQuat4f(index_t boneIndex,
							vec_t w, vec_t x, vec_t y, vec_t z)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
	{
		b->mRotation = Quaternion(w, x, y, z);
	}
}


void freyjaBoneRotateQuat4fv(index_t boneIndex, vec4_t wxyz)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
	{
		b->mRotation = Quaternion(wxyz[0], wxyz[1], wxyz[2], wxyz[3]);
	}
}


const char *freyjaGetBoneName1s(index_t boneIndex)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
	{
		return b->mName;
	}

	return 0x0;
}


void freyjaGetBoneName(index_t boneIndex, uint32 size, char *name)
{
	uint32 i, n = 64;
	Bone *b = Bone::getBone(boneIndex);

	if (b && name != 0x0 && size >= 64)
	{
		for (i = 0; i < n; ++i)
		{
			name[i] = b->mName[i];
		}
	}	
}


index_t freyjaGetBoneParent(index_t boneIndex)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
	{
		return b->mParent;
	}

	return INDEX_INVALID;
}


void freyjaGetBoneRotation4fv(index_t boneIndex, vec4_t wxyz)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
	{
		b->mRotation.getQuaternion4fv(wxyz);
	}
}


void freyjaGetBoneRotation3fv(index_t boneIndex, vec3_t phr)
{
	Bone *b = Bone::getBone(boneIndex);
	Quaternion q = Quaternion(phr[0], phr[1], phr[2]); // P H R -> R P Y

	if (b)
	{
		b->mRotation.getEulerAngles(phr, phr, phr);  // P H R -> H B A
	}
}


void freyjaGetBoneTranslation3fv(index_t boneIndex, vec3_t xyz);


index_t freyjaGetBoneSkeletonIndex(index_t boneIndex)
{
	return 0;
}


index_t freyjaGetBoneChild(index_t boneIndex, uint32 element)
{
	return 0;
}


uint32 freyjaGetBoneChildCount(index_t boneIndex)
{
	return 0;
}



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
