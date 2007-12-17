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
 *-- Todo ---------------------------------------------------
 *           
 * It might be useful later to have checks for deformed faces, etc.
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.08.25:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__FREYJA_MONGOOSE_OCTREEHELPER_H_
#define GUARD__FREYJA_MONGOOSE_OCTREEHELPER_H_

#include <hel/math.h>
#include <hel/Vec3.h>
#include <mstl/Vector.h>


class OctreeHelper
{
 public:

	OctreeHelper();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Octree.
	 *
	 ------------------------------------------------------*/

	virtual ~OctreeHelper();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Deconstructs an object of Octree.
	 *
	 ------------------------------------------------------*/

	virtual void ComputeBBox(hel::Vec3 &min, hel::Vec3 &max) = 0;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns a generalized bounding box for geometery.
	 *
	 ------------------------------------------------------*/

	virtual uint32 GetFaceCountInBBox(const hel::Vec3 &min, const hel::Vec3 &max) = 0;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns number of faces in a generalized bounding box.
	 *
	 ------------------------------------------------------*/

	virtual void GetFacesIndicesInBBox(const hel::Vec3 &min, const hel::Vec3 &max,
	                                   mstl::Vector<uint32> &indices) = 0;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns indices of faces in a generalized bounding box.
	 *
	 ------------------------------------------------------*/

	uint32 GetMaxCount() { return mMaxFaceCount; }
	void SetMaxCount(uint32 count) { mMaxFaceCount = count; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Face count threshold property.
	 *
	 ------------------------------------------------------*/

 private:

	uint32 mMaxFaceCount;                /* Max face count per node. */
};


inline
OctreeHelper::OctreeHelper() :
	mMaxFaceCount(64)
{}


inline
OctreeHelper::~OctreeHelper()
{}


#endif // GUARD__FREYJA_MONGOOSE_OCTREEHELPER_H_


