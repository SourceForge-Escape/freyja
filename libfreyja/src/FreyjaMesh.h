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


#ifdef POST_CHIMERA_PLUS_UMBRA
class FreyjaVertex
{
public:

	class Weight
	{
	public:
		Weight(index_t bone, vec_t weight)
		{
			mBoneIndex = bone;
			mWeight = weight;
		}

		vec_t mWeight;             /* Weight for vertex use */
		index_t mBoneIndex;             /* Bone id */
	};


	FreyjaVertex()
	{
		mIndex = mGobalMap.size();
		mGobalMap.pushBack(this);
	}

	~FreyjaVertex()
	{
		weights.erase();
		frames.erase();
		polygonRef.erase();
	}

	unsigned char flags;
	index_t mesh, id;              /* Unique identifier, mesh[i].vertices[j] */
	vec3_t xyz;                    /* Position in 3 space */
	vec3_t normal;                 /* Normal vector */
	vec3_t uvw;                    /* Texture coordinates */

	//vec4_t color, specular;

	Vector<Weight *> weights;      /* Vector of weights */

	Vector <vec3_t *> frames; 

	Vector <index_t> polygonRef;   /* Ids of polygons referencing vertex */

private:
	long mIndex;
	static Vector<FreyjaVertex *> mGobalMap;
};

class FreyjaPolygon
{
public:

	unsigned long flags;

	index_t mesh, id;                 /* Unique identifier */

	index_t material;                 /* Material id, if (mat != mesh.mat)
									   * to support multimaterial meshes */

	Vector <index_t> vertices;        /* Vertices composing polygon */

	Vector <vec3_t *> texcoords;      /* Polymapped Texcoords (optional) */

	Vector <vec4_t *> colors;         /* Colors for polygon (optional) */
};
#endif


class FreyjaWeight
{
public:
	FreyjaWeight(unsigned int bone, vec_t weight)
	{
		mBone = bone;
		mWeight = weight;
	}

	vec_t mWeight;                     /* Weight for vertex use */
	unsigned int mBone;                /* Bone id */
};


class FreyjaVertexWeight
{
public:

	Vector <FreyjaWeight *> mWeights;
};


class FreyjaPolygon
{
public:

	unsigned int flags;

	unsigned int id;                  /* Unique identifier */

	int material;                     /* Material id, if (mat != mesh.mat)
									   * to support multimaterial meshes */

	Vector <unsigned int> vertices;   /* Vertices composing polygon */

	Vector <unsigned int> texcoords;  /* Texcoords for polygon (polymapped) */
};


class FreyjaUVMap
{
public:
	void transform();

	Vector <unsigned int> polygons;   /* Contains TexCoords composing group
									   * either polymapped or by vertex */
};


class FreyjaVertexGroup
{
public:
	unsigned int id;

	unsigned int mesh;                /* Mesh that owns vertices in group */

	Vector<unsigned int> vertices;    /* Vertices in group */
	
	unsigned int childGroup;          /* For linking multimesh groups */
};


class FreyjaVertexFrame
{
public:
	void transform();

	unsigned int id;

	Vector<vec_t> vertices;           /* Vertex animation frame */

	Vector3d bboxMin;                 /* Min corner of bounding box */
	Vector3d bboxMax;                 /* Max corner of bounding box */

	Vector3d center;                  /* Center of bounding volume */
	vec_t radius;                     /* Radius of bounding sphere if used */
};


/* Vertex no longer a primative object class/type
 * Move csg to plugin maybe child class of Mesh, CSGMesh */
