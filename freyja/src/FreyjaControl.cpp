/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : FreyjaControl
 * License : No use w/o permission (C) 2000-2007 Mongoose
 * Comments: This is the controler class for the client
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongooseichiban@gmail.com>
 * 
 *-- TODO ---------------------------------------------------- 
 *
 * Replace the old switches with more callbacks!
 *
 *-- History ------------------------------------------------- 
 *
 * 2000.09.10:
 * Mongoose - Created
 ==========================================================================*/

#include <stdlib.h> 
#include <stdio.h> 
#include <math.h> 
#include <stdarg.h> 
#include <string.h>

#include <mstl/String.h>
#include <mstl/SystemIO.h>
#include <mgtk/ResourceEvent.h>
#include <mgtk/ResourceEventDelegate.h>
#include <mgtk/ConfirmationDialog.h>

#include <hel/math.h>
#include <hel/Ray.h>

#include <freyja/Plugin.h>
#include <freyja/Bone.h>
#include <freyja/Mesh.h>
#include <freyja/FreyjaImage.h>
#include <freyja/Material.h>
#define USING_FREYJA_CPP_ABI
#include <freyja/LightABI.h>
#include <freyja/MaterialABI.h>
#include <freyja/MeshABI.h>
#include <freyja/SkeletonABI.h>
#include <freyja/PluginABI.h>
#include <freyja/TextureABI.h>

#include "freyja_events.h"

#include "FreyjaControl.h"

using namespace mstl;
using namespace freyja;
using namespace freyja3d;

#define DEBUG_PICK_RAY         0
#define DEBUG_PICK_RAY_PLANAR  0
#define DEBUG_SCREEN_TO_WORLD  0
#define DEBUG_VIEWPORT_MOUSE   0

int load_texture(const char *filename);
int load_shader(const char *filename);

FreyjaControl *FreyjaControl::mInstance = NULL;
MaterialEv FreyjaControl::mMaterial;
uint32 FreyjaControl::mSelectedControlPoint = 0;
Vector<hel::Vec3> FreyjaControl::mControlPoints;

uint32 FreyjaControl::eRotateObjectId = 0;
uint32 FreyjaControl::eScaleObjectId = 0;
uint32 FreyjaControl::eMoveObjectId = 0;
uint32 FreyjaControl::eUnselectId = 0;
uint32 FreyjaControl::eSelectId = 0;
uint32 FreyjaControl::eSelectionByBoxId = 0;
uint32 FreyjaControl::eAxisJointId = 0;
uint32 FreyjaControl::eSphereJointId = 0;
uint32 FreyjaControl::ePointJointId = 0;
uint32 FreyjaControl::eInfoObjectId = 0;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

FreyjaControl::FreyjaControl() :
	mGroupBitmap(0x0),
	mSelectedModel(0),
	mSelectedView(PLANE_FREE),
	mSelectedViewport(0),
	mActionManager(),
	mFlags(fNone),
	mObjectMode(tScene),
	mResourceFilename("freyja-dev.mlisp"),
	mUserPrefsFilename("freyja-dev_prefs.mlisp"),
	mCurrentlyOpenFilename(),
	mSceneTrans(0.0f, -18.0f, 0.0f),
	mRender(NULL),
	mCleared(true),
	mAllowBoneNameUpdate(true),
	mToken(false)
{
	/* Search local paths first ( mostly debugging ) */
	//freyjaPluginAddDirectory("plugins/model/debug");
	/* Search local paths for things like windows builds */
	//freyjaPluginAddDirectory("modules/model");

	/* Search ~/.freyja/plugins/ second ( first for real path for end users ) */
	String sPluginDir = freyja_rc_map_string("plugins/");
	freyjaPluginAddDirectory(sPluginDir.GetCString());

	/* Start up libfreyja backend, and redirect it's stdio */
	freyjaSpawn();
	extern void freyja__setPrinter(Printer *printer, bool freyjaManaged);
	freyja__setPrinter(&mPrinter, false);

	/* Spawn 0th light, and set the light iterator */
	vec4_t lightPos = {12,35,8,0}; // Make this RCed
	freyjaCurrentLight(freyjaLightCreate());
	freyjaLightPosition4v(freyjaGetCurrentLight(), lightPos);

	/* Spawn 0th material, set the iterator, and make a default material */
	Material::mLoadTextureFunc = load_texture;
	Material::mLoadShaderFunc = load_shader;
	int32 mIndex = freyjaMaterialCreate();
	vec4_t rgba = {0,0,0,1};
	freyjaCurrentMaterial(mIndex);
	freyjaMaterialName(mIndex, "Boring default");
	freyjaMaterialEmissive(mIndex, rgba);
	rgba[0] = rgba[1] = rgba[2] = 0.9;
	freyjaMaterialDiffuse(mIndex, rgba);
	rgba[0] = rgba[1] = rgba[2] = 0.2;
	freyjaMaterialAmbient(mIndex, rgba);
	rgba[0] = rgba[1] = rgba[2] = 0.2;
	freyjaMaterialSpecular(mIndex, rgba);
	freyjaMaterialShininess(mIndex, 32.0f);

	/* Hook up the view */
	mRender = FreyjaRender::GetInstance();
	FREYJA_ASSERTMSG(mRender, "FreyjaRender Singleton control allocation failure");

	if (!mRender)
	{
		SystemIO::Print("See ~/.freyja/Freyja.log for possible errors.\n");
		exit(-1);
	}
}


void FreyjaControl::Init()
{
	/* Build the user interface from lisp, and load user preferences */
	LoadResource();

	/* Load up recent files list into file menu */
	freyja_print("Loading %s...", FREYJA_RECENT_FILES);
	bool b = LoadRecentFilesResource(freyja_rc_map_string(FREYJA_RECENT_FILES).GetCString());
	freyja_print("Loading %s %s", FREYJA_RECENT_FILES, b ? "successful" : "failed");

	/* Set some basic defaults */
	SetControlScheme(eScheme_Model);
	SetZoom(1.0f);
	mEventMode = aNone;
	mLastEvent = eEvent;
	mLastCommand = eSelectId;
	mFullScreen = false;

	/* Mongoose 2002.02.23, Tell renderer to start up with some defaults */
	uint32 width = 740;
	uint32 height = 560;
	mRender->InitContext(width, height, true);
	mRender->ResizeContext(width, height);
	InitTexture();

	
	// Init OpenGLPrinter, move along with mTexture to OpenGL facade
	//const char *font = "/home/mongoose/.fonts/tahoma.ttf";
	char *font;
	if (mResource.Lookup("FONT", &font))
	{
		const unsigned int pt = 24, dpi = 100;
		mstl::String s;

		// If this isn't a full path filename look for file in rc_map
		if (!SystemIO::File::DoesFileExist(font))
		{
			s = freyja_rc_map_string(font);
			font = (char *)s.c_str();
		}

		if (!mRender->mPrinter.Init(font, pt, dpi))
		{
			FREYJA_ASSERTMSG(false, "Failed to load font '%s' @ %ipt %idpi.",
							 font, pt, dpi);
		}
	}
	else
	{
		FREYJA_ASSERTMSG(false, "No FONT symbol found in mlisp resource.");
	}


	// Handle loaded from system call
	if (!freyjaGetBoneCount() && !freyjaGetMeshCount())
		mCleared = true;
	else
		mCleared = false;

	mGenMeshHeight = 8.0f;
	mGenMeshCount = 16;
	mGenMeshSegements = 4;
}


FreyjaControl::~FreyjaControl()
{
	if (mRender)
		delete mRender;
}



////////////////////////////////////////////////////////////
// Public Properties
////////////////////////////////////////////////////////////

void FreyjaControl::SetFaceMaterial(index_t faceIndex, index_t material)
{
	if (!mUVMap.empty())
	{
		for (uint32 i = mUVMap.begin(); i < mUVMap.end(); ++i)
		{
			freyjaMeshPolygonMaterial(GetSelectedMesh(), mUVMap[i], material);
		}

		return;
	}

	freyjaMeshPolygonMaterial(GetSelectedMesh(), faceIndex, material);
}


float FreyjaControl::GetZoom()
{
	return mRender->GetZoom();
}


void FreyjaControl::SetZoom(float zoom)
{
	FREYJA_ASSERTMSG(zoom > 0.0f, "Zoom values must be greater than 0.0f");
	//freyja_event_notify_observer1f(eZoomId, zoom); // Update widgets watching
	mRender->SetZoom(zoom);
	freyja_print("Zoom %f", mRender->GetZoom());
	freyja_event_gl_refresh();
}



////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

void FreyjaControl::AttachMethodListeners()
{
	mMaterial.AttachMethodListeners();

	// CreateListener("", &FreyjaControl::);

	CreateListener("eSaveModel", &FreyjaControl::eSaveModel);
	CreateListener("eOpenModel", &FreyjaControl::eOpenModel);

	CreateListener("ePolygonSplit", &FreyjaControl::ePolygonSplit);
	CreateListener("eSetMeshTexture", &FreyjaControl::eSetMeshTexture);
	CreateListener("eSetFacesMaterial", &FreyjaControl::eSetFacesMaterial);
	CreateListener("eSetPolygonTexture", &FreyjaControl::eSetPolygonTexture);

	CreateListener("eZoom", &FreyjaControl::SetZoom);

	CreateListener("eGenMeshHeight", &FreyjaControl::SetGenMeshHeight);
	CreateListener("eGenMeshCount", &FreyjaControl::SetGenMeshCount);
	CreateListener("eGenMeshSegements", &FreyjaControl::SetGenMeshSegements);

	CreateListener("eAnimationSlider", &FreyjaControl::eAnimationSlider);

	CreateListener("eInfo", &FreyjaControl::PrintInfo);
	CreateListener("eFullscreen", &FreyjaControl::Fullscreen);
	CreateListener("eScreenShot", &FreyjaControl::eScreenShot);

	CreateListener("eVertexCombine", &FreyjaControl::VertexCombine);
	CreateListener("eTexcoordCombine", &FreyjaControl::TexcoordCombine);
	CreateListener("eSetKeyFrame", &FreyjaControl::SetKeyFrame);

	CreateListener("eCloseFile", &FreyjaControl::CloseFile);
	CreateListener("eNewFile", &FreyjaControl::NewFile);
	CreateListener("eExportFile", &FreyjaControl::ExportFile);
	CreateListener("eImportFile", &FreyjaControl::ImportFile);
	CreateListener("eSaveFile", &FreyjaControl::SaveFile);
	CreateListener("eOpenFile", &FreyjaControl::OpenFile);
	CreateListener("eSaveFileModel", &FreyjaControl::SaveFileModel);
	CreateListener("eOpenFileModel", &FreyjaControl::OpenFileModel);
	CreateListener("eRevertFile", &FreyjaControl::RevertFile);

	CreateListener("eUndo", &FreyjaControl::Undo);
	CreateListener("eRedo", &FreyjaControl::Redo);

	CreateListener("eShutdown", &FreyjaControl::Shutdown);

	CreateListener("eDelete", &FreyjaControl::DeleteSelectedObject);
	CreateListener("eCreate", &FreyjaControl::CreateObject);
	CreateListener("eDupeObject", &FreyjaControl::DuplicateSelectedObject);
	CreateListener("eMergeObject", &FreyjaControl::MergeSelectedObjects);
	CreateListener("eSplitObject", &FreyjaControl::SplitSelectedObject);
	CreateListener("ePaste", &FreyjaControl::PasteSelectedObject);
	CreateListener("eCopy", &FreyjaControl::CopySelectedObject);
	CreateListener("eCut", &FreyjaControl::Cut);

	CreateListener("eMeshNew", &FreyjaControl::eMeshNew);
	CreateListener("eMeshDelete", &FreyjaControl::eMeshDelete);
	CreateListener("eMeshSelect", &FreyjaControl::eMeshSelect);

	CreateListener("eScale", &FreyjaControl::eScale);
	CreateListener("eMove", &FreyjaControl::eMove);
	CreateListener("eRotate", &FreyjaControl::eRotate);

	CreateListener("eModeUV", &FreyjaControl::eModeUV);
	CreateListener("eModeModel", &FreyjaControl::eModeModel);
	CreateListener("eModeMaterial", &FreyjaControl::eModeMaterial);

	//CreateListener("eModelUpload", &FreyjaControl::eModelUpload);

	CreateListener("eViewportBack", &FreyjaControl::eViewportBack);
	CreateListener("eViewportBottom", &FreyjaControl::eViewportBottom);
	CreateListener("eViewportRight", &FreyjaControl::eViewportRight);
	CreateListener("eViewportFront", &FreyjaControl::eViewportFront);
	CreateListener("eViewportTop", &FreyjaControl::eViewportTop);
	CreateListener("eViewportLeft", &FreyjaControl::eViewportLeft);
	CreateListener("eViewportOrbit", &FreyjaControl::eViewportOrbit);
	CreateListener("eViewportUV", &FreyjaControl::eViewportUV);
	CreateListener("eViewportCurve", &FreyjaControl::eViewportCurve);
	CreateListener("eViewportMaterial", &FreyjaControl::eViewportMaterial);

	CreateListener("eTransformScene", &FreyjaControl::eTransformScene);
	CreateListener("eTransformVertices", &FreyjaControl::eTransformVertices);
	CreateListener("eTransformVertex", &FreyjaControl::eTransformVertex);
	CreateListener("eTransformMeshes", &FreyjaControl::eTransformMeshes);
	CreateListener("eTransformMesh", &FreyjaControl::eTransformMesh);
	CreateListener("eTransformFaces", &FreyjaControl::eTransformFaces);
	CreateListener("eTransformFace", &FreyjaControl::eTransformFace);
	CreateListener("eTransformModel", &FreyjaControl::eTransformModel);
	CreateListener("eTransformBone", &FreyjaControl::eTransformBone);
	CreateListener("eTransformLight", &FreyjaControl::eTransformLight);

	CreateListener("eRecentFiles", &FreyjaControl::eRecentFiles);

	/* One Argument callbacks with cached Ids */

	CreateListener("eRotateObject", &FreyjaControl::eRotateObject);
	eRotateObjectId = ResourceEvent::GetResourceIdBySymbol("eRotateObject");

	CreateListener("eScaleObject", &FreyjaControl::eScaleObject);
	eScaleObjectId = ResourceEvent::GetResourceIdBySymbol("eScaleObject");

	CreateListener("eMoveObject", &FreyjaControl::eMoveObject);
	eMoveObjectId = ResourceEvent::GetResourceIdBySymbol("eMoveObject");

	CreateListener("eUnselect", &FreyjaControl::eUnselect);
	eUnselectId = ResourceEvent::GetResourceIdBySymbol("eUnselect");

	CreateListener("eSelect", &FreyjaControl::eSelect);
	eSelectId = ResourceEvent::GetResourceIdBySymbol("eSelect");

	CreateListener("eInfoObject", &FreyjaControl::eInfoObject);
	eInfoObjectId = ResourceEvent::GetResourceIdBySymbol("eInfoObject");

	CreateListener("eSelectionByBox", &FreyjaControl::eSelectionByBox);
	eSelectionByBoxId = ResourceEvent::GetResourceIdBySymbol("eSelectionByBox");

	CreateListener("eAxisJoint", &FreyjaControl::eAxisJoint);
	eAxisJointId = ResourceEvent::GetResourceIdBySymbol("eAxisJoint");
	CreateListener("eSphereJoint", &FreyjaControl::eSphereJoint);
	eSphereJointId = ResourceEvent::GetResourceIdBySymbol("eSphereJoint");
	CreateListener("ePointJoint", &FreyjaControl::ePointJoint);
	ePointJointId = ResourceEvent::GetResourceIdBySymbol("ePointJoint");
}


// Mongoose - 2006.07.31 
// More crap for old system to be backported then rewritten properly  =/
void FreyjaControl::AdjustMouseXYForViewports(vec_t &x, vec_t &y)
{
#if DEBUG_VIEWPORT_MOUSE
	freyja_print("> Mouse x = %f y = %f", x, y);
#endif

	// FIXME: Add UV editor / Curve support here before breaking these
	// into seperate control classes.  Also Update the view pick drop down menu.

	// Trap the junk 0,0 states that are often tossed around on
	// just mouse button updates
	if ( GetControlScheme() != eScheme_Model &&
		 GetControlScheme() != eScheme_Animation || 
		 x == 0 && y == 0 )
	{
	}
	else if (mRender->GetMode() & FreyjaRender::fViewports)
	{
		// Translate this to it's correct 'plane editing mode'
		// for the viewport and adjust the x, y values here 
		// to reuse the old mouse style handling
		// In other words - just make this work for now
		vec_t h = mRender->GetWindowHeight();
		vec_t w = mRender->GetWindowWidth();
		vec_t halfW = w * 0.5f;
		vec_t halfH = h * 0.5f;
		
		// Handle viewport 0
		if ( x < halfW && y > halfH )
		{
			SetSelectedViewport(0);
			// freyja_print("Selected viewport 0");
			SetSelectedView(mRender->mViewports[0].plane);

			// Adjust actual window space mouse x, y to fit to viewport
			// This makes the x, y fill the 'window' for the viewport
			x = x*2.0f;
			y = y*2.0f - h;
		}
		// Handle viewport 2
		else if ( x > halfW && y < halfH )
		{
			SetSelectedViewport(2);
			// freyja_print("Selected viewport 2");
			SetSelectedView(mRender->mViewports[2].plane);

			// Adjust actual window space mouse x, y to fit to viewport
			// This makes the x, y fill the 'window' for the viewport
			x = x*2.0f - w;
			y = y*2.0f;
		}
		// Handle viewport 1
		else if ( x > halfW && y > halfH )
		{
			SetSelectedViewport(1);
			// freyja_print("Selected viewport 1");
			SetSelectedView(mRender->mViewports[1].plane);

			// Adjust actual window space mouse x, y to fit to viewport
			// This makes the x, y fill the 'window' for the viewport
			x = x*2.0f - w;
			y = y*2.0f - h;
		}
		// Handle viewport 3
		else if ( x < halfW && y < halfH )
		{
			SetSelectedViewport(3);
			// freyja_print("Selected viewport 3");
			SetSelectedView(mRender->mViewports[3].plane); // PLANE_FREE

			// Adjust actual window space mouse x, y to fit to viewport
			// This makes the x, y fill the 'window' for the viewport
			x = x*2.0f;
			y = y*2.0f;
		}

#if DEBUG_VIEWPORT_MOUSE
		freyja_print("       x = %f y = %f", x, y);
#endif
	}
}


