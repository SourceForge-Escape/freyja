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

#ifndef GUARD__FREYJA_MONGOOSE_FREYJACONTROL_H_
#define GUARD__FREYJA_MONGOOSE_FREYJACONTROL_H_

#include <freyja/FreyjaPrinter.h>
#include <freyja/ModelABI.h>
#include <freyja/MeshABI.h>
#include <freyja/BoneABI.h>
#include <mstl/ActionManager.h>
#include <mstl/String.h>
#include <mstl/SystemIO.h>
#include <mstl/Vector.h>
#include <mgtk/Resource.h>

#include "freyja_events.h"
#include "FreyjaRender.h"
#include "Texture.h"
#include "Cursor.h"


namespace freyja3d {

class FreyjaControl
{
 public:

	/* Each of these control schemes will have it's own control class later */
	typedef enum {        
		eScheme_UV = 0,
		eScheme_Model,
		eScheme_Material,
		eScheme_Animation
		
	} ControlScheme;


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


	typedef enum {
		fNone                = 0,
		fPolyMappedTexCoords = 1,
		fDeformBoneVertices  = 2,
		fLoadTextureInSlot   = 4,
		fLoadMaterialInSlot  = 8

	} options_t;


	/* Old event format enums */
	typedef enum {                              /* Editor event modes */
		modeNone = 1,
		POINT_ADD_MODE,
		POINT_DEL_MODE,
		POLYGON_ADD_MODE,
		POLYGON_DEL_MODE,
		BONE_CONNECT_MODE,
		BONE_DISCONNECT_MODE,
		BONE_ADD_MODE,
		TEXEL_COMBINE,
		VERTEX_COMBINE,
		POLYGON_SELECT_MODE,
		
		/* Generic transforms */
		modeMove,
		modeRotate,
		modeScale,
		modeSelect,
		modeSelectByBox,
		modeUnselect,
		modeKeyframe
		
	} EventMode;


	class FreyjaControlPrinter : public FreyjaPrinter
	{
	public:

		virtual void errorArgs(char *format, va_list *args)
		{
			freyja_print_args(format, args);
		}


		virtual void messageArgs(char *format, va_list *args)
		{
			freyja_print_args(format, args);
		}
	};


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

	bool mUsingARBFragments;
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

	const Vec3 &GetSceneTranslation() { return mSceneTrans; }
	void SetSceneTranslation(const Vec3 &v) { mSceneTrans = v; }
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

	uint32 GetSelectedTexture() { return mSelectedTexture; }
	void SetSelectedTexture(uint32 i) { mSelectedTexture = i; }
	uint32 mSelectedTexture;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns currently selected texture index
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
	


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

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

	Resource &GetResource() { return mResource; }
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

	bool event(int event, unsigned int value);
	/*------------------------------------------------------
	 * Pre  : Generic event signal is valid and value is
	 *        valid for the generic event
	 *mControlScheme
	 * Post : Event is handled internally 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.03.22:
	 * Mongoose - Replaces 2000 handleEvent method
	 *
	 * 2004.10.23: 
	 * Mongoose - Created, pulled out of other event methods
	 ------------------------------------------------------*/

	bool event(int event, vec_t value);
	/*------------------------------------------------------
	 * Pre  : Generic event signal is valid and value is
	 *        valid for the generic event
	 *
	 * Post : Event is handled internally 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.10.23: 
	 * Mongoose - Created, pulled out of other event methods
	 ------------------------------------------------------*/

	bool event(int command);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 200X.XX.XX: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	bool handleEvent(int event, int command);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Process event input, such as from a menu
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	static uint32 mSelectedControlPoint;
	static Vector<Vec3> mControlPoints;
	static Vector<Vec3> &GetControlPoints() { return mControlPoints; }
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
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created from GooseEgg
	 ------------------------------------------------------*/

	bool LoadTexture(const char *filename);
	/*------------------------------------------------------
	 * Pre  : Reads material from disk
	 * Post : Returns true if sucessful
	 ------------------------------------------------------*/

