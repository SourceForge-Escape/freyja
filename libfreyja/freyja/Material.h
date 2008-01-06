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

	Material( const char* name );
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

	const char* GetBlendDest( )
	{ return mBlendDest.c_str(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	const char* GetBlendSrc( )
	{ return mBlendSrc.c_str(); }
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

	void SetBlendDest( const char* factor )
	{ mBlendDest = factor; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void SetBlendSrc( const char* factor )
	{ mBlendSrc = factor; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
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

	int16 GetTexture0Id( ) const
	{ return mTexture0Id; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns id or -1.
	 *
	 ------------------------------------------------------*/

	void SetTexture0Id( int16 id )
	{ mTexture0Id = id; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Set to -1 to disable.
	 *
	 ------------------------------------------------------*/

	int16 GetTexture1Id( ) const
	{ return mTexture1Id; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns id or -1.
	 *
	 ------------------------------------------------------*/

	void SetTexture1Id( int16 id )
	{ mTexture1Id = id; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Set to -1 to disable.
	 *
	 ------------------------------------------------------*/

	int16 GetTexture2Id( ) const
	{ return mTexture2Id; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns id or -1.
	 *
	 ------------------------------------------------------*/

	void SetTexture2Id( int16 id )
	{ mTexture2Id = id; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Set to -1 to disable.
	 *
	 ------------------------------------------------------*/

	int16 GetTexture3Id( ) const
	{ return mTexture3Id; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns id or -1.
	 *
	 ------------------------------------------------------*/

	void SetTexture3Id( int16 id )
	{ mTexture3Id = id; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Set to -1 to disable.
	 *
	 ------------------------------------------------------*/

	int16 GetTexture4Id( ) const
	{ return mTexture4Id; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns id or -1.
	 *
	 ------------------------------------------------------*/

	void SetTexture4Id( int16 id )
	{ mTexture4Id = id; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Set to -1 to disable.
	 *
	 ------------------------------------------------------*/

	int16 GetShaderId( ) const
	{ return mShaderId; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns id or -1.
	 *
	 ------------------------------------------------------*/

	void SetShaderId( int16 id )
	{ mShaderId = id; }
	/*------------------------------------------------------
	 * Pre  : Set to -1 if no shader is assigned. 
	 * Post : 
	 *
	 ------------------------------------------------------*/

 	const vec4_t& GetAmbientColor() const
	{ return mAmbient; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns RGBA ambient color value.
	 *
	 ------------------------------------------------------*/

 	void SetAmbientColor( const vec4_t color )
	{ helCopyVec4( color, mAmbient ); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *
	 ------------------------------------------------------*/

 	const vec4_t& GetDiffuseColor() const
	{ return mDiffuse; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns RGBA diffuse color value.
	 *
	 ------------------------------------------------------*/

 	void SetDiffuseColor( const vec4_t color )
	{ helCopyVec4( color, mDiffuse ); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *
	 ------------------------------------------------------*/

 	const vec4_t& GetSpecularColor() const
	{ return mSpecular; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns RGBA specular color value.
	 *
	 ------------------------------------------------------*/

 	void SetSpecularColor( const vec4_t color )
	{ helCopyVec4( color, mSpecular ); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *
	 ------------------------------------------------------*/

 	const vec4_t& GetEmissiveColor() const
	{ return mEmissive; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns RGBA emissive color value.
	 *
	 ------------------------------------------------------*/

 	void SetEmissiveColor( const vec4_t color )
	{ helCopyVec4( color, mEmissive ); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *
	 ------------------------------------------------------*/

 	const vec_t GetShininess() const
	{ return mShininess; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns specular exponent.
	 *
	 ------------------------------------------------------*/

 	void SetShininess( const vec_t exp )
	{ mShininess = exp; }
	/*------------------------------------------------------
	 * Pre  : Specular exponent. 
	 * Post : 
	 *
	 ------------------------------------------------------*/

 	const vec_t GetTransparency() const
	{ return mTransparency; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns alpha ( 0.0f to 1.0f ).
	 *
	 ------------------------------------------------------*/

 	void SetTransparency( const vec_t transparency )
	{ mTransparency = transparency; }
	/*------------------------------------------------------
	 * Pre  : Alpha ( 0.0f to 1.0f ).
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void EnableBlending( const bool t )
	{ mBlending = t; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	const bool IsBlendingEnabled( ) const
	{ return mBlending; }
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

	freyja_ptr ToPtr( )
	{ return (freyja_ptr)this; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : FIXME Add RTTI check.
	 *
	 ------------------------------------------------------*/

	static freyja::Material* Cast( freyja_ptr ptr )
	{ return (freyja::Material*)ptr; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : FIXME Add RTTI check.
	 *
	 ------------------------------------------------------*/

	static freyja::Material* Import( const char* filename );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Import material from file.
	 *
	 ------------------------------------------------------*/
	

 protected:

	////////////////////////////////////////////////////////////
	// Protected
	////////////////////////////////////////////////////////////

	int16 GetPixelBufferId( PixelBuffer* pb, int16 id = -1 ) const
	{ return (pb) ? pb->GetTextureId( ) : id; }
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

	bool mBlending;

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

	int16 mTexture0Id;                /* For use of generated textures. */
	int16 mTexture1Id;
	int16 mTexture2Id;
	int16 mTexture3Id;
	int16 mTexture4Id;
};


inline
const char* Material::GetType() const
{ return "Material"; }

 
inline
uint32 Material::GetVersion() const
{ return 10; }


} // namespace freyja

#endif // GUARD__FREYJA_MATERIAL__H_