String FreyjaControl::ObjectTypeToString(object_type_t t)
{
	String s;

	switch (t)
	{
	case tNone:
		s = String("tNone");
		break;

	case tPoint:
		s = String("tPoint");
		break;

	case tFace:
		s = String("tFace");
		break;

	case tModel:
		s = String("tModel");
		break;

	case tMesh:
		s = String("tMesh");
		break;

	case tBone:
		s = String("tBone");
		break;

	case tKeyFrame:
		s = String("tKeyFrame");
		break;

	case tLight:
		s = String("tLight");
		break;

	case tSkeleton:
		s = String("tSkeleton");
		break;

	case tScene:
		s = String("tScene");
		break;

	case tSelectedVertices:
		s = String("tSelectedVertices");
		break;

	case tSelectedFaces:
		s = String("tSelectedFaces");
		break;

	case tSelectedBones:
		s = String("tSelectedBones");
		break;

	case tSelectedMeshes:
		s = String("tSelectedMeshes");
		break;

	default:
		s = String("Unknown");		
	}

	return s;
}



////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void FreyjaControl::ActionModelModified(Action *action)
{
	if (action)
	{
		//static Freyja3dCursor::Flags lastMode;
		//lastMode = mCursor.GetMode();

		if (mToken)
		{
			mActionManager.Push(action);
		}
		else // This is an extranous event if it doesn't have a token
		{
			DEBUG_MSG("Extranous event dropped, so action is deleted.");
			delete action;
		}
	}

	mCleared = false;
	mToken = false;
}


void FreyjaControl::AddRecentFilename(const char *filename)
{
	bool found = false;
	uint32 idx;


	if (!filename || !filename[0] || 
		!SystemIO::File::DoesFileExist(filename))
	{
		return;
	}

	for (uint32 i = mRecentFiles.begin(), n = mRecentFiles.end(); i < n; ++i)
	{
		if (strcmp(filename, mRecentFiles[i].GetCString()) == 0)
		{
			idx = i;
			found = true;
			break;
		}
	}


	/* 'Boost' this file to top slot, push others down one */
	if (found)
	{
		// Already at top, no change to menu
		if (idx == 0)
			return;

		String swap, old;
		uint32 n = mRecentFiles.end();
		swap = mRecentFiles[0];
		mRecentFiles[0] = mRecentFiles[idx];

		for (uint32 i = 1; i < n; ++i)
		{
			if (i > idx)
			{
				break;
			}

			old = mRecentFiles[i];
			mRecentFiles[i] = swap;
			swap = old;
		}
	}
	else  /* Add new file to top slot, push others down one */
	{
		String swap, old;
		String insert(filename);

		// Bubble up hack
		if (mRecentFiles.end() >= mRecentFileLimit)
		{
			swap = insert;
			uint32 n = mRecentFiles.end();
			for (uint32 i = mRecentFiles.begin(); i < n; ++i)
			{				
				old = mRecentFiles[i];
				mRecentFiles[i] = swap;
				swap = old;
			}
		}
		else
		{
			mRecentFiles.pushBack(insert);
		}
	}

	/* Rebuild menu in order of mRecentFiles */
	uint32 menuId = Resource::mInstance->getIntByName("eRecentFiles");
	freyja_remove_all_items_from_menu(menuId);
		
	uint32 n = mRecentFiles.end();
	for (uint32 i = mRecentFiles.begin(); i < n; ++i)
	{
		mgtk_append_item_to_menu2i(menuId, mRecentFiles[i].GetCString(), menuId, i);
	}


	/* Save recent_files to disk */
	SystemIO::TextFileWriter w;

	if (w.Open(freyja_rc_map_string(FREYJA_RECENT_FILES).GetCString()))
	{
		String swap;
		uint32 n = mRecentFiles.end();
		for (uint32 i = mRecentFiles.begin(); i < n; ++i)
		{
			swap = mRecentFiles[i];
			
			if (!swap.Empty())
			{
				w.Print("%s\n", swap.GetCString());
			}
		}

		w.Close();
	}
}


void FreyjaControl::CastPickRay(vec_t x, vec_t y)
{
	hel::Ray &r = mRender->mTestRay;

	if (!(GetControlScheme() == eScheme_Model ||
		  GetControlScheme() == eScheme_Animation))
		return;

	if (GetSelectedView() == PLANE_FREE)
	{
		// stop gap fix until new camera system is checked in ( so some things can be tested -- like the camera )
		vec_t z;
		GetWorldFromScreen(x, y, z);
		hel::Mat44 m;
		vec3_t v;
		z += 100;

		x -= GetSceneTranslation().mVec[0] * 1/GetZoom();
		y -= GetSceneTranslation().mVec[1] * 1/GetZoom();
		z -= GetSceneTranslation().mVec[2] * 1/GetZoom();
		vec3_t u = {x, y, z};
		mRender->GetRotation(v);
		v[0] = helDegToRad(v[0]);
		v[1] = helDegToRad(v[1]);
		v[2] = helDegToRad(v[2]);
		//DEBUG_MSGF("$$ yaw = %f\n", v[1]);

		m.Rotate(v);
		m.Multiply3fv(u);
		r.mOrigin = hel::Vec3(u);
		r.mDir = hel::Vec3(0, 0, -1);
		m.Multiply3fv(r.mDir.mVec);
	}
	else
	{
		vec_t z;

		GetWorldFromScreen(x, y, z);

		switch (GetSelectedView())
		{
		case PLANE_BACK:
			r.mOrigin = hel::Vec3(x, y, z - 100);
			r.mDir = hel::Vec3(0, 0, 1);
			break;

		case PLANE_FRONT: // Front, XY
			r.mOrigin = hel::Vec3(x, y, z + 100);
			r.mDir = hel::Vec3(0, 0, -1);
			break;

		case PLANE_BOTTOM:
			r.mOrigin = hel::Vec3(x, y - 100, -z);
			r.mDir = hel::Vec3(0, 1, 0);
			break;

		case PLANE_TOP: // Top, XZ
			r.mOrigin = hel::Vec3(x, y + 100, -z);
			r.mDir = hel::Vec3(0, -1, 0);
			break;

		case PLANE_RIGHT:
			r.mOrigin = hel::Vec3(x + 100, y, z);
			r.mDir = hel::Vec3(-1, 0, 0);
			break;

		case PLANE_LEFT: // Side, ZY
			r.mOrigin = hel::Vec3(x - 100, y, z);
			r.mDir = hel::Vec3(1, 0, 0);
			break;

		default:
			;
		}	
	}

	//mRender->setFlag(FreyjaRender::fDrawPickRay);

#if DEBUG_PICK_RAY_PLANAR
	// DEBUG case is a triangle ABC...
	Vec3 a(0,8,0), b(8,0,0), c(8,8,0), i; // test facex
	//const vec_t k = 100.0f;
	//Vec3 a(0,0,0), b(k,0,0), c(0,k,0), i; // test facex
	bool s = r.IntersectTriangle(a.mVec,b.mVec,c.mVec,i);


	freyja_print("! %s, o %f %f %f, d %f %f %f => %f %f %f",
				 s ? "true" : "false",
				 r.mOrigin.mVec[0],r.mOrigin.mVec[1],r.mOrigin.mVec[2],
				 r.mDir.mVec[0],r.mDir.mVec[1],r.mDir.mVec[2],
				 i.mVec[0],i.mVec[1],i.mVec[2]);
#endif
}


void FreyjaControl::CursorMove(float xx, float yy)
{
	mCursor.SetMode( freyja3d::Cursor::Translation );

	switch (GetSelectedView())
	{
	case PLANE_FRONT: // xy
		switch ( mCursor.mAxis )
		{
		case freyja3d::Cursor::eNone:
			break;
		case freyja3d::Cursor::eAll:
			mCursor.mPos.mVec[0] = xx;  // side to side on screen
			mCursor.mPos.mVec[1] = yy; // up and down on screen
			break;
		case freyja3d::Cursor::eX:
			mCursor.mPos.mVec[0] = xx;  // side to side on screen
			break;
		case freyja3d::Cursor::eY:
			mCursor.mPos.mVec[1] = yy; // up and down on screen
			break;
		case freyja3d::Cursor::eZ:
			// In and out of screen by up and down mouse
			mCursor.mPos.mVec[2] = yy;
			break;
		}
		break;

	case PLANE_TOP: // xz
		switch ( mCursor.mAxis )
		{
		case freyja3d::Cursor::eNone:
			break;
		case freyja3d::Cursor::eAll:
			mCursor.mPos.mVec[0] = xx;  // side to side on screen
			mCursor.mPos.mVec[2] = yy; // up and down on screen
			break;
		case freyja3d::Cursor::eX:
			mCursor.mPos.mVec[0] = xx;  // side to side on screen
			break;
		case freyja3d::Cursor::eY:
			// In and out of screen by up and down mouse
			mCursor.mPos.mVec[1] = yy;
			break;
		case freyja3d::Cursor::eZ:
			mCursor.mPos.mVec[2] = yy; // up and down on screen
			break;
		}
		break;

	case PLANE_LEFT: // zy
		switch ( mCursor.mAxis )
		{
		case freyja3d::Cursor::eNone:
			break;
		case freyja3d::Cursor::eAll:
			mCursor.mPos.mVec[2] = xx;  // side to side on screen
			mCursor.mPos.mVec[1] = yy; // up and down on screen
			break;
		case freyja3d::Cursor::eX:
			// In and out of screen by up and down mouse
			mCursor.mPos.mVec[0] = yy;
			break;
		case freyja3d::Cursor::eY:
			mCursor.mPos.mVec[1] = yy; // up and down on screen
			break;
		case freyja3d::Cursor::eZ:
			mCursor.mPos.mVec[2] = xx; // side to side on screen
			break;
		}
		break;
	default: ;
	}	
}


bool FreyjaControl::LoadModel(const char *filename)
{
	int err = freyjaImportModel(filename); 

	if (freyjaGetCurrentSkeleton() == INDEX_INVALID &&
	    freyjaGetSkeletonCount() > 0)
	{
		freyjaCurrentSkeleton(0);
	}

	UpdateSkeletalUI();		
 
	if (err)
	{
		freyja_print("ERROR: File '%s' not found or unknown format\n", filename);
		freyja_print("ERROR CODE %i\n", err);
		return false;
	}
	else
	{
		// 2006.12.09, Not forcing clamping by default -- might revert later
		//freyjaModelClampTexCoords(0); // Only support 1 model per edit atm

		unsigned int i, w, h, bpp, type, count;
		unsigned char *image = 0x0;


		count = freyjaGetTexturePoolCount();
	 
		/* Texture image was stored as raw buffer */
		for (i = 0; i < count; ++i)
		{
			freyjaGetTextureImage(i, w, h, bpp, type, image);

			if (!image)
				continue;

			switch (type)
			{
			case RGBA_32:
				mMaterial.LoadTextureBuffer(image, w, h, 32, 4);
				break;

			case RGB_24:
				mMaterial.LoadTextureBuffer(image, w, h, 24, 3);
				break;

			case INDEXED_8:
				mMaterial.LoadTextureBuffer(image, w, h, 8, 1);
				break;

			default:
				freyja_print("%s> ERROR: Unsupported texture colormap %d",
							"FreyjaModel::loadModel", type);
			}

			freyjaTextureDelete(i); // hhhmmm
		}
	}

	String title;
	title.Set("%s - Freyja", filename);
	freyja_set_main_window_title((char *)title.GetCString());
	AddRecentFilename(filename);
	mCurrentlyOpenFilename = String(filename);
	mCleared = false;

	// FIXME: Only do this for vfspak open
	MSTL_MSG("*** VFS plugin 'auto load test' enabled\n");
	uint32 id = ResourceEvent::GetResourceIdBySymbol("eDialogPakReader");
	ResourceEvent::listen(id);
	id = ResourceEvent::GetResourceIdBySymbol("ePakReaderMenuUpdate");
	ResourceEvent::listen(id);

	return true;
}


bool FreyjaControl::SaveModel(const char *filename, const char *plugin)
{
	if (!filename || !filename[0])
		return false;
	
	bool ret = false;
	String desc = plugin, select = filename;
	freyja::PluginDesc *module = NULL;

	for (uint32 i = 0, count = freyjaGetPluginCount(); i < count; ++i)
	{
		freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(i);
			
		if (plugin == NULL)
			continue;

		if (plugin->mExportFlags && desc == plugin->mDescription)
		{
			//plugin->mDescription.c_str();
			//plugin->mExtention.c_str();
			//plugin->mName.c_str();
			//plugin->mFilename.c_str();
			module = plugin;
			break;
		}
	}


	/* Magically generate dialogs for import/export settings per module */
	if (module)
	{
		// FIXME: Convert this crap to a generated QueryDialog

		//freyja_print("! *** %s", module->mFilename.c_str());
		//freyja_print("! *** %s", module->mName.c_str());

		uint32 i;
		foreach (module->mArgs, i)
		{
			if (module->mArgs[i].GetStringType() == "float")
			{
				float r = module->GetFloatArg(module->mArgs[i].GetName());
				r = mgtk_create_query_dialog_float("gtk-dialog-question",
												   (char*)module->mArgs[i].GetName(),
												   r, -9999, 9999, 
												   1, 4);
				//module->SetFloatArg(module->mArgs[i].GetName(), r)
			}
		}
	}

	if (SystemIO::File::DoesFileExist(select.c_str()) &&
		!mgtk::ExecuteConfirmationDialog("OverwriteFileDialog") )
	{
		ret = true; // 2007.04.01: Counts as a save for UI purposes 
	}
	else if (module &&
			 freyjaExportModelByModule(select.c_str(), module->mFilename.c_str()) == 0)
	{
		String title;
		title.Set("%s - Freyja", select.c_str());
		freyja_set_main_window_title((char*)title.c_str());
		mCurrentlyOpenFilename = select;
		mCleared = true;
		ret = true;
		AddRecentFilename(filename);
	}
	else
	{
		ret = SaveModel(filename);
	}

	return ret;
}


bool FreyjaControl::SaveModel(const char *filename)
{
	if (!filename || !filename[0])
		return false;

	unsigned int len = strlen(filename);
	unsigned int cur;
	for (cur = len; cur > 0; --cur)
	{
		if (filename[cur] == '.')
			break;
	}

	String select = filename;
	bool ret = false;

	if (!cur) /* No extention given, so make it '<filename>.freyja' */
	{
		select += ".freyja";
		cur = len + 1; // Skip '.'
	}
	else
	{
		select = filename;
		++cur; // Skip '.'
	}
 
	if (SystemIO::File::DoesFileExist(select.c_str()) &&
		!mgtk::ExecuteConfirmationDialog("OverwriteFileDialog") )
	{
		ret = false;
	}
	else if (freyjaExportModel(select.c_str(), select.c_str()+cur) == 0)
	{
		String title;
		title.Set("%s - Freyja", select.c_str());
		freyja_set_main_window_title((char*)title.c_str());
		mCurrentlyOpenFilename = select;
		mCleared = true;
		ret = true;
		AddRecentFilename(filename);
	}
	else
	{
		freyja_print("Unable to save with extention '.%s', try using '.freyja'", 
					 select.c_str()+cur);
		ret = false;
	}

	return ret;
}


void FreyjaControl::eViewportBack()
{
	freyja_print("Back view");
	SetSelectedView(PLANE_BACK);
	if (mRender->GetFlags() & FreyjaRender::fViewports)
		mRender->mViewports[mSelectedViewport].plane = PLANE_BACK;
	freyja_event_gl_refresh();
}

void FreyjaControl::eViewportBottom()
{
	freyja_print("Bottom view");  
	SetSelectedView(PLANE_BOTTOM);
	if (mRender->GetFlags() & FreyjaRender::fViewports)
		mRender->mViewports[mSelectedViewport].plane = PLANE_BOTTOM;
	freyja_event_gl_refresh();
}

void FreyjaControl::eViewportRight()
{
	freyja_print("Right view");
	SetSelectedView(PLANE_RIGHT);
	if (mRender->GetFlags() & FreyjaRender::fViewports)
		mRender->mViewports[mSelectedViewport].plane = PLANE_RIGHT;
	freyja_event_gl_refresh();
}

void FreyjaControl::eViewportFront()
{
	freyja_print("Front view");
	SetSelectedView(PLANE_FRONT);
	if (mRender->GetFlags() & FreyjaRender::fViewports)
		mRender->mViewports[mSelectedViewport].plane = PLANE_FRONT;
	freyja_event_gl_refresh();
}

