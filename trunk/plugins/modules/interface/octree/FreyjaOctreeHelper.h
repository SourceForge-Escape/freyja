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
#include <hel/moller-aabb-triangle.h>
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
		hel::Vec3 center = (min + max) * 0.5f;
		hel::Vec3 halfsize = (max - min) * 0.5f;
		unsigned int count = 0; 

		for ( unsigned int i = 0, n = mMesh->GetFaceCount(); i < n; ++i )
		{
			Face *face = mMesh->GetFace( i );

			if ( face )
			{
				switch ( face->mIndices.size() )
				{
				case 3:
					{
						vec3_t a, b, c;
						mMesh->GetVertexPos( face->mIndices[0], a );
						mMesh->GetVertexPos( face->mIndices[1], b );
						mMesh->GetVertexPos( face->mIndices[2], c );

						if (helAABB_OverlapTriangle(center.mVec, halfsize.mVec, a, b, c)) 
						{
							++count;
						}
					}
					break;

				case 4:
					{
						vec3_t a, b, c, d;
						mMesh->GetVertexPos( face->mIndices[0], a );
						mMesh->GetVertexPos( face->mIndices[1], b );
						mMesh->GetVertexPos( face->mIndices[2], c );
						mMesh->GetVertexPos( face->mIndices[3], d );

						if (helAABB_OverlapTriangle(center.mVec, halfsize.mVec, a, b, c) ||
						    helAABB_OverlapTriangle(center.mVec, halfsize.mVec, a, d, c)) 
						{
							++count;
						}
					}
					break;

				default:
					;  // Ignore unoptimized facets.
				}
			}
		}

		return count;
	}

	return 0;
}


inline
void FreyjaOctreeHelper::GetFacesIndicesInBBox(const hel::Vec3 &min, const hel::Vec3 &max,
                                               mstl::Vector<uint32> &indices)
{
	if (mMesh)
	{
		hel::Vec3 center = (min + max) * 0.5f;
		hel::Vec3 halfsize = (max - min) * 0.5f;
 
		for ( unsigned int i = 0, n = mMesh->GetFaceCount(); i < n; ++i )
		{
			Face *face = mMesh->GetFace( i );

			if ( face )
			{
				switch ( face->mIndices.size() )
				{
				case 3:
					{
						vec3_t a, b, c;
						mMesh->GetVertexPos( face->mIndices[0], a );
						mMesh->GetVertexPos( face->mIndices[1], b );
						mMesh->GetVertexPos( face->mIndices[2], c );

						if (helAABB_OverlapTriangle(center.mVec, halfsize.mVec, a, b, c)) 
						{
							indices.push_back( i );
						}
					}
					break;

				case 4:
					{
						vec3_t a, b, c, d;
						mMesh->GetVertexPos( face->mIndices[0], a );
						mMesh->GetVertexPos( face->mIndices[1], b );
						mMesh->GetVertexPos( face->mIndices[2], c );
						mMesh->GetVertexPos( face->mIndices[3], d );

						if (helAABB_OverlapTriangle(center.mVec, halfsize.mVec, a, b, c) ||
						    helAABB_OverlapTriangle(center.mVec, halfsize.mVec, a, d, c)) 
						{
							indices.push_back( i );
						}
					}
					break;

				default:
					;  // Ignore unoptimized facets.
				}
			}
		}


	}
}


inline
void FreyjaOctreeHelper::SetMesh(freyja::Mesh *mesh)
{
	mMesh = mesh;
}


#endif // GUARD__FREYJA_FREYJAOCTREEHELPER_H_


