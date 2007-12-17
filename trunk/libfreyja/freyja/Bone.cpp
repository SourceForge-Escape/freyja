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

#include "Skeleton.h"
#include "Bone.h"

using namespace freyja;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Bone::Bone( freyja::Skeleton* skeleton, const char* name ) :
	Node( name ),
	mSkeleton( skeleton ),
	mBindPose(),
	mBindToWorld()
{ }


Bone::~Bone()
{
	/* Remove from parent's child list. */
	if ( mParent )
	{
		mParent->RemoveChild( this );
	}
		
	/* Reparent children to parent, and update bind pose. */
	for ( NodeChildIterator it = GetChildIterator(); it != it.end(); ++it )
	{
		freyja::Bone* child = (freyja::Bone*)(*it);
		
		if ( child )
		{
			/* Add this child to parent's child list. 
			 * Parent can be NULL. */
			child->SetParent( mParent );

			/* Update transforms. */
			child->UpdateBindPose();
			child->UpdateBindPoseForChildren();
		}
	}
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// XML serialization.
////////////////////////////////////////////////////////////


//#warning FIXME This needs to be rewritten for 'BONE' level serail unit to be called from skeleton.

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
		//        which should never happen, as the pool 
		//        should be 'cleaned' to avoid sparse bones.
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


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

Bone *Bone::Duplicate()
{ 
	/* We don't duplicate children in this method. */
	Bone *b = FREYJA_NEW Bone();
	b->mMetaData = mMetaData;
	b->mFlags = mFlags;
	b->mName = "Copy of "
	b->mName += mName;
	b->mSkeleton = mSkeleton;
	b->mParent = mParent;
	b->mRotation = mRotation;
	b->mTranslation = mTranslation;
	b->mLocalTransform = mLocalTransform;
	b->mBindPose = mBindPose;
	b->mBindToWorld = mBindToWorld;

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


#if 0
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
#endif


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




