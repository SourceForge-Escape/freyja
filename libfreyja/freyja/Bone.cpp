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
#include "SkeletonABI.h"
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
	mChildren(), // 0
	mRotation(),
	mTranslation(0.0f, 0.0f, 0.0f),
	mBindPose(),
	mBindToWorld(),
	mTrack(),
	mTrackCount(0),
	mUID(INDEX_INVALID)
{
	mName[0] = '\0';

	// Still allow skeleton only by disabling this snippet here
#if 0
	// Force a single keyframe for loc/rot on all new bones.
	uint32 b = NewTrack();
	BoneTrack &t = GetTrack(b);
	index_t key = t.GetKeyfameIndex(0.0f);
	Vec3KeyFrame *kt = t.GetLocKeyframe(key);
	if (kt) kt->SetData(hel::Vec3());
	key = t.GetKeyfameIndex(0.0f);
	Vec3KeyFrame *kr = t.GetRotKeyframe(key);
	if (kr) kr->SetData(hel::Vec3());
#endif
}


Bone::~Bone()
{
	Bone *parent = GetBone(mParent);

	// Remove from parent's child list.
	if (parent)
	{
		RemoveChild(GetUID()); // 0.9.5.9
	}

	// Reparent children to parent, and update bind pose.
	for (unsigned int i = 0, n = mChildren.size(); i < n; ++i)
	{
		Bone *b = GetBone(mChildren[i]);

		if (b)
		{
			// Add this child to parent's child list.
			if (parent) parent->AddChild(mChildren[i]); // 0.9.5.9

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

	// 0.9.5.8, Use signed values for 'nil' parent.
	w.Print("\t mParent %i\n", (mParent == INDEX_INVALID) ? -1 : mParent);

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

	w.Print("\t mTracks %u\n", GetTrackCount());
	for (uint32 j = 0; j < GetTrackCount(); ++j)
	{
		BoneTrack &track = GetTrack(j);
		track.Serialize(w);
	}

	return true;
}


#if TINYXML_FOUND
////////////////////////////////////////////////////////////
// Optional XML serialization.
////////////////////////////////////////////////////////////

bool Bone::SerializePool(TiXmlElement *container)
{	
	if (!container)
		return false;

	TiXmlElement *bones = new TiXmlElement("bones");
	bones->SetAttribute("version", "2");

	// Assuming only one skeleton is ever loaded at a time.  ( It better be)
	const char *skeleton = freyjaGetSkeletonName(0) ? freyjaGetSkeletonName(0) : "unknown";
	bones->SetAttribute("skeleton", skeleton);

	container->LinkEndChild(bones);

	uint32 i;
	foreach(mGobalPool, i)
	{
		Bone *b = mGobalPool[i];

		if (!b)
			continue;

		TiXmlElement *bone = new TiXmlElement("bone");
		bone->SetAttribute("id", b->mUID);
		bone->SetAttribute("name", b->mName);
		bone->SetAttribute("flags", b->mFlags);

		{
			int idx = (b->mParent == INDEX_INVALID) ? -1 : b->mParent;
			bone->SetAttribute("parent", idx);
		}

		//bone->SetAttribute("skeleton", b->mSkeleton);

		TiXmlElement *translate = new TiXmlElement("translate");
		translate->SetDoubleAttribute("x", b->mTranslation.mX);
		translate->SetDoubleAttribute("y", b->mTranslation.mY);
		translate->SetDoubleAttribute("z", b->mTranslation.mZ);
		bone->LinkEndChild(translate);

		TiXmlElement *rotate = new TiXmlElement("rotate");
		rotate->SetDoubleAttribute("x", b->mRotation.mX);
		rotate->SetDoubleAttribute("y", b->mRotation.mY);
		rotate->SetDoubleAttribute("z", b->mRotation.mZ);
		rotate->SetDoubleAttribute("w", b->mRotation.mW);
		bone->LinkEndChild(rotate);

		// Metadata is NOT written out as CDATA to avoid parsing it.
		if ( b->mMetaData.c_str() )
		{
			TiXmlElement *metadata = new TiXmlElement("metadata");
			TiXmlText *text = new TiXmlText( b->mMetaData.c_str() );
			//text->SetCDATA(true);
			metadata->LinkEndChild(text);
			bone->LinkEndChild(metadata);
		}

		bones->LinkEndChild(bone);
	}

	return true;
}


bool Bone::UnserializePool(TiXmlElement *container)
{
	if (!container)
		return false;

	TiXmlElement *bones = container->FirstChildElement("bones");

	if (!bones)
		return false;

	int version = 0;
	bones->QueryIntAttribute("version", &version);

	if (version != 2)
		return false;
	
	/* This is only called to swap out skeletal systems, so it's brutal. */
	Vector<Bone *> tmp;
	ResetPool(); // NUKE ANY OTHER BONES 
	freyjaSkeletonPoolClear(); // NUKE ANY OTHER SKELETONS

	// Create a new skeleton.
	index_t skeleton = freyjaSkeletonCreate();
	freyjaSkeletonName(skeleton, bones->Attribute("skeleton"));

	TiXmlElement *bone = (bones) ? bones->FirstChildElement() : NULL;
	for( ; bone; bone = bone->NextSiblingElement() )
	{
		Bone *b = FREYJA_NEW Bone();
		tmp.push_back(b);
		b->mSkeleton = skeleton;
		b->AddToPool();
		freyjaSkeletonAddBone(skeleton, b->GetUID());

		int idx = -1;
		bone->QueryIntAttribute("id", &idx);
		b->mUID = (idx > -1) ? idx : INDEX_INVALID;
		
		{
			const char *name = bone->Attribute("name");
			freyjaPrintMessage("! bone '%s'...", name);
			b->SetName(name);
		}

		idx = 0;
		bone->QueryIntAttribute("flags", &idx);
		b->mFlags = idx;

		idx = -1;
		bone->QueryIntAttribute("parent", &idx);
		b->mParent = (idx > -1) ? idx : INDEX_INVALID;

		TiXmlElement *child = bone->FirstChildElement();
		for( ; child; child = child->NextSiblingElement() )
		{
			String s = child->Value();

			if (s == "translate")
			{
				child->QueryFloatAttribute("x", &b->mTranslation.mX);
				child->QueryFloatAttribute("y", &b->mTranslation.mY);
				child->QueryFloatAttribute("z", &b->mTranslation.mZ);
			}
			else if (s == "rotate")
			{
				child->QueryFloatAttribute("x", &b->mRotation.mX);
				child->QueryFloatAttribute("y", &b->mRotation.mY);
				child->QueryFloatAttribute("z", &b->mRotation.mZ);
				child->QueryFloatAttribute("w", &b->mRotation.mW);
			}
			else if (s == "metadata")
			{
				b->mMetaData = child->GetText();
			}
		}
	}

	unsigned int i;
	foreach(tmp, i)
	{
		// FIXME: Map UIDs to rebulid skeleton properly
		//        to account for sparse bone pool...
		if (tmp[i])
		{
			unsigned int j = tmp[i]->mParent;

			if (j < tmp.size() && tmp[j])
			{
				printf("%i -> %i\n", i, j);	
				tmp[j]->AddChild(i);
			}
		}
	}

	// Rebuild this mess!
	freyjaSkeletonRootIndex(skeleton, 0);
	freyjaSkeletonUpdateBones(skeleton);

	return true;
}

#endif


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
			mGobalPool.push_back(this);
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


Bone *Bone::Duplicate()
{ 
	// We don't duplicate children/tracks in this method.
	Bone *b = FREYJA_NEW Bone();
	b->mMetaData = mMetaData;
	b->mFlags = mFlags;
	strncpy(b->mName, mName, 64);
	b->mName[63] = 0;
	b->mSkeleton = mSkeleton;
	b->mParent = mParent;
	//b->mChildren;  // We don't duplicate children in this method.
	b->mRotation = mRotation;
	b->mTranslation = mTranslation;
	b->mLocalTransform = mLocalTransform;
	b->mBindPose = mBindPose;
	b->mBindToWorld = mBindToWorld;

	//mTrack = b->mTrack;
	//mTrackCount = b->mTrackCount;

	return b;
}


void Bone::DuplicateChildren(Bone *orig_parent, Bone *parent, 
							 bool recurse, bool link)
{
	if (orig_parent && parent)
	{
		uint32 count = orig_parent->mChildren.size();

		for (uint32 i = 0; i < count; ++i)
		{
			Bone *orig_child = Bone::GetBone(orig_parent->mChildren[i]);
			Bone *child = (orig_child) ? orig_child->Duplicate() : NULL;

			if (child)
			{
				child->mSkeleton = parent->mSkeleton;
				child->mParent = parent->GetUID();
				
				if (true) //link)
				{
					// Might want to factor this out to a recursve util method.
					child->AddToPool();
					parent->AddChild(child->GetUID());
					freyjaSkeletonAddBone(parent->mSkeleton, child->GetUID());
				}

				if (recurse)
				{
					child->DuplicateChildren(orig_child, child, recurse, link);
				}
			}
		}
	}
}


#if 0



Bone *Bone::DuplicateWithNewParent(index_t parent)
{
	// We don't duplicate children/tracks in this method.
	Bone *b = Duplicate();
	b->mParent = (parent == INDEX_INVALID) ? mParent : parent;

	return b;
}


index_t Bone::DuplicateChain(index_t start, mtsl::Vector<index_t> end)
{
	Bone *cur = GetBone(start);

	if (!cur)
		return INDEX_INVALID;

	Bone *b = cur->Duplicate();
	b->mSkeleton = cur->mSkeleton;
	b->AddToPool();
	freyjaSkeletonAddBone(b->mSkeleton, b->GetUID());

	return b->GetUID();
}
#endif


void Bone::ResetPool()
{
	uint32 i;

	foreach (mGobalPool, i)
	{
		if (mGobalPool[i])
		{
			delete mGobalPool[i];
			mGobalPool[i] = NULL;
		}
	}

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


void Bone::UpdateWorldPoseForChildren(index_t track, vec_t time)
{
	for (uint32 i = 0, n = mChildren.size(); i < n; ++i)
	{
		Bone *b = GetBone(mChildren[i]);

		if (b)
		{
			BoneTrack &t = b->GetTrack(track);
			hel::Vec3 rot = t.GetRot(time);
			hel::Vec3 loc = t.GetLoc(time);

			/* In this local case order doesn't matter for these operations. */
			t.mLocal.SetIdentity();
			t.mLocal.SetRotation(rot.mX, rot.mY, rot.mZ);
			t.mLocal.Translate(loc);

			/* Relative animations are currently being used... */
			helMatrixPostMultiply(b->mLocalTransform.mMatrix,  
								  t.mLocal.mMatrix,
								  t.mLocal.mMatrix);

			helMatrixPostMultiply(GetWorldPose().mMatrix,  
								  t.mLocal.mMatrix,
								  t.mWorld.mMatrix);
		}
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
	{
		int i = r.ParseInteger();
		mParent = ( i < 0 ) ? INDEX_INVALID : i; 
	}

	r.ParseSymbol(); // mChildren
	int32 count = r.ParseInteger();
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
		index_t t = NewTrack();
		BoneTrack &track = GetTrack(t);
		track.Serialize(r);
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




