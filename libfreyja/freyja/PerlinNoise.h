/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Midgard
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : PerlinNoise
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This class makes with the perlin noise ;)
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_PERLINNOISE - Builds PerlinNoise class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.03.11:
 * Mongoose - Created
 ================================================================*/


#ifndef GUARD__MIDGARD_MONGOOSE_PERLINNOISE_H_
#define GUARD__MIDGARD_MONGOOSE_PERLINNOISE_H_

#include <hel/math.h>


class PerlinNoise
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	PerlinNoise();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of PerlinNoise
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.11: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~PerlinNoise();
	/*------------------------------------------------------
	 * Pre  : PerlinNoise object is allocated
	 * Post : Deconstructs an object of PerlinNoise
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.11: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	void clampBufferIntensity(byte *buffer, uint32 w, uint32 h,
									  vec_t a, vec_t b, vec_t c);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	vec_t interpolate(vec_t a, vec_t b, vec_t c);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	vec_t linearInterpolate(vec_t a, vec_t b, vec_t c);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	vec_t noise(int32 n);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	vec_t noise2D(vec_t x, vec_t y);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	vec_t smoothedNoise2D(vec_t x, vec_t y);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	vec_t interpolatedNoise2D(vec_t x, vec_t y);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	vec_t perlinNoise2D(vec_t x, vec_t y, uint32 octaves,
							  uint32 seed, double period, uint32 width);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	byte *generateBuffer(uint32 width, uint32 height, uint32 seed);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////


	uint32 mPrimeSeeds[3];   /* The prime numbers to seed the function */
};

#endif
