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

#ifndef GUARD__FREYJA_MATERIAL__H_
#define GUARD__FREYJA_MATERIAL__H_

#include <hel/math.h>
#include <mstl/SystemIO.h>
#include <mstl/String.h>
#include "PixelBuffer.h"
#include "XMLSerializer.h"


namespace freyja {

class Material :
		public XMLSerializer
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Material();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	Material( const Material& material );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Copy constructor.
	 *
	 ------------------------------------------------------*/

	Material &operator=(const Material& material);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Assignment operator. 
	 *
	 ------------------------------------------------------*/

	virtual ~Material();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Destructor.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	bool operator < ( const Material& material ) const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sorting is used to automatically order a
	 *        RenderList.  For example 'alpha' render pass is
	 *        always after 'solid'.
	 *
	 ------------------------------------------------------*/

	const char* GetMetadata() const
	{ return mMetadata.c_str(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	const char* GetName() const
	{ return mName.c_str(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	const char* GetShaderFilename() const
	{ return mShaderFilename.c_str(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	const char* GetFilename() const
	{ return mFilename.c_str(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void SetName(const char* name)
	{ mName = name; }
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

	void SetShaderFilename(const char *filename)
	{ mShaderFilename = filename; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void SetFilename(const char *filename)
	{ mFilename = filename; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static void (*LoadTextureCallback)( const char* filename ); //PixelBuffer* pixbuf );
	/*------------------------------------------------------
	 * Pre  : eg int load_texture(const char* filename)
	 *        must be implemented in caller.
	 *
	 * Post : Callback will return texture id assigned to
	 *        
	 *
	 ------------------------------------------------------*/

	static int (*LoadShaderCallback)( const char* filename );
	/*------------------------------------------------------
	 * Pre  : eg int load_shader(const char* filename)
	 *        must be implemented in caller.
	 *
	 * Post : Callback will return program id assigned to
	 *        shader.  FIXME replace with Shader class!
	 *
	 ------------------------------------------------------*/

	FREYJA_XMLSERIALIZER_INTERFACE
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : XmlSerializer implementation.
	 *
	 ------------------------------------------------------*/

	static freyja::Material* Cast( freyja_ptr ptr )
	{ return (freyja::Material*)ptr; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : FIXME Add RTTI check.
	 *
	 ------------------------------------------------------*/

	int16 GetDecalMapId( ) const
	{ return GetPixelBufferId( mDecalMap ); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns id or -1.
	 *
	 ------------------------------------------------------*/

	int16 GetSpecularMapId( ) const
	{ return GetPixelBufferId( mSpecularMap ); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns id or -1.
	 *
	 ------------------------------------------------------*/
 	
	int16 GetNormalMapId( ) const
	{ return GetPixelBufferId( mNormalMap ); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns id or -1.
	 *
	 ------------------------------------------------------*/
 	
	int16 GetHeightMapId( ) const
	{ return GetPixelBufferId( mHeightMap ); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns id or -1.
	 *
	 ------------------------------------------------------*/
 
	int16 GetEmissiveMapId( ) const
	{ return GetPixelBufferId( mEmissiveMap ); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns id or -1.
	 *
	 ------------------------------------------------------*/

	int16 GetShaderId( ) const
	{ return mShaderId; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns id or -1.
	 *
	 ------------------------------------------------------*/

 	const vec4_t& GetAmbientColor() const
	{ return mAmbient; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns RGBA ambient color value.
	 *
	 ------------------------------------------------------*/

 	const vec4_t& GetDiffuseColor() const
	{ return mDiffuse; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns RGBA diffuse color value.
	 *
	 ------------------------------------------------------*/

 	const vec4_t& GetSpecularColor() const
	{ return mSpecular; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns RGBA specular color value.
	 *
	 ------------------------------------------------------*/

 	const vec4_t& GetEmissiveColor() const
	{ return mEmissive; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns RGBA emissive color value.
	 *
	 ------------------------------------------------------*/

 	const vec_t GetShininess() const
	{ return mShininess; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns specular exponent.
	 *
	 ------------------------------------------------------*/

 	const vec_t GetTransparency() const
	{ return mTransparency; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns alpha ( 0.0f to 1.0f ).
	 *
	 ------------------------------------------------------*/


 protected:

	////////////////////////////////////////////////////////////
	// Protected
	////////////////////////////////////////////////////////////

	int16 GetPixelBufferId( PixelBuffer* pb ) const
	{ return (pb) ? pb->GetTextureId( ) : -1; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns id or -1.
	 *
	 ------------------------------------------------------*/


	//const static uint32 mVersion = 4; /* Material version. */

	mstl::String mName;               /* Material name */

	mstl::String mFilename;           /* Material filename or NULL/empty string. */

	mstl::String mShaderFilename;     /* GLSL / ARB / etc shader filename. */

	bool mAlpha;                      /* Mostly for depth sorting use. */

	int32 mShaderId;                  /* Shader program index. */

	mstl::String mMetadata;           /* Useful for additional plugin material data. */

	mstl::String mShader;             /* Generated shader. */

	mstl::String mBlendSrc;           /* Blending source string. */

	mstl::String mBlendDest;          /* Blending destination string. */

	vec4_t mAmbient;                  /* Ambient color. */

	vec4_t mDiffuse;                  /* Diffuse color. */

	vec4_t mSpecular;                 /* Specular color. */

	vec4_t mEmissive;                 /* Emissive color. */

	vec_t mShininess;                 /* Specular exponent. */

	vec_t mTransparency;              /* Alpha 0.0f to 1.0f. */

	PixelBuffer* mDiffuseMap;         /* Various specific texture maps. */
	PixelBuffer* mDecalMap;
	PixelBuffer* mEmissiveMap;
	PixelBuffer* mHeightMap;
	PixelBuffer* mNormalMap;
	PixelBuffer* mSpecularMap;
};


inline
const char* Material::GetType() const
{ return "Material"; }

 
inline
uint32 Material::GetVersion() const
{ return 10; }


} // namespace freyja

#endif // GUARD__FREYJA_MATERIAL__H_
