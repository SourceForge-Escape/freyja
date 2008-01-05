/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : avl_tree<Key, Data>
 * License : No use w/o permission (C) 2000-2002 Mongoose
 * Comments: Templated AVL Tree implementation.
 *  
 *-- History ------------------------------------------------ 
 *
 * 2007.11.12:
 * Mongoose - Created, just wrap the red-black tree for now.
 *            The only difference is that AVL has better worst case
 *            lookup.
 ================================================================*/

#ifndef GUARD__MSTL_MAP__H_
#define GUARD__MSTL_MAP__H_

#include <stdlib.h>
#include <stdio.h>
#include "Tree.h"

namespace mstl {

template <typename Key, typename Data> 
class map
{
public:
	
	map() :
		mError( false ),
		mTree( )
	{ }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/
	
	~map()
	{ clear(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/
	
	void clear()
	{ mTree.Clear(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool error()
	{ return mError; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	Data find( Key& key ) const
	{ bool err; return mTree.SearchByKey(key, &err); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	//Data& find( Key& key )
	//{ return mTree.SearchByKey(key, &mError); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	Data& operator [] ( Key& key )
	{ return FindDataByKey(key); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/
	
	void insert(Key key, Data data)
	{ mTree.Insert( key, data ); }
	//mstl::map<Key, Data>::iterator insert( mstl::map<Key, Data>::iterator pos, Data& data )
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void remove(Key key)
	{ mTree.RemoveByKey( key ); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

private:
	
	bool mError;
	
	Tree<Key, Data> mTree;
};

} // namespace mstl

#endif // GUARD__MSTL_MAP__H_