void FreyjaControl::eViewportTop()
{
	freyja_print("Top view");  
	SetSelectedView(PLANE_TOP);
	if (mRender->GetFlags() & FreyjaRender::fViewports)
		mRender->mViewports[mSelectedViewport].plane = PLANE_TOP;
	freyja_event_gl_refresh();
}

void FreyjaControl::eViewportLeft()
{
	freyja_print("Left view");
	SetSelectedView(PLANE_LEFT);
	if (mRender->GetFlags() & FreyjaRender::fViewports)
		mRender->mViewports[mSelectedViewport].plane = PLANE_LEFT;
	freyja_event_gl_refresh();
}

void FreyjaControl::eViewportOrbit()
{
	freyja_print("Orbital view");
	SetSelectedView(PLANE_FREE);
	if (mRender->GetFlags() & FreyjaRender::fViewports)
		mRender->mViewports[mSelectedViewport].plane = PLANE_FREE;
	freyja_event_gl_refresh();
}

void FreyjaControl::eViewportUV()
{
	freyja_print("UV Editor view");
	SetSelectedView(DRAW_UV);
	if (mRender->GetFlags() & FreyjaRender::fViewports)
		mRender->mViewports[mSelectedViewport].plane = DRAW_UV;
	freyja_event_gl_refresh();
}

void FreyjaControl::eViewportCurve()
{
	freyja_print("Curve editor view");
	SetSelectedView(DRAW_CURVE);
	if (mRender->GetFlags() & FreyjaRender::fViewports)
		mRender->mViewports[mSelectedViewport].plane = DRAW_CURVE;
	freyja_event_gl_refresh();
}

void FreyjaControl::eViewportMaterial()
{
	freyja_print("Material preview");
	SetSelectedView(DRAW_MATERIAL);
	if (mRender->GetFlags() & FreyjaRender::fViewports)
		mRender->mViewports[mSelectedViewport].plane = DRAW_MATERIAL;
	freyja_event_gl_refresh();
}


void FreyjaControl::eMove()
{
	mToken = true;
	Transform(mObjectMode, fTranslate,
			  freyja_event_get_float(eMove_X),
			  freyja_event_get_float(eMove_Y),
			  freyja_event_get_float(eMove_Z));

	freyja_event_set_float(eMove_X, 0.0f);
	freyja_event_set_float(eMove_Y, 0.0f);
	freyja_event_set_float(eMove_Z, 0.0f);
	freyja_event_gl_refresh();
}

void FreyjaControl::eRotate()
{
	mToken = true;
	Transform(mObjectMode, fRotate,
			  freyja_event_get_float(eRotate_X),
			  freyja_event_get_float(eRotate_Y),
			  freyja_event_get_float(eRotate_Z));
		
	freyja_event_set_float(eRotate_X, 0.0f);
	freyja_event_set_float(eRotate_Y, 0.0f);
	freyja_event_set_float(eRotate_Z, 0.0f);
	freyja_event_gl_refresh();
}

void FreyjaControl::eScale()
{
	mToken = true;
	Transform(mObjectMode, fScale,
			  freyja_event_get_float(eScale_X),
			  freyja_event_get_float(eScale_Y),
			  freyja_event_get_float(eScale_Z));
		
	freyja_event_set_float(eScale_X, 1.0f);
	freyja_event_set_float(eScale_Y, 1.0f);
	freyja_event_set_float(eScale_Z, 1.0f);
	freyja_event_gl_refresh();
}

void FreyjaControl::eModeUV()
{
	mRender->SetViewMode(VIEWMODE_TEXTURE_EDIT);
	freyja_event_gl_refresh();
	freyja_print("UV Editor Scheme");
	SetControlScheme(eScheme_UV);
}

void FreyjaControl::eModeModel()
{
	// Radio button like func for multiple widgets on same event
	mgtk_toggle_value_set(eModeAutoKeyframe, 0);

	mRender->SetViewMode(VIEWMODE_MODEL_EDIT);
	freyja_event_gl_refresh();
	freyja_print("Model Editor Scheme");
	SetControlScheme(eScheme_Model);
}

void FreyjaControl::eModeMaterial()
{
	mRender->SetViewMode(VIEWMODE_MATERIAL_EDIT);
	freyja_event_gl_refresh();
	freyja_print("Material Editor Scheme");
	SetControlScheme(eScheme_Material);
}

void FreyjaControl::eMeshNew()
{
	mObjectMode = tMesh;
	CreateObject();
	Dirty();
}

void FreyjaControl::eMeshDelete()
{
	mObjectMode = tMesh;
	DeleteSelectedObject();
	Dirty();
}

void FreyjaControl::eMeshSelect()
{
	mObjectMode = tMesh;
	mEventMode = aSelect;
	freyja_print("Select object mesh...");
}

void FreyjaControl::Undo()
{
	if (mActionManager.Undo())
	{
		freyja_event_gl_refresh();
	}
	else
	{
		freyja_print("Undo system has no more actions to revert.");
	}		
}


void FreyjaControl::Redo()
{
	freyja_print("Redo is not avalible in this build.");
}


void FreyjaControl::Cut()
{
	if (CopySelectedObject())
		DeleteSelectedObject();
	freyja_event_gl_refresh();
}


void FreyjaControl::CloseFile()
{
	if (mgtk::ExecuteConfirmationDialog("CloseNewFileDialog"))
	{
		Clear();
		freyja_print("Closing Model...");
		freyja_set_main_window_title(BUILD_NAME);
		mCurrentlyOpenFilename = String();
		mCleared = true;
	}
}


void FreyjaControl::NewFile()
{
	switch (GetControlScheme())
	{
	case eScheme_Animation:
		freyja_print("!(%s:%i) %s: eScheme_Animation, Not implemented", 
					 __FILE__, __LINE__, __func__);
		break;
		
	case eScheme_Model:
		if (mgtk::ExecuteConfirmationDialog("CloseNewFileDialog"))
		{
			Clear();
			freyja_print("Closing Model...");
			freyja_set_main_window_title(BUILD_NAME);
			mCurrentlyOpenFilename = String();
			mCleared = true;
		}
		break;
		
	case eScheme_Material:
		{
			unsigned int i = freyjaMaterialCreate();
			freyja_print("New material [%i] created.", i);
		}
		break;
		
	default:
		;
	}
}


void FreyjaControl::ExportFile()
{
	freyja_event_file_dialog(FREYJA_MODE_SAVE_MODEL, "Export model...");
	freyja_print("Exporting is handled from Save As using file extentions...");
}


void FreyjaControl::ImportFile()
{
	freyja_event_file_dialog(FREYJA_MODE_LOAD_MODEL, "Import model...");
	freyja_print("Importing is handled automatically from Open...");
}


void FreyjaControl::SaveFile()
{
	switch (GetControlScheme())
	{
	case eScheme_Animation:
		freyja_print("!(%s:%i) %s: eScheme_Animation, Not implemented", 
					 __FILE__, __LINE__, __func__);
		break;

	case eScheme_Model:
		if (mCleared) // safety
		{
			freyja_print("We don't save empty files anymore");
		}
		else if (mCurrentlyOpenFilename.Empty())
		{
			freyja_event_file_dialog(FREYJA_MODE_SAVE_MODEL, "Save model as...");				
		}
		else
		{
			{
				const char *s = mCurrentlyOpenFilename.GetCString();
				
				if (SaveModel(s))
				{
					freyja_print("Model '%s' Saved", s);
				}
				else
				{
					freyja_print("Model '%s' failed to save", s);
				}
			}
		}
		break;

	case eScheme_Material:
		freyja_event_file_dialog(FREYJA_MODE_SAVE_MATERIAL, "Save material as...");
		break;
		
	default:
		;
	}
}
	

void FreyjaControl::OpenFile()
{
	switch (GetControlScheme())
	{
	case eScheme_Animation:
		freyja_print("!(%s:%i) %s: eScheme_Animation, Not implemented", 
					 __FILE__, __LINE__, __func__);
		break;
		
	case eScheme_UV:
		freyja_event_file_dialog(FREYJA_MODE_LOAD_TEXTURE, "Open texture...");
		break;
		
	case eScheme_Model:
		if (!mCleared)
		{
			if (mgtk::ExecuteConfirmationDialog("CloseToOpenFileDialog"))
			{
				Clear();
				freyja_print("Closing Model...");
				freyja_set_main_window_title(BUILD_NAME);
				
				
				freyja_event_file_dialog(FREYJA_MODE_LOAD_MODEL, "Open model...");
			}
		}
		else
		{
			freyja_event_file_dialog(FREYJA_MODE_LOAD_MODEL, "Open model...");
		}
		break;
		
	case eScheme_Material:
		freyja_event_file_dialog(FREYJA_MODE_LOAD_MATERIAL, "Open material...");
		break;
	}
}

	
void FreyjaControl::SaveFileModel()
{
	if (mCleared) // safety
	{
		freyja_print("No changes to be saved.");
	}
	else if (mCurrentlyOpenFilename.Empty())
	{
		freyja_event_file_dialog(FREYJA_MODE_SAVE_MODEL, "Save model as...");
	}
	else
	{
		const char *s = mCurrentlyOpenFilename.GetCString();
		if (SaveModel(s))
			freyja_print("Model '%s' Saved", s);
		else
			freyja_print("Model '%s' failed to save", s);
	}
}


void FreyjaControl::OpenFileModel()
{
	if (!mCleared)
	{
		if (mgtk::ExecuteConfirmationDialog("CloseToOpenFileDialog"))
		{
			Clear();
			freyja_print("Closing Model...");
			freyja_set_main_window_title(BUILD_NAME);
				
			freyja_event_file_dialog(FREYJA_MODE_LOAD_MODEL, "Open model...");
		}
	}
	else
	{
		freyja_event_file_dialog(FREYJA_MODE_LOAD_MODEL, "Open model...");
	}
}
	
	
void FreyjaControl::RevertFile()
{
	if (mCurrentlyOpenFilename.Empty())
	{
		freyja_print("Revert requires a model being previously loaded");
	}
	else if (mgtk::ExecuteConfirmationDialog("RevertFileDialog"))
	{
		Clear();
		freyja_print("Reverting Model...");
		freyja_set_main_window_title(BUILD_NAME);
		
		if (LoadModel(mCurrentlyOpenFilename.GetCString()))
			mCleared = false;
	}
}


void FreyjaControl::Shutdown()
{
	bool exiting = true;

	if (!mCleared)
	{
		exiting = mgtk::ExecuteConfirmationDialog("ExitWarningDialog");
	}

	if (exiting)
	{
		SaveUserPreferences();
		freyja_event_exit();
	}
}


void FreyjaControl::TexcoordCombine()
{	
	if (mEventMode != aTexcoordCombine)
	{
		mEventMode = aTexcoordCombine;
	}
	else
	{	
		mEventMode = aNone;
	}
	
	freyja_print("Texcoord combine mode is [%s]", 
				 (mEventMode == aTexcoordCombine) ? "on" : "off");
}


void FreyjaControl::VertexCombine()
{	
	if (mEventMode != aVertexCombine)
	{
		mEventMode = aVertexCombine;
	}
	else
	{	
		mEventMode = aNone;
	}
	
	freyja_print("Vertex combine mode is [%s]", 
				 (mEventMode == aVertexCombine) ? "on" : "off");
}


#warning FIXME "event() functions must be replaced for Kagura"
bool FreyjaControl::event(int event, unsigned int value)
{
	if (ResourceEvent::listen(event - ePluginEventBase, value))
		return true;

	vec_t x, y, z;

	switch (event)
	{
	case eBoneIterator:
		if (!freyja_event_set_range(event, value, 0, freyjaGetBoneCount()))
		{
			SetSelectedBone(value);

			if (value == GetSelectedBone() && freyjaIsBoneAllocated(value))
			{
				/* Mongoose 2002.08.31, Update spin buttons dependent 
				 * on this one */
				GetBoneRotation(&x, &y, &z);
				freyja_event_set_float(520, x);
				freyja_event_set_float(521, y);
				freyja_event_set_float(522, z);

				{ 
					vec3_t xyz;
					freyjaGetBoneTranslation3fv(value, xyz);
					x = xyz[0], y = xyz[1], z = xyz[2];
				}

				freyja_event_set_float(510, x);
				freyja_event_set_float(511, y);
				freyja_event_set_float(512, z);
				freyja_event_gl_refresh();
				freyja_print("Selecting bone[%i] ...", value);

				/* Update any bone name listeners, 
				 * 1. Dup string to avoid evil widgets that want to mutate it
				 * 2. Disable event hook up in case of event loop */
				extern void mgtk_textentry_value_set(int event, const char *s);
				char dupname[64];
				strncpy(dupname, GetBoneName(value), 64);
				mAllowBoneNameUpdate = false;
				mgtk_textentry_value_set(eSetCurrentBoneName, dupname);
				mAllowBoneNameUpdate = true;
			}
		}
		break;

	case ePolygonIterator:
		if (!freyja_event_set_range(event, value, 0, freyjaGetMeshPolygonCount(GetSelectedMesh())))
		{
			SetSelectedFace(value);			
			freyja_event_gl_refresh();
			freyja_print("Selecting polygon[%i] ...", value);
		}
		break;

	case eMeshIterator:
		if (!freyja_event_set_range(event, value, 0, freyjaGetMeshCount()))
		{
			SetSelectedMesh(value);			
			freyja_event_gl_refresh();
			freyja_print("Selecting mesh[%i] ...", value);
		}
		break;

	case eModeAutoKeyframe:
		if (value)
		{
			mRender->SetViewMode(VIEWMODE_MODEL_EDIT);
			mRender->SetFlag(FreyjaRender::fKeyFrameAnimation);
			freyja_event_gl_refresh();
			freyja_print("Animation Scheme");
			SetControlScheme(eScheme_Animation);
		}
		else
		{
			// If you disable auto keyframe you must set a new major mode
			mRender->SetViewMode(VIEWMODE_MODEL_EDIT);
			mRender->ClearFlag(FreyjaRender::fKeyFrameAnimation);
			freyja_event_gl_refresh();
			freyja_print("Model Editor Scheme");
			SetControlScheme(eScheme_Model);
		}
		break;

	default:
		freyja_print("!Unhandled { event = %d, value = %u }", event, value);
		return false;
	}

	return true;
}

void FreyjaControl::ePointJoint(uint32 value)
{
	if (value)
	{
		// radio button like func for multiple widgets on same event
		mgtk_toggle_value_set(eSphereJointId, 0);
		mgtk_toggle_value_set(eAxisJointId, 0);
		FreyjaRender::mJointRenderType = 1;
	}
}

void FreyjaControl::eSphereJoint(uint32 value)
{
	if (value)
	{
		// radio button like func for multiple widgets on same event
		mgtk_toggle_value_set(ePointJointId, 0);
		mgtk_toggle_value_set(eAxisJointId, 0);
		FreyjaRender::mJointRenderType = 2;
	}
}

void FreyjaControl::eAxisJoint(uint32 value)
{
	if (value)
	{
		// radio button like func for multiple widgets on same event
		mgtk_toggle_value_set(eSphereJointId, 0);
		mgtk_toggle_value_set(ePointJointId, 0);
		FreyjaRender::mJointRenderType = 3;
	}
}


void FreyjaControl::eAnimationSlider(uint32 value)
{
	if (value != GetSelectedKeyFrame())
	{
		// FIXME: Needs better defined wrapping and bounds. 
		//freyja_event_set_range(eAnimationSliderId, value, 0, 500);
		SetSelectedKeyFrame(value);
		freyja_event_gl_refresh();
	}
}


void FreyjaControl::eSelectionByBox(uint32 value)
{
	if (value)
	{
		// Radio button like func for multiple widgets on same event
		mgtk_toggle_value_set(eRotateObjectId, 0);
		mgtk_toggle_value_set(eScaleObjectId, 0);
		mgtk_toggle_value_set(eMoveObjectId, 0);
		mgtk_toggle_value_set(eUnselectId, 0);
		mgtk_toggle_value_set(eSelectId, 0);
		mgtk_toggle_value_set(eInfoObjectId, 0);
		
		// FIXME: Add bbox selection back to this build
		mCursor.SetMode(freyja3d::Cursor::Invisible);
		mEventMode = aSelectByBox;
		mRender->SetFlag(FreyjaRender::fBoundingVolSelection);
		
		// We only need 2 control points for bbox selector
		mControlPoints.resize(0);
		mSelectedControlPoint = 0;
		mControlPoints.push_back(hel::Vec3(20, 20, 10));
		mControlPoints.push_back(hel::Vec3(-20, 10, -10));
		mCursor.mPos = mControlPoints[0];
		mCursor.SetMode(Cursor::Translation);
		
		freyja_print("Press Ctrl+Right Mouse to end selection");
		freyja_event_gl_refresh();
	}
	else
	{
		mControlPoints.resize(0);
		mRender->ClearFlag(FreyjaRender::fBoundingVolSelection);
		freyja_event_gl_refresh();
	}
}


