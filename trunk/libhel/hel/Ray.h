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

#include "hel/Vec3.h"

namespace hel {

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
	 *
	 ------------------------------------------------------*/

	Ray(const Vec3 &origin, const Vec3 &dir);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Ray
	 *
	 ------------------------------------------------------*/

	Ray(const Ray &ray);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Ray
	 *
	 ------------------------------------------------------*/

	~Ray();
	/*------------------------------------------------------
	 * Pre  : Ray object is allocated
	 * Post : Deconstructs an object of Ray
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Properties
	////////////////////////////////////////////////////////////

	const Vec3& GetDir();
	void SetDir(const Vec3& dir);
	void SetDir(const vec_t i, const vec_t j, const vec_t k);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get/Set ray origin property.
	 *
	 ------------------------------------------------------*/

	const Vec3& GetOrigin();
	void SetOrigin(const Vec3& origin);
	void SetOrigin(const vec_t x, const vec_t y, const vec_t z);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get/Set ray direction property.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Methods
	////////////////////////////////////////////////////////////

	bool IntersectBox(vec3_t v0, vec3_t v1, vec3_t v2, vec3_t v3, 
	                  vec3_t v4, vec3_t v5, vec3_t v6, vec3_t v7, vec_t &t);
	/*------------------------------------------------------
	 * Pre  : Given some completely generic bounding box.
	 *
	 * Post : Returns <t> value along ray if Box and ray intersect. 
	 *        Returns true if Box and ray intersect.
	 *
	 ------------------------------------------------------*/

	bool IntersectBox(vec3_t min, vec3_t max, vec_t &t);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : FIXME: Obsolete API call for AABB intersection.
	 *
	 ------------------------------------------------------*/

	bool IntersectAABB(vec3_t min, vec3_t max); 
	/*------------------------------------------------------
	 * Pre  : Given <min> and <max> of AABB.
	 * Post : Returns true if AABB and ray intersect.
	 *
	 ------------------------------------------------------*/

	bool IntersectAABB(vec3_t min, vec3_t max, vec_t &t);
	/*------------------------------------------------------
	 * Pre  : Given <min> and <max> of AABB. 
	 *
	 * Post : Returns <t> value along ray if AABB and ray intersect. 
	 *        Returns true if AABB and ray intersect.
	 *
	 ------------------------------------------------------*/

	bool IntersectAABB_Old(vec3_t min, vec3_t max, vec_t &t);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool IntersectAABB_SAT(Vec3 min, Vec3 max, vec_t& t);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool IntersectOBB(vec3_t center, vec3_t u, vec3_t v, vec3_t w, vec_t &t);
	/*------------------------------------------------------
	 * Pre  : Given <min> and <max> of OBB. 
	 *
	 * Post : Returns <t> value along ray if AABB and ray intersect. 
	 *        Returns true if OBB and ray intersect.
	 *
	 ------------------------------------------------------*/

	//bool IntersectDOP(...);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool IntersectSphere(vec3_t center, vec_t radius, vec_t &t);
	/*------------------------------------------------------
	 * Pre  : Given generic sphere.
	 *
	 * Post : Returns true if there is an intersection.
	 *        Returns distance along ray <t> if intersects.
	 *
	 ------------------------------------------------------*/

	bool IntersectTriangle(const Vec3& a, const Vec3& b, const Vec3& c, Vec3& tuv);
	/*------------------------------------------------------
	 * Pre  : Given 3 vertices ABC to define triangle...
	 * Post : Returns true if ray intersects.
	 *
	 *        Returns tuv:
	 *           T - Distance to the plane ( of the triangle )
	 *           UV coords of the intersection on that triangle
	 ------------------------------------------------------*/

	bool IntersectTriangle(vec3_t a, vec3_t b, vec3_t c, vec3_t tuv);
	/*------------------------------------------------------
	 * Pre  : Given 3 vertices ABC to define triangle...
	 * Post : Returns true if ray intersects.
	 *
	 *        Returns tuv:
	 *           T - Distance to the plane ( of the triangle )
	 *           UV coords of the intersection on that triangle
	 ------------------------------------------------------*/

	void Norm();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Normalize the dir vector.
	 *
	 ------------------------------------------------------*/


 private:

	////////////////////////////////////////////////////////////
	// Private Methods
	////////////////////////////////////////////////////////////

	Vec3 mOrigin;		/* Ray origin. */

	Vec3 mDir;			/* Direction of the Ray. */


};


////////////////////////////////////////////////////////////
// Inline Public Methods
////////////////////////////////////////////////////////////

inline
Ray::Ray() :
	mOrigin(),
	mDir()
{ }


inline
Ray::Ray(const Vec3 &origin, const Vec3 &dir) :
	mOrigin(origin),
	mDir(dir)
{ }


inline
Ray::Ray(const Ray &ray) :
	mOrigin(ray.mOrigin),
	mDir(ray.mDir)
{ }


inline
Ray::~Ray()
{ }


inline
const Vec3& Ray::GetDir() 
{ return mDir; }


inline
void Ray::SetDir(const Vec3& dir) 
{ mDir = dir; }


inline
void Ray::SetDir(const vec_t i, const vec_t j, const vec_t k) 
{ mDir.Set(i,j,k); }


inline
const Vec3& Ray::GetOrigin() 
{ return mOrigin; }


inline
void Ray::SetOrigin(const Vec3& origin) 
{ mOrigin = origin; }


inline
void Ray::SetOrigin(const vec_t x, const vec_t y, const vec_t z) 
{ mOrigin.Set(x,y,z); }


inline
bool Ray::IntersectBox(vec3_t min, vec3_t max, vec_t &t)
{
	return IntersectAABB_SAT(min, max, t);
}


inline
bool Ray::IntersectAABB(vec3_t min, vec3_t max, vec_t &t)
{
	return IntersectAABB_SAT(min, max, t);
}


inline 
void Ray::Norm()
{ mDir.Norm(); }



} // namespace hel

#endif
