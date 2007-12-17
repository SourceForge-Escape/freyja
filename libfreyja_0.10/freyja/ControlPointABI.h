/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2006 Mongoose
 * Comments: Control points in the backend are for special interface use.
 *
 * 
 *-- History ------------------------------------------------ 
 *
 * 2006.12.14:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__FREYJA_CONTROLPOINTABI_H_
#define GUARD__FREYJA_CONTROLPOINTABI_H_

#include "freyja.h"


extern "C" {

	///////////////////////////////////////////////////////////////////////
	// ControlPoint ABI 0.9.5
	///////////////////////////////////////////////////////////////////////

	uint32 freyjaGetControlPointCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the number of control point groups
	 ------------------------------------------------------*/

	index_t freyjaControlPointCreate();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns new index or INDEX_INVALID
	 ------------------------------------------------------*/

	void freyjaControlPointCallback(index_t group, void (*cb)(vec3_t));
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns new index or INDEX_INVALID
	 ------------------------------------------------------*/

	void freyjaControlPointDelete(index_t group);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Deletes control point <group> given any valid index 
	 ------------------------------------------------------*/

	uint32 freyjaGetControlPointItemCount(index_t group);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the number of control points in <group>
	 ------------------------------------------------------*/

	index_t freyjaControlPointItemCreate2fv(index_t group, vec2_t v);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns new index or INDEX_INVALID
	 ------------------------------------------------------*/

	index_t freyjaControlPointItemCreate2f(index_t group, vec_t u, vec_t v);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns new index or INDEX_INVALID
	 ------------------------------------------------------*/

	index_t freyjaControlPointItemCreate3fv(index_t group, vec3_t v);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns new index or INDEX_INVALID
	 ------------------------------------------------------*/

	index_t freyjaControlPointItemCreate3f(index_t group, 
										   vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns new index or INDEX_INVALID
	 ------------------------------------------------------*/
}

#endif // GUARD__FREYJA_CONTROLPOINTABI_H_
