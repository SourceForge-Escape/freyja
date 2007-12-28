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


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Skeleton::Skeleton(const char* name )
{
#warning FIXME
}


Skeleton::~Skeleton( )
{
#warning FIXME
}


freyja::Bone* Skeleton::CreateBone( )
{
#warning FIXME
	return NULL;
}


freyja::Bone* Skeleton::CreateBone( const char* name )
{
#warning FIXME
	return NULL;
}


freyja::Bone* Skeleton::GetBone( uint16 id ) const
{
#warning FIXME
	return NULL;
}


freyja::Bone* Skeleton::GetBone( const char* name ) const	
{
#warning FIXME
	return NULL;
}


uint32 Skeleton::GetBoneCount()
{
#warning FIXME
	return 0;
}


bool Skeleton::HasBone( const char* name ) const
{
#warning FIXME
	return false;
}


void Skeleton::RemoveBone( uint16 bone )
{
#warning FIXME
}


const char* Skeleton::GetType() const
{
	return "Skeleton"; 
}
 

uint32 Skeleton::GetVersion() const
{
	return 0; 
}


freyja::Node* Skeleton::Duplicate() const
{
#warning FIXME
	return NULL; 
}


freyja::Material* Skeleton::GetMaterial() const
{
#warning FIXME
	return NULL; 
}


void Skeleton::DuplicateChildren( freyja::Node* parent, bool recurse )
{
#warning FIXME
}


bool Skeleton::Serialize( XMLSerializerNode parent ) const
{
#warning FIXME
	return false;
}


bool Skeleton::Unserialize( XMLSerializerNode node )
{
#warning FIXME
	return false;
}


mstl::String Skeleton::GetInfo() const
{
#warning FIXME
	return mstl::String( "Skeleton" );
}


const hel::Quat& Skeleton::GetWorldOrientation() const
{
#warning FIXME
	return mOrientation;
}


const hel::Vec3& Skeleton::GetWorldPosition() const
{
#warning FIXME
	return mPosition;
}
