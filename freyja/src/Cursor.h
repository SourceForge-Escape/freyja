/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja/
 * Email   : mongoose@icculus.org
 * Object  : Freyja3dCursor
 * License : No use w/o permission (C) 2000 Mongoose
 * Comments: This is the controler class for the client
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2006.07.15:
 * Mongoose - Created, moved into it's own header from FreyjaModel 'test'
 ==========================================================================*/

#ifndef GUARD__FREYJA3D_CURSOR_H_
#define GUARD__FREYJA3D_CURSOR_H_

#include <hel/math.h>
#include <hel/Ray.h>
#include <mstl/stack.h>

#include "freyja_events.h"

#ifdef FIXME
#   include "FreyjaState.h"
#endif

namespace freyja3d {

class Cursor
{
 public:

	typedef enum {
		Rotation,
		Translation,
		Scale,
		Invisible
	} Flags;


	typedef enum {

		eNone = 0,
		eX,
		eY,
		eZ,
		eAll

	} axis_t;


	static vec_t min;
	static vec_t mid;
	static vec_t max;


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Cursor();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Cursor.
	 *
	 ------------------------------------------------------*/

	~Cursor();
	/*------------------------------------------------------
	 * Pre  : Cursor object is allocated.
	 * Post : Deconstructs an object of Cursor.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	void Display();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Render cursor.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	bool CheckForRayCollision(hel::Ray &r);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if given pick ray <r> hits an
	 *        active hit box.  If there is a hit then the
	 *        corresponding axis is stored along with
	 *        flags noting a selection has been made.
	 *
	 ------------------------------------------------------*/

	void Reset();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Reset cursor to defaults.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Properties
	////////////////////////////////////////////////////////////

	Flags GetMode() { return mMode; }
	void SetMode(Flags n) { mMode = n; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Current transform mode.
	 *
	 ------------------------------------------------------*/

	bool mSelected;

	axis_t mAxis;

	hel::Vec3 mLastPos;

	hel::Vec3 mPos;

	hel::Vec3 mScale;

	hel::Vec3 mRotate;


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////

	void Draw3dCursorLoc(const vec_t min, const vec_t mid, const vec_t max);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Render cursor in Loc mode.
	 *
	 ------------------------------------------------------*/

	void Draw3dCursorRot(const vec_t min, const vec_t mid, const vec_t max);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Render cursor for Rot mode.
	 *
	 ------------------------------------------------------*/

	void Draw3dCursorScale(const vec_t min, const vec_t mid, const vec_t max);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Render cursor for Size mode.
	 *
	 ------------------------------------------------------*/

	void Draw3dCursorRotHitBoxes();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Render hit boxes for cursor in Rot mode.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	Flags mMode;
};


} //namespace freyja3d 

#endif // GUARD__FREYJA3D_CURSOR_H_
