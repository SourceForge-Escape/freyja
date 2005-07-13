/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : Vertex
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: Vertex class, split from dancer prototype Mesh class.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_VERTEX - Builds Vertex class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2005.07.11:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_VERTEX_H_
#define GUARD__FREYJA_MONGOOSE_VERTEX_H_

#include <mstl/Vector.h>
#include <hel/math.h>
#include <hel/Vector3d.h>
#include "freyja.h"
#include "FileReader.h"
#include "FileWriter.h"


namespace freyja {

void freyjaVertexListTransform(Vector<index_t> &list,
							   freyja_transform_action_t action, 
							   vec_t x, vec_t y, vec_t z);

void freyjaPolygonVertexDeleteHandler(Vector<index_t> polygons, index_t vertex);


class Weight
{
public:

	Weight(index_t bone, vec_t weight)
	{
		mBoneIndex = bone;
		mWeight = weight;
	}

	~Weight();

	vec_t mWeight;             /* Weight for vertex use */

	index_t mBoneIndex;        /* Bone id */
};


class Vertex
{
 public:

	enum Flags {
		fNone = 0,
		fOld_UIDs = 1,
		fSelected = 2,
		fReservedA = 4,
		fReservedB = 8,
		fReservedC = 16,
		fReservedD = 32,
		fReservedE = 64,
		fReservedF = 128
	};


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Vertex();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Vertex
	 ------------------------------------------------------*/

	~Vertex();
	/*------------------------------------------------------
	 * Pre  : Vertex object is allocated
	 * Post : Deconstructs an object of Vertex
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	static index_t findVertexIndexByOldUID(index_t oldUID);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	static uint32 getCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns number of unique solts in gobal vector
	 ------------------------------------------------------*/

	uint32 getSerializedSize();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Size of this class in bytes if serialized 
	 ------------------------------------------------------*/

	uint32 getChunkType();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	static Vertex *getVertex(index_t uid);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	index_t getUID();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	bool serialize(FileWriter &w);
	/*------------------------------------------------------
	 * Pre  : Writes this object out to disk
	 * Post : Returns true on success
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void assignToMesh(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	static index_t combine(index_t vertexIndexA, index_t vertexIndexB);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	bool serialize(FileReader &r);
	/*------------------------------------------------------
	 * Pre  : Reads this object from disk
	 * Post : Returns true on success
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	byte flags;                    /* Various options stored in bitflag */

	index_t mesh;                  /* Unique identifier for parent mesh */

	vec3_t xyz;                    /* Position in 3 space */

	vec3_t normal;                 /* Normal vector */

	vec3_t uvw;                    /* Texture coordinates */

	//vec4_t color;                /* Material property now */

	//vec4_t specular;             /* Material property now */

	Vector<Weight *> weights;      /* Vector of weights */

	Vector<index_t> polygonRef;    /* UIDs of polygons referencing vertex */


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////


	index_t mUID;                       /* Unique identifier, key for pool */

	index_t mOldUID;                    /* UID when this was saved to disk */

	static uint32 mType;                /* Type of file chunk */

	static uint32 mVersion;             /* File chunk version */

	static Vector<Vertex *> mGobalPool; /* Storage for gobal access */

	static Vector<index_t> mFreePool;   /* Tracks unused gobal pool slots */
};

}
#endif
