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
#include <mstl/Vector.h>
#include <mstl/String.h>
#include "FreyjaTexture.h"


namespace freyja {

class Material
{
 public:

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

	uint32 GetId();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns unique material id ( 1..N, or 0 if invalid )
	 ------------------------------------------------------*/

	const char* GetMetaData() //const
	{ return mMetaData.c_str(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets metadata
	 ------------------------------------------------------*/

	const char* GetName() //const
	{ return mName.c_str(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns Material's name or NULL
	 ------------------------------------------------------*/

	int16 GetMultiTexture( const uint16 i ) const
	{ return ( i < mMultiTextureIds.size() ) ? mMultiTextureIds[i] : -1; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	const char* GetShaderFilename() //const
	{ return mShaderFilename.c_str(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets Material's shader filename or NULL
	 ------------------------------------------------------*/

	const char* GetTextureFilename() //const
	{ return mTextureFilename.c_str(); }
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

	void DisableMultitexture()
	{ mMultitexture = false; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void EnableMultitexture()
	{ mMultitexture = true; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	const bool IsMultitextureEnabled() const
	{ return mMultitexture; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void DisableBlending()
	{ mBlending = false; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void EnableBlending()
	{ mBlending = true; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	const bool IsBlendingEnabled() const
	{ return mBlending; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	virtual bool Serialize(SystemIO::TextFileReader &r);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Serializes from diskfile
	 ------------------------------------------------------*/

	virtual bool Serialize(SystemIO::FileReader &r);
	/*------------------------------------------------------
	 * Pre  : Reads the material data from disk
	 * Post : Returns true on success
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

	void SetName(const char *name)
	{ if (name && name[0]) mName = name; }
	/*------------------------------------------------------
	 * Pre  : Name is valid string
	 * Post : Sets Material's name
	 ------------------------------------------------------*/

	void SetMetaData(const char *s) 
	{ mMetaData = s; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets metadata
	 ------------------------------------------------------*/

	void SetMultiTextureName( const uint16 i, const char* name )
	{ MARK_MSG("Not implemented."); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void SetMultiTexture( const uint16 i, const uint16 id )
	{ if ( i < mMultiTextureIds.size() ) mMultiTextureIds[i] = id; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void SetShaderFilename(const char *name)
	{ if (name && name[0]) mShaderFilename = name; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets Material's shader filename or NULL
	 ------------------------------------------------------*/

	void SetTextureFilename(const char *name)
	{ if (name && name[0]) mTextureFilename = name; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets Material's texture filename or NULL
	 ------------------------------------------------------*/

	uint32 GetFlags();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Legacy C ABI flags support.
	 ------------------------------------------------------*/

	void SetFlags( uint32 flags );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Legacy C ABI flags support.
	 ------------------------------------------------------*/

	const static uint32 mVersion = 3;

	int32 mId;                  /* Unique identifier */

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
	// Protected Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Protected Mutators
	////////////////////////////////////////////////////////////

	bool mMultitexture;               /* Multitexture use state. */

	bool mBlending;                   /* Blending use state. */

	mstl::String mName;               /* Material name. */

	mstl::String mBlendSrcString;     /* Blending source human readable string */

	mstl::String mBlendDestString;    /* Blending source human readable string */

	mstl::String mTextureFilename;    /* This is used for file I/O to map classes */

	mstl::String mShaderFilename;     /* This is used for file I/O to map classes */

	mstl::String mMetaData;           /* Useful for additional plugin material data */

	mstl::Vector<int16> mMultiTextureIds;
};

} // End namespace freyja

#endif
