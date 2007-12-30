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

//class SceneManager;

class SceneResource;
typedef mstl::avl_tree<mstl::String, SceneResource*> SceneResourceDictionary;

class SceneResource
{
public:
	// Abstract out the current, dict, list, selected list into this class.

	SceneResource( const char* type, SceneResourceDictionary& dict ) :
		mType( type ),
		mNodes(),
		mSelected(),
		mDictionary()
	{
		dict.insert( mType, this ); 
	}

	~SceneResource()
	{ }

	const mstl::String& GetType()
	{ return mType; }

	NodeIterator GetIterator() const
	{ return mNodes.begin(); }

	NodeList& GetList()
	{ return mNodes; }

	NodeList& GetSelectedList()
	{ return mSelected; }

	NodeDictionary& GetDictionary()
	{ return mDictionary; }

	bool Serialize( XMLSerializerNode container ) const
	{
		bool ret = true;

		for ( NodeIterator it = GetIterator(); it != it.end(); ++it )
		{
			ret = ret && (*it)->Serialize( container );
		}

		// FIXME: Serialize selection list too?
		return ret;
	}

	mstl::String mType;
	NodeList mNodes;        
	NodeList mSelected;
	NodeDictionary mDictionary;
};


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

	//bool SerializeSelected( const char* filename ) const;
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

	bool Add( freyja::Node* node );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Uses RTTI to append to proper list.
	 *
	 ------------------------------------------------------*/

	bool Remove( freyja::Node* node );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Uses RTTI to remove from proper list.
	 *
	 ------------------------------------------------------*/

	void Select( freyja::Node* node );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Uses RTTI to add to proper selected list.
	 *
	 ------------------------------------------------------*/

	void Unselect( freyja::Node* node );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Uses RTTI to remove from proper selected list.
	 *
	 ------------------------------------------------------*/

	bool GetModified(  ) const
	{ return true; } // FIXME
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Has this been modified since the last call to
	 *        Serialize( )?
	 *
	 ------------------------------------------------------*/

	RenderableIterator GetRenderListIterator() const
	{ return mRenderList.begin(); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *
	 ------------------------------------------------------*/

	Mesh* GetCurrentMesh( ) const
	{ return mCurrentMesh; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Get currently used mesh for editing.
	 *
	 ------------------------------------------------------*/

	void SetCurrentMesh( Mesh* mesh )
	{ mCurrentMesh = mesh; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Set currently used mesh for editing.
	 *
	 ------------------------------------------------------*/


protected:

	NodeList* GetList( freyja::Node* node );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Uses RTTI to get proper type list.
	 *
	 ------------------------------------------------------*/

	NodeList* GetSelectionList( freyja::Node* node );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Uses RTTI to get proper type selection list.
	 *
	 ------------------------------------------------------*/

	mstl::String mFilename;    /* Filename if this has been saved as a disk file. */

	RenderableList mRenderList;/* */

	SceneResourceDictionary mResourceDict; /* Dictionary based on node type string. */

	/* Move selected bones to skeleton itself? a la mesh and it's components */
	NodeList mSelectedBones;      /* Special case - bones are contained in Skeletons. */
	Bone* mCurrentBone;

	SceneResource mCameras;       /* */
	Camera* mCurrentCamera;

	SceneResource mLights;        /* */
	Light* mCurrentLight;

	// FIXME SkeletalMesh, MorphMesh
	SceneResource mMeshes;        /* */
	Mesh* mCurrentMesh;

	SceneResource mMetadata;      /* */
	Metadata* mCurrentMetadata;

	SceneResource mSkeletons;     /* */
	Skeleton* mCurrentSkeleton;

	// FIXME: These should be shared by all scenes.
	// Break these out into a level above scene, and use instances in scenes.
	// Something like ResourceManager or something, and have them inherit from Resource.

	mstl::list<freyja::Material*> mMaterials;     /* */

	mstl::list<freyja::PixelBuffer*> mPixelBuffers;       /* */
};


} // namespace freyja

#endif // GUARD__FREYJA_SCENE_H_
