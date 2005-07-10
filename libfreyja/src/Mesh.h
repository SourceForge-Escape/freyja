/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : FreyjaMesh
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This is the FreyjaMesh and classes that compose it.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_FREYJAMESH - Builds FreyjaMesh class as a unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.10.22:
 * Mongoose - Created, expanded from Freyja class
 ==========================================================================*/

#ifndef GUARD__FREYJA_MONGOOSE_FREYJAMESH_H_
#define GUARD__FREYJA_MONGOOSE_FREYJAMESH_H_

#include <hel/math.h>
#include <hel/Vector3d.h>
#include <mstl/Vector.h>
#include "freyja.h"


#define FreyjaMesh Mesh


namespace freyja {


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

	Vertex()
	{
		uint32 i, count;


		flags = fNone;
		mesh = INDEX_INVALID;
		mOldUID = INDEX_INVALID;

		/* Setup gobal pool UID reference */
		mUID = count = mFreePool.size();

		for (i = 0; i < count; ++i)
		{
			if (mGobalPool[i] == 0x0)
			{
				mUID = i;
				break;
			}	
		}

		mGobalPool.assign(mUID, this);
	}

	~Vertex()
	{
		/* Mark NULL in pool, marking free slot */
		mGobalPool.assign(mUID, 0x0);

		/* Notify Polygons about deleted vertex */
		freyjaPolygonVertexDeleteHandler(polygonRef, mUID);
		polygonRef.erase();

		/* Bones don't depend on vertices, so no notification needed */
		weights.erase();
	}


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	static index_t findVertexIndexByOldUID(index_t oldUID)
	{
		uint32 i, count = getCount();
		Vertex *v;

		if (oldUID == INDEX_INVALID)
			return INDEX_INVALID;

		for (i = 0; i < count; ++i)
		{
			v = getVertex(i);

			if (v && v->mOldUID == oldUID)
			{
				return i;
			}
		}
	}

	static uint32 getCount()
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns number of unique materials
	 ------------------------------------------------------*/
	{
		return mGobalPool.size();
	}

	static Vertex *getVertex(index_t uid)
	{
		if (uid >= getCount() || uid == INDEX_INVALID)
			return 0x0;

		return mGobalPool[uid];
	}

	bool serialize(FileWriter &w)
	/*------------------------------------------------------
	 * Pre  : Writes this object out to disk
	 * Post : Returns true on success
	 ------------------------------------------------------*/
	{
		uint32 i, count, size;


		/* Header */
		w.writeInt32U(mType);
		w.writeInt32U(mVersion);

		/* Size of the following data */
		size = (4 + 1 + 4 + 12 + 12 + 12 +
				4 + 8 * weights.size() + 
				4 + 4 * polygonRef.size());

		w.writeInt32U(size);
		
		/* Data */
		w.writeInt32U(mUID);

		w.writeInt8U(flags);

		w.writeInt32U(mesh);

		w.writeFloat32(xyz[0]);
		w.writeFloat32(xyz[1]);
		w.writeFloat32(xyz[2]);

		w.writeFloat32(normal[0]);
		w.writeFloat32(normal[1]);
		w.writeFloat32(normal[2]);

		w.writeFloat32(uvw[0]);
		w.writeFloat32(uvw[1]);
		w.writeFloat32(uvw[2]);

		/* Weights */
		count = weights.size();
		w.writeInt32U(count);

		for (i = 0; i < count; ++i)
		{
			if (weights[i])
			{
				w.writeInt32U(weights[i]->mBoneIndex);
				w.writeFloat32(weights[i]->mWeight);
			}
			else
			{
				w.writeInt32U(INDEX_INVALID);
				w.writeFloat32(0.0f);
			}
		}

		/* Polygon references */
		count = polygonRef.size();
		w.writeInt32U(count);

		for (i = 0; i < count; ++i)
		{
			w.writeInt32U(polygonRef[i]);
		}

		return true;
	}


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void assignToMesh(index_t meshIndex)
	{
		mesh = meshIndex;
	}

	bool serialize(FileReader &r)
	/*------------------------------------------------------
	 * Pre  : Reads this object from disk
	 * Post : Returns true on success
	 ------------------------------------------------------*/
	{
		uint32 i, count, type, version, size, uid, idx;
		vec_t weight;


		/* Header */
		//type = r.readInt32U();  // Checked and read at higher level
		//version = r.readInt32U();
		//size = r.readInt32U();

		if (version != mVersion)
			return false;

		/* Data */
		mOldUID = r.readInt32U(); // 'Old' uid
		flags = r.readInt8U();
		flags |= fOld_UIDs; // Flag that the UIDs used are old
		mesh = r.readInt32U(); // 'Old' mesh uid

		xyz[0] = r.readFloat32();
		xyz[1] = r.readFloat32();
		xyz[2] = r.readFloat32();

		normal[0] = r.readFloat32();
		normal[1] = r.readFloat32();
		normal[2] = r.readFloat32();

		uvw[0] = r.readFloat32();
		uvw[1] = r.readFloat32();
		uvw[2] = r.readFloat32();

		/* Weights */
		count = r.readInt32U();

		for (i = 0; i < count; ++i)
		{
			idx = r.readInt32U(); // 'Old' bone UID
			weight = r.readFloat32();

			if (idx != INDEX_INVALID)
			{
				weights.pushBack(new Weight(idx, weight));
			}
		}

		/* Polygon references */
		count = r.readInt32U();

		for (i = 0; i < count; ++i)
		{
			idx = r.readInt32U();

			if (idx != INDEX_INVALID)
			{		
				polygonRef.pushBack(idx); // 'Old' polygon UID
			}
		}

		return true;
	}

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

