/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2005-2007 Mongoose
 * Comments: 
 *
 * 
 *-- History ------------------------------------------------ 
 *
 * 2007.12.12:
 * Mongoose - Created.
 ==========================================================================*/

#ifndef GUARD__FREYJA_RESOURCEMANAGER__H_
#define GUARD__FREYJA_RESOURCEMANAGER__H_

#include <mstl/list.h>
#include <mstl/map.h>
#include "freyja.h"


namespace freyja {

template <typename Type>
class ResourceManager
{
public:
 
	////////////////////////////////////////////////////////////
	// ResourceManager interface.
	////////////////////////////////////////////////////////////

	bool Register( const mstl::String& name, Type* object );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Register an object for this manager.
	 *        Returns false on name collision.
	 *
	 ------------------------------------------------------*/

	void Unregister( const mstl::String& name, bool destroy = false );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Unregister an object from this manager.
	 *        Delete the object as well if marked with destory.
	 *
	 ------------------------------------------------------*/

	Type* Request( const mstl::String& name );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Return object by name or NULL if not found. 
	 *
	 ------------------------------------------------------*/

	void DestoryAll( );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Delete all objects held by this manager.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Singleton methods.
	////////////////////////////////////////////////////////////

	ResourceManager* GetInstance( );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Allocate the singleton instance if needed.
	 *        Return a pointer to the singleton instance.
	 *
	 ------------------------------------------------------*/

	void DestroyInstance( );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Deallocate the singleton instance.
	 *
	 ------------------------------------------------------*/

protected:

	ResourceManager( ) :
		mObjects( ),
		mOverwriteOnCollision( false ),
		mDeleteOnCollision( false )
	{ }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	~ResourceManager( )
	{ }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Destructor.
	 *
	 ------------------------------------------------------*/

	typedef mstl::map<mstl::String, T*> ObjectMap;

	ObjectMap mObjects;

	bool mOverwriteOnCollision;

	bool mDeleteOnCollision;

private:

	static ResourceManager* mInstance;
};



////////////////////////////////////////////////////////////
// Inline singleton methods.
////////////////////////////////////////////////////////////

template <typename Type>
inline
freyja::ResourceManager* ResourceManager::GetInstance( )
{
	if ( !mInstance )
	{
		mInstance = new Resource( );
	}

	return mInstance;
}


template <typename Type>
inline
void ResourceManager::DestroyInstance( )
{
	if ( mInstance )
	{
		delete mInstance;
		mInstance = NULL;
	}
}

} // namespace freyja

#endif // GUARD__FREYJA_RESOURCEMANAGER__H_
