/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : FreyjaSkeleton
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This is the FreyjaSkeleton class and the classes that compose it.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_FREYJASKELETON - Builds FreyjaSkeleton class as a unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.10.22:
 * Mongoose - Created, expanded from Freyja class
 ==========================================================================*/

#ifndef GUARD__FREYJA_MONGOOSE_FREYJASKELETON_H_
#define GUARD__FREYJA_MONGOOSE_FREYJASKELETON_H_

#include <hel/math.h>
#include <hel/Vector3d.h>
#include <hel/Matrix.h>
#include <hel/Quaternion.h>

#include <mstl/Vector.h>


class FreyjaBone 
{
public:
	void translate(vec3_t xyz);
	void rotate(vec3_t xyz);

	void setName(const char *name);


	unsigned int id;                 /* Unique identifier */

	unsigned int flags;              /* For special external use, may remove */

	char name[64];                   /* Human readable identifier */

	int parent;                      /* Parent of this bone */

	Vector <unsigned int> children;  /* Children bones */

	Vector <unsigned int> meshes;    /* Meshes are bound to bone if meshtree */

	Quaternion rotation;             /* Cached for editing use */
	vec3_t position;

	Matrix matrix;                   /* Cached matrix for mesh deform use */
	Matrix transform;                /* Transform mesh/children */
};



class FreyjaSkeleton
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	FreyjaSkeleton();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of FreyjaSkeleton
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.10.22: 
	 * Mongoose - Created, from FreyjaSkeleton in Freyja
	 ------------------------------------------------------*/

	~FreyjaSkeleton();
	/*------------------------------------------------------
	 * Pre  : FreyjaSkeleton object is allocated
	 * Post : Deconstructs an object of FreyjaSkeleton
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.10.22: 
	 * Mongoose - Created, from FreyjaSkeleton in Freyja
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	unsigned int getBoneCount() { return mBones.end(); }

	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void connectBones(unsigned int parent, unsigned int child);

	void disconnectBones(unsigned int parent, unsigned int child);


	//private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	int mId;                          /* Unique identifier */

	Vector<FreyjaBone *> mBones;      /* Bones in this skeleton */

	unsigned int mRoot;               /* Root bone index */

	Vector3d mPosition;               /* Position of skeleton */

	static unsigned int mCounter;     /* Used to generate ids */
};

#endif
