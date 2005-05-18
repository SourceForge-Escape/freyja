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
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_FREYJAMATERIAL_H_
#define GUARD__FREYJA_MONGOOSE_FREYJAMATERIAL_H_

#include <hel/math.h>

#include "FreyjaTexture.h"
#include "FreyjaFileReader.h"
#include "FreyjaFileWriter.h"


class FreyjaMaterial
{
 public:

	enum Flags
	{
		fMaterial_DetailTexure = 1,		
	};


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	FreyjaMaterial();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of FreyjaMaterial
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.01.08: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual ~FreyjaMaterial();
	/*------------------------------------------------------
	 * Pre  : FreyjaMaterial object is allocated
	 * Post : Deconstructs an object of FreyjaMaterial
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.01.08: 
	 * Mongoose - Created
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
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.07.03:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	uint32 getId();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns unique material id ( 1..N, or 0 if invalid )
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.22:
	 *  Mongoose - Created
	 ------------------------------------------------------*/

	const char *getName();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns Material's name or NULL
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.21:
	 *  Mongoose - Created
	 ------------------------------------------------------*/

	const char *getTextureName();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets Material's texture filename or NULL
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.21:
	 *  Mongoose - Created
	 ------------------------------------------------------*/

	const char *getDetailTextureName();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets Material's detail texture filename or NULL
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.07.03:
	 *  Mongoose - Created
	 ------------------------------------------------------*/

	virtual bool serialize(FreyjaFileWriter &w);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
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
	 * Pre  : 
	 * Post : 
	 *
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

	int32 mId;                  /* Unique identifier */

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

	const static uint32 mVersion = 0x0001;
};

#endif
