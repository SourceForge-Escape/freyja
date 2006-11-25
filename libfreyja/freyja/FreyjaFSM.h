/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : FreyjaFSM
 * License : GPL, also (C) 2000 Mongoose
 * Comments: This is the Egg plugin handler class.
 *
 *-- Test Defines -------------------------------------------
 *
 * UNIT_TEST_FREYJAFSM  Builds module test code as a console program
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.12.17: (v0.0.9)
 * Mongoose -  FreyjaPlugin ABI compatible FreyjaFSM
 *
 * 2004.03.23: (v0.0.3+)
 * Mongoose - New singleton implementation
 *            New plugin features for texture loading
 *            Removed last of old List STL use
 *
 *            New style, which affects compiler portability
 *            Mostly removing leading _underscores
 *
 * 2001.11.02: (v0.0.2)
 * Mongoose - Experimental export API added 
 *
 * 2001.02.24:
 * Mongoose - Created, based on python test code
 ==========================================================================*/

#ifndef GUARD__MONGOOSE_FREYJA_FREYJAFSM_H
#define GUARD__MONGOOSE_FREYJA_FREYJAFSM_H

#include <stdarg.h>
#include <mstl/stack.h>
#include <mstl/Vector.h>
#include <hel/math.h>

#include "FreyjaPluginABI.h"
#include "FreyjaPlugin.h"
#include "Mesh.h"

using namespace mstl;

class FreyjaFSM
{
public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	virtual ~FreyjaFSM();
	/*------------------------------------------------------
	 * Pre  : FreyjaFSM object is allocated
	 * Post : Deconstructs an object of FreyjaFSM
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	uint32 freyjaGetCount(freyja_object_t type);
	/*------------------------------------------------------
	 * Pre  : Type is valid egg plugin enum
	 * Post : Returns total number of objects of type in model
	 *        Returns 0 on invalid type or 'empty state'
	 ------------------------------------------------------*/

	index_t freyjaIterator(freyja_object_t type, index_t item);
	/*------------------------------------------------------
	 * Pre  : Type is valid egg plugin enum 
	 *        Item is either LIST command or item id
	 *
	 * Post : Sets current item in internal iterator
	 *        Returns INDEX_INVALID on error
	 ------------------------------------------------------*/

	void freyjaGetVertex(vec3_t xyz);
	void freyjaGetVertexNormal(vec3_t nxyz);
	void freyjaGetVertexTexCoord(vec2_t uv);
	void freyjaGetVertexWeights(Vector<index_t> &bones,
								Vector<float> &weights);
	/*------------------------------------------------------
	 * Pre  : Gets current vertex's data 
	 * Post : Sets passed references to vertex's values
	 ------------------------------------------------------*/

	index_t freyjaGetCurrent(freyja_object_t type);
	/*------------------------------------------------------
	 * Pre  : Complex type passed
	 * Post : Returns id of current internal complex
	 *        type ( last generated )
	 *        Returns INDEX_INVALID on error
	 ------------------------------------------------------*/

	index_t freyjaGetBoneMeshIndex(index_t element);
	/*------------------------------------------------------
	 * Pre  : Bone selected
	 *        Value set to Mesh id of Bone's mesh_list[item]
	 * Post : Returns INDEX_INVALID if Bone doesn't exist
	 *        or if item is invalid
	 ------------------------------------------------------*/

	uint32 freyjaGetBoneMeshCount();
	/*------------------------------------------------------
	 * Pre  : Tag selected
	 * Post : Returns number of Meshes in this Tag
	 *        Returns PLUGIN_ERROR if Tag doesn't exist
	 ------------------------------------------------------*/

	void getBoneRotation(vec_t *x, vec_t *y, vec_t *z);
	/*------------------------------------------------------
	 * Pre  : Bone selected
	 * Post : 
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void freyjaBegin(freyja_object_t type);
	/*------------------------------------------------------
	 * Pre  : Type is valid egg plugin enum
	 * Post : A new data object had begun construction
	 *        Returns an object id
	 ------------------------------------------------------*/

	void freyjaEnd();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : A data object has finished construction
	 ------------------------------------------------------*/

	index_t freyjaTexCoordCreate2f(vec_t s, vec_t t);
	/*------------------------------------------------------
	 * Pre  : s, t are 0.0 to 1.0 texels
	 * Post : A new texel is created in the model
	 *        Returns the native index of that texel
	 ------------------------------------------------------*/

