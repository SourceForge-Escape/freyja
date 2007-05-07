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

#include <hel/math.h>
#include "Bone.h"

using namespace freyja;

Vector<Bone *> Bone::mGobalPool; 


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Bone::Bone() :
	mMetaData(""),
	mFlags(0),
	mSkeleton(INDEX_INVALID),
	mParent(INDEX_INVALID),
	mChildren(0),
	mRotation(),
	mTranslation(0.0f, 0.0f, 0.0f),
	mBindPose(),
	mBindToWorld(),
	mTrack(),
	mTrackCount(0),
	mUID(INDEX_INVALID)
{
	mName[0] = '\0';
}


Bone::~Bone()
{
	for (unsigned int i = 0, n = mChildren.size(); i < n; ++i)
	{
		Bone *b = GetBone(mChildren[i]);

		if (b)
		{
			b->mParent = mParent;
			b->UpdateBindPose();
			b->UpdateBindPoseForChildren();
		}
	}
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


bool Bone::Serialize(SystemIO::TextFileWriter &w)
{
	w.Print("Bone\n");
	w.Print("\t mVersion 1\n");

	w.Print("\t mUID %u\n", mUID);
	
	w.Print("\t mFlags %u\n", mFlags);

	w.Print("\t mName \"%s\"\n", mName);

	w.Print("\t mSkeleton %u\n", mSkeleton);

	w.Print("\t mParent %u\n", mParent);

	w.Print("\t mChildren %u ", mChildren.size());

	uint32 i;
	foreach(mChildren, i)
	{
		w.Print("%u ", mChildren[i]);
	}
	w.Print("\n");

	w.Print("\t mRotation %f %f %f %f\n", 
			mRotation.mX, mRotation.mY, mRotation.mZ, mRotation.mW);

	w.Print("\t mTranslation %f %f %f\n", 
			mTranslation[0], mTranslation[1], mTranslation[2]);

	w.Print("\t mBindPose ");
	for (i = 0; i < 16; ++i)
	{
		if (i % 4 == 0) w.Print("\n\t");			
		w.Print("%f\t", mBindPose[i]);		
	}
	w.Print("\n");

	w.Print("\t mBindToWorld ");
	for (i = 0; i < 16; ++i)
	{
		if (i % 4 == 0) w.Print("\n\t");			
		w.Print("%f\t", mBindToWorld[i]);
	}
	w.Print("\n");

	w.Print("\t mTracks %u\n", 1); // only have one in test =p
	mTrack.Serialize(w);

	return true;
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
				break;
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
	// In this local case order doesn't matter for these operations
	mLocalTransform = mRotation;
	mLocalTransform.Translate(mTranslation);

	Bone *parent = Bone::GetBone( GetParent() );

	if (parent)
	{
		helMatrixPostMultiply(parent->mBindPose.mMatrix, 
							  mLocalTransform.mMatrix,
							  mBindPose.mMatrix);
	}
	else
	{
		mBindPose = mLocalTransform;
	}

	mBindPose.GetInverse(mBindToWorld);
}


void Bone::UpdateBindPoseForParent()
{
	// In this local case order doesn't matter for these operations
	mLocalTransform = mRotation;
	mLocalTransform.Translate(mTranslation);

	Bone *parent = Bone::GetBone( GetParent() );

	if (parent)
	{
		parent->UpdateBindPose();

		// FIXME: Pre or Post?  damn refactoring -- time to move into the API
		//mBindPose = parent->mBindPose * mLocalTransform;
		helMatrixPostMultiply(parent->mBindPose.mMatrix, 
							  mLocalTransform.mMatrix,
							  mBindPose.mMatrix);
	}
	else
	{
		mBindPose = mLocalTransform;
	}

	mBindPose.GetInverse(mBindToWorld);

	// Update children
	//UpdateBindPose(mBindPose); // BAD IDEA, don't write code after 0400 ;)
}


void Bone::UpdateBindPoseForChildren()
{
	for (uint32 i = 0, n = mChildren.size(); i < n; ++i)
	{
		Bone *b = GetBone(mChildren[i]);

		if (b)
		{
			b->mLocalTransform = b->mRotation;
			b->mLocalTransform.Translate(b->mTranslation);

			helMatrixPostMultiply(mBindPose.mMatrix, 
								  b->mLocalTransform.mMatrix,
								  b->mBindPose.mMatrix);

			b->mBindPose.GetInverse(b->mBindToWorld);
			b->UpdateBindPoseForChildren();
		}
	}
}


void Bone::UpdateWorldPose(index_t track, vec_t time)
{
	BoneTrack &t = GetTrack(track);
	hel::Vec3 rot = t.GetRot(time);
	hel::Vec3 loc = t.GetLoc(time);

	/* In this local case order doesn't matter for these operations. */
	t.mLocal.SetIdentity();
	t.mLocal.SetRotation(rot.mX, rot.mY, rot.mZ);
	t.mLocal.Translate(loc);

	/* Relative animations are currently being used... */
	helMatrixPostMultiply(mLocalTransform.mMatrix,  
						  t.mLocal.mMatrix,
						  t.mLocal.mMatrix);

	/* Update pose completely to be sure ancestors haven't changed. */
	Bone *parent = Bone::GetBone( GetParent() );

	if (parent)
	{
		parent->UpdateWorldPose(track, time);

		helMatrixPostMultiply(parent->GetWorldPose().mMatrix,  
							  t.mLocal.mMatrix,
							  t.mWorld.mMatrix);
	}
	else
	{
		t.mWorld = t.mLocal;
	}	
}


bool Bone::Serialize(SystemIO::TextFileReader &r)
{
	r.ParseSymbol(); // Bone
	
	r.ParseSymbol(); // mVersion
	r.ParseInteger(); // == 1

	r.ParseSymbol(); // mUID
	r.ParseInteger();

	r.ParseSymbol(); // mFlags
	mFlags = r.ParseInteger();

	r.ParseSymbol(); // mName
	SetName(r.ParseStringLiteral()); // might want to free returned string =p

	r.ParseSymbol(); // mSkeleton
	mSkeleton = r.ParseInteger();

	r.ParseSymbol(); // mParent
	mParent = r.ParseInteger();

	r.ParseSymbol(); // mChildren
	uint32 count = r.ParseInteger();
	while (count > 0)
	{
		mChildren.push_back(r.ParseInteger());
		--count;
	}

	r.ParseSymbol(); // mRotation
	mRotation.mX = r.ParseFloat();
	mRotation.mY = r.ParseFloat();
	mRotation.mZ = r.ParseFloat();
	mRotation.mW = r.ParseFloat();

	r.ParseSymbol(); // mTranslation
	mTranslation[0] = r.ParseFloat();
	mTranslation[1] = r.ParseFloat();
	mTranslation[2] = r.ParseFloat();

	r.ParseSymbol(); // mBindPose
	for (uint32 i = 0; i < 16; ++i)
	{ 
		mBindPose[i] = r.ParseFloat();
	}

	r.ParseSymbol(); //mBindToWorld
	for (uint32 i = 0; i < 16; ++i)
	{
		mBindToWorld[i] = r.ParseFloat();
	}

	r.ParseSymbol(); // mTracks
	count = r.ParseInteger();
	while (count > 0)
	{
		// only have one in test =p
		mTrack.Serialize(r);
		--count;
	}

	return true;
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




