/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : MaterialControl
 * License : No use w/o permission, (C) 2000-2007 Mongoose
 * Comments: This class is an event control for backend materials, and
 *           the related user interface signals.
 *
 * 
 *-- History ------------------------------------------------- 
 *
 * 2007.04.10:
 * Mongoose - Created, reformed from various events and such.
 ==========================================================================*/

#ifndef GUARD__FREYJA3D_MATERIALCONTROL_H_
#define GUARD__FREYJA3D_MATERIALCONTROL_H_

#include <hel/math.h>
#include <hel/Ray.h>
#include <mstl/stack.h>
#include <freyja/Material.h>

#include "Control.h"
#include "RecentFiles.h"


namespace freyja3d {

class MaterialControl : public Control
{
 public:

	/* FreyjaControl options */
	typedef enum {
		fNone                = 0,
		fLoadMaterialInSlot  = 1,
		fLoadTextureInSlot   = 2,
		fUsingARBFragments   = 4

	} options_t;


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	static MaterialControl* GetInstance( ) 
	{ return mInstance ? mInstance : ( mInstance = new MaterialControl() ); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : "Singleton" Constructor for MaterialControl.
	 *
	 ------------------------------------------------------*/

	~MaterialControl( )
	{ mInstance = NULL; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Deconstructor for MaterialControl.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	byte *GenerateTextureCheckerBoard(byte bg[4], byte fg[4],
									  uint32 width, uint32 height,
									  uint32 runlen);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	int32 GetTextureId() { return mTextureId; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get the current active texture slot.
	 *
	 ------------------------------------------------------*/

	bool LoadMaterial(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Unserializes current material and its metadata. 
	 *
	 ------------------------------------------------------*/

	void RefreshInterface();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Completely reset material interface to 
	 *        match backend values.
	 *        Useful for changing materials in the editor.
	 *
	 ------------------------------------------------------*/

	bool SaveMaterial(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Serializes current material and its metadata.
	 *
	 ------------------------------------------------------*/

	void SetAmbient(uint32 i, vec_t value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set ambient material value per component.
	 *
	 ------------------------------------------------------*/

	void SetDiffuse(uint32 i, vec_t value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set diffuse material value per component.
	 *
	 ------------------------------------------------------*/

	void SetEmissive(uint32 i, vec_t value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set emissive material value per component.
	 *
	 ------------------------------------------------------*/

	void SetSpecular(uint32 i, vec_t value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set specular material value per component.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void AttachMethodListeners();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Hooks up MethodDelegates to the event system.
	 ------------------------------------------------------*/

	bool InitRecentFilesMenu();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Generates recent files menu data model.
	 *
	 ------------------------------------------------------*/

	bool LoadTexture(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if sucessful.
	 *
	 ------------------------------------------------------*/

	bool LoadTextureBuffer(byte *image, uint32 width, uint32 height, 
						   uint32 bpp, uint32 colortype);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if sucessful.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Properities 
	////////////////////////////////////////////////////////////

	uint32 mSelected;
	uint32 GetSelected() { return mSelected; }
	void SetSelected(uint32 i) { mSelected = i; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns currently selected material index.
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Events 
	////////////////////////////////////////////////////////////

	uint32 EvSelectId;
	void EvSelect(uint32 value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Event for selecting current material by index.
	 *
	 ------------------------------------------------------*/

	uint32 EvAmbientId[4];
	void EvAmbient0(vec_t value) { SetAmbient(0, value); }
	void EvAmbient1(vec_t value) { SetAmbient(1, value); }
	void EvAmbient2(vec_t value) { SetAmbient(2, value); }
	void EvAmbient3(vec_t value) { SetAmbient(3, value); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Events for setting ambient material values.
	 *
	 ------------------------------------------------------*/

	uint32 EvDiffuseId[4];
	void EvDiffuse0(vec_t value) { SetDiffuse(0, value); }
	void EvDiffuse1(vec_t value) { SetDiffuse(1, value); }
	void EvDiffuse2(vec_t value) { SetDiffuse(2, value); }
	void EvDiffuse3(vec_t value) { SetDiffuse(3, value); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Events for setting diffuse material values.
	 *
	 ------------------------------------------------------*/

	uint32 EvEmissiveId[4];
	void EvEmissive0(vec_t value) { SetEmissive(0, value); }
	void EvEmissive1(vec_t value) { SetEmissive(1, value); }
	void EvEmissive2(vec_t value) { SetEmissive(2, value); }
	void EvEmissive3(vec_t value) { SetEmissive(3, value); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Events for setting emissive material values.
	 *
	 ------------------------------------------------------*/

	uint32 EvSpecularId[4];
	void EvSpecular0(vec_t value) { SetSpecular(0, value); }
	void EvSpecular1(vec_t value) { SetSpecular(1, value); }
	void EvSpecular2(vec_t value) { SetSpecular(2, value); }
	void EvSpecular3(vec_t value) { SetSpecular(3, value); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Events for setting specular material values.
	 *
	 ------------------------------------------------------*/

	uint32 EvShineId;
	void EvShine(vec_t value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Events for setting material shine value.
	 *
	 ------------------------------------------------------*/

	uint32 EvSetNameId;
	void EvSetName(char *text);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Event to set 'human readable' material name.
	 *
	 ------------------------------------------------------*/

	void EvBlendSrc(uint32 value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Event to set material blend source.
	 *
	 ------------------------------------------------------*/

	void EvBlendDest(uint32 value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Event to set material blend destination.
	 *
	 ------------------------------------------------------*/

	void EvEnableDetailTexture(uint32 value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Event to toggle material detail texture.
	 *
	 ------------------------------------------------------*/

	void EvEnableNormalize(uint32 value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Event to toggle material normailzation.
	 *
	 ------------------------------------------------------*/

	uint32 EvEnableBlendingId;
	void EvEnableBlending(uint32 value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Event to toggle material blending.
	 *
	 ------------------------------------------------------*/

	uint32 EvEnableTextureId;
	void EvEnableTexture(uint32 value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Event to toggle material texture rendering.
	 *
	 ------------------------------------------------------*/

	void EvNewMaterial();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Handle event for create new material.
	 *
	 ------------------------------------------------------*/

	void EvOpenMaterial();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Handle event for unserialize material.
	 *        Opens file dialog for user input.
	 *
	 ------------------------------------------------------*/

	void EvSaveMaterial();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Handle event for serialize material.
	 *        Opens file dialog for user input.
	 *
	 ------------------------------------------------------*/

	uint32 EvSetTextureNameId;
	void EvSetTextureName(char *text);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Handle event for texture name/desc/filename.
	 *
	 ------------------------------------------------------*/

	uint32 EvSetShaderFilenameId;
	void EvSetShaderFilename(char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Handle event for shader filename.
	 *
	 ------------------------------------------------------*/

	uint32 EvSetShaderId;
	void EvSetShader(uint32 value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Handle event for active shader change.
	 *
	 ------------------------------------------------------*/

	uint32 EvSetTextureId;
	void EvSetTexture(uint32 value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set the current texel unit to assign texture maps 
	 *        to for the current material.
	 *
	 ------------------------------------------------------*/

	void EvTextureSlotLoad(uint32 i);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : If i == 0, disable the current texel unit in 
	 *        the current material.
	 *
	 ------------------------------------------------------*/

	void EvMaterialSlotLoad(uint32 i)
	{
		SetFlag(fLoadMaterialInSlot, i);
		Print("Material load into slot is [%s]", i ? "ON" : "OFF");
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Handle event for active texture change.
	 *
	 ------------------------------------------------------*/

	void EvTextureUpload(uint32 id);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Handle event for texture loading from freyja buffer.
	 *
	 ------------------------------------------------------*/

	void EvOpenTexture(char *text);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Handle event for texture loading from filedialog.
	 *
	 ------------------------------------------------------*/

	void EvOpenShader(char *text);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Handle event for shader loading from filedialog.
	 *
	 ------------------------------------------------------*/

	void EvRecentFile(uint32 value) 
	{ LoadMaterial( mRecent.GetFilename(value) ); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Load a file from the recently used menu.
	 *
	 ------------------------------------------------------*/

	uint32 EvGLSLFragmentModeId;
	void EvGLSLFragmentMode(uint32 value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Load only GLSL shaders in shader slot.
	 *
	 ------------------------------------------------------*/

	uint32 EvARBFragmentModeId;
	void EvARBFragmentMode(uint32 value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Load only ARB shaders in shader slot.
	 *
	 ------------------------------------------------------*/

	void EvNop() { Print("MaterialControl::Nop() Called."); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : NOP.
	 *
	 ------------------------------------------------------*/

	freyja_ptr GetCurrentMaterialPtr( ) const 
	{ return (mCurrentMaterial) ? mCurrentMaterial->ToPtr() : NULL; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	freyja::Material* GetCurrentMaterial( ) const 
	{ return mCurrentMaterial; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void SetCurrentMaterial( freyja::Material* material ) 
	{ mCurrentMaterial = material; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	int16 GetBlendId( const char* name );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	const char* GetBlendString( uint32 value );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/


protected:

	MaterialControl( );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor for MaterialControl.
	 *
	 ------------------------------------------------------*/


 private:

	typedef void (MaterialControl::*MethodPtr)();
	typedef void (MaterialControl::*MethodPtr1f)(vec_t);
	typedef void (MaterialControl::*MethodPtr1s)(char *);
	typedef void (MaterialControl::*MethodPtr1u)(uint32);

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////

	uint32 CreateListener(const char *name, MethodPtr ptr);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Bind a MethodDelegate to an external event.
	 *        Returns event Id.
	 *
	 ------------------------------------------------------*/

	uint32 CreateListener1f(const char *name, MethodPtr1f ptr);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Bind a MethodDelegate to an external event.
	 *        Returns event Id.
	 *
	 ------------------------------------------------------*/

	uint32 CreateListener1s(const char *name, MethodPtr1s ptr);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Bind a MethodDelegate to an external event.
	 *        Returns event Id.
	 *
	 ------------------------------------------------------*/

	uint32 CreateListener1u(const char *name, MethodPtr1u ptr);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Bind a MethodDelegate to an external event.
	 *        Returns event Id.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	void SetFlag(options_t flag, bool b);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Pretty interface for setting bitflags.
	 *
	 ------------------------------------------------------*/

	static MaterialControl *mInstance;  /* "Singleton" instance. */

	freyja::Material* mCurrentMaterial;

	uint32 mFlags;                      /* Options for this class */

	int32 mTextureId;                   /* Currently active texture slot. */

	int32 mTextureMap;

	RecentFiles mRecent;                /* Recent material files lists. */
};


////////////////////////////////////////////////////////////
// Inline Private Mutators
////////////////////////////////////////////////////////////

inline
void MaterialControl::SetFlag(options_t flag, bool b)
{
	if (b) 
	{
		mFlags |= flag;
	}
	else 
	{
		mFlags &= ~flag;
	}
}


inline
bool MaterialControl::InitRecentFilesMenu()
{
	return mRecent.LoadResource();
}



} // namespace freyja3d 

#endif // GUARD__FREYJA3D_MATERIALCONTROL_H_
