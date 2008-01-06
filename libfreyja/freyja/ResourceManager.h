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

#ifndef GUARD__FREYJA_RESOURCEMANAGER_H__
#define GUARD__FREYJA_RESOURCEMANAGER_H__

#include <mstl/list.h>
#include <mstl/map.h>
#include "freyja.h"


namespace freyja {

template <typename Type>
class ResourceManager
{
public:

	typedef mstl::map<mstl::String, Type*> ObjectMap;
	typedef mstl::list<mstl::String> KeyList;

 
	////////////////////////////////////////////////////////////
	// ResourceManager interface.
	////////////////////////////////////////////////////////////

	bool Register( const char* name, Type* object );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Register an object for this manager.
	 *        Returns false on name collision.
	 *
	 ------------------------------------------------------*/

	void Unregister( const char* name, bool destroy = false );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Unregister an object from this manager.
	 *        Delete the object as well if marked with destroy.
	 *
	 ------------------------------------------------------*/

	void UnregisterAll( bool destroy = false );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Unregister all objects from this manager.
	 *        Delete the objects as well if marked with destroy.
	 *
	 ------------------------------------------------------*/

	Type* Request( const char* name );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Return object by name or NULL if not found. 
	 *
	 ------------------------------------------------------*/

	KeyList::iterator& GetIterator( )
	{ return mKeyList.begin( ); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Return object key iterator. 
	 *
	 ------------------------------------------------------*/

	uint32 GetCount( )
	{ return mKeyList.size( ); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Return number of objects registered. 
	 *
	 ------------------------------------------------------*/

	void SetOverwriteOnCollision( const bool t )
	{ mOverwriteOnCollision = t; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : If there is a name collision overwrite old object.
	 *
	 ------------------------------------------------------*/

	void SetDeleteOnCollision( const bool t )
	{ mDeleteOnCollision = t; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : If there is: a name collision,
	 *        DeleteOnCollision is true and Overwrite is true:
	 *        delete old object.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Singleton methods.
	////////////////////////////////////////////////////////////

	static ResourceManager<Type>* GetInstance( );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Allocate the singleton instance if needed.
	 *        Return a pointer to the singleton instance.
	 *
	 ------------------------------------------------------*/

	static void DestroyInstance( );
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

	ObjectMap mObjects;

	KeyList mKeyList;

	bool mOverwriteOnCollision;

	bool mDeleteOnCollision;

private:

	static ResourceManager<Type>* mInstance;
};


////////////////////////////////////////////////////////////
// Inline ResourceManager methods.
////////////////////////////////////////////////////////////

template <typename Type>
inline
bool ResourceManager<Type>::Register( const char* name, Type* object )
{
	mstl::String key( name );
	Type* obj = mObjects.find( key );

	if ( obj )
	{
		if ( mOverwriteOnCollision )
		{
			mObjects.remove( key );
			mObjects.insert( mstl::String( name ), object );

			if ( mDeleteOnCollision )
			{
				delete obj;
			}
		}

		return false;
	}

	mObjects.insert( key, object );
	mKeyList.push_back( key );

	return true;
}


template <typename Type>
inline
void ResourceManager<Type>::Unregister( const char* name, bool destroy )
{
	mstl::String key( name );
	Type* obj = mObjects.find( key );
	if ( destroy )
	{
		delete obj;
	}

	mObjects.remove( key );
	mKeyList.remove( key );
}


template <typename Type>
inline
void ResourceManager<Type>::UnregisterAll( bool destroy )
{
	for (KeyList::iterator it = mKeyList.begin(); it != it.end(); it++ )
	{
		Unregister( (*it).c_str(), destroy );
	}
}


template <typename Type>
inline
Type* ResourceManager<Type>::Request( const char* name )
{
	mstl::String key( name );
	return mObjects.find( key );
}


////////////////////////////////////////////////////////////
// Inline singleton methods.
////////////////////////////////////////////////////////////

template <typename Type>
inline
ResourceManager<Type>* ResourceManager<Type>::GetInstance( )
{
	if ( !mInstance )
	{
		mInstance = new ResourceManager<Type>( );
	}

	return mInstance;
}


template <typename Type>
inline
void ResourceManager<Type>::DestroyInstance( )
{
	if ( mInstance )
	{
		delete mInstance;
		mInstance = NULL;
	}
}

} // namespace freyja

#endif // GUARD__FREYJA_RESOURCEMANAGER_H__
