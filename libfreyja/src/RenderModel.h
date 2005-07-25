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


/* These classes sheild FreyjaRender from Egg use for polygons, meshes, etc. 
 * They only use all natural, wholesome Hel math types. */

class RenderBone
{
public:

	uint32 getChildrenCount()
	{
		return 0;
		//FIXMEreturn mTag->slave.end();
	}

	index_t getBoneIndex(uint32 index)
	{
		return 0;
		//FIXME return mTag->slave[index];
	}

#ifdef FIXME
	void set(egg_tag_t *tag)
	{
		mTag = tag;
		translate = Vector3d(tag->center);
		rotate = Vector3d(tag->rot);
	}
#endif

	Vector3d translate;
	Vector3d rotate;

private:
	//FIXME egg_tag_t *mTag;
};


class RenderSkeleton
{
public:

	uint32 getBoneCount()
	{
		//FIXME:return mTags->end();
		return 0;
	}

	bool getBone(index_t boneIndex, RenderBone &bone)
	{
		//FIXME: if ((*mTags)[index])
		//FIXME: {
		//FIXME: bone.set((*mTags)[index]);
		//FIXME: 	return true;
		//FIXME:}

		return false;
	}

	//FIXME
	//void setTags(Vector<egg_tag_t *> *tags)
	//{
	//	mTags = tags;
	//}

private:
	//FIXME: Vector<egg_tag_t *> *mTags;
};


typedef struct 
{
	vec3_t vertices[4];
	vec3_t texcoords[4];
	vec3_t normals[4];
	vec4_t colors[4];
	index_t material;

} freyja_quad_t;


typedef struct 
{
	vec3_t vertices[3];
	vec3_t texcoords[3];
	vec3_t normals[3];
	vec4_t colors[3];
	index_t material;

} freyja_triangle_t;


class RenderStrip
{
	vec3_t *vertices;
	uint32 count;

	vec3_t *texcoords;
	vec3_t *normals;
	vec4_t *colors;
	index_t material;
};

class RenderPolygon
{ 
public:
       Vector3d vertices[6];
        Vector3d texcoords[6];
        Vector3d normals[6];
        vec4_t colors[6];
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
		//	egg_group_t *grp;
	// FIXME
	//	if ((grp = mEgg->getGroup(mMesh->group[i])))
	//	{
	//		return Vector3d(grp->center);
	//	}

		v.zero();
		return v;
	}

	// FIXME
	//void setEgg(Egg *egg, egg_mesh_t *mesh, Vector<egg_polygon_t *> *polygons)
	//{
	//	mEgg = egg;
	//	mMesh = mesh;
	//	mPolygons = polygons;
	//}


	bool getPolygon(index_t index, RenderPolygon &face)
	{
		//static egg_polygon_t *poly;

// 		if (mPolygons)
// 		{
// 			poly = (*mPolygons)[index];
			
// 			if (poly)
// 			{
// 				return RenderMesh::createRenderPolygon(face, *poly, (int32)-1);
// 			}
// 		}

		return false;
	}


	bool getPolygon(unsigned int index, long frame, RenderPolygon &face)
	{
// 		static egg_polygon_t *poly;

// 		if (mPolygons)
// 		{
// 			poly = (*mPolygons)[index];
			
// 			if (poly)
// 			{
// 				return RenderMesh::createRenderPolygon(face, *poly, (int32)frame);
// 			}
// 		}

		return false;
	}

	unsigned int getPolygonCount()
	{
// 		if (mPolygons->empty()) 
			return 0;

// 		return mPolygons->end();
	}

// 	static bool createRenderPolygon(RenderPolygon &face,
// 									egg_polygon_t &polygon, int32 frame);
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Egg realtime data translator method
	 *        Gets polygon for given egg polygon, vertex animation frame
	 ------------------------------------------------------*/



	long id;
	long frame;
	unsigned int gbegin, gend;

private:
// 	Egg *mEgg; // For sheilding renderer from egg calls for groups in mesh
// 	Vector<egg_polygon_t *> *mPolygons;
// 	egg_mesh_t *mMesh;
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
	
	//void createRenderMesh(RenderMesh &rmesh, egg_mesh_t &mesh, int32 frame);
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Egg realtime data translator method
	 *
	 ------------------------------------------------------*/
	
// 	void setEgg(Egg *egg);
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Egg backend is set
	 *
	 ------------------------------------------------------*/

	int32 mIndex;

// 	Vector<egg_mesh_t *> *mMeshlist;

// 	Egg *mEgg;



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
