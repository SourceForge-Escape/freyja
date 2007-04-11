/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : Model
 * License : No use w/o permission (C) 2006 Mongoose
 * Comments: This is the model container class, which mainly 
 *           seperates data structures to allow multiple 
 *           editing contexts in one system.
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_MODEL - Builds Model class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2006.08.02:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__FREYJA_MODEL_H_
#define GUARD__FREYJA_MODEL_H_

#include <mstl/Vector.h>
#include "freyja.h"


class Model
{
 public:

	typedef enum {
		fNone        =  0,
		fHighlighted =  1,
		fSelected    =  2,
		fHidden      =  4,
		fRayHit      =  8
	} Flags;


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Model();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Model
	 *
	 ------------------------------------------------------*/

	~Model();
	/*------------------------------------------------------
	 * Pre  : Model object is allocated
	 * Post : Deconstructs an object of Model
	 *
	 ------------------------------------------------------*/

	////////////////////////////////////////////////////////////
	// Properities
	////////////////////////////////////////////////////////////

	byte GetFlags() { return mFlags; }
	void ClearFlag(Flags flag) { mFlags &= ~flag; }
	void SetFlag(Flags flag) { mFlags |= flag; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : The option flags property for this model
	 *
	 ------------------------------------------------------*/

	index_t GetSkeleton() { return mSkeleton; }
	void SetSkeleton(index_t skeleton) {  mSkeleton = skeleton; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : The option flags property for this model
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	uint32 GetMeshCount() { return mMeshes.size(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : The number of meshes in this model
	 *
	 ------------------------------------------------------*/

	index_t GetMeshIndex(uint32 i) 
	{ return (i < mMeshes.size()) ? mMeshes[i] : INDEX_INVALID; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : The gobal mesh index for the 'ith' model mesh
	 *        This can return INDEX_INVALID for 'sparse' meshes too.
	 *
	 ------------------------------------------------------*/

	bool Intersect(hel::Ray &r, vec_t &t);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if 'hit', and <t> the time along ray <r>
	 *        If Model is fHidden this will always return false.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void AddMesh(index_t mesh);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : <mesh> is appended to this Model
	 *
	 ------------------------------------------------------*/

	void RemoveMesh(index_t mesh);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : <mesh> is removed from this Model
	 *
	 ------------------------------------------------------*/


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	byte mFlags;                    /* Option flags */

	index_t mSkeleton;              /* Corresponding skeleton gobal index */

	mstl::Vector<index_t> mMeshes;  /* Meshes owned by this model */
};

#endif
