/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : RenderModel
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the runtime data structure translation class, which
 *           allows freyja to swap backend structures but export a
 *           standard structure for rendering
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_RENDERMODEL - Builds RenderModel class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2005.05.07:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_RENDERMODEL_H_
#define GUARD__FREYJA_MONGOOSE_RENDERMODEL_H_


#include <hel/math.h>
#include <hel/Vector3d.h>
#include <mstl/Vector.h>

#include "freyja.h"
#include "BoneABI.h"
#include "FreyjaPluginABI.h"
#include "SkeletonABI.h"


class RenderBone
{
public:

	unsigned int getChildrenCount()
	{
		return freyjaGetBoneChildCount(mBoneIndex);
	}

	long getBoneIndex(unsigned int index)
	{
		return freyjaGetBoneChild(mBoneIndex, index);
	}

	void set(index_t boneIndex)
	{
		mBoneIndex = boneIndex;
		freyjaGetBoneRotationEuler3fv(boneIndex, rotate.mVec);
		rotate = rotate * HEL_180_OVER_PI;
		freyjaGetBoneTranslation3fv(boneIndex, translate.mVec);
	}

	void set(index_t boneIndex, unsigned int frame)
	{
		mBoneIndex = boneIndex;
		freyjaGetBoneRotationEuler3fv(boneIndex, rotate.mVec);
		rotate = rotate * HEL_180_OVER_PI;
		freyjaGetBoneTranslation3fv(boneIndex, translate.mVec);
		/*
		unsigned int i, n = tag->keyframes.size();
		mTag = tag;

		if (n > 0)
		{
			for (i = 0; i < n; ++i)
			{
				if (tag->keyframes[i]->frameIndex == (int)frame)
				{
					translate = Vector3d(tag->keyframes[i]->translate);
					rotate = Vector3d(tag->keyframes[i]->rotate);
				}
			}
		}

		translate = Vector3d(tag->center);
		rotate = Vector3d(tag->rot);
		*/
	}

	index_t mBoneIndex;
	Vector3d translate;
	Vector3d rotate;
};


class RenderSkeleton
{
public:

	uint32 getBoneCount()
	{
		return freyjaGetSkeletonBoneCount(mSkeletonIndex);
	}


	bool getBoneKeyframe(uint32 index, uint32 frame, RenderBone &bone)
	{
		index_t boneIndex = freyjaGetSkeletonBoneIndex(mSkeletonIndex, index);
		index_t keyframe = INDEX_INVALID;

		if (boneIndex != INDEX_INVALID && keyframe != INDEX_INVALID)
		{
			bone.set(boneIndex, keyframe);
			return true;
		}

		return false;
	}


	bool getBone(uint32 index, RenderBone &bone)
	{
		index_t boneIndex = freyjaGetSkeletonBoneIndex(mSkeletonIndex, index);

		if (boneIndex != INDEX_INVALID)
		{
			bone.set(boneIndex);
			return true;
		}

		return false;
	}

	void set(uint32 skeletonIndex)
	{
		mSkeletonIndex = skeletonIndex;
	}

	index_t mSkeletonIndex;
};


class RenderPolygon  
{
public:
	/* I've limited the renderer to 6-side polygons recently, so deal */
	Vector3d vertices[6];
	Vector3d texcoords[6];
	Vector3d normals[6];
	vec4_t colors[6];
	unsigned char flags;
	long material;
	long id;
	unsigned int count; // vertex count
};



class RenderMesh
{
public:

	RenderMesh() :
		id(INDEX_INVALID),
		frame(0),
		count(0),
		gbegin(0),
		gend(0)
	{
	}

	Vector3d getGroupCenter(unsigned int i)
	{
		Vector3d v;

		vec3_t center;
		//FIXME: This is just a 0.9.3 placeholder that's fine for now
		freyjaGetMeshFrameCenter(id, 0, center);
		v.Set(center);
		return v;
	}


	bool getPolygon(unsigned int index, RenderPolygon &face);


	bool getPolygon(unsigned int findex, long frame, RenderPolygon &face)
	{
		getPolygon(findex, face);

		return false;
	}

	unsigned int getPolygonCount()
	{
		return freyjaGetMeshPolygonCount(id);
	}

	index_t id;
	long frame;
	uint32 count;
	unsigned int gbegin, gend;

private:
};



class RenderModel
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	RenderModel();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of RenderModel
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.07: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~RenderModel();
	/*------------------------------------------------------
	 * Pre  : RenderModel object is allocated
	 * Post : Deconstructs an object of RenderModel
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.07: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	bool getRenderMesh(uint32 polygonIndex, RenderMesh &rmesh, int32 frame);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if valid RenderMesh is set.
	 *        
	 ------------------------------------------------------*/

	bool getRenderPolygon(uint32 polygonIndex, RenderPolygon &face);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if valid RenderPolygon is set.
	 *        
	 ------------------------------------------------------*/

	RenderSkeleton &getSkeleton();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *        
	 ------------------------------------------------------*/

	unsigned int getMeshCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *        
	 ------------------------------------------------------*/

	bool getMesh(int32 index, RenderMesh &mesh, int32 frame);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *        
	 ------------------------------------------------------*/



	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////
	
	void createRenderMesh(RenderMesh &rmesh, index_t meshIndex, int32 frame);
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Egg realtime data translator method
	 *
	 ------------------------------------------------------*/


	int32 mIndex;


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	RenderSkeleton mSkeleton;

};


bool freyjaGetRenderModelPolygon(uint32 modelIndex, uint32 polygonIndex, 
								 RenderPolygon &face);

bool freyjaGetRenderModelMesh(uint32 modelIndex, uint32 meshIndex, uint32 frame,
							  RenderMesh &mesh);

uint32 freyjaGetRenderModelCount();

bool freyjaGetRenderModel(uint32 modelIndex, RenderModel &model);

#endif
