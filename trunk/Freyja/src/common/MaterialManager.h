/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Midgard
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : MaterialManager
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This is the all singing all dancing interface 
 *           to Material objects
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_MATERIALMANAGER - Builds MaterialManager class as 
 *                             a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.03.25:
 * Mongoose - Created
 =========================================================================*/


#ifndef GUARD__MIDGARD_MONGOOSE_MATERIALMANAGER_H_
#define GUARD__MIDGARD_MONGOOSE_MATERIALMANAGER_H_

#include <mstl/Vector.h>
#include <hel/math.h>
#include "Material.h"
#include "Texture.h"
#include "Light.h"


#define gMaterialManager MaterialManager::mInstance


class MaterialManager
{
 protected:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	MaterialManager();
	MaterialManager(const MaterialManager&);
	MaterialManager& operator= (const MaterialManager&);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of MaterialManager
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.25: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

 public:
	
	typedef enum {
		fLoadTextureInSlot = 1

	} fOptionFlags;

	typedef enum {	
		eAmbient = 1,
		eDiffuse,
		eSpecular,
		eEmissive

	} eColorTypes;


	static void DestroyInstance();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of MaterialManager
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.25: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	static MaterialManager *Instance();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of MaterialManager
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.25: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~MaterialManager();
	/*------------------------------------------------------
	 * Pre  : MaterialManager object is allocated
	 * Post : Deconstructs an object of MaterialManager
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.25: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	void applyEffectGL();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Apply current material using OpenGL
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.25: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void applyEffectGL(unsigned int index);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Apply material[index] using OpenGL,
	 *        this call doesn't affect Current index
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.25: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int getBlendSourceIndex();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get current material's Blend Source Index
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.25: 
	 * Mongoose - Created
	 ------------------------------------------------------*/
	
	unsigned int getBlendDestIndex();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get current material's Blend Dest Index
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.25: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int getCurrent();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get current material index
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.25: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void getColor(eColorTypes type, vec4_t color);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get current material's color
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.25: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int getFlags();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get current material flags
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.25: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int getGeneralFlags();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get MaterialManager flags
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.25: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	const char *getName();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get current material name
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.25: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	vec_t getShininess();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get current material's shininess
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.25: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int getTexture(unsigned int layer);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get current material's texture id for layer
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.25: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int getTextureLoadFlags();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns Texture loading flags
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.26:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int save(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Save current material as filename
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.25: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void takeScreenshot(const char *filename,
						unsigned int width, unsigned int height);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Dumps screenshot to <filename> 
	 *        Returns 0 on success
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.27:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void clearFlag(Material::MaterialFlags_t flag);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Clears current material's flag
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.26:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void clearGeneralFlag(MaterialManager::fOptionFlags flag);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get MaterialManager flags
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.25: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int load(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Load material from filename over current material 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.25: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int loadTexturePalette(const char *filename);
	/*------------------------------------------------------
	 * Pre  : <filename> is valid string of fullpath and filename
	 * Post : Returns 0 on success
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.26:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int loadTexture(const char *filename);
	/*------------------------------------------------------
	 * Pre  : <filename> is valid string of fullpath and filename
	 * Post : Returns 0 on success
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.26:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int loadTextureBuffer(unsigned char *image, 
						  unsigned int width, unsigned int height, 
						  unsigned int bpp, Texture::ColorMode type);
	/*------------------------------------------------------
	 * Pre  : <filename> is valid string of fullpath and filename
	 * Post : Returns 0 on success
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.26:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setBlendSource(unsigned int blend);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set current material's Blend Source
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.25: 
	 * Mongoose - Created
	 ------------------------------------------------------*/
	
	void setBlendDest(unsigned int blend);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set current material's Blend Dest
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.25: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setCurrent(unsigned int index);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set current material index
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.25: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setColor(eColorTypes type, vec4_t color);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get current material's color
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.25: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setFlag(Material::MaterialFlags_t flag);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets current material's flag
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.26:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setGeneralFlag(MaterialManager::fOptionFlags flag);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get MaterialManager flags
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.25: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setName(const char *name);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set current material's name
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.25: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setShininess(vec_t shine);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set current material's shininess
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.25: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setTexture(unsigned int layer, unsigned int id);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set current material's texture id for layer
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.25: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setTextureLoadFlags(unsigned int flags);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets Texture loading flags
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.26:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	static MaterialManager *mInstance;   /* Singleton instance */

	unsigned int mTextureId;


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	Vector<Material *> mMaterials;       /* List of materials */

	Texture mTexture;

	unsigned char *mPalette;

	unsigned int mFlags;

	unsigned int mCurrent;               /* Current material to operate on */
};

#endif
