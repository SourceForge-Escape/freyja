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
#include "Camera.h"
#include "Light.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "Metadata.h"
#include "PixelBuffer.h"


namespace freyja {

class SceneManager;

class Scene 
{
public:

	typedef mstl::list<freyja::Renderable*>::iterator RenderListIterator;
	typedef mstl::list<freyja::Renderable*> RenderList;


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

	bool Serialize( const char* filename ) const;
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool Unserialize( const char* filename );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool Add( freyja::Mesh* mesh );

	freyja::Mesh* GetSelectedMesh() const;

	void SetSelectedMesh( freyja::Mesh* mesh );

	bool Remove( freyja::Mesh* mesh );

	RenderListIterator GetRenderListIterator()
	{ return mRenderList.begin(); }


protected:

	mstl::String mFilename;                       /* Filename if this has been saved as a disk file. */

	RenderList mRenderList;                       /* */

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

	mstl::list<freyja::PixelBuffer*> mPixelBuffers;       /* */
};


} // namespace freyja

#endif // GUARD__FREYJA_SCENE_H_