	bool LoadTexture(const char *filename, int &id)
	{
		bool b = false;
		if (LoadTexture(filename)) { id = mTextureId-1; b = true; }
		return b;
	}
	/*------------------------------------------------------
	 * Pre  : Reads material from disk
	 * Post : Returns true if sucessful
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

	void SetRenderFlag(FreyjaRender::flags_t flag, bool t, const char *s)
	{
		mRender->Flag(flag, t);
		freyja_print("%s is [%s]", s, t ? "ON" : "OFF");
		freyja_event_gl_refresh();
	}

	void Display()
	{
		FREYJA_ASSERTMSG(mRender, "FreyjaRender Singleton not allocated");
		mRender->Display();
	}


	void HandleResize(uint32 width, uint32 height)
	{
		FREYJA_ASSERTMSG(mRender, "FreyjaRender Singleton not allocated");
		mRender->ResizeContext(width, height);
	}


	void SetFlag(options_t flag, bool t) 
	{ (t) ? mFlags |= flag : mFlags ^= flag; }


	bool ToggleFlag(options_t flag) { mFlags ^= flag; return mFlags & flag; }

	uint32 GetViewMode() { return mRender->GetViewMode(); }

	int GetResourceInt(const char *symbol)
	{
		return (int)GetResourceFloat(symbol);
	}

	void SetResourceInt(const char *symbol, int i)
	{
		return SetResourceFloat(symbol, i);
	}

	float GetResourceFloat(const char *symbol)
	{
		int event;
		mResource.Lookup((char*)symbol, &event);
		return freyja_event_get_float(event);
	}

	void SetResourceFloat(const char *symbol, float r)
	{
		int event;
		mResource.Lookup((char*)symbol, &event);
		return freyja_event_set_float(event, r);
	}

	void GetResourceColor(const char *symbol, 
						  float &r, float &g, float &b, float &a)
	{
		int event;
		mResource.Lookup((char*)symbol, &event);
		freyja_event_get_color(event, r, g, b, a);
	}

	void SetResourceColor(const char *symbol, 
						  float r, float g, float b, float a)
	{
		int event;
		mResource.Lookup((char*)symbol, &event);
		freyja_event_set_color(event, r, g, b, a);
	}

	void SetKeyFrame();

	int GetEventIdByName(const char *symbol)
	{
		return GetResource().GetEventIdByName(symbol);
	}


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
					double *rayOrigin, double *rayVector);
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

	Vec3 GetCursorData(freyja_transform_action_t action)
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/
	{
		Vec3 v;

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
		case modeMove:
			return fTranslate;
			break;

		case modeRotate:
			return fRotate;
			break;

		case modeScale:
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

	void addObject();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.01: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

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
		mTextureId = 1;
	}

	int32 mTextureId;

	void SetFaceMaterial(index_t faceIndex, index_t material);

	void CreatePolyMappedUVMap(int i) {BUG_ME("Not implemented in this build");}

	void CursorMove(float xx, float yy);

	void GetBoneRotation(float *x, float *y, float *z)
	{
		vec3_t xyz;

		freyjaGetBoneRotationEuler3fv(GetSelectedBone(), xyz);
		*x = HEL_RAD_TO_DEG(xyz[0]);
		*y = HEL_RAD_TO_DEG(xyz[1]);
		*z = HEL_RAD_TO_DEG(xyz[2]);
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
		vec3_t xyz = {HEL_DEG_TO_RAD(x), HEL_DEG_TO_RAD(y), HEL_DEG_TO_RAD(z)};
		freyjaBoneRotateEuler3fv(GetSelectedBone(), xyz);
	}

	void SetMaterialForSelectedFaces(uint32 material);

	void Clear()
	{
		freyjaModelClear(0); // Only 1 edit model supported in this bulid
		UpdateSkeletalUI();
		UpdateSkeletonUI_Callback(0); // Update skeletal UI
		mCursor.Reset();
		mCleared = true;
	}

	bool DuplicateSelectedObject();
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

	bool SplitSelectedObject();
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
	 *-- History ------------------------------------------
	 *
	 * 2004.08.01: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	bool CopySelectedObject();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.11.30: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	bool PasteSelectedObject();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.11.30: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

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

	void SelectObjectByBox(Vec3 min, Vec3 max);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void SelectObject(vec_t mouseX, vec_t mouseY);
	void UnselectObject(vec_t mouseX, vec_t mouseY);
	//void SelectObject(vec_t x, vec_t y, freyja_plane_t plane);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.01: 
	 * Mongoose - Created
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


	ActionManager mActionManager;           /* New Undo/Redo system */

	uint32 mFlags;                          /* Option bitflags */

	object_type_t mObjectMode;              /* Current object type to edit */

	Texture mTexture;                       /* Collection of Texture utils */

	const static uint32 mRecentFileLimit = 10;
	Vector<String> mRecentFiles;            /* Recently loaded model files */

	Vector<int32> mUVMap;                   /* 'Texture faces' grouping */

	FreyjaControlPrinter mPrinter;          /* Reroute text msg from backend */

	String mResourceFilename;	            /* Resource file for control */

	String mUserPrefsFilename;	            /* Resource file for prefs */

	String mCurrentlyOpenFilename;          /* What file is loaded now? */

	Vec3 mSceneTrans;                       /* Offset of scene in 3 space */

	freyja3d::Cursor mCursor;               /* Special mouse input handler */

	Resource mResource;                     /* Resource system */
	
	FreyjaRender *mRender;                  /* OpenGL renderer */

	unsigned int mLastEvent, mLastCommand;  /* The last command pair recieved*/

	unsigned int mMouseButton, mModKey;     /* Cached mouse button event */

	int mMouseState;                        /* Cached mouse state event */

	ControlScheme mControlScheme;           /* Current control scheme in use */

	EventMode mEventMode;                   /* Mode of generic event handler */
	
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
