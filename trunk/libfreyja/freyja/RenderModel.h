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

#ifdef USING_EGG
#   include "Egg.h"
#endif

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

	Vector3d getGroupCenter(unsigned int i)
	{
		Vector3d v;
#ifdef USING_EGG
		egg_group_t *grp;

		if ((grp = mEgg->getGroup(mMesh->group[i])))
		{
			return Vector3d(grp->center);
		}
#else
		vec3_t center;
		//FIXME: This is just a 0.9.3 placeholder that's fine for now
		freyjaGetMeshFrameCenter(id, 0, center);
		v.Set(center);
		return v;
#endif
		v.zero();
		return v;
	}


#ifdef USING_EGG
	void setEgg(Egg *egg, egg_mesh_t *mesh, Vector<egg_polygon_t *> *polygons)
	{
		mEgg = egg;
		mMesh = mesh;
		mPolygons = polygons;
	}
#endif


	bool getPolygon(unsigned int index, RenderPolygon &face);


	bool getPolygon(unsigned int findex, long frame, RenderPolygon &face)
	{
#ifdef USING_EGG
		static egg_polygon_t *poly;

		if (mPolygons)
		{
			poly = (*mPolygons)[index];
			
			if (poly)
			{
				return RenderMesh::createRenderPolygon(face, *poly, (int32)frame);
			}
		}
#else
		getPolygon(findex, face);
		//BUG_ME("getPolygon Not Implemented", __FILE__, __LINE__);
#endif

		return false;
	}

	unsigned int getPolygonCount()
	{
#ifdef USING_EGG
		if (mPolygons->empty()) 
			return 0;

		return mPolygons->end();
#else
		return freyjaGetMeshPolygonCount(id);
#endif
	}

#ifdef USING_EGG
	static bool createRenderPolygon(RenderPolygon &face,
									egg_polygon_t &polygon, int32 frame);
#endif
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Egg realtime data translator method
	 *        Gets polygon for given egg polygon, vertex animation frame
	 ------------------------------------------------------*/



	index_t id;
	long frame;
	uint32 count;
	unsigned int gbegin, gend;

private:
#ifdef USING_EGG
	Egg *mEgg; // For sheilding renderer from egg calls for groups in mesh
	Vector<egg_polygon_t *> *mPolygons;
	egg_mesh_t *mMesh;
#endif
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
	
#ifdef USING_EGG
	void createRenderMesh(RenderMesh &rmesh, egg_mesh_t &mesh, int32 frame);
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Egg realtime data translator method
	 *
	 ------------------------------------------------------*/
	
	void setEgg(Egg *egg);
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Egg backend is set
	 *
	 ------------------------------------------------------*/

	Vector<egg_mesh_t *> *mMeshlist;

	Egg *mEgg;
#else
	void createRenderMesh(RenderMesh &rmesh, index_t meshIndex, int32 frame);
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Egg realtime data translator method
	 *
	 ------------------------------------------------------*/

#endif

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
