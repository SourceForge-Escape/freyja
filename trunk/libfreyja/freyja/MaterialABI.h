/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the material ABI.
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

#ifndef GUARD__FREYJA_MATERIALABI__H_
#define GUARD__FREYJA_MATERIALABI__H_

#include "freyja.h"

extern "C" {

	///////////////////////////////////////////////////////////////////////
	// Material ABI 
	//
	///////////////////////////////////////////////////////////////////////

	freyja_ptr freyjaMaterialCreate( const char* name );
	/*------------------------------------------------------
	 * Pre  : Pass NULL for name to have Resource generate 
	 *        a unique name.
	 *
	 * Post : Returns new Material with name or NULL 
	 *        if there is a name collision in Resource.
	 *
	 ------------------------------------------------------*/

	freyja_ptr freyjaMaterialCreateFromFile( const char *filename );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Material is unserialized.
	 *        Returns material or NULL on error.
	 *        Valid materials will be inserted in Resource.
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialDestroy( freyja_ptr material );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Deallocates material and removes from Resource.
	 *
	 ------------------------------------------------------*/

	freyja_ptr freyjaMaterialFind( const char* name );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns Material with name or NULL 
	 *        if there is no match in Resource.
	 *
	 ------------------------------------------------------*/

	uint32 freyjaMaterialGetCount( );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the number of Materials being managed.
	 *
	 ------------------------------------------------------*/

	const char* freyjaMaterialGetName( freyja_ptr material );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns a pointer to name string.
	 *        Returns NULL on error.
	 *
	 ------------------------------------------------------*/

	const char* freyjaMaterialGetFilename( freyja_ptr material );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns a pointer to filename string.
	 *        Returns NULL if invalid.
	 *
	 ------------------------------------------------------*/

	int16 freyjaMaterialGetShaderId( freyja_ptr material );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns program id or -1 if none.
	 *
	 ------------------------------------------------------*/

	freyja_ptr freyjaMaterialGetDecalMap( freyja_ptr material );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns PixelBuffer or NULL if none.
	 *
	 ------------------------------------------------------*/

	freyja_ptr freyjaMaterialGetDiffuseMap( freyja_ptr material );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns PixelBuffer or NULL if none.
	 *
	 ------------------------------------------------------*/

	freyja_ptr freyjaMaterialGetEmissiveMap( freyja_ptr material );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns PixelBuffer or NULL if none.
	 *
	 ------------------------------------------------------*/

	freyja_ptr freyjaMaterialGetHeightMap( freyja_ptr material );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns PixelBuffer or NULL if none.
	 *
	 ------------------------------------------------------*/

	freyja_ptr freyjaMaterialGetNormalMap( freyja_ptr material );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns PixelBuffer or NULL if none.
	 *
	 ------------------------------------------------------*/

	freyja_ptr freyjaMaterialGetSpecularMap( freyja_ptr material );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns PixelBuffer or NULL if none.
	 *
	 ------------------------------------------------------*/

	const char* freyjaMaterialGetShaderFilename( freyja_ptr material );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns filename of shader used by this material or NULL.
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialGetAmbientColor( freyja_ptr material, vec4_t ambient );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns <ambient> color.
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialGetDiffuseColor(freyja_ptr material, vec4_t diffuse);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Returns <diffuse> color
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialGetSpecularColor(freyja_ptr material, vec4_t specular);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Returns <specular> color
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialGetEmissiveColor(freyja_ptr material, vec4_t emissive);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Returns <emissive> color
	 *
	 ------------------------------------------------------*/

	vec_t freyjaMaterialGetShininess(freyja_ptr material);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Returns specular exponent or -1.0 on error
	 *
	 ------------------------------------------------------*/

	vec_t freyjaMaterialGetTransparency(freyja_ptr material);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Returns transparency or -1.0 on error
	 *
	 ------------------------------------------------------*/

	const char* freyjaMaterialGetBlendSource(freyja_ptr material);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Returns blend source factor or NULL on error
	 *
	 ------------------------------------------------------*/

	const char* freyjaMaterialGetBlendDestination(freyja_ptr material);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Returns blend destination factor or NULL on error
	 *
	 ------------------------------------------------------*/


	/* Material Mutators */


	void freyjaMaterialSetName(freyja_ptr material, const char *name);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Material's <name> id is set
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialSetShaderFilename(freyja_ptr material, const char* filename);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Material's <shader> index is set
	 *        0 will disable shading
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialSetShaderId(freyja_ptr material, int16 shader);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Material's <shader> index is set
	 *        0 will disable shading
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialSetDecalMap( freyja_ptr material, freyja_ptr pixelbuffer );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialSetDiffuseMap( freyja_ptr material, freyja_ptr pixelbuffer );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialSetEmissiveMap( freyja_ptr material, freyja_ptr pixelbuffer );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialSetHeightMap( freyja_ptr material, freyja_ptr pixelbuffer );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialSetNormalMap( freyja_ptr material, freyja_ptr pixelbuffer );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialSetSpecularMap( freyja_ptr material, freyja_ptr pixelbuffer );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialSetAmbientColor(freyja_ptr material, const vec4_t ambient);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Material's <ambient> color is set
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialSetDiffuseColor(freyja_ptr material, const vec4_t diffuse);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Material's <diffuse> color is set
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialSetSpecularColor(freyja_ptr material, const vec4_t specular);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Material's <specular> color is set
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialSetEmissiveColor(freyja_ptr material, const vec4_t emissive);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Material's <emissive> color is set
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialSetShininess(freyja_ptr material, vec_t exponent);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Material's Specular <exponent> is set
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialSetTransparency(freyja_ptr material, vec_t transparency);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 *        <transparency> is a value from 0.0 to 1.0
	 *
	 * Post : Material's <transparency> is set
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialSetBlendSource(freyja_ptr material, const char* factor);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Material's blend source <factor> is set
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialSetBlendDestination(freyja_ptr material, const char* factor);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Material's blend destination <factor> is set
	 *
	 ------------------------------------------------------*/

} // extern "C"

#endif // GUARD__FREYJA_MATERIALABI__H_
