/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : Bone
 * License : No use w/o permission (C) 2005-2007 Mongoose
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

#ifndef GUARD__FREYJA_BONE_H_
#define GUARD__FREYJA_BONE_H_

#include "Node.h"


namespace freyja {

class Skeleton;

class Bone : 
		public Node
{
public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Bone( freyja::Skeleton* skeleton, const char* name );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Constructor. 
	 *
	 ------------------------------------------------------*/

	~Bone();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Destructor.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Interfaces
	////////////////////////////////////////////////////////////

	FREYJA_XMLSERIALIZER_INTERFACE
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : XmlSerializer interface macro.
	 *
	 ------------------------------------------------------*/

	virtual const char* GetType() const
	{ return "Bone"; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : XmlSerializer tag type.
	 *
	 ------------------------------------------------------*/
 
	virtual uint32 GetVersion() const
	{ return 0; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : XmlSerializer tag version.
	 *
	 ------------------------------------------------------*/

	FREYJA_RENDERABLE_INTERFACE
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : XmlSerializer interface macro.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	const hel::Mat44& GetBindPose() const
	{ return mBindPose; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns this bone's rest/bind pose transform
	 ------------------------------------------------------*/

	const hel::Mat44& GetInverseBindPose() const
	{ return mBindToWorld; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns this bone's 'rest to world' transform
	 ------------------------------------------------------*/

	//const hel::Mat44& GetWorldPose() const { return mTrack.mWorld; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns this bone's 'world' transform
	 ------------------------------------------------------*/

	virtual freyja::Node* Duplicate() const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Copy of this node sans children.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	//void AddChild(index_t child);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Add child bone to this bone by <child> UID.
	 *
	 ------------------------------------------------------*/

	//void RemoveChild(index_t child);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Remove child bone of this bone by <child> UID.
	 *
	 ------------------------------------------------------*/

	//void SetLoc(const hel::Vec3 &v);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Set parent relative translation.
	 *
	 ------------------------------------------------------*/

	//void SetRot(const hel::Quat &q);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Set parent relative rotation.
	 *
	 ------------------------------------------------------*/

	void UpdateBindPose();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Only updates bind pose for this bone using
	 *        parent bind transform and a newly computed
	 *        local transform.
	 *
	 ------------------------------------------------------*/

	void UpdateWorldPose( index_t track, vec_t time );
	/*------------------------------------------------------
	 * Pre  : Input <track> and <time> is a valid animation.
	 * Post : Traces back the heirarchy to get a full transform.
	 *        Alters/updates parents as well.
	 *
	 ------------------------------------------------------*/

	static freyja::Bone* Cast( freyja_ptr ptr )
	{ return (freyja::Bone*)ptr; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 * Notes: FIXME Add RTTI checking.
	 *
	 ------------------------------------------------------*/

protected:

	void UpdateBindPoseOfChildren();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Updates this bone's child bind transforms only.
	 *
	 ------------------------------------------------------*/

	void UpdateWorldPoseOfChildren( index_t track, vec_t time );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Updates this bone's child world transforms only.
	 *
	 ------------------------------------------------------*/

	freyja::Skeleton* mSkeleton;     /* Skeleton to which this bone belongs */

	//hel::Mat44 mLocalTransform;      /* Might as well cache local transform */

	hel::Mat44 mBindPose;            /* Store the bind transform from the 
									  * origin ( this bulids off parents ).
									  * This transforms vertices to rest pose */

	hel::Mat44 mBindToWorld;         /* Cached inverse of mBindTransform:
									  * Transform vertices to world coords with
									  * this cache of the current orientation 
									  * and translation in matrix form */

private:
	Bone(const freyja::Bone&);
	
};


} // namespace freyja

#endif // GUARD__FREYJA_BONE_H_
