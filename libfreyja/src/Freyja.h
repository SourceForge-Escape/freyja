/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : FreyjaScene
 * License : GPL, also (C) 2000-2004 Mongoose
 * Comments: These classes compose the scene data model. 
 *
 * 
 *-- Test Defines ----------------------------------------------------
 *           
 * UNIT_TEST_FREYJA  - Builds module as test code as a console program
 *
 *-- History --------------------------------------------------------- 
 *
 * 2004.10.19:
 * Mongoose - libfreyja v9 -- new ABI and features
 *
 * 2004.08.12:
 * Mongoose - Eggv9 implementation hammered out, now I have some free time
 *
 * 2004.04.08:
 * Mongoose - All new API using Hel math/physics lib, endian safe
 *            file I/O, and generic methods.
 *
 *            Metadata chunk/type back
 *
 *            Removed a ton of code.
 *
 *            Renamed texels to texcoords, since they haven't been
 *            texels for several years now.
 *
 * 2004.03.15:
 * Mongoose - Replaced List collection use with Vector
 *            Some coding style changes to remove mismatch
 *            This is a bigger deal than it seems  =)
 *
 * 2002.08.31:
 * Mongoose - Introducing simple vectors to replace Lists and
 *            the Chain prototype collections.
 *
 *            Should reduce memory requirements and improve speed
 *
 * 2002.07.05:
 * Mongoose - Refactoring and rewritten to conform to new code style
 *
 *            The reason I do this IS to break the API for testing
 *            if you don't like it make an adpater
 *
 *            Removed marker system ( No longer used by anyone? )
 *
 * 2001.07.05:
 * Mongoose - New mtk3d API and other major changes started
 *
 * 2001-04-14:
 * Mongoose - Finishing rollover from EggV7 to EggV8 
 *
 * 2001-01-31:
 * Mongoose - Composite Eggv* starts, using pieces of 
 *            tracers ( prototypes ) and reformed to the
 *            EggV8 001 (20010101) spec.  
 *
 *            Also new major egg releases include major number 
 *            in classname, to ease transitions of other projects.
 *
 *            Removal of internal shaders, just id refernce now.
 *
 *            New focus on documentation, to aid other projects
 *            using this module.
 *
 * 2000-12-09:
 * Mongoose - Move to new data structures
 *
 * 2000-11-20:
 * Mongoose - Implementation of v8 design ( Freyja backend )
 *
 * 2000-10-16:
 * Mongoose - Documentation addtion and interface changes
 *
 * 2000-10-07:
 * Mongoose - Frame, mesh, and interface changes
 *
 * 2000-09-15:
 * Mongoose - API change!  Transforms have 1 new API call
 *            The 3 old methods per mesh, frame, list are gone
 ===========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_FREYJA_H_
#define GUARD__FREYJA_MONGOOSE_FREYJA_H_

#include <stdio.h>

#include <hel/math.h>
#include <hel/Vector3d.h>
#include <hel/Matrix.h>
#include <hel/Quaternion.h>

#include <mstl/Vector.h>
#include <mstl/Map.h>

#include "FreyjaCamera.h"
#include "FreyjaLight.h"
#include "FreyjaMesh.h"
#include "FreyjaSkeleton.h"
#include "FreyjaMaterial.h"


class FreyjaMetaData /* For storing ASCII files as strings, textures, etc */
{
public:

	FreyjaMetaData()
	{
		id = -1;
		mBoneIndex = -1;
		mSymbol[0] = 0;
		mDescription[0] = 0;
		mTypeId = 0;
		mDataSize = 0;
		mData = 0x0;
	}

	~FreyjaMetaData()
	{
		if (mData)
			delete mData;
	}

	

	long id;                  /* Unique identifier */

	char mSymbol[64];

	char mDescription[64];

	long mBoneIndex;         /* If 'tagged' to a bone this is 
								the id or it's ( -1 ) */

	long mTypeId;

	unsigned long mDataSize;

	unsigned char *mData;
};


class FreyjaAnimation
{
public:

	void setName(const char *name)
	{
		strncpy(mName, name, 64);
		mName[63] = 0;
	}
	
	long mId;

	char mName[64];

	vec_t mFrameRate;

	vec_t mTime;

	long mStartBone;          /* For animation blending (subsets) use */

	long mBoneCount;

	long mCurrentFrame; // render use mostly

	long mLastFrame;    // render use mostly

	vec_t mLastTime;    // render use mostly

	Vector<FreyjaKeyFrame *> mKeyFrames;  // keyCount / mBoneCount = frames
};


class FreyjaModel
{
public:

	unsigned int getAnimationCount() { return animations.end(); }
	unsigned int getSkeletonCount() { return skeletons.end(); }
	unsigned int getMeshCount() { return meshes.end(); }
	unsigned int getMetaDataCount() { return metadata.end(); }

	int getIndex();

	Vector<unsigned int> animations;  /* Animation data */