void FreyjaControl::eScreenShot()
{
	OpenGL::TakeScreenShot("Freyja", 
						   mRender->GetWindowWidth(), 
						   mRender->GetWindowHeight());
}

	
void FreyjaControl::eSelect(uint32 value)
{
	if (value)
	{
		// Radio button like func for multiple widgets on same event
		mgtk_toggle_value_set(eRotateObjectId, 0);
		mgtk_toggle_value_set(eScaleObjectId, 0);
		mgtk_toggle_value_set(eMoveObjectId, 0);
		mgtk_toggle_value_set(eUnselectId, 0);
		mgtk_toggle_value_set(eSelectionByBoxId, 0);
		mgtk_toggle_value_set(eInfoObjectId, 0);

		mEventMode = aSelect;
		mCursor.SetMode(freyja3d::Cursor::Invisible);
		freyja_print("Select %s, hold SHIFT to unselect...", 
					 ObjectTypeToString(GetObjectMode()).c_str());
		freyja_event_gl_refresh();
	}
}

void FreyjaControl::eUnselect(uint32 value)
{
	if (value)
	{
		// Radio button like func for multiple widgets on same event
		mgtk_toggle_value_set(eRotateObjectId, 0);
		mgtk_toggle_value_set(eScaleObjectId, 0);
		mgtk_toggle_value_set(eSelectId, 0);
		mgtk_toggle_value_set(eMoveObjectId, 0);
		mgtk_toggle_value_set(eSelectionByBoxId, 0);
		mgtk_toggle_value_set(eInfoObjectId, 0);
		
		mEventMode = aUnselect;
		mCursor.SetMode(freyja3d::Cursor::Invisible);
		freyja_print("Unselect %s...", 
					 ObjectTypeToString(GetObjectMode()).c_str());
		freyja_event_gl_refresh();
	}
}


void FreyjaControl::eInfoObject(uint32 value)
{
	if (value)
	{
		// Radio button like func for multiple widgets on same event
		mgtk_toggle_value_set(eRotateObjectId, 0);
		mgtk_toggle_value_set(eScaleObjectId, 0);
		mgtk_toggle_value_set(eSelectId, 0);
		mgtk_toggle_value_set(eMoveObjectId, 0);
		mgtk_toggle_value_set(eSelectionByBoxId, 0);
		mgtk_toggle_value_set(eUnselectId, 0);
		
		mEventMode = aInfo;
		mCursor.SetMode(freyja3d::Cursor::Invisible);
		freyja_print("Info %s...", 
					 ObjectTypeToString(GetObjectMode()).c_str());
		freyja_event_gl_refresh();
	}
}


void FreyjaControl::eMoveObject(uint32 value)
{
	if (value)
	{
		// Radio button like func for multiple widgets on same event
		mgtk_toggle_value_set(eRotateObjectId, 0);
		mgtk_toggle_value_set(eScaleObjectId, 0);
		mgtk_toggle_value_set(eSelectId, 0);
		mgtk_toggle_value_set(eUnselectId, 0);
		mgtk_toggle_value_set(eSelectionByBoxId, 0);
		mgtk_toggle_value_set(eInfoObjectId, 0);
		
		mEventMode = aMove;
		mCursor.SetMode(freyja3d::Cursor::Translation);
		freyja_print("Move %s...", 
					 ObjectTypeToString(GetObjectMode()).c_str());
		freyja_event_gl_refresh();
	}
}

void FreyjaControl::eScaleObject(uint32 value)
{
	if (value)
	{
		// Radio button like func for multiple widgets on same event
		mgtk_toggle_value_set(eRotateObjectId, 0);
		mgtk_toggle_value_set(eMoveObjectId, 0);
		mgtk_toggle_value_set(eSelectId, 0);
		mgtk_toggle_value_set(eUnselectId, 0);
		mgtk_toggle_value_set(eSelectionByBoxId, 0);
		mgtk_toggle_value_set(eInfoObjectId, 0);

		// Reset cursor
		mEventMode = aScale;
		mCursor.SetMode(freyja3d::Cursor::Scale);
		mCursor.mScale = hel::Vec3(1.0f, 1.0f, 1.0f);
		freyja_print("Scale %s...", 
					 ObjectTypeToString(GetObjectMode()).c_str());
		freyja_event_gl_refresh();
	}
}


void FreyjaControl::eRotateObject(uint32 value)
{
	if (value)
	{
		// Radio button like func for multiple widgets on same event
		mgtk_toggle_value_set(eMoveObjectId, 0);
		mgtk_toggle_value_set(eScaleObjectId, 0);
		mgtk_toggle_value_set(eSelectId, 0);
		mgtk_toggle_value_set(eUnselectId, 0);
		mgtk_toggle_value_set(eSelectionByBoxId, 0);
		mgtk_toggle_value_set(eInfoObjectId, 0);

		mEventMode = aRotate;
		mCursor.SetMode(freyja3d::Cursor::Rotation);
		
		switch (GetObjectMode())
		{
		case tMesh:
			{
				Mesh *m = Mesh::GetMesh(GetSelectedMesh());
				if (m) mCursor.mPos = m->GetBoundingVolumeCenter();
			}
			break;
			
		default:
			;
		}
		
		freyja_print("Rotate %s...", 
					 ObjectTypeToString(GetObjectMode()).c_str());
		
		freyja_event_gl_refresh();
	}
}


#warning FIXME "event() functions must be replaced for Kagura"
bool FreyjaControl::event(int event, vec_t value)
{
	if (ResourceEvent::listen(event - 10000 /*ePluginEventBase*/, value))
		return true;

	/* These Transform Box events don't trigger on their own */
	switch (event)
	{
	case eMove_X:
	case eMove_Y:
	case eMove_Z:
		break;

	case eRotate_X:
	case eRotate_Y:
	case eRotate_Z:
		break;

	case eScale_X:
	case eScale_Y:
	case eScale_Z:
		if (value == 0.0f)
			freyja_event_set_float(event, 1.0f);
		break;

	default:
		freyja_print("!Unhandled { event = %d, value = %f }", event, value);
		return false;
	}

	return true;
}


void FreyjaControl::UnselectMode()
{
	mEventMode = aUnselect;
	mCursor.SetMode(freyja3d::Cursor::Invisible);
	freyja_print("! (mode) Unselect object...");
	freyja_event_gl_refresh();
}


void FreyjaControl::SelectMode()
{
	mEventMode = aSelect;
	mCursor.SetMode(freyja3d::Cursor::Invisible);
	freyja_print("! (mode) Select object...");
	freyja_event_gl_refresh();
}


#warning FIXME "event() functions must be replaced for Kagura"
bool FreyjaControl::handleEvent(int mode, int cmd)
{
	bool handled = true;
	//freyja_print("! Call to handleEvent is deprecated...\n");

	// Mongoose, 2007.04.07:
	// 'Ids' here are no longer const, so had to switch to if/else block
	// This handler exists from the old 'held key mode' callback codebase.
	// These calls are typically made by holding down a modifer key.
	if (mode == (int)eSelectId)
	{
		SelectMode();
	}
	else if (mode == (int)eUnselectId)
	{
		UnselectMode();
	}
	else
	{
		handled = ResourceEvent::listen(cmd - 10000 /*ePluginEventBase*/);
	}

	return handled;
}


#warning FIXME "event() functions must be replaced for Kagura"
void FreyjaControl::handleTextEvent(int event, const char *text)
{
	static bool haltTextureA = false;
	bool empty = (text == NULL || text[0] == 0);

	if (!empty && ResourceEvent::listen(event - 10000 /*ePluginEventBase*/, text))
		return; // true;


	switch (event)
	{
	case eSetTextureNameA:
		if (!haltTextureA)
		{
			haltTextureA = true;
			freyjaMaterialTextureName(freyjaGetCurrentMaterial(), text);
			haltTextureA = false;
		}
		break;

	case eSkeletonName:
		freyjaSkeletonName(GetSelectedSkeleton(), text);
		break;

	case eSetCurrentBoneName:
		if (mAllowBoneNameUpdate)
			SetBoneName(GetSelectedBone(), text);
		break;

	case FREYJA_MODE_SAVE_MODEL:
		SaveModel(text);
		break;

	default:
		freyja_print("handleTextEvent(%i, '%s'): Unhandled event.", event, text);
	}
}


void FreyjaControl::eOpenModel(char *filename)
{
	if (LoadModel(filename))
	{
		char title[1024];
		snprintf(title, 1024, "%s - Freyja", filename);
		freyja_set_main_window_title(title);
		AddRecentFilename(filename);
	}
}


void FreyjaControl::eSaveModel(char *filename, char *extension)
{
	if (SaveModel(filename, extension))
	{
		// This was commented out... it shouldn't matter if it 'overwrites'
		char title[1024];
		snprintf(title, 1024, "%s - Freyja", filename);
		freyja_set_main_window_title(title);
		AddRecentFilename(filename);
	}
}


void FreyjaControl::PrintInfo()
{
	String s, t;
	index_t model = GetSelectedModel();

	s = "Current Model Properties\n";

	t.Set("   %i Bones\n", freyjaGetBoneCount());
	s += t;

	t.Set("   %i Meshes\n", freyjaGetModelMeshCount(model));
	s += t;

	for (uint32 i = 0, n = freyjaGetModelMeshCount(model); i < n; ++i)
	{
		index_t mesh = freyjaGetModelMeshIndex(model, i);
		if (freyjaIsMeshAllocated(mesh))
		{
			t.Set("      %i. '%s' %i Polyons, %i Vertices\n", i, 
				  freyjaGetMeshNameString(mesh), 
				  freyjaGetMeshPolygonCount(mesh),
				  freyjaGetMeshVertexCount(mesh));
			s += t;
		}
	}

	freyja_event_info_dialog("gtk-dialog-info", (char *)s.c_str());
}


bool FreyjaControl::MotionEvent(int x, int y)
{
	//	freyja_print("motion -> %i, %i", x, y);


	switch (GetControlScheme())
	{
	case eScheme_Animation:
	case eScheme_Model:

		switch (mMouseButton)
		{
		case MOUSE_BTN_MIDDLE:
			{
				static int old_y = 0, old_x = 0;
				const float treshold = 1.0f;
				float xyz[3];

				xyz[0] = 0.0f;
				xyz[1] = 0.0f;
				xyz[2] = 0.0f;
		
				if (x > old_x + treshold)
					xyz[0] = 1.0f;
				else if (x < old_x - treshold)
					xyz[0] = -1.0f;
		
				if (y > old_y + treshold)
					xyz[1] = -1.0f;
				else if (y < old_y - treshold)
					xyz[1] = 1.0f;
		
				old_x = x;
				old_y = y;

				if (xyz[0] == 0.0f && xyz[1] == 0.0f)
				{
					//DEBUG_MSG("Early out");
					return true;
				}

				switch (GetSelectedView())
				{
				case PLANE_BACK:
					mSceneTrans += hel::Vec3(xyz[0], xyz[1], xyz[2]);
					break;
				
				case PLANE_BOTTOM:
					mSceneTrans += hel::Vec3(xyz[0], xyz[2], xyz[1]);
					break;
				
				case PLANE_RIGHT:
					mSceneTrans += hel::Vec3(xyz[2], xyz[1], xyz[0]);
					break;

				case PLANE_FRONT: // front, xy
					mSceneTrans += hel::Vec3(xyz[0], xyz[1], xyz[2]);
					break;
				
				case PLANE_TOP: // top, xz
					mSceneTrans += hel::Vec3(xyz[0], xyz[2], xyz[1]);
					break;
				
				case PLANE_LEFT: // left, zy
					mSceneTrans += hel::Vec3(xyz[2], xyz[1], xyz[0]);
					break;
				
				default:
					;
				}
			}
			break;


		case MOUSE_BTN_RIGHT: 
			if (GetSelectedView() == PLANE_FREE)
			{
				static float old_x = x, old_y = y;

				if (x > old_x)
					mRender->Rotate(Y_F, -1.0f);
				else if (x < old_x)
					mRender->Rotate(Y_F, 1.0f);
			
				if (y > old_y)
					mRender->Rotate(X_F, -1.0f);
				else if (y < old_y)
					mRender->Rotate(X_F, 1.0f);

				old_x = x;
				old_y = y;
			}
			break;

		case MOUSE_BTN_LEFT:
			switch (mEventMode)
			{
			case aSelectByBox:
			case aMove:
				{
					vec_t vx = x, vy = y;
					AdjustMouseXYForViewports(vx, vy);
					MoveObject(vx, vy);
				}
				break;

			case aRotate: 
				rotateObject(x, y, GetSelectedView());
				break;
			
			case aScale:
				scaleObject(x, y, GetSelectedView());
				break;
			
			default:
				break;
			}

			break;
		}
		break;

	case eScheme_UV:
		switch (mMouseButton)
		{
		case MOUSE_BTN_RIGHT:
			{
				float s;
				float t;
				
				s = (float)x / (float)mRender->GetWindowWidth();
				t = (float)y / (float)mRender->GetWindowHeight();

				if (s > 1.0) s = 1.0;
				if (s < 0.0) s = 0.0;
				if (t > 1.0) t = 1.0;
				if (t < 0.0) t = 0.0;
				
				TexCoordMove(s, t);
				return true;
			}
			break;

		default:
			;
		}
		break;

	default:
		; // Unhandled
	}

	return true;
}


void FreyjaControl::SetKeyFrame()
{
	switch (mObjectMode)
	{
	case tBone:
		{
			// FIXME: There is no way to support multiple skeletons like this
			// in ABI *yet
			//Bone *b = freyjaModelGetBoneClass(GetSelectedSkeleton(), 
			//								  GetSelectedBone());
			Bone *b = Bone::GetBone(GetSelectedBone());

			if (b)
			{
				BoneTrack &track = b->GetTrack(GetSelectedAnimation());
				uint32 k = FreyjaControl::mInstance->GetSelectedKeyFrame();
				vec_t time = (vec_t)k / track.GetRate(); 
				index_t id = track.NewRotKeyframe(time);
				uint32 count = track.GetRotKeyframeCount();

				Vec3KeyFrame *key = track.GetRotKeyframe(id);
				freyjaGetBoneRotationEuler3fv(GetSelectedBone(), key->mData.mVec);
				key->mData *= 57.295779513082323f;
				//FIXME
				//freyjaGetBoneTranslation3fv(GetSelectedBone(), key->mLoc.mVec);

				freyja_print("Created tBone keyframe[%i] <- %.3fs, %i/%i", 
							 mSelectedKeyFrame, time, id, count);
			}
		}
		break;

	case tMesh:
		{
			Mesh *m = Mesh::GetMesh(GetSelectedMesh());

			if (m)
			{
				Track &track = m->GetTransformTrack(GetSelectedAnimation());
				uint32 k = FreyjaControl::mInstance->GetSelectedKeyFrame();
				vec_t time = (vec_t)k / track.GetRate(); 
				index_t id = track.NewKeyframe(time);
				uint32 count = track.GetKeyframeCount();
			
				freyja_print("Created tMesh keyframe[%i] <- %.3fs, %i/%i | %i", 
							 mSelectedKeyFrame, time, id, count,
							 track.mKeyFrames.size());
			}
		}
		break;

	default:
		freyja_print("%s: Type '%s' not supported yet.", 
					 __func__, ObjectTypeToString(mObjectMode).c_str());
	}
}


void FreyjaControl::SelectCursorAxis(vec_t vx, vec_t vy)
{
	switch (mCursor.GetMode())
	{
	case freyja3d::Cursor::Invisible:
		break;

	case freyja3d::Cursor::Rotation:
		if (GetControlScheme() == eScheme_Model ||
			GetControlScheme() == eScheme_Animation)
		{
			freyja_print("! SelectCursorAxis...");
			CastPickRay(vx, vy);
			hel::Ray &r = FreyjaRender::mTestRay;
			mCursor.mAxis = freyja3d::Cursor::eNone;

			if (mCursor.CheckForRayCollision(r))
			{
				mToken = true;

				switch (mCursor.mAxis)
				{
				case freyja3d::Cursor::eX:
					freyja_print("Cursor ray picked X");
					break;

				case freyja3d::Cursor::eY:
					freyja_print("Cursor ray picked Y");
					break;

				case freyja3d::Cursor::eZ:
					freyja_print("Cursor ray picked Z");
					break;

				default:
					;
				}
			}			
		}
		break;
		
	default:
		if (GetControlScheme() == eScheme_Model ||
			GetControlScheme() == eScheme_Animation)
		{
			freyja_print("! SelectCursorAxis...");
			CastPickRay(vx, vy);
			hel::Ray &r = FreyjaRender::mTestRay;
			//bool picked = false;
			//Vec3 o;

			mCursor.mAxis = freyja3d::Cursor::eNone;
			mCursor.mLastPos = mCursor.mPos;

			if (mCursor.CheckForRayCollision(r))
			{
				mToken = true;
					
				switch (mCursor.mAxis)
				{
				case freyja3d::Cursor::eX:
					freyja_print("Cursor ray picked X");
					break;
					
				case freyja3d::Cursor::eY:
					freyja_print("Cursor ray picked Y");
					break;
					
				case freyja3d::Cursor::eZ:
					freyja_print("Cursor ray picked Z");
					break;
					
				case freyja3d::Cursor::eAll:
					freyja_print("Cursor ray picked ALL");
					break;
					
				default:
					;
				}
			}
		}
	}


	// No cursor 'hit' while in bbox select look for a control point pick
	if (mEventMode == aSelectByBox)
	{
		CastPickRay(vx, vy);
		hel::Ray &r = FreyjaRender::mTestRay;
		hel::Vec3 p;
		vec_t t, closest;
		int selected = -1;

		for (uint32 i = 0, count = mControlPoints.size(); i < count; ++i)
		{
			p = mControlPoints[i];
			
			if (r.IntersectSphere(p.mVec, 0.5f, t))
			{
				if (selected == -1 || t < closest)
				{
					selected = i;
					closest = t;
				}
			}
		}

		if (selected > -1)
		{
			mSelectedControlPoint = selected;
			mCursor.mPos = mControlPoints[selected];
		}
	}
}


