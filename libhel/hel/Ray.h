/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : hel
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : Ray
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the ray class
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_RAY - Builds Ray class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2005.07.03:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__HEL_MONGOOSE_RAY_H_
#define GUARD__HEL_MONGOOSE_RAY_H_

#include "hel/Vector3d.h"

class Ray
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Ray();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Ray
	 ------------------------------------------------------*/

	Ray(const Vec3 &origin, const Vec3 &dir);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Ray
	 ------------------------------------------------------*/

	Ray(const Ray &ray);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Ray
	 ------------------------------------------------------*/

	~Ray();
	/*------------------------------------------------------
	 * Pre  : Ray object is allocated
	 * Post : Deconstructs an object of Ray
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	const Vec3 &GetDir() { return mDir; }


	const Vec3 &GetOrigin() { return mOrigin; }


	bool IntersectSphere(vec3_t center, vec_t radius, vec_t &t);
	

	bool IntersectTriangle(vec3_t vert0, vec3_t vert1, vec3_t vert2, vec3_t tuv);
	/*------------------------------------------------------
	 * Pre  : Given 3 vertices to define triangle
	 * Post : Returns true if ray intersects
	 *
	 *        Returns tuv:
	 *           T - Distance to the plane ( of the triangle )
	 *           UV coords of the intersection on that triangle
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void GetDir(const Vec3 &d) { mDir = d; }

	void SetOrigin(const Vec3 &o) { mOrigin = o; }


	Vec3 mOrigin;		/* */

	Vec3 mDir;			/* */

 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////



};

#endif
