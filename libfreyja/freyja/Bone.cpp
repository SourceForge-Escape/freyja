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
	mTrack(),
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
			mRotation.X(), mRotation.Y(), mRotation.Z(), mRotation.W());

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


	w.Print("\t mTracks %u\n", 0);

	//BoneTrack mTrack;                /* Animation track(s) - only one in test */

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


void Bone::UpdateBindPose(index_t boneIndex, Matrix &m)
{
	Bone *b = Bone::GetBone(boneIndex);

	if (b)
	{
		if (Bone::GetBone(b->GetParent()) != NULL)
		{
			UpdateBindPose(b->GetParent(), m);
		}

		Matrix n;
		n = b->mRotation;
		n.Translate(b->mTranslation);

		m = m * n;
		b->mBindPose = m;
	}
}


void Bone::UpdateBindPose(const Matrix &m)
{
	mBindPose = Matrix(mRotation) * m;
	mBindPose.translate(mTranslation.mVec);

	for (uint32 i = 0; i < mChildren.size(); ++i)
	{
		Bone *b = GetBone(mChildren[i]);

		if (b)
		{
			b->UpdateBindPose(mBindPose);
		}
	}
}


void Bone::UpdateBindPose()
{
#if 0
	// MSTL_MSG("\tDisabled");
#elif 1
	Matrix m;
	UpdateBindPose(GetUID(), m);
#else
	Matrix m;
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
#endif
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
	mRotation.X(r.ParseFloat());
	mRotation.Y(r.ParseFloat());
	mRotation.Z(r.ParseFloat());
	mRotation.W(r.ParseFloat());

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
		//mTracks...
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




