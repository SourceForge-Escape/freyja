/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://icculus.org/freyja/
 * Email   : mongoose@icculus.org
 * Object  : FreyjaControl
 * License : No use w/o permission (C) 2000 Mongoose
 * Comments: This is the controler class for the client
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2000-09-10:
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
#include <freyja/FreyjaPlugin.h>
#include <freyja/FreyjaPluginABI.h>
#include <freyja/Mesh.h>
#include <freyja/MeshABI.h>
#include <freyja/FreyjaImage.h>
#include <freyja/PerlinNoise.h>
#include <hel/math.h>
#include <hel/Ray.h>

#include "freyja_events.h"

#include "FreyjaControl.h"


using namespace mstl;
using namespace freyja;
using namespace freyja3d;

#define DEBUG_PICK_RAY 0
#define DEBUG_PICK_RAY_PLANAR 0
#define DEBUG_SCREEN_TO_WORLD 0
#define DEBUG_VIEWPORT_MOUSE 0
#define FREYJA_RECENT_FILES "recent_files-dev"

void mgtk_event_dialog_visible_set(int dialog, int visible);
extern void freyja__setPrinter(FreyjaPrinter *printer, bool freyjaManaged);

FreyjaControl *FreyjaControl::mInstance = NULL;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

FreyjaControl::FreyjaControl() :
	mSelectedTexture(0),
	mSelectedViewport(0),
	mActionManager(),
	mFlags(fNone),
	mObjectMode(tScene),
	mResourceFilename("freyja-dev.mlisp"),
	mCurrentlyOpenFilename(),
	mSceneTrans(0.0f, -18.0f, 0.0f),
	mResource(),
	mRender(NULL),
	mCleared(true),
	mAllowBoneNameUpdate(true),
	mToken(false)
{
	/* Search local paths first ( mostly debugging ) */
	freyjaPluginAddDirectory("plugins/model/debug");

	/* Search local paths for things like windows builds */
	freyjaPluginAddDirectory("modules/model");

	/* Search ~/.freyja/plugins/ second ( first for real path for end users ) */
	String sPluginDir = freyja_rc_map_string("plugins/");
	freyjaPluginAddDirectory(sPluginDir.GetCString());

	/* Start up libfreyja backend, and redirect it's stdio */
	freyjaSpawn();
	freyja__setPrinter(&mPrinter, false);

	/* Spawn 0th light, and set the light iterator */
	vec4_t lightPos = {12,35,8,0}; // Make this RCed {64,64,64,0};
	freyjaCurrentLight(freyjaLightCreate());
	freyjaLightPosition4v(freyjaGetCurrentLight(), lightPos);

	/* Spawn 0th material, set the iterator, and make a default material */
	int32 mIndex = freyjaMaterialCreate();
	vec4_t rgba = {0,0,0,1};
	freyjaCurrentMaterial(mIndex);
	freyjaMaterialName(mIndex, "Boring default");
	freyjaMaterialSpecular(mIndex, rgba);
	freyjaMaterialEmissive(mIndex, rgba);
	rgba[0] = rgba[1] = rgba[2] = 0.9;
	freyjaMaterialDiffuse(mIndex, rgba);
	rgba[0] = rgba[1] = rgba[2] = 0.2;
	freyjaMaterialAmbient(mIndex, rgba);
	freyjaMaterialShininess(mIndex, 0.0f);

	/* Hook up the view */
	mRender = new FreyjaRender();
	ASSERT_MSG(mRender, "FreyjaRender Singleton control allocation failure");

	if (!mRender)
	{
		SystemIO::Print("See ~/.freyja/Freyja.log for possible errors.\n");
		exit(-1);
	}

	mInstance = this;
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
	mEventMode = modeNone;
	mLastEvent = eEvent;
	mLastCommand = eSelect;
	mFullScreen = false;

	/* Mongoose 2002.02.23, Tell renderer to start up with some defaults */
	uint32 width = 740;
	uint32 height = 560;
	mRender->InitContext(width, height, true);
	mRender->ResizeContext(width, height);
	InitTexture();

	// Handle loaded from system call
	if (!freyjaGetCount(FREYJA_BONE) && !freyjaGetCount(FREYJA_VERTEX))
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
			freyjaPolygonSetMaterial1i(mUVMap[i], material);
		}

		return;
	}

	freyjaPolygonSetMaterial1i(faceIndex, material);
}


float FreyjaControl::GetZoom()
{
	return mRender->GetZoom();
}


void FreyjaControl::SetZoom(float zoom)
{
	ASSERT_MSG(zoom > 0.0f, "You can't have a zoom less than 0.0f");

	freyja_event_notify_observer1f(eZoom, zoom);
	mRender->SetZoom(zoom);
	freyja_print("Zoom set to %f", mRender->GetZoom());
}



////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

