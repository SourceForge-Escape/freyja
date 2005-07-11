/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : FreyjaLight
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This is the light data model class.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_FREYJALIGHT - Builds FreyjaLight class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.10.22:
 * Mongoose - Created, based on Freyja's Light class from 2002.01.27
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_FREYJALIGHTABI_H_
#define GUARD__FREYJA_MONGOOSE_FREYJALIGHTABI_H_

#include "freyja.h"

extern "C" {

	index_t freyjaLightCreate();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns index of light spawned
	 ------------------------------------------------------*/

	void freyjaLightDelete(index_t lightIndex);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Light[index] is removed from light pool
	 ------------------------------------------------------*/

	index_t freyjaGetCurrentLight();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns index of light spawned
	 ------------------------------------------------------*/


	// Light mutators /////////////////

	void freyjaLightPosition4v(index_t lightIndex, vec4_t position);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Sets <position> 
	 ------------------------------------------------------*/

	void freyjaLightAmbient(index_t lightIndex, vec4_t ambient);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Sets <ambient> color
	 ------------------------------------------------------*/

	void freyjaLightDiffuse(index_t lightIndex, vec4_t diffuse);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Sets <diffuse> color
	 ------------------------------------------------------*/

	void freyjaLightSpecular(index_t lightIndex, vec4_t specular);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Sets <specular> color
	 ------------------------------------------------------*/


	// Light accessors /////////////////

	void freyjaGetLightPosition4v(index_t lightIndex, vec4_t position);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Returns <position> 
	 ------------------------------------------------------*/

	void freyjaGetLightAmbient(index_t lightIndex, vec4_t ambient);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Returns <ambient> color
	 ------------------------------------------------------*/

	void freyjaGetLightDiffuse(index_t lightIndex, vec4_t diffuse);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Returns <diffuse> color
	 ------------------------------------------------------*/

	void freyjaGetLightSpecular(index_t lightIndex, vec4_t specular);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Returns <specular> color
	 ------------------------------------------------------*/
}
#endif
