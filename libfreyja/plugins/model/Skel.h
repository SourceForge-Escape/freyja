/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : Skel
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This is an ASE like ASCII skeleton.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_SKEL - Builds Skel class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.12.31:
 * Mongoose - Created, from my old ASE class' 'ASESkelExt'
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_SKEL_H_
#define GUARD__FREYJA_MONGOOSE_SKEL_H_


#ifndef vec_t
typedef float vec_t;
#endif

#ifndef vec2_t
typedef vec_t vec2_t[2];
#endif

#ifndef vec3_t
typedef vec_t vec3_t[3];
#endif

#ifndef vec4_t
typedef vec_t vec4_t[4];
#endif


class Skel
{
 public:

	typedef struct weight_s
	{
		vec_t weight;
		int boneId;

		struct weight_s *next;

	} weight_t;


	typedef struct bone_s
	{
		char name[64];
		unsigned int id;
		int parent;
		unsigned int childrenCount; 
		unsigned int *children;

		vec3_t translation;     /* XYZ */
		vec4_t rotation;        /* XYZW Quaterion*/

	} bone_t;


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Skel();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Skel
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.12.31: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~Skel();
	/*------------------------------------------------------
	 * Pre  : Skel object is allocated
	 * Post : Deconstructs an object of Skel
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.12.31: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	int check(char *filename);
	/*------------------------------------------------------
	 * Pre  : <filename> is valid string
	 * Post : Returns 0 if check thinks filename is skel
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.12.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void print();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Prints out description of Skel
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.24:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int save(char *filename);
	/*------------------------------------------------------
	 * Pre  : <filename> is valid string
	 * Post : Returns 0 if load is successful
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.24:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	int load(char *filename);
	/*------------------------------------------------------
	 * Pre  : <filename> is valid string
	 * Post : Returns 0 if load is successful
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.22:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	char mName[64];

	unsigned long mBoneCount;

	unsigned long mWeightCount;
	
	weight_t *mWeights;

	bone_t *mBones;


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////


	/* */
};

#endif
