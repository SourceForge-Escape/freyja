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
#include "MaterialControl.h"
#include "RecentFiles.h"


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
		aPaint,

		/* Specialized actions, which should be changed to generic */
		aVertexNew,
		aVertexDelete,
		aVertexCombine,
		aTexcoordCombine
		
	} action_type_t;


	typedef enum {

		ePaintWeight = 1,
		ePaintUnweight,
		ePaintSelect,
		ePaintUnselect,
		ePaintMaterial,
		ePaintHeight,
		ePaintDmap

	} PaintMode;


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
	 * Post : Instance FreyjaControl.
	 *
	 ------------------------------------------------------*/

	~FreyjaControl();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Deconstruct current instance.
	 *
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
	 *
	 ------------------------------------------------------*/

	uint32 GetFaceEdgeCount() { return mFaceEdgeCount; }
	void SetFaceEdgeCount(uint32 i) { if (i>2) mFaceEdgeCount = i; }
	uint32 mFaceEdgeCount;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns face 'edge count' for draw poly from 
	 *        points interface.  ( An odd, yet popular feature. ) 
	 *
	 ------------------------------------------------------*/

	const hel::Vec3 &GetSceneTranslation() { return mSceneTrans; }
	void SetSceneTranslation(const hel::Vec3 &v) { mSceneTrans = v; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns xyz offset of the scene
	 *
	 ------------------------------------------------------*/
	
	uint32 GetSelectedLight() { return mSelectedLight; }
	void SetSelectedLight(uint32 i) { mSelectedLight = i; }
	uint32 mSelectedLight;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns currently selected Light index
	 *
	 ------------------------------------------------------*/

	uint32 GetSelectedAnimation() { return mSelectedAnimation; }
	void SetSelectedAnimation(uint32 i) { mSelectedAnimation = i; }
	uint32 mSelectedAnimation;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns currently selected animation index
	 *
	 ------------------------------------------------------*/

	uint32 GetSelectedBone() { return mSelectedBone; }
	void SetSelectedBone(uint32 i) 
	{ if (i < freyjaGetBoneCount()) mSelectedBone = i; }
	uint32 mSelectedBone;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns currently selected bone index
	 *
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
	 *
	 ------------------------------------------------------*/

	uint32 GetSelectedMesh() { return mSelectedMesh; }
	void SetSelectedMesh(uint32 i);
	uint32 mSelectedMesh;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns currently selected mesh index
	 *
	 ------------------------------------------------------*/

	uint32 GetSelectedSkeleton() { return mSelectedSkeleton; }
	void SetSelectedSkeleton(uint32 i) 
	{ if (i < freyjaGetSkeletonCount()) mSelectedSkeleton = i; }
	uint32 mSelectedSkeleton;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns currently selected skeleton index
	 *
	 ------------------------------------------------------*/

	uint32 GetSelectedTexCoord() { return mSelectedTexCoord; }
	void SetSelectedTexCoord(uint32 i) 
	{ if (i < freyjaGetMeshTexCoordCount(mSelectedMesh)) mSelectedTexCoord = i; }
	uint32 mSelectedTexCoord;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns currently selected texcoord index
	 *
	 ------------------------------------------------------*/

	uint32 GetSelectedVertex() { return mSelectedVertex; }
	void SetSelectedVertex(uint32 i) 
	{ if (i < freyjaGetMeshVertexCount(mSelectedMesh)) mSelectedVertex = i; }
	uint32  mSelectedVertex;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns currently selected vertex
	 *
	 ------------------------------------------------------*/

	freyja_plane_t GetSelectedView() { return mSelectedView; }
	void SetSelectedView(freyja_plane_t p) { mSelectedView = p; }
	freyja_plane_t mSelectedView;
	/*------------------------------------------------------
	 * Pre  : - TEMP USE ONLY -
	 * Post : Returns currently selected view
	 *
	 ------------------------------------------------------*/

	uint32 GetSelectedViewport() { return mSelectedViewport; }
	void SetSelectedViewport(uint32 i) { if (i < 4) mSelectedViewport = i; }
	uint32 mSelectedViewport;
	/*------------------------------------------------------
	 * Pre  : - TEMP USE ONLY -
	 * Post : Returns currently selected view
	 *
	 ------------------------------------------------------*/

	vec_t GetZoom();
	void SetZoom(vec_t zoom);
	/*------------------------------------------------------
	 * Pre  : Set() <zoom> is a number greater than 0.0
	 * Post : Returns current viewing zoom of scene
	 *
	 ------------------------------------------------------*/

	uint32 GetViewMode() { return mRender->GetViewMode(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get the current view mode for the renderer.
	 *        
	 ------------------------------------------------------*/

	uint32 GetSelectedMaterial() { return mMaterial.GetSelected(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get currently selected material id.
	 *        
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	void AttachMethodListeners();
	/*------------------------------------------------------
	 * Pre  : Only call once, and only for singleton.
	 * Post : Hooks up MethodDelegates to the event system.
	 *
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
	 * Post : Get current 3d cursor. 
	 *
	 ------------------------------------------------------*/

	bool UnserializeMesh(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Serializes only currently selected mesh to file. 
	 *       Returns true if sucessful
	 *
	 ------------------------------------------------------*/

	bool SerializeBones(const char *filename);
	bool UnserializeBones(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Serializes only bones/joints to file. 
	 *       Returns true if sucessful
	 *
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

	PaintMode GetPaintMode() { return mPaintMode; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
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

	void CastPickRay(vec_t x, vec_t y);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Casts the pick array given a screen x, y.
	 *        Accounts for view mode ( front, top, side, free )
	 ------------------------------------------------------*/

	void ClearFlag(options_t flag) { mFlags &= ~flag; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *        
	 ------------------------------------------------------*/

	static uint32 mSelectedControlPoint;
	static Vector<hel::Vec3> mControlPoints;
	static Vector<hel::Vec3> &GetControlPoints() { return mControlPoints; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
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

	bool LoadModel(const char *filename);
	/*------------------------------------------------------
	 * Pre  : Reads model from disk
	 * Post : Returns true if sucessful
	 *
	 ------------------------------------------------------*/

	bool LoadMaterial(const char *filename);
	/*------------------------------------------------------
	 * Pre  : Reads material from disk
	 * Post : Returns true if sucessful
	 *
	 ------------------------------------------------------*/

	void LoadResource();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Loads the Resource file from disk and sets cvars
	 *
	 ------------------------------------------------------*/

	bool LoadTexture(const char *filename, int &id);
	/*------------------------------------------------------
	 * Pre  : Loads texture <filename> into texture manager.
	 * Post : Sets texture <id> and returns true if sucessful.
	 *
	 ------------------------------------------------------*/

	bool LoadUserPreferences();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Loads the user resource from disk and sets cvars
	 *        Returns true if found and loads.
	 ------------------------------------------------------*/

	bool MouseEvent(int btn, int state, int mod, int x, int y);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Process mouse input.
	 *        
	 ------------------------------------------------------*/
	
	bool MotionEvent(int x, int y);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Process mouse motion input.
	 *        
	 ------------------------------------------------------*/

	void PaintObject(vec_t x, vec_t y);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Casts the pick array given a screen x, y.
	 *        Uses current brush and subevent to paint object.
	 ------------------------------------------------------*/

	bool SaveUserPreferences();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Saves the user resource to disk for some cvars
	 *        Returns true if sucessful.
	 ------------------------------------------------------*/

	void SelectAll(bool set);
	/*------------------------------------------------------
	 * Pre  : <set> - true for select, false for unselect
	 * Post : (un)selects all objects of a certain type.
	 *        
	 ------------------------------------------------------*/

	void SetObjectMode(object_type_t m);
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

	void SetFlag(options_t flag) { mFlags |= flag; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *        
	 ------------------------------------------------------*/
 

	////////////////////////////////////////////////////////////
	// Events, once all these are 'sorted' decouple.
	////////////////////////////////////////////////////////////


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

	void EvSerializeBones();
	void EvUnserializeBones();

	void EvSerializeMesh();
	void EvUnserializeMesh();

	void EvSelectAll() { SelectAll(true); } 
	void EvUnselectAll() { SelectAll(false); } 

	void EvPaintWeight() { mPaintMode = ePaintWeight; }
	void EvPaintUnweight() { mPaintMode = ePaintUnweight; }
	void EvPaintSelect() { mPaintMode = ePaintSelect; }
	void EvPaintUnselect() { mPaintMode = ePaintUnselect; }
	void EvPaintMaterial() { mPaintMode = ePaintMaterial; }
	void EvPaintHeight() { mPaintMode = ePaintHeight; }
	void EvPaintDmap() { mPaintMode = ePaintDmap; }

	void EvLoadLuaScript();
	void EvLoadPythonScript();

	void EvLaunchFourms()
	{ mstl::SystemIO::OpenURLInWebBrowser(FREYJA_FORUMS_URL); }

	void EvLaunchBugs()
	{ mstl::SystemIO::OpenURLInWebBrowser(FREYJA_BUGS_URL); }

	// FIXME: Later this will do automatic upgrades
	void EvLaunchUpdate()
	{ mstl::SystemIO::OpenURLInWebBrowser(FREYJA_NEWS_URL); }

	void EvMeshRepack()
	{
		Mesh *m = Mesh::GetMesh( GetSelectedMesh() );
		if (m) m->Repack();
	}

	void EvPolygonSplit()
	{
		Print("Splitting polygon[%i] ...", GetSelectedFace());
		freyjaMeshPolygonSplit(GetSelectedMesh(), GetSelectedFace());
		Dirty();
	}

	void EvSetMeshTexture()
	{
		Print("Switching all of Mesh[%i]'s faces to material %i ...",
			  GetSelectedMesh(), GetSelectedMaterial());
		freyjaMeshMaterial(GetSelectedMesh(), GetSelectedMaterial());
		Dirty();
	}

	void EvSetFacesMaterial()
	{
		Print("Switching all of selected faces to material %i",
			  GetSelectedMesh(), GetSelectedMaterial());
		SetMaterialForSelectedFaces(GetSelectedMaterial());
		Dirty();
	}

	void EvSetPolygonTexture()
	{
		Print("Face to material set to %i", GetSelectedMaterial());
		SetFaceMaterial(GetSelectedFace(), GetSelectedMaterial());
		Dirty();
	}

	void EvSelectVerticesByFaces()
	{
		Mesh *m = Mesh::GetMesh( GetSelectedMesh() );
		if (m) m->SelectVerticesOfSelectedFaces();
	}

	void SetModiferMode(action_type_t mode, const char *name,
						uint32 event, freyja3d::Cursor::Flags flags);
	/*------------------------------------------------------
	 * Pre  : <event> modifer mode event id.
	 * Post : Modifer mode event widgets are reset as needed.
	 *
	 ------------------------------------------------------*/

	void EvBoxSelectObject(unsigned int value);
	static uint32 eBoxSelectObjectId;

	void EvSelectObject(unsigned int value);
	static uint32 eSelectObjectId;

	void EvUnselectObject(unsigned int value);
	static uint32 eUnselectObjectId;

	void EvMoveObject(unsigned int value);
	static uint32 eMoveObjectId;

	void EvScaleObject(unsigned int value);
	static uint32 eScaleObjectId;

	void EvRotateObject(unsigned int value);
	static uint32 eRotateObjectId;

	void EvInfoObject(unsigned int value);
	static uint32 eInfoObjectId;

	void EvPaintObject(unsigned int value);
	static uint32 ePaintObjectId;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Modifer mode event callbacks.
	 *
	 ------------------------------------------------------*/

	void EvMeshSubDiv();

	void EvMove();
	void EvRotate();
	void EvScale();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Transform box event callbacks.
	 *
	 ------------------------------------------------------*/

	void EvModeUV();
	void EvModeModel();
	void EvModeMaterial();
	static uint32 EvModeAutoKeyframeId;
	void EvModeAutoKeyframe(unsigned int value);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Event callbacks for hooking to widget system 
	 *        ( Break up into separate classes later. )
	 *
	 ------------------------------------------------------*/

	void EvTransformVertices() { SetObjectMode(tSelectedVertices); }

	void EvTransformScene() { SetObjectMode(tScene); }

	void EvTransformVertex() { SetObjectMode(tPoint); }

	void EvTransformFaces() { SetObjectMode(tSelectedFaces); }

	void EvTransformFace() { SetObjectMode(tFace); }

	void EvTransformMesh() { SetObjectMode(tMesh); }

	void EvTransformMeshes() { SetObjectMode(tSelectedMeshes); }

	void EvTransformModel() { SetObjectMode(tModel); }

	void EvTransformBone() { SetObjectMode(tBone); }

	void EvTransformSkeleton() { SetObjectMode(tSkeleton); }

	void EvTransformLight() { SetObjectMode(tLight); }

	void EvRecentFiles(uint32 value) 
	{ LoadModel( mRecentModel.GetFilename(value)); }

	void EvRecentLua(uint32 value);
	void EvRecentPython(uint32 value);

	void EvRecentKeyframe(uint32 value);

	void EvRecentMeshXML(uint32 value) 
	{ UnserializeMesh( mRecentMesh.GetFilename(value) ); }

	void EvRecentSkeletonXML(uint32 value) 
	{ UnserializeBones( mRecentSkeleton.GetFilename(value) ); }

	void EvOpenShader(char *text);
	void EvOpenTexture(char *text);
	//void EvModelUpload(char *filename) { LoadModel(filename); }

	void EvAnimationSlider(unsigned int value);

	static uint32 eAxisJointId;
	void EvAxisJoint(uint32 value);
	static uint32 eSphereJointId;
	void EvSphereJoint(uint32 value);
	static uint32 ePointJointId;
	void EvPointJoint(uint32 value);

	void EvRenderWireframe(uint32 value);
	void EvRenderFace(uint32 value);
	void EvRenderVertex(uint32 value);
	void EvRenderNormals(uint32 value);
	void EvRenderTexture(uint32 value);
	void EvRenderLighting(uint32 value);
	void EvRenderMaterial(uint32 value);
	void EvRenderSkeleton(uint32 value);
	void EvRenderGrid(uint32 value);
	void EvRenderSolidGround(uint32 value);
	void EvRenderBbox(uint32 value);
	void EvRenderPickRay(uint32 value);

	void EvViewports(uint32 value);
	void EvViewportBack();
	void EvViewportBottom();
	void EvViewportRight();
	void EvViewportFront();
	void EvViewportTop();
	void EvViewportLeft();
	void EvViewportOrbit();
	void EvViewportUV();
	void EvViewportCurve();
	void EvViewportMaterial();

	void EvMeshNew();
	void EvMeshDelete();
	void EvMeshSelect();
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


	////////////////////////////////////////////////////////////
	// End Events
	////////////////////////////////////////////////////////////


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

	void RecordSavedModel(const char *filename);
	/*------------------------------------------------------
	 * Pre  : Successfully saved a model file.
	 * Post : This updates states that track saved models.
	 *
	 ------------------------------------------------------*/

	void UpdateSkeletalUI();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Update node graph UI.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Protected 
	////////////////////////////////////////////////////////////

protected:

	FreyjaControl();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : FreyjaControl object is constructed
	 *
	 ------------------------------------------------------*/

	static FreyjaControl *mInstance;     /* Singleton instance */


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

	void GetPickRay(vec_t mouseX, vec_t mouseY, 
					vec4_t rayOrigin, vec4_t rayVector);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns pick ray in object cooridnates
	 ------------------------------------------------------*/

	void GetScreenToWorldOBSOLETE(vec_t &x, vec_t &y);
	/*------------------------------------------------------
	 * Pre  : Emulates the old ScreenToWorld() method behavior
	 * Post : This function will be removed after the Model rewrite
	 *
	 ------------------------------------------------------*/

	void GetWorldFromScreen(vec_t &x, vec_t &y, vec_t &z);
	/*------------------------------------------------------
	 * Pre  : X and Y are the mouse position
	 *
	 * Post : Converts x,y on screen to x,y,z in world
	 *        for whichever plane was last selected
	 *
	 ------------------------------------------------------*/

	uint32 GetViewFlags() { return mRender->GetFlags(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Wrapper for view flags accessor
	 ------------------------------------------------------*/

	hel::Vec3 GetCursorData(freyja_transform_action_t action);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns transform data for given action.
	 *
	 ------------------------------------------------------*/

	freyja_transform_action_t GetEventAction();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	void CursorMove(float xx, float yy);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void InitTexture();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Setup for texture manager.
	 *
	 ------------------------------------------------------*/

	void KeyframeTransform(object_type_t obj, freyja_transform_action_t action,
						   vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : <x>, <y>, <z> are in radians or other proper units.
	 * Post : Transform <action> is performed on current <obj> keyframe.
	 *
	 ------------------------------------------------------*/

	void Transform(object_type_t obj, freyja_transform_action_t action,
				   vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : <x>, <y>, <z> are in radians or other proper units.
	 * Post : Transform <action> is performed on current <obj>.
	 *
	 ------------------------------------------------------*/

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
	 ------------------------------------------------------*/

	bool SaveKeyFrame(char const *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Save current keyframe to disk.
	 *
	 ------------------------------------------------------*/

	void SelectMode();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void UnselectMode();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void PrintInfo();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void TexCoordMove(vec_t u, vec_t v);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void TexCoordSelect(vec_t u, vec_t v);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void Clear();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Reset data model and default status UI. 
	 *
	 ------------------------------------------------------*/

	static int CreateListener(const char *name, MethodPtr ptr);
	static int CreateListener(const char *name, MethodPtrB ptr);
	static int CreateListener1f(const char *name, MethodPtr1f ptr);
	static int CreateListener1s(const char *name, MethodPtr1s ptr);
	static int CreateListener1u(const char *name, MethodPtr1u ptr);
	static int CreateListener2s(const char *name, MethodPtr2s ptr);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Maps mlisp symbol <name> to method <ptr> action.
	 *        Returns event id bound to <name>.
	 *
	 ------------------------------------------------------*/

	void InfoObject();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Produces information dialog for selected object.
	 *
	 ------------------------------------------------------*/

	void MoveObject(vec_t vx, vec_t vy);
	/*------------------------------------------------------
	 * Pre  : Pass viewport adjusted x, y 
	 * Post : Determines type and which object to move based
	 *        on editing mode and selection, and moves it.
	 *
	 ------------------------------------------------------*/

	void RotateObject(int x, int y, freyja_plane_t plane);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void ScaleObject(int x, int y, freyja_plane_t plane);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void SelectCursorAxis(vec_t vx, vec_t vy);
	/*------------------------------------------------------
	 * Pre  : vx, vy are mouse coords adjusted for viewports
	 * Post : Sets selected cursor 'axis' X Y Z
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

	void SelectObjectByBox(hel::Vec3 min, hel::Vec3 max);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool MouseEdit(int btn, int state, int mod, int x, int y);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Process mouse input in edit mode
	 *
	 ------------------------------------------------------*/

	index_t PickFace(const hel::Ray &ray);
	index_t PickBone(const hel::Ray &ray);
	index_t PickLight(const hel::Ray &ray);
	index_t PickMesh(const hel::Ray &ray);
	index_t PickMeshes(const hel::Ray &ray);
	index_t PickVertex(const hel::Ray &ray);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Object picks factored out and reduced.
	 *
	 * TODO : Move these into their own Controls.
	 *
	 ------------------------------------------------------*/

	freyja3d::Cursor mCursor;               /* Special mouse input handler */

	freyja3d::MaterialControl mMaterial;    /* Material control */

	hel::Vec3 mSceneTrans;                  /* Offset of scene in 3 space */

	mstl::ActionManager mActionManager;     /* Undo/Redo system */

	RecentFiles mRecentModel;               /* Recent files lists. */

	RecentFiles mRecentMesh;

	RecentFiles mRecentSkeleton;

	RecentFiles mRecentKeyframe;

	RecentFiles mRecentLua;

	RecentFiles mRecentPython;

	Texture mTexture;                       /* Collection of Texture utils */	

	FreyjaRender *mRender;                  /* OpenGL renderer */

	String mResourceFilename;	            /* Resource file for control */

	String mUserPrefsFilename;	            /* Resource file for prefs */

	String mCurrentlyOpenFilename;          /* What file is loaded now? */

	ControlScheme mControlScheme;           /* Current control scheme in use */

	action_type_t mEventMode;               /* Mode of generic event handler */

	object_type_t mObjectMode;              /* Current object type to edit */

	PaintMode mPaintMode;                   /* Current paint type. */

	uint32 mFlags;                          /* Option bitflags */

	uint32 mModKey;                         /* Cached modifier key event */

	uint32 mMouseButton;                    /* Cached mouse button event */

	index_t mTexCoordArrayIndex;            /* Current modification texcoord */

	int mMouseState;                        /* Cached mouse state event */	

	int mUVMouseState;                      /* Mouse state on texture canvas */
	
	int mXYZMouseState;                     /* Mouse state on modeler canvas */

	bool mAllowBoneNameUpdate;              /* Specfic text event safeguard */

	bool mCleared;                          /* Model not modified status */

	bool mFullScreen;                       /* Window fullscreen status */

	bool mToken;                            /* Action/Undo stack safeguard */
};


////////////////////////////////////////////////////////////
// Inline methods
////////////////////////////////////////////////////////////

inline
void FreyjaControl::Clear()
{
	freyjaModelClear(GetSelectedModel());
	UpdateSkeletalUI();
	mCursor.Reset();
	mCleared = true;
	RefreshContext();
}


inline
hel::Vec3 FreyjaControl::GetCursorData(freyja_transform_action_t action)
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


inline
freyja_transform_action_t FreyjaControl::GetEventAction()	
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


inline
void FreyjaControl::InitTexture()
{
	mTexture.reset();
	mTexture.setMaxTextureCount(64);
	mTexture.setFlag(Texture::fUseMipmaps);

	unsigned char rgba[4] = {255, 255, 255, 255};
	mTexture.loadColorTexture(rgba, 32, 32);
	//mTextureId = 1;
}


inline
bool FreyjaControl::LoadTexture(const char *filename, int &id)
{
	if ( mMaterial.LoadTexture(filename) ) 
	{ 
		id = mMaterial.GetTextureId()-1; 
		return true; 
	}

	return false;
}


inline
void FreyjaControl::RecordSavedModel(const char *filename)
{
	mstl::String title;
	title.Set("%s - Freyja", filename);
	freyja_set_main_window_title(title.c_str());
	mCleared = true;
	mCurrentlyOpenFilename = filename;
	mRecentModel.AddFilename(filename);
}


inline
bool FreyjaControl::SaveKeyFrame(char const *filename)
{
	mstl::String s;
	s.Set("%s-keyframe-%i.smd", filename, GetSelectedKeyFrame() );
	return SaveModel( s.c_str() );
}


inline 
void FreyjaControl::SetObjectMode(object_type_t m) 
{ 
	mObjectMode = m; 
	Print("Object mode set to %s...", ObjectTypeToString(m).c_str());
}


} /* End namespace freyja3d */


#endif