bool FreyjaControl::MouseEdit(int btn, int state, int mod, int x, int y)
{
	bool ret = false;
	bool swappedModes = false;
	vec_t vx = x, vy = y;
	action_type_t mode = mEventMode;
	Cursor::Flags cm = mCursor.GetMode();
	mMouseButton = btn;
	mMouseState = state;
	mModKey = mod;


	if (!btn && !state && mToken && GetControlScheme() == eScheme_Model)
	{
		switch (GetObjectMode())
		{
		case tModel:
		case tMesh:
		case tSelectedFaces:
			{
				hel::Vec3 v = GetCursorData(GetEventAction());

				// Mongoose - Does transform, undo, etc for ya, bub
				Transform(GetObjectMode(), GetEventAction(),
						  v.mVec[0], v.mVec[1], v.mVec[2]);
			}			
			break;

		default:
			;
		}
	}
	else if (!btn && !state && mToken && 
			 GetControlScheme() == eScheme_Animation)
	{
		switch (GetObjectMode())
		{
		case tModel:
		case tMesh:
			{
				//freyja_print("! Model[%i] %s %f %f %f",
				//			 0, 
				//			 ActionToString(GetEventAction()).c_str(),
				//			 GetEventAction(),
				//			 v.mVec[0], v.mVec[1], v.mVec[2]);

				hel::Vec3 v = GetCursorData(GetEventAction());

				// Mongoose - Does transform, undo, etc for ya, bub
				KeyframeTransform(GetObjectMode(), GetEventAction(),
								  v.mVec[0], v.mVec[1], v.mVec[2]);

				// Reset transforms in the cursor
				mCursor.mRotate = hel::Vec3(0,0,0);
				mCursor.mScale = hel::Vec3(1,1,1);
				//mCursor.mPos = hel::Vec3(0,0,0);
			}			
			break;

		default:
			;
		}
	}


	/* Handle mode */
	/* FIXME: These should be seperate 'windows' with their own 
	 * MouseEvent() methods or the like later */
	switch (GetControlScheme())
	{
	case eScheme_UV:
		if (mUVMouseState)
		{
			mUVMouseState = false;
			return true;
		}
		else if (btn == MOUSE_BTN_LEFT && state == MOUSE_BTN_STATE_PRESSED)
		{
			// Mongoose: Convert window coords to UV coords, and clamp UVs
			float s = (float)x / (float)mRender->GetWindowWidth();
			if (s > 1.0) s = 1.0;
			else if (s < 0.0) s = 0.0;

			float t = (float)y / (float)mRender->GetWindowHeight();
			if (t > 1.0) t = 1.0;
			else if (t < 0.0) t = 0.0;
			
			
			if (mEventMode == aTexcoordCombine)
			{
				//TexelCombine(s, t);
			}
			else
			{
				TexCoordSelect(s, t);
			}
			
			mUVMouseState = true;
			return true;
		}
		break;

	case eScheme_Model:
		// Mongoose 2002.01.12, Allow temp mode override
		if (mEventMode == aSelectByBox)
		{
		}
		else if (mod & KEY_LCTRL)
		{
			SelectMode(); //handleEvent(eEvent, eSelectId);
			swappedModes = true;
		}
		else if (mod & KEY_LSHIFT)
		{
			UnselectMode(); //handleEvent(eEvent, eUnselectId);
			swappedModes = true;
		}


		switch (mEventMode)
		{
		case aMove:
			mXYZMouseState = 1;
			//VertexSelect(xx, yy);
			break;

		case aVertexCombine:
			if (mXYZMouseState == 0)
			{
				//VertexCombine(xx, yy);
				mXYZMouseState = 1;
			}
			else
			{
				mXYZMouseState = 0;
			}
			break;

		case aSelectByBox:
			if (mXYZMouseState == 0)
			{
				mXYZMouseState = 1;
			}
			else
			{
				mXYZMouseState = 0;
			}
			break;

		default:
			;
		}
		break;

	default:
		;
	}


	/* Get the viewport adjusted mouse coordinates, and swap modes if needed */
	AdjustMouseXYForViewports(vx, vy);

	/* Handle key modifers -- notice this are temp state changes */
	if (mEventMode == aSelectByBox)
	{
		if (mod & KEY_LCTRL || mod & KEY_RCTRL &&
			btn == MOUSE_BTN_RIGHT && state == MOUSE_BTN_STATE_PRESSED)
		{
			SelectObjectByBox(mControlPoints[0], mControlPoints[1]);
		}
	}
	else if (mod & KEY_LCTRL)
	{
		SelectMode();
	}
	else if (mod & KEY_LSHIFT)
	{
		UnselectMode();
	}

	/* Handle left clicks */
	if (btn == MOUSE_BTN_LEFT && state == MOUSE_BTN_STATE_PRESSED)
	{
		ret = true;
		switch (mEventMode)
		{
		case aSelect:
			SelectObject(vx, vy);
			break;

		case aUnselect:
			UnselectObject(vx, vy);
			break;

		case aInfo:
			SelectObject(vx, vy);
			InfoObject();
			break;


		default:
			SelectCursorAxis(vx, vy);
			ret = false;
		}
	}

	if (mEventMode != mode)
	{
		mCursor.SetMode(cm);
	}

	mEventMode = mode;

	return ret;
}


bool FreyjaControl::MouseEvent(int btn, int state, int mod, int x, int y)
{
	// We've just let go of the button that was being held down
	if (mCursor.mSelected && !btn && !state) 
	{
		mCursor.mSelected = false;
		freyja_print("! Cursor was released.");
	}

	// New mouse Edit event handler
	if (MouseEdit(btn, state, mod, x, y)) 
		return true;

	action_type_t mode = mEventMode;
	Cursor::Flags cm = mCursor.GetMode();
	mMouseButton = btn;
	mMouseState = state;
	mModKey = mod;
	vec_t vx = x, vy = y;
	AdjustMouseXYForViewports(vx, vy);


	switch (GetControlScheme())
	{
	case eScheme_UV:
		if (mUVMouseState)
		{
			mUVMouseState = false;
			return true;
		}
		else if (btn == MOUSE_BTN_LEFT && state == MOUSE_BTN_STATE_PRESSED)
		{
			float s, t;
			
			
			s = (float)x / (float)mRender->GetWindowWidth();
			t = (float)y / (float)mRender->GetWindowHeight();
			
			// Mongoose: Clamp texels to be bound by min and max
			if (s > 1.0) s = 1.0;
			if (s < 0.0) s = 0.0;
			if (t > 1.0) t = 1.0;
			if (t < 0.0) t = 0.0;
			
			if (mEventMode == aTexcoordCombine)
			{
				//TexelCombine(s, t);
			}
			else
			{
				//TexelSelect(s, t);
			}
			
			mUVMouseState = true;
			return true;
		}
		break;

	default:
		;
	}

	// Mongoose 2002.01.12, Reset mode
	if (mEventMode != mode)
	{
		mCursor.SetMode(cm);
	}

	mEventMode = mode;
	return true;
}


