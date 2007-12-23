/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : Scene
 * License : No use w/o permission (C) 2005-2007 Mongoose
 * Comments: This is the scene graph.
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_SCENE - Builds class as a console unit test. 
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.12.06:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__FREYJA_SCENE_H_
#define GUARD__FREYJA_SCENE_H_

#include <hel/math.h>
#include <hel/Vec3.h>
#include <hel/Mat44.h>
#include <hel/Quat.h>
#include <mstl/Vector.h>
#include <mstl/String.h>

#include "Node.h"


namespace freyja {

class SceneManager;

class Scene 
{
public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Scene( );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post :
	 * 
	 ------------------------------------------------------*/

	~Scene( );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *
	 ------------------------------------------------------*/

	FREYJA_XMLSERIALIZER_INTERFACE
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : XmlSerializer interface macro.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	hel::Mat44 &GetBindPose() { return mBindPose; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns this bone's rest/bind pose transform
	 ------------------------------------------------------*/

	hel::Mat44 &GetInverseBindPose() { return mBindToWorld; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns this bone's 'rest to world' transform
	 ------------------------------------------------------*/

	hel::Mat44 &GetWorldPose() { return mTrack.mWorld; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns this bone's 'world' transform
	 ------------------------------------------------------*/

	byte GetFlags() { return mFlags; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Get this bone's option flags
	 ------------------------------------------------------*/

	const char *GetName() { return mName; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Get the human readable name of this bone
	 ------------------------------------------------------*/

	index_t GetParent() { return mParent; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Get the UID of this bone's parent bone or 
	 *        INDEX_INVALID if it has no parent
	 ------------------------------------------------------*/

	uint32 GetTrackCount() { return mTrackCount; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	BoneTrack &GetTrack(uint32 track) { return mTrack; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Only supporting one 'range/anim' in test: F(track) <- F(0)
	 ------------------------------------------------------*/

	bool Serialize(SystemIO::TextFileWriter &w);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	mstl::String mMetaData;          /* Metadata for bone */

	byte mFlags;                     /* Options bitmap */

	mstl::String mName;              /* Human readable identifier */

	mstl::String mFilename;


private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	mstl::list<freyja::Bone*> mBones;             /* */

	mstl::list<freyja::Camera*> mCamera;          /* */

	mstl::list<freyja::Light*> mLights;           /* */

	mstl::list<freyja::Mesh*> mMeshes;            /* */

	mstl::list<freyja::Metadata*> mMetadata;      /* */

	// FIXME: These should be shared by all scenes.
	// Break these out into a level above scene, and use instances in scenes.
	// Something like ResourceManager or something, and have them inherit from Resource.

	mstl::list<freyja::Skeleton*> mSkeletons;     /* */

	mstl::list<freyja::Material*> mMaterials;     /* */

	mstl::list<freyja::Texture*> mTextures;       /* */
};


} // namespace freyja

#endif // GUARD__FREYJA_SCENE_H_
