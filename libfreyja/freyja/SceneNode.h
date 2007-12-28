/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : SceneNode
 * License : No use w/o permission (C) 2005-2007 Mongoose
 * Comments: This is the scene graph node interface.
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_NODE - Builds class as a console unit test. 
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.12.06:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__FREYJA_SCENENODE_H_
#define GUARD__FREYJA_SCENENODE_H_

#include <hel/BoundingVolume.h>

//#include "NodeAnimation.h"
#include "Node.h"

namespace freyja {

class SceneNode : public Node
{
public:
	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	SceneNode()
	{ }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	SceneNode(const char* name) :
		Node( name )
	{ }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	virtual ~SceneNode()
	{ }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Bounding Volume property
	////////////////////////////////////////////////////////////

	const hel::Vec3& GetBoundingBoxMax( ) const
	{ return mBoundingVolume.mBox.mMax;	}
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	const hel::Vec3& GetBoundingBoxMin( ) const
	{ return mBoundingVolume.mBox.mMin;	}
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	const hel::Vec3& GetBoundingSphereCenter( ) const
	{ return mBoundingVolume.mSphere.mCenter; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	const vec_t GetBoundingSphereRadius( ) const
	{ return mBoundingVolume.mSphere.mRadius; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Properties
	////////////////////////////////////////////////////////////

	//virtual NodeAnimationTrack* CreateNodeAnimationTrack( Animation* animation );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *
	 ------------------------------------------------------*/


protected:

	hel::BoundingBoxCombo mBoundingVolume;    /* BoundingVolume composite. */
};

} // namespace freyja

#endif // GUARD__FREYJA_SCENENODE_H_