void FreyjaControl::Fullscreen()
{
	mFullScreen = !mFullScreen;
	
	if (mFullScreen)
	{
		freyja_event_fullscreen();
	}
	else
	{
		freyja_event_unfullscreen();
	}
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////

void FreyjaControl::getScreenToWorldOBSOLETE(vec_t &x, vec_t &y)
{
	vec_t z;

	//FREYJA_ASSERTMSG(false, "Obsolete function call.");

	GetWorldFromScreen(x, y, z);

	switch (GetSelectedView())
	{
	case PLANE_BACK: // Back, X-Y
		x = -x;
		break;

	case PLANE_FRONT: // Front, XY
		/* Nothing to do for XY */
		break;
		
	case PLANE_BOTTOM: // Bottom, X-Z
		z = -z;

	case PLANE_TOP: // Top, XZ
		y = -z;
		break;
		
	case PLANE_RIGHT: // Right, Z-Y
		z = -z;

	case PLANE_LEFT: // Left, ZY
		x = z;
		break;

	case PLANE_FREE:
		/* Not Implemented, since there is no such 'ideal plane' concept */
		break;

	default:
		; // This isn't a 3d view or it's some fucked up user raster
	}
}


void FreyjaControl::getPickRay(vec_t mouseX, vec_t mouseY, 
							   vec4_t rayOrigin, vec4_t rayVector)
{
	/* This does not use viewport: quadrant checks, widths, and heights */

	vec_t winH = mRender->GetWindowHeight();
	vec_t winY = (winH - mouseY) - winH * 0.5f;
	vec_t normY = winY / ( winH * 0.5f );
	vec_t winX = mouseX - winH * 0.5f;
	vec_t normX = winX / ( winH * 0.5f );
	vec_t zNear = -400.0f; // mRender->getZNear();
	vec_t nearH = 20.0f;//mRender->getNearHeight();
	vec_t aspect = mRender->GetWindowAspectRatio();

	// Now transform by inverse of modelview matrix to object coordinates
	extern matrix_t gModelViewMatrix; // This is actual cached render transform
	hel::Mat44 m(gModelViewMatrix);
	m.Invert();
	m.Transpose();

	// This is now ray in eye coordinates
	vec4_t rayVec = {nearH * normY, nearH * aspect * normX, -zNear, 0.0f};
	m.Multiply4fv(rayVec, rayVector); // src, dest can be same

	vec4_t rayPnt = {0.0f, 0.0f, 0.0f, 1.0f };
	m.Multiply4fv(rayPnt, rayOrigin); // src, dest can be same

	//vec_t x = rayVector[2];
	//rayVector[2] = rayVector[1];
	//rayVector[1] = x;

#if DEBUG_PICK_RAY
	freyja_print("!%f, %f => <%f %f %f> @ %f, %f, %f\n",
				 mouseX, mouseY,
				 rayVector[0], rayVector[1], rayVector[2],
				 rayOrigin[0], rayOrigin[1], rayOrigin[2]);
#endif
}


void FreyjaControl::GetWorldFromScreen(vec_t &x, vec_t &y, vec_t &z)
{
	hel::Vec3 scroll(GetSceneTranslation());
	vec_t nearHeight = mRender->GetNearHeight() * 2.0f;
	vec_t width = mRender->GetWindowWidth();
	vec_t height = mRender->GetWindowHeight();
	

#if DEBUG_SCREEN_TO_WORLD
	printf("Screen (%.3f, %.3f); aspect_ratio = %.3f; zoom = %.3f\n", 
			 x, y, width/height, mRender->GetZoom());
	printf("Scroll (%.3f, %.3f, %.3f)\n", 
			 scroll.mVec[0], scroll.mVec[1], scroll.mVec[2]);
#endif

	vec_t invz = (1.0 / mRender->GetZoom());
	vec_t fs = (nearHeight * invz) / height;  // fov 40?

	x = (x - width * 0.5f) * fs;
	y = -(y - height * 0.5f) * fs;

	scroll *= invz;
	
	switch (GetSelectedView())
	{
	case PLANE_BACK:
		x = scroll.mVec[0] - x;
		y -= scroll.mVec[1];
		z = 0.0f;
		break;

	case PLANE_BOTTOM:
		x = x - scroll.mVec[0];
		z = -y + scroll.mVec[2];
		y = 0.0f;
		break;

	case PLANE_RIGHT:
		z = -x + scroll.mVec[2];
		y -= scroll.mVec[1];
		x = 0.0f;
		break;

	case PLANE_FRONT: // XY
		x -= scroll.mVec[0];
		y -= scroll.mVec[1];
		z = 0.0f;
		break;

	case PLANE_TOP: // XZ
		x -= scroll.mVec[0];
		z = y - scroll.mVec[2];
		y = 0.0f;
		break;

	case PLANE_LEFT: // ZY
		z = x - scroll.mVec[2];
		y -= scroll.mVec[1];
		x = 0.0f;
		break;

	default:
		;
	}

#if DEBUG_SCREEN_TO_WORLD
	printf("World (%.3f, %.3f, %.3f)\n", x, y, z);
#endif
}


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////

bool FreyjaControl::DuplicateSelectedObject()
{
	switch (mObjectMode)
	{
	case tMesh:
		if (mgtk::ExecuteConfirmationDialog("DupeMeshDialog"))
		{
			Mesh *m = Mesh::GetMesh(GetSelectedMesh());
			if (m)
			{
				Mesh *dupe = new Mesh(*m);
				dupe->AddToPool();
			}
		}
		break;

	default:
		freyja_print("%s(): type '%s' is not supported.", 
					 __func__, ObjectTypeToString(mObjectMode).GetCString());
		return false;
	}

	return true;
}


bool FreyjaControl::SplitSelectedObject()
{
	switch (mObjectMode)
	{
	case tMesh:
#if 0
		if (mgtk::ExecuteConfirmationDialog("SplitMeshDialog"))
#endif
		{
			bool duplicate = mgtk::ExecuteConfirmationDialog("SplitMeshCullDialog");

			Mesh *m = Mesh::GetMesh(GetSelectedMesh());
			if (m)
			{
				// Passing false would cause duplication
				Mesh *split = m->Split(!duplicate);
				split->AddToPool();
			}
		}
		break;

	default:
		freyja_print("%s(): type '%s' is not supported.", 
					 __func__, ObjectTypeToString(mObjectMode).GetCString());
		return false;
	}

	return true;
}


bool FreyjaControl::MergeSelectedObjects()
{
	switch (mObjectMode)
	{
	case tSelectedMeshes:
		{
			Mesh *m = Mesh::GetMesh(GetSelectedMesh());
			if (m)
			{
				// Only merge every mesh marked fSelected with selected mesh...
				for (uint32 i = 0, n = Mesh::GetCount(); i < n; ++i)
				{
					Mesh *mergee = Mesh::GetMesh(i);

#if 0
					if (mgtk::ExecuteConfirmationDialog("MergeObjDialog"))
#endif
					if (m != mergee && mergee->GetFlags() & Mesh::fSelected)
					{
						bool remove = mgtk::ExecuteConfirmationDialog("MergeObjDelDialog");

						m->Merge(mergee);
						
						if (remove)
						{
							// Allow for undo here...
							index_t uid = mergee->GetUID();
							if (Mesh::GetMesh(uid))
							{
								mToken = true;
								ActionModelModified(new ActionMeshDelete(uid));
							}

							freyjaMeshDelete(uid);
						}
					}
				}
			}
		}
		break;

	default:
		freyja_print("%s(): type '%s' is not supported.", 
					 __func__, ObjectTypeToString(mObjectMode).GetCString());
		return false;
	}

	return true;
}

Vector<Mesh *> mCopyMeshes;
bool FreyjaControl::CopySelectedObject()
{
	switch (mObjectMode)
	{
#if 0
	case tSelectedVertices:
		return CopyVertexBuffer();
		break;
#endif

	case tMesh:
		{
			Mesh *m = Mesh::GetMesh(GetSelectedMesh());

			if (m)
			{
				if (mCopyMeshes.size() == 0 || mCopyMeshes[0] == NULL)
				{ 
					// FIXME: ATM only allowing copy of one mesh at a time
					if (!mCopyMeshes.size())
						mCopyMeshes.push_back(NULL);

					mCopyMeshes[0] = new Mesh(*m);
					freyja_print("Copied mesh (%i)...", GetSelectedMesh());
				}
				else
				{
					freyja_print("The mesh copy buffer is already full...");
				}
			}
		}
		break;

	default:
		freyja_print("%s(): type '%s' is not supported.", 
					 __func__, ObjectTypeToString(mObjectMode).GetCString());
		return false;
	}

	return true;
}


bool FreyjaControl::PasteSelectedObject()
{
	switch (mObjectMode)
	{

#if 0
	case tSelectedVertices:
		return PasteSelectedMesh(); // vertexbuffercopy shares buf
		break;
#endif

	case tMesh:
		{
			uint32 i;
			foreach (mCopyMeshes, i)
			{
				if (mCopyMeshes[i])
					mCopyMeshes[i]->AddToPool();

				mCopyMeshes[i] = NULL;
			}
		}
		break;


	default:
		freyja_print("%s(): type '%s' is not supported.", 
					 __func__, ObjectTypeToString(mObjectMode).GetCString());
		return false;
	}

	return true;
}


void FreyjaControl::DeleteSelectedObject()
{
	switch (mObjectMode)
	{
	case tPoint:
		mEventMode = aVertexDelete;
		freyja_print("Click select to delete each vertex");
		break;

	case tFace:
		freyjaMeshPolygonDelete(GetSelectedMesh(), GetSelectedFace());
		break;

	case tSelectedFaces:
		{		
			Mesh *m = freyjaGetMeshClass(FreyjaControl::mInstance->GetSelectedMesh());
			
			if (m)
			{
				m->DeleteSelectedFaces();
			}
		}
		break;

	case tSelectedVertices:
		if (mgtk::ExecuteConfirmationDialog("DelVertDialog"))
		{
			BUG_ME("FIXME");//CullUsingVertexBuffer();
		}
		break;

	case tBone:
		freyjaBoneDelete(GetSelectedBone());
		break;

	case tMesh:
		if (Mesh::GetMesh(GetSelectedMesh()))
		{
			mToken = true;
			ActionModelModified(new ActionMeshDelete(GetSelectedMesh()));

			freyjaMeshDelete(GetSelectedMesh());
			freyja_print("Deleted mesh (%i)...", GetSelectedMesh());
		}
		freyja_print("Can not [Delete] mesh (%i). It does not exist.", GetSelectedMesh());
		break;

	default:
		freyja_print("%s Object type '%s' is not supported.", 
					 __func__, ObjectTypeToString(mObjectMode).GetCString());
	}

	freyja_event_gl_refresh();
}


void FreyjaControl::SetMaterialForSelectedFaces(uint32 material)
{
	Mesh *m = Mesh::GetMesh(GetSelectedMesh());

	if (!m)
		return;

	// Render wireframe faces	
	for (uint32 i = 0, n = m->GetFaceCount(); i < n; ++i)
	{
		Face *f = m->GetFace(i);

		if (f && f->mFlags & Face::fSelected)
		{
			freyjaMeshPolygonMaterial(GetSelectedMesh(), i, material);
		}
	}   
}
	

void FreyjaControl::InfoObject()
{
	switch (mObjectMode)
	{
	case tBone:
		{
			uint32 idx = FreyjaControl::mInstance->GetSelectedBone();
			Bone *bone = Bone::GetBone(idx);

			if (bone == NULL)
				return;

			mstl::String info, s;

			info.Set("\nBone %i, '%s'\n", bone->GetUID(), bone->GetName());

			Bone *parent = Bone::GetBone( bone->GetParent() );
			s.Set("\nParent %i, '%s'\n", bone->GetParent(), 
				  parent ? parent->GetName() : "NULL");
			info += s;
			
			info += "\nLocalTransform\n";
			s = bone->mLocalTransform.ToString();
			s.Replace('{', ' ');
			s.Replace('}', '\n');
			s.Replace('|', '\n');
			info += s;

			info += "\nBindPose\n";
			s = bone->mBindPose.ToString();
			s.Replace('{', ' ');
			s.Replace('}', '\n');
			s.Replace('|', '\n');
			info += s;

			info += "\nWorld\n";
			s = bone->mTrack.mWorld.ToString();
			s.Replace('{', ' ');
			s.Replace('}', '\n');
			s.Replace('|', '\n');
			info += s;

			FREYJA_INFOMSG(0, info.c_str());
		}
		break;

	default:
		freyja_print("%s Object type '%s' is not supported.", __func__, ObjectTypeToString(mObjectMode).GetCString());
	}
}


void FreyjaControl::CreateObject()
{
	switch (mObjectMode)
	{
	case tPoint:
		mEventMode = aVertexNew;
		break;

	case tBone:
		{
			if (freyjaGetSkeletonCount() == 0)
			{
				SetSelectedSkeleton(freyjaSkeletonCreate());
			}

			index_t skel = GetSelectedSkeleton();
			index_t parent = GetSelectedBone();
			index_t idx = freyjaBoneCreate(skel);

			if (idx == 0)
			{
				freyjaSkeletonRootIndex(skel, idx);
				freyjaBoneParent(idx, INDEX_INVALID);
			}
			else //if (index > 0 && index != root)
			{
				freyjaBoneParent(idx, parent);
				freyjaBoneAddChild(parent, idx);
			}

			//if (idx > -1)
			{
				String s;

				freyjaBoneTranslate3f(idx, 0.0f, 0.0f, 0.0f);
				freyjaBoneRotateEuler3f(idx, 0.0f, 0.0f, 0.0f);
				freyjaBoneFlags(idx, 0x0);
				s.Set("bone_%i", idx);
				freyjaBoneName(idx, s.GetCString());
				freyjaSkeletonAddBone(skel, idx);
				freyjaSkeletonUpdateBones(skel);
				UpdateSkeletonUI_Callback(skel);

				SetSelectedBone(idx);

				freyja_event_gl_refresh();
				freyja_print("New Skel[%u].Bone[%u], parent = %i",
							 skel, idx, freyjaGetBoneParent(idx));
			}


			ActionModelModified(NULL);
		}
		break;

	case tMesh:
		//MeshNew();
		ActionModelModified(NULL);
		break;

	default:
		freyja_print("%s Object type '%s' is not supported.", __func__, ObjectTypeToString(mObjectMode).GetCString());
	}

	mCleared = false;
	freyja_event_gl_refresh();
}


void FreyjaControl::UnselectObject(vec_t mouseX, vec_t mouseY)
{
	// Look for objects with depth ( was using planar projection )
	//mRender->SetFlag(FreyjaRender::fDrawPickRay); // debugging
	CastPickRay(mouseX, mouseY);

	switch (mObjectMode)
	{
	case tPoint:
		{
			Mesh *m = Mesh::GetMesh(GetSelectedMesh());

			if ( m )
			{
				int selected = -1;
				m->IntersectClosestVertex(FreyjaRender::mTestRay, selected, 2.0f);
				
				if (selected > -1)
				{
					freyja_print("! Vertex[%i] unselected by pick ray.", selected);
					m->ClearVertexFlags(selected, Vertex::fSelected);
					freyja_event_gl_refresh();
				}
			}
		}
		break;

	case tFace:
		{
			// New backend ( picks faces and marks them selected )
			Mesh *m = Mesh::GetMesh(GetSelectedMesh());

			if ( m )
			{
				int selected = -1;
				m->IntersectFaces(FreyjaRender::mTestRay, selected, false);
				
				if (selected > -1)
				{
					freyja_print("Face[%i] selected by pick ray.", selected);
					m->ClearFaceFlags(selected, Face::fSelected);
				}
			}
		}
		break;

	case tSelectedMeshes:
		{
			vec_t t, best = 9999.9f;
			int32 selected = -1;

			for (uint32 i = 0, n = freyjaGetMeshCount(); i < n; ++i)
			{
				Mesh *m = freyjaGetMeshClass(i);

				if (!m)
					continue;

				if (m->IntersectPerFace(FreyjaRender::mTestRay, t))
				{
					if (t < best)
					{
						best = t;
						selected = i;
					}
				}
			}

			if ( selected > -1 )
			{
				freyja_print("Mesh[%i] unselected by pick ray.", selected);
				//SetSelectedMesh(selected);

				Mesh *m = freyjaGetMeshClass(selected);

				if (m)
				{
					m->ClearFlag(Mesh::fSelected);
					//mCursor.mPos = m->GetPosition(); // why was this disabled?
				}
			}
		}
		break;

	default:
		{
			String s;
			s = ObjectTypeToString(mObjectMode);
			freyja_print("UnselectObject '%s' not supported.", s.GetCString());
			MARK_MSGF("Case '%s' not supported", s.GetCString());
		}
	}
}


void FreyjaControl::SelectObjectByBox(hel::Vec3 min, hel::Vec3 max)
{
	switch (mObjectMode)
	{
	case tSelectedFaces:
	case tFace:
		{
			Mesh *m = Mesh::GetMesh(GetSelectedMesh());

			if ( m )
			{
				m->SelectFacesByBox(min, max);
			}
		}
		break;

	case tPoint:
		{
			Mesh *m = Mesh::GetMesh(GetSelectedMesh());

			if ( m )
			{
				m->SelectVerticesByBox(min, max);
			}
		}
		break;

	default:
		{
			String s = ObjectTypeToString(mObjectMode);
			freyja_print("%s() '%s' not supported.", __func__, s.GetCString());
		}
	}	
}


void FreyjaControl::SelectObject(vec_t mouseX, vec_t mouseY)
{
	// Look for objects with depth ( was using planar projection )
	//mRender->SetFlag(FreyjaRender::fDrawPickRay); // debugging
	CastPickRay(mouseX, mouseY);

	switch (mObjectMode)
	{
	case tSelectedMeshes:
		{
			vec_t t, best = 9999.9f;
			int32 selected = -1;

			for (uint32 i = 0, n = freyjaGetMeshCount(); i < n; ++i)
			{
				Mesh *m = freyjaGetMeshClass(i);

				if (!m)
					continue;

				if (m->IntersectPerFace(FreyjaRender::mTestRay, t))
				{
					if (t < best)
					{
						best = t;
						selected = i;
					}
				}
			}

			if ( selected > -1 )
			{
				freyja_print("Mesh[%i] selected and marked by pick ray.", selected);
				SetSelectedMesh(selected);

				Mesh *m = freyjaGetMeshClass(selected);

				if (m)
				{
					m->SetFlag(Mesh::fSelected);
					mCursor.mPos = m->GetPosition(); // why was this disabled?
				}
			}
		}
		break;


	case tPoint:
		{
			Mesh *m = Mesh::GetMesh(GetSelectedMesh());

			if ( m )
			{
				int selected = -1;
				m->IntersectClosestVertex(FreyjaRender::mTestRay, selected, 2.0f);
				
				if (selected > -1)
				{
					freyja_print("! Vertex[%i] selected by pick ray.", selected);
					m->SetVertexFlags(selected, Vertex::fSelected);
					SetSelectedVertex(selected);
					mCursor.mPos = m->GetVertexPosition(selected);
				}
			}
		}
		break;

	case tLight:
		{
			hel::Vec3 xyz;
			vec4_t pos;
			vec_t t, closest;
			int selected = -1;

			for (uint32 i = 0, count = freyjaGetLightCount(); i < count; ++i)
			{
				freyjaGetLightPosition4v(i, pos);
				xyz = hel::Vec3(pos);

				if (FreyjaRender::mTestRay.IntersectSphere(xyz.mVec, 5.0f, t))
				{
					if (selected == -1 || t < closest)
					{
						selected = i;
						closest = t;
					}
				}
			}

			if (selected > -1)
			{
				freyjaCurrentLight(selected);
				mCursor.mPos = xyz;
				freyja_print("! Light[%i] selected by pick ray.", selected);
			}
		}
		break;

	case tFace:
		{
			// New backend ( picks faces and marks them selected )
			Mesh *m = Mesh::GetMesh(GetSelectedMesh());

			if ( m )
			{
				int selected = -1;
				m->IntersectClosestFace(FreyjaRender::mTestRay, selected);
				
				if (selected > -1)
				{
					freyja_print("Face[%i] selected by pick ray.", selected);
					m->SetFaceFlags(selected, Face::fSelected);
					SetSelectedFace(selected);
					//mCursor.mPos = m->GetFacePosition();
				}
			}
		}
		break;

	case tBone:
		{
			hel::Vec3 p;
			vec_t t, closest;
			int selected = -1;

			for (uint32 i = 0, count = freyjaGetBoneCount(); i < count; ++i)
			{
				freyjaBoneBindTransformVertex(i, p.mVec, 1.0f);
				//freyjaGetBoneWorldPos3fv(i, p.mVec);

				if (FreyjaRender::mTestRay.IntersectSphere(p.mVec, 0.5f, t))
				{
					if (selected == -1 || t < closest)
					{
						selected = i;
						closest = t;
					}
				}
			}

			if (selected > -1)
			{
				SetSelectedBone(selected);

				mCursor.mPos = hel::Vec3();
				freyjaBoneBindTransformVertex(selected, mCursor.mPos.mVec, 1.0f);

				switch (GetControlScheme())
				{
				case eScheme_Animation:
					{
						Bone *b = Bone::GetBone(GetSelectedBone());
						if (b)
						{
							uint32 anim = GetSelectedAnimation();
							BoneTrack &track = b->GetTrack(anim);
							uint32 k = GetSelectedKeyFrame();
							Vec3KeyFrame *key = track.GetRotKeyframe(k);
							
							if (key)
							{
								// FIXME Not using relative keyframes
								mCursor.mRotate = key->GetData();
							}

							Vec3KeyFrame *keyLoc = track.GetLocKeyframe(k);
							
							if (keyLoc)
							{
								// Relative keyframes
								mCursor.mPos += keyLoc->GetData();
							}
						}
					}
					break;

				case eScheme_Model:
					{
						hel::Vec3 o;
						// Set cursor rotation
						freyjaGetBoneRotationEuler3fv(GetSelectedBone(),o.mVec);
						mCursor.mRotate.mVec[0] = helRadToDeg(o.mVec[0]);
						mCursor.mRotate.mVec[1] = helRadToDeg(o.mVec[1]);
						mCursor.mRotate.mVec[2] = helRadToDeg(o.mVec[2]);
					}
					break;
					
				default:
					;
				}

				freyja_print("! Bone[%i] '%s' selected by pick ray.", 
							 selected, freyjaGetBoneNameString(selected));
			}
		}
		break;

	case tMesh:
		{
			vec_t t, best = 9999.9f;
			int32 selected = -1;

			for (uint32 i = 0, n = freyjaGetMeshCount(); i < n; ++i)
			{
				Mesh *m = freyjaGetMeshClass(i);

				if (!m)
					continue;

				if (m->IntersectPerFace(FreyjaRender::mTestRay, t))
				{
					if (t < best)
					{
						best = t;
						selected = i;
					}
				}
			}

			if ( selected > -1 )
			{
				freyja_print("Mesh[%i] selected by pick ray.", selected);
				SetSelectedMesh(selected);

				Mesh *m = freyjaGetMeshClass(selected);

				if (m)
				{
#if 0
					m->SetFlags(selected, Mesh::fSelected);
#endif
					mCursor.mPos = m->GetPosition(); // why was this disabled?
				}
			}
		}
		break;


	default:
		{
			String s;
			s = ObjectTypeToString(mObjectMode);
			freyja_print("SelectObject '%s' not supported.", s.GetCString());
			MARK_MSGF("Case '%s' not supported", s.GetCString());
		}
	}
}


void FreyjaControl::KeyframeTransform(object_type_t obj, 
									  freyja_transform_action_t action,
									  vec_t x, vec_t y, vec_t z)
{
	// Pretty meger atm
	hel::Vec3 v(x, y, z);
	hel::Vec3 u;

	switch (action)
	{
	case fRotateAboutOrigin:
	case fRotate:
		u = v;
		break;

	case fScaleAboutOrigin:
	case fScale:
		u = v;
		u.mVec[0] = 1.0f / v.mVec[0];
		u.mVec[1] = 1.0f / v.mVec[1];
		u.mVec[2] = 1.0f / v.mVec[2];
		break;
		
	case fTranslate:
		u = -v;
		break;

	default:
		MARK_MSGF("%s(): type=%s, action=%i not implemented.", 
				  __func__, ObjectTypeToString(obj).c_str(), action);
		return;
	}

	
	if (mToken)
	{
		int32 id = -1;

		switch (obj)
		{
		case tModel:
			id = 0;
			break;

		case tMesh:
			id = GetSelectedMesh();
			break;

		default:
			id = -1;
		}

		freyja_print("! %s[%i].keyframe[%i] %s <%f, %f, %f>",
					 ObjectTypeToString(obj).c_str(),
					 id, 
					 GetSelectedKeyFrame(),
					 ActionToString(GetEventAction()).c_str(),
					 GetEventAction(),
					 v.mVec[0], v.mVec[1], v.mVec[2]);
	}

	switch (obj)
	{
	case tMesh:
		if (mToken)
		{
			//Action *a = new ActionMeshKeyframe(GetSelectedMesh(), action, u);
			//ActionModelModified(a);

			Mesh *m = Mesh::GetMesh(GetSelectedMesh());
			if (m)
			{				
				// FIXME: This is temp test constant - replace with track switching later
				const uint32 track = 0;

				uint32 k = FreyjaControl::mInstance->GetSelectedKeyFrame();	
				Vec3x3KeyFrame *key = m->GetTransformTrack(track).GetKeyframe(k);
				
				if (key)
				{
					switch (action)
					{
					case fRotate:
						key->mData[0] = v;
						break;
						
					case fScale:
						key->mData[1] = v;
						break;
						
					case fTranslate:
						key->mData[2] = v;
						break;

					default:
						freyja_print("! Keyframe transform (%s) is unsupported",
									 ActionToString(action).c_str());
					}
				}
			}
		}
		break;

	default:
		freyja_print("%s(): type=%s, action=%i not implemented.", 
					 __func__, ObjectTypeToString(obj).c_str(), action);
	}
}


void FreyjaControl::Transform(object_type_t obj, 
							  freyja_transform_action_t action,
							  vec_t x, vec_t y, vec_t z) 
{
	switch (GetControlScheme())
	{
	case eScheme_Animation:
		KeyframeTransform(obj, action, x, y, z);
		return;
		break;

	default:
		;
	}

	hel::Vec3 v(x, y, z);
	hel::Vec3 u;

	switch (action)
	{
	case fRotateAboutOrigin:
	case fRotate:
		v *= HEL_PI_OVER_180;
		u = -v;
		break;

	case fScaleAboutOrigin:
	case fScale:
		u.mVec[0] = 1.0f / v.mVec[0];
		u.mVec[1] = 1.0f / v.mVec[1];
		u.mVec[2] = 1.0f / v.mVec[2];
		break;
		
	case fTranslate:
		u = -v;
		break;

	default:
		MARK_MSGF("Undo for %s action=%i not implemented", 
				  ObjectTypeToString(obj).c_str(), action);
	}

	
	if (mToken)
	{
		String s;

		switch (obj)
		{
		case tModel:
			s.Set("%i", GetSelectedModel());
			break;

		case tMesh:
			s.Set("%i", GetSelectedMesh());
			break;

		case tFace:
			s.Set("%i", GetSelectedFace());
			break;

		case tPoint:
			s.Set("%i", GetSelectedVertex());
			break;

		case tSelectedMeshes:
			s = "...";
			break;

		case tSelectedFaces:
			s = "...";
			break;

		case tSelectedVertices:
			s = "...";
			break;

		default:
			s = "?";
		}

		freyja_print("! %s[%s] %s <%f, %f, %f>",
					 ObjectTypeToString(obj).c_str(),
					 s.c_str(), 
					 ActionToString(GetEventAction()).c_str(),
					 GetEventAction(),
					 v.mVec[0], v.mVec[1], v.mVec[2]);

	}

	switch (obj)
	{
#if 0
	case tFace: 
		if (mToken)
		{
			Mesh *m = Mesh::GetMesh(GetSelectedMesh());

			if (!m)
			{
				freyja_print("No Mesh selected for Faces...");
				return;
			}

			Matrix mat;
		  
			switch (action)
			{
			case fRotate:
				mat.Rotate(v);
				break;

			case fScale:
				mat.Scale(v);
				break;

			case fTranslate:
				mat.Translate(v);
				break;

			default:
				freyja_print("%s(): Does not support (%s) for Faces...", 
							 __func__, ActionToString(action).c_str());
				return;
			}
			
			Vector<index_t> faces;
			face.push_back(GetSelectedFace());
			m->TransformFacesInList(faces, mat);
			Action *a = new ActionFacesTransform(GetSelectedMesh(), faces, mat);
			ActionModelModified(a);
		}
		break;
#endif

	case tSelectedFaces:		
		if (mToken)
		{
			Mesh *m = Mesh::GetMesh(GetSelectedMesh());

			if (!m)
			{
				freyja_print("No Mesh selected for Faces...");
				return;
			}

			hel::Mat44 mat;
		  
			switch (action)
			{
			case fRotate:
				mat.Rotate(v);
				break;

			case fScale:
				mat.Scale(v);
				break;

			case fTranslate:
				mat.Translate(v);
				break;

			default:
				freyja_print("%s(): Does not support (%s) for Faces...", 
							 __func__, ActionToString(action).c_str());
				return;
			}
			
			Vector<index_t> faces = m->GetSelectedFaces();
			m->TransformFacesInList(faces, mat);
			Action *a = new ActionFacesTransform(GetSelectedMesh(), faces, mat);
			ActionModelModified(a);
		}
		break;

	case tBone:
		if (mToken)
		{
			Action *a = new ActionBoneTransform(GetSelectedBone(), action, v);
			ActionModelModified(a);
			freyjaBoneTransform3fv(GetSelectedBone(), action, v.mVec);
			freyjaBoneUpdateBindPose(GetSelectedBone());
		}
		break;

	case tMesh:
		if (mToken)
		{
			Action *a = new ActionMeshTransform(GetSelectedMesh(), action, u);
			ActionModelModified(a);
			freyjaMeshTransform3fv(GetSelectedMesh(), action, v.mVec);
		}
		break;

	case tModel:
		if (mToken)
		{
			Action *a = new ActionModelTransform(0, action, u);
			ActionModelModified(a);
			freyjaModelTransform(GetSelectedModel(), action, v[0], v[1], v[2]);
		}
		break;

	default:
		MARK_MSGF("Not Implemented"); 
	}


	// Reset transforms in the cursor
	//mCursor.mRotate = hel::Vec3(0,0,0);
	mCursor.mScale = hel::Vec3(1,1,1);
	//mCursor.mPos = hel::Vec3(0,0,0);
}


void FreyjaControl::Transform(freyja_transform_t obj, 
							  freyja_transform_action_t action,
							  index_t owner, index_t id,
							  vec_t x, vec_t y, vec_t z) 
{ 
	hel::Vec3 v(x, y, z);

	if (mToken) 
	{
		Action *a = new ActionGenericTransform(obj, action, owner, id, v);
		ActionModelModified(a);
	}

	switch (obj)
	{
	case fTransformMesh:
		freyjaMeshTransform3fv(GetSelectedMesh(), action, v.mVec);
		break;

	default:
		MARK_MSGF("Not Implemented");
	}
}


void FreyjaControl::MoveObject(vec_t vx, vec_t vy)
{
	hel::Vec3 t;

	mCursor.mPos.Get(t.mVec);
	getScreenToWorldOBSOLETE(vx, vy);

	/* Exact movement based on cursor position, 
	 * but limited to a plane of movement */
	switch (GetSelectedView())
	{
	case PLANE_BACK:
		t.mVec[0] = -vx - t.mVec[0];
		t.mVec[1] = vy - t.mVec[1];
		t.mVec[2] = 0;
		break;

	case PLANE_BOTTOM:
		t.mVec[0] = vx - t.mVec[0];
		t.mVec[1] = 0;
		t.mVec[2] = -vy - t.mVec[2];
		break;

	case PLANE_RIGHT:
		t.mVec[0] = 0;
		t.mVec[1] = vy - t.mVec[1];
		t.mVec[2] = -vx - t.mVec[2];
		break;

	case PLANE_FRONT:
		t.mVec[0] = vx - t.mVec[0];
		t.mVec[1] = vy - t.mVec[1];
		t.mVec[2] = 0;
		break;

	case PLANE_TOP:
		t.mVec[0] = vx - t.mVec[0];
		t.mVec[1] = 0;
		t.mVec[2] = vy - t.mVec[2];
		break;

	case PLANE_LEFT:
		t.mVec[0] = 0;
		t.mVec[1] = vy - t.mVec[1];
		t.mVec[2] = vx - t.mVec[2];
		break;

	case PLANE_FREE: // FIXME: Use ray casting to move cursor along basis
		t.mVec[0] = vx - t.mVec[0];
		t.mVec[1] = vy - t.mVec[1];
		t.mVec[2] = vx - t.mVec[2];
		break;

	default:
		freyja_print("! %s(): This view (%i) isn't supported.", 
					__func__, GetSelectedView());
		return;
	}


	/* Cursor axis determined limited movement */
	switch (mCursor.mAxis)
	{
	case freyja3d::Cursor::eAll:
		break;

	case freyja3d::Cursor::eX:
		t.mVec[1] = 0;
		t.mVec[2] = 0;
		break;

	case freyja3d::Cursor::eY:
		t.mVec[0] = 0;
		t.mVec[2] = 0;
		break;

	case freyja3d::Cursor::eZ:
		t.mVec[0] = 0;
		t.mVec[1] = 0;
		break;

	case freyja3d::Cursor::eNone:
		return;
		break;
	}

	mCursor.mPos += t;
	mCursor.SetMode(freyja3d::Cursor::Translation);

	if (mEventMode == aSelectByBox)
	{
		mControlPoints[mSelectedControlPoint] = mCursor.mPos;
		return;
	}

	switch (mObjectMode)
	{
	case tLight:
		{
			vec3_t pos;
			freyjaGetLightPosition4v(GetSelectedLight(), pos);
			mCursor.mLastPos = hel::Vec3(pos);
			freyjaLightPosition4v(GetSelectedLight(), mCursor.mPos.mVec);
		}
		break;


	case tModel: 
		{
			Mesh *m = Mesh::GetMesh(GetSelectedMesh());
			if (m)
			{
				mCursor.mLastPos = m->GetPosition();
			}

			mToken = true;
		}
		break;

	case tMesh:
		{	
			Mesh *m = Mesh::GetMesh(GetSelectedMesh());

			if (m)
			{
				mCursor.mLastPos = m->GetPosition();
			}

			mToken = true;
		}
		break;

	case tSelectedVertices:
		{
			Mesh *m = Mesh::GetMesh(GetSelectedMesh());

			if (m)
			{
				if (mToken) 
				{
					Vector<index_t> list;
					Vector<hel::Vec3> list2;
					bool found = false;

					for (uint32 i = 0, n = m->GetVertexCount(); i < n; ++i)
					{
						Vertex *v = m->GetVertex(i);
						
						if (v && v->mFlags & Vertex::fSelected)
						{
							if (!found)
							{
								// should be i?
								mCursor.mPos = m->GetVertexPosition(v->mVertexIndex);
								found = true;
							}

							list.pushBack(i);
							// should be i?
							list2.pushBack(m->GetVertexPosition(v->mVertexIndex));
						}
					}

					
					Action *a = new ActionVertexListTransformExt(GetSelectedMesh(), list, fTranslate, list2, mCursor.mPos);
					ActionModelModified(a);

					return;
				}

				hel::Vec3 u;

				for (uint32 i = 0, n = m->GetVertexCount(); i < n; ++i)
				{
					Vertex *v = m->GetVertex(i);
	
					if (v && v->mFlags & Vertex::fSelected)
					{
						u = t + m->GetVertexPosition(v->mVertexIndex);
						m->SetVertexPos(v->mVertexIndex, u.mVec);
					}
				}
			}
		}
		break;

	case tBone: 
		{
			// FIXME: This shouldn't be 'set' here

			// Get _WORLD_ position of bone, and use _WORLD_ space transforms
			// to match the rest of this system, since local space design
			// was canned

			index_t bone = GetSelectedBone();
			index_t parent = freyjaGetBoneParent(bone);
			Bone *b = Bone::GetBone(bone);

			if (!b)
			{
				freyja_print("Tried to move invalid bone");
			}
			else if (parent == INDEX_INVALID)
			{
				freyjaBoneTranslate3fv(bone, mCursor.mPos.mVec);
			}
			else if (bone != parent) // FIXME: only edits for bind pose for now
			{
				hel::Vec3 p;
				freyjaBoneBindTransformVertex(parent, p.mVec, 1.0f);
				//freyjaGetBoneWorldPos3fv(parent, p.mVec);
				//p = b->mRotation.rotate(p);
				p = mCursor.mPos - p;
				freyjaBoneTranslate3fv(bone, p.mVec);
			}
		}
		break;

	case tFace: 
		{
			Mesh *m = Mesh::GetMesh(GetSelectedMesh());

			if (m)
			{				
				if (mToken) 
				{
					Vector<index_t> list;
					Vector<hel::Vec3> list2;
					bool found = false;

					Face *f = m->GetFace(GetSelectedFace());

					if (!f)
						return;

					uint32 i, idx;
					foreach (f->mIndices, i)
					{
						idx = f->mIndices[i];

						Vertex *v = m->GetVertex(idx);

						if (v)
						{
							if (!found)
							{
								// should be idx?
								mCursor.mPos = m->GetVertexPosition(v->mVertexIndex);
								found = true;
							}

							list.pushBack(idx);
							// should be idx?
							list2.pushBack(m->GetVertexPosition(v->mVertexIndex));
						}
					}

					
					Action *a = new ActionVertexListTransformExt(GetSelectedMesh(), list, fTranslate, list2, mCursor.mPos);
					ActionModelModified(a);

					return;
				}

				Face *f = m->GetFace(GetSelectedFace());
				hel::Vec3 u;
				uint32 i, idx;
				if (f)
				foreach (f->mIndices, i)
				{
					idx = f->mIndices[i];
					u = t + m->GetVertexPosition(idx);
					m->SetVertexPos(idx, u.mVec);
				}
			}
		}
		break;


	case tSelectedFaces:		
		{
			// FIXME: Update render list for fSelected or you will only
			//        see the cursor move!
			mToken = true;
		}
		break;

	case tPoint:
		if (mToken) 
		{
			Mesh *m = Mesh::GetMesh(GetSelectedMesh());

			if (m)
			{
				m->GetVertexPos(GetSelectedVertex(), mCursor.mPos.mVec);
			}

			Action *a = new ActionVertexTransformExt(GetSelectedMesh(), GetSelectedVertex(), fTranslate, mCursor.mPos.mVec, mCursor.mPos);
			ActionModelModified(a);
		}
		freyjaMeshVertexPos3fv(GetSelectedMesh(), GetSelectedVertex(), mCursor.mPos.mVec);
		break;


	default:
		if (mToken) 
		{
			//mActionManager.Push(new ActionGenericTransformExt(GetSelectedMesh(), fTranslate, mCursor.mPos.mVec, mCursor.mPos));
		}

		//Transform(mObjectMode, fTranslate, xf, yf, zf);
		break;
	}
}


//FIXME: Using object Pos as a pivot and selected point on cursor arc...
//       mark off displacement of motion and rotate by that angle
void FreyjaControl::rotateObject(int x, int y, freyja_plane_t plane)
{
	// TODO Rewrite this to use pivot and arc delta ( Generate the delta sweep )
	static int old_y = 0, old_x = 0;
	const float t = 0.5f, m = 1.0f;
	hel::Vec3 o, d;
	//float xf, yf, zf;
	int swap;

	// Swap x, y and truncate for stepping...
	swap = x;
	x = -y;
	y = -swap;

	/* Mongoose: Compute a relative movement value too here */
	vec_t dx = ((x < old_x - t) ? -m : ((x > old_x + t) ? m : 0));
	vec_t dy = ((y < old_y - t) ? m : ((y > old_y + t) ? -m : 0));

	switch (plane)
	{
	case PLANE_BACK:
		d = hel::Vec3(dx, dy, 0.0f);
		break;

	case PLANE_FRONT:
		d = hel::Vec3(dx, -dy, 0.0f);
		break;

	case PLANE_BOTTOM:
		d = hel::Vec3(-dx, 0.0f, dy);
		break;

	case PLANE_TOP:
		d = hel::Vec3(dx, 0.0f, dy);
		break;

	case PLANE_RIGHT:
		d = hel::Vec3(0.0f, dy, -dx);
		break;

	case PLANE_LEFT:
		d = hel::Vec3(0.0f, dy, dx);
		break;

	case PLANE_FREE:
		d = hel::Vec3(dy, dy, dy);
		break;

	default:
		freyja_print("! RotateObject(): This view (%i) isn't supported.",plane);
		return;
	}


	/* Cursor axis determined limited movement */
	switch (mCursor.mAxis)
	{
	case freyja3d::Cursor::eAll:
		break;

	case freyja3d::Cursor::eX:
		d.mVec[1] = 0;
		d.mVec[2] = 0;
		break;

	case freyja3d::Cursor::eY:
		d.mVec[0] = 0;
		d.mVec[2] = 0;
		break;

	case freyja3d::Cursor::eZ:
		d.mVec[0] = 0;
		d.mVec[1] = 0;
		break;

	case freyja3d::Cursor::eNone:
		return;
		break;
	}

	switch (mObjectMode)
	{
	case tBone:
		{
			// Reduce rotation for bones to be 1/2 normal rate
			d *= 0.5f;

			// You have to reset cursor on selection / mode change now!
		}
		break;

	default:
		;
	}

	mCursor.mRotate += d; 

	// Interface is in degrees, while backend is radians
	if (mCursor.mRotate.mVec[0] > 360.0f) 
		mCursor.mRotate.mVec[0] -= 360.0f;

	if (mCursor.mRotate.mVec[1] > 360.0f)
		mCursor.mRotate.mVec[1] -= 360.0f;

	if (mCursor.mRotate.mVec[2] > 360.0f)
		mCursor.mRotate.mVec[2] -= 360.0f;

	mCursor.SetMode(freyja3d::Cursor::Rotation);

	switch (mObjectMode)
	{
	case tBone:
		{
			vec3_t xyz;

#if 1
			xyz[0] = helDegToRad(mCursor.mRotate.mVec[0]);
			xyz[1] = helDegToRad(mCursor.mRotate.mVec[1]);
			xyz[2] = helDegToRad(mCursor.mRotate.mVec[2]);
#else
			// DeltaRot
			d *= HEL_PI_OVER_180;
			d.Get(xyz);
#endif
			switch (GetControlScheme())
			{
			case eScheme_Animation:
				{
					Bone *b = Bone::GetBone(GetSelectedBone());
					if (b)
					{
						BoneTrack &track = b->GetTrack(GetSelectedAnimation());
						uint32 k = GetSelectedKeyFrame();
						Vec3KeyFrame *key = track.GetRotKeyframe(k);

						if (key)
						{
							//key->SetEulerRotation(mCursor.mRotate);
							key->SetData(mCursor.mRotate);
						}
					}
				}
				break;

			case eScheme_Model:
				// FIXME!
				//if (!SystemIO::IsFloatNan(xyz[0]) && !SystemIO::IsFloatNan(xyz[1]) &&	!SystemIO::IsFloatNan(xyz[2]))
				{
					hel::Vec3 p;
					freyjaBoneRotateEuler3fv(GetSelectedBone(), xyz);
#if 0
					freyjaGetBoneTranslation3fv(GetSelectedBone(), p.mVec);
					freyja_print("rot %f %f %f, loc %f %f %f", 
								 xyz[0], xyz[1], xyz[2],
								 p.mVec[0], p.mVec[1], p.mVec[2]);
#endif
				}
				freyja_print("rot %f %f %f", xyz[0], xyz[1], xyz[2]);
				break;

			default:
				;
			}
		}
		break;


	case tMesh:
		{
			mCursor.SetMode(freyja3d::Cursor::Rotation);

			// This handles ctrl selects ( select without leaving rotate mode )
			Mesh *m = Mesh::GetMesh(GetSelectedMesh());
			if (m) 
			{
				mCursor.mPos = m->GetBoundingVolumeCenter();
				mToken = true;
			}
		}
		break;

	case tModel:
		mToken = true;

	default:
		{
			//hel::Vec3 r(xf, yf, zf);

			mCursor.mRotate += d;//r;

			// Interface is in degrees, while backend is radians
			if (mCursor.mRotate.mVec[0] > 360.0f) 
				mCursor.mRotate.mVec[0] -= 360.0f;

			if (mCursor.mRotate.mVec[1] > 360.0f)
				mCursor.mRotate.mVec[1] -= 360.0f;

			if (mCursor.mRotate.mVec[2] > 360.0f)
				mCursor.mRotate.mVec[2] -= 360.0f;

			mCursor.SetMode(freyja3d::Cursor::Rotation);
		}

		freyja_print("! RotateObject(): ObjectType (%i) not handled.", 
					 ObjectTypeToString(mObjectMode).c_str());
	}
	
	old_x = x;
	old_y = y;
}


void FreyjaControl::scaleObject(int x, int y, freyja_plane_t plane)
{
	const vec_t up = 1.01f, down = 0.99f, stay = 1.0f;
	static int32 old_y = 0, old_x = 0;
	vec_t dx = (x > old_x) ? up : (x < old_x) ? down : stay;
	vec_t dy = (y > old_y) ? down : (y < old_y) ? up : stay;

	switch (plane)
	{
	case PLANE_BACK:
		dx = (x > old_x) ? down : (x < old_x) ? up : stay;
	case PLANE_FRONT:
		mCursor.mScale.mVec[0] *= dx;
		mCursor.mScale.mVec[1] *= dy;
		break;
		
	case PLANE_TOP:
		dy = (y > old_y) ? up : (y < old_y) ? down : stay;
	case PLANE_BOTTOM:
		mCursor.mScale.mVec[0] *= dx;
		mCursor.mScale.mVec[2] *= dy;
		break;
		
	case PLANE_RIGHT:
		dx = (x > old_x) ? down : (x < old_x) ? up : stay;
	case PLANE_LEFT:
		mCursor.mScale.mVec[2] *= dx;
		mCursor.mScale.mVec[1] *= dy;
		break;

	case PLANE_FREE:
		// FIXME: Needs to account for view orientation
		switch (mCursor.mAxis)
		{
		case freyja3d::Cursor::eAll:
			mCursor.mScale.mVec[2] *= dx;
			mCursor.mScale.mVec[1] *= dx;
			mCursor.mScale.mVec[0] *= dx;
			break;
			
		case freyja3d::Cursor::eX:
			mCursor.mScale.mVec[0] *= dx;
			break;
			
		case freyja3d::Cursor::eY:
			mCursor.mScale.mVec[1] *= dx;
			break;
			
		case freyja3d::Cursor::eZ:
			mCursor.mScale.mVec[2] *= dx;
			break;

		default:
			;
		}
		break;

	default:
		freyja_print("! ScaleObject(): This view (%i) isn't supported.", plane);
	}

	old_x = x;
	old_y = y;

	// WTF! It was doing direct scaling like the old, old codebase
	// Leaving this here as reminder for next code clean up
	//Transform(mObjectMode, fScale, 1.0, 0.99, 1.0);
}


void FreyjaControl::SetSelectedMesh(uint32 i) 
{
	index_t model = GetSelectedModel();

	if (i < freyjaGetModelMeshCount(model) && freyjaIsMeshAllocated(i))
	{		
		Mesh *m = Mesh::GetMesh(freyjaGetModelMeshIndex(model, i));
		mSelectedMesh = i;

		if (m)
		{
			mCursor.mPos = m->GetPosition();
		}
	}
}


bool FreyjaControl::LoadRecentFilesResource(const char *filename)
{
	/* Recent files persistance */
	SystemIO::TextFileReader r;
	
	if (r.Open(filename))
	{
		for (uint32 j = 0; j < mRecentFileLimit && !r.IsEndOfFile(); ++j)
		{
			const char *sym = r.GetLine();
			AddRecentFilename(sym);
		}
		
		r.Close();
		return true;
	}

	return false;
}


void FreyjaControl::LoadResource()
{
	String s = freyja_rc_map_string(mResourceFilename.c_str());
	int i, x, y;
	bool failed = true;


	// Setup the UI
	if (!mResource.Load((char *)s.c_str()))
	{
		failed = false;
	}

	if (failed)
	{
		MARK_MSGF("ERROR: Couldn't find resource file '%s'\n", s.c_str());
		freyja_event_shutdown();
	}


	/* GUI stuff */
	LoadUserPreferences();

	if (mResource.Lookup("WINDOW_X", &x))
	{
		if (mResource.Lookup("WINDOW_Y", &y))
		{
			extern void application_window_move(int x, int y);
			freyja_application_window_move(x, y);
		}
	}

	if (mResource.Lookup("DUMP_SYMTAB", &i) && i)
	{
		mResource.Print();
	}

	if (mResource.Lookup("FLUSH_RESOURCE", &i) && i)
	{
		mResource.Flush();
	}

	// FIXME: Rework image plugins with flags like model plugins
	/* Image file dialog patterns - texture */
	uint32 eOpenTextureId = GetEventIdByName("eOpenTexture");
	mgtk_event_fileselection_append_pattern(eOpenTextureId, 
											"All Files (*.*)", "*.*");
	mgtk_event_fileselection_append_pattern(eOpenTextureId, 
											"BMP Image (*.bmp)", "*.bmp");
	mgtk_event_fileselection_append_pattern(eOpenTextureId, 
											"DDS Image (*.dds)", "*.dds");
	mgtk_event_fileselection_append_pattern(eOpenTextureId, 
											"JPEG Image (*.jpg)", "*.jpg");
	mgtk_event_fileselection_append_pattern(eOpenTextureId, 
											"PCX Image (*.pcx)", "*.pcx");
	mgtk_event_fileselection_append_pattern(eOpenTextureId, 
											"PNG Image (*.png)", "*.png");
	mgtk_event_fileselection_append_pattern(eOpenTextureId, 
											"PPM Image (*.ppm)", "*.ppm");
	mgtk_event_fileselection_append_pattern(eOpenTextureId, 
											"TARGA Image (*.tga)", "*.tga");

	/* Shader file dialog patterns */
	uint32 eOpenShaderId = GetEventIdByName("eOpenShader");
	mgtk_event_fileselection_append_pattern(eOpenShaderId, 
											"All Files (*.*)", "*.*");
	mgtk_event_fileselection_append_pattern(eOpenShaderId, 
											"GLSL Fragment (*.frag)", "*.frag");
	mgtk_event_fileselection_append_pattern(eOpenShaderId, 
											"ARB Fragment (*.frag)", "*.frag");

	/* Material file dialog patterns */
	{
		int loadEventId = GetEventIdByName("eOpenMaterial");
		int saveEventId = GetEventIdByName("eSaveMaterial");

		
		mgtk_event_fileselection_append_pattern(loadEventId, 
												"Freyja Material (*.mat)", "*.mat");

		mgtk_event_fileselection_append_pattern(saveEventId, 
												"Freyja Material (*.mat)", "*.mat");
	}


	/* Model file dialog patterns */
	freyjaPluginsInit();

	{
		// yes, you can even query eventIds for even func binds
		int loadEventId = GetEventIdByName("eOpenModel");
		int saveEventId = GetEventIdByName("eSaveModel");
		long i, count = freyjaGetPluginCount();

		mgtk_event_fileselection_append_pattern(loadEventId, 
												"All Files (*.*)", "*.*");

		mgtk_event_fileselection_append_pattern(saveEventId, 
												"All Files (*.*)", "*.*");

		for (i = 0; i < count; ++i)
		{
			freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(i);
			
			if (plugin == NULL)
				continue;

			MSTL_MSG("%i/%i [%s] %s, %s%s\n", i, count, 
					 plugin->mName.c_str(),
					 plugin->mFilename.c_str(),
					 plugin->mImportFlags ? "Import " : "",
					 plugin->mExportFlags ? "Export " : "");

			if (plugin->mImportFlags)
			{
				char *desc = (char*)plugin->mDescription.c_str();
				char *ext = (char*)plugin->mExtention.c_str();
				mgtk_event_fileselection_append_pattern(loadEventId, desc, ext);
			}

			if (plugin->mExportFlags)
			{
				char *desc = (char*)plugin->mDescription.c_str();
				char *ext = (char*)plugin->mExtention.c_str();
				//uint32 menuId = ResourceEvent::GetResourceIdBySymbol("eExportMenu");
				mgtk_event_fileselection_append_pattern(saveEventId, desc, ext);
			}
		}
	}
}


bool FreyjaControl::LoadUserPreferences()
{
	String filename = freyja_rc_map_string(mUserPrefsFilename.c_str());
	MSTL_MSG("\tLoading '%s'...", filename.c_str());	

	return mResource.Load((char *)filename.c_str());	
}


bool FreyjaControl::SaveUserPreferences()
{
	String filename = freyja_rc_map_string(mUserPrefsFilename.c_str());
	SystemIO::TextFileWriter w;
	MSTL_MSG("\tSaving '%s'...", filename.c_str());	

	if (!w.Open(filename.c_str()))
		return false;


	w.Print(";; Custom colors\n");

	for (uint32 i = 0; i < 11; ++i)
	{
		String s;
		float r, g, b, a;
		int id;
	 
		switch (i)
		{
		case 0:
			s = "eColorBackground";
			id = eColorBackground;
			break;

		case 1:
			s = "eColorGrid";
			id = eColorGrid;
			break;

		case 2:
			s = "eColorMesh";
			id = eColorMesh;
			break;

		case 3:
			s = "eColorMeshHighlight";
			id = eColorMeshHighlight;
			break;

		case 4:
			s = "eColorVertex";
			id = eColorVertex;
			break;

		case 5:
			s = "eColorVertexHighlight";
			id = eColorVertexHighlight;
			break;

		case 6:
			s = "eColorBone";
			id = eColorBone;
			break;

		case 7:
			s = "eColorBoneHighlight";
			id = eColorBoneHighlight;
			break;

		case 8:
			s = "eColorLightAmbient";
			id = eColorLightAmbient;
			break;

		case 9:
			s = "eColorLightDiffuse";
			id = eColorLightDiffuse;
			break;

		case 10:
			s = "eColorLightSpecular";
			id = eColorLightSpecular;
			break;

		default:
			continue;
		}

		freyja_event_get_color(id, r, g, b, a);
		w.Print("(color\t%s\t%f %f %f %f)\n", s.c_str(), r, g, b, a);
	}

	w.Print("\n");

	{
		w.Print("; Custom 'toggles'n");
		int n = (mRender->GetFlags() & FreyjaRender::fGrid) ? 1 : 0;
		w.Print("(func_set_toggle eRenderGrid %i)\n", n);

		n = (mRender->GetFlags() & FreyjaRender::fFace) ? 1 : 0;
		w.Print("(func_set_toggle eRenderFace %i)\n", n);

		n = (mRender->GetFlags() & FreyjaRender::fBoundingVolumes) ? 1 : 0;
		w.Print("(func_set_toggle eRenderBbox %i)\n", n);

		n = (mRender->GetFlags() & FreyjaRender::fPoints) ? 1 : 0;
		w.Print("(func_set_toggle eRenderVertex %i)\n", n);

		n = (mRender->GetFlags() & FreyjaRender::fWireframe) ? 1 : 0;
		w.Print("(func_set_toggle eRenderWireframe %i)\n", n);

		n = (mRender->GetFlags() & FreyjaRender::fLighting) ? 1 : 0;
		w.Print("(func_set_toggle eRenderLighting %i)\n", n);

		n = (mRender->GetFlags() & FreyjaRender::fMaterial) ? 1 : 0;
		w.Print("(func_set_toggle eRenderMaterial %i)\n", n);

		n = (mRender->GetFlags() & FreyjaRender::fDrawPickRay) ? 1 : 0;
		w.Print("(func_set_toggle eRenderPickRay %i)\n", n);

		n = (mRender->GetFlags() & FreyjaRender::fBones) ? 1 : 0;
		w.Print("(func_set_toggle eRenderSkeleton %i)\n", n);

		n = (mRender->GetFlags() & FreyjaRender::fBones2) ? 1 : 0;
		w.Print("(func_set_toggle eRenderSkeleton2 %i)\n", n);

		n = (mRender->GetFlags() & FreyjaRender::fBones3) ? 1 : 0;
		w.Print("(func_set_toggle eRenderSkeleton3 %i)\n", n);

		n = (mRender->GetFlags() & FreyjaRender::fBoneName) ? 1 : 0;
		w.Print("(func_set_toggle eRenderBoneName %i)\n", n);

		n = (mRender->GetFlags() & FreyjaRender::fSkeletalVertexBlending) ? 1 : 0;
		w.Print("(func_set_toggle eSkeletalDeform %i)\n", n);
	}

	return true;
}


void FreyjaControl::TexCoordMove(vec_t u, vec_t v)
{
	Mesh *m = Mesh::GetMesh(GetSelectedMesh());

	if (!m || mTexCoordArrayIndex == INDEX_INVALID)
		return;

	static uint32 texcoord = INDEX_INVALID;

	if (texcoord != mTexCoordArrayIndex)
	{
		texcoord = mTexCoordArrayIndex;
		vec3_t uv;
		m->GetTexCoord(mTexCoordArrayIndex, uv);
		
		mActionManager.Push(new ActionTexCoordTransform(GetSelectedMesh(), mTexCoordArrayIndex, uv[0], uv[1]));
	}

	vec3_t uvw = { u, v, 0.0f };
	
	m->SetTexCoord(mTexCoordArrayIndex, uvw);
}


void FreyjaControl::TexCoordSelect(vec_t u, vec_t v)
{
	mTexCoordArrayIndex = INDEX_INVALID;

	Mesh *m = Mesh::GetMesh(GetSelectedMesh());

	if (!m)
		return;

	const vec_t cutoff = 0.005f; // must be within this many 'uv units' to pick
	hel::Vec3 uv(u, v, 0.0f);
	hel::Vec3 uvB;
	Face *f;
	vec_t dist, bestDist = 2.0f; // Outside 0.0f - 1.0f
	
	for (uint32 i = 0, n = m->GetFaceCount(); i < n; ++i)
	{
		f = m->GetFace(i);

		/* Only consider UVs using selected material */
		if (!f || f->mMaterial != GetSelectedMaterial())
			continue;

		// NOTE: Right now we can have mixed texcoord polymapped faces
		/* Compare UVs as 2 space coords ( for the most part ) */
		if (f->mFlags & Face::fPolyMappedTexCoords)
		{
			uint32 j;
			foreach (f->mTexCoordIndices, j)
			{
				m->GetTexCoord(f->mTexCoordIndices.current(j), uvB.mVec);
				uvB.mVec[2] = 0.0f;
				dist = helDist3v(uv.mVec, uvB.mVec);
				
				if (dist < cutoff && dist < bestDist)
				{
					bestDist = dist;
					mTexCoordArrayIndex = f->mTexCoordIndices.current(j);
				}
			}
		}
		else
		{
			for (uint32 j = 0, jn = f->mIndices.size(); j < jn; ++j)
			{
				// FIXME: This must change when vertices use texcoordIndex
				m->GetTexCoord(f->mIndices[j], uvB.mVec);
				uvB.mVec[2] = 0.0f;				
				dist = helDist3v(uv.mVec, uvB.mVec);
				
				if (dist < cutoff && dist < bestDist)
				{
					bestDist = dist;
					mTexCoordArrayIndex = f->mIndices[j];
				}				
			}
		}
	}

	((mTexCoordArrayIndex == INDEX_INVALID) ? 
	 freyja_print("No UVs selected!") : 
	 freyja_print("Selected UV[%i].  Move with Right Mouse Button.", mTexCoordArrayIndex));
}


void FreyjaControl::CreateListener(const char *name, MethodPtr ptr)
{
	MethodDelegate *d = new MethodDelegateArg0<FreyjaControl>(mInstance, ptr);
	ResourceEventDelegate::add(name, d);
}


void FreyjaControl::CreateListener(const char *name, bMethodPtr ptr)
{
	MethodDelegate *d = new MethodDelegateArg0<FreyjaControl, bool>(mInstance, ptr);
	ResourceEventDelegate::add(name, d);
}


void FreyjaControl::CreateListener(const char *name, MethodPtr1u ptr)
{
	MethodDelegate *d = new MethodDelegateArg1<FreyjaControl, unsigned int>(mInstance, ptr);
	ResourceEventDelegate::add(name, d);
}


void FreyjaControl::CreateListener(const char *name, MethodPtr1f ptr)
{
	MethodDelegate *d = new MethodDelegateArg1<FreyjaControl, float>(mInstance, ptr);
	ResourceEventDelegate::add(name, d);
}


void FreyjaControl::CreateListener(const char *name, MethodPtr1s ptr)
{
	MethodDelegate *d = new MethodDelegateArg1<FreyjaControl, char*>(mInstance, ptr);
	ResourceEventDelegate::add(name, d);
}


void FreyjaControl::CreateListener(const char *name, MethodPtr2s ptr)
{
	MethodDelegate *d = new MethodDelegateArg2<FreyjaControl, char*, char*>(mInstance, ptr);
	ResourceEventDelegate::add(name, d);
}


//////////////////////////////////////////////////////////////////////////
// Non-class methods, callbacks, and wrappers
//////////////////////////////////////////////////////////////////////////

int load_texture(const char *filename)
{
	int id = 0;
	FreyjaControl::mInstance->LoadTexture(filename, id);
	freyja_print("! Texture callback %i : '%s'...", id, filename);
	return id;
}


int load_shader(const char *filename)
{
	uint32 id = 0;

	freyja_print("! Shader callback %i : '%s'...", id, filename);

	if (!freyja3d::OpenGL::LoadFragmentGLSL(filename, id))
		return 0;

	return id;
}

