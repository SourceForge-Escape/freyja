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

#ifndef GUARD__FREYJA_FREYJAOCTREEHELPER_H_
#define GUARD__FREYJA_FREYJAOCTREEHELPER_H_

#include <hel/math.h>
#include <hel/Vec3.h>
#include <freyja/Mesh.h>
#include <mstl/Vector.h>


class FreyjaOctreeHelper : public OctreeHelper
{
 public:

	FreyjaOctreeHelper();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Octree.
	 *
	 ------------------------------------------------------*/

	virtual ~FreyjaOctreeHelper();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Deconstructs an object of Octree.
	 *
	 ------------------------------------------------------*/

	virtual void ComputeBBox(hel::Vec3 &min, hel::Vec3 &max);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns a generalized bounding box for geometery.
	 *
	 ------------------------------------------------------*/

	virtual uint32 GetFaceCountInBBox(const hel::Vec3 &min, const hel::Vec3 &max);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns number of faces in a generalized bounding box.
	 *
	 ------------------------------------------------------*/

	virtual void GetFacesIndicesInBBox(const hel::Vec3 &min, const hel::Vec3 &max,
	                                   mstl::Vector<uint32> &indices);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns indices of faces in a generalized bounding box.
	 *
	 ------------------------------------------------------*/

	void SetMesh(freyja::Mesh *mesh);
	/*------------------------------------------------------
	 * Pre  : <mesh> can be NULL.
	 * Post : Set the mesh being used for octree terrian.
	 *
	 ------------------------------------------------------*/

 private:

	freyja::Mesh *mMesh;
};


inline
FreyjaOctreeHelper::FreyjaOctreeHelper() :
	mMesh(NULL)
{}


inline
FreyjaOctreeHelper::~FreyjaOctreeHelper()
{}


inline
void FreyjaOctreeHelper::ComputeBBox(hel::Vec3 &min, hel::Vec3 &max)
{
	if (mMesh)
	{
		mMesh->GetBBox(min.mVec, max.mVec);
	}
}


inline
uint32 FreyjaOctreeHelper::GetFaceCountInBBox(const hel::Vec3 &min, const hel::Vec3 &max)
{
	if (mMesh)
	{
		mMesh->ClearFlagForSelectedFaces(Face::fSelected);

		hel::Vec3 a(min), b(max);
		mMesh->SelectFacesByBox( a, b );

		Vector<index_t> faces = mMesh->GetSelectedFaces();
		return faces.size();
	}

	return 0;
}


inline
void FreyjaOctreeHelper::GetFacesIndicesInBBox(const hel::Vec3 &min, const hel::Vec3 &max,
                                               mstl::Vector<uint32> &indices)
{
	if (mMesh)
	{
		mMesh->ClearFlagForSelectedFaces(Face::fSelected);

		hel::Vec3 a(min), b(max);
		mMesh->SelectFacesByBox( a, b );

		indices = mMesh->GetSelectedFaces();
	}
}


inline
void FreyjaOctreeHelper::SetMesh(freyja::Mesh *mesh)
{
	mMesh = mesh;
}


#endif // GUARD__FREYJA_FREYJAOCTREEHELPER_H_


