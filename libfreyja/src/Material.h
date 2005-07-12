/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : FreyjaMaterial
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: Material class for libfreyja, yadda yadda
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_FREYJAMATERIAL - Builds FreyjaMaterial class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2005.01.08:
 * Mongoose - Created, based on old Material class
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_FREYJAMATERIAL_H_
#define GUARD__FREYJA_MONGOOSE_FREYJAMATERIAL_H_

#include <hel/math.h>
#include <mstl/Vector.h>


#include "freyja.h"
#include "Texture.h"
#include "FileReader.h"
#include "FileWriter.h"

#define FreyjaMaterial Material

namespace freyja {

class Material
{
 public:

	enum Flags
	{
		fMaterial_DetailTexure = 1,		
	};


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Material();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of FreyjaMaterial
	 ------------------------------------------------------*/

	virtual ~Material();
	/*------------------------------------------------------
	 * Pre  : FreyjaMaterial object is allocated
	 * Post : Deconstructs an object of FreyjaMaterial
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

   static int32 getBlendIndex(int blend);
	/*------------------------------------------------------
	 * Pre  : Pass blend value, Built with HAVE_GL
	 * Post : Returns -1 if not used, or index if used
	 ------------------------------------------------------*/

	static uint32 getCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns number of unique materials
	 ------------------------------------------------------*/

	uint32 getFlags();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get currently set flags
	 ------------------------------------------------------*/

	static Material *getMaterial(index_t uid);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns material or 0x0 ( NULL )
	 ------------------------------------------------------*/

	const char *getName();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns Material's name or NULL
	 ------------------------------------------------------*/

	const char *getTextureName();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets Material's texture filename or NULL
	 ------------------------------------------------------*/

	uint32 getSerializeSize();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	index_t getUID();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns unique material index
	 ------------------------------------------------------*/

	virtual bool serialize(FreyjaFileWriter &w);
	/*------------------------------------------------------
	 * Pre  : Writes this material out to disk
	 * Post : Returns true on success
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void clearFlag(Flags flag);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Unsets passed flag
	 ------------------------------------------------------*/

	virtual bool serialize(FreyjaFileReader &r);
	/*------------------------------------------------------
	 * Pre  : Reads the material data from disk
	 * Post : Returns true on success
	 ------------------------------------------------------*/

	void setFlag(Flags flag);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets passed flag
	 ------------------------------------------------------*/

	void setName(const char *name);
	/*------------------------------------------------------
	 * Pre  : Name is valid string
	 * Post : Sets Material's name
	 ------------------------------------------------------*/

	void setTextureName(const char *name);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets Material's texture filename or NULL
	 ------------------------------------------------------*/

	const static uint32 mVersion = 2;

	const static uint32 mType = 0x5454414D;

	char mName[64];             /* Material name */

	uint32 mFlags;              /* Bit flags */

	int32 mParent;              /* Linked material id, for shader use */

	vec4_t mAmbient;            /* Ambient color */

	vec4_t mDiffuse;            /* Diffuse color */

	vec4_t mSpecular;           /* Specular color */

	vec4_t mEmissive;           /* Emissive color */

	vec_t mShininess;           /* Specular exponent */

	vec_t mTransparency;        /* Alpha 0.0 - 1.0 */

	vec4_t mColor;              /* Solid color */

	uint32 mBlendSrc;    		/* Blend source factor */

	uint32 mBlendDest;		   	/* Blend destination factor */

	int32 mTexture;             /* TextureData index */

	bool mHasAlphaChannel;      /* For depth sorting use */


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	char *mTextureName;         /* This is used for file I/O to map classes */

	index_t mUID;                         /* Unique identifier, key for pool */

	index_t mOldUID;                      /* UID when this was saved to disk */

	static Vector<Material *> mGobalPool; /* Storage for gobal access */
};

}

#endif
