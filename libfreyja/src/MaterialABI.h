/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the material data model class.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 *
 *-- History ------------------------------------------------ 
 *
 * 2005.07.12:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_FREYJAMATERIALABI_H_
#define GUARD__FREYJA_MONGOOSE_FREYJAMATERIALABI_H_

#include "freyja.h"

extern "C" {

	enum freyja_material_flags {
		fFreyjaMaterial_Blending = 1,
		fFreyjaMaterial_Texture = 2,
		fFreyjaMaterial_DetailTexture = 4,
		fFreyjaMaterial_Normalize = 8
	};


	index_t freyjaMaterialCreate();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Creates a new Freyja Material object
	 *        Returns the new Material's index or -1 on error
	 ------------------------------------------------------*/

	void freyjaMaterialDelete(index_t materialIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Creates a new Freyja Material object
	 *        Returns the new Material's index or -1 on error
	 ------------------------------------------------------*/

	index_t freyjaGetCurrentMaterial();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaCurrentMaterial(index_t materialIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/


	/* Material Accessors */

	uint32 freyjaGetMaterialCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the number of Materials being managed
	 ------------------------------------------------------*/

	index_t freyjaGetMaterialIndex(index_t materialIndex, uint32 element);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns the gobal object index ( from the local
	 *        Material element index ) or -1 on error
	 ------------------------------------------------------*/

	char *freyjaGetMaterialName(index_t materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 *        Don't alter the returned string
	 *
	 * Post : Returns a pointer to NULL terminated name string
	 *        Returns 0x0 on error
	 ------------------------------------------------------*/

	byte freyjaGetMaterialFlags(index_t materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns flags or -1 on error
	 ------------------------------------------------------*/

	uint32 freyjaGetMaterialTexture(index_t materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns texture index or -1 for error or no texture
	 ------------------------------------------------------*/

	const char *freyjaGetMaterialTextureName(index_t materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaGetMaterialAmbient(index_t materialIndex, vec4_t ambient);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns <ambient> color
	 ------------------------------------------------------*/

	void freyjaGetMaterialDiffuse(index_t materialIndex, vec4_t diffuse);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns <diffuse> color
	 ------------------------------------------------------*/

	void freyjaGetMaterialSpecular(index_t materialIndex, vec4_t specular);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns <specular> color
	 ------------------------------------------------------*/

	void freyjaGetMaterialEmissive(index_t materialIndex, vec4_t emissive);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns <emissive> color
	 ------------------------------------------------------*/

	vec_t freyjaGetMaterialShininess(index_t materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns specular exponent or -1.0 on error
	 ------------------------------------------------------*/

	vec_t freyjaGetMaterialTransparency(index_t materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns transparency or -1.0 on error
	 ------------------------------------------------------*/

	int32 freyjaGetMaterialBlendSource(index_t materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns blend source factor or -1 on error
	 ------------------------------------------------------*/

	int32 freyjaGetMaterialBlendDestination(index_t materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns blend destination factor or -1 on error
	 ------------------------------------------------------*/


	/* Material Mutators */

	void freyjaMaterialName(index_t materialIndex, const char *name);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's <name> id is set
	 ------------------------------------------------------*/

	void freyjaMaterialClearFlag(index_t materialIndex, int32 flag);
	void freyjaMaterialSetFlag(index_t materialIndex, int32 flag);
	void freyjaMaterialFlags(index_t materialIndex, int32 flags);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's bit <flags> are set
	 ------------------------------------------------------*/

	void freyjaMaterialTextureName(index_t materialIndex, const char *name);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material textures's <name> id is set
	 ------------------------------------------------------*/

	void freyjaMaterialTexture(index_t materialIndex, index_t textureIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's <texture> index is set
	 ------------------------------------------------------*/

	void freyjaMaterialAmbient(index_t materialIndex, const vec4_t ambient);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's <ambient> color is set
	 ------------------------------------------------------*/

	void freyjaMaterialDiffuse(index_t materialIndex, const vec4_t diffuse);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's <diffuse> color is set
	 ------------------------------------------------------*/

	void freyjaMaterialSpecular(index_t materialIndex, const vec4_t specular);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's <specular> color is set
	 ------------------------------------------------------*/

	void freyjaMaterialEmissive(index_t materialIndex, const vec4_t emissive);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's <emissive> color is set
	 ------------------------------------------------------*/

	void freyjaMaterialShininess(index_t materialIndex, vec_t exponent);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's Specular <exponent> is set
	 ------------------------------------------------------*/

	void freyjaMaterialTransparency(index_t materialIndex, vec_t transparency);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 *        <transparency> is a value from 0.0 to 1.0
	 *
	 * Post : Material's <transparency> is set
	 ------------------------------------------------------*/

	void freyjaMaterialBlendSource(index_t materialIndex, uint32 factor);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's blend source <factor> is set
	 ------------------------------------------------------*/

	void freyjaMaterialBlendDestination(index_t materialIndex, uint32 factor);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's blend destination <factor> is set
	 ------------------------------------------------------*/


}

#endif
