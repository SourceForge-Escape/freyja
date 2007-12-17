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

#ifndef GUARD__FREYJA_MATERIAL_H_
#define GUARD__FREYJA_MATERIAL_H_

#if TINYXML_FOUND
#   include <tinyxml/tinyxml.h>
#endif

#include <hel/math.h>
#include <mstl/SystemIO.h>
#include <mstl/String.h>
#include "Texture.h"

using namespace mstl;


namespace freyja {

class Material
{
 public:

	typedef enum {
		fMaterial_DetailTexure = 1,		
	} Flags;


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Material();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of FreyjaMaterial
	 ------------------------------------------------------*/

	Material(const Material &fm);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of FreyjaMaterial
	 ------------------------------------------------------*/

	Material &operator=(const Material &fm);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	virtual ~Material();
	/*------------------------------------------------------
	 * Pre  : FreyjaMaterial object is allocated
	 * Post : Deconstructs an object of FreyjaMaterial
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

   static int32 GetBlendIndex(int blend);
	/*------------------------------------------------------
	 * Pre  : Pass blend value, Built with HAVE_GL
	 * Post : Returns -1 if not used, or index if used
	 ------------------------------------------------------*/

	static uint32 GetCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns number of unique materials
	 ------------------------------------------------------*/

	uint32 GetFlags();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get currently set flags
	 ------------------------------------------------------*/

	uint32 GetId();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns unique material id ( 1..N, or 0 if invalid )
	 ------------------------------------------------------*/

	const char *GetMetaData() { return mMetaData.c_str(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets metadata
	 ------------------------------------------------------*/

	const char *GetName() { return mName.c_str(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns Material's name or NULL
	 ------------------------------------------------------*/

	const char *GetShaderFilename();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets Material's shader filename or NULL
	 ------------------------------------------------------*/

	const char *GetTextureFilename();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets Material's texture filename or NULL
	 ------------------------------------------------------*/

	uint32 GetSerializeSize();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	virtual bool Serialize(SystemIO::FileWriter &w);
	/*------------------------------------------------------
	 * Pre  : Writes this material out to disk
	 * Post : Returns true on success
	 ------------------------------------------------------*/

	virtual bool Serialize(SystemIO::TextFileWriter &w);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Serializes the to diskfile as a chunk
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void ClearFlag(Flags flag) { mFlags &= ~flag; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Unsets passed flag
	 ------------------------------------------------------*/

#if TINYXML_FOUND
	bool Serialize(TiXmlElement *container);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool Unserialize(TiXmlElement *container);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/
#endif

	void SetFlag(Flags flag) { mFlags |= flag; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets passed flag
	 ------------------------------------------------------*/

	void SetName(const char *name);
	/*------------------------------------------------------
	 * Pre  : Name is valid string
	 * Post : Sets Material's name
	 ------------------------------------------------------*/

	void SetMetaData(const char *s) { mMetaData = s; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets metadata
	 ------------------------------------------------------*/

	void SetShaderFilename(const char *name);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets Material's shader filename or NULL
	 ------------------------------------------------------*/

	void SetTextureFilename(const char *name);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets Material's texture filename or NULL
	 ------------------------------------------------------*/

	int32 mId;                  /* Unique identifier */

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

	int32 mShaderId;            /* Shader index */

	bool mHasAlphaChannel;      /* For depth sorting use */

	// int load_texture(const char *filename)
	static int (*mLoadTextureFunc)(const char *filename);

	// int load_shader(const char *filename)
	static int (*mLoadShaderFunc)(const char *filename);


 protected:

	////////////////////////////////////////////////////////////
	// Protected
	////////////////////////////////////////////////////////////

	const static uint32 mVersion = 3;

	String mName;               /* Material name */

	String mBlendSrcString;     /* Blending source human readable string */

	String mBlendDestString;    /* Blending source human readable string */

	String mTextureFilename;    /* This is used for file I/O to map classes */

	String mShaderFilename;     /* This is used for file I/O to map classes */

	String mMetaData;           /* Useful for additional plugin material data */
};

} // End namespace freyja

#endif
