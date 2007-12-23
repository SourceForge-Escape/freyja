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

#include <mstl/list.h>
#include <mstl/avl_tree.h>
#include <mstl/String.h>
#include <mstl/Vector.h>

#include "Node.h"
#include "Bone.h"
#include "AnimationFactory.h"

namespace freyja {

class Skeleton : 
		public Node,
		public AnimationFactory
{
 public:

	typedef mstl::avl_tree<mstl::String, freyja::Bone*> BoneDictionary;

	typedef mstl::avl_tree<mstl::String, freyja::Animation*> AnimationDictionary;


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Skeleton(const char* name );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	~Skeleton( );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Destructor.
	 *
	 ------------------------------------------------------*/

	freyja::Bone* CreateBone( );
	freyja::Bone* CreateBone( const char* name );
	/*------------------------------------------------------
	 * Pre  : The <name> must be unique.
	 * Post : Returns a new bone, or NULL if name is not unique.
	 *
	 ------------------------------------------------------*/

	freyja::Bone* GetBone( uint16 id ) const;
	freyja::Bone* GetBone( const char* name ) const;
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns bone matching <id> or NULL.
	 *
	 ------------------------------------------------------*/	

	uint32 GetBoneCount();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns number of bones in this scene.
	 *
	 ------------------------------------------------------*/

	bool HasBone( const char* name ) const;
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void RemoveBone( uint16 bone );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Removes a bone from this skeleton.
	 *
	 ------------------------------------------------------*/

	freyja::Bone* GetRoot() 
	{ return mRoot; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns the 'root' joint, which is the parent
	 *        of the main heirachry -- there may be other
	 *        unparented bones however.
	 *
	 ------------------------------------------------------*/

	void SetRoot( freyja::Bone* root ) 
	{ mRoot = root; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Sets the root bone.
	 *
	 ------------------------------------------------------*/

	static freyja::Skeleton* Cast( freyja_ptr ptr )
	{ return (freyja::Skeleton*)ptr; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 * Notes: FIXME Add RTTI checking.
	 *
	 ------------------------------------------------------*/

	FREYJA_XMLSERIALIZER_INTERFACE
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : XmlSerializer interface macro.
	 *
	 ------------------------------------------------------*/


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	mstl::Vector<Bone*> mBoneArray; 

	mstl::Vector<uint16> mBoneIdMap;

	BoneDictionary mBoneDict;           /* Lookup Bone by name. */

	AnimationDictionary mAnimationDict; /* Lookup Animation by name. */

	freyja::Bone* mRoot;                /* Root bone. */
};

} // namespace freyja
 
#endif // GUARD__FREYJA_SKELETON_H_