	index_t mUID;                       /* Unique identifier, key for pool */

	index_t mOldUID;                    /* UID when this was saved to disk */

	static uint32 mType;                /* Type of file chunk */

	static uint32 mVersion;             /* File chunk version */

	static Vector<Vertex *> mGobalPool; /* Storage for gobal access */

	static Vector<index_t> mFreePool;   /* Tracks unused gobal pool slots */ 
};


class Polygon
{
public:

	enum Flags {
		fOld_UIDs = 1,
		fSelected = 2,
		fReservedA = 4,
		fReservedB = 8,
		fReservedC = 16,
		fReservedD = 32,
		fReservedE = 64,
		fReservedF = 128
	};

	enum SmoothingGroupFlags {
		fGroup_1 = 1,
		fGroup_2 = 2,
		fGroup_3 = 4,
		fGroup_4 = 8,
		fGroup_5 = 16,
		fGroup_6 = 32,
		fGroup_7 = 64,
		fGroup_8 = 128,
		fGroup_9 = 256,
		fGroup_10 = 1024,
		fGroup_11 = 2048
		// ...
	};


	byte flags;                       /* Options for polygon */

	index_t mesh;                     /* Mesh owner UID */

	index_t material;                 /* Material id, if (mat != mesh.mat)
									   * to support multimaterial meshes */

	uint32 smoothingGroups;           /* BitFlag for smoothing groups use */

	Vector<index_t> vertices;         /* Vertices composing polygon */

	Vector<vec3_t *> normals;         /* Polymapped Normals (optional) */

	Vector<vec3_t *> texcoords;       /* Polymapped Texcoords (optional) */

	Vector<vec4_t *> colors;          /* Colors for polygon (optional) */


private:

	index_t UID;                      /* Unique identifier, key for pool */

	Vector<Polygon *> mGobalPool;     /* Storage for gobal access */
};


class SmoothingGroup
{
public:
	void transform();

	byte flags;

	char name[64];

	index_t material;

	Vector<index_t> polygons;      /* Contains TexCoords composing group
									 * either polymapped or by vertex */
private:

	index_t UID;                      /* Unique identifier, key for pool */

	static Vector<SmoothingGroup *> mGobalPool;    /* Storage for gobal access */
};


class UVMap
{
public:
	void transform();

	Vector<index_t> polygons;   /* Contains TexCoords composing group
								 * either polymapped or by vertex */
private:

	index_t UID;                   /* Unique identifier, key for pool */

	Vector<UVMap *> mGobalPool;    /* Storage for gobal access */
};


class VertexGroup
{
	Vector<index_t> vertices;       /* Vertices in group */

private:

	index_t UID;                         /* Unique identifier, key for pool */

	Vector<VertexGroup *> mGobalPool;    /* Storage for gobal access */
};


class VertexFrame
{
public:
	void transform();

	Vector<vec3_t*> transforms;       /* Vertex transforms (animation) */

	Vector3d bboxMin;                 /* Min corner of bounding box */

	Vector3d bboxMax;                 /* Max corner of bounding box */

	Vector3d center;                  /* Center of bounding volume */

	vec_t radius;                     /* Radius of bounding sphere if used */

private:

	index_t UID;                         /* Unique identifier, key for pool */

	Vector<VertexFrame *> mGobalPool;    /* Storage for gobal access */
};


/* Vertex no longer a primative object class/type
 * Move csg to plugin maybe child class of Mesh, CSGMesh */
class Mesh
{
 public:

	class PolygonRef
	{
	public:
		Vector<unsigned int> polygons;
	};

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Mesh();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of FreyjaMesh
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.10.22: 
	 * Mongoose - Created, from FreyjaMesh in Freyja
	 ------------------------------------------------------*/

	~Mesh();
	/*------------------------------------------------------
	 * Pre  : FreyjaMesh object is allocated
	 * Post : Deconstructs an object of FreyjaMesh
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.10.22: 
	 * Mongoose - Created, from FreyjaMesh in Freyja
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	unsigned int getVertexCount() { return vertices.end(); }

	unsigned int getPolygonCount() { return polygons.end(); }


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void rotate(vec_t x, vec_t y, vec_t z);

	void rotateAboutPoint(vec3_t point, vec_t x, vec_t y, vec_t z);

	void scale(vec_t x, vec_t y, vec_t z);

	void transform();

	void translate(vec_t x, vec_t y, vec_t z);


private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	Vector<Vertex *> vertices;          /* Vertices (complex class version) */

	Vector<Polygon *> polygons;         /* Polygons of this mesh */

	Vector<VertexFrame *> frames;       /* Vertex morph frames */

	Vector<UVMap *> uvmaps;             /* UVMaps of this mesh */

	Vector<VertexWeight *> weights;     /* Vertex weights */

	Vector<SmoothingGroup *> groups;    /* Smoothing Groups of this mesh */

	Vector3d mPosition;                 /* Position of the mesh */

	index_t mMaterial;                  /* Base material id */


	/* UID system */

	index_t UID;                        /* Unique identifier, key for pool */

	Vector<Mesh *> mGobalPool;          /* Storage for gobal access */
};

}

#endif
