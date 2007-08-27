/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
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

#ifndef GUARD__FREYJA_MONGOOSE_OCTREE_H_
#define GUARD__FREYJA_MONGOOSE_OCTREE_H_

#include <hel/math.h>
#include <mstl/Vector.h>
#include <mstl/String.h>
#include "OctreeHelper.h"


class Octree
{
 public:

	class Node
	{
	 public:
		Node(hel::Vec3 min, hel::Vec3 max);

		Node();

		~Node();

		void Reset();

		void SubdivChild(OctreeHelper &helper, hel::Vec3 &min, hel::Vec3 &max);

		void Subdiv(OctreeHelper &helper);

		mstl::Vector<Node *> mChildren;

		mstl::Vector <uint32> mFaces;

		hel::Vec3 mMin, mMax;
	};

	// GeoNode // Arrays of vertex arrays to render|resolve

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

	//bool RayCollision(Ray &r, );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	Octree::Node &GetRoot() { return mRoot; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool Serialize(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool Unserialize(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////


 private:

	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	mstl::String mMetadata;         /* Useful for linking to geometry, etc. */

	Octree::Node mRoot;             /* Root node. */
};


inline
Octree::Node::Node(hel::Vec3 min, hel::Vec3 max) : 
	mChildren(), 
	mFaces(), 
	mMin(min), 
	mMax(max)
{}


inline
Octree::Node::Node() : 
	mChildren(), 
	mFaces(), 
	mMin(), 
	mMax()
{}


inline
Octree::Node::~Node()
{
	Reset();
}


inline
void Octree::Node::Reset()
{
	mMin = mMax = hel::Vec3();
	mFaces.clear();

	uint32 i;
	foreach (mChildren, i)
	{
		if ( mChildren[i] )
		{
			delete mChildren[i];
		}
	}

	mChildren.clear();
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


#endif // GUARD__FREYJA_MONGOOSE_OCTREE_H_


