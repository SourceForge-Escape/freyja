/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : ttp://icculus.org/freyja/
 * Email   : mongoose@icculus.org
 * Object  : FreyjaControl
 * License : No use w/o permission (C) 2000 Mongoose
 * Comments: This is the controler class for the client
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 *
 *-- TODO --------------------------------------------------- 
 *
 * + Command pattern event refactoring
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.08.14:
 * Mongoose - Documentation update and Freyja coding guidelines ( finally )
 *
 * 2000.09.10:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__FREYJA3D_FREYJACONTROL_H_
#define GUARD__FREYJA3D_FREYJACONTROL_H_

#include <freyja/ModelABI.h>
#include <freyja/MeshABI.h>
#include <freyja/BoneABI.h>
#include <mstl/ActionManager.h>
#include <mstl/String.h>
#include <mstl/SystemIO.h>
#include <mstl/Vector.h>

#include "freyja_events.h"
#include "FreyjaRender.h"
#include "Texture.h"
#include "Cursor.h"
#include "Control.h"
#include "MaterialEv.h"


namespace freyja3d {

class FreyjaControl : public Control
{
 public:

	/* Each of these control schemes will have it's own control class later */
	typedef enum {        
		eScheme_UV = 0,
		eScheme_Model,
		eScheme_Material,
		eScheme_Animation
		
	} ControlScheme;


	/* Editor modes */
	typedef enum {
		tNone = 0,
		tPoint,
		tFace,
		tMesh,
		tModel,
		tBone,
		tSkeleton,
		tKeyFrame,
		tScene,
		tLight,
		tCamera,
		tSelectedVertices,
		tSelectedFaces,
		tSelectedBones,
		tSelectedMeshes,
		tControlPoint
	} object_type_t;


	/* Editor action modes - mostly for generic submodes */
	typedef enum {

		/* Generic transforms */
		aNone = 1,
		aMove,
		aRotate,
		aScale,
		aSelect,
		aSelectByBox,
		aUnselect,
		aKeyframe,
		aInfo,

		/* Specialized actions */
		aVertexNew,
		aVertexDelete,
		aVertexCombine,
		aTexcoordCombine
		
	} action_type_t;


	/* FreyjaControl options */
	typedef enum {
		fNone                = 0,
		fPolyMappedTexCoords = 1,
		fDeformBoneVertices  = 2

	} options_t;


