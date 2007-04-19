/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : MaterialEv
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

#ifndef GUARD__FREYJA3D_MATERIALEV_H_
#define GUARD__FREYJA3D_MATERIALEV_H_

#include <hel/math.h>
#include <hel/Ray.h>
#include <mstl/stack.h>

#include "Control.h"


namespace freyja3d {

class MaterialEv : public Control
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

	MaterialEv();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor for MaterialEv.
	 *
	 ------------------------------------------------------*/

	~MaterialEv();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Deconstructor for MaterialEv.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

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

	uint32 eSelectId;
	void eSelect(uint32 value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Event for selecting current material by index.
	 *
	 ------------------------------------------------------*/

	uint32 eAmbientId[4];
	void eAmbient0(vec_t value) { SetAmbient(0, value); }
	void eAmbient1(vec_t value) { SetAmbient(1, value); }
	void eAmbient2(vec_t value) { SetAmbient(2, value); }
	void eAmbient3(vec_t value) { SetAmbient(3, value); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Events for setting ambient material values.
	 *
	 ------------------------------------------------------*/

	uint32 eDiffuseId[4];
	void eDiffuse0(vec_t value) { SetDiffuse(0, value); }
	void eDiffuse1(vec_t value) { SetDiffuse(1, value); }
	void eDiffuse2(vec_t value) { SetDiffuse(2, value); }
	void eDiffuse3(vec_t value) { SetDiffuse(3, value); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Events for setting diffuse material values.
	 *
	 ------------------------------------------------------*/

	uint32 eEmissiveId[4];
	void eEmissive0(vec_t value) { SetEmissive(0, value); }
	void eEmissive1(vec_t value) { SetEmissive(1, value); }
	void eEmissive2(vec_t value) { SetEmissive(2, value); }
	void eEmissive3(vec_t value) { SetEmissive(3, value); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Events for setting emissive material values.
	 *
	 ------------------------------------------------------*/

	uint32 eSpecularId[4];
	void eSpecular0(vec_t value) { SetSpecular(0, value); }
	void eSpecular1(vec_t value) { SetSpecular(1, value); }
	void eSpecular2(vec_t value) { SetSpecular(2, value); }
	void eSpecular3(vec_t value) { SetSpecular(3, value); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Events for setting specular material values.
	 *
	 ------------------------------------------------------*/

	uint32 eShineId;
	void eShine(vec_t value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Events for setting material shine value.
	 *
	 ------------------------------------------------------*/

	uint32 eSetNameId;
	void eSetName(char *text);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Event to set 'human readable' material name.
	 *
	 ------------------------------------------------------*/

	void eBlendSrc(uint32 value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Event to set material blend source.
	 *
	 ------------------------------------------------------*/

	void eBlendDest(uint32 value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Event to set material blend destination.
	 *
	 ------------------------------------------------------*/

	void eEnableDetailTexture(uint32 value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Event to toggle material detail texture.
	 *
	 ------------------------------------------------------*/

	void eEnableNormalize(uint32 value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Event to toggle material normailzation.
	 *
	 ------------------------------------------------------*/

	uint32 eEnableBlendingId;
	void eEnableBlending(uint32 value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Event to toggle material blending.
	 *
	 ------------------------------------------------------*/

	uint32 eEnableTextureId;
	void eEnableTexture(uint32 value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Event to toggle material texture rendering.
	 *
	 ------------------------------------------------------*/

	void eNewMaterial();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Handle event for create new material.
	 *
	 ------------------------------------------------------*/

	void eOpenMaterial(char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Handle event for unserialize material.
	 *
	 ------------------------------------------------------*/

	void eSaveMaterial(char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Handle event for serialize material.
	 *
	 ------------------------------------------------------*/

	uint32 eSetShaderFilenameId;
	void eSetShaderFilename(char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Handle event for shader filename.
	 *
	 ------------------------------------------------------*/

	uint32 eSetShaderId;
	void eSetShader(uint32 value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Handle event for active shader change.
	 *
	 ------------------------------------------------------*/

	uint32 eSetTextureId;
	void eSetTexture(uint32 value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Handle event for active texture change.
	 *
	 ------------------------------------------------------*/

	void eTextureSlotLoad(uint32 i)
	{
		SetFlag(fLoadTextureInSlot, i);
		Print("Texture load into current slot [%s]", i ? "on" : "off");
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Handle event for active texture change.
	 *
	 ------------------------------------------------------*/

	void eMaterialSlotLoad(uint32 i)
	{
		SetFlag(fLoadMaterialInSlot, i);
		Print("Material load into slot is [%s]", i ? "ON" : "OFF");
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Handle event for active texture change.
	 *
	 ------------------------------------------------------*/

	void eTextureUpload(uint32 id);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Handle event for texture loading from freyja buffer.
	 *
	 ------------------------------------------------------*/

	void eOpenTexture(char *text);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Handle event for texture loading from filedialog.
	 *
	 ------------------------------------------------------*/

	void eOpenShader(char *text);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Handle event for shader loading from filedialog.
	 *
	 ------------------------------------------------------*/

	uint32 eGLSLFragmentModeId;
	void eGLSLFragmentMode(uint32 value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Load only GLSL shaders in shader slot.
	 *
	 ------------------------------------------------------*/

	uint32 eARBFragmentModeId;
	void eARBFragmentMode(uint32 value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Load only ARB shaders in shader slot.
	 *
	 ------------------------------------------------------*/

	void eNop() { Print("MaterialEv::Nop() Called."); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : NOP.
	 *
	 ------------------------------------------------------*/

	static MaterialEv *mHack;  // FIXME: Stopgap until refactor is done 


 private:

	typedef void (MaterialEv::*MethodPtr)();
	typedef void (MaterialEv::*MethodPtr1f)(vec_t);
	typedef void (MaterialEv::*MethodPtr1s)(char *);
	typedef void (MaterialEv::*MethodPtr1u)(uint32);

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

	uint32 CreateListener(const char *name, MethodPtr1f ptr);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Bind a MethodDelegate to an external event.
	 *        Returns event Id.
	 *
	 ------------------------------------------------------*/

	uint32 CreateListener(const char *name, MethodPtr1s ptr);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Bind a MethodDelegate to an external event.
	 *        Returns event Id.
	 *
	 ------------------------------------------------------*/

	uint32 CreateListener(const char *name, MethodPtr1u ptr);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Bind a MethodDelegate to an external event.
	 *        Returns event Id.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	void SetFlag(options_t flag, bool b)
	{
		if (b) {
			mFlags |= flag;
		}
		else {
			mFlags &= ~flag;
		}
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Pretty interface for setting bitflags.
	 *
	 ------------------------------------------------------*/

	uint32 mFlags;           /* Options for this class */

	int32 mTextureId;        /* Currently active texture slot. */
};

} // namespace freyja3d 

#endif // GUARD__FREYJA3D_MATERIAL_H_
