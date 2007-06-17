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
	mUID(INDEX_INVALID),
	mBones(),
	mRoot(0)
{
	mName[0] = '\0';
}


Skeleton::~Skeleton()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

uint32 Skeleton::GetCount()
{
	return mGobalPool.size();
}


Skeleton *Skeleton::GetSkeleton(index_t uid)
{
	if (uid < mGobalPool.size())
		return mGobalPool[uid];

	return 0x0;
}


bool Skeleton::Serialize(SystemIO::TextFileWriter &w)
{
	w.Print("Skeleton\n");
	w.Print("\t mVersion 1\n");
	
	w.Print("\t mUID %u\n", mUID);

	w.Print("\t mName \"%s\"\n", mName);

	w.Print("\t mBones %u ", mBones.size());
	for (uint32 i = 0, n = mBones.size(); i < n; ++i)
	{
		w.Print("%u ", mBones[i]);
	}
	w.Print("\n");

	w.Print("\t mRoot %u\n", mRoot);

	w.Print("END");

	return true;
}


#if 0 //TINYXML_FOUND
bool Skeleton::Serialize(TiXmlElement *container)
{
	w.Print("Skeleton\n");
	w.Print("\t mVersion 1\n");
	
	w.Print("\t mUID %u\n", mUID);

	w.Print("\t mName \"%s\"\n", mName);

	w.Print("\t mBones %u ", mBones.size());
	for (uint32 i = 0, n = mBones.size(); i < n; ++i)
	{
		w.Print("%u ", mBones[i]);
	}
	w.Print("\n");

	w.Print("\t mRoot %u\n", mRoot);

	w.Print("END");

	return true;
}

	bool Unserialize(TiXmlElement *container);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Unserializes object from XML.
	 *
	 ------------------------------------------------------*/
#endif


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void Skeleton::AddBone(index_t bone)
{
	uint32 i;
	foreach(mBones, i)
	{
		if (mBones[i] == bone)
			return;
	}

	// Mongoose 2006.12.08, Getting double hardcore with refs now
	Bone *b = Bone::GetBone(bone);
	if (b)
	{
		b->mSkeleton = GetUID();
	}

	mBones.push_back(bone);
}


index_t Skeleton::AddToPool()
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


void Skeleton::RemoveBone(index_t bone)
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


void Skeleton::RemoveFromPool()
{
	if (mUID < mGobalPool.size())
		mGobalPool.assign(mUID, 0x0);

	mUID = INDEX_INVALID;
}


void Skeleton::ResetPool()
{
	mGobalPool.clear();
}


bool Skeleton::Serialize(SystemIO::TextFileReader &r)
{
	if (strcmp(r.ParseSymbol(), "Skeleton"))
		return false;

	r.ParseSymbol(); // mVersion
	r.ParseInteger(); // == 1
	
	r.ParseSymbol(); // mUID
	r.ParseInteger(); // old mUID

	r.ParseSymbol(); // mName
	SetName(r.ParseStringLiteral());

	r.ParseSymbol(); // mBones
	uint32 count = r.ParseInteger();
	while (count > 0)
	{
		AddBone(r.ParseInteger());
		--count;
	}

	r.ParseSymbol(); // mRoot
	mRoot = r.ParseInteger();

	r.ParseSymbol(); // END

	return true;
}


void Skeleton::SetName(const char *name)
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
	Skeleton::ResetPool();
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

	return skel->AddToPool();
}


void freyjaSkeletonDelete(index_t skeletonIndex)
{
	Skeleton *skel = Skeleton::GetSkeleton(skeletonIndex);

	if (skel)
	{
		skel->RemoveFromPool();
		delete skel;
	}
}


void freyjaSkeletonUpdateBones(index_t skeletonIndex)
{
	Skeleton *skel = Skeleton::GetSkeleton(skeletonIndex);

	if (!skel)
		return;

	uint32 i;
	foreach( skel->GetBones(), i )
	{
		Bone *b = Bone::GetBone( skel->GetBones()[i] );

		if (b)
		{
			b->UpdateBindPose();	
		}
	}

#if 0 // not all joints are children of root 
	Bone *root = Bone::GetBone(skel ? skel->GetRoot() : 0);

	if (root) 
	{
		root->UpdateBindPose();
		root->UpdateBindPoseForChildren();
	}
#endif
}


void freyjaSkeletonRootIndex(index_t skeletonIndex, index_t boneIndex)
{
	Skeleton *skel = Skeleton::GetSkeleton(skeletonIndex);
	Bone *bone = Bone::GetBone(boneIndex);

	if (skel && bone)
		skel->SetRoot(boneIndex);
}


void freyjaSkeletonAddBone(index_t skeletonIndex, index_t boneIndex)
{
	Skeleton *skel = Skeleton::GetSkeleton(skeletonIndex);
	Bone *bone = Bone::GetBone(boneIndex);

	if (skel && bone)
	{
		unsigned int i;
		bool found = false;

		foreach (skel->GetBones(), i)
		{
			if (skel->GetBones()[i] == boneIndex )
				found = true;
		}

		if (!found)
			skel->AddBone(boneIndex);

		bone->mSkeleton = skel->GetUID();
	}
}

// ABI Accessors ///////////////////////

uint32 freyjaGetSkeletonCount()
{
	return Skeleton::GetCount();
}


uint32 freyjaGetSkeletonBoneCount(index_t skeletonIndex)
{
	Skeleton *skel = Skeleton::GetSkeleton(skeletonIndex);

	if (skel)
		return skel->GetBoneCount();

	return 0;
}


index_t freyjaGetSkeletonBoneIndex(index_t skeletonIndex, uint32 element)
{
	Skeleton *skel = Skeleton::GetSkeleton(skeletonIndex);

	if (skel && element < skel->GetBoneCount())
		return skel->GetBone(element);

	return INDEX_INVALID;
}


index_t freyjaGetSkeletonRootIndex(index_t skeletonIndex)
{
	Skeleton *skel = Skeleton::GetSkeleton(skeletonIndex);

	if (skel)
		return skel->GetRoot();

	return INDEX_INVALID;
}


void freyjaSkeletonTransform(index_t skeleton, 
                             freyja_transform_action_t action, 
                             vec_t x, vec_t y, vec_t z)
{
	
	Skeleton *skel = Skeleton::GetSkeleton(skeleton);

	if (skel)
	{
		switch (action)
		{
		case fTransformActionNone:
			break;

		case fTranslate:
		case fRotateAboutOrigin:
		case fRotate:
			freyjaBoneTransform(skel->GetRoot(), action, x, y, z);
			freyjaBoneUpdateBindPose(skel->GetRoot());			
			break;

		default:
			{
				unsigned int i, idx;
				foreach (skel->GetBones(), i)
				{
					idx = skel->GetBones()[i];
					freyjaBoneTransform(idx, action, x, y, z);
					freyjaBoneUpdateBindPose(idx);			
				}
			}
		}
	}
}


void freyjaSkeletonName(index_t skeleton, const char *name)
{
	Skeleton *skel = Skeleton::GetSkeleton(skeleton);
	if (skel)
		skel->SetName(name);
}


const char *freyjaGetSkeletonName(index_t skeleton)
{
	Skeleton *skel = Skeleton::GetSkeleton(skeleton);
	if (skel)
		return skel->GetName();
	return NULL;
}