// Mongoose - 2006.07.31 
// More crap for old system to be backported then rewritten properly  =/
void FreyjaControl::AdjustMouseXYForViewports(vec_t &x, vec_t &y)
{
#if DEBUG_VIEWPORT_MOUSE
	freyja_print("> Mouse x = %f y = %f", x, y);
#endif

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
			SetSelectedView(mRender->mViewports[0].plane);

			// Adjust actual window space mouse x, y to fit to viewport
			// This makes the x, y fill the 'window' for the viewport
			x = x*2.0f;
			y = y*2.0f - h;
		}
		// Handle viewport 2
		else if ( x > halfW && y < halfH )
		{
			SetSelectedView(mRender->mViewports[2].plane);

			// Adjust actual window space mouse x, y to fit to viewport
			// This makes the x, y fill the 'window' for the viewport
			x = x*2.0f - w;
			y = y*2.0f;
		}
		// Handle viewport 1
		else if ( x > halfW && y > halfH )
		{
			SetSelectedView(mRender->mViewports[1].plane);

			// Adjust actual window space mouse x, y to fit to viewport
			// This makes the x, y fill the 'window' for the viewport
			x = x*2.0f - w;
			y = y*2.0f - h;
		}
		// Handle viewport 3
		else if ( x < halfW && y < halfH )
		{
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

	case tMesh:
		s = String("tMesh");
		break;

	case tBone:
		s = String("tBone");
		break;

	case tKeyFrame:
		s = String("tKeyFrame");
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
	freyja_remove_all_items_to_menu(menuId);
		
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
	Ray &r = mRender->mTestRay;

	if (!(GetControlScheme() == eScheme_Model ||
		  GetControlScheme() == eScheme_Animation))
		return;

	if (GetSelectedView() == PLANE_FREE)
	{
#if 0   // This is for a pers view ray pick, which deps on camera
		double rayOrigin[4];
		double rayVector[4];
		getPickRay(x, y, rayOrigin, rayVector);

		HEL_VEC3_COPY(rayOrigin, r.mOrigin.mVec);
		HEL_VEC3_COPY(rayVector, r.mDir.mVec);
#else
		// stop gap fix until new camera system is checked in ( so some things can be tested -- like the camera )
		vec_t z;
		//y += GetSceneTranslation().mVec[1] + 18.0f;
		GetWorldFromScreen(x, y, z);
		Matrix m;
		vec3_t v;
		z += 100;

		y += 10.0f * 1/GetZoom();
		vec3_t u = {x, y, z};
		mRender->GetRotation(v);
		//v[0] = 0;
		v[0] = HEL_DEG_TO_RAD(v[0]);
		v[1] = HEL_DEG_TO_RAD(v[1]);
		v[2] = HEL_DEG_TO_RAD(v[2]);
		DEBUG_MSGF("$$ yaw = %f\n", v[1]);

		m.rotate(v);
		m.Multiply3v(u);
		r.mOrigin = Vec3(u);
		r.mDir = Vec3(0, 0, -1);
		m.Multiply3v(r.mDir.mVec);
#endif
	}
	else
	{
		vec_t z;

		GetWorldFromScreen(x, y, z);

		switch (GetSelectedView())
		{
		case PLANE_BACK:
			r.mOrigin = Vec3(x, y, z - 100);
			r.mDir = Vec3(0, 0, 1);
			break;

		case PLANE_FRONT: // Front, XY
			r.mOrigin = Vec3(x, y, z + 100);
			r.mDir = Vec3(0, 0, -1);
			break;

		case PLANE_BOTTOM:
			r.mOrigin = Vec3(x, y - 100, -z);
			r.mDir = Vec3(0, 1, 0);
			break;

		case PLANE_TOP: // Top, XZ
			r.mOrigin = Vec3(x, y + 100, -z);
			r.mDir = Vec3(0, -1, 0);
			break;

		case PLANE_RIGHT:
			r.mOrigin = Vec3(x + 100, y, z);
			r.mDir = Vec3(-1, 0, 0);
			break;

		case PLANE_LEFT: // Side, ZY
			r.mOrigin = Vec3(x - 100, y, z);
			r.mDir = Vec3(1, 0, 0);
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


bool FreyjaControl::LoadMaterial(const char *filename)
{
	SystemIO::TextFileReader r;


	if (!filename || !filename[0])
	{
		return false;
	}

	if (!r.Open(filename))
	{
		perror("Material::loadFile> ");
		return false;
	}


	int32 matIndex;
	uint32 mode = 0;
	bool perlinLoaded = false;
	vec_t iA = 1.0f, iB = 2.0f, d = 20.0f;
	int32 width = 256, height = 256, seed = 257;
	bool clamp = false;
	float addcolor[3] = {130.0/255.0, 130.0/255.0, 74.0/255.0};
	float modcolor[3] = {156.0/255.0, 130.0/255.0, 89.0/255.0};

	while (!r.IsEndOfFile())
	{
		const char *buffer = r.ParseSymbol();

		if (strncmp(buffer, "[Material]", 11) == 0)
		{
			if (mFlags & fLoadMaterialInSlot)  
			{
				matIndex = freyjaGetCurrentMaterial();
			}
			else
			{
				matIndex = freyjaMaterialCreate();
			}

			mode = 1;
		}
		else if (strncmp(buffer, "[PerlinNoise]", 14) == 0)
		{
			mode = 2;
		}
		else if (mode == 1)
		{
			// Lex('=')
			r.FindNextChar('=');

			if (strncmp(buffer, "Shininess", 9) == 0)
			{				
				freyjaMaterialShininess(matIndex, r.ParseFloat());
			}
			else if  (strncmp(buffer, "Texture", 7) == 0)
			{
				if (LoadTexture(r.ParseStringLiteral()))
				{
					if (mTextureId > -1)
					{
						freyjaMaterialTexture(matIndex, mTextureId-1);
						freyjaMaterialSetFlag(matIndex, fFreyjaMaterial_Texture);
					}
				}
			}
			else if  (strncmp(buffer, "TextureName", 11) == 0)
			{
				if (LoadTexture(r.ParseStringLiteral()))
				{
					if (mTextureId > -1)
					{
						freyjaMaterialTexture(matIndex, mTextureId-1);
						freyjaMaterialSetFlag(matIndex, fFreyjaMaterial_Texture);
					}
				}
			}
			else if  (strncmp(buffer, "Name", 4) == 0)
			{
				freyjaMaterialName(matIndex, r.ParseStringLiteral());
			}
			else if  (strncmp(buffer, "EnableBlending", 14) == 0)
			{
				r.ParseBool();
			}
			else if (strncmp(buffer, "Blend", 5) == 0)
			{
				bool is_src = false;
				if (strncmp(buffer, "BlendSource", 11) == 0)
					is_src = true;

				const char *buf = r.ParseSymbol();
				int32 val = (strncmp(buf, "GL_ZERO", 11) == 0) ? GL_ZERO :
				(strncmp(buf, "GL_SRC_COLOR", 9) == 0) ? GL_SRC_COLOR :
				(strncmp(buf, "GL_ONE_MINUS_SRC_COLOR", 22) == 0) ? GL_ONE_MINUS_SRC_COLOR :
				(strncmp(buf, "GL_DST_COLOR", 9) == 0) ? GL_DST_COLOR :
				(strncmp(buf, "GL_ONE_MINUS_DST_COLOR", 22) == 0) ? GL_ONE_MINUS_DST_COLOR :
				(strncmp(buf, "GL_SRC_ALPHA", 9) == 0) ? GL_SRC_ALPHA :
				(strncmp(buf, "GL_ONE_MINUS_SRC_ALPHA", 22) == 0) ? GL_ONE_MINUS_SRC_ALPHA :
				(strncmp(buf, "GL_DST_ALPHA", 9) == 0) ? GL_DST_ALPHA :
				(strncmp(buf, "GL_ONE_MINUS_DST_ALPHA", 22) == 0) ? GL_ONE_MINUS_DST_ALPHA :
				(strncmp(buf, "GL_SRC_ALPHA_SATURATE", 21) == 0) ? GL_SRC_ALPHA_SATURATE :
				(strncmp(buf, "GL_CONSTANT_COLOR", 17) == 0) ? GL_CONSTANT_COLOR :
				(strncmp(buf, "GL_ONE_MINUS_CONSTANT_COLOR", 27) == 0) ? GL_ONE_MINUS_CONSTANT_COLOR :
				(strncmp(buf, "GL_ONE", 6) == 0) ? GL_ONE :
				(strncmp(buf, "GL_CONSTANT_ALPHA", 17) == 0) ? GL_CONSTANT_ALPHA :					GL_ONE_MINUS_CONSTANT_ALPHA;
				
				if (is_src)
				{
					freyjaMaterialBlendSource(matIndex, val);
				}
				else
				{
					freyjaMaterialBlendDestination(matIndex, val);
				}
			}
			else if (strncmp(buffer, "Ambient", 7) == 0)
			{
				vec4_t c;
				c[0] = r.ParseFloat();
				r.FindNextChar(',');
				c[1] = r.ParseFloat();
				r.FindNextChar(',');
				c[2] = r.ParseFloat();
				r.FindNextChar(',');
				c[3] = r.ParseFloat();
				freyjaMaterialAmbient(matIndex, c);
			}
			else if (strncmp(buffer, "Diffuse", 7) == 0)
			{
				vec4_t c;
				c[0] = r.ParseFloat();
				r.FindNextChar(',');
				c[1] = r.ParseFloat();
				r.FindNextChar(',');
				c[2] = r.ParseFloat();
				r.FindNextChar(',');
				c[3] = r.ParseFloat();
				freyjaMaterialDiffuse(matIndex, c);
			}
			else if (strncmp(buffer, "Specular", 8) == 0)
			{
				vec4_t c;
				c[0] = r.ParseFloat();
				r.FindNextChar(',');
				c[1] = r.ParseFloat();
				r.FindNextChar(',');
				c[2] = r.ParseFloat();
				r.FindNextChar(',');
				c[3] = r.ParseFloat();
				freyjaMaterialSpecular(matIndex, c);
			}
			else if (strncmp(buffer, "Emissive", 8) == 0)
			{
				vec4_t c;
				c[0] = r.ParseFloat();
				r.FindNextChar(',');
				c[1] = r.ParseFloat();
				r.FindNextChar(',');
				c[2] = r.ParseFloat();
				r.FindNextChar(',');
				c[3] = r.ParseFloat();
				freyjaMaterialEmissive(matIndex, c);				
			}
		}
		else if (mode == 2)
		{
			// Lex('=')
			r.FindNextChar('=');

			perlinLoaded = true;

			if (strncmp(buffer, "Seed", 4) == 0)
			{
				seed = r.ParseInteger();
				SetResourceInt("ePerlinNoiseSeed", seed);
			}
			else if (strncmp(buffer, "Width", 5) == 0)
			{
				width = r.ParseInteger();
				SetResourceInt("ePerlinNoiseW", width);
			}
			else if (strncmp(buffer, "Height", 6) == 0)
			{
				height = r.ParseInteger();
				SetResourceInt("ePerlinNoiseH", height);
			}
			else if (strncmp(buffer, "Clamp", 5) == 0)
			{
				clamp = r.ParseBool();
				SetResourceInt("ePerlinNoiseClamp", clamp);
			}
			else if (strncmp(buffer, "AddColor", 8) == 0)
			{
				addcolor[0] = r.ParseFloat();
				r.FindNextChar(',');
				addcolor[1] = r.ParseFloat();
				r.FindNextChar(',');
				addcolor[2] = r.ParseFloat();
				SetResourceColor("eColorPerlinAdd", 
								 addcolor[0], addcolor[1], addcolor[2], 1.0);
			}
			else if (strncmp(buffer, "ModulateColor", 14) == 0)
			{
				modcolor[0] = r.ParseFloat();
				r.FindNextChar(',');
				modcolor[1] = r.ParseFloat();
				r.FindNextChar(',');
				modcolor[2] = r.ParseFloat();
				SetResourceColor("eColorPerlinMult", 
								 modcolor[0], modcolor[1], modcolor[2], 1.0);
			}
			else if (strncmp(buffer, "iA", 2) == 0)
			{
				iA = r.ParseFloat();
				SetResourceFloat("ePerlinNoiseIA", iA);
			}
			else if (strncmp(buffer, "iB", 2) == 0)
			{
				iB = r.ParseFloat();
				SetResourceFloat("ePerlinNoiseIB", iB);
			}
			else if (strncmp(buffer, "d", 1) == 0)
			{
				d = r.ParseFloat();
				SetResourceFloat("ePerlinNoiseD", d);
			}
		}
	}
	
	r.Close();

	if (perlinLoaded)
	{
		PerlinNoise perlin;
		byte *image = perlin.generateBuffer(width, height, seed);
		
		if (image)
		{	
			if (clamp)
				perlin.clampBufferIntensity(image, width, height, iA, iB, d);
			
			/* Modulate by a color and add a base half intensity */
			FreyjaImage img;
			byte *rgb;

			img.loadPixmap(image, width, height, FreyjaImage::INDEXED_8);
			img.getImage(&rgb);

			for (uint32 i = 0, n = width * height * 3; i < n; ++i)
			{
				/* NOTE: No clamping or scaling of colors, however there is a 
				   weakened 50 / 50 add in the sense that ADD can only contrib
				   _up_to_ 50% of full intensity ( 255 ).

				   The reason for this is to allow bleeding for plasma, etc.
				*/

				// Modulate and adjust intensity per pixel
				rgb[i] = (byte)(rgb[i] * modcolor[0]) +	(byte)(128 * addcolor[0]);
				++i;
				rgb[i] = (byte)(rgb[i] * modcolor[1]) +	(byte)(128 * addcolor[1]);
				++i;
				rgb[i] = (byte)(rgb[i] * modcolor[2]) +	(byte)(128 * addcolor[2]);
			}

			freyja_load_texture_buffer(rgb, width, height, 24);
			freyjaMaterialTexture(matIndex, mTextureId-1);
			freyjaMaterialSetFlag(matIndex, fFreyjaMaterial_Texture);
		}
	}

	freyja_refresh_material_interface();

	return true;
}


bool FreyjaControl::SaveMaterial(const char *filename)
{
	int32 matIndex = freyjaGetCurrentMaterial();

	float ambient[4];          /* Ambient color */
	float diffuse[4];          /* Diffuse color */
	float specular[4];         /* Specular color */
	float emissive[4];         /* Emissive color */
	float shininess;           /* Specular exponent */
	unsigned int texture;      /* Texture id */
	//unsigned int texture2;     /* Detail Texture id */
	unsigned int blend_src;    /* Blend source factor */
	unsigned int blend_dest;   /* Blend destination factor */
	char *name;

	freyjaGetMaterialAmbient(matIndex, ambient);
	freyjaGetMaterialDiffuse(matIndex, diffuse);
	freyjaGetMaterialSpecular(matIndex, specular);
	freyjaGetMaterialEmissive(matIndex, emissive);
	shininess = freyjaGetMaterialShininess(matIndex);
	name = (char*)freyjaGetMaterialName(matIndex);
	texture = freyjaGetMaterialTexture(matIndex);
	blend_dest = freyjaGetMaterialBlendDestination(matIndex);
	blend_src = freyjaGetMaterialBlendSource(matIndex);



	FILE *f;

	
	if (!filename || !filename[0])
	{
		return false;
	}


	f = fopen(filename, "w");

	if (!f)
	{
		perror("Material::saveFile> ");
		return false;
	}

	fprintf(f, "[Material]\n");
	fprintf(f, "Name = \"%s\"\n", name);

	if (texture && freyjaGetMaterialTextureName(matIndex))
	{
		fprintf(f, "TextureName = \"%s\"\n", freyjaGetMaterialTextureName(matIndex));
	}

	//fprintf(f, "EnableBlending=%s\n", 
	//		  (m_flags & fEnable_Blending) ? "true" : "false");

	fprintf(f, "BlendSource = %s\n", 
			  (blend_src == GL_ZERO) ? "GL_ZERO" :
			  (blend_src == GL_ONE) ? "GL_ONE" :
			  (blend_src == GL_SRC_COLOR) ? "GL_SRC_COLOR" :
			  (blend_src == GL_ONE_MINUS_SRC_COLOR) ? "GL_ONE_MINUS_SRC_COLOR" :
			  (blend_src == GL_DST_COLOR) ? "GL_DST_COLOR" :
			  (blend_src == GL_ONE_MINUS_DST_COLOR) ? "GL_ONE_MINUS_DST_COLOR" :
			  (blend_src == GL_SRC_ALPHA) ? "GL_SRC_ALPHA" :
			  (blend_src == GL_ONE_MINUS_SRC_ALPHA) ? "GL_ONE_MINUS_SRC_ALPHA" :
			  (blend_src == GL_DST_ALPHA) ? "GL_DST_ALPHA" :
			  (blend_src == GL_ONE_MINUS_DST_ALPHA) ? "GL_ONE_MINUS_DST_ALPHA" :
			  (blend_src == GL_SRC_ALPHA_SATURATE) ? "GL_SRC_ALPHA_SATURATE" :
			  (blend_src == GL_CONSTANT_COLOR) ? "GL_CONSTANT_COLOR" :
			  (blend_src == GL_ONE_MINUS_CONSTANT_COLOR) ? "GL_ONE_MINUS_CONSTANT_COLOR" :
			  (blend_src == GL_CONSTANT_ALPHA) ? "GL_CONSTANT_ALPHA" : "GL_ONE_MINUS_CONSTANT_ALPHA");


	fprintf(f, "BlendDest = %s\n", 
			  (blend_dest == GL_ZERO) ? "GL_ZERO" :
			  (blend_dest == GL_ONE) ? "GL_ONE" :
			  (blend_dest == GL_SRC_COLOR) ? "GL_SRC_COLOR" :
			  (blend_dest == GL_ONE_MINUS_SRC_COLOR) ? "GL_ONE_MINUS_SRC_COLOR" :
			  (blend_dest == GL_DST_COLOR) ? "GL_DST_COLOR" :
			  (blend_dest == GL_ONE_MINUS_DST_COLOR) ? "GL_ONE_MINUS_DST_COLOR" :
			  (blend_dest == GL_SRC_ALPHA) ? "GL_SRC_ALPHA" :
			  (blend_dest == GL_ONE_MINUS_SRC_ALPHA) ? "GL_ONE_MINUS_SRC_ALPHA" :
			  (blend_dest == GL_DST_ALPHA) ? "GL_DST_ALPHA" :
			  (blend_dest == GL_ONE_MINUS_DST_ALPHA) ? "GL_ONE_MINUS_DST_ALPHA" :
			  (blend_dest == GL_SRC_ALPHA_SATURATE) ? "GL_SRC_ALPHA_SATURATE" :
			  (blend_dest == GL_CONSTANT_COLOR) ? "GL_CONSTANT_COLOR" :
			  (blend_dest == GL_ONE_MINUS_CONSTANT_COLOR) ? "GL_ONE_MINUS_CONSTANT_COLOR" :
			  (blend_dest == GL_CONSTANT_ALPHA) ? "GL_CONSTANT_ALPHA" : "GL_ONE_MINUS_CONSTANT_ALPHA");

	fprintf(f, "Ambient = %f, %f, %f, %f\n", 
			  ambient[0], ambient[1], ambient[2], ambient[3]);
	fprintf(f, "Diffuse = %f, %f, %f, %f\n", 
			  diffuse[0], diffuse[1], diffuse[2], diffuse[3]);
	fprintf(f, "Specular = %f, %f, %f, %f\n", 
			  specular[0], specular[1], specular[2], specular[3]);
	fprintf(f, "Emissive = %f, %f, %f, %f\n", 
			  emissive[0], emissive[1], emissive[2], emissive[3]);
	fprintf(f, "Shininess = %f\n", shininess);

	// No 'named' texture dump perlin noise
	if (texture && !freyjaGetMaterialTextureName(matIndex))
	{
		int seed = GetResourceInt("ePerlinNoiseSeed");
		int w = GetResourceInt("ePerlinNoiseW");
		int h = GetResourceInt("ePerlinNoiseH");
		int clamp = GetResourceInt("ePerlinNoiseClamp");
		float ia = GetResourceFloat("ePerlinNoiseIA");
		float ib = GetResourceFloat("ePerlinNoiseIB");
		float d = GetResourceFloat("ePerlinNoiseD");
		float mr, mb, mg, ma;
		GetResourceColor("eColorPerlinMult", mr, mb, mg, ma);
		float ar, ab, ag, aa;
		GetResourceColor("eColorPerlinAdd", ar, ab, ag, aa);


		fprintf(f, "\n\n[PerlinNoise]\nSeed = %i\nWidth = %i\nHeight = %i\nClamp = %s\niA = %f\niB = %f\nd = %f\nModulateColor = %f, %f, %f\nAddColor = %f, %f, %f", seed, w, h, clamp ? "true" : "false", ia, ib, d, mr, mb, mg, ar, ab, ag);
	}


	fclose(f);

	return true;
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
		freyjaModelClampTexCoords(0); // Only support 1 model per edit atm

		unsigned int i, w, h, bpp, type, count;
		unsigned char *image = 0x0;


		count = freyjaGetTexturePoolCount();
	 
		/* Texture image was stored as raw buffer */
		for (i = 0; i < count; ++i)
		{
			freyjaGetTextureImage(i, &w, &h, &bpp, &type, &image);

			if (!image)
				continue;

			switch (type)
			{
			case RGBA_32:
				LoadTextureBuffer(image, w, h, 32, Texture::RGBA);
				break;

			case RGB_24:
				LoadTextureBuffer(image, w, h, 24, Texture::RGB);
				break;

			case INDEXED_8:
				LoadTextureBuffer(image, w, h, 8, Texture::INDEXED);
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


bool FreyjaControl::SaveModel(const char *filename)
{
	unsigned int l, s, i;
	int ret = -1;
	char ext[32];


  
	if (!filename)
		return false;

	l = strlen(filename);
  
	for (s = l; s > 0; s--)
	{
		if (filename[s] == '.')
			break;
	}

	if (!s) /* No extention given, so make it '.ja' */
	{
		char buffer[4096];
		snprintf(buffer, 4095, "%s.ja", filename);
		buffer[4095] = 0;
		ret = freyjaExportModel(buffer, "ja");
	}
	else
	{
		if (s == 0 || (l - s) > 30)
			return false;

		s++;

		memset(ext, 0, 32);

		for (i = 0; s < l; s++, i++)
		{
			ext[i] = filename[s];
		}

		ret = freyjaExportModel(filename, ext);
	}

	if (ret)
	{
		freyja_print("Unknown file export extention: '%s', try using '.ja'", 
					ext);
		return !ret;
	}
  
	String title;
	title.Set("%s - Freyja", filename);
	freyja_set_main_window_title((char*)title.GetCString());
	mCurrentlyOpenFilename = filename;
	mCleared = true;

	return true;
}


bool FreyjaControl::LoadTexture(const char *filename)
{
	FreyjaImage img;
	unsigned char *image;
	unsigned int w, h;
	int err = -1;

	// Mongoose 2002.01.10, Evil...
	if (SystemIO::File::CompareFilenameExtention(filename, ".lst") == 0)
	{
		FILE *f;
		const unsigned int bufferSize = 256;
		unsigned int i = 0;
		char buffer[bufferSize];
		char c;


		f = fopen(filename, "r");
		
		if (!f)
		{
			perror(filename);
			return false;
		}

		while (fscanf(f, "%c", &c) != EOF)
		{
			switch (c)
			{
			case ' ':
			case '\t':
			case '\n':
				break;
			case ';':
				printf("Loading texture from list '%s'\n", buffer);
				LoadTexture(buffer);
				
				i = 0;
				buffer[0] = 0;
				break;
			default:
				if (i > bufferSize-1)
					i = bufferSize-1;
				
				buffer[i++] = c;
				buffer[i] = 0;
			}
		} 
		
		fclose(f);

		return true;
	}
	
	printf("[FreyjaModel::loadTexture]\n");
	printf(" Loading texture '%s'\n", filename);

	if (!img.loadImage(filename))
	{
		img.getImage(&image);
		w = img.getWidth();
		h = img.getHeight();


		switch (img.getColorMode())
		{
		case FreyjaImage::RGBA_32:
			err = LoadTextureBuffer(image, w, h, 32, Texture::RGBA);
			break;

		case FreyjaImage::RGB_24:
			err = LoadTextureBuffer(image, w, h, 24, Texture::RGB);
			break;

		case FreyjaImage::INDEXED_8:
			err = LoadTextureBuffer(image, w, h, 8, Texture::INDEXED);
			break;

		default:
			printf("MaterialManager: Use RGB_24 and RGBA_32 images only.\n");
			
			if (image)
				delete [] image;

			if (err >= 0)
			{
				freyjaMaterialTexture(freyjaGetCurrentMaterial(), err);
				freyjaMaterialSetFlag(freyjaGetCurrentMaterial(), fFreyjaMaterial_Texture);
				freyja_print("Material[%i].texture = { %i }", freyjaGetCurrentMaterial(), err);
				freyja_refresh_material_interface();
			}

			return false;
		}
		
		delete [] image;
		
		printf("[Success]\n");

		return true;
	}
	
	return false;
}


bool FreyjaControl::LoadTextureBuffer(byte *image, uint32 width, uint32 height, 
									  uint32 bpp, Texture::ColorMode type)
{
	int err = 0;


	if (image == 0x0 || width == 0 || height == 0 || bpp == 0)
		return false;

#ifdef NO_GLU_IN_UPLOAD_SCALE_HERE 
	FreyjaImage img;

	switch (type)
	{
	case Texture::RGBA:
		img.loadPixmap(image, width, height, FreyjaImage::RGBA_32);
		img.scaleImage(1024, 1024);
		delete [] image;
		img.getImage(&image);
		width = img.getWidth();
		height = img.getHeight();		
		break;

	default:
		;
	}
#endif

	if (mFlags & fLoadTextureInSlot)
	{
		err = mTexture.loadBufferSlot(image, width, height, type, bpp,
									  mTextureId);
	}
	else
	{
		err = mTextureId = mTexture.loadBuffer(image, width, height, type, bpp);

		printf("-- %i\n", mTextureId);
	}

	if (err < 0)
	{
		printf("MaterialManager::loadTextureBuffer> ERROR Loading buffer\n");
	}

	return (err == 0);
}


bool FreyjaControl::event(int event, unsigned int value)
{
	vec_t x, y, z;


	if (ResourceEvent::listen(event - 10000 /*ePluginEventBase*/, value))
		return true;

	switch (event)
	{
	case 503:
		if (value != GetSelectedTexture())
		{
			SetSelectedTexture(value);
			freyja_event_gl_refresh();
		}
		break;

	case ePolygonSize:
		SetFaceEdgeCount(value);
		freyja_print("Polygons creation using %i sides", GetFaceEdgeCount());
		break;


	case eAnimationSlider: // FIXME: Wrapping and bounds 
		if (value != GetSelectedKeyFrame())
		{
			freyja_event_set_range(event, value, 0, freyjaGetAnimationFrameCount(GetSelectedAnimation()));
			SetSelectedKeyFrame(value);
			freyja_event_gl_refresh();
		}
		break;


	case eSelectMaterial:
		if (!freyja_event_set_range(event, value, 0, freyjaGetMaterialCount()-1))
		{
			if (value != freyjaGetCurrentMaterial())
			{
				freyjaCurrentMaterial(value);
				freyja_print("Selected material[%i].", value);
				freyja_refresh_material_interface();
				freyja_event_gl_refresh();
			}
		}
		break;


	case eBoneIterator:
		if (!freyja_event_set_range(event, value, 0, freyjaGetCount(FREYJA_BONE)))
		{
			char dupname[64];

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
				strncpy(dupname, GetBoneName(value), 64);
				mAllowBoneNameUpdate = false;
				mgtk_textentry_value_set(eSetCurrentBoneName, dupname);
				mAllowBoneNameUpdate = true;
			}
		}
		break;


	case ePolygonIterator:
		if (!freyja_event_set_range(event, value, 0, freyjaGetCount(FREYJA_POLYGON)))
		{
			SetSelectedFace(value);			
			freyja_event_gl_refresh();
			freyja_print("Selecting polygon[%i] ...", value);
		}
		break;


	case eMeshIterator:
		if (!freyja_event_set_range(event, value, 0, freyjaGetCount(FREYJA_MESH)))
		{
			SetSelectedMesh(value);			
			freyja_event_gl_refresh();
			freyja_print("Selecting mesh[%i] ...", value);
		}
		break;

	case eSetMaterialTexture:
		SetSelectedTexture(value);
		freyjaMaterialTexture(freyjaGetCurrentMaterial(), value);
		freyja_print("Material[%i].texture = %i",
					 freyjaGetCurrentMaterial(), 
					 freyjaGetMaterialTexture(freyjaGetCurrentMaterial()));
		freyja_event_gl_refresh();
		break;

	case eGenMeshCount:
		mGenMeshCount = value;
		break;

	case eGenMeshSegements:
		mGenMeshSegements = value;
		break;

	case eModeAutoKeyframe:
		if (value)
		{
			mRender->SetViewMode(VIEWMODE_MODEL_EDIT);
			freyja_event_gl_refresh();
			freyja_print("Animation Scheme");
			SetControlScheme(eScheme_Animation);
		}
		else
		{
			// If you disable auto keyframe you must set a new major mode
			mRender->SetViewMode(VIEWMODE_MODEL_EDIT);
			freyja_event_gl_refresh();
			freyja_print("Model Editor Scheme");
			SetControlScheme(eScheme_Model);
		}
		break;

	case eSelectionByBox:
		if (value)
		{
			// Radio button like func for multiple widgets on same event
			mgtk_toggle_value_set(eRotateObject, 0);
			mgtk_toggle_value_set(eScaleObject, 0);
			mgtk_toggle_value_set(eMoveObject, 0);
			mgtk_toggle_value_set(eUnselect, 0);
			mgtk_toggle_value_set(eSelect, 0);
			
			// FIXME: Add bbox selection back to this build
			mCursor.SetMode(freyja3d::Cursor::Invisible);
			mEventMode = modeSelectByBox;
			mRender->SetFlag(FreyjaRender::fBoundingVolSelection);
			freyja_print("Press right mouse to end selection");
			freyja_event_gl_refresh();
		}
		else
		{
			mRender->ClearFlag(FreyjaRender::fBoundingVolSelection);
		}
		break;

	case eSelect:
		if (value)
		{
			// Radio button like func for multiple widgets on same event
			mgtk_toggle_value_set(eRotateObject, 0);
			mgtk_toggle_value_set(eScaleObject, 0);
			mgtk_toggle_value_set(eMoveObject, 0);
			mgtk_toggle_value_set(eUnselect, 0);
			mgtk_toggle_value_set(eSelectionByBox, 0);

			mEventMode = modeSelect;
			mCursor.SetMode(freyja3d::Cursor::Invisible);
			freyja_print("Select object, hold SHIFT to unselect...");
			freyja_event_gl_refresh();
		}
		break;

	case eUnselect:
		if (value)
		{
			// Radio button like func for multiple widgets on same event
			mgtk_toggle_value_set(eRotateObject, 0);
			mgtk_toggle_value_set(eScaleObject, 0);
			mgtk_toggle_value_set(eSelect, 0);
			mgtk_toggle_value_set(eMoveObject, 0);
			mgtk_toggle_value_set(eSelectionByBox, 0);

			mEventMode = modeUnselect;
			mCursor.SetMode(freyja3d::Cursor::Invisible);
			freyja_print("Unselect object...");
			freyja_event_gl_refresh();
		}
		break;

	case eMoveObject:
		if (value)
		{
			// Radio button like func for multiple widgets on same event
			mgtk_toggle_value_set(eRotateObject, 0);
			mgtk_toggle_value_set(eScaleObject, 0);
			mgtk_toggle_value_set(eSelect, 0);
			mgtk_toggle_value_set(eUnselect, 0);
			mgtk_toggle_value_set(eSelectionByBox, 0);

			mEventMode = modeMove;
			mCursor.SetMode(freyja3d::Cursor::Translation);
			freyja_print("Move object...");
			freyja_event_gl_refresh();
		}
		break;

	case eScaleObject:
		if (value)
		{
			// Radio button like func for multiple widgets on same event
			mgtk_toggle_value_set(eRotateObject, 0);
			mgtk_toggle_value_set(eMoveObject, 0);
			mgtk_toggle_value_set(eSelect, 0);
			mgtk_toggle_value_set(eUnselect, 0);
			mgtk_toggle_value_set(eSelectionByBox, 0);

			mEventMode = modeScale;
			mCursor.SetMode(freyja3d::Cursor::Scale);
			freyja_print("Scale object...");
			freyja_event_gl_refresh();
		}
		break;

	case eRotateObject:
		if (value)
		{
			// Radio button like func for multiple widgets on same event
			mgtk_toggle_value_set(eMoveObject, 0);
			mgtk_toggle_value_set(eScaleObject, 0);
			mgtk_toggle_value_set(eSelect, 0);
			mgtk_toggle_value_set(eUnselect, 0);
			mgtk_toggle_value_set(eSelectionByBox, 0);

			mEventMode = modeRotate;
			mCursor.SetMode(freyja3d::Cursor::Rotation);
			
			switch (GetObjectMode())
			{
			case tMesh:
				{
					Mesh *m = freyjaModelGetMeshClass(0, GetSelectedMesh());
					if (m) mCursor.mPos = m->GetBoundingVolumeCenter();
					freyja_print("Rotate %s... (Won't modify data until fixed)",
								 ObjectTypeToString(GetObjectMode()).c_str());
				}
				break;

			default:
				freyja_print("Rotate %s...", 
							 ObjectTypeToString(GetObjectMode()).c_str());
			}

			freyja_event_gl_refresh();
		}
		break;

	case eViewports:
		if (value)
		{
			mRender->SetFlag(FreyjaRender::fViewports);
		}
		else
		{
			mRender->ClearFlag(FreyjaRender::fViewports);
		}
		freyja_print("Viewport rendering [%s]", 
					(GetViewFlags() & FreyjaRender::fViewports) ? "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_RENDER_BONETAG:
		if (value)
		{
			mRender->SetFlag(FreyjaRender::fBones);
		}
		else
		{
			mRender->ClearFlag(FreyjaRender::fBones);
		}

		freyja_print("Bone Rendering [%s]", 
					 (GetViewFlags() & FreyjaRender::fBones) ? "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_RENDER_POINTS:
		if (value)
		{
			mRender->SetFlag(FreyjaRender::fPoints);
		}
		else
		{
			mRender->ClearFlag(FreyjaRender::fPoints);
		}

		freyja_print("Point Rendering [%s]", 
					 (GetViewFlags() & FreyjaRender::fPoints) ? 
					 "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_RENDER_WIREFRAME:
		if (value)
		{
			mRender->SetFlag(FreyjaRender::fWireframe);
		}
		else
		{
			mRender->ClearFlag(FreyjaRender::fWireframe);
		}

		freyja_print("Wireframe Rendering [%s]", 
					 (GetViewFlags() & FreyjaRender::fWireframe) ? "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_RENDER_NORMALS:
		if (value)
		{
			mRender->SetFlag(FreyjaRender::fNormals);
		}
		else
		{
			mRender->ClearFlag(FreyjaRender::fNormals);
		}

		freyja_print("Normal Rendering [%s]", 
					 (GetViewFlags() & FreyjaRender::fNormals) ? 
					 "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_RENDER_LIGHTING:
		if (value)
		{
			mRender->SetFlag(FreyjaRender::fLighting);
		}
		else
		{
			mRender->ClearFlag(FreyjaRender::fLighting);
		}

		freyja_print("GL Lighting is [%s]", 
					 (GetViewFlags() & FreyjaRender::fLighting) ? "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_RENDER_TEXTURE:
		if (value)
		{
			mRender->SetFlag(FreyjaRender::fTexture);
		}
		else
		{
			mRender->ClearFlag(FreyjaRender::fTexture);
		}

		freyja_print("Texture rendering is [%s]", 
					 (GetViewFlags() & FreyjaRender::fTexture) ? "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_RENDER_MATERIAL:
		if (value)
		{
			mRender->SetFlag(FreyjaRender::fMaterial);
		}
		else
		{
			mRender->ClearFlag(FreyjaRender::fMaterial);
		}
		//mRender->toggleFlag(FreyjaRender::fMaterial);
		freyja_print("Material rendering is [%s]", 
					 (GetViewFlags() & FreyjaRender::fMaterial) ? 
					 "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_RENDER_GRID:
		if (value)
		{
			mRender->SetFlag(FreyjaRender::fGrid);
		}
		else
		{
			mRender->ClearFlag(FreyjaRender::fGrid);
		}
		//mRender->toggleFlag(FreyjaRender::fGrid);
		freyja_print("Edit Grid rendering [%s]",
					 (GetViewFlags() & FreyjaRender::fGrid) ? 
					 "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_RENDER_FACE:
		if (value)
		{
			mRender->SetFlag(FreyjaRender::fFace);
		}
		else
		{
			mRender->ClearFlag(FreyjaRender::fFace);
		}

		freyja_print("Face rendering is [%s]", value ? "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case eRenderPickRay:
		if (value)
		{
			mRender->SetFlag(FreyjaRender::fDrawPickRay);
		}
		else
		{
			mRender->ClearFlag(FreyjaRender::fDrawPickRay);
		}

		freyja_print("Pick ray visibility is [%s]", value ? "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case eRenderBbox:
		if (value)
		{
			// radio button like func for multiple widgets on same event
			//mgtk_toggle_value_set(eRenderBbox, 1);
			mRender->SetFlag(FreyjaRender::fBoundingVolumes);
		}
		else
		{
			//mgtk_toggle_value_set(eRenderBbox, 0);
			mRender->ClearFlag(FreyjaRender::fBoundingVolumes);
		}

		//mRender->toggleFlag(FreyjaRender::fBoundingVolumes);//FreyjaRender::fBoundingVol);
		freyja_print("Bounding volume rendering [%s]", 
					 (GetViewFlags() & FreyjaRender::fBoundingVolumes/*FreyjaRender::fBoundingVol*/) ? 
					"ON" : "OFF");
			freyja_event_gl_refresh();
		break;

	case ePointJoint:
		if (value)
		{
			// radio button like func for multiple widgets on same event
			mgtk_toggle_value_set(eSphereJoint, 0);
			mgtk_toggle_value_set(eAxisJoint, 0);
			FreyjaRender::mJointRenderType = 1;
		}
		break;

	case eSphereJoint:
		if (value)
		{
			// radio button like func for multiple widgets on same event
			mgtk_toggle_value_set(ePointJoint, 0);
			mgtk_toggle_value_set(eAxisJoint, 0);
			FreyjaRender::mJointRenderType = 2;
		}
		break;

	case eAxisJoint:
		if (value)
		{
			// radio button like func for multiple widgets on same event
			mgtk_toggle_value_set(eSphereJoint, 0);
			mgtk_toggle_value_set(ePointJoint, 0);
			FreyjaRender::mJointRenderType = 3;
		}
		break;

	default:
		freyja_print("!Unhandled { event = %d, value = %u }", event, value);
		return false;
	}

	return true;
}


bool FreyjaControl::event(int event, vec_t value)
{
	Vec3 v; 
	vec4_t color, pos;
	vec_t x, y, z;


	if (ResourceEvent::listen(event - 10000 /*ePluginEventBase*/, value))
		return true;

	switch (event)
	{
	case 510:
	case 511:
	case 512:
		freyjaGetBoneTranslation3fv(GetSelectedBone(), v.mVec);
		
		switch (event - 510)
		{
		case 0:
			v.mVec[0] = value;
			freyjaBoneTranslate3fv(GetSelectedBone(), v.mVec);
			break;

		case 1:
			v.mVec[1] = value;
			freyjaBoneTranslate3fv(GetSelectedBone(), v.mVec);
			break;

		case 2:
			v.mVec[2] = value;
			freyjaBoneTranslate3fv(GetSelectedBone(), v.mVec);
			break;
		}

		ActionModelModified(NULL);
		freyja_event_gl_refresh();
		break;


	case 520:
	case 521:
	case 522:
		GetBoneRotation(&x, &y, &z);

		switch (event - 520)
		{
		case 0:
			SetBoneRotation(value, y, z);
			break;
		case 1:
			SetBoneRotation(x, value, z);
			break;
		case 2:
			SetBoneRotation(x, y, value);
			break;
		}
		
		ActionModelModified(NULL);
		freyja_event_gl_refresh();
		break;


	case 700:
	case 701:
	case 702:
	case 703:
		freyjaGetMaterialAmbient(freyjaGetCurrentMaterial(), color);
		color[event - 700] = value;
		freyjaMaterialAmbient(freyjaGetCurrentMaterial(), color);
		freyja_event_gl_refresh();
		break;


	case 704:
	case 705:
	case 706:
	case 707:
		freyjaGetMaterialDiffuse(freyjaGetCurrentMaterial(), color);
		color[event - 704] = value;
		freyjaMaterialDiffuse(freyjaGetCurrentMaterial(), color);
		freyja_event_gl_refresh();
		break;


	case 708:
	case 709:
	case 710:
	case 711:
		freyjaGetMaterialSpecular(freyjaGetCurrentMaterial(), color);
		color[event - 708] = value;
		freyjaMaterialSpecular(freyjaGetCurrentMaterial(), color);
		freyja_event_gl_refresh();
		break;


	case 712:
	case 713:
	case 714:
	case 715:
		freyjaGetMaterialEmissive(freyjaGetCurrentMaterial(), color);
		color[event - 712] = value;
		freyjaMaterialEmissive(freyjaGetCurrentMaterial(), color);
		freyja_event_gl_refresh();
		break;


	case 716:
		freyjaMaterialShininess(freyjaGetCurrentMaterial(), value);
		freyja_event_gl_refresh();
		break;


	case eLightPosX:
	case eLightPosY:
	case eLightPosZ:
		{
			int i;

			switch (event)
			{
			case eLightPosX:
				i = 0;
				break;
			case eLightPosY:
				i = 1;
				break;
			case eLightPosZ:				
			default:
				i = 2;
			}

			freyjaGetLightPosition4v(0, pos);

			pos[i] = value;
			freyjaLightPosition4v(0, pos);
			freyja_event_gl_refresh();
		}
		break;


	/* Move/Rotate/Scale generics are used by a larger event as a vector */
	case eMoveBone_X:
	case eMoveBone_Y:
	case eMoveBone_Z:
		break;

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
		break;


	case eZoom:
		SetZoom(value);//freyja_event_get_float(eZoom));
		freyja_print("Zoom %f", mRender->GetZoom());
		freyja_event_gl_refresh();
		break;

	case eGenMeshHeight:
		mGenMeshHeight = value;
		break;

	default:
		freyja_print("!Unhandled { event = %d, value = %f }", event, value);
		return false;
	}

	return true;
}


// HACK FIXME
#include <GL/gl.h>

bool FreyjaControl::event(int command)
{
	unsigned int i, flags;


	if (ResourceEvent::listen(command - 10000 /*ePluginEventBase*/))
		return true;

	switch (command)
	{
	case 1000:
		freyjaMaterialBlendSource(freyjaGetCurrentMaterial(), GL_ZERO);
		break;
	case 1001:
		freyjaMaterialBlendSource(freyjaGetCurrentMaterial(), GL_ONE);
		break;
	case 1002:
		freyjaMaterialBlendSource(freyjaGetCurrentMaterial(), GL_SRC_COLOR);
		break;
	case 1003:
		freyjaMaterialBlendSource(freyjaGetCurrentMaterial(), GL_ONE_MINUS_SRC_COLOR);
		break;
	case 1004:
		freyjaMaterialBlendSource(freyjaGetCurrentMaterial(), GL_DST_COLOR);
		break;
	case 1005:
		freyjaMaterialBlendSource(freyjaGetCurrentMaterial(), GL_ONE_MINUS_DST_COLOR);
		break;
	case 1006:
		freyjaMaterialBlendSource(freyjaGetCurrentMaterial(), GL_SRC_ALPHA);
		break;
	case 1007:
		freyjaMaterialBlendSource(freyjaGetCurrentMaterial(), GL_ONE_MINUS_SRC_ALPHA);
		break;
	case 1008:
		freyjaMaterialBlendSource(freyjaGetCurrentMaterial(), GL_DST_ALPHA);
		break;
	case 1009:
		freyjaMaterialBlendSource(freyjaGetCurrentMaterial(), GL_ONE_MINUS_DST_ALPHA);
		break;
	case 1010:
		freyjaMaterialBlendSource(freyjaGetCurrentMaterial(), GL_SRC_ALPHA_SATURATE);
		break;
	case 1011:
		freyjaMaterialBlendSource(freyjaGetCurrentMaterial(), GL_CONSTANT_COLOR);
		break;
	case 1012:
		freyjaMaterialBlendSource(freyjaGetCurrentMaterial(), GL_ONE_MINUS_CONSTANT_COLOR);
		break;
	case 1013:
		freyjaMaterialBlendSource(freyjaGetCurrentMaterial(), GL_CONSTANT_ALPHA);
		break;
	case 1014:
		freyjaMaterialBlendSource(freyjaGetCurrentMaterial(), GL_ONE_MINUS_CONSTANT_ALPHA);
		break;


	case 2000:
		freyjaMaterialBlendDestination(freyjaGetCurrentMaterial(), GL_ZERO);
		break;
	case 2001:
		freyjaMaterialBlendDestination(freyjaGetCurrentMaterial(), GL_ONE);
		break;
	case 2002:
		freyjaMaterialBlendDestination(freyjaGetCurrentMaterial(), GL_SRC_COLOR);
		break;
	case 2003:
		freyjaMaterialBlendDestination(freyjaGetCurrentMaterial(), GL_ONE_MINUS_SRC_COLOR);
		break;
	case 2004:
		freyjaMaterialBlendDestination(freyjaGetCurrentMaterial(), GL_DST_COLOR);
		break;
	case 2005:
		freyjaMaterialBlendDestination(freyjaGetCurrentMaterial(), GL_ONE_MINUS_DST_COLOR);
		break;
	case 2006:
		freyjaMaterialBlendDestination(freyjaGetCurrentMaterial(), GL_SRC_ALPHA);
		break;
	case 2007:
		freyjaMaterialBlendDestination(freyjaGetCurrentMaterial(), GL_ONE_MINUS_SRC_ALPHA);
		break;
	case 2008:
		freyjaMaterialBlendDestination(freyjaGetCurrentMaterial(), GL_DST_ALPHA);
		break;
	case 2009:
		freyjaMaterialBlendDestination(freyjaGetCurrentMaterial(), GL_ONE_MINUS_DST_ALPHA);
		break;
	case 2010:
		freyjaMaterialBlendDestination(freyjaGetCurrentMaterial(), GL_SRC_ALPHA_SATURATE);
		break;
	case 2011:
		freyjaMaterialBlendDestination(freyjaGetCurrentMaterial(), GL_CONSTANT_COLOR);
		break;
	case 2012:
		freyjaMaterialBlendDestination(freyjaGetCurrentMaterial(), GL_ONE_MINUS_CONSTANT_COLOR);
		break;
	case 2013:
		freyjaMaterialBlendDestination(freyjaGetCurrentMaterial(), GL_CONSTANT_ALPHA);
		break;
	case 2014:
		freyjaMaterialBlendDestination(freyjaGetCurrentMaterial(), GL_ONE_MINUS_CONSTANT_ALPHA);
		break;

	case ePolygonSplit:
		freyjaPolygonSplit(GetSelectedMesh(), GetSelectedFace());
		freyja_print("Splitting polygon[%i]", GetSelectedFace());
		break;

	case eMeshFlipNormals:
		freyjaMeshNormalFlip(GetSelectedMesh());
		freyja_print("Flipping normals for mesh[%i]", GetSelectedMesh());
		break;

	case eMaterialTex:
		flags = freyjaGetMaterialFlags(freyjaGetCurrentMaterial());
		
		if (flags & fFreyjaMaterial_Texture)
			freyjaMaterialClearFlag(freyjaGetCurrentMaterial(), fFreyjaMaterial_Texture);
		else
			freyjaMaterialSetFlag(freyjaGetCurrentMaterial(), fFreyjaMaterial_Texture);
		
		freyja_print("OpenGL texturing is [%s]", 
					 !(flags & fFreyjaMaterial_Texture) ? "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case eMaterialMultiTex:
		flags = freyjaGetMaterialFlags(freyjaGetCurrentMaterial());

		if (flags & fFreyjaMaterial_DetailTexture)
			freyjaMaterialClearFlag(freyjaGetCurrentMaterial(), fFreyjaMaterial_DetailTexture);
		else
			freyjaMaterialSetFlag(freyjaGetCurrentMaterial(), fFreyjaMaterial_DetailTexture);

		freyja_print("OpenGL detail texturing is [%s]", 
					 !(flags & fFreyjaMaterial_DetailTexture) ? "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case eOpenGLNormalize:
		flags = freyjaGetMaterialFlags(freyjaGetCurrentMaterial());

		if (flags & fFreyjaMaterial_Normalize)
			freyjaMaterialClearFlag(freyjaGetCurrentMaterial(), fFreyjaMaterial_Normalize);
		else
			freyjaMaterialSetFlag(freyjaGetCurrentMaterial(), fFreyjaMaterial_Normalize);

		freyja_print("OpenGL normalization of normals is [%s]", 
					 !(flags & fFreyjaMaterial_Normalize) ? "ON" : "OFF");
		freyja_event_gl_refresh();
		break;


	case eOpenGLBlend:
		flags = freyjaGetMaterialFlags(freyjaGetCurrentMaterial());
		
		if (flags & fFreyjaMaterial_Blending)
			freyjaMaterialClearFlag(freyjaGetCurrentMaterial(), fFreyjaMaterial_Blending);
		else
			freyjaMaterialSetFlag(freyjaGetCurrentMaterial(), fFreyjaMaterial_Blending);
		
		freyja_print("OpenGL blending [%s]", 
					 !(flags & fFreyjaMaterial_Blending) ? "ON" : "OFF");
		freyja_event_gl_refresh();
		break;


	case eInfo:
		PrintInfo();
		break;

	case eHelpDialog:
		mgtk_event_dialog_visible_set(eHelpDialog, 1);
		break; 

	case ePreferencesDialog:
		extern void mgtk_event_dialog_visible_set(int dialog, int visible);
		mgtk_event_dialog_visible_set(ePreferencesDialog, 1);
		break;

	case eAboutDialog:
		extern void mgtk_event_dialog_visible_set(int dialog, int visible);
		mgtk_event_dialog_visible_set(eAboutDialog, 1);
		break;   

	case eCloseFile:
		if (freyja_create_confirm_dialog("gtk-dialog-question",
		   "You must close the currently open model to create a new model.",
		   "Would you like to close the model and lose unsaved changes?",
			"gtk-cancel", "_Cancel", "gtk-close", "C_lose"))
		{
			Clear();
			freyja_print("Closing Model...");
			freyja_set_main_window_title(BUILD_NAME);
			mCurrentlyOpenFilename = String();
			mCleared = true;
		}
		break;


	case eNewFile:
		switch (GetControlScheme())
		{
		case eScheme_Animation:
			freyja_print("!(%s:%i) %s: eScheme_Animation, Not implemented", 
						 __FILE__, __LINE__, __func__);
			break;

		case eScheme_Model:
			if (freyja_create_confirm_dialog("gtk-dialog-question",
			  "You must close the currently open model to create a new model.",
			  "Would you like to close the model and possibly lose unsaved changes?",
			  "gtk-cancel", "_Cancel", "gtk-close", "C_lose"))
			{
				Clear();
				freyja_print("Closing Model...");
				freyja_set_main_window_title(BUILD_NAME);
				mCurrentlyOpenFilename = String();
				mCleared = true;
			}
			break;

		case eScheme_Material:
			i = freyjaMaterialCreate();
			freyja_print("New material [%i] created.", i);
			break;

		default:
			;
		}
		break;


	/* File dialog operations */
	case eExportFile:
		freyja_event_file_dialog(FREYJA_MODE_SAVE_MODEL, "Export model...");
		freyja_print("Exporting is handled from Save As using file extentions...");
		break;

	case eImportFile:
		freyja_event_file_dialog(FREYJA_MODE_LOAD_MODEL, "Import model...");
		freyja_print("Importing is handled automatically from Open...");
		break;

	case eSaveFile:
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
				break;
			}

			if (mCurrentlyOpenFilename.Empty())
			{
				freyja_event_file_dialog(FREYJA_MODE_SAVE_MODEL, "Save model as...");				
			}
			else
			{
				if (freyja_create_confirm_dialog("gtk-dialog-question",
										 "You are about to overwrite a file.",
										 "Are you sure you want to overwrite the file?",
										 "gtk-cancel", "_Cancel", "gtk-ok", "_Overwrite"))
				{
					const char *s = mCurrentlyOpenFilename.GetCString();
					
					if (SaveModel(s))
						freyja_print("Model '%s' Saved", s);
					else
						freyja_print("Model '%s' failed to save", s);
				}
			}
			break;

		case eScheme_Material:
			freyja_event_file_dialog(FREYJA_MODE_SAVE_MATERIAL, "Save material as...");
			break;

		default:
			;
		}
		break;


	case eOpenFile:
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
				if (freyja_create_confirm_dialog("gtk-dialog-question",
												 "You must close the current model to open a new one.",
												 "Open the new model and lose unsaved changes?",
												 "gtk-cancel", "_Cancel", "gtk-open", "_Open"))
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
		break;

	case eSaveFileModel:
		if (mCleared) // safety
		{
			freyja_print("No changes to be saved.");
			break;
		}

		if (mCurrentlyOpenFilename.Empty())
		{
			freyja_event_file_dialog(FREYJA_MODE_SAVE_MODEL, "Save model as...");				
		}
		else
		{
			if (freyja_create_confirm_dialog("gtk-dialog-question",
											 "You are about to save a file in the EXPERIMENTAL dev build of Freyja.",
											 "Are you sure you want to save the file instead of making a copy?",
											 "gtk-cancel", "_Cancel", "gtk-ok", "_Save"))
			{
				const char *s = mCurrentlyOpenFilename.GetCString();
				if (SaveModel(s))
					freyja_print("Model '%s' Saved", s);
				else
					freyja_print("Model '%s' failed to save", s);
			}
		}
		break;

	case eOpenFileModel:
		if (!mCleared)
		{
			if (freyja_create_confirm_dialog("gtk-dialog-question",
											 "You must close the current model to open a new one.",
											 "Open the new model and lose unsaved changes?",
											 "gtk-cancel", "_Cancel", "gtk-open", "_Open"))
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


	case eRevertFile:
		if (mCurrentlyOpenFilename.Empty())
		{
			freyja_print("Revert requires a model being previously loaded");
			break;
		}

		if (freyja_create_confirm_dialog("gtk-dialog-question",
										 "You will lose all changes you made by reverting.",
										 "Would you like to revert the model and lose unsaved changes?",
										 "gtk-cancel", "_Cancel", "gtk-revert", "_Revert"))
		{
			Clear();
			freyja_print("Reverting Model...");
			freyja_set_main_window_title(BUILD_NAME);

			if (LoadModel(mCurrentlyOpenFilename.GetCString()))
				mCleared = false;
		}
		break;

	case eShutdown:
		if (mCleared ||
			freyja_create_confirm_dialog("gtk-dialog-question",
						  "If you exit you will lose any unsaved data.",
						  "Would you like to exit?", 
						  "gtk-cancel", "_Cancel", "gtk-quit", "_Exit"))
		{
			freyja_event_exit();
		}
		break;

	case eFullscreen:
		mFullScreen = !mFullScreen;
		
		if (mFullScreen)
		{
			freyja_event_fullscreen();
		}
		else
		{
			freyja_event_unfullscreen();
		}
		break;


	case eScreenShot:
		mTexture.glScreenShot("Freyja", mRender->GetWindowWidth(), mRender->GetWindowHeight());
		break;


	case eGenerateCone:
		{
			Vector3d v;

			v.zero();
			mCleared = false;
			freyjaGenerateConeMesh(v.mVec, mGenMeshHeight, mGenMeshCount);
			freyja_event_gl_refresh();
		}
		break;


	case eGenerateCylinder:
		{
			Vector3d v;

			v.zero();
			mCleared = false;
			freyjaGenerateCylinderMesh(v.mVec, mGenMeshHeight, 
									   mGenMeshCount, mGenMeshSegements);
			freyja_event_gl_refresh();
		}
		break;


	case eGenerateTube:
		{
			Vector3d v;

			v.zero();
			mCleared = false;
			freyjaGenerateTubeMesh(v.mVec, mGenMeshHeight, 
								   mGenMeshCount, mGenMeshSegements);
			freyja_event_gl_refresh();
		}
		break;


	case eGenerateSphere:
		{
			Vector3d v;

			v.zero();
			mCleared = false;
			freyjaGenerateSphereMesh(v.mVec, mGenMeshHeight, 
									 mGenMeshCount, mGenMeshCount);
			freyja_event_gl_refresh();
		}
		break;


	case eGenerateCube:
		{
			Vector3d v;

			v.zero();
			mCleared = false;
			freyjaGenerateQuadCubeMesh(v.mVec, mGenMeshHeight);
			freyja_event_gl_refresh();
		}
		break;


	case eGeneratePlane:
		{
			Vector3d v;

			v.zero();
			mCleared = false;
			freyjaGenerateQuadPlaneMesh(v.mVec, mGenMeshHeight);
			freyja_event_gl_refresh();
		}
		break;


	case eGenerateCircle:
		{
			Vector3d v;

			v.zero();
			mCleared = false;
			freyjaGenerateCircleMesh(v.mVec, mGenMeshCount);
			freyja_event_gl_refresh();
		}
		break;








	case eMirrorUV_X:
		freyjaModelMirrorTexCoord(0, GetSelectedTexCoord(), mUVMap, true, false);
		freyja_event_gl_refresh();
		break;

	case eMirrorUV_Y:
		freyjaModelMirrorTexCoord(0, GetSelectedTexCoord(), mUVMap, false, true);
		freyja_event_gl_refresh();
		break;

	case eTmpUVMapOn:
		CreatePolyMappedUVMap(GetSelectedFace());
		freyja_event_gl_refresh();
		break;

	case eTmpUVMapOff:
		CreatePolyMappedUVMap(-1);
		freyja_event_gl_refresh();
		break;

	case eTranslateUV:
		break;

	case eRotateUV:
		CreatePolyMappedUVMap(GetSelectedFace());
		//mModel->transformTexCoord(GetSelectedTexCoord(),
		//						  fRotateAboutPoint, 45, 0); 
		freyja_event_gl_refresh();
		break;

	case ePolyMapTexturePolygon:
		freyjaMeshPolygonSplitTexCoords(GetSelectedMesh(),
									GetSelectedFace());
		break;

	case eScaleUV:
		freyjaModelTransformTexCoord(0, GetSelectedTexCoord(), fScale, 0.5, 0.5);
		break;

	case eSetMeshTexture:
		freyja_print("Switching all of Mesh[%i]'s faces to material %i",
					 GetSelectedMesh(), GetSelectedTexture());
		freyjaMeshMaterial(GetSelectedMesh(),GetSelectedTexture());
		freyja_event_gl_refresh();
		break;

	case eSetFacesMaterial:
		freyja_print("Switching all of selected faces to material %i",
					 GetSelectedMesh(), GetSelectedTexture());
		SetMaterialForSelectedFaces(GetSelectedTexture());
		freyja_event_gl_refresh();
		break;

	case eSetPolygonTexture:
		freyja_print("Face to material set to %i", GetSelectedTexture());
		SetFaceMaterial(GetSelectedFace(), GetSelectedTexture());
		freyja_event_gl_refresh();
		break;

	case eTransformSelectedVertices:
		mObjectMode = tSelectedVertices;
		break;

	case eTransformScene:
		mObjectMode = tScene;
		break;

	case eTransformPoint:
		mObjectMode = tPoint;
		break;

	case eTransformFace:
		mObjectMode = tFace;
		break;

	case eTransformMesh:
		mObjectMode = tMesh;
		break;

	case eTransformModel:
		mObjectMode = tModel;
		break;

	case eTransformBone:
		mObjectMode = tBone;
		break;

	case eDelete:
		DeleteSelectedObject();
		freyja_event_gl_refresh();
		break;

	case eAddObject:
		addObject();
		mCleared = false;
		break;

	case eCopyAppendMode:
		// ATM modeler only handles 1 model, so only call for 0th index
		freyjaModelAppendMeshMode(0, !freyjaGetModelAppendMeshMode(0)); 
		break;

	case eSplitObject:
		freyja_print("Object splitting removed from this build");
		break;

	case eSelectAll:
		freyja_print("Select All is not avalible in this build");
		break;

	case eUndo:
		if (mActionManager.Undo())
		{
			freyja_event_gl_refresh();
		}
		else
		{
			freyja_print("Undo system has no more actions to revert.");
		}		
		break;

	case eRedo:
		freyja_print("Redo is not avalible in this build");
		break;

	case eCut:
		if (CopySelectedObject())
			DeleteSelectedObject();
		freyja_event_gl_refresh();
		break;

	case eCopy:
		CopySelectedObject();
		freyja_event_gl_refresh();
		break;

	case ePaste:
		PasteSelectedObject();
		freyja_event_gl_refresh();
		break;

	case eMoveObject:
		event(eScaleObject, (uint32)1);
		break;

	case eRotateObject:
		event(eScaleObject, (uint32)1);
		break;

	case eScaleObject:
		event(eScaleObject, (uint32)1);
		break;

	case eSelect:
		event(eSelect, (uint32)1);
		break;

	case eUnselect:
		event(eUnselect, (uint32)1);
		break;

	// FIXME: WTF was this for?  it's not autokey frame or set keyframe iirc
	case eKeyFrame:
		mEventMode = modeKeyframe;
		mCursor.SetMode(freyja3d::Cursor::Invisible);
		freyja_print("Keyframe object...");
		freyja_event_gl_refresh();
		break;

	/* Transform box */
	case eMove:
		mToken = true;
		Transform(mObjectMode, fTranslate,
				  freyja_event_get_float(eMove_X),
				  freyja_event_get_float(eMove_Y),
				  freyja_event_get_float(eMove_Z));

		freyja_event_set_float(eMove_X, 0.0f);
		freyja_event_set_float(eMove_Y, 0.0f);
		freyja_event_set_float(eMove_Z, 0.0f);
		freyja_event_gl_refresh();
		break;

	case eRotate:
		mToken = true;
		Transform(mObjectMode, fRotate,
				  freyja_event_get_float(eRotate_X),
				  freyja_event_get_float(eRotate_Y),
				  freyja_event_get_float(eRotate_Z));
		
		freyja_event_set_float(eRotate_X, 0.0f);
		freyja_event_set_float(eRotate_Y, 0.0f);
		freyja_event_set_float(eRotate_Z, 0.0f);
		freyja_event_gl_refresh();
		break;

	case eScale:
		mToken = true;
		Transform(mObjectMode, fScale,
				  freyja_event_get_float(eScale_X),
				  freyja_event_get_float(eScale_Y),
				  freyja_event_get_float(eScale_Z));
		
		freyja_event_set_float(eScale_X, 1.0f);
		freyja_event_set_float(eScale_Y, 1.0f);
		freyja_event_set_float(eScale_Z, 1.0f);
		freyja_event_gl_refresh();
		break;


	/* POLYGONS */
	case CMD_POLYGON_DELETE:
		mEventMode = POLYGON_DEL_MODE;
		freyja_print("Select polygon by vertices to delete...");
		break;
	case CMD_POLYGON_ADD:
		mEventMode = POLYGON_ADD_MODE;
		freyja_print("Select vertices to create a polygon...");
		break;
	case CMD_POLYGON_SELECT:
		mEventMode = POLYGON_SELECT_MODE;
		freyja_print("Select polygon by vertices with mouse...");
		break;


	/* VERTICES */
	case CMD_POINT_DELETE:
		mEventMode = POINT_DEL_MODE;
		freyja_print("Select vertex to delete...");
		break;
	case CMD_POINT_ADD:
		mCleared = false;
		mEventMode = POINT_ADD_MODE;
		freyja_print("Select point in space to create a vertex...");
		break;    
	case CMD_POINT_COMBINE:
		if (mEventMode != VERTEX_COMBINE)
		{
			mEventMode = VERTEX_COMBINE;
		}
		else
		{	
			mEventMode = modeNone;
		}

		freyja_print("Vertex combine mode is [%s]", 
					 (mEventMode == VERTEX_COMBINE) ? "on" : "off");
		break;


	/* BONES */
	case CMD_BONE_SELECT:
		mObjectMode = tBone;
		freyja_print("Select bone...");
		mEventMode = modeSelect;
		break;
	case CMD_BONE_NEW:
		mObjectMode = tBone;
		addObject();
		mCleared = false;
		//freyja_print("Select new child bone placement directly...");
		//mEventMode = BONE_ADD_MODE;
		break;
	case CMD_BONE_CONNECT:
		mObjectMode = tBone;
		freyja_print("Select a bone to connect to current bone...");
		mEventMode = BONE_CONNECT_MODE;
		break;

	case CMD_BONE_DISCONNECT:
		mObjectMode = tBone;
		freyja_print("Select bone to break from current");
		mEventMode = BONE_DISCONNECT_MODE;
		break;

	case CMD_BONE_NEXT:
		mObjectMode = tBone;
		SetSelectedBone(GetSelectedBone() + 1);
		break;

	case CMD_BONE_PREV:
		mObjectMode = tBone;
		if (GetSelectedBone())
			SetSelectedBone(GetSelectedBone() - 1);
		break;

	case eExtrude:
		{
			Vec3 v = FreyjaRender::mTestRay.mDir;
			v *= -8.0f;
			freyjaMeshPolygonExtrudeQuad1f(GetSelectedMesh(), GetSelectedFace(),
										   v.mVec);
		}
#if 0  // Currently we don't support this ( it marks all the new face vertices as selected )
		if (freyjaGetPolygonVertexCount(GetSelectedFace()))
		{
			long polygonIndex = GetSelectedFace();
			long i, v, count = freyjaGetPolygonVertexCount(polygonIndex);
			Vector<unsigned int> list = GetVertexSelectionList();

			list.clear();

			for (i = 0; i < count; ++i)
			{
				v = freyjaGetPolygonVertexIndex(polygonIndex, i);
				list.pushBack(v);
			}
		}
#endif
		freyja_event_gl_refresh();
		break;


		
	/* MESHES */
	case eMeshTesselate:
		freyjaMeshTesselateTriangles(GetSelectedMesh());
		break;
	case eMeshTexcoordSpherical:
		freyjaMeshUVMapSpherical(GetSelectedMesh());

		if (freyja_create_confirm_dialog("gtk-dialog-question",
										 "You generated vertex UV mapping, but it is not polymapped.",
										 "Would you like to promote it to ploymapped texcoords?",
										 "gtk-cancel", "_Cancel", "gtk-ok", "_Promote"))
		{
			freyjaMeshPromoteTexcoordsToPloymapping(GetSelectedMesh());
		}
		break;
	case eMeshTexcoordCylindrical:
		freyjaMeshUVMapCylindrical(GetSelectedMesh());

		if (freyja_create_confirm_dialog("gtk-dialog-question",
										 "You generated vertex UV mapping, but it is not polymapped.",
										 "Would you like to promote it to ploymapped texcoords?",
										 "gtk-cancel", "_Cancel", "gtk-ok", "_Promote"))
		{
			freyjaMeshPromoteTexcoordsToPloymapping(GetSelectedMesh());
		}
		break;
	case eMeshGenerateNormals:
		freyjaMeshGenerateVertexNormals(GetSelectedMesh());
		freyja_event_gl_refresh();
		break;
	case eMeshNew:
		mObjectMode = tMesh;
		addObject();
		mCleared = false;
		break;
	case eMeshDelete:
		mObjectMode = tMesh;
		DeleteSelectedObject();
		mCleared = false;
		break;
	case eMeshSelect:
		mObjectMode = tMesh;
		mEventMode = modeSelect;
		freyja_print("Select object mesh...");
		break;
	case eMeshMirrorX:
		//mirrorUsingVertexBuffer(true, false, false);
		break;
	case eMeshMirrorY:
		//mirrorUsingVertexBuffer(false, true, false);
		break;
	case eMeshMirrorZ:
		//mirrorUsingVertexBuffer(false, false, true);
		break;


	/* ANIMATIONS */
	case eAnimationNext:
		SetSelectedAnimation(GetSelectedAnimation() + 1);
		freyja_print("Animation[%i].", GetSelectedAnimation());
		break;

	case eAnimationPrev:
		if (GetSelectedAnimation())
			SetSelectedAnimation(GetSelectedAnimation() - 1);
		else
			SetSelectedAnimation(0);

		freyja_print("Animation[%i].", GetSelectedAnimation());
		break;

	case eSetKeyFrame:
		SetKeyFrame();
		break;

	case eZoom:
		SetZoom(freyja_event_get_float(eZoom));
		freyja_event_gl_refresh();
		break;

	case eTexcoordCombine:
		if (mEventMode != TEXEL_COMBINE)
		{
			mEventMode = TEXEL_COMBINE;
		}
		else
		{	
			mEventMode = modeNone;
		}

		freyja_print("Texel combine [%s]", 
					(mEventMode == TEXEL_COMBINE) ? "on" : "off");
		break;



	case CMD_MISC_GEN_TEXMAP_XY:
		freyjaMeshUVMapPlanar(GetSelectedMesh());
		break;



	case CMD_MISC_ZOOM_IN:
		if (mRender->GetZoom() <= 0.02)
		{
			mRender->SetZoom(mRender->GetZoom() + 0.0001);
		}
		else
		{
			mRender->SetZoom(mRender->GetZoom() + 0.01);
		}

		freyja_event_gl_refresh();
		break;

	case CMD_MISC_ZOOM_OUT:
		if (mRender->GetZoom() <= 0.02)
		{
			mRender->SetZoom(mRender->GetZoom() - 0.0001);
		}
		else
		{
			mRender->SetZoom(mRender->GetZoom() - 0.01);
		}

		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_TEXTURE_EDIT:
		mRender->SetViewMode(VIEWMODE_TEXTURE_EDIT);
		freyja_event_gl_refresh();
		freyja_print("UV Editor Scheme");
		SetControlScheme(eScheme_UV);
		break;

	case FREYJA_MODE_MODEL_EDIT:
		// Radio button like func for multiple widgets on same event
		mgtk_toggle_value_set(eModeAutoKeyframe, 0);

		mRender->SetViewMode(VIEWMODE_MODEL_EDIT);
		freyja_event_gl_refresh();
		freyja_print("Model Editor Scheme");
		SetControlScheme(eScheme_Model);
		break;

	case FREYJA_MODE_MATERIAL_EDIT:
		mRender->SetViewMode(VIEWMODE_MATERIAL_EDIT);
		freyja_event_gl_refresh();
		freyja_print("Material Editor Scheme");
		SetControlScheme(eScheme_Material);
		break;

	case eViewportBack:
		freyja_print("Back view");
		SetSelectedView(PLANE_BACK);
		if (mRender->GetFlags() & FreyjaRender::fViewports)
			mRender->mViewports[mSelectedViewport].plane = PLANE_BACK;
		freyja_event_gl_refresh();
		break;

	case eViewportBottom:
		freyja_print("Bottom view");  
		SetSelectedView(PLANE_BOTTOM);
		if (mRender->GetFlags() & FreyjaRender::fViewports)
			mRender->mViewports[mSelectedViewport].plane = PLANE_BOTTOM;
		freyja_event_gl_refresh();
		break;

	case eViewportRight:
		freyja_print("Right view");
		SetSelectedView(PLANE_RIGHT);
		if (mRender->GetFlags() & FreyjaRender::fViewports)
			mRender->mViewports[mSelectedViewport].plane = PLANE_RIGHT;
		freyja_event_gl_refresh();
		break;

	case eViewportFront:
		freyja_print("Front view");
		SetSelectedView(PLANE_FRONT);
		if (mRender->GetFlags() & FreyjaRender::fViewports)
			mRender->mViewports[mSelectedViewport].plane = PLANE_FRONT;
		freyja_event_gl_refresh();
		break;

	case eViewportTop:
		freyja_print("Top view");  
		SetSelectedView(PLANE_TOP);
		if (mRender->GetFlags() & FreyjaRender::fViewports)
			mRender->mViewports[mSelectedViewport].plane = PLANE_TOP;
		freyja_event_gl_refresh();
		break;

	case eViewportLeft:
		freyja_print("Left view");
		SetSelectedView(PLANE_LEFT);
		if (mRender->GetFlags() & FreyjaRender::fViewports)
			mRender->mViewports[mSelectedViewport].plane = PLANE_LEFT;
		freyja_event_gl_refresh();
		break;

	case eViewportOrbit:
		freyja_print("Orbital view");
		SetSelectedView(PLANE_FREE);
		if (mRender->GetFlags() & FreyjaRender::fViewports)
			mRender->mViewports[mSelectedViewport].plane = PLANE_FREE;
		freyja_event_gl_refresh();
		break;

	case eViewportUV:
		freyja_print("UV Editor view");
		SetSelectedView(DRAW_UV);
		if (mRender->GetFlags() & FreyjaRender::fViewports)
			mRender->mViewports[mSelectedViewport].plane = DRAW_UV;
		freyja_event_gl_refresh();
		break;

	case eViewportCurve:
		freyja_print("Curve editor view");
		SetSelectedView(DRAW_CURVE);
		if (mRender->GetFlags() & FreyjaRender::fViewports)
			mRender->mViewports[mSelectedViewport].plane = DRAW_CURVE;
		freyja_event_gl_refresh();
		break;

	case eViewportMaterial:
		freyja_print("Material preview");
		SetSelectedView(DRAW_MATERIAL);
		if (mRender->GetFlags() & FreyjaRender::fViewports)
			mRender->mViewports[mSelectedViewport].plane = DRAW_MATERIAL;
		freyja_event_gl_refresh();
		break;

	default:
		freyja_print("!Unhandled event(%d)", command);
		return false;
	}   

	return true;
}


bool FreyjaControl::handleEvent(int mode, int cmd)
{
	bool handled = true;
	//freyja_print("! Call to handleEvent is deprecated...\n");

	switch (mode)
	{
	case eSelect:
		mEventMode = modeSelect;
		mCursor.SetMode(freyja3d::Cursor::Invisible);
		freyja_print("! (mode) Select object...");
		freyja_event_gl_refresh();
		break;

	case eUnselect:
		mEventMode = modeUnselect;
		mCursor.SetMode(freyja3d::Cursor::Invisible);
		freyja_print("! (mode) Unselect object...");
		freyja_event_gl_refresh();
		break;

	default:
		handled = event(cmd);
	}

	return handled;
}


void FreyjaControl::handleTextEvent(int event, const char *text)
{
	bool empty = (text == NULL || text[0] == 0);


	switch (event)
	{
	case eOpenTexture:
		{
			if (empty) return;

			bool loaded = LoadTexture(text);
			if (loaded)
			{
				uint32 e = resourceGetEventId1s("eSetTextureNameA");
				uint32 texture = mTextureId - 1;
				uint32 mat = freyjaGetCurrentMaterial();

				mgtk_textentry_value_set(e, text);
				freyjaMaterialSetFlag(mat, fFreyjaMaterial_Texture);
				mgtk_spinbutton_value_set(eSetMaterialTexture, texture);
				freyjaMaterialTexture(mat, texture);
				
				freyja_event_gl_refresh();
			}

			freyja_print("%s %s", text, loaded ? "loaded" : "failed to load");
		}
		break;

	case eOpenTextureB:
		{
			if (empty) return;

			if (LoadTexture(text))
			{
				uint32 e = resourceGetEventId1s("eSetTextureNameB");
				//uint32 texture = mTextureId - 1;
				uint32 mat = freyjaGetCurrentMaterial();

				mgtk_textentry_value_set(e, text);
				freyjaMaterialSetFlag(mat, fFreyjaMaterial_Texture);
				DEBUG_MSG("Not implemented in backend yet");
				//mgtk_spinbutton_value_set(eSetMaterialTexture, texture);
				//freyjaMaterialTexture(mat, texture);
				//
				freyja_event_gl_refresh();
			}
		}
		break;

	case eSetMaterialName:
		freyjaMaterialName(freyjaGetCurrentMaterial(), text);
		break;

	case eSetTextureNameA:
		freyjaMaterialTextureName(freyjaGetCurrentMaterial(), text);
		break;

	case eSetTextureNameB:
		// FIXME: Need to make N level texture support in backend first
		/* Text here is assumed to be a filename */
		//mMaterial->loadDetailTexture(text);
		break;

	case eSetCurrentBoneName:
		if (mAllowBoneNameUpdate)
			SetBoneName(GetSelectedBone(), text);
		break;

	default:
		freyja_print("Unhandled event(%i) '%s'", event, text);
	}
}


void FreyjaControl::PrintInfo()
{
	const long bufSz = 511;
	char buf[bufSz+1];

	snprintf(buf, bufSz, 
			 "Current Model Properties\n Bones    \t%d\n Meshes  \t%d\n Polygons \t%d\n Vertices  \t%d\n",
			 freyjaGetCount(FREYJA_BONE), 
			 freyjaGetCount(FREYJA_MESH), 
			 freyjaGetCount(FREYJA_POLYGON), 
			 freyjaGetCount(FREYJA_VERTEX));	
	buf[bufSz] = 0;

	freyja_event_info_dialog("gtk-dialog-info", buf);
}


bool FreyjaControl::motionEvent(int x, int y)
{
	switch (GetControlScheme())
	{
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


		case MOUSE_BTN_MIDDLE:
			{
				float s;
				float t;
				
				s = (float)x / (float)mRender->GetWindowWidth();
				t = (float)y / (float)mRender->GetWindowHeight();

				if (s > 1.0) s = 1.0;
				if (s < 0.0) s = 0.0;
				if (t > 1.0) t = 1.0;
				if (t < 0.0) t = 0.0;
				
				//UVMapMotion(s, t);
				return true;
			}
			break;


		case MOUSE_BTN_LEFT:
			if (mUVMouseState)
			{
				float s;
				float t;
				
				s = (float)x / (float)mRender->GetWindowWidth();
				t = (float)y / (float)mRender->GetWindowHeight();
				
				if (s > 1.0) s = 1.0;
				if (s < 0.0) s = 0.0;
				if (t > 1.0) t = 1.0;
				if (t < 0.0) t = 0.0;
				
				//TexelMove(s, t);
				return true;
			}
			break;
		}
		break;


	case eScheme_Model:
		MotionEdit(x, y, GetSelectedView());
		break;

	default:
		;
	}
	
	//	freyja_print("motion -> %i, %i", x, y);

	return true;
}


void FreyjaControl::SetKeyFrame()
{
	switch (mObjectMode)
	{
	case tMesh:
		{
			index_t id = freyjaKeyFrameCreate(mSelectedKeyFrame);
			freyja_print("Created keyframe[%i] <- %i", mSelectedKeyFrame, id);
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
			Ray &r = FreyjaRender::mTestRay;
			mCursor.mAxis = freyja3d::Cursor::eNone;

			Vec3 o = r.GetOrigin();
			Vec3 d = r.GetDir();
			Vec3 v = o + d;
			Vec3 u = o + d*2.0f;
			Vec3 n = Vector3d::cross(v, u);


			freyja_print("!------------");
			freyja_print("!o <%f %f %f>", o.mVec[0], o.mVec[1], o.mVec[2]);
			freyja_print("!d <%f %f %f>", d.mVec[0], d.mVec[1], d.mVec[2]);

			freyja_print("!v <%f %f %f>", v.mVec[0], v.mVec[1], v.mVec[2]);
			freyja_print("!u <%f %f %f>", u.mVec[0], u.mVec[1], u.mVec[2]);
			freyja_print("!n <%f %f %f>", n.mVec[0], n.mVec[1], n.mVec[2]);
			n.normalize();
			freyja_print("!n <%f %f %f>", n.mVec[0], n.mVec[1], n.mVec[2]);

			// Now you have a normal to the ray
			// Generate a bbox to test with the transformed line segments
			// composing the circles in the rotation cursor
			Vec3 min, max; // FIXME generate prism by these points

			min = o + n;
			max = o + d - n;

			// Pick the closest 'hit' from each 'axis' ring
			vec_t dist, best;
			bool fhit = false, hit;

			// X
			for (int i = 0, n = 64; i < n; ++i)
			{
				Ray r; // FIXME generate a ray from each segment
				hit = r.IntersectBox(min.mVec, max.mVec, dist);
				if (hit && !fhit || dist < best)
				{
					fhit = true;
					mCursor.mAxis = freyja3d::Cursor::eX;
					best = dist;
				}
			}

			// Y
			for (int i = 0, n = 64; i < n; ++i)
			{
				Ray r; // FIXME generate a ray from each segment
				hit = r.IntersectBox(min.mVec, max.mVec, dist);
				if (hit && !fhit || dist < best)
				{
					mCursor.mAxis = freyja3d::Cursor::eY;
					best = dist;
				}
			}

			// Z
			for (int i = 0, n = 64; i < n; ++i)
			{
				Ray r; // FIXME generate a ray from each segment
				hit = r.IntersectBox(min.mVec, max.mVec, dist);
				if (hit && !fhit || dist < best)
				{
					mCursor.mAxis = freyja3d::Cursor::eZ;
					best = dist;
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
			Ray &r = FreyjaRender::mTestRay;
			//bool picked = false;
			Vec3 o;
			vec_t t;

			mCursor.mAxis = freyja3d::Cursor::eNone;
			mCursor.mLastPos = mCursor.mPos;

			switch (GetSelectedView())
			{
			case PLANE_BACK:
				o = Vec3(1.0f*freyja3d::Cursor::mid,0,0) + mCursor.mPos;

				if (r.IntersectSphere(o.mVec, freyja3d::Cursor::min*2, t))
				{
					DEBUG_MSGF("Sphere hit!");
					mCursor.mAxis = freyja3d::Cursor::eX;
					mCursor.mSelected = true;
					freyja_print("! Cursor ray picked X");
					mToken = true;
					return;
				}

				o = Vec3(0, freyja3d::Cursor::mid,0) + mCursor.mPos;

				if (r.IntersectSphere(o.mVec, freyja3d::Cursor::min*2, t))
				{
					DEBUG_MSGF("Sphere hit!");
					mCursor.mAxis = freyja3d::Cursor::eY;
					mCursor.mSelected = true;
					freyja_print("! Cursor ray picked Y");
					mToken = true;
					return;
				}

				o = Vec3(0, 0, freyja3d::Cursor::mid) + mCursor.mPos;
				
				if (r.IntersectSphere(o.mVec, freyja3d::Cursor::min*2, t))
				{
					DEBUG_MSGF("Sphere hit!");
					mCursor.mAxis = freyja3d::Cursor::eAll;
					mCursor.mSelected = true;
					freyja_print("! Cursor ray picked All");
					mToken = true;
					return;
				}
				break;

			case PLANE_FRONT: // Front, XY
				o = Vec3(freyja3d::Cursor::mid,0,0) + mCursor.mPos;

				if (r.IntersectSphere(o.mVec, freyja3d::Cursor::min*2, t))
				{
					DEBUG_MSGF("Sphere hit!");
					mCursor.mAxis = freyja3d::Cursor::eX;
					mCursor.mSelected = true;
					freyja_print("! Cursor ray picked X");
					mToken = true;
					return;
				}

				o = Vec3(0, freyja3d::Cursor::mid,0) + mCursor.mPos;

				if (r.IntersectSphere(o.mVec, freyja3d::Cursor::min*2, t))
				{
					DEBUG_MSGF("Sphere hit!");
					mCursor.mAxis = freyja3d::Cursor::eY;
					mCursor.mSelected = true;
					freyja_print("! Cursor ray picked Y");
					mToken = true;
					return;
				}

				o = Vec3(0, 0, freyja3d::Cursor::mid) + mCursor.mPos;
				
				if (r.IntersectSphere(o.mVec, freyja3d::Cursor::min*2, t))
				{
					DEBUG_MSGF("Sphere hit!");
					mCursor.mAxis = freyja3d::Cursor::eAll;
						mCursor.mSelected = true;
						freyja_print("! Cursor ray picked All");
						mToken = true;
						return;
				}
				break;

			case PLANE_TOP: // XZ
				o = Vec3(freyja3d::Cursor::mid,0,0) + mCursor.mPos;

				if (r.IntersectSphere(o.mVec, freyja3d::Cursor::min*2, t))
				{
					DEBUG_MSGF("Sphere hit!");
					mCursor.mAxis = freyja3d::Cursor::eX;
					mCursor.mSelected = true;
					mToken = true;
					freyja_print("! Cursor ray picked X");
					return;
				}

				o = Vec3(0, 0, freyja3d::Cursor::mid) + mCursor.mPos;

				if (r.IntersectSphere(o.mVec, freyja3d::Cursor::min*2, t))
				{
					DEBUG_MSGF("Sphere hit!");
					mCursor.mAxis = freyja3d::Cursor::eZ;
					mCursor.mSelected = true;
					freyja_print("! Cursor ray picked Z");
					mToken = true;
					return;
				}

				o = Vec3(0, freyja3d::Cursor::mid, 0) + mCursor.mPos;
				
				if (r.IntersectSphere(o.mVec, freyja3d::Cursor::min*2, t))
				{
					DEBUG_MSGF("Sphere hit!");
					mCursor.mAxis = freyja3d::Cursor::eAll;
					mCursor.mSelected = true;
					freyja_print("! Cursor ray picked All");
					mToken = true;
					return;
				}
				break;


			case PLANE_BOTTOM:
				o = Vec3(freyja3d::Cursor::mid,0,0) + mCursor.mPos;

				if (r.IntersectSphere(o.mVec, freyja3d::Cursor::min*2, t))
				{
					DEBUG_MSGF("Sphere hit!");
					mCursor.mAxis = freyja3d::Cursor::eX;
					mCursor.mSelected = true;
					mToken = true;
					freyja_print("! Cursor ray picked X");
					return;
				}

				o = Vec3(0, 0, freyja3d::Cursor::mid) + mCursor.mPos;

				if (r.IntersectSphere(o.mVec, freyja3d::Cursor::min*2, t))
				{
					DEBUG_MSGF("Sphere hit!");
					mCursor.mAxis = freyja3d::Cursor::eZ;
					mCursor.mSelected = true;
					freyja_print("! Cursor ray picked Z");
					mToken = true;
					return;
				}

				o = Vec3(0, freyja3d::Cursor::mid, 0) + mCursor.mPos;
				
				if (r.IntersectSphere(o.mVec, freyja3d::Cursor::min*2, t))
				{
					DEBUG_MSGF("Sphere hit!");
					mCursor.mAxis = freyja3d::Cursor::eAll;
					mCursor.mSelected = true;
					freyja_print("! Cursor ray picked All");
					mToken = true;
					return;
				}
				break;

			case PLANE_RIGHT:
				o = Vec3(0,0,freyja3d::Cursor::mid) + mCursor.mPos;

				if (r.IntersectSphere(o.mVec, freyja3d::Cursor::min*2, t))
				{
					DEBUG_MSGF("Sphere hit!");
					mCursor.mAxis = freyja3d::Cursor::eZ;
					mCursor.mSelected = true;
					freyja_print("! Cursor ray picked Z");
					mToken = true;
					return;
				}

				o = Vec3(0, freyja3d::Cursor::mid,0) + mCursor.mPos;

				if (r.IntersectSphere(o.mVec, freyja3d::Cursor::min*2, t))
				{
					DEBUG_MSGF("Sphere hit!");
					mCursor.mAxis = freyja3d::Cursor::eY;
					mCursor.mSelected = true;
					freyja_print("! Cursor ray picked Y");
					mToken = true;
					return;
				}

				o = Vec3(freyja3d::Cursor::mid,0,0) + mCursor.mPos;
				
				if (r.IntersectSphere(o.mVec, freyja3d::Cursor::min*2, t))
				{
					DEBUG_MSGF("Sphere hit!");
					mCursor.mAxis = freyja3d::Cursor::eAll;
					mCursor.mSelected = true;
					freyja_print("! Cursor ray picked All");
					mToken = true;
					return;
				}
				break;

			case PLANE_LEFT: // ZY 
				o = Vec3(0,0,freyja3d::Cursor::mid) + mCursor.mPos;

				if (r.IntersectSphere(o.mVec, freyja3d::Cursor::min*2, t))
				{
					DEBUG_MSGF("Sphere hit!");
					mCursor.mAxis = freyja3d::Cursor::eZ;
					mCursor.mSelected = true;
					freyja_print("! Cursor ray picked Z");
					mToken = true;
					return;
				}

				o = Vec3(0, freyja3d::Cursor::mid,0) + mCursor.mPos;

				if (r.IntersectSphere(o.mVec, freyja3d::Cursor::min*2, t))
				{
					DEBUG_MSGF("Sphere hit!");
					mCursor.mAxis = freyja3d::Cursor::eY;
					mCursor.mSelected = true;
					freyja_print("! Cursor ray picked Y");
					mToken = true;
					return;
				}

				o = Vec3(freyja3d::Cursor::mid,0,0) + mCursor.mPos;
				
				if (r.IntersectSphere(o.mVec, freyja3d::Cursor::min*2, t))
				{
					DEBUG_MSGF("Sphere hit!");
					mCursor.mAxis = freyja3d::Cursor::eAll;
					mCursor.mSelected = true;
					freyja_print("! Cursor ray picked All");
					mToken = true;
					return;
				}
				break;

			default:
				;
			}
		}
	}
}


bool FreyjaControl::MouseEdit(int btn, int state, int mod, int x, int y)
{
	bool ret = false;
	bool swappedModes = false;
	vec_t vx = x, vy = y;
	EventMode mode = mEventMode;
	mMouseButton = btn;
	mMouseState = state;
	mModKey = mod;


	if (!btn && !state && mToken && GetControlScheme() == eScheme_Model)
	{
		switch (GetObjectMode())
		{
		case tModel:
			{
				Vec3 v = GetCursorData(GetEventAction());

				freyja_print("! Model[%i] %s %f %f %f",
							 0, 
							 ActionToString(GetEventAction()).c_str(),
							 GetEventAction(),
							 v.mVec[0], v.mVec[1], v.mVec[2]);

				// Mongoose - Does transform, undo, etc for ya, bub
				Transform(GetObjectMode(), GetEventAction(),
						  v.mVec[0], v.mVec[1], v.mVec[2]);

				// Reset transforms in the cursor
				mCursor.mRotate = Vec3(0,0,0);
				mCursor.mScale = Vec3(1,1,1);
				//mCursor.mPos = Vec3(0,0,0);
			}			
			break;

		case tMesh:
			{
				Vec3 v = GetCursorData(GetEventAction());

				// Mongoose - Does transform, undo, etc for ya, bub
				Transform(GetObjectMode(), GetEventAction(),
						  v.mVec[0], v.mVec[1], v.mVec[2]);

				// Reset transforms in the cursor
				mCursor.mRotate = Vec3(0,0,0);
				mCursor.mScale = Vec3(1,1,1);
				//mCursor.mPos = Vec3(0,0,0);
			}
			break;


		default:
			;
		}
	}


	/* Handle mode */
	/* FIXME: These should be seperate 'windows' with their own 
	 * MouseEdit() methods or the like later */
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
			
			
			if (mEventMode == TEXEL_COMBINE)
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
		if (mod & KEY_LCTRL)
		{
			handleEvent(eEvent, eSelect);
			swappedModes = true;
		}
		else if (mod & KEY_LSHIFT)
		{
			handleEvent(eEvent, eRotate);
			swappedModes = true;
		}


		switch (mEventMode)
		{
		case modeMove:
			mXYZMouseState = 1;
			//VertexSelect(xx, yy);
			break;

		case VERTEX_COMBINE:
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

		case modeSelectByBox:
			if (mXYZMouseState == 0)
			{
				freyja_print("! FIXME: Selection by box was removed after 0.9.3!");
				mXYZMouseState = 1;
			}
			else
			{
				mXYZMouseState = 0;
			}
			break;

		case BONE_CONNECT_MODE:
			MARK_MSGF("FIXME");
			//master_tag = GetSelectedBone();
			//selectBone(xx, yy);
			//connectBone(master_tag, GetSelectedBone());
			//SetSelectedBone(master_tag);
			break;
		case BONE_DISCONNECT_MODE:
			MARK_MSGF("FIXME");
			//master_tag = GetSelectedBone();
			//selectBone(xx, yy);
			//removeMeshFromBone(master_tag, GetSelectedBone());
			//SetSelectedBone(master_tag);
			break;
		case POINT_ADD_MODE: 
			//VertexNew(xx, yy);
			break;
		case BONE_ADD_MODE:
			//i = newBone(xyz[0], xyz[1], xyz[2], 0x0);
			//freyja_print("New bone[%i] created", i);
			break;
		case POLYGON_ADD_MODE:
			//PolygonAddVertex(xx, yy);
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
	if (mod & KEY_LCTRL)
	{
		handleEvent(eEvent, eSelect);
	}
	else if (mod & KEY_LSHIFT)
	{
		handleEvent(eEvent, eUnselect);
	}

	/* Handle left clicks */
	if (btn == MOUSE_BTN_LEFT && state == MOUSE_BTN_STATE_PRESSED)
	{
		ret = true;
		switch (mEventMode)
		{
		case modeSelect:
			SelectObject(vx, vy);
			break;

		case modeUnselect:
			UnselectObject(vx, vy);
			break;

		default:
			SelectCursorAxis(vx, vy);
			ret = false;
		}
	}

	mEventMode = mode;

	return ret;
}


bool FreyjaControl::mouseEvent(int btn, int state, int mod, int x, int y)
{
	//freyja_print("!.. %i %i", btn, state);

	// We've just let go of the button that was being held down
	if (mCursor.mSelected && !btn && !state) 
	{
		mCursor.mSelected = false;
		freyja_print("! Cursor was released.");
	}

	// New mouse Edit event handler
	if (MouseEdit(btn, state, mod, x, y)) 
		return true;

	EventMode mode = mEventMode;
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
			
			if (mEventMode == TEXEL_COMBINE)
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

	case eScheme_Model:
		// Mongoose 2002.01.12, Allow temp mode override
		if (mod & KEY_LCTRL)
		{
			handleEvent(eEvent, eSelect);
		}
		else if (mod & KEY_LSHIFT)
		{
			handleEvent(eEvent, eRotate);
		}

		MouseEdit(btn, state, mod, x, y);
		break;
	default:
		;
	}

	// Mongoose 2002.01.12, Reset mode
	mEventMode = mode;
	return true;
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////

void FreyjaControl::getScreenToWorldOBSOLETE(vec_t &x, vec_t &y)
{
	vec_t z;

	//ASSERT_MSG(false, "Obsolete function call.");

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
								double *rayOrigin, double *rayVector)
{
	/* This does not use viewport: quadrant checks, widths, and heights */
	extern double gMatrix[16]; // This is actual cached render transform
	double inverse[16];
	double rayPnt[4] = {0.0f, 0.0f, 0.0f, 1.0f };
	double rayVec[4];
	vec_t winH = mRender->GetWindowHeight();
	vec_t winY = (winH - mouseY) - winH * 0.5f;
	vec_t normY = winY / ( winH * 0.5f );
	vec_t winX = mouseX - winH * 0.5f;
	vec_t normX = winX / ( winH * 0.5f );
	vec_t zNear = -400.0f; // mRender->getZNear();
	vec_t nearH = 20.0f;//mRender->getNearHeight();
	vec_t aspect = mRender->GetWindowAspectRatio();

	// This is now ray in eye coordinates
	rayVec[0] = nearH * normY;
	rayVec[1] = nearH * aspect * normX;
	rayVec[2] = -zNear;
	rayVec[3] = 0.0f;

	// Now transform by inverse of modelview matrix to object coordinates
	matrixInvert16dv(gMatrix, inverse);
	matrix4dvx16dv(inverse, rayVec, rayVector); // src, dest can be same
	matrix4dvx16dv(inverse, rayPnt, rayOrigin); // src, dest can be same

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
	Vec3 scroll(GetSceneTranslation());
	vec_t nearHeight = mRender->getNearHeight() * 2.0f;
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

bool FreyjaControl::CopySelectedObject()
{
	switch (mObjectMode)
	{
	case tSelectedVertices:
		//return CopyVertexBuffer();
		break;

	case tMesh:
		//return CopySelectedMesh();
		break;

	default:
		freyja_print("This object type is not supported by Generic Copy.");
		return false;
	}

	return true;
}


bool FreyjaControl::PasteSelectedObject()
{
	switch (mObjectMode)
	{
	case tSelectedVertices:
		//return PasteSelectedMesh(); // vertexbuffercopy shares buf
		break;

	case tMesh:
		//return pasteSelectedMesh();
		break;

	default:
		freyja_print("This object type '%s' is not supported by Generic Paste.", ObjectTypeToString(mObjectMode).GetCString());
		return false;
	}

	return true;
}


void FreyjaControl::DeleteSelectedObject()
{
	switch (mObjectMode)
	{
	case tPoint:
		mEventMode = POINT_DEL_MODE;
		freyja_print("Click select to delete -- this will delete all selected later");
		break;

	case tFace:
		freyjaMeshPolygonDelete(GetSelectedMesh(), GetSelectedFace());
		break;

	case tSelectedVertices:
		if (freyja_create_confirm_dialog("gtk-dialog-question",
										 "You are about to delete the selected vertices.",
										 "Are you sure you want to delete these vertices?",
										 "gtk-cancel", "_Cancel", "gtk-ok", "_Delete"))
		{
			BUG_ME("FIXME");//CullUsingVertexBuffer();
		}
		break;

	case tBone:
		freyjaBoneDelete(GetSelectedBone());
		break;

	case tMesh:
		if (freyjaModelGetMeshClass(0, GetSelectedMesh()))
		{
			mToken = true;
			ActionModelModified(new ActionMeshDelete(GetSelectedMesh()));
		}
		freyjaMeshDelete(GetSelectedMesh());
		break;

	default:
		freyja_print("%s Object type '%s' is not supported.", __func__, ObjectTypeToString(mObjectMode).GetCString());
	}
}


void FreyjaControl::SetMaterialForSelectedFaces(uint32 material)
{
	Mesh *m = freyjaModelGetMeshClass(0, GetSelectedMesh());


	if (!m)
		return;

	// Render wireframe faces	
	for (uint32 i = 0, n = m->GetFaceCount(); i < n; ++i)
	{
		Face *f = m->GetFace(i);

		if (f && f->mFlags & Face::fSelected)
		{
			freyjaPolygonSetMaterial1i(i, material);
		}
	}   
}


void FreyjaControl::addObject()
{
	int index = -1, root;


	switch (mObjectMode)
	{
	case tPoint:
		mEventMode = POINT_ADD_MODE;
		break;

	case tBone:
		if (!freyjaGetCount(FREYJA_SKELETON))
		{
			SetSelectedSkeleton(freyjaSkeletonCreate());
		}

		root = GetSelectedBone(); // really parent
		index = freyjaBoneCreate(GetSelectedSkeleton());

		if (index == 0)
		{
			freyjaSkeletonRootIndex(GetSelectedSkeleton(), index);
		}

		if (index > 0 && index != root)
		{
			freyjaBoneParent(index, root);
		}

		if (index > -1)
		{
			String s;
			int skel = GetSelectedSkeleton();

			freyjaBoneTranslate3f(index, 0.0f, 0.0f, 0.0f);
			freyjaBoneRotateEuler3f(index, 0.0f, 0.0f, 0.0f);
			freyjaBoneFlags(index, 0x0);
			s.Set("bone_%i", index);
			freyjaBoneName(index, s.GetCString());
			freyjaSkeletonAddBone(skel, index);
			freyjaSkeletonUpdateBones(skel);
			UpdateSkeletonUI_Callback(skel);

			SetSelectedBone(index);

			freyja_event_gl_refresh();
			freyja_print("New Skel[%u].Bone[%u], parent = %i",
						 skel, index, root);
		}


		ActionModelModified(NULL);
		break;

	case tMesh:
		//MeshNew();
		ActionModelModified(NULL);
		break;
	default:
		freyja_print("%s Object type '%s' is not supported.", __func__, ObjectTypeToString(mObjectMode).GetCString());
	}
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
			Mesh *m = freyjaModelGetMeshClass(0, GetSelectedMesh());

			if ( m )
			{
				int selected = -1;
				m->IntersectClosestVertex(FreyjaRender::mTestRay, selected, 2.0f);
				
				if (selected > -1)
				{
					freyja_print("! Vertex[%i] unselected by pick ray.", selected);
					m->ClearVertexFlags(selected, Vertex::fSelected);
					
				}
			}
		}
		break;

	case tFace:
		{
			// New backend ( picks faces and marks them selected )
			Mesh *m = freyjaModelGetMeshClass(0, GetSelectedMesh());

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

	default:
		{
			String s;
			s = ObjectTypeToString(mObjectMode);
			freyja_print("UnselectObject '%s' not supported.", s.GetCString());
			MARK_MSGF("Case '%s' not supported", s.GetCString());
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
	case tPoint:
		{
			Mesh *m = freyjaModelGetMeshClass(0, GetSelectedMesh());

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
			Vec3 xyz;
			vec4_t pos;
			vec_t t, closest;
			int selected = -1;

			for (uint32 i = 0, count = freyjaGetLightCount(); i < count; ++i)
			{
				freyjaGetLightPosition4v(i, pos);
				xyz = Vector3d(pos);

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
			}

		}
		break;

	case tFace:
		{
			// New backend ( picks faces and marks them selected )
			Mesh *m = freyjaModelGetMeshClass(0, GetSelectedMesh());

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

	case tMesh:
		{
			vec_t t, best = 9999.9f;
			int32 selected = -1;

			for (uint32 i = 0, n = freyjaModelGetMeshCount(0); i < n; ++i)
			{
				Mesh *m = freyjaModelGetMeshClass(0, i);

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
#if 0
				Mesh *m = freyjaModelGetMeshClass(0, selected);
				m->SetFlags(selected, Mesh::fSelected);
				mCursor.mPos = m->GetPosition();
#endif
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


void FreyjaControl::Transform(object_type_t obj, 
							  freyja_transform_action_t action,
							  vec_t x, vec_t y, vec_t z) 
{
	Vec3 v(x, y, z);
	Vec3 u;

	switch (action)
	{
	case fRotateAboutPoint:
	case fRotate:
		v *= HEL_PI_OVER_180;
		u = -v;
		break;

	case fScaleAboutPoint:
	case fScale:
		u.mVec[0] = 1.0f / v.mVec[0];
		u.mVec[1] = 1.0f / v.mVec[1];
		u.mVec[2] = 1.0f / v.mVec[2];
		break;
		
	case fTranslate:
		u = -v;
		break;

	default:
		MARK_MSGF("Undo for %s action=%i not implemented", ObjectTypeToString(obj).c_str(), action);
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

		freyja_print("! %s[%i] %s <%f, %f, %f>",
					 ObjectTypeToString(obj).c_str(),
					 id, 
					 ActionToString(GetEventAction()).c_str(),
					 GetEventAction(),
					 v.mVec[0], v.mVec[1], v.mVec[2]);

	}

	switch (obj)
	{
	case tMesh:
		if (mToken)
		{
			Action *a = new ActionMeshTransform(GetSelectedMesh(), action, u);
			ActionModelModified(a);

			freyjaModelMeshTransform3fv(0, GetSelectedMesh(), action, v.mVec);
		}
		break;

	case tModel:
		if (mToken)
		{
			Action *a = new ActionModelTransform(0, action, u);
			ActionModelModified(a);

			for (uint32 i = 0, n = freyjaModelGetMeshCount(0); i < n; ++i)
				freyjaModelMeshTransform3fv(0, i, action, v.mVec);
		}
		break;

	default:
		MARK_MSGF("Not Implemented"); 
	}
}


void FreyjaControl::Transform(freyja_transform_t obj, 
							  freyja_transform_action_t action,
							  index_t owner, index_t id,
							  vec_t x, vec_t y, vec_t z) 
{ 
	Vec3 v(x, y, z);

	if (mToken) 
	{
		Action *a = new ActionGenericTransform(obj, action, owner, id, v);
		ActionModelModified(a);
	}

	switch (obj)
	{
	case fTransformMesh:
		freyjaModelMeshTransform3fv(0, GetSelectedMesh(), action, v.mVec);
		break;

	default:
		MARK_MSGF("Not Implemented");
	}
}


void FreyjaControl::MoveObject(vec_t vx, vec_t vy)
{
	Vec3 t;

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

	default:
		;
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

	switch (mObjectMode)
	{
	case tModel: 
		{
			mToken = true;
		}
		break;

	case tMesh:
		{	
#if 0
			Mesh *m = freyjaModelGetMeshClass(0, GetSelectedMesh());

			if (m)
			{
				if (mToken)
				{
					// This is always a relative transform due to implementation
					Action *a = new ActionMeshTranslateExt(GetSelectedMesh(), m->GetPosition(), mCursor.mPos);
					mCursor.mPos = m->GetPosition();
					ActionModelModified(a);
					return;
				}

				// Set mesh position to be at cursor pos
				Matrix t;
				Vec3 u = mCursor.mPos - m->GetPosition();
				t.translate(u.mVec[0], u.mVec[1], u.mVec[2]);
				m->SetPosition(mCursor.mPos);
				m->TransformVertices(t);
			}
#else
			Mesh *m = freyjaModelGetMeshClass(0, GetSelectedMesh());

			if (m)
			{
				mCursor.mLastPos = m->GetPosition();
			}

			mToken = true;	
#endif
		}
		break;

	case tSelectedVertices:
		{
			Mesh *m = freyjaModelGetMeshClass(0, GetSelectedMesh());

			if (m)
			{
				if (mToken) 
				{
					Vector<index_t> list;
					Vector<Vec3> list2;
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

				Vec3 u;

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
			// Get _WORLD_ position of bone, and use _WORLD_ space transforms
			// to match the rest of this system, since local space design
			// was canned

			
		}
		break;

	case tFace: 
		{
			Mesh *m = freyjaModelGetMeshClass(0, GetSelectedMesh());

			if (m)
			{				
				if (mToken) 
				{
					Vector<index_t> list;
					Vector<Vec3> list2;
					bool found = false;

					Face *f = m->GetFace(GetSelectedFace());
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
				Vec3 u;
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

	case tPoint:
		if (mToken) 
		{
			Mesh *m = freyjaModelGetMeshClass(0, GetSelectedMesh());

			if (m)
			{
				m->GetVertexPos(GetSelectedVertex(), mCursor.mPos.mVec);
			}

			Action *a = new ActionVertexTransformExt(GetSelectedMesh(), GetSelectedVertex(), fTranslate, mCursor.mPos.mVec, mCursor.mPos);
			ActionModelModified(a);
		}
		freyjaMeshVertexTranslate3fv(GetSelectedMesh(), GetSelectedVertex(), mCursor.mPos.mVec);
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
	static int old_y = 0, old_x = 0;
	const float t = 0.5f, m = 1.0f;
	Vec3 o, d;
	float xf, yf, zf;
	int swap;

	/* Mongoose: Compute a relative movement value too here */
	swap = x;
	x = -y;
	y = -swap;


	// Rewrite this to use pivot and arc delta ( and generate the delta sweep )

	switch (plane)
	{
	case PLANE_FRONT:
		xf = ((x < old_x-t) ? -m : ((x > old_x+t) ? m : 0));
		yf = -((y < old_y-t) ? -m : ((y > old_y+t) ? m : 0));
		zf = 0.0f;
		break;

	case PLANE_TOP:
		xf = ((x < old_x-t) ? -m : ((x > old_x+t) ? m : 0));
		yf = 0.0f;
		zf = ((y < old_y-t) ? m : ((y > old_y+t) ? -m : 0));
		break;

	case PLANE_LEFT:
		xf = 0.0f;
		zf = ((x < old_x-t) ? -m : ((x > old_x+t) ? m : 0));
		yf = ((y < old_y-t) ? -m : ((y > old_y+t) ? m : 0));
		break;

	default:
		;
	}

	d = Vec3(xf, yf, zf);

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
			// Set cursor pos and rotation
			freyjaGetBoneTranslation3fv(GetSelectedBone(), mCursor.mPos.mVec);
			//freyjaGetBoneWorldPos3fv(GetSelectedBone(), mCursor.mPos.mVec);
			freyjaGetBoneRotationEuler3fv(GetSelectedBone(), o.mVec);

			mCursor.mRotate.mVec[0] = HEL_RAD_TO_DEG(o.mVec[0]);
			mCursor.mRotate.mVec[1] = HEL_RAD_TO_DEG(o.mVec[1]);
			mCursor.mRotate.mVec[2] = HEL_RAD_TO_DEG(o.mVec[2]);
		}
		break;

	default:
		freyja_print("!Case not handled in rotateObject");
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

			xyz[0] = HEL_DEG_TO_RAD(mCursor.mRotate.mVec[0]);
			xyz[1] = HEL_DEG_TO_RAD(mCursor.mRotate.mVec[1]);
			xyz[2] = HEL_DEG_TO_RAD(mCursor.mRotate.mVec[2]);

			freyjaBoneRotateEuler3fv(GetSelectedBone(), xyz);
		}
		break;


	case tMesh:
		{
			mCursor.SetMode(freyja3d::Cursor::Rotation);

			// This handles ctrl selects ( select without leaving rotate mode )
			Mesh *m = freyjaModelGetMeshClass(0, GetSelectedMesh());
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
			Vec3 r(xf, yf, zf);

			mCursor.mRotate += r;

			// Interface is in degrees, while backend is radians
			if (mCursor.mRotate.mVec[0] > 360.0f) 
				mCursor.mRotate.mVec[0] -= 360.0f;

			if (mCursor.mRotate.mVec[1] > 360.0f)
				mCursor.mRotate.mVec[1] -= 360.0f;

			if (mCursor.mRotate.mVec[2] > 360.0f)
				mCursor.mRotate.mVec[2] -= 360.0f;

			mCursor.SetMode(freyja3d::Cursor::Rotation);
		}
	}
	
	old_x = x;
	old_y = y;
}


void FreyjaControl::scaleObject(int x, int y, freyja_plane_t plane)
{
	static int old_y = 0, old_x = 0;

	if (y > old_y)
	{
		switch (plane)
		{
		case PLANE_FRONT:
			Transform(mObjectMode, fScale, 1.0, 0.99, 1.0);
			break;
		case PLANE_TOP:
			Transform(mObjectMode, fScale, 1.0, 1.0, 1.01);
			break;
		case PLANE_LEFT:
			Transform(mObjectMode, fScale, 1.0, 0.99, 1.0);
			break;
		default:
			;
		}
	}
	else if (y < old_y)
	{
		switch (plane)
		{
		case PLANE_FRONT:
			Transform(mObjectMode, fScale, 1.0, 1.01, 1.0);
			break;
		case PLANE_TOP:
			Transform(mObjectMode, fScale, 1.0, 1.0, 0.99);
			break;
		case PLANE_LEFT:
			Transform(mObjectMode, fScale, 1.0, 1.01, 1.0);
		default:
			;
		}
	}
	
	if (x < old_x)
	{
		switch (plane)
		{
		case PLANE_FRONT:
			Transform(mObjectMode, fScale, 0.99, 1.0, 1.0);
			break;
		case PLANE_TOP:
			Transform(mObjectMode, fScale, 0.99, 1.0, 1.0);
			break;
		case PLANE_LEFT:
			Transform(mObjectMode, fScale, 1.0, 1.0, 0.99);
			break;
		default:
			;
		}
	}
	else if (x > old_x)
	{
		switch (plane)
		{
		case PLANE_FRONT:
			Transform(mObjectMode, fScale, 1.01, 1.0, 1.0);
			break;
		case PLANE_TOP:
			Transform(mObjectMode, fScale, 1.01, 1.0, 1.0);
			break;
		case PLANE_LEFT:
			Transform(mObjectMode, fScale, 1.0, 1.0, 1.01);
		default:
			;
		}
	}
	
	old_x = x;
	old_y = y;
}


void FreyjaControl::MotionEdit(int x, int y, freyja_plane_t plane)
{
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
				return;
			}

			switch (GetSelectedView())
			{
			case PLANE_BACK:
				mSceneTrans += Vec3(xyz[0], xyz[1], xyz[2]);
				break;
				
			case PLANE_BOTTOM:
				mSceneTrans += Vec3(xyz[0], xyz[2], xyz[1]);
				break;
				
			case PLANE_RIGHT:
				mSceneTrans += Vec3(xyz[2], xyz[1], xyz[0]);
				break;

			case PLANE_FRONT: // front, xy
				mSceneTrans += Vec3(xyz[0], xyz[1], xyz[2]);
				break;
				
			case PLANE_TOP: // top, xz
				mSceneTrans += Vec3(xyz[0], xyz[2], xyz[1]);
				break;
				
			case PLANE_LEFT: // left, zy
				mSceneTrans += Vec3(xyz[2], xyz[1], xyz[0]);
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
		case modeMove:
			{
				vec_t vx = x, vy = y;
				AdjustMouseXYForViewports(vx, vy);
				MoveObject(vx, vy);
			}
			break;

		case modeRotate: 
			rotateObject(x, y, plane);
			break;
			
		case modeScale:
			scaleObject(x, y, plane);
			break;

		case modeSelectByBox:
			freyja_print("! FIXME: Selection by box was removed after 0.9.3!");
			break;
			
		default:
			break;
		}

		break;
	}
}


bool FreyjaControl::LoadRecentFilesResource(const char *filename)
{
	/* Recent files persistance */
	SystemIO::TextFileReader r;
	//String file = freyja_rc_map_string(filename);
	
	if (r.Open(filename/*file.GetCString()*/))
	{
		for (uint32 j = 0; j < mRecentFileLimit && !r.IsEndOfFile(); ++j)
		{
			const char *sym = r.GetLine();//r.ParseSymbol();
			MSTL_MSG("**** %s", sym);
			AddRecentFilename(sym);
		}
		
		r.Close();
		return true;
	}

	return false;
}


void FreyjaControl::LoadResource()
{
	String s = freyja_rc_map_string(mResourceFilename.GetCString());
	int i, x, y;
	bool failed = true;


	// Setup the UI
	if (!mResource.Load((char *)s.GetCString()))
	{
		failed = false;
	}

	if (failed)
	{
		MARK_MSGF("ERROR: Couldn't find resource file '%s'\n", s.GetCString());
		freyja_event_shutdown();
	}


	/* GUI stuff */
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

	if (mResource.Lookup("GRID_ON", &i))
	{
		if (i)
		{
			mRender->SetFlag(FreyjaRender::fGrid);
		}
		else
		{
			mRender->ClearFlag(FreyjaRender::fGrid);
		}
	}

	if (mResource.Lookup("FLUSH_RESOURCE", &i) && i)
	{
		mResource.Flush();
	}

	// FIXME: Rework image plugins with flags like model plugins
	/* Image file dialog patterns - texture */
	mgtk_event_fileselection_append_pattern(eOpenTexture, 
											"All Files (*.*)", "*.*");
	mgtk_event_fileselection_append_pattern(eOpenTexture, 
											"BMP Image (*.bmp)", "*.bmp");
	mgtk_event_fileselection_append_pattern(eOpenTexture, 
											"DDS Image (*.dds)", "*.dds");
	mgtk_event_fileselection_append_pattern(eOpenTexture, 
											"JPEG Image (*.jpg)", "*.jpg");
	mgtk_event_fileselection_append_pattern(eOpenTexture, 
											"PCX Image (*.pcx)", "*.pcx");
	mgtk_event_fileselection_append_pattern(eOpenTexture, 
											"PNG Image (*.png)", "*.png");
	mgtk_event_fileselection_append_pattern(eOpenTexture, 
											"PPM Image (*.ppm)", "*.ppm");
	mgtk_event_fileselection_append_pattern(eOpenTexture, 
											"TARGA Image (*.tga)", "*.tga");

	/* Image file dialog patterns - textureB */
	mgtk_event_fileselection_append_pattern(eOpenTextureB, 
											"All Files (*.*)", "*.*");
	mgtk_event_fileselection_append_pattern(eOpenTextureB, 
											"BMP Image (*.bmp)", "*.bmp");
	mgtk_event_fileselection_append_pattern(eOpenTextureB, 
											"DDS Image (*.dds)", "*.dds");
	mgtk_event_fileselection_append_pattern(eOpenTextureB, 
											"JPEG Image (*.jpg)", "*.jpg");
	mgtk_event_fileselection_append_pattern(eOpenTextureB, 
											"PCX Image (*.pcx)", "*.pcx");
	mgtk_event_fileselection_append_pattern(eOpenTextureB, 
											"PNG Image (*.png)", "*.png");
	mgtk_event_fileselection_append_pattern(eOpenTextureB, 
											"PPM Image (*.ppm)", "*.ppm");
	mgtk_event_fileselection_append_pattern(eOpenTextureB, 
											"TARGA Image (*.tga)", "*.tga");


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
		extern void mgtk_add_menu_item(char *text, long event);
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
			FreyjaPluginDesc *plugin = freyjaGetPluginClassByIndex(i);
			
			if (plugin && plugin->mImportFlags)
			{
				mgtk_event_fileselection_append_pattern(loadEventId, 
														plugin->mDescription,
														plugin->mExtention);
			}

			if (plugin && plugin->mExportFlags)
			{
				mgtk_event_fileselection_append_pattern(saveEventId, 
														plugin->mDescription,
														plugin->mExtention);
			}
		}
	}
}


void FreyjaControl::TexCoordMove(vec_t u, vec_t v)
{
	Mesh *m = freyjaModelGetMeshClass(0, GetSelectedMesh());

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

	Mesh *m = freyjaModelGetMeshClass(0, GetSelectedMesh());

	if (!m)
		return;

	const vec_t cutoff = 0.005f; // must be within this many 'uv units' to pick
	Vec3 uv(u, v, 0.0f);
	Vec3 uvB;
	Face *f;
	vec_t dist, bestDist = 2.0f; // Outside 0.0f - 1.0f
	
	for (uint32 i = 0, n = m->GetFaceCount(); i < n; ++i)
	{
		f = m->GetFace(i);

		/* Only consider UVs using selected material */
		if (!f || f->mMaterial != GetSelectedTexture())
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
	 freyja_print("Selected UV[%i].  Move with right mouse button.", mTexCoordArrayIndex));
}


////////////////////////////////////////////////////////////
// Non-object Event Handler Functions
////////////////////////////////////////////////////////////


void eCurrentFaceFlagAlpha()
{
	freyjaPolygonFlagAlpha(FreyjaControl::mInstance->GetSelectedFace(), 1);
}


void freyja_load_texture_buffer(byte *image, uint32 w, uint32 h, uint32 bpp)
{
	if (bpp == 24)
		FreyjaControl::mInstance->LoadTextureBuffer(image, w, h, 24, Texture::RGB);
	else if (bpp == 32)
		FreyjaControl::mInstance->LoadTextureBuffer(image, w, h, 32, Texture::RGBA);
}


void eTextureSlotLoadToggle()
{
	bool on = FreyjaControl::mInstance->ToggleFlag(FreyjaControl::fLoadTextureInSlot);

	freyja_print("Texture loading into current slot [%s]",
				on ? "on" : "off");
}


void eMaterialSlotLoadToggle()
{
	bool on = FreyjaControl::mInstance->ToggleFlag(FreyjaControl::fLoadMaterialInSlot);

	freyja_print("Texture loading into current slot [%s]",
				on ? "on" : "off");
}


void eModelUpload(char *filename)
{
	FreyjaControl::mInstance->LoadModel(filename);
}


void eTextureUpload(unsigned int id)
{
	byte *image;
	uint32 w, h, bpp, type;

	/* Texture image was stored as raw buffer */
	freyjaGetTextureImage(id, &w, &h, &bpp, &type, &image);
	freyja_print("!test");

	if (image)
	{
		switch (type)
		{
		case RGBA_32:
			FreyjaControl::mInstance->LoadTextureBuffer(image, w, h, 32, Texture::RGBA);
			break;

		case RGB_24:
			FreyjaControl::mInstance->LoadTextureBuffer(image, w, h, 24, Texture::RGB);
			break;

		case INDEXED_8:
			FreyjaControl::mInstance->LoadTextureBuffer(image, w, h, 8, Texture::INDEXED);
			break;

		default:
			freyja_print("%s> ERROR: Unsupported texture colormap %d",
						"FreyjaModel::loadModel", type);
		}
	}
}


void eOpenModel(char *filename)
{
	if (FreyjaControl::mInstance->LoadModel(filename))
	{
		char title[1024];
		snprintf(title, 1024, "%s - Freyja", filename);
		freyja_set_main_window_title(title);
		FreyjaControl::mInstance->AddRecentFilename(filename);
	}
}


void eSaveModel(char *filename)
{
	FreyjaControl::mInstance->SaveModel(filename);
}


void eOpenMaterial(char *filename)
{
	if (FreyjaControl::mInstance->LoadMaterial(filename))
	{
		freyja_refresh_material_interface();
	}
}


void eSaveMaterial(char *filename)
{
	FreyjaControl::mInstance->SaveMaterial(filename);
}


void eMeshUnselectFaces()
{
	Mesh *m = freyjaModelGetMeshClass(0, FreyjaControl::mInstance->GetSelectedMesh());

	if ( m )
	{
		for (uint32 i = 0, n = m->GetFaceCount(); i < n; ++i)
		{
			m->ClearFaceFlags(i, Vertex::fSelected);
		}

		freyja_print("Reset selected flag on all faces in mesh.");
	}
}


void eMeshUnselectVertices()
{
	Mesh *m = freyjaModelGetMeshClass(0, FreyjaControl::mInstance->GetSelectedMesh());

	if ( m )
	{
		for (uint32 i = 0, n = m->GetVertexCount(); i < n; ++i)
		{
			m->ClearVertexFlags(i, Vertex::fSelected);
		}

		freyja_print("Reset selected flag on all vertices in mesh.");
	}
}


void eNotImplementedVec(vec_t v)
{
	freyja_print("Not Implementated");	
}

void eSetSelectedViewport(unsigned int value)
{
	FreyjaControl::mInstance->SetSelectedViewport(value);
}


void FreyjaControlEventsAttach()
{
	ResourceEventCallback::add("eMeshUnselectFaces", &eMeshUnselectFaces);
	ResourceEventCallback::add("eMeshUnselectVertices", &eMeshUnselectVertices);
	ResourceEventCallbackString::add("eOpenMaterial", &eOpenMaterial);
	ResourceEventCallbackString::add("eSaveMaterial", &eSaveMaterial);
	ResourceEventCallbackString::add("eOpenModel", &eOpenModel);
	ResourceEventCallbackString::add("eSaveModel", &eSaveModel);
	ResourceEventCallbackString::add("eModelUpload", &eModelUpload);
	ResourceEventCallback::add("eTextureSlotLoadToggle", &eTextureSlotLoadToggle);
	ResourceEventCallback::add("eMaterialSlotLoadToggle", &eMaterialSlotLoadToggle);
	ResourceEventCallback::add("eCurrentFaceFlagAlpha", &eCurrentFaceFlagAlpha);

	ResourceEventCallbackUInt::add("eSetSelectedViewport", &eSetSelectedViewport);

	ResourceEventCallbackVec::add("eUVPickRadius", &eNotImplementedVec);
	ResourceEventCallbackVec::add("eVertexPickRadius", &eNotImplementedVec); 
}


////////////////////////////////////////////////////////////////
// View Events
////////////////////////////////////////////////////////////////

void eLineBone(unsigned int value)
{
	if (value)
	{
		uint32 id = ResourceEvent::GetResourceIdBySymbol("ePolyMeshBone");
		mgtk_toggle_value_set(id, 0);
		FreyjaRender::mBoneRenderType = 1;
	}
}


void ePolyMeshBone(unsigned int value)
{
	if (value)
	{
		uint32 id = ResourceEvent::GetResourceIdBySymbol("eLineBone");
		mgtk_toggle_value_set(id, 0);
		FreyjaRender::mBoneRenderType = 2;
	}
}


void eSetNearHeight(vec_t f)
{
	FreyjaRender::mSingleton->SetNearHeight(f);
	freyja_event_gl_refresh();
}


void eSetZoomLevel(vec_t f)
{
	FreyjaRender::mSingleton->SetNearHeight(f*20.0f);
	freyja_event_gl_refresh();
}


void eRenderToggleBoneZClear(unsigned int value)
{
	if (value)
	{
		FreyjaRender::mSingleton->SetFlag(FreyjaRender::fBonesNoZbuffer);
	}
	else
	{
		FreyjaRender::mSingleton->ClearFlag(FreyjaRender::fBonesNoZbuffer);
	}

	freyja_print("Bone rendering with cleared Z buffer [%s]",
				 value ? "ON" : "OFF");
}


void eRenderToggleGridZClear(unsigned int value)
{
	if (value)
	{
		FreyjaRender::mSingleton->SetFlag(FreyjaRender::fRenderGridClearedZBuffer);
	}
	else
	{
		FreyjaRender::mSingleton->ClearFlag(FreyjaRender::fRenderGridClearedZBuffer);
	}

	freyja_print("Grid rendering with cleared Z buffer [%s]",
				 value ? "ON" : "OFF");
}


void eImplementationRemoved()
{
	freyja_print("Implementation removed");
	DEBUG_MSG("Implementation removed");
}


void eImplementationRemovedUInt(unsigned int u)
{
	freyja_print("Implementation removed");
	DEBUG_MSG("Implementation removed");
}


void FreyjaViewEventsAttach()
{
	ResourceEventCallback::add("eTextureSlotLoadToggleB", &eImplementationRemoved);
	ResourceEventCallback::add("eOpenFileTextureB", &eImplementationRemoved);
	ResourceEventCallback::add("eCollapseFace", &eImplementationRemoved);


	ResourceEventCallbackUInt::add("eSetMaterialTextureB", &eImplementationRemovedUInt);
	ResourceEventCallbackUInt::add("eRenderToggleGridZClear", &eRenderToggleGridZClear);
	ResourceEventCallbackUInt::add("eRenderToggleBoneZClear", &eRenderToggleBoneZClear);
	ResourceEventCallbackUInt::add("ePolyMeshBone", &ePolyMeshBone);
	ResourceEventCallbackUInt::add("eLineBone", &eLineBone);


	ResourceEventCallbackVec::add("eSetNearHeight", &eSetNearHeight);
	ResourceEventCallbackVec::add("eSetZoomLevel", &eSetZoomLevel);
}