	Vector<unsigned int> skeletons;   /* Skeletal data for this model */

	Vector<unsigned int> meshes;      /* Geometery structure */

	Vector<unsigned int> metadata;    /* Metadata for external use */

	int id;                           /* Unique identifier */
};


class FreyjaSceneGraph
{
public:

	class FreyjaSceneGraphNode;

	FreyjaSceneGraphNode *mRoot; // Thinking about doing a DAG... hhmmm

protected:

	class FreyjaSceneGraphNode
	{
	public:

		Vector<FreyjaSceneGraphNode *> mChildren;

		Vector<long> mModels;
	};
};


class FreyjaScene
{
public:

#ifdef COMMENT
	typedef enum {
		VERTEX     = 0x54524556,
		TEXCOORD   = 0x54455843,
		NORMAL     = 0x4D524F4E,
		WEIGHT     = 0x54484757,
		POLYGON    = 0x504F4C59,
		GROUP      = 0x56475250,
		MESH       = 0x4D455348,
		SKELETON   = 0x534B454C,
		BONE       = 0x424F4E45,
		ANIMATION  = 0x414E494D,
		METADATA   = 0x4D455441
	} freyja_object_t;


	typedef enum {
		
		SCALE               = 0, 
		ROTATE              = 1, 
		TRANSLATE           = 2, 
		ROTATE_ABOUT_POINT  = 3,
		SCALE_ABOUT_POINT   = 4
	} freyja_transform_t;
	

	typedef enum {
		UNION = 1,
        INTERSECTION,
		DIFFERENCE
	} freyja_csg_t;
#endif


	/* Mongoose 2004.11.27, 
	 * This needs a tree structure and data pad of models per branch */
	class SceneGraph
	{
	public:
		unsigned int parent;
		Vector<unsigned int> children;
	};


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	FreyjaScene();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an Egg object
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~FreyjaScene();
	/*------------------------------------------------------
	 * Pre  : This object is allocated
	 * Post : Deconstructs the Egg object
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	
	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	unsigned int getMeshCount();
	unsigned int getSkeletonCount();
	unsigned int getAnimationCount();
	unsigned int getModelCount();
	unsigned int getMaterialCount();
	unsigned int getTextureCount();
	unsigned int getVertexGroupCount();


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	bool exportScene(const char *filename);

	void translate(vec_t x, vec_t y, vec_t z);

	void erase();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post :  All data is reset and cleared
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.08.01:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setDebugLevel(unsigned int level);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Debugging is set 0 ( Off ) to 5 ( Highest )
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.07.05:
	 * Mongoose - Now supports multiple levels 
	 *
	 * 2001.01.31: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

#ifdef FIXME

	unsigned int csg(egg_type_t type, egg_csg_t operation,
					 unsigned int a, unsigned int b);
	/*------------------------------------------------------
	 * Pre  : Don't count on more than simple vertex culling now
	 *
	 * Post : OPERATION on TYPE object A and B in model
	 *
	 *        A and B are perserved, creates new object C 
	 *        where: C = A OPERATION B
	 *
	 *        UNION        : C = A u B
	 *                       if A eq B then copy of A is made
	 *
	 *        INTERSECTION : C = A n B
	 *                       if A eq B then copy of A is made
	 *
	 *        DIFFERENCE   : C = A - B
	 *                       B culls A, order matters
	 *                       if A eq B then undefined behavior
	 *
	 *        Returns C's index
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.04.08:
	 * Mongoose - Created with new generic API based on mtk
	 ------------------------------------------------------*/

	void remove(egg_type_t type, unsigned int index);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Remove TYPE object[index] from model
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.04.08:
	 * Mongoose - New generic API that supports all types 
	 *            in one method
	 *
	 * 2000.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void transform(egg_type_t type, egg_transform_t transform, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.04.08:
	 * Mongoose - New generic API that supports all types 
	 *            in one method
	 *
	 * 2000.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/
#endif

	SceneGraph mModelGraph;

	Vector<FreyjaModel *> models;         /* Model container */

	Vector<FreyjaMaterial *> materials;   /* Material container */

	Vector<FreyjaTexture *> textures;     /* Texture container */

	Vector<FreyjaAnimation *> animations; /* Animation container */

	Vector<FreyjaSkeleton*> skeletons;    /* Skeletal data for this model */

	Vector<FreyjaMesh *> meshes;          /* Geometery structure */

	Vector<FreyjaMetaData *> metadata;    /* Metadata for external use */

	Vector<FreyjaLight *> lights;

	Vector<FreyjaCamera *> cameras;

	Vector<FreyjaVertexGroup *> vertexgroups;


private:
	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	bool isDebugLevel(unsigned int level);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if debug level is greater or equal
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.05.06:
	 * Mongoose - Created, split from printDebug
	 ------------------------------------------------------*/

	unsigned int mDebugLevel;              /* Set debug output at runtime */
};

#endif
