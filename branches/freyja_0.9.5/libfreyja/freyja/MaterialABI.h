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

#ifndef GUARD__FREYJA_MATERIALABI_H_
#define GUARD__FREYJA_MATERIALABI_H_

#include "freyja.h"

extern "C" {

	///////////////////////////////////////////////////////////////////////
	// Material ABI 0.9.3 - 0.9.5
	///////////////////////////////////////////////////////////////////////

	index_t freyjaMaterialCreate();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Creates a new Freyja Material object
	 *        Returns the new Material's index or -1 on error
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialClearAll();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Flush all allocated materials.
	 *
	 *        Materials are typically overwritten instead of
	 *        deallocated.  A rare exception to the rule.
	 *
	 ------------------------------------------------------*/

	index_t freyjaGetCurrentMaterial();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void freyjaCurrentMaterial(index_t material);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/


	/* Material Accessors */

	uint32 freyjaGetMaterialCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the number of Materials being managed
	 *
	 ------------------------------------------------------*/

	index_t freyjaGetMaterialIndex(index_t material, uint32 element);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Returns the gobal object index ( from the local
	 *        Material element index ) or INDEX_INVALID
	 *
	 ------------------------------------------------------*/

	const char *freyjaGetMaterialName(index_t material);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 *        Don't alter the returned string
	 *
	 * Post : Returns a pointer to NULL terminated name string
	 *        Returns 0x0 on error
	 *
	 ------------------------------------------------------*/

	uint32 freyjaGetMaterialFlags(index_t material);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Returns flags or -1 on error
	 *
	 ------------------------------------------------------*/

	index_t freyjaGetMaterialShader(index_t material);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Returns shader index or 0 for error or no shader
	 *
	 ------------------------------------------------------*/

	index_t freyjaGetMaterialTexture(index_t material);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Returns texture index or -1 for error or no texture
	 *
	 ------------------------------------------------------*/

	const char *freyjaGetMaterialShaderName(index_t material);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : 
	 *
	 ------------------------------------------------------*/

	const char *freyjaGetMaterialTextureName(index_t material);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void freyjaGetMaterialAmbient(index_t material, vec4_t ambient);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Returns <ambient> color
	 *
	 ------------------------------------------------------*/

	void freyjaGetMaterialDiffuse(index_t material, vec4_t diffuse);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Returns <diffuse> color
	 *
	 ------------------------------------------------------*/

	void freyjaGetMaterialSpecular(index_t material, vec4_t specular);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Returns <specular> color
	 *
	 ------------------------------------------------------*/

	void freyjaGetMaterialEmissive(index_t material, vec4_t emissive);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Returns <emissive> color
	 *
	 ------------------------------------------------------*/

	vec_t freyjaGetMaterialShininess(index_t material);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Returns specular exponent or -1.0 on error
	 *
	 ------------------------------------------------------*/

	vec_t freyjaGetMaterialTransparency(index_t material);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Returns transparency or -1.0 on error
	 *
	 ------------------------------------------------------*/

	int32 freyjaGetMaterialBlendSource(index_t material);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Returns blend source factor or -1 on error
	 *
	 ------------------------------------------------------*/

	int32 freyjaGetMaterialBlendDestination(index_t material);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Returns blend destination factor or -1 on error
	 *
	 ------------------------------------------------------*/


	/* Material Mutators */

	int32 freyjaLoadMaterialASCII(index_t material, const char *filename);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Material is reset by loaded disk file
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialName(index_t material, const char *name);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Material's <name> id is set
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialClearFlag(index_t material, uint32 flag);
	void freyjaMaterialSetFlag(index_t material, uint32 flag);
	void freyjaMaterialFlags(index_t material, uint32 flags);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Material's bit <flags> are set
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialShaderName(index_t material, const char *filename);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Material shader <filename> is set
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialTextureName(index_t material, const char *name);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Material textures's <name> id is set
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialShader(index_t material, index_t shader);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Material's <shader> index is set
	 *        0 will disable shading
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialTexture(index_t material, index_t texture);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Material's <texture> index is set
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialAmbient(index_t material, const vec4_t ambient);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Material's <ambient> color is set
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialDiffuse(index_t material, const vec4_t diffuse);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Material's <diffuse> color is set
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialSpecular(index_t material, const vec4_t specular);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Material's <specular> color is set
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialEmissive(index_t material, const vec4_t emissive);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Material's <emissive> color is set
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialShininess(index_t material, vec_t exponent);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Material's Specular <exponent> is set
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialTransparency(index_t material, vec_t transparency);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 *        <transparency> is a value from 0.0 to 1.0
	 *
	 * Post : Material's <transparency> is set
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialBlendSource(index_t material, uint32 factor);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Material's blend source <factor> is set
	 *
	 ------------------------------------------------------*/

	void freyjaMaterialBlendDestination(index_t material, uint32 factor);
	/*------------------------------------------------------
	 * Pre  : Material <material> exists
	 * Post : Material's blend destination <factor> is set
	 *
	 ------------------------------------------------------*/
}


#   if defined( __cplusplus ) && defined( USING_FREYJA_CPP_ABI )
#      include <mstl/SystemIO.h>
#      include <freyja/Material.h>
freyja::Material *freyjaGetMaterialClass(index_t materialIndex);
bool freyjaMaterialLoadChunkTextJA(SystemIO::TextFileReader &r);

#   endif // defined( __cplusplus ) && defined( USING_FREYJA_CPP_ABI )

#endif // GUARD__FREYJA_MATERIALABI_H_
