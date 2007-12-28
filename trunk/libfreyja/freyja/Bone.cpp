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
#include <mstl/String.h>

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
	for ( Node::ChildIterator it = GetChildIterator(); it != it.end(); ++it )
	{
		freyja::Bone* child = (freyja::Bone*)(*it);
		
		if ( child )
		{
			/* Add this child to parent's child list. 
			 * Parent can be NULL. */
			child->SetParent( mParent );

			/* Update transforms. */
			child->UpdateBindPose();
			child->UpdateBindPoseOfChildren();
		}
	}
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// XML serialization.
////////////////////////////////////////////////////////////

bool Bone::Serialize( XMLSerializerNode container ) const
{	
	if ( !container || !mSkeleton )
		return false;

	TiXmlElement* bone = new TiXmlElement( GetType() );
	bone->SetAttribute( "version", GetVersion() );	
	//bone->SetAttribute( "skeleton", mSkeleton->GetName() );
	bone->SetAttribute( "name", GetName() );
	//bone->SetAttribute( "flags", GetFlags() );

	if ( GetParent() )
		bone->SetAttribute( "parent", GetParent()->GetName() );

	TiXmlElement* position = new TiXmlElement( "position" );
	position->SetDoubleAttribute( "x", GetPosition().mX );
	position->SetDoubleAttribute( "y", GetPosition().mY );
	position->SetDoubleAttribute( "z", GetPosition().mZ );
	bone->LinkEndChild( position );

	TiXmlElement* orientation = new TiXmlElement( "orientation" );
	orientation->SetDoubleAttribute( "x", GetOrientation().mX );
	orientation->SetDoubleAttribute( "y", GetOrientation().mY );
	orientation->SetDoubleAttribute( "z", GetOrientation().mZ );
	orientation->SetDoubleAttribute( "w", GetOrientation().mW );
	bone->LinkEndChild( orientation );

	if ( GetMetadata() )
	{
		TiXmlElement* metadata = new TiXmlElement("metadata");
		TiXmlText* text = new TiXmlText( GetMetadata() );

		/* Metadata is NOT written out as CDATA to avoid parsing it. */
		//text->SetCDATA(true);

		metadata->LinkEndChild(text);
		bone->LinkEndChild(metadata);
	}

	container->LinkEndChild( bone );

	return true;
}


bool Bone::Unserialize( XMLSerializerNode xml_bone )
{
	if ( !xml_bone )
		return false;

	{
		/* Make sure it's the same serialization version. */
		int version = 0;
		xml_bone->QueryIntAttribute("version", &version);
		if ( version != (int)GetVersion() )
			return false;
	}

	/*
	const char* skeleton_name = xml_bone->Attribute( "skeleton" );
	freyja::Skeleton* skeleton = ResourceManager::GetSkeleton( skeleton_name );

	if ( !skeleton )
		return false;

	const char* bone_name = xml_bone->Attribute( "name" );
	freyja::Bone* bone = skeleton->CreateBone( bone_name );
	*/

	const char* bone_name = xml_bone->Attribute( "name" );
	SetName( bone_name );

	{
		const char* parent_name = xml_bone->Attribute( "parent" );
		freyja::Bone* parent = mSkeleton->GetBone( parent_name );
		
		if ( parent )
			parent->AddChild( this );
	}

	TiXmlElement *child = xml_bone->FirstChildElement();
	for( ; child; child = child->NextSiblingElement() )
	{
		mstl::String s = child->Value();
		
		if (s == "position")
		{
			child->QueryFloatAttribute( "x", &mPosition.mX );
			child->QueryFloatAttribute( "y", &mPosition.mY );
			child->QueryFloatAttribute( "z", &mPosition.mZ );
		}
		else if (s == "orientation")
		{
			child->QueryFloatAttribute( "x", &mOrientation.mX );
			child->QueryFloatAttribute( "y", &mOrientation.mY );
			child->QueryFloatAttribute( "z", &mOrientation.mZ );
			child->QueryFloatAttribute( "w", &mOrientation.mW );
		}
		else if (s == "metadata")
		{
			SetMetadata( child->GetText() );
		}
	}

	return true;
}


const hel::Quat& Bone::GetWorldOrientation() const
{
#warning FIXME
	return mOrientation;
}


const hel::Vec3& Bone::GetWorldPosition() const
{
#warning FIXME
	return mPosition;
}


freyja::Material* Bone::GetMaterial() const
{
#warning FIXME
	return NULL;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

freyja::Node* Bone::Duplicate() const
{ 
	/* We don't duplicate children in this method. */
	mstl::String name = "Copy of " + mName;

	freyja::Bone* bone = FREYJA_NEW freyja::Bone( mSkeleton, name.c_str() );

	bone->mParent = mParent;
	bone->mPosition = mPosition;
	bone->mOrientation = mOrientation;
	bone->mMetadata = mMetadata;
	bone->mLocalTransform = mLocalTransform;
	bone->mBindPose = mBindPose;
	bone->mBindToWorld = mBindToWorld;
	bone->mRenderFlags = GetRenderFlags();

	return bone;
}


void Bone::DuplicateChildren( freyja::Node* parent, bool recurse )
{
#warning FIXME
#if FIXME
	Bone* orig_parent = parent;
	parent = this;
	bool link = true;

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
#endif
}


mstl::String Bone::GetInfo() const
{
	mstl::String info;

	info = "Bone";
#if 0
	info += " '" + mName + "'\n";
	info += "Parent '";
	info += ( mParent ? mParent->GetName() : "NULL" ) + "'\n";


			s.Set("\nTranslate %f %f %f\n", bone->mTranslation.mX, bone->mTranslation.mY, bone->mTranslation.mZ);
			info += s;

			{
				hel::Vec3 v;
				bone->mRotation.GetEulerAngles(v.mVec);
				s.Set("\nRotation (quat->Euler) %f %f %f\n", v.mX, v.mY, v.mZ);
				info += s;
			}

			info += "\nLocalTransform\n";
			s = bone->mLocalTransform.ToString();
			s.Replace('{', ' ');
			s.Replace('}', '\n');
			s.Replace('|', '\n');
			info += s;

			info += "\nBindPose\n";
			s = bone->mBindPose.ToString();
			s.Replace('{', ' ');
			s.Replace('}', '\n');
			s.Replace('|', '\n');
			info += s;

			info += "\nWorld\n";
			s = bone->mTrack.mWorld.ToString();
			s.Replace('{', ' ');
			s.Replace('}', '\n');
			s.Replace('|', '\n');
	info += s;	
#endif

	return info;			
}


void Bone::UpdateBindPose()
{
#if 0
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
#endif
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


void Bone::UpdateBindPoseOfChildren()
{
#if FIXME
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
#endif
}


#if 0
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
#endif


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




