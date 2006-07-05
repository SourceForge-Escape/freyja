/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : CopyModel
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This holds all the 'copy and paste' classes
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_COPYMODEL - Builds CopyModel class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2005.05.06:
 * Mongoose - Created, Moved from FreyjaModel for better abstraction
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_COPYMODEL_H_
#define GUARD__FREYJA_MONGOOSE_COPYMODEL_H_

#include <hel/math.h>
#include <mstl/Vector.h>

using namespace mstl;

class CopyGroup
{
public:
	Vector <unsigned int> vertices;     /* Vertices composing group */	
	vec3_t center;
};


// FIXME: dumps vertex weights and for now
// Polygon, etc references are never copied -- since objects will be duped
class CopyVertex
{
public:
	vec3_t pos;
	vec3_t uv;       /* Remember only uv is used in this build */
	vec3_t norm;
	// vec4_t color;  // In this build color is func of polymapped face
};


class CopyTexCoord
{
public:
	vec2_t uv;
};


class CopyPolygon
{
public:
	Vector <unsigned int> vertices;
	Vector <unsigned int> texcoords; // this build also uses this for color
	int material;
};


// FIXME: Drops any vertex animation frames
class CopyMesh
{
public:

	CopyMesh() :
		vertices(),
		texcoords(),
		polygons(),
		groups(),
		flags(0)
	{
		center[0] = center[1] = center[2] = 0.0f;
	}


	~CopyMesh()
	{
		erase();
	}

	void erase()
	{
		flags = 0;
		vertices.erase();
		texcoords.erase();
		polygons.erase();
		groups.erase();
	}

	Vector <CopyVertex *> vertices;
	Vector <CopyTexCoord *> texcoords;
	Vector <CopyPolygon *> polygons;
	Vector <CopyGroup *> groups;
	vec3_t center;
	unsigned int flags;
};


class CopyModel
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

#ifdef USING_EGG
	CopyModel(Egg *egg);
#else
	CopyModel();
#endif
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of CopyModel
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~CopyModel();
	/*------------------------------------------------------
	 * Pre  : CopyModel object is allocated
	 * Post : Deconstructs an object of CopyModel
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	bool getAppendMeshMode();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/



	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	bool copyMesh(int currentMesh, int currentGroup);
	/*------------------------------------------------------
	 * Pre  : Makes a copy of given mesh with given vertex frame
	 * Post : Returns true if sucessful
	 *
	 ------------------------------------------------------*/

	bool copyVertexBuffer(Vector<unsigned int> &list, 
						  int currentMesh, int currentGroup);
	/*------------------------------------------------------
	 * Pre  : Makes a copy of given partial mesh with given vertex frame
	 * Post : Returns true if sucessful
	 *
	 ------------------------------------------------------*/

	bool pasteMesh();
	/*------------------------------------------------------
	 * Pre  : Adds a 'copied' mesh to freyja world
	 * Post : Returns true if sucessful
	 *
	 ------------------------------------------------------*/

	void setAppendMeshMode(bool on);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

#ifdef USING_EGG
	Egg *mEgg;                      /* Temp backend still used in transition */
#endif

	bool mAppendMode;               /* Copy system multiobject copy state */

	CopyMesh mCopyMesh;             /* This is the fucking buffer for meshes
									 * it handles things the backend should
									 * but the backend can't in this build for
									 * copy/cut/paste */
};

#endif
