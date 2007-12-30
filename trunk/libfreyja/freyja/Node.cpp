/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : Node
 * License : No use w/o permission (C) 2005-2007 Mongoose
 * Comments: This is the scene graph node interface.
 * 
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.12.06:
 * Mongoose - Created
 ==========================================================================*/

#include "Node.h"

using namespace freyja;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

freyja::Node* Node::GetChild( const uint16 id ) const
{
#warning There is no random access overlay or index map for this container.
	if ( id > GetChildCount() )
		return NULL;

	NodeIterator it = GetChildIterator();
	Node* child = NULL;

	for ( uint16 i = 0; *it && i <= id; ++i, ++it )
	{
		if ( i == id )
		{
			child = *it;
			break;
		}
	}

	return child;
}


freyja::Node* Node::GetChild( const char* name ) const
{
	mstl::String key( name );
	freyja::Node* child = mChildDictionary.find( key );
	return child;
}


void Node::CopyNodeMembers( freyja::Node* node ) const
{ 
	/* We don't duplicate children in this method. */
	if ( node )
	{
		node->mMuted = mMuted;
		node->mName = "Copy of " + mName;
		node->mMetadata = mMetadata;
		node->mPosition = mPosition;
		node->mOrientation = mOrientation;
		node->mScale = mScale;
		node->mLocalTransform = mLocalTransform;
		node->mParent = mParent;
	}
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void Node::RemoveChild( freyja::Node* child )
{
	if ( child )
	{
		child->mParent = NULL;
		mChildren.remove( child );

		/* It may not be a good idea to signal 
		 * parent change here, but it's hard to avoid. */
		child->NotifyOnParentChange( );
	}
}


void Node::RemoveChildren( )
{
	NodeIterator it = GetChildIterator();
	while ( *it )
	{
		RemoveChild( *it );
	}
}


void Node::SetParent( freyja::Node* parent ) 
{
	if ( parent != mParent )
	{
		if ( mParent ) 
		{
			parent->AddChild( this );
		}

		mParent = parent; 
		NotifyOnParentChange( );
	}
}


////////////////////////////////////////////////////////////
// Transforms
////////////////////////////////////////////////////////////

void Node::Pitch( const vec_t angle, TransformSpace about )
{
#warning FIXME
}


void Node::Roll( const vec_t angle, TransformSpace about )
{
#warning FIXME
}


void Node::Yaw( const vec_t angle, TransformSpace about )
{
#warning FIXME
}


void Node::Rotate( const vec_t angle, const hel::Vec3& axis, TransformSpace about )
{
#warning FIXME
}


void Node::Scale( const hel::Vec3& scale )
{
	mScale.mX *= scale.mX;
	mScale.mY *= scale.mY;
	mScale.mZ *= scale.mZ;
}


void Node::Translate( const hel::Vec3& v, TransformSpace about )
{
	if ( !mParent )
		about = tParent;

	switch ( about )
	{
	case tLocal:  
#warning FIXME Past libfreyja designs everything were world space only.  Need to add a local/world translation method first.
		break;

	case tParent:
		mPosition.mX += v.mX;
		mPosition.mY += v.mY;
		mPosition.mZ += v.mZ;
		break;

	case tWorld:
		{
			// FIXME: Use derived instead of world transform -- which might be based on animation.
			//        Likely going to retain the 'overlay' of animation transforms like older freyja.
			hel::Vec3 u = v - GetWorldPosition();
			mPosition.mX += u.mX;
			mPosition.mY += u.mY;
			mPosition.mZ += u.mZ;
		}
		break;

	default:
		;
	}
}


////////////////////////////////////////////////////////////
// Protected methods.
////////////////////////////////////////////////////////////

void Node::AddChild( freyja::Node* child ) 
{ 
	if ( child->mParent && child->mParent != this )
	{
		child->mParent->RemoveChild( child );
	}

	/* Keep children unique. */
	bool found = false;
	for ( NodeIterator it = GetChildIterator(); *it; it++ )
	{
		if ( *it == child )
		{
			found = true;
			break;
		}
	}
	
	if ( !found )
	{
		/* Resolve any unique name conflicts. */
		Node* node_with_same_name = GetChild( child->GetName() );

		if ( node_with_same_name )
		{
			// FIXME: Add a 'pretty' solution later.
			//ResolveNameConflict( child );

			/* They can't have the same pointer, so use that. */
			mstl::String s;
			s.Set( "%s_%p", child->GetName(), child );
			child->SetName( s.c_str() );
		}

		mChildDictionary.insert( mstl::String( child->GetName() ), child );
		mChildren.push_back( child );
	}

	child->mParent = this;
}


void Node::NotifyOnDelete() const
{
	ObserverIterator it = GetObserverIterator( );
	while ( *it )
	{
		((NodeObserver*)(*it))->NotifyDelete( this );
		++it;
	}
}


void Node::NotifyOnParentChange() const
{
	ObserverIterator it = GetObserverIterator( );
	while ( *it )
	{
		((NodeObserver*)(*it))->NotifyParentChange( this );
		++it;
	}
}


XMLSerializerNode Node::CreateXMLSerializerNode( ) const
{
	TiXmlElement* node = new TiXmlElement( GetType() );
	node->SetAttribute( "version", GetVersion() );	
	node->SetAttribute( "name", GetName() );

	if ( GetParent() )
	{
		/* Instead of storing a list of children --
		   we just store parent name reference here. */
		node->SetAttribute( "parent", GetParent()->GetName() );
	}

	TiXmlElement* properties = new TiXmlElement( "node-properties" );
	properties->SetAttribute( "muted", GetMute() );
	node->LinkEndChild( properties );

	TiXmlElement* position = new TiXmlElement( "position" );
	position->SetDoubleAttribute( "x", GetPosition().mX );
	position->SetDoubleAttribute( "y", GetPosition().mY );
	position->SetDoubleAttribute( "z", GetPosition().mZ );
	node->LinkEndChild( position );

	TiXmlElement* orientation = new TiXmlElement( "orientation" );
	orientation->SetDoubleAttribute( "x", GetOrientation().mX );
	orientation->SetDoubleAttribute( "y", GetOrientation().mY );
	orientation->SetDoubleAttribute( "z", GetOrientation().mZ );
	orientation->SetDoubleAttribute( "w", GetOrientation().mW );
	node->LinkEndChild( orientation );

	TiXmlElement* scale = new TiXmlElement( "scale" );
	scale->SetDoubleAttribute( "x", GetPosition().mX );
	scale->SetDoubleAttribute( "y", GetPosition().mY );
	scale->SetDoubleAttribute( "z", GetPosition().mZ );
	node->LinkEndChild( scale );

	if ( GetMetadata() )
	{
		TiXmlElement* metadata = new TiXmlElement( "metadata" );
		TiXmlText* text = new TiXmlText( GetMetadata() );
		metadata->LinkEndChild( text );
		node->LinkEndChild( metadata );
	}
}


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




