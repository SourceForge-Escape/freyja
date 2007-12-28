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

#include <hel/math.h>
#include <mstl/SystemIO.h>
#include <mstl/String.h>
#include "PixelBuffer.h"
#include "XMLSerializer.h"


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

	Material(const Material& material);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of FreyjaMaterial
	 ------------------------------------------------------*/

	Material &operator=(const Material& material);
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

	bool operator < ( const Material& material ) const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sorting is used to automatically order
	 *        RenderList.  For example 'alpha' render pass is
	 *        always after 'solid'.
	 *
	 ------------------------------------------------------*/

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

	uint32 GetFlags() const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get currently set flags
	 ------------------------------------------------------*/

	uint32 GetId() const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns unique material id ( 1..N, or 0 if invalid )
	 ------------------------------------------------------*/

	const char* GetMetadata() const
	{ return mMetadata.c_str(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets metadata
	 ------------------------------------------------------*/

	const char* GetName() const
	{ return mName.c_str(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns Material's name or NULL
	 ------------------------------------------------------*/

	const char* GetShaderFilename() const
	{ return mShaderFilename.c_str(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets Material's shader filename or NULL
	 ------------------------------------------------------*/

	const char* GetTextureFilename() const
	{ return mTextureFilename.c_str(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets Material's texture filename or NULL
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void ClearFlag(Flags flag)
	{ mFlags &= ~flag; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Unsets passed flag
	 ------------------------------------------------------*/

	bool Serialize(XMLSerializerNode container);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool Unserialize(XMLSerializerNode container);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void SetFlag(Flags flag) { mFlags |= flag; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets passed flag
	 ------------------------------------------------------*/

	void SetName(const char* name);
	/*------------------------------------------------------
	 * Pre  : Name is valid string
	 * Post : Sets Material's name
	 ------------------------------------------------------*/

	void SetMetadata(const char* s)
	{ mMetadata = s; }
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

	const static uint32 mVersion = 4; /* Material version. */

	mstl::String mName;               /* Material name */

	mstl::String mFilename;           /* Material filename or NULL/empty string. */

	mstl::String mMetadata;           /* Useful for additional plugin material data. */

	mstl::String mShader;             /* Generated shader. */

	PixelBuffer* mDiffuseMap;            /* Various texture maps. */
	PixelBuffer* mEmissiveMap;
	PixelBuffer* mSpecularMap;
	PixelBuffer* mNormalMap;
	PixelBuffer* mHeightMap;
	PixelBuffer* mDecalMap;


	// 0.9.5 API

	mstl::String mBlendSrcString;     /* Blending source human readable string */

	mstl::String mBlendDestString;    /* Blending source human readable string */

	mstl::String mTextureFilename;    /* This is used for file I/O to map classes */

	mstl::String mShaderFilename;     /* This is used for file I/O to map classes */
};


} // End namespace freyja

#endif
