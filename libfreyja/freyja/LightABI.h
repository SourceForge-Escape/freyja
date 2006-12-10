/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the Light ABI.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 *
 *-- History ------------------------------------------------ 
 *
 * 2006.07.09:
 * Mongoose - Created, moved from other headers
 ==========================================================================*/

#ifndef GUARD__FREYJA_LIGHTABI_H_
#define GUARD__FREYJA_LIGHTABI_H_

#include "freyja.h"

extern "C" {

	////////////////////////////////////////////////////////////////
	// Light ABI 0.9.3-0.9.5
	////////////////////////////////////////////////////////////////

	index_t freyjaLightCreate();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns index of light spawned
	 *
	 ------------------------------------------------------*/

	uint32 freyjaGetLightCount();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns index of light spawned
	 *
	 ------------------------------------------------------*/

	index_t freyjaGetCurrentLight();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns index of selected (backend) light 
	 *
	 ------------------------------------------------------*/

	void freyjaCurrentLight(uint32 lightIndex);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Sets selected (backend) light 
	 *
	 ------------------------------------------------------*/

	void freyjaLightDelete(index_t lightIndex);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Light is removed from light pool
	 *
	 ------------------------------------------------------*/

	// Light mutators /////////////////

	void freyjaLightPosition4v(index_t lightIndex, vec4_t position);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Sets <position> 
	 *
	 ------------------------------------------------------*/

	void freyjaLightAmbient(index_t lightIndex, vec4_t ambient);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Sets <ambient> color
	 *
	 ------------------------------------------------------*/

	void freyjaLightDiffuse(index_t lightIndex, vec4_t diffuse);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Sets <diffuse> color
	 *
	 ------------------------------------------------------*/

	void freyjaLightSpecular(index_t lightIndex, vec4_t specular);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Sets <specular> color
	 *
	 ------------------------------------------------------*/

	// Light accessors /////////////////

	void freyjaGetLightPosition4v(index_t lightIndex, vec4_t position);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Returns <position> 
	 *
	 ------------------------------------------------------*/

	void freyjaGetLightAmbient(index_t lightIndex, vec4_t ambient);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Returns <ambient> color
	 *
	 ------------------------------------------------------*/

	void freyjaGetLightDiffuse(index_t lightIndex, vec4_t diffuse);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Returns <diffuse> color
	 *
	 ------------------------------------------------------*/

	void freyjaGetLightSpecular(index_t lightIndex, vec4_t specular);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Returns <specular> color
	 *
	 ------------------------------------------------------*/

}

#endif // GUARD__FREYJA_LIGHTABI_H_
