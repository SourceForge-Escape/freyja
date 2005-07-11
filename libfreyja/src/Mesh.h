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
#include "Vertex.h"


#define FreyjaMesh Mesh


namespace freyja {


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

	Vector<index_t> vertices;           /* Vertices (complex class version) */

	Vector<index_t> polygons;           /* Polygons of this mesh */

	Vector<index_t> frames;             /* Vertex morph frames */

	Vector<index_t> uvmaps;             /* UVMaps of this mesh */

	Vector<index_t> groups;             /* Smoothing Groups of this mesh */

	Vector3d mPosition;                 /* Position of the mesh */

	index_t mMaterial;                  /* Base material id */


	/* UID system */

	index_t UID;                        /* Unique identifier, key for pool */

	Vector<Mesh *> mGobalPool;          /* Storage for gobal access */
};

}

#endif
