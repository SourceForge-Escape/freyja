/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : Skeleton
 * License : No use w/o permission (C) 2005, 2006 Mongoose
 * Comments: This is the skeletal class for freyja.
 *
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_SKELETON - Builds Skeleton class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2006.12.08:
 * Mongoose - API and code style clean up for 0.9.5 RC2 requirements
 *
 * 2005.06.29:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__FREYJA_SKELETON_H_
#define GUARD__FREYJA_SKELETON_H_

#if TINYXML_FOUND
#   include <tinyxml/tinyxml.h>
#endif

#include <hel/math.h>
#include <mstl/Vector.h>
#include "freyja.h"
#include "Bone.h"


namespace freyja {

class Skeleton
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Skeleton();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Skeleton
	 *
	 ------------------------------------------------------*/

	~Skeleton();
	/*------------------------------------------------------
	 * Pre  : Skeleton object is allocated
	 * Post : Deconstructs an object of Skeleton
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	index_t GetUID() { return mUID; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns UID of this skeleton
	 ------------------------------------------------------*/

	Vector<index_t> &GetBones() { return mBones; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns bones of this skeleton
	 ------------------------------------------------------*/

	uint32 GetBoneCount() { return mBones.size(); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Number of bones in this skeleton
	 ------------------------------------------------------*/

	index_t GetBone(uint32 i) { return mBones[i]; }
	/*------------------------------------------------------
	 * Pre  : <i> is less than GetBoneCount()
	 * Post : Returns bone index at the ith element
	 ------------------------------------------------------*/

	static uint32 GetCount();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Number of gobal skeleton slots active
	 ------------------------------------------------------*/

	const char *GetName() { return mName; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns human readable name of this skeleton
	 ------------------------------------------------------*/

	static Skeleton *GetSkeleton(index_t skeletonIndex);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns skeleton or NULL from the requested slot
	 ------------------------------------------------------*/

	uint32 GetRoot() { return mRoot; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns this skeleton's root index
	 ------------------------------------------------------*/

	bool Serialize(SystemIO::TextFileWriter &w);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Writes skeleton chunk out to text file
	 ------------------------------------------------------*/

#if TINYXML_FOUND
	bool Serialize(TiXmlElement *container);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Serializes object to XML.
	 *
	 ------------------------------------------------------*/

	bool Unserialize(TiXmlElement *container);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Unserializes object from XML.
	 *
	 ------------------------------------------------------*/
#endif


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void AddBone(index_t bone);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Adds bone to this skeleton
	 ------------------------------------------------------*/

	index_t AddToPool();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Adds this skeleton to gobal pool
	 ------------------------------------------------------*/

	void RemoveBone(index_t bone);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Removes a bone from this skeleton
	 ------------------------------------------------------*/

	void RemoveFromPool();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Removes this skeleton from the gobal pool
	 ------------------------------------------------------*/

	static void ResetPool();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Resets the gobal skeleton pool
	 ------------------------------------------------------*/

	bool Serialize(SystemIO::TextFileReader &r);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Reads skeleton chunk from text file
	 ------------------------------------------------------*/

	void SetName(const char *name);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Sets the name of this skeleton
	 ------------------------------------------------------*/

	void SetRoot(uint32 bone) { mRoot = bone; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Sets the root bone of this skeleton
	 ------------------------------------------------------*/


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	index_t mUID;                 /* System for allowing gobal references, 
	                               * which are mostly for C API callbacks */

	char mName[64];               /* Human readable identifier */

	Vector<index_t> mBones;       /* Bones in this skeleton */

	index_t mRoot;                /* Root bone index */

	static Vector<Skeleton *> mGobalPool; /* Storage for gobal access */
};

}

#endif