	typedef void (FreyjaControl::*MethodPtr)();
	typedef bool (FreyjaControl::*MethodPtrB)();
	typedef void (FreyjaControl::*MethodPtr1u)(unsigned int);
	typedef void (FreyjaControl::*MethodPtr1f)(float);
	typedef void (FreyjaControl::*MethodPtr1s)(char *);
	typedef void (FreyjaControl::*MethodPtr2s)(char *, char *);


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	static FreyjaControl *GetInstance() 
	{ return (mInstance ? mInstance : mInstance = new FreyjaControl()); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : FreyjaControl object is constructed
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~FreyjaControl();
	/*------------------------------------------------------
	 * Pre  : FreyjaControl object is allocated
	 * Post : FreyjaControl object is deconstructed
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Properties
	////////////////////////////////////////////////////////////

	uint32 mGroupBitmap;

	ControlScheme GetControlScheme() { return mControlScheme; }
	void SetControlScheme(ControlScheme scheme) { mControlScheme = scheme; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Alters the way input is handled for this control
	 ------------------------------------------------------*/

	uint32 GetFaceEdgeCount() { return mFaceEdgeCount; }
	void SetFaceEdgeCount(uint32 i) { if (i>2) mFaceEdgeCount = i; }
	uint32 mFaceEdgeCount;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns face 'edge count' for draw poly from 
	 *        points interface.  ( An odd, yet popular feature. ) 
	 ------------------------------------------------------*/

	const hel::Vec3 &GetSceneTranslation() { return mSceneTrans; }
	void SetSceneTranslation(const hel::Vec3 &v) { mSceneTrans = v; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns xyz offset of the scene
	 ------------------------------------------------------*/
	
	uint32 GetSelectedLight() { return mSelectedLight; }
	void SetSelectedLight(uint32 i) { mSelectedLight = i; }
	uint32 mSelectedLight;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns currently selected Light index
	 ------------------------------------------------------*/

	uint32 GetSelectedAnimation() { return mSelectedAnimation; }
	void SetSelectedAnimation(uint32 i) { mSelectedAnimation = i; }
	uint32 mSelectedAnimation;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns currently selected animation index
	 ------------------------------------------------------*/

	uint32 GetSelectedBone() { return mSelectedBone; }
	void SetSelectedBone(uint32 i) 
	{ if (i < freyjaGetBoneCount()) mSelectedBone = i; }
	uint32 mSelectedBone;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns currently selected bone index
	 ------------------------------------------------------*/

	uint32 GetSelectedFace() { return mSelectedFace; }
	void SetSelectedFace(uint32 i) 
	{ if (i < freyjaGetMeshPolygonCount(mSelectedMesh)) mSelectedFace = i; }
	uint32 mSelectedFace;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns currently selected face index
	 ------------------------------------------------------*/

	uint32 GetSelectedKeyFrame() { return mSelectedKeyFrame; }
	void SetSelectedKeyFrame(uint32 i) { mSelectedKeyFrame = i; }
	uint32 mSelectedKeyFrame;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns currently selected keyframe index
	 ------------------------------------------------------*/

	uint32 GetSelectedModel() { return mSelectedModel; }
	void SetSelectedModel(uint32 i) { mSelectedModel = i; }
	uint32 mSelectedModel;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns currently selected model index
	 ------------------------------------------------------*/

	uint32 GetSelectedMesh() { return mSelectedMesh; }
	void SetSelectedMesh(uint32 i);
	uint32 mSelectedMesh;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns currently selected mesh index
	 ------------------------------------------------------*/

	uint32 GetSelectedSkeleton() { return mSelectedSkeleton; }
	void SetSelectedSkeleton(uint32 i) 
	{ if (i < freyjaGetSkeletonCount()) mSelectedSkeleton = i; }
	uint32 mSelectedSkeleton;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns currently selected skeleton index
	 ------------------------------------------------------*/

	uint32 GetSelectedTexCoord() { return mSelectedTexCoord; }
	void SetSelectedTexCoord(uint32 i) 
	{ if (i < freyjaGetMeshTexCoordCount(mSelectedMesh)) mSelectedTexCoord = i; }
	uint32 mSelectedTexCoord;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns currently selected texcoord index
	 ------------------------------------------------------*/

	uint32 GetSelectedVertex() { return mSelectedVertex; }
	void SetSelectedVertex(uint32 i) 
	{ if (i < freyjaGetMeshVertexCount(mSelectedMesh)) mSelectedVertex = i; }
	uint32  mSelectedVertex;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns currently selected vertex
	 ------------------------------------------------------*/

	freyja_plane_t GetSelectedView() { return mSelectedView; }
	void SetSelectedView(freyja_plane_t p) { mSelectedView = p; }
	freyja_plane_t mSelectedView;
	/*------------------------------------------------------
	 * Pre  : - TEMP USE ONLY -
	 * Post : Returns currently selected view
	 ------------------------------------------------------*/

	uint32 GetSelectedViewport() { return mSelectedViewport; }
	void SetSelectedViewport(uint32 i) { if (i < 4) mSelectedViewport = i; }
	uint32 mSelectedViewport;
	/*------------------------------------------------------
	 * Pre  : - TEMP USE ONLY -
	 * Post : Returns currently selected view
	 ------------------------------------------------------*/

	vec_t GetZoom();
	void SetZoom(vec_t zoom);
	/*------------------------------------------------------
	 * Pre  : Set() <zoom> is a number greater than 0.0
	 * Post : Returns current viewing zoom of scene
	 ------------------------------------------------------*/

	vec_t GetGenMeshHeight() { return mGenMeshHeight; }
	void SetGenMeshHeight(vec_t v) { mGenMeshHeight = v; }

	uint32 GetGenMeshCount() { return mGenMeshCount; }
	void SetGenMeshCount(uint32 i) { mGenMeshCount = i; }

	uint32 GetGenMeshSegements() { return mGenMeshSegements; }
	void SetGenMeshSegements(uint32 i) { mGenMeshSegements = i; }



	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	static void AttachMethodListeners();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Hooks up MethodDelegates to the event system.
	 ------------------------------------------------------*/

	void AdjustMouseXYForViewports(vec_t &x, vec_t &y);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Maps actual mouse x, y to veiwport's local x, y.
	 *        As if the viewport was the same size as 
	 *        entire context.
	 ------------------------------------------------------*/

	freyja3d::Cursor &GetCursor() { return mCursor; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	bool SaveModel(const char *filename, const char *plugin);
	/*------------------------------------------------------
	 * Pre  : Writes model to disk
	 * Post : Returns true if sucessful
	 ------------------------------------------------------*/

	bool SaveModel(const char *filename);
	/*------------------------------------------------------
	 * Pre  : Writes model to disk
	 * Post : Returns true if sucessful
	 ------------------------------------------------------*/

	bool SaveMaterial(const char *filename);
	/*------------------------------------------------------
	 * Pre  : Writes material to disk
	 * Post : Returns true if sucessful
	 ------------------------------------------------------*/

	object_type_t GetObjectMode() { return mObjectMode; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	String ObjectTypeToString(object_type_t t);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns String for transform type
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////
	
	void ActionModelModified(Action *action);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : New event 'helper' for undo/redo.
	 *
	 *        Call this after every modification outside the
	 *        normal state system.
	 ------------------------------------------------------*/

	void AddRecentFilename(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Appends filename to recently opened file list
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.14: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void CastPickRay(vec_t x, vec_t y);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Casts the pick array given a screen x, y.
	 *        Accounts for view mode ( front, top, side, free )
	 ------------------------------------------------------*/

	static uint32 mSelectedControlPoint;
	static Vector<hel::Vec3> mControlPoints;
	static Vector<hel::Vec3> &GetControlPoints() { return mControlPoints; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	const char *GetRecentFilename(uint32 i)
	{ return mRecentFiles[i].GetCString(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void handleTextEvent(int event, const char *text);
	/*------------------------------------------------------
	 * Pre  : text is valid
	 * Post : Processes event using text given
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.16:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void Init();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Loads resource, which sets up the interface
	 *        Also initilizes all the subsystems of this control
	 ------------------------------------------------------*/

	bool LoadTextureBuffer(byte *image, uint32 width, uint32 height, 
						  uint32 bpp, Texture::ColorMode type);
	/*------------------------------------------------------
	 * Pre  : Don't pass in invalid/mismatched data
	 * Post : Loads a texture given a pixel buffer
	 ------------------------------------------------------*/

	// HandleMouseEvent
	bool MouseEvent(int btn, int state, int mod, int x, int y);
	/*--------------------------------------------
	 * Created  : 2000-09-10 by Mongoose
	 * Modified : 
	 * Pre      : 
	 * Post     : Process mouse input
	 --------------------------------------------*/
	
	bool MotionEvent(int x, int y);
	/*--------------------------------------------
	 * Created  : 2000-09-10 by Mongoose
	 * Modified : 
	 * Pre      : 
	 * Post     : Process mouse motion input
	 --------------------------------------------*/
	
	bool Key(unsigned int key, int x, int y, int modifer_keys);
	/*--------------------------------------------
	 * Created  : 2000-09-10 by Mongoose
	 * Modified : 
	 * Pre      : 
	 * Post     : Process alphanumeric keyboard 
	 *            input
	 --------------------------------------------*/

	bool LoadModel(const char *filename);
	/*------------------------------------------------------
	 * Pre  : Reads model from disk
	 * Post : Returns true if sucessful
	 ------------------------------------------------------*/

	bool LoadMaterial(const char *filename);
	/*------------------------------------------------------
	 * Pre  : Reads material from disk
	 * Post : Returns true if sucessful
	 ------------------------------------------------------*/

	void LoadResource();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Loads the Resource file from disk and sets cvars
	 *
	 ------------------------------------------------------*/

	bool LoadTexture(const char *filename, int &id)
	{
		bool b = false;
		if (mMaterial.LoadTexture(filename)) 
		{ id = mMaterial.GetTextureId()-1; b = true; }
		return b;
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool LoadUserPreferences();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Loads the user resource from disk and sets cvars
	 *        Returns true if found and loads.
	 ------------------------------------------------------*/

	bool SaveUserPreferences();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Saves the user resource to disk for some cvars
	 *        Returns true if sucessful.
	 ------------------------------------------------------*/

	void SetObjectMode(object_type_t m) 
	{ 
		mObjectMode = m; 
		freyja_print("Object mode set to %s...",
					 ObjectTypeToString(mObjectMode).c_str());
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets major editing mode for the control.
	 *        
	 ------------------------------------------------------*/

	void SetActionMode(action_type_t a) { mEventMode = a; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets major editing mode action state for the control.
	 *        
	 ------------------------------------------------------*/

	// FIXME: This stuff from the old code merge at least needs to be 
	// broken into remaining classes 
	void SetFlag(options_t flag, bool t) 
	{ (t) ? mFlags |= flag : mFlags ^= flag; } // &= ~t;?


	bool ToggleFlag(options_t flag) { mFlags ^= flag; return mFlags & flag; }

	uint32 GetViewMode() { return mRender->GetViewMode(); }

	uint32 GetSelectedMaterial() { return mMaterial.GetSelected(); }
 

	////////////////////////////////////////////////////////////
	// Events, once all these are 'sorted' decouple.
	////////////////////////////////////////////////////////////

	static uint32 eRotateObjectId;
	static uint32 eScaleObjectId;
	static uint32 eMoveObjectId;
	static uint32 eSelectionByBoxId;
	static uint32 eInfoObjectId;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Event ids used to replace old event system
	 *        usage of Ids to 'link' widget behavior.
	 *        These statics replace constant event values.
	 *
	 *        In the old system Ids could be passed around,
	 *        and all kinds of crazy event paths where made
	 *        that still depend on similar behavior.
	 ------------------------------------------------------*/

	static uint32 eUnselectId;
	static uint32 eSelectId;

	// Nop events
	void EvFloatNop(float value) { }

	// Float events
	static uint32 EvMoveXId, EvMoveYId, EvMoveZId;
	static uint32 EvRotateXId, EvRotateYId, EvRotateZId;
	static uint32 EvScaleXId, EvScaleYId, EvScaleZId;
	void EvScaleX(float value);
	void EvScaleY(float value);
	void EvScaleZ(float value);

	// Iterator events
	void EvPolygonIterator(unsigned int value);
	void EvMeshIterator(unsigned int value);
	static uint32 EvBoneIteratorId;
	void EvBoneIterator(unsigned int value);

	// Text events
	void EvSkeletonName(char *text);
	static uint32 EvSetBoneNameId;
	void EvSetBoneName(char *text);
	void EvOpenModel(char *filename);

	// 2x Text events
	void EvSaveModel(char *filename, char *extension);


	void ePolygonSplit()
	{
		Print("Splitting polygon[%i] ...", GetSelectedFace());
		freyjaMeshPolygonSplit(GetSelectedMesh(), GetSelectedFace());
		Dirty();
	}

	void eSetMeshTexture()
	{
		Print("Switching all of Mesh[%i]'s faces to material %i ...",
			  GetSelectedMesh(), GetSelectedMaterial());
		freyjaMeshMaterial(GetSelectedMesh(), GetSelectedMaterial());
		Dirty();
	}

	void eSetFacesMaterial()
	{
		Print("Switching all of selected faces to material %i",
			  GetSelectedMesh(), GetSelectedMaterial());
		SetMaterialForSelectedFaces(GetSelectedMaterial());
		Dirty();
	}

	void eSetPolygonTexture()
	{
		Print("Face to material set to %i", GetSelectedMaterial());
		SetFaceMaterial(GetSelectedFace(), GetSelectedMaterial());
		Dirty();
	}


	void eSelectionByBox(unsigned int value);
	void eSelect(unsigned int value);
	void eUnselect(unsigned int value);
	void eMoveObject(unsigned int value);
	void eScaleObject(unsigned int value);
	void eRotateObject(unsigned int value);
	void eInfoObject(unsigned int value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Event callbacks for hooking to widget system 
	 *        ( Break up into separate classes later. )
	 *
	 ------------------------------------------------------*/

	void eModeUV();
	void eModeModel();
	void eModeMaterial();
	static uint32 EvModeAutoKeyframeId;
	void EvModeAutoKeyframe(unsigned int value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Event callbacks for hooking to widget system 
	 *        ( Break up into separate classes later. )
	 *
	 ------------------------------------------------------*/

	void eTransformVertices() { SetObjectMode(tSelectedVertices); }

	void eTransformScene() { SetObjectMode(tScene); }

	void eTransformVertex() { SetObjectMode(tPoint); }

	void eTransformFaces() { SetObjectMode(tSelectedFaces); }

	void eTransformFace() { SetObjectMode(tFace); }

	void eTransformMesh() { SetObjectMode(tMesh); }

	void eTransformMeshes() { SetObjectMode(tSelectedMeshes); }

	void eTransformModel() { SetObjectMode(tModel); }

	void eTransformBone() { SetObjectMode(tBone); }

	void eTransformLight() { SetObjectMode(tLight); }

	void eRecentFiles(uint32 value) { LoadModel(GetRecentFilename(value)); }

	void eOpenShader(char *text);
	void eOpenTexture(char *text);
	//void eModelUpload(char *filename) { LoadModel(filename); }

	void eScale();
	void eMove();
	void eRotate();

	void EvAnimationSlider(unsigned int value);

	void eGenMeshHeight(vec_t value);

	static uint32 eAxisJointId;
	void eAxisJoint(uint32 value);
	static uint32 eSphereJointId;
	void eSphereJoint(uint32 value);
	static uint32 ePointJointId;
	void ePointJoint(uint32 value);

	void eRenderWireframe(uint32 value);
	void eRenderFace(uint32 value);
	void eRenderVertex(uint32 value);
	void eRenderNormals(uint32 value);
	void eRenderTexture(uint32 value);
	void eRenderLighting(uint32 value);
	void eRenderMaterial(uint32 value);
	void eRenderSkeleton(uint32 value);
	void eRenderGrid(uint32 value);
	void eRenderSolidGround(uint32 value);
	void eRenderBbox(uint32 value);
	void eRenderPickRay(uint32 value);

	void eViewports(uint32 value);
	void eViewportBack();
	void eViewportBottom();
	void eViewportRight();
	void eViewportFront();
	void eViewportTop();
	void eViewportLeft();
	void eViewportOrbit();
	void eViewportUV();
	void eViewportCurve();
	void eViewportMaterial();

	void eMeshNew();
	void eMeshDelete();
	void eMeshSelect();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Event callbacks for hooking to widget system 
	 *        ( Break up into separate classes later. )
	 *
	 ------------------------------------------------------*/

	void Redo();
	void Undo();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Event callbacks for hooking to widget system 
	 *        ( Break up into separate classes later. )
	 *
	 ------------------------------------------------------*/

	void NewFile();
	void CloseFile();
	void ExportFile();
	void ImportFile();
	void SaveFile();
	void OpenFile();
	void SaveFileModel();
	void OpenFileModel();
	void RevertFile();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Event callbacks for hooking to widget system 
	 *        ( Break up into separate classes later. )
	 *
	 ------------------------------------------------------*/

	void Shutdown();
	void VertexCombine();
	void TexcoordCombine();
	void Fullscreen();
	void SetKeyFrame();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Event callbacks for hooking to widget system 
	 *        ( Break up into separate classes later. )
	 *
	 ------------------------------------------------------*/

	void SetMaterialForSelectedFaces(uint32 material); // prv
	void SetFaceMaterial(index_t faceIndex, index_t material); //prv

	void CreateObject();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void DeleteSelectedObject();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool MergeSelectedObjects();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool DuplicateSelectedObject();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool SplitSelectedObject();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool CopySelectedObject();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool PasteSelectedObject();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void Cut();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void Dirty() { mCleared = false; freyja_event_gl_refresh(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Call this when some state in the model or editor
	 *        has changed that would be need to be saved.
	 *
	 *        It has a side effect of refreshing the display.
	 *        This is a hint to call this *after state change.
	 *
	 ------------------------------------------------------*/

	void RecordSavedModel(const char *filename)
	{
		mstl::String title;
		title.Set("%s - Freyja", filename);
		freyja_set_main_window_title(title.c_str());
		mCleared = true;
		mCurrentlyOpenFilename = filename;
		AddRecentFilename(filename);
	}
	/*------------------------------------------------------
	 * Pre  : Successfully saved a model file.
	 * Post : This updates states that track saved models.
	 *
	 ------------------------------------------------------*/

	static FreyjaControl *mInstance;


protected:

	FreyjaControl();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : FreyjaControl object is constructed
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////

	String ActionToString(freyja_transform_action_t action)
	{ return String(freyjaActionToString(action)); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void getPickRay(vec_t mouseX, vec_t mouseY, 
					vec4_t rayOrigin, vec4_t rayVector);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns pick ray in object cooridnates
	 ------------------------------------------------------*/

	void getScreenToWorldOBSOLETE(vec_t &x, vec_t &y);
	/*------------------------------------------------------
	 * Pre  : Emulates the old ScreenToWorld() method behavior
	 * Post : This function will be removed after the Model rewrite
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.15: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void GetWorldFromScreen(vec_t &x, vec_t &y, vec_t &z);
	/*------------------------------------------------------
	 * Pre  : X and Y are the mouse position
	 *
	 * Post : Converts x,y on screen to x,y,z in world
	 *        for whichever plane was last selected
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25: 
	 * Mongoose - Created from ScreenToWorld from GooseEgg
	 ------------------------------------------------------*/

	uint32 GetViewFlags() { return mRender->GetFlags(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Wrapper for view flags accessor
	 ------------------------------------------------------*/

	hel::Vec3 GetCursorData(freyja_transform_action_t action)
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/
	{
		hel::Vec3 v;

		switch (action)
		{
		case fTranslate:
			v = mCursor.mPos - mCursor.mLastPos;
			break;

		case fRotateAboutOrigin:
		case fRotate:
			v = mCursor.mRotate;
			break;

		case fScaleAboutOrigin:
		case fScale:
			v = mCursor.mScale;
			break;

		default:
			DEBUG_MSG("\tInvalid mode\n");;
		}

		return v;
	}

	freyja_transform_action_t GetEventAction()
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/
	{
		switch (mEventMode)
		{
		case aMove:
			return fTranslate;
			break;

		case aRotate:
			return fRotate;
			break;

		case aScale:
			return fScale;
			break;

		default:
			;
		}

		DEBUG_MSG("\tUnknown Action\n");
		return fTransformActionNone;
	}


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	// Collection of some hasty merging  ///////////////////////////

	// FIXME: This currently won't dump keyframe as you'd expect in the new API
	bool SaveKeyFrame(char const *filename)
	{
		char filename2[512];
		snprintf(filename2, 512, "%s-keyframe-%i.smd", filename, 
				 GetSelectedKeyFrame());
		return SaveModel(filename2);
	}

	// FIXME: Temp here to let this work during rewrite so it could be uploaded to public svn to fix revision corruption in public svn
	void InitTexture()
	{
		unsigned char rgba[4] = {255, 255, 255, 255};
		mTexture.reset();
		mTexture.setMaxTextureCount(64);
		mTexture.setFlag(Texture::fUseMipmaps);
		mTexture.loadColorTexture(rgba, 32, 32);
		//mTextureId = 1;
	}

	void CreatePolyMappedUVMap(int i) {BUG_ME("Not implemented in this build");}

	void CursorMove(float xx, float yy);

	void GetBoneRotation(float *x, float *y, float *z)
	{
		vec3_t xyz;
		freyjaGetBoneRotationEuler3fv(GetSelectedBone(), xyz);
		*x = helRadToDeg(xyz[0]);
		*y = helRadToDeg(xyz[1]);
		*z = helRadToDeg(xyz[2]);
		freyja_print("%f %f %f", xyz[0], xyz[1], xyz[2]);
	}

	void KeyframeTransform(object_type_t obj, 
						   freyja_transform_action_t action,
						   vec_t x, vec_t y, vec_t z);

	void Transform(object_type_t obj, 
				   freyja_transform_action_t action,
				   vec_t x, vec_t y, vec_t z);// { MARK_MSGF("Not Implemented"); }
	void Transform(freyja_transform_t obj, 
				   freyja_transform_action_t action,
				   index_t owner, index_t id,
				   vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : obj is {fTransformModel, fTransformMesh, etc }
	 *        action is { fTranslate, fRotate, fScale, etc }
	 *        x, y, z are in radians or units
	 *
	 * Post : Transform is performed
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void SelectMode();
	void UnselectMode();

	void UpdateSkeletalUI() { UpdateSkeletonUI_Callback(GetSelectedSkeleton()); }

	const char *GetBoneName(unsigned int boneIndex)
	{
		return (freyjaIsBoneAllocated(boneIndex)) ? freyjaGetBoneNameString(boneIndex) : 0x0;
	}

	void SetBoneName(unsigned int boneIndex, const char *name)
	{
		if (freyjaIsBoneAllocated(boneIndex) && name && name[1])
		{
			freyjaBoneName(boneIndex, name);
			freyja_print("bone[%i].name = '%s'", boneIndex, name);
			UpdateSkeletalUI();
		}
	}

	void PrintInfo();

	void TexCoordMove(vec_t u, vec_t v);
	void TexCoordSelect(vec_t u, vec_t v);
	index_t mTexCoordArrayIndex;

	void SetBoneRotation(float x, float y, float z)
	{
		vec3_t xyz = {helDegToRad(x), helDegToRad(y), helDegToRad(z)};
		freyjaBoneRotateEuler3fv(GetSelectedBone(), xyz);
	}

	void Clear()
	{
		freyjaModelClear(0); // Only 1 edit model supported in this bulid
		UpdateSkeletalUI();
		UpdateSkeletonUI_Callback(0); // Update skeletal UI
		mCursor.Reset();
		mCleared = true;

		RefreshContext();
	}

	void MoveObject(vec_t vx, vec_t vy);
	/*------------------------------------------------------
	 * Pre  : Pass viewport adjusted x, y 
	 * Post : Determines type and which object to move based
	 *        on editing mode and selection, and moves it.
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.01: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void rotateObject(int x, int y, freyja_plane_t plane);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.01: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	void SelectCursorAxis(vec_t vx, vec_t vy);
	/*------------------------------------------------------
	 * Pre  : vx, vy are mouse coords adjusted for viewports
	 * Post : Sets selected cursor 'axis' X Y Z
	 *
	 ------------------------------------------------------*/

	void SelectObjectByBox(hel::Vec3 min, hel::Vec3 max);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void InfoObject();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Produces information dialog for selected object.
	 *
	 ------------------------------------------------------*/

	void SelectObject(vec_t mouseX, vec_t mouseY, bool set);
	/*------------------------------------------------------
	 * Pre  : Given mouse click coordinates and flag.
	 * Post : Casts a pick ray for current object type.
	 *        If there is a hit sets/clears flags based on <set>.
	 *        If selected object is also set to current object.
	 *
	 ------------------------------------------------------*/

	void scaleObject(int x, int y, freyja_plane_t plane);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.01: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	bool LoadRecentFilesResource(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	bool MouseEdit(int btn, int state, int mod, int x, int y);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Process mouse input in edit mode
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created  ( Updated comment style )
	 ------------------------------------------------------*/

	static int CreateListener(const char *name, MethodPtr ptr);
	static int CreateListener(const char *name, MethodPtrB ptr);
	static int CreateListener1u(const char *name, MethodPtr1u ptr);
	static int CreateListener1f(const char *name, MethodPtr1f ptr);
	static int CreateListener1s(const char *name, MethodPtr1s ptr);
	static int CreateListener2s(const char *name, MethodPtr2s ptr);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Maps mlisp symbol <name> to method <ptr> action.
	 *        Returns event id bound to <name>.
	 *
	 ------------------------------------------------------*/

	index_t PickMeshes(const hel::Ray &ray);
	index_t PickVertex(const hel::Ray &ray);
	index_t PickLight(const hel::Ray &ray);
	index_t PickFace(const hel::Ray &ray);
	index_t PickBone(const hel::Ray &ray);
	index_t PickMesh(const hel::Ray &ray);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Object picks factored out and reduced.
	 *
	 * TODO : Move these into their own Controls.
	 *
	 ------------------------------------------------------*/


	ActionManager mActionManager;           /* New Undo/Redo system */

	uint32 mFlags;                          /* Option bitflags */

	object_type_t mObjectMode;              /* Current object type to edit */

	Texture mTexture;                       /* Collection of Texture utils */

	static MaterialEv mMaterial;            /* Material control */

	const static uint32 mRecentFileLimit = 10;
	Vector<String> mRecentFiles;            /* Recently loaded model files */

	Vector<int32> mUVMap;                   /* 'Texture faces' grouping */

	String mResourceFilename;	            /* Resource file for control */

	String mUserPrefsFilename;	            /* Resource file for prefs */

	String mCurrentlyOpenFilename;          /* What file is loaded now? */

	hel::Vec3 mSceneTrans;                  /* Offset of scene in 3 space */

	freyja3d::Cursor mCursor;               /* Special mouse input handler */
	
	FreyjaRender *mRender;                  /* OpenGL renderer */

	unsigned int mLastEvent, mLastCommand;  /* The last command pair recieved*/

	unsigned int mMouseButton, mModKey;     /* Cached mouse button event */

	int mMouseState;                        /* Cached mouse state event */

	ControlScheme mControlScheme;           /* Current control scheme in use */

	action_type_t mEventMode;               /* Mode of generic event handler */
	
	int mUVMouseState;                      /* Mouse state on texture canvas */
	
	int mXYZMouseState;                     /* Mouse state on modeler canvas */

	bool mFullScreen;

	bool mCleared;

	bool mAllowBoneNameUpdate;

	float mGenMeshHeight;
	unsigned long mGenMeshCount;
	unsigned long mGenMeshSegements;

	bool mToken;                            /* This puts the breaks on over
											 * doing pushing actions */
};


} /* End namespace freyja3d */

void FreyjaControlEventsAttach();

#endif
