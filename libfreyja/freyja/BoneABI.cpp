/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the ABI to expose Bone class and utils to C.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2006.07.09:
 * Mongoose - Created, Split from Bone.cpp
 ==========================================================================*/

#include "freyja.h"
#include "Bone.h"
#include "Skeleton.h"
#include "BoneABI.h"

using namespace freyja;


///////////////////////////////////////////////////////////////////////
// freyjaBone 0.10.0 ABI
//
///////////////////////////////////////////////////////////////////////

freyja_ptr freyjaBoneCreate( freyja_ptr skeleton_ptr, const char* name )
{
	freyja::Skeleton* skeleton = Skeleton::Cast( skeleton_ptr );
	freyja::Bone* bone = ( skeleton ) ? skeleton->CreateBone( name ) : NULL;
	return bone;
}


void freyjaBoneDelete( freyja_ptr bone_ptr )
{
	freyja::Bone* bone = Bone::Cast( bone_ptr );

	if ( bone )
	{
		delete bone;
	}
}
