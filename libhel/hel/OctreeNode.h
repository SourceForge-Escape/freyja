/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongooseichiban@gmail.com
 * Object  : OctreeNode
 * License : No use w/o permission (C) 2007 Mongoose
 * Comments: 
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongooseichban@gmail.com>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2007.08.25:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__HEL_OCTREENODE_H_
#define GUARD__HEL_OCTREENODE_H_

#include <hel/math.h>
#include <mstl/Vector.h>

#if TINYXML_FOUND
#   include <tinyxml/tinyxml.h>
#endif

#include "OctreeHelper.h"

namespace hel {


class OctreeNode
{
 public:
	OctreeNode(hel::Vec3 min, hel::Vec3 max);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	OctreeNode();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	~OctreeNode();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void Reset();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void SubdivChild(OctreeHelper &helper, hel::Vec3 &min, hel::Vec3 &max);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void Subdiv(OctreeHelper &helper);
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


	mstl::Vector<OctreeNode *> mChildren;

	mstl::Vector <uint32> mFaces;

	hel::Vec3 mMin, mMax;
};


////////////////////////////////////////////////////////////
// Inline Methods
////////////////////////////////////////////////////////////

inline
OctreeNode::OctreeNode(hel::Vec3 min, hel::Vec3 max) : 
	mChildren(), 
	mFaces(), 
	mMin(min), 
	mMax(max)
{}


inline
OctreeNode::OctreeNode() : 
	mChildren(), 
	mFaces(), 
	mMin(), 
	mMax()
{}


inline
OctreeNode::~OctreeNode()
{
	Reset();
}


inline
void OctreeNode::Reset()
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
void OctreeNode::SubdivChild(OctreeHelper &helper, hel::Vec3 &min, hel::Vec3 &max)
{
	// Test for geometry in the node region.
	uint32 count = helper.GetFaceCountInBBox(min, max); 

	if ( count )
	{
		OctreeNode *child = new OctreeNode(min, max);
		mChildren.push_back( child );

		if ( count > helper.GetMaxCount() )
		{
			child->Subdiv(helper);
		}
		else
		{
			// Only save face indices in leaves for now.
			helper.GetFacesIndicesInBBox(min, max, child->mFaces);
		}
	}
}


inline
void OctreeNode::Subdiv(OctreeHelper &helper)
{
	//helper.GetFacesIndicesInBBox(mMin, mMax, mFaces);

	// Compute the axis aligned bounding box (AABB) centroid.
	hel::Vec3 center = (mMin + mMax) * 0.5f;

	// Use the centroid to compute 8 new AABBs for subdivision.

	// TODO: Threading would be nice here.  This means you'd have to
	// use the external data structure for CS access control.  freyja::Mesh is not
	// thread safe for example.  This is why threading isn't used here.
	//
	// Using mstl thread delegate on the SubdivChild calls would be easy then.

	// 'Bottom' AABBs
	SubdivChild(helper, mMin, center);

	{
		hel::Vec3 min(mMin), max(center); 
		min.mX = center.mX;
		max.mX = mMax.mX;
		SubdivChild(helper, min, max);
	}

	{
		hel::Vec3 min(mMin), max(center); 
		min.mZ = center.mZ;
		max.mZ = mMax.mZ;
		SubdivChild(helper, min, max);
	}

	{
		hel::Vec3 min(center), max(mMax); 
		min.mY = mMin.mY;
		max.mY = center.mY;
		SubdivChild(helper, min, max);
	}

	// 'Top' AABBs.
	SubdivChild(helper, center, mMax);

	{
		hel::Vec3 min(center), max(mMax); 
		min.mX = mMin.mX;
		max.mX = center.mX;
		SubdivChild(helper, min, max);
	}

	{
		hel::Vec3 min(center), max(mMax); 
		min.mZ = mMin.mZ;
		max.mZ = center.mZ;
		SubdivChild(helper, min, max);
	}

	{
		hel::Vec3 min(mMin), max(center); 
		min.mY = center.mY;
		max.mY = mMax.mY;
		SubdivChild(helper, min, max);
	}
}


} // namespace hel

#endif // GUARD__HEL_OCTREENODE_H_