class FreyjaMesh
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

	FreyjaMesh();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of FreyjaMesh
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.10.22: 
	 * Mongoose - Created, from FreyjaMesh in Freyja
	 ------------------------------------------------------*/

	~FreyjaMesh();
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

	unsigned int getVertexCount() { return vertices.end()/3; }
	unsigned int getTexCoordCount() { return texcoords.end()/2; }
	unsigned int getNormalCount() { return normals.end()/3; }
	unsigned int getVertexWeightCount() { return weights.end(); }
	unsigned int getPolygonCount() { return polygons.end(); }
	unsigned int getVertexFrameCount() { return frames.end(); }

	void getNormal(unsigned int index, vec3_t xyz)
	{
		if (index*3 > normals.end() || !getNormalCount())
			return;

		vec_t *array = normals.getVectorArray();

		xyz[0] = array[index/3];
		xyz[1] = array[index/3+1];
		xyz[2] = array[index/3+2];
	}

	void getTexCoord(unsigned int index, vec2_t uv)
	{
		if (index*2 > texcoords.end() || !getTexCoordCount())
			return;

		vec_t *array = texcoords.getVectorArray();

		uv[0] = array[index/2];
		uv[1] = array[index/2+1];
	}

	void getVertex(unsigned int index, vec3_t xyz)
	{
		if (index*3 > vertices.end() || !getVertexCount())
			return;

		vec_t *array = vertices.getVectorArray();

		xyz[0] = array[index/3];
		xyz[1] = array[index/3+1];
		xyz[2] = array[index/3+2];
	}


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	unsigned int addNormal(vec3_t xyz)
	{
		normals.pushBack(xyz[0]);
		normals.pushBack(xyz[1]);
		normals.pushBack(xyz[2]);
		return (normals.end() / 3) - 1;
	}

	unsigned int addTexCoord(vec2_t uv)
	{
		texcoords.pushBack(uv[0]);
		texcoords.pushBack(uv[1]);
		return (texcoords.end() / 2) - 1;
	}

	unsigned int addVertex(vec3_t xyz)
	{
		vertices.pushBack(xyz[0]);
		vertices.pushBack(xyz[1]);
		vertices.pushBack(xyz[2]);

		weights.pushBack(new FreyjaVertexWeight());
		refPolygons.pushBack(new PolygonRef());

		return (vertices.end() / 3) - 1;
	}

	int addVertexWeight(unsigned int index, 
						vec_t weight, unsigned int bone)
	{
		if (index*3 > vertices.end() || !weights[index])
			return -1;

		unsigned int i;

		/* Here index corresponds to vertex index and it's assc weight vector */
		for (i = weights[index]->mWeights.begin(); 
			 i < weights[index]->mWeights.end(); ++i)
		{
			if (weights[index]->mWeights[i]->mBone == bone)
			{
				weights[index]->mWeights[i]->mWeight = weight;
				return i;
			}
		}

		weights[index]->mWeights.pushBack(new FreyjaWeight(bone, weight));

		return (weights.end() - 1);
	}

	void setNormal(unsigned int index, vec3_t xyz)
	{
		if (index*3 > normals.end())
			return;

		vec_t *array = normals.getVectorArray();

		array[index/3] = xyz[0];
		array[index/3+1] = xyz[1];
		array[index/3+2] = xyz[2];
	}

	void setTexCoord(unsigned int index, vec2_t uv)
	{
		if (index*2 > texcoords.end())
			return;

		vec_t *array = texcoords.getVectorArray();

		array[index/2] = uv[0];
		array[index/2+1] = uv[1];
	}

	void setVertex(unsigned int index, vec3_t xyz)
	{
		if (index*3 > vertices.end())
			return;

		vec_t *array = vertices.getVectorArray();

		array[index/3] = xyz[0];
		array[index/3+1] = xyz[1];
		array[index/3+2] = xyz[2];
	}

	void rotate(vec_t x, vec_t y, vec_t z);

	void rotateAboutPoint(vec3_t point, vec_t x, vec_t y, vec_t z);

	void scale(vec_t x, vec_t y, vec_t z);

	void transform();

	void translate(vec_t x, vec_t y, vec_t z);

	bool combineTexcoords(unsigned int a, unsigned int b)
	{
		FreyjaPolygon *polygon;
		unsigned int i;


		// Make all polygons referencing A point to B
		for (i = polygons.begin(); i < polygons.end(); ++i)
		{
			polygon = polygons[i];

			if (polygon && polygon->texcoords[i] == a)
				polygon->texcoords.assign(i, b);
		}

		// Mark A as unused in the texcoord array reference
		//usedTexCoord.pushBack(a);

		return true;
	}

	bool combineVertices(unsigned int a, unsigned int b)
	{
		FreyjaPolygon *polygon;
		unsigned int i;


		// Make all polygons referencing A point to B
		for (i = polygons.begin(); i < polygons.end(); ++i)
		{
			polygon = polygons[i];

			if (polygon && polygon->vertices[i] == a)
				polygon->vertices.assign(i, b);
		}

		// Mark A as unused in the texcoord array reference
		//usedVertex.pushBack(a);

		// Don't bother touching weights, they aren't managed

		return true;
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Combines object A and B in model
	 *
	 *        Destroys A and B then replaces them with new 
	 *        object with index A where: A = A + B
	 *
	 *        Returns true on sucess
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.05.04:
	 * Mongoose - Hard ABI back
	 *
	 * 2004.04.08:
	 * Mongoose - New generic API that supports all types 
	 *            in one method
	 *
	 * 2000.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	FreyjaMesh *csgUnion(FreyjaMesh *a, FreyjaMesh *b);
	FreyjaMesh *csgIntersection(FreyjaMesh *a, FreyjaMesh *b);
	FreyjaMesh *csgDifference(FreyjaMesh *a, FreyjaMesh *b);
	//unsigned int csg(egg_type_t type, egg_csg_t operation,
	//				 unsigned int a, unsigned int b);
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

	// FIXME: temp fix for testing
	// private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	Vector<FreyjaPolygon *> polygons;         /* Polygons of this mesh */

	Vector<FreyjaVertexFrame *> frames;       /* Vertex morph frames */

	Vector<FreyjaUVMap *> uvmaps;             /* UVMaps of this mesh */

	Vector<FreyjaVertexWeight *> weights;     /* Vertex weights */

	Vector <unsigned int> groups;       /* Vertex groups of this mesh */

	Vector <unsigned int> refVertices;  /* Tracks unused vertex pool slots */ 

	Vector<PolygonRef *> refPolygons;   /* Matches vertex list, shows
										 * which polygons ref which vertices */

	Vector<vec_t> vertices;             /* Vertex array, could be replaced 
										 * with VertexFrame's vertices */

	Vector<vec_t> texcoords;            /* Texcoord array */

	Vector<vec_t> normals;              /* Normal array */

	vec3_t position;                    /* Position of the mesh */

	unsigned int id;                    /* Unique identifier */

	unsigned int texcoordDepth;         /* 1d, 2d, 3d */

	int material;                       /* Base material id */

	unsigned int currentFrame;          /* Current vertex morph frame */
};

#endif
