/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : Bone
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

#include "Bone.h"

using namespace freyja;

Vector<Bone *> Bone::mGobalPool; 


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Bone::Bone() :
	mFlags(0),
	mSkeleton(INDEX_INVALID),
	mParent(INDEX_INVALID),
	mChildren(0),
	mRotation(0.0f, 0.0f, 0.0f),
	mTranslation(0.0f, 0.0f, 0.0f),
	mBindPose(),
	mBindToWorld(),
	mKeyframes(),
	mUID(INDEX_INVALID)
{
	mName[0] = '\0';
}


Bone::~Bone()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

index_t Bone::GetUID()
{
	return mUID;
}


uint32 Bone::GetCount() 
{
	return mGobalPool.size();
} 


Bone *Bone::GetBone(index_t uid)
{
	if (uid < mGobalPool.size())
		return mGobalPool[uid];

	return 0x0;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void Bone::AddChild(index_t child)
{
	mChildren.pushBack(child);
}


index_t Bone::AddToPool()
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


void Bone::RemoveChild(index_t child)
{
	Vector<index_t> children;
	uint32 i, n;

	// Yeah, it's crap -- but it's the only real way to repack w/o errors
	// and adjusting bounds -- also this isn't time sensitive
	for (i = 0, n = mChildren.size(); i < n; ++i)
	{
		if (mChildren[i] != child)
		{
			children.pushBack(mChildren[i]);
		}
	}

	mChildren.clear();

	for (i = 0, n = children.size(); i < n; ++i)
	{
		mChildren.pushBack(children[i]);
	}
}


void Bone::RemoveFromPool()
{
	if (mUID < mGobalPool.size())
		mGobalPool.assign(mUID, 0x0);

	mUID = INDEX_INVALID;
}


void Bone::ResetPool()
{
	mGobalPool.clear();
}


void Bone::SetName(const char *name)
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


void Bone::UpdateBindPose()
{
	Matrix m;
	uint32 i, n;

	m.setIdentity();
	m = mRotation;
	m.translate(mTranslation.mVec);

	Bone *parent = GetBone(mParent);

	if (parent)
	{
		mBindPose = parent->mBindPose * m;
	}
	else
	{
		mBindPose = m;
	}

	// Cache off a 'to world' 
	mBindToWorld = mBindPose;
	mBindPose.invert();

	for (i = 0, n = mChildren.size(); i < n; ++i)
	{
		Bone *b = GetBone(mChildren[i]);

		if (b)
		{
			b->UpdateBindPose();
		}
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

#ifdef UNIT_TEST_BONE
int runBoneUnitTest(int argc, char *argv[])
{
	Bone bone;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[Bone class test]\n");

	return runBoneUnitTest(argc, argv);
}
#endif




