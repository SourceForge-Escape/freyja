/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongooseichiban@gmail.com
 * Object  : Octree
 * License : No use w/o permission (C) 2007 Mongoose
 * Comments: 
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongooseichban@gmail.com>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_OCTREE - Builds Octree class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.08.25:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__HEL_OCTREE_H_
#define GUARD__HEL_OCTREE_H_

#include <hel/math.h>
#include <mstl/Vector.h>
#include <mstl/String.h>

#if TINYXML_FOUND
#   include <tinyxml/tinyxml.h>
#endif

#include "OctreeHelper.h"
#include "OctreeNode.h"

namespace hel {

class Octree
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Octree();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Octree
	 *
	 ------------------------------------------------------*/

	~Octree();
	/*------------------------------------------------------
	 * Pre  : Octree object is allocated
	 * Post : Deconstructs an object of Octree
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Properties
	////////////////////////////////////////////////////////////

	const char *GetMetadata() { return mMetadata.c_str(); }
	void SetMetadata(const char *s) { mMetadata = s; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	void Reset();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void Generate(OctreeHelper &helper);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	//OctreeNode *RayCollision(const hel::Ray &ray);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	//OctreeNode *GetNodeAtPos(vec3_t pos);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	OctreeNode &GetRoot() { return mRoot; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

#if TINYXML_FOUND

	bool Serialize(TiXmlElement* element);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool Unserialize(TiXmlElement* element);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

#endif // TINYXML_FOUND


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////


 private:

	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	mstl::String mMetadata;         /* Useful for linking to geometry, etc. */

	OctreeNode mRoot;               /* Root node. */
};


////////////////////////////////////////////////////////////
// Inline Methods
////////////////////////////////////////////////////////////

inline
Octree::Octree() :
	mMetadata(),
	mRoot()
{ }


inline
Octree::~Octree()
{
	Reset(); 
}



inline
void Octree::Reset()
{
	mRoot.Reset();
}


inline 
void Octree::Generate(OctreeHelper &helper)
{
	/* 1. Find min/max of the scene or terrian piece. */
	helper.ComputeBBox(mRoot.mMin, mRoot.mMax);

	/* 2. Subdivide region recursively by 8 accounting for face count threshold. */
	mRoot.Subdiv(helper);

	/* 3. Prune out empty regions. */

	// FIXME: Not implemented for now.
}




} // namespace hel 

#endif // GUARD__HEL_OCTREE_H_