	index_t freyjaVertexCreate3f(vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : eggBegin(FREYJA_GROUP);
	 *        x,y,z are valid 3space coors
	 * Post : A new vertex created in the model
	 *        Returns the native index of that vertex
	 ------------------------------------------------------*/

	void freyjaPolygonAddVertex1i(index_t index);
	/*------------------------------------------------------
	 * Pre  : eggBegin(FREYJA_POLYGON);
	 *        Egg_id is the native index
	 * Post : Adds a vertex to a polygon
	 ------------------------------------------------------*/

	void freyjaPolygonAddTexCoord1i(index_t index);
	/*------------------------------------------------------
	 * Pre  : eggBegin(FREYJA_POLYGON);
	 *        Egg_id is the native index
	 * Post : Adds a texel to a polygon
	 ------------------------------------------------------*/

	void freyjaPolygonGroup1u(uint32 group);
	/*------------------------------------------------------
	 * Pre  : eggBegin(FREYJA_POLYGON);
	 *        Egg_id is the native index
	 * Post : Adds polygon to group
	 ------------------------------------------------------*/

	void freyjaPolygonMaterial1i(index_t index);
	/*------------------------------------------------------
	 * Pre  : eggBegin(FREYJA_POLYGON);
	 *        Egg_id is the native index
	 * Post : Adds a texture to a polygon
	 ------------------------------------------------------*/

	void freyjaGroupCenter(vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : eggBegin(FREYJA_GROUP);
	 * Post : Current group's center is set
	 ------------------------------------------------------*/

	void boneName(const char *name);
	/*------------------------------------------------------
	 * Pre  : eggBegin(FREYJA_TAG);
	 * Post : Set current bones's name
	 ------------------------------------------------------*/

	void freyjaBonePosition(vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : eggBegin(FREYJA_TAG);
	 * Post : Current tag's origin is set
	 ------------------------------------------------------*/

	void freyjaMeshFlags1u(uint32 flags);
	/*------------------------------------------------------
	 * Pre  : Pass valid egg_mesh_flags_t's bitmap
	 * Post : Sets flags for current mesh
	 ------------------------------------------------------*/

	void boneFlags(uint32 flags);
	/*------------------------------------------------------
	 * Pre  : eggBegin(FREYJA_TAG);
	 * Post : Current tag's mesh/tag tree flag is set
	 *
	 *        0x00 - push
	 *        0x01 - pop
	 ------------------------------------------------------*/

	void boneAddChild(index_t childBoneIndex);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_BONE);
	 * Post : chlidBoneIndex added to current bone's child list
	 ------------------------------------------------------*/

	void freyjaBoneRotate(vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_BONE);
	 * Post : Sets tag rotation with eular angles
	 ------------------------------------------------------*/

	void skeletonAddBone(index_t boneIndex);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_SKELETON);
	 * Post : Appends tag to mesh tree frame
	 ------------------------------------------------------*/

	void freyjaSkelAnimAppendKeyFrame(index_t keyFrameIndex);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_SKEL_ANIM);
	 * Post : Appends keyframe to skeletal animation
	 ------------------------------------------------------*/

	static FreyjaFSM *GetInstance()
	{
		if ( mInstance == NULL )
		{
			mInstance = new FreyjaFSM();
		}

		return mInstance;
	}

protected:

	FreyjaFSM();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of FreyjaFSM
	 ------------------------------------------------------*/

	static FreyjaFSM *mInstance; /* Singleton for reals -- put this off
								  * until some user plugins where trying 
								  * to use it! */

private:

	FreyjaFSM(const FreyjaFSM &fsm);

	FreyjaFSM &operator=(const FreyjaFSM &fsm);


	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	stack<freyja_object_t> mStack;      /* Object stack to keep up with 
										 * accumulation modes and etc */

	Vector<index_t> mVertexList;        /* Current polygon's vertex list */

	Vector<index_t> mTexCoordList;      /* Current polygon's texel list */

	freyja::Mesh *mMesh;                /* Mesh outside Scene for FSM use */

	index_t mTextureId;                 /* Texture id for current polygon */

	index_t mGroupId;                   /* Texture id for current polygon */

	index_t mIndexModel;

	index_t mIndexVertex;               /* Indices controlled by fsm */

	index_t mIndexTexCoord;

	index_t mIndexPolygon;

	index_t mIndexGroup;

	index_t mIndexMesh;

	index_t mIndexBone;

	index_t mIndexSkeleton;

	index_t mIndexSkeletonAnim;

	index_t mIndexSkelKeyframe;
};

#endif


