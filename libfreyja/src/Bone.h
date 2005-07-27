/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : Bone
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is part of the skeletal animation subsystem
 *
 *           Skeletons and their animations are seperate data structures, so
 *           reuse and blending is possible.  Not to mention more flexible.
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_BONE - Builds Bone class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2005.06.29:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_BONE_H_
#define GUARD__FREYJA_MONGOOSE_BONE_H_

#include <hel/math.h>
#include <hel/Vector3d.h>
#include <hel/Matrix.h>
#include <hel/Quaternion.h>

#include <mstl/Vector.h>
#include "freyja.h"


namespace freyja {

class Bone 
{
public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Bone();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	~Bone();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	index_t getUID();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	static uint32 getCount() { return mGobalPool.size(); } 
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	static Bone *getBone(index_t uid);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void addChild(index_t child);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	void removeChild(index_t child);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	void setName(const char *name);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	void updateBoneToWorld();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/


	byte mFlags;                     /* Options bitmap */

	char mName[64];                  /* Human readable identifier */

	index_t mSkeleton;               /* Skeleton to which this bone belongs */

	index_t mParent;                 /* Parent bone of this bone */

	Vector<index_t> mChildren;       /* Children bones of this bone */

	Quaternion mRotation;            /* Oreintation of this bone */

	Vector3d mTranslation;           /* Offset of this bone from parent */

	Matrix mBoneToWorld;             /* Transform vertices to world coords with
									  * this cache of the current orientation 
									  * and translation in matrix form */

private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////
	
	index_t mUID;                     /* System for allowing gobal references, 
									   * which are mostly for C API callbacks */

	static Vector<Bone *> mGobalPool; /* Storage for gobal access */
};

}

#endif

