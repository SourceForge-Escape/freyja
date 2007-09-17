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

#include "config.h"

#include <stdlib.h> 
#include <stdio.h> 
#include <math.h> 
#include <stdarg.h> 
#include <string.h>

#if TINYXML_FOUND
#   include <tinyxml/tinyxml.h>
#endif

#include <mstl/String.h>
#include <mstl/SystemIO.h>
#include <mgtk/QueryDialog.h>
#include <mgtk/ResourceEvent.h>
#include <mgtk/ResourceEventDelegate.h>
#include <mgtk/ConfirmationDialog.h>

#include <hel/math.h>
#include <hel/Ray.h>
#include <hel/Quat.h>

#include <freyja/Plugin.h>
#include <freyja/Bone.h>
#include <freyja/Mesh.h>
#include <freyja/FreyjaImage.h>
#include <freyja/Material.h>
#include <freyja/CameraABI.h>
#include <freyja/LightABI.h>
#include <freyja/LuaABI.h>
#include <freyja/MaterialABI.h>
#include <freyja/Metadata.h>
#include <freyja/MeshABI.h>
#include <freyja/PythonABI.h>
#include <freyja/SkeletonABI.h>
#include <freyja/PluginABI.h>
#include <freyja/QueryABI.h>
#include <freyja/TextureABI.h>

#include "FreyjaOpenGL.h" // for load_shader
#include "freyja_events.h"

#include "FreyjaControl.h"

using namespace mstl;
using namespace freyja;
using namespace freyja3d;

using hel::Vec3;


int load_texture(const char *filename);
int load_shader(const char *filename);
void freyjaQueryCallbackHandler(unsigned int size, freyja_query_t *array);
void ePluginImport(ResourceEvent *e);
void ePluginExport(ResourceEvent *e);
mgtk_tree_t *freyja_generate_skeletal_ui(uint32 skelIndex, uint32 rootIndex, 
										 mgtk_tree_t *tree);

FreyjaControl *FreyjaControl::mInstance = NULL;
uint32 FreyjaControl::mSelectedControlPoint = 0;
Vector<hel::Vec3> FreyjaControl::mControlPoints;

/* Cached event ids */
uint32 FreyjaControl::eRotateObjectId = 0;
uint32 FreyjaControl::eScaleObjectId = 0;
uint32 FreyjaControl::eMoveObjectId = 0;
uint32 FreyjaControl::eUnselectObjectId = 0;
uint32 FreyjaControl::eSelectObjectId = 0;
uint32 FreyjaControl::eBoxSelectObjectId = 0;
uint32 FreyjaControl::eInfoObjectId = 0;
uint32 FreyjaControl::ePaintObjectId = 0;

uint32 FreyjaControl::eAxisJointId = 0;
uint32 FreyjaControl::eSphereJointId = 0;
uint32 FreyjaControl::ePointJointId = 0;

uint32 FreyjaControl::EvModeAutoKeyframeId = 0;
uint32 FreyjaControl::EvBoneIteratorId = 0;
uint32 FreyjaControl::EvSetBoneNameId = 0;
uint32 FreyjaControl::EvMoveXId = 0;
uint32 FreyjaControl::EvMoveYId = 0;
uint32 FreyjaControl::EvMoveZId = 0;
uint32 FreyjaControl::EvRotateXId = 0;
uint32 FreyjaControl::EvRotateYId = 0;
uint32 FreyjaControl::EvRotateZId = 0;
uint32 FreyjaControl::EvScaleXId = 0;
uint32 FreyjaControl::EvScaleYId = 0;
uint32 FreyjaControl::EvScaleZId = 0;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

FreyjaControl::FreyjaControl() :
	mGroupBitmap(0x0),
	mSelectedCamera(0),
	mSelectedAnimation(0),
	mSelectedMetadata(0),
	mSelectedKeyFrame(0),
	mSelectedModel(0),
	mSelectedView(PLANE_FREE),
	mSelectedViewport(0),

	mCursor(),
	mMaterial(),
	mSceneTrans(0.0f, -18.0f, 0.0f),
	mActionManager(),

	mRecentModel(FREYJA_RECENT_FILES, "eRecentFiles"),
	mRecentMesh("freyja-dev-recent-mesh_xml", "eRecentMeshXML"),
	mRecentMetadata("freyja-dev-recent-metadata_xml", "eRecentMetadataXML"),
	mRecentSkeleton("freyja-dev-recent-skeleton_xml", "eRecentSkeletonXML"),
	mRecentKeyframe("freyja-dev-recent-keyframe_xml", "eRecentKeyframe"),
	mRecentLua("freyja-dev-recent-lua", "eRecentLua"),
	mRecentPython("freyja-dev-recent-python", "eRecentPython"),

	mTexture(),
	mRender(NULL),
	mResourceFilename("freyja-dev.mlisp"),
	mUserPrefsFilename("freyja-dev_prefs.mlisp"),
	mCurrentlyOpenFilename(),
	mControlScheme(eScheme_Model),
	mEventMode(aNone),
	mObjectMode(tNone),
	mFlags(fNone),
	mModKey(0),
	mMouseButton(0),
	mTexCoordArrayIndex(INDEX_INVALID),
	mMouseState(0),
	mUVMouseState(0),
	mXYZMouseState(0),
	mAllowBoneNameUpdate(true),
	mCleared(true),
	mFullScreen(false),
	mToken(false)
{
	/* Add extra paths for freyja plugins. */
	String sPluginDir = freyja_rc_map_string("plugins/");
	freyjaPluginAddDirectory(sPluginDir.c_str());

	/* Start up libfreyja backend, and redirect its print/logging. */
	freyjaSpawn();
	extern void freyja__setPrinter(Printer *printer, bool freyjaManaged);
	freyja__setPrinter(&mPrinter, false);

	/* Spawn 0th light, and set the light iterator */
	vec4_t lightPos = { 12.0f, 35.0f, 8.0f, 0.0f }; // FIXME: Use mlisp cvars.
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
	rgba[0] = rgba[1] = rgba[2] = 0.79f;
	freyjaMaterialDiffuse(mIndex, rgba);
	rgba[0] = rgba[1] = rgba[2] = 0.2f;
	freyjaMaterialAmbient(mIndex, rgba);
	rgba[0] = rgba[1] = rgba[2] = 0.01f;
	freyjaMaterialSpecular(mIndex, rgba);
	freyjaMaterialShininess(mIndex, 0.0f);
	freyjaMaterialSetFlag(mIndex, fFreyjaMaterial_Texture); // texture on!

	// 0th camera
	mIndex = freyjaCameraCreate();
	freyjaCameraPos3f(mIndex, 64.0f, 64.0f, 64.0f);
	freyjaCameraTarget3f(mIndex, 0.0f, 32.0f, 0.0f);


	/* Hook up the view */
	mRender = FreyjaRender::GetInstance();
	FREYJA_ASSERTMSG(mRender, "FreyjaRender Singleton control allocation failure");

	if (!mRender)
	{
		SystemIO::Print("See freyja-dev.log for possible errors.\n");
		exit(-1);
	}
}


void FreyjaControl::Init()
{
	/* Build the user interface from lisp, and load user preferences */
	LoadResource();

	/* Load up recent files list into file menu */
	mMaterial.InitRecentFilesMenu();

	Print("Loading %s...", mRecentModel.GetResourceFilename());
	if ( !mRecentModel.LoadResource() )
	{
		Print("Failed to load '%s'.", mRecentModel.GetResourceFilename() );
	}

	Print("Loading %s...", mRecentMesh.GetResourceFilename());
	if ( !mRecentMesh.LoadResource() )
	{
		Print("Failed to load '%s'.", mRecentMesh.GetResourceFilename() );
	}

	Print("Loading %s...", mRecentMetadata.GetResourceFilename());
	if ( !mRecentMetadata.LoadResource() )
	{
		Print("Failed to load '%s'.", mRecentMetadata.GetResourceFilename() );
	}

	Print("Loading %s...", mRecentSkeleton.GetResourceFilename());
	if ( !mRecentSkeleton.LoadResource() )
	{
		Print("Failed to load '%s'.", mRecentSkeleton.GetResourceFilename() );
	}

	Print("Loading %s...", mRecentLua.GetResourceFilename());
	
	if ( !mRecentLua.LoadResource() )
	{
		Print("Failed to load '%s'.", mRecentLua.GetResourceFilename() );
	}

	Print("Loading %s...", mRecentPython.GetResourceFilename());
	
	if ( !mRecentPython.LoadResource() )
	{
		Print("Failed to load '%s'.", mRecentPython.GetResourceFilename() );
	}


	/* Set query handler. */
	freyjaQuerySetCallback( freyjaQueryCallbackHandler );


	/* Set some basic defaults */
	SetControlScheme(eScheme_Model);
	SetZoom(1.0f);
	mEventMode = aNone;
	mFullScreen = false;

	/* Mongoose 2002.02.23, Tell renderer to start up with some defaults */
	uint32 width = 1024; //800;//740;
	uint32 height = 768; //600;//560;
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
	Print("Zoom set to %f", mRender->GetZoom());
	RefreshContext();
}



////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////


// Mongoose - 2006.07.31 
// More crap for old system to be backported then rewritten properly  =/
void FreyjaControl::AdjustMouseXYForViewports(vec_t &x, vec_t &y)
{
#if DEBUG_VIEWPORT_MOUSE
	Print("> Mouse x = %f y = %f", x, y);
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
			// Print("Selected viewport 0");
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
			// Print("Selected viewport 2");
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
			// Print("Selected viewport 1");
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
			// Print("Selected viewport 3");
			SetSelectedView(mRender->mViewports[3].plane); // PLANE_FREE

			// Adjust actual window space mouse x, y to fit to viewport
			// This makes the x, y fill the 'window' for the viewport
			x = x*2.0f;
			y = y*2.0f;
		}

#if DEBUG_VIEWPORT_MOUSE
		Print("       x = %f y = %f", x, y);
#endif
	}
}


String FreyjaControl::ObjectTypeToString(object_type_t t)
{
	String s;

	switch (t)
	{
	case tCamera:
		s = String("tCamera");
		break;

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

	case tMetadata:
		s = String("tMetadata");
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


void FreyjaControl::CastPickRay(vec_t x, vec_t y)
{
	hel::Ray &r = mRender->mTestRay;

	switch (GetControlScheme())
	{
	case eScheme_UV:
		// FIXME: This doesn't handle UV in sized viewports properly.
		r.SetOrigin( hel::Vec3(x / mRender->GetWindowWidth(), 
							   y / mRender->GetWindowHeight(), 
							   10.0f) );
		r.SetDir( hel::Vec3(0.0f, 0.0f, -1.0f) );
		break;

	case eScheme_Model:
	case eScheme_Animation:
		if (GetSelectedView() == PLANE_FREE)
		{
			// FIXME: Stop gap fix until new camera system is checked in...
			// ( so some things can be tested -- like the camera )
			hel::Vec3 t(x, y, 0.0f);
			GetWorldFromScreen(t.mX, t.mY, t.mZ);
			t.mZ += 100;
			t -= ( GetSceneTranslation() * ( 1 / GetZoom() ) );

			hel::Vec3 v;
			mRender->GetRotation(v.mVec);
			v *= HEL_PI_OVER_180; // Deg -> Rad

			//Print("$$ yaw = %f", v.mY);

			hel::Mat44 m;
			m.Rotate(v);
			m.Multiply3fv(t.mVec);
			r.SetOrigin(t);
			r.SetDir(0.0f, 0.0f, -1.0f);

			hel::Vec3 tmp = r.GetDir();
			m.Multiply3fv(tmp.mVec);
			r.SetDir(tmp);
		}
		else
		{
			vec_t z = 0.0f;

			GetWorldFromScreen(x, y, z);

			switch (GetSelectedView())
			{
			case PLANE_BACK:
				r.SetOrigin(x, y, z - 100);
				r.SetDir(0, 0, 1);
				break;

			case PLANE_FRONT: // Front, XY
				r.SetOrigin(x, y, z + 100);
				r.SetDir(0, 0, -1);
				break;

			case PLANE_BOTTOM:
				r.SetOrigin(x, y - 100, -z);
				r.SetDir(0, 1, 0);
				break;

			case PLANE_TOP: // Top, XZ
				r.SetOrigin(x, y + 100, -z);
				r.SetDir(0, -1, 0);
				break;

			case PLANE_RIGHT:
				r.SetOrigin(x + 100, y, z);
				r.SetDir(-1, 0, 0);
				break;

			case PLANE_LEFT: // Side, ZY
				r.SetOrigin(x - 100, y, z);
				r.SetDir(1, 0, 0);
				break;

			default:
				;
			}	
		}
		break;

	default:
		// This scheme has no ray pick support.
		;
	}
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
	// FIXME: Break up into 'native' and 'plugins'
	int err = freyjaImportModel(filename); 

	if (freyjaGetCurrentSkeleton() == INDEX_INVALID &&
	    freyjaGetSkeletonCount() > 0)
	{
		freyjaCurrentSkeleton(0);
	}

	UpdateSkeletalUI();		
 
	if (err)
	{
		Print("ERROR: File '%s' not found or unknown format\n", filename);
		Print("ERROR CODE %i\n", err);
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
				Print("%s> ERROR: Unsupported texture colormap %d",
							"FreyjaModel::loadModel", type);
			}

			freyjaTextureDelete(i); // hhhmmm
		}
	}

	String title;
	title.Set("%s - Freyja", filename);
	freyja_set_main_window_title(title.c_str());
	mRecentModel.AddFilename(filename);
	mCurrentlyOpenFilename = String(filename);
	mCleared = false;

	return true;
}


bool FreyjaControl::UnserializeMesh(const char *filename)
{
#if TINYXML_FOUND
	if (!filename)
		return false;

	TiXmlDocument doc(filename);

	if ( !doc.LoadFile() )
	{
		printf("XML ERROR: %s, Line %i, Col %i\n", 
			   doc.ErrorDesc(), doc.ErrorRow(), doc.ErrorCol() );
		return false;
	}

	TiXmlElement *root = doc.RootElement(); 

	if (!root) 
	{
		printf("Couldn't find document root!\n");
		return false;
	}

	TiXmlElement *child = root->FirstChildElement();
	for( ; child; child = child->NextSiblingElement() )
	{
		String s = child->Value();

		if (s == "mesh" || s == "Mesh")
		{
			Mesh *m = new Mesh();
			m->AddToPool();

		   	if ( m->Unserialize( child ) )
			{
				Print("Mesh '%s' Loaded", filename);
			}
			else
			{
				Print("Mesh '%s' failed to load.  <%s>", filename, 
					  child ? child->Value() : "(null)");
			}
		}
		else if (s == "material")
		{
			index_t idx = freyjaMaterialCreate();
			Material *mat = freyjaGetMaterialClass(idx);
			mat->Unserialize(child);
		}
		else if (s == "weights")
		{
			TiXmlElement *child2 = child; 
			for( ; child2; child2 = child2->NextSiblingElement() )
			{
				const char *key = child2->Value();
				//printf("-- vertexweight '%s'\n", child2->Value());
				
				if ( !strncmp("vertexweight", key, 12) )
				{
					int v, b;
					float w = 0.0f;
					child2->QueryIntAttribute("vertexindex", &v);
					child2->QueryIntAttribute("boneindex", &b);
					child2->QueryFloatAttribute("weight", &w);
					
					freyjaMeshVertexWeight(GetSelectedMesh(), v, b, w);
				}
			}
		}
	}

	mRecentMesh.AddFilename(filename);

	return true;
#else
	return false;
#endif
}


bool FreyjaControl::SerializeBones(const char *filename)
{
#if TINYXML_FOUND
	TiXmlDocument doc;
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	TiXmlElement *container = new TiXmlElement("freyja");
	container->SetAttribute("version", VERSION);
	container->SetAttribute("build-date", __DATE__);
	container->SetAttribute("file-version", 1);
	doc.LinkEndChild(container);

	Bone::SerializePool(container);

	mRecentSkeleton.AddFilename(filename);
	return doc.SaveFile(filename);;
#else
	return false;
#endif
}


bool FreyjaControl::UnserializeBones(const char *filename)
{
#if TINYXML_FOUND
	TiXmlDocument doc(filename);

	if ( !doc.LoadFile() )
	{
		printf("XML ERROR: %s, Line %i, Col %i\n", 
			   doc.ErrorDesc(), doc.ErrorRow(), doc.ErrorCol() );
		return false;
	}

	TiXmlElement *root = doc.RootElement(); 

	if (!root) 
	{
		printf("Couldn't find document root!\n");
		return false;
	}

	//TiXmlElement *container = root->FirstChildElement();

	if ( Bone::UnserializePool(root) )
	{
		if (freyjaGetCurrentSkeleton() == INDEX_INVALID &&
		    freyjaGetSkeletonCount() > 0)
		{
			freyjaCurrentSkeleton(0);
		}

		UpdateSkeletalUI();	
		mRecentSkeleton.AddFilename(filename);
		return true;
	}
	
	return false;
#else
	return false;
#endif
}


bool FreyjaControl::SaveModel(const char *filename, const char *plugin_desc)
{
	if (!filename || !filename[0])
		return false;

	mstl::String desc = plugin_desc;
	mstl::String select = filename;
	freyja::PluginDesc *module = NULL;

	for (uint32 i = 0, count = freyjaGetPluginCount(); i < count; ++i)
	{
		freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(i);
			
		if (plugin == NULL)
			continue;

		if (plugin->mExportFlags && desc == plugin->mDescription)
		{
			module = plugin;
			break;
		}
	}

	bool ret = false;

	if (module)
	{
		/* Overwrite test, No longer altering filename from here on... */
		if ( SystemIO::File::DoesFileExist( select.c_str() ) )
		{
			if ( !mgtk::ExecuteConfirmationDialog("OverwriteFileDialog") )
			{
				return true; // 'Capture' event, but don't actually write to disk.
			}
		}

		/* Magically generate dialogs for import/export settings per module */

		//module->mDescription.c_str();
		//module->mExtention.c_str();
		//module->mName.c_str();
		//module->mFilename.c_str();
		
		uint32 i;
		foreach (module->mArgs, i)
		{
			// FIXME: Convert this crap to a generated QueryDialog
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

		/* Export the model */
		ret = !(freyjaExportModelByModule(select.c_str(), 
										  module->mFilename.c_str()));
	}
	else
	{
		ret = SaveModel(filename);
	}

	if (ret)
	{
		mstl::String title;
		title.Set("%s - Freyja", select.c_str());
		freyja_set_main_window_title(title.c_str());
		mCurrentlyOpenFilename = select;
		mCleared = true;
		mRecentModel.AddFilename(filename);
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
		ret = true;
	}
	else
	{
		Print("Unable to save with extention '.%s', try using '.freyja'", 
			  select.c_str()+cur);
		ret = false;
	}

	return ret;
}


void FreyjaControl::Undo()
{
	if (mActionManager.Undo())
	{
		RefreshContext();
	}
	else
	{
		Print("Undo system has no more actions to revert.");
	}		
}


void FreyjaControl::Redo()
{
	Print("Redo is not avalible in this build.");
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
		Print("Closing Model...");
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
		Print("Animation mode has no NewFile action.");
		break;

	case eScheme_Model:
		if (mgtk::ExecuteConfirmationDialog("CloseNewFileDialog"))
		{
			Clear();
			Print("Closing Model...");
			freyja_set_main_window_title(BUILD_NAME);
			mCurrentlyOpenFilename = String();
			mCleared = true;
		}
		break;
		
	case eScheme_Material:
		{
			unsigned int i = freyjaMaterialCreate();
			Print("New material [%i] created.", i);
		}
		break;
		
	default:
		;
	}
}


void FreyjaControl::EvUnserializeBones()
{
	char *filename =
	mgtk_filechooser_blocking("freyja - Load Bones...", 
							  mRecentSkeleton.GetPath(), 0,
							  "Bones XML (*.xml)", "*.xml");

	if (filename && UnserializeBones(filename) )
	{
		//mRecentSkeleton.AddFilename();
		Print("Loaded skeleton snippet '%s'", filename);
	}
	else if (filename)
	{
		Print("Bones '%s' failed to load.", filename);
	}
	
	mgtk_filechooser_blocking_free(filename);
}


void FreyjaControl::EvSerializeBones()
{
	char *filename =
	mgtk_filechooser_blocking("freyja - Save Bones...", 
							  mRecentSkeleton.GetPath(), 1,
							  "Bones XML (*.xml)", "*.xml");

	if (filename && SerializeBones(filename) )
	{
		//mRecentSkeleton.AddFilename();
		Print("Bones '%s' Saved", filename);
	}
	else if (filename)
	{
		Print("Bones '%s' failed to save.", filename);
	}
	
	mgtk_filechooser_blocking_free(filename);
}


void FreyjaControl::SaveFile()
{
	switch (GetControlScheme())
	{
	case eScheme_Animation:
		Print("Animation mode has no SaveFile action.");
		break;

	case eScheme_Model:
		if (mCleared) // safety
		{
			Print("We don't save empty files anymore");
		}
		else if (mCurrentlyOpenFilename.empty())
		{
			char *filename =
			mgtk_filechooser_blocking("freyja - Save Model...", 
									  mRecentModel.GetPath(), 1,
									  "Freyja Model (*.freyja)", "*.freyja");

			if (filename && SaveModel(filename))
			{
				Print("Model '%s' Saved", filename);
			}
			else if (filename)
			{
				Print("Model '%s' failed to save", filename);
			}
			
			mgtk_filechooser_blocking_free(filename);
		}
		else
		{
			const char *s = mCurrentlyOpenFilename.c_str();
			
			if (SaveModel(s))
			{
				Print("Model '%s' Saved", s);
			}
			else
			{
				Print("Model '%s' failed to save", s);
			}
		}
		break;

	case eScheme_Material:
		mMaterial.EvSaveMaterial(  );	
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
		Print("Animation mode has no OpenFile action.");
		break;
		
	case eScheme_UV:
		//freyja_event_file_dialog(FREYJA_MODE_LOAD_TEXTURE, "Open texture...");
		break;
		
	case eScheme_Model:
		if (!mCleared)
		{
			if (mgtk::ExecuteConfirmationDialog("CloseToOpenFileDialog"))
			{
				Clear();
				Print("Closing Model...");
				freyja_set_main_window_title(BUILD_NAME);
			}
		}

		if (mCleared)
		{
			char *filename =
			mgtk_filechooser_blocking("freyja - Open Model...", 
									  mRecentModel.GetPath(), 0,
									  "Freyja Modeler (*.freyja)", "*.freyja");
		
			if (filename)
			{
				if (LoadModel(filename))
				{
						
				}
			}

			mgtk_filechooser_blocking_free(filename);
		}
		break;
		
	case eScheme_Material:
		//freyja_event_file_dialog(FREYJA_MODE_LOAD_MATERIAL, "Open material...");
		break;
	}
}

	
void FreyjaControl::SaveFileModel()
{
	if (mCleared) // safety
	{
		Print("No changes to be saved.");
	}
	else if (mCurrentlyOpenFilename.empty())
	{
		mstl::String s = freyja_rc_map_string("/");
		char *filename =
		mgtk_filechooser_blocking("freyja - Open Pak...", s.c_str(), 1,
								  "Freyja Modeler (*.freyja)", "*.freyja");
		
		if (filename)
		{
			if (SaveModel(filename))
			{
				
			}
		}

		mgtk_filechooser_blocking_free(filename);
	}
	else
	{
		const char *s = mCurrentlyOpenFilename.c_str();

		if (SaveModel(s))
		{
			Print("Model '%s' Saved", s);
		}
		else
		{
			Print("Model '%s' failed to save", s);
		}
	}
}


void FreyjaControl::OpenFileModel()
{
	if (!mCleared)
	{
		if (mgtk::ExecuteConfirmationDialog("CloseToOpenFileDialog"))
		{
			Clear();
			Print("Closing Model...");
			freyja_set_main_window_title(BUILD_NAME);
				
			//freyja_event_file_dialog(FREYJA_MODE_LOAD_MODEL, "Open model...");
		}
	}
	else
	{
		//freyja_event_file_dialog(FREYJA_MODE_LOAD_MODEL, "Open model...");
	}
}
	
	
void FreyjaControl::RevertFile()
{
	if (mCurrentlyOpenFilename.empty())
	{
		Print("Revert requires a model being previously loaded");
	}
	else if (mgtk::ExecuteConfirmationDialog("RevertFileDialog"))
	{
		Clear();
		Print("Reverting Model...");
		freyja_set_main_window_title(BUILD_NAME);
		
		if (LoadModel(mCurrentlyOpenFilename.c_str()))
			mCleared = false;
	}
}


void FreyjaControl::Shutdown()
{
	bool exiting = true;

	if ( !mCleared && !mgtk::ExecuteConfirmationDialog("ExitWarningDialog") )
	{
		exiting = false;
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
	
	Print("Texcoord combine mode is [%s]", 
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
	
	Print("Vertex combine mode is [%s]", 
				 (mEventMode == aVertexCombine) ? "on" : "off");
}


void FreyjaControl::EvPolygonIterator(unsigned int value)
{
	int event = LookupEventSymbol("ePolygonIterator"); // cache this id

	if (!freyja_event_set_range(event, value, 0, freyjaGetMeshPolygonCount(GetSelectedMesh())))
	{
		SetSelectedFace(value);			
		RefreshContext();
		Print("Selected polygon[%i].", value);
	}
}


void FreyjaControl::EvMeshIterator(unsigned int value)
{
	int event = LookupEventSymbol("eMeshIterator"); // cache this id

	if (!freyja_event_set_range(event, value, 0, freyjaGetMeshCount()))
	{
		SetSelectedMesh(value);			
		RefreshContext();
		Print("Selected Mesh[%i] '%s'.", 
			  value, freyjaGetMeshNameString(value));
	}
}


void FreyjaControl::EvModeAutoKeyframe(unsigned int value)
{
	if (value)
	{
		mRender->SetViewMode(VIEWMODE_MODEL_EDIT);
		mRender->SetFlag(FreyjaRender::fKeyFrameAnimation);

		// 0.9.5.13 - disabled as it was very annoying
		//mRender->eRenderSkeleton2(0); // 0.9.5.9 auto set view flags
		//mRender->eRenderSkeleton3(1);

		SetControlScheme(eScheme_Animation);
		Print("Animation Scheme selected.");
		RefreshContext();
	}
	else
	{
		// If you disable auto keyframe you must set a new major mode
		mRender->SetViewMode(VIEWMODE_MODEL_EDIT);
		mRender->ClearFlag(FreyjaRender::fKeyFrameAnimation);

		// 0.9.5.13 - disabled as it was very annoying
		//mRender->eRenderSkeleton2(1); // 0.9.5.9 auto set view flags
		//mRender->eRenderSkeleton3(0);		

		SetControlScheme(eScheme_Model);
		Print("Model Editor Scheme selected.");
		RefreshContext();
	}
}


void FreyjaControl::EvBoneIterator(unsigned int value)
{
	int event = EvBoneIteratorId; //LookupEventSymbol("eBoneIterator");

	if (!freyja_event_set_range(event, value, 0, freyjaGetBoneCount()))
	{
		SetSelectedBone(value);

		if (value == GetSelectedBone() && freyjaIsBoneAllocated(value))
		{
			/* Update any bone name listeners, 
			 * 1. Dup string to avoid evil widgets that want to mutate it
			 * 2. Disable event hook up in case of UI event loop */
			char dupname[64];
			strncpy(dupname, freyjaGetBoneNameString(value), 64);
			mAllowBoneNameUpdate = false;
			mgtk_textentry_value_set(EvSetBoneNameId, dupname);
			mAllowBoneNameUpdate = true;

			// Update any spinbuttons hooked into this event.
			freyja_event_set_float(event, value);

			RefreshContext(); // Update bone selection highlights

			Print("Selected Bone[%i] '%s'.", 
				  value, freyjaGetBoneNameString(value));
		}
	}
}


void FreyjaControl::EvPointJoint(uint32 value)
{
	if (value)
	{
		// radio button like func for multiple widgets on same event
		mgtk_toggle_value_set(eSphereJointId, 0);
		mgtk_toggle_value_set(eAxisJointId, 0);
		FreyjaRender::mJointRenderType = 1;
	}
}


void FreyjaControl::EvSphereJoint(uint32 value)
{
	if (value)
	{
		// radio button like func for multiple widgets on same event
		mgtk_toggle_value_set(ePointJointId, 0);
		mgtk_toggle_value_set(eAxisJointId, 0);
		FreyjaRender::mJointRenderType = 2;
	}
}


void FreyjaControl::EvAxisJoint(uint32 value)
{
	if (value)
	{
		// radio button like func for multiple widgets on same event
		mgtk_toggle_value_set(eSphereJointId, 0);
		mgtk_toggle_value_set(ePointJointId, 0);
		FreyjaRender::mJointRenderType = 3;
	}
}


void FreyjaControl::EvAnimationSlider(unsigned int value)
{
	if (value != GetSelectedKeyFrame())
	{
		// FIXME: Needs better defined wrapping and bounds. 
		//freyja_event_set_range(eAnimationSliderId, value, 0, 500);
		SetSelectedKeyFrame(value);
		RefreshContext();
		//Print("Keyframe = %i", value);
	}
}


void FreyjaControl::EvBoxSelectObject(uint32 value)
{
	if (value)
	{
		mRender->SetFlag(FreyjaRender::fBoundingVolSelection);
		
		// We only need 2 control points for bbox selector
		mControlPoints.resize(0);
		mSelectedControlPoint = 0;
		mControlPoints.push_back(hel::Vec3(20, 20, 10));
		mControlPoints.push_back(hel::Vec3(-20, 10, -10));
		mCursor.mPos = mControlPoints[0];
		mCursor.SetMode(Cursor::Translation);
		
		SetModiferMode(aSelectByBox, "BoxSelect", eBoxSelectObjectId, 
					   Cursor::Translation);

		Print("Press Ctrl+Right Mouse Button to end selection.");
	}
	else
	{
		mControlPoints.resize(0);
		mRender->ClearFlag(FreyjaRender::fBoundingVolSelection);
		RefreshContext();
	}
}

	
void FreyjaControl::EvSelectObject(uint32 value)
{
	if (value)
	{
		SetModiferMode(aSelect, "BoxSelect", eSelectObjectId, 
					   Cursor::Invisible);

		mEventMode = aSelect;
		mCursor.SetMode(freyja3d::Cursor::Invisible);
		Print("Select %s, hold SHIFT to unselect...", 
					 ObjectTypeToString(GetObjectMode()).c_str());
	}
}

void FreyjaControl::EvUnselectObject(uint32 value)
{
	if (value)
	{
		SetModiferMode(aUnselect, "Unselect", eUnselectObjectId, 
					   Cursor::Invisible);
	}
}


void FreyjaControl::EvInfoObject(uint32 value)
{
	if (value)
	{
		SetModiferMode(aInfo, "Info", eInfoObjectId, Cursor::Invisible);
	}
}


void FreyjaControl::EvPaintObject(unsigned int value)
{
	if (value)
	{
		SetModiferMode(aPaint, "Paint", ePaintObjectId, Cursor::Invisible);
		Print("Paint mode -- using bone weight value for paint weight.");
	}
}


void FreyjaControl::SetModiferMode(action_type_t mode, 
								   const char *name, uint32 event,
								   freyja3d::Cursor::Flags flags)
{
	// Radio button like functionality for modifer mode widgets.
	uint32 modifers [] = { 
		eRotateObjectId, eScaleObjectId, eSelectObjectId, eInfoObjectId,
		eMoveObjectId, eBoxSelectObjectId, eUnselectObjectId, ePaintObjectId
	};

	for (uint32 i = 0; i < 8; ++i)
	{
		if (modifers[i] != event)
		{
			mgtk_toggle_value_set(modifers[i], 0);
		}
	}

	mEventMode = mode;
	mCursor.SetMode(flags);
	Print("%s %s...", name, ObjectTypeToString(GetObjectMode()).c_str());
	RefreshContext();
}


void FreyjaControl::EvMoveObject(uint32 value)
{
	if (value)
	{
		SetModiferMode(aMove, "Translate", 
					   eMoveObjectId, Cursor::Translation);
	}
}


void FreyjaControl::EvScaleObject(uint32 value)
{
	if (value)
	{
		mCursor.mScale = hel::Vec3(1.0f, 1.0f, 1.0f);
		SetModiferMode(aScale, "Scale", 
					   eScaleObjectId, Cursor::Scale);
	}
}


void FreyjaControl::EvRotateObject(uint32 value)
{
	if (value)
	{
		switch ( GetObjectMode() )
		{
		case tMesh:
			{
				Mesh *m = Mesh::GetMesh( GetSelectedMesh() );
				if (m) mCursor.mPos = m->GetBoundingVolumeCenter();
			}
			break;
			
		default:
			;
		}

		SetModiferMode(aRotate, "Rotate", 
					   eRotateObjectId, Cursor::Rotation);
	}
}


void FreyjaControl::EvScaleX(float value)
{
	if (value == 0.0f)
		freyja_event_set_float(EvScaleXId, 1.0f);
}


void FreyjaControl::EvScaleY(float value)
{
	if (value == 0.0f)
		freyja_event_set_float(EvScaleYId, 1.0f);
}


void FreyjaControl::EvScaleZ(float value)
{
	if (value == 0.0f)
		freyja_event_set_float(EvScaleZId, 1.0f);
}


void FreyjaControl::UnselectMode()
{
	mEventMode = aUnselect;
	mCursor.SetMode(freyja3d::Cursor::Invisible);
	Print("! (mode) Unselect object...");
	RefreshContext();
}


void FreyjaControl::SelectMode()
{
	mEventMode = aSelect;
	mCursor.SetMode(freyja3d::Cursor::Invisible);
	Print("! (mode) Select object...");
	RefreshContext();
}


void FreyjaControl::EvSkeletonName(char *text)
{
	freyjaSkeletonName(GetSelectedSkeleton(), text);
}


void FreyjaControl::EvSetBoneName(char *name)
{
	index_t bone = GetSelectedBone();

	if (mAllowBoneNameUpdate &&
		freyjaIsBoneAllocated(bone) && name && name[1])
	{
		freyjaBoneName(bone, name);
		Print("Bone[%i].name = '%s'", bone, name);
		UpdateSkeletalUI();
	}
}


void FreyjaControl::EvOpenModel(char *filename)
{
	LoadModel(filename);
}


void FreyjaControl::EvSaveModel(char *filename, char *extension)
{
	SaveModel(filename, extension);
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

	freyja_event_info_dialog("gtk-dialog-info", s.c_str());
}


void FreyjaControl::UpdateSkeletalUI()
{
	uint32 skeleton = GetSelectedSkeleton();
	index_t root = freyjaGetSkeletonRootIndex(skeleton);

	// FIXME: Should be skeleton based, and support bones not off root.
	if (root != INDEX_INVALID && freyjaIsBoneAllocated(root))
	{
		int eventId = ResourceEvent::GetResourceIdBySymbol("eSkeletonName");
		mgtk_textentry_value_set(eventId, freyjaGetSkeletonName(skeleton));

		mgtk_tree_t *tree = freyja_generate_skeletal_ui(skeleton, root, 0x0);
		mgtk_event_update_tree(FreyjaControl::EvBoneIteratorId, tree);
		mgtk_event_tree_delete(tree);
	}
	else
	{
#if 0
		char *path = mgtk_tree_new(FreyjaControl::EvBoneIteratorId, 
								   "sii",//"%s%i%i",
								   "Skeleton", 2, -1);
		mgtk_tree_free_path(path);
#else
		mgtk_event_update_tree(FreyjaControl::EvBoneIteratorId, NULL);
#endif
	}
}


bool FreyjaControl::MotionEvent(int x, int y)
{
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
				RotateObject(x, y, GetSelectedView());
				break;
			
			case aScale:
				ScaleObject(x, y, GetSelectedView());
				break;
			
			case aPaint:
				{
					vec_t vx = x, vy = y;
					AdjustMouseXYForViewports(vx, vy);
					PaintObject(vx, vy);
				}
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

				Print("Created tBone keyframe[%i] <- %.3fs, %i/%i", 
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
				uint32 k = GetSelectedKeyFrame();
				vec_t time = (vec_t)k / track.GetRate(); 
				index_t id = track.NewKeyframe(time);
				uint32 count = track.GetKeyframeCount();

				Print("Created tMesh keyframe[%i] <- %.3fs, %i/%i | %i", 
					  k, time, id, count,
					  track.mKeyFrames.size());
			}
		}
		break;

	default:
		Print("%s: Type '%s' not supported yet.", 
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
			Print("! SelectCursorAxis...");
			CastPickRay(vx, vy);
			hel::Ray &r = FreyjaRender::mTestRay;
			mCursor.mAxis = freyja3d::Cursor::eNone;

			if (mCursor.CheckForRayCollision(r))
			{
				mToken = true;

				switch (mCursor.mAxis)
				{
				case freyja3d::Cursor::eX:
					Print("Cursor ray picked X");
					break;

				case freyja3d::Cursor::eY:
					Print("Cursor ray picked Y");
					break;

				case freyja3d::Cursor::eZ:
					Print("Cursor ray picked Z");
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
			Print("! SelectCursorAxis...");
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
					Print("Cursor ray picked X");
					break;
					
				case freyja3d::Cursor::eY:
					Print("Cursor ray picked Y");
					break;
					
				case freyja3d::Cursor::eZ:
					Print("Cursor ray picked Z");
					break;
					
				case freyja3d::Cursor::eAll:
					Print("Cursor ray picked ALL");
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
		vec_t t, closest = 9999.0f;
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

				switch( GetEventAction() )
				{
				case fRotate:
					// This allows WYSWYG cursor rotation with valid undo.
					v = -v;
					mCursor.mRotate = hel::Vec3();
					break;

				default:
					;
				}

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
				//Print("! Model[%i] %s %f %f %f",
				//			 0, 
				//			 ActionToString(GetEventAction()).c_str(),
				//			 GetEventAction(),
				//			 v.mVec[0], v.mVec[1], v.mVec[2]);

				hel::Vec3 v = GetCursorData(GetEventAction());

				switch( GetEventAction() )
				{
				case fRotate:
					v = helDegToRad(v);
					break;

				default:
					;
				}

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
		
		switch (GetObjectMode())
		{
		case tFace:
			SelectObject(x, y, true);
			return true;
			break;

		default:
			;
		}

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
			SelectObject(vx, vy, true);
			break;

		case aUnselect:
			SelectObject(vx, vy, false);
			break;

		case aInfo:
			SelectObject(vx, vy, true);
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
		Print("! Cursor was released.");
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

void FreyjaControl::GetScreenToWorldOBSOLETE(vec_t &x, vec_t &y)
{
	vec_t z = 0.0f;

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


void FreyjaControl::GetPickRay(vec_t mouseX, vec_t mouseY, 
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
	Print("!%f, %f => <%f %f %f> @ %f, %f, %f\n",
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

	case tBone:
		//mgtk::ExecuteConfirmationDialog("DupeObjectDialog"))
		if (mgtk_execute_query_dialog("DuplicateBoneDialog"))
		{
			int recurse = mgtk_get_query_dialog_int("DuplicateBoneDialog", "recurse");

			Bone *b = Bone::GetBone(GetSelectedBone());
			Bone *b2 = (b) ? b->Duplicate() : NULL;
			if (b2)
			{
				Bone *parent = (b) ? Bone::GetBone(b->mParent) : NULL;
				b2->AddToPool();
				if (parent) parent->AddChild(b2->GetUID());
				freyjaSkeletonAddBone(b2->mSkeleton, b2->GetUID());	

				if (1) // QueryDialog
				{
					Bone::DuplicateChildren(b, b2, recurse, true);
				}

				SetSelectedBone(b2->GetUID());
				Print("Bone %i duplicated from bone %i.  Selected new bone.", 
					  b2->GetUID(), b->GetUID());
			}
		}
		break;

	default:
		Print("%s(): type '%s' is not supported.", 
					 __func__, ObjectTypeToString(mObjectMode).c_str());
		return false;
	}

	return true;
}


bool FreyjaControl::SplitSelectedObject()
{
	switch (mObjectMode)
	{
	case tFace:
		EvPolygonSplit();
		break;

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
		Print("%s(): type '%s' is not supported.", 
					 __func__, ObjectTypeToString(mObjectMode).c_str());
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
		Print("%s(): type '%s' is not supported.", 
					 __func__, ObjectTypeToString(mObjectMode).c_str());
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
					Print("Copied mesh (%i)...", GetSelectedMesh());
				}
				else
				{
					Print("The mesh copy buffer is already full...");
				}
			}
		}
		break;

	default:
		Print("%s(): type '%s' is not supported.", 
					 __func__, ObjectTypeToString(mObjectMode).c_str());
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
		Print("%s(): type '%s' is not supported.", 
					 __func__, ObjectTypeToString(mObjectMode).c_str());
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
		Print("Click select to delete each vertex");
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
			FREYJA_INFOMSG(false, "FIXME: tSelectedVertices support disabled");
			//CullUsingVertexBuffer();
		}
		break;

	case tBone:
		if (mgtk::ExecuteConfirmationDialog("DelBoneDialog"))
		{
			freyjaBoneDelete(GetSelectedBone());
		}
		break;

	case tMesh:
		if (Mesh::GetMesh(GetSelectedMesh()))
		{
			mToken = true;
			ActionModelModified(new ActionMeshDelete(GetSelectedMesh()));

			freyjaMeshDelete(GetSelectedMesh());
			Print("Deleted mesh (%i)...", GetSelectedMesh());
		}
		Print("Can not [Delete] mesh (%i). It does not exist.", GetSelectedMesh());
		break;

	default:
		Print("%s Object type '%s' is not supported.", 
					 __func__, ObjectTypeToString(mObjectMode).c_str());
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

			s.Set("\nTranslate %f %f %f\n", bone->mTranslation.mX, bone->mTranslation.mY, bone->mTranslation.mZ);
			info += s;

			{
				hel::Vec3 v;
				bone->mRotation.GetEulerAngles(v.mVec);
				s.Set("\nRotation (quat->Euler) %f %f %f\n", v.mX, v.mY, v.mZ);
				info += s;
			}

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


	case tMetadata:
		{
			Metadata* metadata = Metadata::GetObjectByUid( GetSelectedMetadata() );

			if ( metadata )
			{
				const char* dialog = "MetadataPropertyDialog";
				mgtk_set_query_dialog_string_default( dialog, "name", metadata->GetName() );
				mgtk_set_query_dialog_string_default( dialog, "type", metadata->GetType() );
				mgtk_set_query_dialog_string_default( dialog, "metadata", metadata->GetMetadata() );
				mgtk_set_query_dialog_string_default( dialog, "model", metadata->GetModel() );
				mgtk_set_query_dialog_string_default( dialog, "material", metadata->GetMaterial() );

				int update = mgtk_execute_query_dialog( dialog );

				if ( update )
				{
					const char* s = mgtk_get_query_dialog_string(dialog, "name");
					if (s) metadata->SetName( s );

					s = mgtk_get_query_dialog_string(dialog, "type");
					if (s) metadata->SetType( s );

					s = mgtk_get_query_dialog_string(dialog, "metadata");
					if (s) metadata->SetMetadata( s );

					s = mgtk_get_query_dialog_string(dialog, "model");
					if (s) metadata->SetModel( s );

					s = mgtk_get_query_dialog_string(dialog, "material");
					if (s) metadata->SetMaterial( s );

					FreyjaControl::GetInstance()->Dirty();
				}

				// model_filename
				// material_filename
			}
		}
		break;


	default:
		Print("%s Object type '%s' is not supported.", __func__, ObjectTypeToString(mObjectMode).c_str());
	}
}


void FreyjaControl::CreateObject()
{
	switch (mObjectMode)
	{
	case tPoint:
		mEventMode = aVertexNew;
		mCleared = false;
		RefreshContext();
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
				freyjaBoneName(idx, s.c_str());
				freyjaSkeletonAddBone(skel, idx);
				freyjaSkeletonUpdateBones(skel);

				mCleared = false;
				SetSelectedBone(idx);
				UpdateSkeletalUI();
				RefreshContext();
				Print("New Skeleton[%u].Bone[%u], parent = %i",
					  skel, idx, freyjaGetBoneParent(idx));
			}


			ActionModelModified(NULL);
		}
		break;

	case tMesh:
		//MeshNew();
		ActionModelModified(NULL);
		mCleared = false;
		RefreshContext();
		break;

	case tMetadata:
		{
			Metadata *m = new Metadata();

			if ( !m->AddToPool() )
			{
				Print("Error creating new metadata!");
			}
			else
			{
				Print("New metadata object #%i created.", m->GetUid() );
			}
		}

		ActionModelModified(NULL);
		mCleared = false;
		RefreshContext();
		break;

	default:
		Print("%s Object type '%s' is not supported.", 
			  __func__, ObjectTypeToString(mObjectMode).c_str());
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
			Print("%s() '%s' not supported.", __func__, s.c_str());
		}
	}	
}


void FreyjaControl::SelectAll(bool set)
{
	switch (mObjectMode)
	{
	case tPoint:
	case tSelectedVertices:
		{
			Mesh *m = Mesh::GetMesh( GetSelectedMesh() );

			if ( m )
			{
				if (set)
				{
					for (uint32 i = 0, n = m->GetVertexCount(); i < n; ++i)
					{
						m->SetVertexFlags(i, Vertex::fSelected);
					}
					
					Print("Selected all vertices in mesh.");
				}
				else
				{
					for (uint32 i = 0, n = m->GetVertexCount(); i < n; ++i)
					{
						m->ClearVertexFlags(i, Vertex::fSelected);
					}
					
					Print("Unselected all vertices in mesh.");					
				}
			}
		}
		break;

	case tFace:
	case tSelectedFaces:
		{
			Mesh *m = Mesh::GetMesh( GetSelectedMesh() );

			if ( m )
			{
				if (set)
				{
					for (uint32 i = 0, n = m->GetFaceCount(); i < n; ++i)
					{
						m->SetFaceFlags(i, Vertex::fSelected);
					}
					
					Print("Selected all faces in mesh.");
				}
				else
				{
					for (uint32 i = 0, n = m->GetFaceCount(); i < n; ++i)
					{
						m->ClearFaceFlags(i, Vertex::fSelected);
					}
					
					Print("Unselected all faces in mesh.");
				}
			}
		}
		break;

	default:
		{
			String s = ObjectTypeToString(mObjectMode);
			Print("%s() '%s' not supported.", __func__, s.c_str());
		}
	}	
}


// TODO: Break this up per object type x set
void FreyjaControl::SelectObject(vec_t mouseX, vec_t mouseY, bool set)
{
	// Look for objects by casting a pick ray.
	CastPickRay(mouseX, mouseY);

	switch (mObjectMode)
	{
	case tBone:
		{
			index_t selected = PickBone(FreyjaRender::mTestRay);

			if (selected != INDEX_INVALID)
			{
				if (!set)  // There is no unselect for this object type.
					return;

				SetSelectedBone(selected);

				mCursor.mPos = hel::Vec3();
				freyjaBoneBindTransformVertex(selected, mCursor.mPos.mVec, 1.0f);

				// Reset cursor for newly selected bone
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
								// FIXME: Relative keyframes
								mCursor.mRotate = key->GetData();
								mCursor.mRotate *= HEL_PI_OVER_180; 
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
						mCursor.mRotate *= HEL_PI_OVER_180; 
					}
					break;
					
				default:
					;
				}

				EvBoneIterator(selected);
			}
		}
		break;

	case tCamera:
		{
			index_t selected = PickCameraPos(FreyjaRender::mTestRay);

			if (selected != INDEX_INVALID )
			{
				if (!set)  // There is no unselect for this object type.
					return;

				Vec3 p;
				SetSelectedCamera(selected);
				freyjaGetCameraPos3fv(selected, p.mVec);
				mCursor.mPos = p;
				Print("! Camera[%i] selected by pick ray.", selected);
			}
		}
		break;

	case tSelectedFaces:
	case tFace:
		{
			index_t selected = PickFace(FreyjaRender::mTestRay);
			Mesh *m = Mesh::GetMesh(GetSelectedMesh());

			if (m && selected != INDEX_INVALID )
			{
				if (set) 
				{
					m->SetFaceFlags(selected, Face::fSelected);
					//mCursor.mPos = m->GetFacePosition(selected);
					SetSelectedFace(selected);
					Print("Face[%i] selected by pick ray.", selected);
				}
				else
				{
					m->ClearFaceFlags(selected, Face::fSelected);
					Print("Face[%i] unselected by pick ray.", selected);
				}
			}
		}
		break;

	case tLight:
		{
			index_t selected = PickLight(FreyjaRender::mTestRay);

			if (selected != INDEX_INVALID )
			{
				if (!set)  // There is no unselect for this object type.
					return;

				freyjaCurrentLight(selected);
				vec4_t p;
				freyjaGetLightPosition4v(selected, p);
				mCursor.mPos.Set(p);
				Print("! Light[%i] selected by pick ray.", selected);
			}
		}
		break;

	case tMesh:
		{
			index_t selected = PickMesh(FreyjaRender::mTestRay);
			Mesh *m = freyjaGetMeshClass(selected);

			if ( selected != INDEX_INVALID && m )
			{
				if (set)
				{
					// Only flagging in multi-mesh mode -- for grouping.
					//m->SetFlags(selected, Mesh::fSelected);
					mCursor.mPos = m->GetPosition();
					EvMeshIterator(selected);
				}
			}
		}
		break;

	case tMetadata:
		{
			const vec_t radius = 0.5f;
			hel::Ray& ray = FreyjaRender::mTestRay; //CastPickRay2(vx, vy);
			vec_t closest = 9999.0f;
		
			for (uint32 i = 0, n = Metadata::GetObjectCount(); i < n; ++i)
			{
				Metadata* metadata = Metadata::GetObjectByUid( i );
			
				// This method will encounter any 'gaps' ( NULL pointers ) in the container.
				if ( metadata )
				{
					vec_t t;

					if ( ray.IntersectSphere((vec_t*)metadata->GetPos().mVec, radius, t) )
					{
						if ( t < closest )
						{
							closest = t;
							SetSelectedMetadata( i );
							mCursor.mPos = metadata->GetPos();
						}
					}
				}
			}

			Print("Selected metadata object #%i.", GetSelectedMetadata() );
		}
		break;

	case tSelectedVertices:
	case tPoint:
		{
			index_t selected = PickVertex(FreyjaRender::mTestRay);
			Mesh *m = freyjaGetMeshClass(GetSelectedMesh());

			if ( selected != INDEX_INVALID && m )
			{
				if (set)
				{
					SetSelectedVertex(selected);
					m->SetVertexFlags(selected, Vertex::fSelected);
					mCursor.mPos = m->GetVertexPosition(selected);
					Print("! Vertex[%i] selected by pick ray.", selected);
				}
				else
				{
					m->ClearVertexFlags(selected, Vertex::fSelected);
					Print("! Vertex[%i] unselected by pick ray.", selected);
				}
			}

		}
		break;

	case tSelectedMeshes:
		{
			index_t selected = PickMeshes(FreyjaRender::mTestRay);
			Mesh *m = freyjaGetMeshClass(selected);

			if ( selected != INDEX_INVALID && m )
			{
				if (set)
				{
					m->SetFlag(Mesh::fSelected);
					mCursor.mPos = m->GetPosition();
					EvMeshIterator(selected);
				}
				else
				{
					m->ClearFlag(Mesh::fSelected);
					Print("Mesh[%i] unselected by pick ray.", selected);
				}
			}
		}
		break;

	default:
		{
			String s;
			s = ObjectTypeToString(mObjectMode);
			Print("SelectObject '%s' not supported.", s.c_str());
			MARK_MSGF("Case '%s' not supported", s.c_str());
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

		Print("! %s[%i].Anim[%i].Keyframe[%i] %s <%f, %f, %f>",
					 ObjectTypeToString(obj).c_str(),
					 id, 
					 GetSelectedAnimation(),
					 GetSelectedKeyFrame(),
					 ActionToString(GetEventAction()).c_str(),
					 GetEventAction(),
					 v.mVec[0], v.mVec[1], v.mVec[2]);
	}

	switch (obj)
	{
#if FIXME
	case tBone:
		if (mToken)
		{
			Bone *b = Bone::GetBone(GetSelectedBone());

			freyjaBoneBindTransformVertex(selected, mCursor.mPos.mVec, 1.0f);

			switch (GetControlScheme())
			{
			case eScheme_Animation:
				{				const uint32 track = 0;

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
						Print("! Keyframe transform (%s) is unsupported",
									 ActionToString(action).c_str());
					}
				}
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

		break;
#endif

	case tMesh:
		if (mToken)
		{
			//Action *a = new ActionMeshKeyframe(GetSelectedMesh(), action, u);
			//ActionModelModified(a);

			Mesh *m = Mesh::GetMesh(GetSelectedMesh());
			if (m)
			{				
				// FIXME: This is temp test constant - replace with multitrack
				const uint32 track = 0;

				uint32 k = GetSelectedKeyFrame();	
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
						Print("! Keyframe transform (%s) is unsupported",
									 ActionToString(action).c_str());
					}
				}
			}
		}
		break;

	default:
		Print("%s(): type=%s, action=%i not implemented.", 
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
	hel::Vec3 orig = v;

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

		Print("! %s[%s] %s <%f, %f, %f>",
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
				Print("No Mesh selected for Faces...");
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
				Print("%s(): Does not support (%s) for Faces...", 
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
				Print("No Mesh selected for Faces...");
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
				Print("%s(): Does not support (%s) for Faces...", 
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
			freyjaSkeletonTransform(GetSelectedSkeleton(), action, v[0], v[1], v[2]);
		}
		break;

	case tSkeleton:
		if (mToken)
		{
			//Action *a = new ActionModelTransform(0, action, u);
			//ActionModelModified(a);
			freyjaSkeletonTransform(GetSelectedSkeleton(), action, v[0], v[1], v[2]);
		}
		break;

	case tSelectedVertices:
		if (mToken &&
			action == fScale)
		{
			Mesh *m = Mesh::GetMesh(GetSelectedMesh());

			for (uint32 i = 0, n = m->GetVertexCount(); i < n; ++i)
			{
				Vertex *vert = m->GetVertex(i);
				
				if (vert && vert->mFlags & Vertex::fSelected)
				{
					hel::Vec3 p = m->GetVertexPosition(vert->mVertexIndex);

					// Argh... switch inside a branch inside loop...
					switch (action)
					{
					case fScale:
						p.mX *= v.mX;
						p.mY *= v.mY;
						p.mZ *= v.mZ;
						break;

					default:
						;
					}

					m->SetVertexPos(vert->mVertexIndex, p.mVec);
				}
			}
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
	GetScreenToWorldOBSOLETE(vx, vy);

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
		{
			// Stopgap 'fix' for reverse cursor bug
			if (mCursor.mAxis == freyja3d::Cursor::eZ)
			{
				hel::Vec3 v;
				mRender->GetRotation(v.mVec);
				vx *= (v.mY < 180.0f) ? 1.0f : -1.0f; 
				Print("%f", v.mY);
			}
			else if  (mCursor.mAxis == freyja3d::Cursor::eX)
			{
				hel::Vec3 v;
				mRender->GetRotation(v.mVec);
				vx *= (v.mY < 90.0f || v.mY > 270.0f) ? 1.0f : -1.0f; 
				Print("%f", v.mY);
			}

			t.mVec[0] = vx - t.mVec[0];
			t.mVec[1] = vy - t.mVec[1];
			t.mVec[2] = vx - t.mVec[2];
		}
		break;

	default:
		Print("! %s(): This view (%i) isn't supported.", 
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
	case tCamera:
		{
			Vec3 p;
			freyjaGetCameraPos3fv(GetSelectedCamera(), p.mVec);
			mCursor.mLastPos = p;
			freyjaCameraPos3f(GetSelectedCamera(), 
							  mCursor.mPos.mX, mCursor.mPos.mY, mCursor.mPos.mZ);
		}
		break;

	case tLight:
		{
			vec3_t pos;
			freyjaGetLightPosition4v(GetSelectedLight(), pos);
			mCursor.mLastPos = hel::Vec3(pos);
			freyjaLightPosition4v(GetSelectedLight(), mCursor.mPos.mVec);
		}
		break;

	case tMetadata:
		{
			Metadata *metadata = Metadata::GetObjectByUid( mSelectedMetadata );

			if ( metadata )
			{
				mCursor.mLastPos = metadata->GetPos();
				metadata->SetPos( mCursor.mPos );
			}
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
				Print("Tried to move invalid bone");
			}
			else if (parent == INDEX_INVALID)
			{
				switch (GetControlScheme())
				{
				case eScheme_Animation:
					{
						BoneTrack &track = b->GetTrack(GetSelectedAnimation());
						uint32 k = GetSelectedKeyFrame();
						Vec3KeyFrame *key = track.GetLocKeyframe(k);
							
						if (key)
						{
							hel::Vec3 p;
							freyjaBoneBindTransformVertex(bone, p.mVec, 1.0f);
							p = mCursor.mPos - p;
							key->SetData(mCursor.mPos);
						}
					}
					break;

				case eScheme_Model:
					freyjaBoneTranslate3fv(bone, mCursor.mPos.mVec);
					freyjaBoneUpdateBindPose(GetSelectedBone());
					break;

				default:
					;
				}
			}
			else if (bone != parent) // FIXME: only edits for bind pose for now
			{
				switch (GetControlScheme())
				{
				case eScheme_Animation:
					{
						BoneTrack &track = b->GetTrack(GetSelectedAnimation());
						uint32 k = GetSelectedKeyFrame();
						Vec3KeyFrame *key = track.GetLocKeyframe(k);
							
						if (key)
						{
							hel::Vec3 p;
							freyjaBoneBindTransformVertex(bone, p.mVec, 1.0f);
							p = mCursor.mPos - p;
							key->SetData(mCursor.mPos);
						}
					}
					break;

				case eScheme_Model:
					{
						hel::Vec3 p;
						freyjaBoneBindTransformVertex(parent, p.mVec, 1.0f);
						//freyjaGetBoneWorldPos3fv(parent, p.mVec);
						//p = b->mRotation.rotate(p);
						p = mCursor.mPos - p;
						freyjaBoneTranslate3fv(bone, p.mVec);
						freyjaBoneUpdateBindPose(GetSelectedBone());
					}
					break;

				default:
					;
				}
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
void FreyjaControl::RotateObject(int x, int y, freyja_plane_t plane)
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
		Print("! RotateObject(): This view (%i) isn't supported.",plane);
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
	case tMetadata:
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
							key->SetData(xyz);//mCursor.mRotate);
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
					Print("rot %f %f %f, loc %f %f %f", 
								 xyz[0], xyz[1], xyz[2],
								 p.mVec[0], p.mVec[1], p.mVec[2]);
#endif
				}

				freyjaBoneUpdateBindPose(GetSelectedBone());
				Print("rot %f %f %f", xyz[0], xyz[1], xyz[2]);
				break;

			default:
				;
			}
		}
		break;

	case tMetadata:
		{
			//mCursor.SetMode(freyja3d::Cursor::Rotation);
			Metadata *metadata = Metadata::GetObjectByUid( mSelectedMetadata );

			if ( metadata )
			{
				mCursor.mPos = metadata->GetPos(); // Move cursor
				hel::Quat q = metadata->GetRot();
				q.SetByEulerAngles( helDegToRad( mCursor.mRotate.mX ),
									helDegToRad( mCursor.mRotate.mY ),
									helDegToRad( mCursor.mRotate.mZ ) );
				metadata->SetRot( q );
				//Print("%f %f %f %f", q.mX, q.mY, q.mZ, q.mW);
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

		Print("! RotateObject(): ObjectType (%i) not handled.", 
					 ObjectTypeToString(mObjectMode).c_str());
	}
	
	old_x = x;
	old_y = y;
}


void FreyjaControl::ScaleObject(int x, int y, freyja_plane_t plane)
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
		Print("! ScaleObject(): This view (%i) isn't supported.", plane);
	}


	// It's VERY UNUSAL to allow scaling here
	switch (mObjectMode)
	{
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
						u = m->GetVertexPosition(v->mVertexIndex);

						u.mX *= mCursor.mScale.mX;
						u.mY *= mCursor.mScale.mY;
						u.mZ *= mCursor.mScale.mZ;

						m->SetVertexPos(v->mVertexIndex, u.mVec);
					}
				}
			}
		}
		break;

	case tMetadata:
		{
			Metadata *metadata = Metadata::GetObjectByUid( mSelectedMetadata );

			if ( metadata )
			{
				hel::Vec3 scale = metadata->GetScale();
				scale *= mCursor.mScale.mX;
				scale *= mCursor.mScale.mY;
				scale *= mCursor.mScale.mZ;
				metadata->SetScale( scale );
				mCursor.mScale = Vec3(1.0f, 1.0f, 1.0f);
			}
		}
		break;

	default:
		;
	}

	old_x = x;
	old_y = y;
}


void FreyjaControl::SetSelectedMesh(uint32 i) 
{
	index_t model = GetSelectedModel();

	if ( i < freyjaGetModelMeshCount(model) && freyjaIsMeshAllocated(i) )
	{		
		Mesh *m = Mesh::GetMesh(freyjaGetModelMeshIndex(model, i));
		mSelectedMesh = i;

		if (m)
		{
			mCursor.mPos = m->GetPosition();
		}
	}
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
		FREYJA_ASSERTMSG(0, "ERROR: Couldn't find resource file '%s'\n", s.c_str());
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


	/* Model file dialog patterns */
	freyjaPluginsInit();

	{
		// yes, you can even query eventIds for even func binds
		int loadEventId = GetEventIdByName("eOpenModel");
		int saveEventId = GetEventIdByName("eSaveModel");
		int exportEventId = GetEventIdByName("eExportFile");
		int importEventId = GetEventIdByName("eImportFile");
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

			Print("! %i/%i %i. [%s] %s, %s%s\n", i, count, 
				  plugin->GetId(),
				  plugin->mName.c_str(),
				  plugin->mFilename.c_str(),
				  plugin->mImportFlags ? "Import " : "",
				  plugin->mExportFlags ? "Export " : "");

			// Only append plugins that are external modules with import flags.
			if (plugin->mFilename.c_str() &&
				plugin->mName.c_str() && plugin->mImportFlags)
			{
				char *desc = (char*)plugin->mDescription.c_str();
				char *ext = (char*)plugin->mExtention.c_str();

				// Append filter to generic filedialog.
				mgtk_event_fileselection_append_pattern(loadEventId, desc, ext);

				// Generate menu item.
				mstl::String s;
				s.Set("%i|%s-ePluginImport", 
					  plugin->GetId(), plugin->mName.c_str());
				ResourceEventCallback2::add(s.c_str(), &ePluginImport);
				int EvImportModelId = LookupEventSymbol(s.c_str());
				mgtk_append_item_to_menu(importEventId, desc, EvImportModelId);
			}
			else if (plugin->mImportFlags)
			{
				char *desc = (char*)plugin->mDescription.c_str();
				char *ext = (char*)plugin->mExtention.c_str();

				// Append filter to generic filedialog.
				mgtk_event_fileselection_append_pattern(loadEventId, desc, ext);
			}


			// Only append plugins that are external modules with export flags.
			if (plugin->mFilename.c_str() &&
				plugin->mName.c_str() && plugin->mExportFlags)
			{
				char *desc = (char*)plugin->mDescription.c_str();
				char *ext = (char*)plugin->mExtention.c_str();

				// Append filter to generic filedialog.
				mgtk_event_fileselection_append_pattern(saveEventId, desc, ext);

				// Generate menu item.
				mstl::String s;
				s.Set("%i|%s-ePluginExport", 
					  plugin->GetId(), plugin->mName.c_str());
				ResourceEventCallback2::add(s.c_str(), &ePluginExport);
				int EvExportModelId = LookupEventSymbol(s.c_str());
				mgtk_append_item_to_menu(exportEventId, desc, EvExportModelId);
			}
			else if (plugin->mExportFlags)
			{
				char *desc = (char*)plugin->mDescription.c_str();
				char *ext = (char*)plugin->mExtention.c_str();

				// Append filter to generic filedialog.
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

	for (uint32 i = 0; i < 13; ++i)
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

		case 11:
			s = "eColorJoint";
			id = eColorJoint;
			break;

		case 12:
			s = "eColorJointHighlight";
			id = eColorJointHighlight;
			break;

		default:
			continue;
		}

		freyja_event_get_color(id, &r, &g, &b, &a);
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


void FreyjaControl::PaintObject(vec_t x, vec_t y)
{
	extern float gWeight;

	bool paint = true;
	float weight = gWeight;

	switch ( GetPaintMode() )
	{
	case ePaintSelect:
		SelectObject(x, y, true);
		break;

	case ePaintUnselect:
		SelectObject(x, y, false);
		break;

	case ePaintHeight:
		{
			Mesh *m = freyjaGetMeshClass( GetSelectedMesh() );

			if (mModKey & KEY_LSHIFT) weight = - weight;

			if (m)
			{
				CastPickRay(x, y);

				hel::Vec3 tuv;
				int face = m->PickFace(Face::fHidden, 
									   FreyjaRender::mTestRay, tuv);

				if (face > -1)
				{
					Face *f = m->GetFace(face);

					if (f)
					{
						//f->mMaterial = GetSelectedMaterial();
						//Print("^");

						uint32 i;
						foreach(f->mIndices, i)
						{
							hel::Vec3 v;
							
							m->GetVertexPos(f->mIndices[i], v.mVec);
							v.mY += weight;//0.01;
							m->SetVertexPos(f->mIndices[i], v.mVec);
						}

						Vec3 a, b, c;
						m->GetVertexPos(f->mIndices[0], a.mVec);
						m->GetVertexPos(f->mIndices[1], b.mVec);
						m->GetVertexPos(f->mIndices[2], c.mVec);
						
						Vec3 n = -Vec3::Cross(a - b, c - b);
						n.Norm();

						// Auto update with a face normal for now
						m->SetNormal(f->mIndices[0], n.mVec);
						m->SetNormal(f->mIndices[1], n.mVec);
						m->SetNormal(f->mIndices[2], n.mVec);
					}
				}
			}
		}	
		break;

	case ePaintDmap:
		{
			Mesh *m = freyjaGetMeshClass( GetSelectedMesh() );

			if (mModKey & KEY_LSHIFT) weight = - weight;

			if (m)
			{
				CastPickRay(x, y);

				hel::Vec3 tuv;
				int face = m->PickFace(Face::fHidden, 
									   FreyjaRender::mTestRay, tuv);

				if (face > -1)
				{
					Face *f = m->GetFace(face);

					if (f)
					{
#if 1
						Vec3 r = FreyjaRender::mTestRay.GetDir();
#else
						Vec3 r = f->mNormal;
#endif
						r *= weight;

						uint32 i;
						foreach(f->mIndices, i)
						{
							Vec3 v;
							
							m->GetVertexPos(f->mIndices[i], v.mVec);
							v += r;
							m->SetVertexPos(f->mIndices[i], v.mVec);
						}
						
						m->RecomputeFaceNormal(face, true);
					}
				}
			}
		}	
		break;

	case ePaintMaterial:
		{
			Mesh *m = freyjaGetMeshClass( GetSelectedMesh() );

			if (m)
			{
				CastPickRay(x, y);

				hel::Vec3 tuv;
				int face = m->PickFace(Face::fHidden, 
									   FreyjaRender::mTestRay, tuv);

				if (face > -1)
				{
					Face *f = m->GetFace(face);

					if (f)
					{
						f->mMaterial = GetSelectedMaterial();
					}

					Print("-- f = %i, t = %f, uv = %f, %f", 
						  face, tuv.mX, tuv.mY, tuv.mZ);
				}
			}
		}		
		break;

	case ePaintUnweight:
		paint = false;  // cheesy

	case ePaintWeight:
		{
			/* TODO: Sphere 'brush' test next.
			 *
			 * 1. Cast ray to find face hit.
			 *
			 * 2. At point of intersection generate a logical sphere
			 *    given a ray's t-bias and sphere's radius.
			 *
			 * 3. Calculate these weights with given weight to adjust
			 *    vertex weight.
			 */

			CastPickRay(x, y);

#if 1 // test face based brush for now  
			Mesh *m = freyjaGetMeshClass( GetSelectedMesh() );
			index_t bone = GetSelectedBone();

			if (m && bone != INDEX_INVALID)
			{
				hel::Vec3 tuv;
				int face = m->PickFace(Face::fHidden, FreyjaRender::mTestRay, tuv);

				if (face > -1)
				{
					Face *f = m->GetFace(face);

					// Ignore already selected faces to help unsteady hands
					if (f && !(f->mFlags & Face::fSelected) )
					{
						// Ignore this on next pass.
						//f->mFlags |= Face::fHidden;

						// free visual feedback since it's SELECTED.
						f->mFlags |= Face::fSelected;

						unsigned int i;
						foreach (f->mIndices, i)
						{
							index_t vertex = f->mIndices[i];
							if (paint)
							{
								m->AddWeight(vertex, weight, bone); 
								//Print("! Vertex[%i] painted weight.", vertex);
							}
							else
							{
								m->RemoveWeight(vertex);
								//Print("! Vertex[%i] removed all weights.", vertex);
							}
						}
					}

					Print("-- f = %i, t = %f, uv = %f, %f", 
						  face, tuv.mX, tuv.mY, tuv.mZ);
				}
			}
#else
			index_t vertex = PickVertex(FreyjaRender::mTestRay);
			index_t bone = GetSelectedBone();
			Mesh *m = freyjaGetMeshClass( GetSelectedMesh() );

			if ( m && vertex != INDEX_INVALID && bone != INDEX_INVALID )
			{
				if (paint)
				{
					m->AddWeight(vertex, weight, bone); 
					Print("! Vertex[%i] painted weight.", vertex);
				}
				else
				{
					m->RemoveWeight(vertex);
					Print("! Vertex[%i] removed all weights.", vertex);
				}
			}
#endif
		}
	    break;

	default:
	    ;
	}
}


void FreyjaControl::TexCoordMove(vec_t u, vec_t v)
{
	Mesh *m = Mesh::GetMesh(GetSelectedMesh());

	if (!m || mTexCoordArrayIndex == INDEX_INVALID)
		return;

	static uint32 texcoord = INDEX_INVALID;  // FIXME: hackkk

	if (texcoord != mTexCoordArrayIndex)
	{
		texcoord = mTexCoordArrayIndex;
		vec3_t uv = {0.0f, 0.0f};
		m->GetTexCoord(mTexCoordArrayIndex, uv);
		
		mActionManager.Push(new ActionTexCoordTransform(GetSelectedMesh(), mTexCoordArrayIndex, uv[0], uv[1]));

#if 0	
		// UVMaps
		if (GetObjectMode() == tSelectedFaces)
		{
			vec3_t uv;
			m->GetTexCoord(mTexCoordArrayIndex, uv);
			m->TranslateUVMap(uv[0] - u, uv[1] - v, 0.0f);
		}
#endif
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
	 Print("No UVs selected!") : 
	 Print("Selected UV[%i].  Move with Right Mouse Button.", mTexCoordArrayIndex));
}


index_t FreyjaControl::PickMeshes(const hel::Ray &ray)
{
	vec_t t, best = 9999.0f;
	index_t selected = INDEX_INVALID;

	for (uint32 i = 0, n = freyjaGetMeshCount(); i < n; ++i)
	{
		Mesh *m = freyjaGetMeshClass(i);

		if (!m)
			continue;

		if (m->IntersectPerFace(FreyjaRender::mTestRay, t))
		{
			if (selected == INDEX_INVALID || t < best)
			{
				best = t;
				selected = i;
			}
		}
	}

	return selected;
}


index_t FreyjaControl::PickVertex(const hel::Ray &ray)
{
	index_t selected = INDEX_INVALID;
	Mesh *m = Mesh::GetMesh( GetSelectedMesh() );

	if (m)
	{
		const vec_t radius = FreyjaRender::mVertexPointSize * 0.05f;//0.25f;
		int pick = -1;
		m->IntersectClosestVertex(FreyjaRender::mTestRay, pick, radius);
		selected = (pick > -1) ? pick : INDEX_INVALID;
	}

	return selected;
}
		

index_t FreyjaControl::PickCameraTarget(const hel::Ray &ray)
{
	hel::Vec3 p;
	vec_t t, closest = 9999.0f;
	index_t selected = INDEX_INVALID;

	for (uint32 i = 0, count = freyjaGetCameraCount(); i < count; ++i)
	{
		freyjaGetCameraTarget3fv(i, p.mVec);
		
		if (FreyjaRender::mTestRay.IntersectSphere(p.mVec, 5.0f, t))
		{
			if (selected == INDEX_INVALID || t < closest)
			{
				selected = i;
				closest = t;
			}
		}
	}

	return selected;
}
		

index_t FreyjaControl::PickCameraPos(const hel::Ray &ray)
{
	hel::Vec3 p;
	vec_t t, closest = 9999.0f;
	index_t selected = INDEX_INVALID;

	for (uint32 i = 0, count = freyjaGetCameraCount(); i < count; ++i)
	{
		freyjaGetCameraPos3fv(i, p.mVec);
		
		if (FreyjaRender::mTestRay.IntersectSphere(p.mVec, 5.0f, t))
		{
			if (selected == INDEX_INVALID || t < closest)
			{
				selected = i;
				closest = t;
			}
		}
	}

	return selected;
}
		

index_t FreyjaControl::PickLight(const hel::Ray &ray)
{
	hel::Vec3 xyz;
	vec4_t pos;
	vec_t t, closest = 9999.0f;
	index_t selected = INDEX_INVALID;

	for (uint32 i = 0, count = freyjaGetLightCount(); i < count; ++i)
	{
		freyjaGetLightPosition4v(i, pos);
		xyz = hel::Vec3(pos);
		
		if (FreyjaRender::mTestRay.IntersectSphere(xyz.mVec, 5.0f, t))
		{
			if (selected == INDEX_INVALID || t < closest)
			{
				selected = i;
				closest = t;
			}
		}
	}

	return selected;
}


index_t FreyjaControl::PickFace(const hel::Ray &ray)
{
	index_t selected = INDEX_INVALID;
	Mesh *m = Mesh::GetMesh(GetSelectedMesh());

	// Remeber: New freyja::Mesh picks faces and marks them selected.
	if ( m )
	{
		int pick = -1;

		switch (GetControlScheme())
		{
		case eScheme_UV:
			m->IntersectUVFaces(FreyjaRender::mTestRay, pick, false,
								GetSelectedMaterial());
			break;

		default:
			m->IntersectClosestFace(FreyjaRender::mTestRay, pick);
		}

		selected = (pick < 0) ? INDEX_INVALID : pick;
	}

	return selected;
}


index_t FreyjaControl::PickBone(const hel::Ray &ray)	
{
	const vec_t radius = FreyjaRender::mBonePointSize * 0.1f;
	hel::Vec3 p;
	vec_t t, closest = 9999.0f;
	index_t selected = INDEX_INVALID;

	for (uint32 i = 0, count = freyjaGetBoneCount(); i < count; ++i)
	{
		switch (GetControlScheme())
		{
		case eScheme_Animation:
			freyjaGetBoneWorldPos3fv(i, p.mVec);
			break;

		default:
			freyjaBoneBindTransformVertex(i, p.mVec, 1.0f);
		}

		if (FreyjaRender::mTestRay.IntersectSphere(p.mVec, radius, t))
		{
			if (selected == INDEX_INVALID || t < closest)
			{
				selected = i;
				closest = t;
			}
		}
	}

	return selected;
}


index_t FreyjaControl::PickMesh(const hel::Ray &ray)
{
	vec_t t, best = 9999.0f;
	index_t selected = INDEX_INVALID;

	for (uint32 i = 0, n = freyjaGetMeshCount(); i < n; ++i)
	{
		Mesh *m = freyjaGetMeshClass(i);

		if (!m)
			continue;

		if (m->IntersectPerFace(FreyjaRender::mTestRay, t))
		{
			if (selected == INDEX_INVALID || t < best)
			{
				best = t;
				selected = i;
			}
		}
	}

	return selected;
}
		

//////////////////////////////////////////////////////////////////////////
// "Event" methods for threaded closures and callbacks.
//////////////////////////////////////////////////////////////////////////

void FreyjaControl::AttachMethodListeners()
{
	mMaterial.AttachMethodListeners();

	//Float events
	EvMoveXId = CreateListener1f("eMove_X", &FreyjaControl::EvFloatNop);
	EvMoveYId = CreateListener1f("eMove_Y", &FreyjaControl::EvFloatNop);
	EvMoveZId = CreateListener1f("eMove_Z", &FreyjaControl::EvFloatNop);
	EvRotateXId = CreateListener1f("eRotate_X", &FreyjaControl::EvFloatNop);
	EvRotateYId = CreateListener1f("eRotate_Y", &FreyjaControl::EvFloatNop);
	EvRotateZId = CreateListener1f("eRotate_Z", &FreyjaControl::EvFloatNop);
	EvScaleXId = CreateListener1f("eScale_X", &FreyjaControl::EvScaleX);
	EvScaleYId = CreateListener1f("eScale_Y", &FreyjaControl::EvScaleY);
	EvScaleZId = CreateListener1f("eScale_Z", &FreyjaControl::EvScaleZ);
	CreateListener1f("eZoom", &FreyjaControl::SetZoom);

	// Mode events
	EvModeAutoKeyframeId =
	CreateListener1u("eModeAnim", &FreyjaControl::EvModeAutoKeyframe);
	CreateListener("eModeUV", &FreyjaControl::EvModeUV);
	CreateListener("eModeModel", &FreyjaControl::EvModeModel);
	CreateListener("eModeMaterial", &FreyjaControl::EvModeMaterial);

	// Iterator/Integer events
	CreateListener1u("ePolygonIterator", &FreyjaControl::EvPolygonIterator);
	CreateListener1u("eMeshIterator", &FreyjaControl::EvMeshIterator);
	EvBoneIteratorId = 
	CreateListener1u("eBoneIterator", &FreyjaControl::EvBoneIterator);
	CreateListener1u("eAnimationSlider", &FreyjaControl::EvAnimationSlider);

	eRotateObjectId = 
	CreateListener1u("eRotateObject", &FreyjaControl::EvRotateObject);

	eScaleObjectId = 
	CreateListener1u("eScaleObject", &FreyjaControl::EvScaleObject);

	eMoveObjectId = 
	CreateListener1u("eMoveObject", &FreyjaControl::EvMoveObject);

	eUnselectObjectId = 
	CreateListener1u("eUnselect", &FreyjaControl::EvUnselectObject);

	eSelectObjectId = 
	CreateListener1u("eSelect", &FreyjaControl::EvSelectObject);

	eInfoObjectId = 
	CreateListener1u("eInfoObject", &FreyjaControl::EvInfoObject);

	ePaintObjectId = 
	CreateListener1u("ePaintObject", &FreyjaControl::EvPaintObject);

	eBoxSelectObjectId = 
	CreateListener1u("eSelectionByBox", &FreyjaControl::EvBoxSelectObject);

	eAxisJointId = 
	CreateListener1u("eAxisJoint", &FreyjaControl::EvAxisJoint);

	eSphereJointId = 
	CreateListener1u("eSphereJoint", &FreyjaControl::EvSphereJoint);

	ePointJointId = 
	CreateListener1u("ePointJoint", &FreyjaControl::EvPointJoint);

	// Text events
	CreateListener1s("eSkeletonName", &FreyjaControl::EvSkeletonName);
	EvSetBoneNameId = 
	CreateListener1s("eSetBoneName", &FreyjaControl::EvSetBoneName);
	CreateListener1s("eOpenModel", &FreyjaControl::EvOpenModel);

	// Misc
	CreateListener2s("eSaveModel", &FreyjaControl::EvSaveModel);

	CreateListener("ePolygonSplit", &FreyjaControl::EvPolygonSplit);
	CreateListener("eSetMeshTexture", &FreyjaControl::EvSetMeshTexture);
	CreateListener("eSetFacesMaterial", &FreyjaControl::EvSetFacesMaterial);
	CreateListener("eSetPolygonTexture", &FreyjaControl::EvSetPolygonTexture);

	CreateListener("eInfo", &FreyjaControl::PrintInfo);
	CreateListener("eFullscreen", &FreyjaControl::Fullscreen);

	CreateListener("eVertexCombine", &FreyjaControl::VertexCombine);
	CreateListener("eTexcoordCombine", &FreyjaControl::TexcoordCombine);
	CreateListener("eSetKeyFrame", &FreyjaControl::SetKeyFrame);

	CreateListener("eCloseFile", &FreyjaControl::CloseFile);
	CreateListener("eNewFile", &FreyjaControl::NewFile);
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

	CreateListener("eMeshNew", &FreyjaControl::EvMeshNew);
	CreateListener("eMeshDelete", &FreyjaControl::EvMeshDelete);
	CreateListener("eMeshSelect", &FreyjaControl::EvMeshSelect);

	CreateListener("eScale", &FreyjaControl::EvScale);
	CreateListener("eMove", &FreyjaControl::EvMove);
	CreateListener("eRotate", &FreyjaControl::EvRotate);

	CreateListener("eSelectVerticesByFaces", &FreyjaControl::EvSelectVerticesByFaces);

	CreateListener("eViewportBack", &FreyjaControl::EvViewportBack);
	CreateListener("eViewportBottom", &FreyjaControl::EvViewportBottom);
	CreateListener("eViewportRight", &FreyjaControl::EvViewportRight);
	CreateListener("eViewportFront", &FreyjaControl::EvViewportFront);
	CreateListener("eViewportTop", &FreyjaControl::EvViewportTop);
	CreateListener("eViewportLeft", &FreyjaControl::EvViewportLeft);
	CreateListener("eViewportOrbit", &FreyjaControl::EvViewportOrbit);
	CreateListener("eViewportUV", &FreyjaControl::EvViewportUV);
	CreateListener("eViewportCurve", &FreyjaControl::EvViewportCurve);
	CreateListener("eViewportCamera", &FreyjaControl::EvViewportCamera);
	CreateListener("eViewportMaterial", &FreyjaControl::EvViewportMaterial);

	CreateListener("eTransformScene", &FreyjaControl::EvTransformScene);
	CreateListener("eTransformVertices", &FreyjaControl::EvTransformVertices);
	CreateListener("eTransformVertex", &FreyjaControl::EvTransformVertex);
	CreateListener("eTransformMeshes", &FreyjaControl::EvTransformMeshes);
	CreateListener("eTransformMesh", &FreyjaControl::EvTransformMesh);
	CreateListener("eTransformMetadata", &FreyjaControl::EvTransformMetadata);
	CreateListener("eTransformFaces", &FreyjaControl::EvTransformFaces);
	CreateListener("eTransformFace", &FreyjaControl::EvTransformFace);
	CreateListener("eTransformModel", &FreyjaControl::EvTransformModel);
	CreateListener("eTransformSkeleton", &FreyjaControl::EvTransformSkeleton);
	CreateListener("eTransformBone", &FreyjaControl::EvTransformBone);
	CreateListener("eTransformLight", &FreyjaControl::EvTransformLight);
	CreateListener("eTransformCamera", &FreyjaControl::EvTransformCamera);


	CreateListener("eMetaToXML", &FreyjaControl::EvSerializeMetadata);
	CreateListener("eXMLToMeta", &FreyjaControl::EvUnserializeMetadata);

	CreateListener("eMeshToXML", &FreyjaControl::EvSerializeMesh);
	CreateListener("eXMLToMesh", &FreyjaControl::EvUnserializeMesh);

	CreateListener("eBonesToXML", &FreyjaControl::EvSerializeBones);
	CreateListener("eXMLToBones", &FreyjaControl::EvUnserializeBones);

	CreateListener("eMeshRepack", &FreyjaControl::EvMeshRepack);

	CreateListener1u("eRecentFiles", &FreyjaControl::EvRecentFiles);
	CreateListener1u("eRecentMeshXML", &FreyjaControl::EvRecentMeshXML);
	CreateListener1u("eRecentMetadataXML", &FreyjaControl::EvRecentMetadataXML);
	CreateListener1u("eRecentSkeletonXML", &FreyjaControl::EvRecentSkeletonXML);
	CreateListener1u("eRecentKeyframe", &FreyjaControl::EvRecentKeyframe);
	CreateListener1u("eRecentLua", &FreyjaControl::EvRecentLua);
	CreateListener1u("eRecentPython", &FreyjaControl::EvRecentPython);


	CreateListener("ePaintWeight", &FreyjaControl::EvPaintWeight);
	CreateListener("ePaintUnweight", &FreyjaControl::EvPaintUnweight);
	CreateListener("ePaintSelect", &FreyjaControl::EvPaintSelect);
	CreateListener("ePaintUnselect", &FreyjaControl::EvPaintUnselect);
	CreateListener("ePaintMaterial", &FreyjaControl::EvPaintMaterial);
	CreateListener("ePaintHeight", &FreyjaControl::EvPaintHeight);
	CreateListener("ePaintDmap", &FreyjaControl::EvPaintDmap);

	CreateListener("eLoadLuaScript", &FreyjaControl::EvLoadLuaScript);
	CreateListener("eLoadPythonScript", &FreyjaControl::EvLoadPythonScript);

	CreateListener("eLaunchBugTracker", &FreyjaControl::EvLaunchBugs);
	CreateListener("eLaunchOnlineHelp", &FreyjaControl::EvLaunchFourms);
	CreateListener("eLaunchUpdate", &FreyjaControl::EvLaunchUpdate);

	CreateListener("eMeshSubDivLoop", &FreyjaControl::EvMeshSubDiv);

	CreateListener("eSelectAll", &FreyjaControl::EvSelectAll);
	CreateListener("eUnSelectAll", &FreyjaControl::EvUnselectAll);
}


int FreyjaControl::CreateListener(const char *name, MethodPtr ptr)
{
	MethodDelegate *d = new MethodDelegateArg0<FreyjaControl>(mInstance, ptr);
	ResourceEventDelegate::add(name, d);
	return ResourceEvent::GetResourceIdBySymbol(name);
}


int FreyjaControl::CreateListener(const char *name, MethodPtrB ptr)
{
	MethodDelegate *d = new MethodDelegateArg0<FreyjaControl, bool>(mInstance, ptr);
	ResourceEventDelegate::add(name, d);
	return ResourceEvent::GetResourceIdBySymbol(name);
}


int FreyjaControl::CreateListener1u(const char *name, MethodPtr1u ptr)
{
	MethodDelegate *d = new MethodDelegateArg1<FreyjaControl, unsigned int>(mInstance, ptr);
	ResourceEventDelegate::add(name, d);
	return ResourceEvent::GetResourceIdBySymbol(name);
}


int FreyjaControl::CreateListener1f(const char *name, MethodPtr1f ptr)
{
	MethodDelegate *d = new MethodDelegateArg1<FreyjaControl, float>(mInstance, ptr);
	ResourceEventDelegate::add(name, d);
	return ResourceEvent::GetResourceIdBySymbol(name);
}


int FreyjaControl::CreateListener1s(const char *name, MethodPtr1s ptr)
{
	MethodDelegate *d = new MethodDelegateArg1<FreyjaControl, char*>(mInstance, ptr);
	ResourceEventDelegate::add(name, d);
	return ResourceEvent::GetResourceIdBySymbol(name);
}


int FreyjaControl::CreateListener2s(const char *name, MethodPtr2s ptr)
{
	MethodDelegate *d = new MethodDelegateArg2<FreyjaControl, char*, char*>(mInstance, ptr);
	ResourceEventDelegate::add(name, d);
	return ResourceEvent::GetResourceIdBySymbol(name);
}


void FreyjaControl::EvLoadLuaScript()
{
	char *filename =
	mgtk_filechooser_blocking("freyja - Open Script...", 
							  mRecentLua.GetPath(), 0,
							  "Lua script (*.lua)", "*.lua");

	if (filename)
	{
		freyjaLuaScript1s(filename);
		mRecentLua.AddFilename(filename);
	}

	mgtk_filechooser_blocking_free(filename);
}


void FreyjaControl::EvLoadPythonScript()
{

	char *filename =
	mgtk_filechooser_blocking("freyja - Open Script...", 
							  mRecentPython.GetPath(), 0,
							  "Python script (*.py)", "*.py");

	if (filename)
	{
		freyjaPython1s(filename, "DefaultAction", "none");
		mRecentPython.AddFilename(filename);
	}

	mgtk_filechooser_blocking_free(filename);
}



void FreyjaControl::EvSerializeMesh()
{
#if TINYXML_FOUND
	Mesh *m = Mesh::GetMesh( GetSelectedMesh() );

	if (!m)
		return;

	char *filename =
	mgtk_filechooser_blocking("freyja - Save Selected Mesh...", 
							  mRecentMesh.GetPath(), 1,
							  "Mesh XML (*.xml)", "*.xml");

	if (!filename)
		return;

	TiXmlDocument doc;
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	TiXmlElement *container = new TiXmlElement("freyja");
	container->SetAttribute("version", VERSION);
	container->SetAttribute("build-date", __DATE__);
	container->SetAttribute("file-version", 1);
	doc.LinkEndChild(container);

	for (uint32 i = 0, n = freyjaGetMaterialCount(); i < n; ++i)
	{
		Material *mat = freyjaGetMaterialClass(i);

		if (mat && i) // skip 'boring material'
			mat->Serialize(container);
	}

	m->Repack();

	if (filename &&  m->Serialize(container) && doc.SaveFile(filename))
	{
		mRecentMesh.AddFilename(filename);
		Print("Mesh '%s' Saved", filename);
	}
	else if (filename)
	{
		Print("Mesh '%s' failed to save.", filename);
	}

	mgtk_filechooser_blocking_free(filename);
#endif
}


void FreyjaControl::EvMeshSubDiv()
{
	Mesh *m = Mesh::GetMesh( GetSelectedMesh() );
	if (m) m->SubDivLoop();

	Print("Non-smoothed loop subdiv requested.");
}


void FreyjaControl::EvRecentKeyframe(uint32 value)
{
	Print("FIXME: Not fully implemented %s:%i", __FILE__, __LINE__);
	//mRecentKeyframe.GetFilename(value);
}


void FreyjaControl::EvRecentLua(uint32 value)
{
	freyjaLuaScript1s( mRecentLua.GetFilename(value) );
}


void FreyjaControl::EvRecentPython(uint32 value)
{
	freyjaPython1s( mRecentPython.GetFilename(value), "DefaultAction", "none" );
}


void FreyjaControl::EvUnserializeMesh()
{
#if TINYXML_FOUND
	char *filename =
	mgtk_filechooser_blocking("freyja - Open Selected Mesh...", 
							  mRecentMesh.GetPath(), 0,
							  "Mesh XML (*.xml)", "*.xml");

	if ( UnserializeMesh(filename) )
	{
		mRecentMesh.SetPathByFilename(filename);
		Print("Loaded mesh snippet '%s'", filename);
	}

	mgtk_filechooser_blocking_free(filename);
#endif
}


void FreyjaControl::EvSerializeMetadata()
{
#if TINYXML_FOUND
	Metadata* metadata = Metadata::GetObjectByUid( GetSelectedMetadata() );

	if (!metadata)
		return;

	char *filename =
	mgtk_filechooser_blocking("freyja - Save Selected Metadata...", 
							  mRecentMetadata.GetPath(), 1,
							  "Metadata Object (*.xml)", "*.xml");

	if (!filename)
		return;

	TiXmlDocument doc;
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	TiXmlElement *container = new TiXmlElement("freyja");
	container->SetAttribute("version", VERSION);
	container->SetAttribute("build-date", __DATE__);
	container->SetAttribute("file-version", 1);
	doc.LinkEndChild(container);

	if (filename &&  metadata->Serialize(container) && doc.SaveFile(filename) )
	{
		mRecentMetadata.AddFilename(filename);
		Print("Saved metadata '%s'.", filename);
	}
	else if (filename)
	{
		Print("Failed to save metadata '%s'.", filename);
	}

	mgtk_filechooser_blocking_free(filename);
#endif
}


void FreyjaControl::EvUnserializeMetadata()
{
#if TINYXML_FOUND

	Metadata *metadata = new Metadata();

	if ( !metadata->AddToPool() )
	{
		delete metadata;
		Print("Error creating new metadata!");
	}
	else
	{
		Print("New metadata object #%i created.", metadata->GetUid() );
	}

	char *filename =
	mgtk_filechooser_blocking("freyja - Open Metadata object...", 
							  FreyjaControl::GetInstance()->GetRecentMetadata().GetPath(), 0,
							  "Metadata Object (*.xml)", "*.xml");

	// Canceled
	if ( !filename )
		return;

	// If you got here push the filename into a recently used menu.
	if ( metadata->Unserialize( filename ) )
	{
		FreyjaControl::GetInstance()->GetRecentMetadata().AddFilename( filename );
	}
	else
	{
		freyja_print("Error loading metadata xml.\n");
	}

	mgtk_filechooser_blocking_free( filename );

	FreyjaControl::GetInstance()->Dirty();
#endif
}


void FreyjaControl::EvViewportBack()
{
	Print("Back view");
	SetSelectedView(PLANE_BACK);
	if (mRender->GetFlags() & FreyjaRender::fViewports)
		mRender->mViewports[mSelectedViewport].plane = PLANE_BACK;
	freyja_event_gl_refresh();
}


void FreyjaControl::EvViewportBottom()
{
	Print("Bottom view");  
	SetSelectedView(PLANE_BOTTOM);
	if (mRender->GetFlags() & FreyjaRender::fViewports)
		mRender->mViewports[mSelectedViewport].plane = PLANE_BOTTOM;
	freyja_event_gl_refresh();
}


void FreyjaControl::EvViewportRight()
{
	Print("Right view");
	SetSelectedView(PLANE_RIGHT);
	if (mRender->GetFlags() & FreyjaRender::fViewports)
		mRender->mViewports[mSelectedViewport].plane = PLANE_RIGHT;
	freyja_event_gl_refresh();
}


void FreyjaControl::EvViewportFront()
{
	Print("Front view");
	SetSelectedView(PLANE_FRONT);
	if (mRender->GetFlags() & FreyjaRender::fViewports)
		mRender->mViewports[mSelectedViewport].plane = PLANE_FRONT;
	freyja_event_gl_refresh();
}


void FreyjaControl::EvViewportTop()
{
	Print("Top view");  
	SetSelectedView(PLANE_TOP);
	if (mRender->GetFlags() & FreyjaRender::fViewports)
		mRender->mViewports[mSelectedViewport].plane = PLANE_TOP;
	freyja_event_gl_refresh();
}


void FreyjaControl::EvViewportLeft()
{
	Print("Left view");
	SetSelectedView(PLANE_LEFT);
	if (mRender->GetFlags() & FreyjaRender::fViewports)
		mRender->mViewports[mSelectedViewport].plane = PLANE_LEFT;
	freyja_event_gl_refresh();
}


void FreyjaControl::EvViewportOrbit()
{
	Print("Orbital view");
	SetSelectedView(PLANE_FREE);
	if (mRender->GetFlags() & FreyjaRender::fViewports)
		mRender->mViewports[mSelectedViewport].plane = PLANE_FREE;
	freyja_event_gl_refresh();
}


void FreyjaControl::EvViewportUV()
{
	Print("UV Editor view");
	SetSelectedView(DRAW_UV);
	if (mRender->GetFlags() & FreyjaRender::fViewports)
		mRender->mViewports[mSelectedViewport].plane = DRAW_UV;
	freyja_event_gl_refresh();
}


void FreyjaControl::EvViewportCurve()
{
	Print("Curve editor view");
	SetSelectedView(DRAW_CURVE);
	if (mRender->GetFlags() & FreyjaRender::fViewports)
		mRender->mViewports[mSelectedViewport].plane = DRAW_CURVE;
	freyja_event_gl_refresh();
}


void FreyjaControl::EvViewportCamera()
{
	Print("Curve editor view");
	SetSelectedView(DRAW_CURVE);
	if (mRender->GetFlags() & FreyjaRender::fViewports)
	{
		mRender->mViewports[mSelectedViewport].plane = DRAW_CAMERA;
		mRender->mViewports[mSelectedViewport].camera = GetSelectedCamera();
	}
	freyja_event_gl_refresh();
}


void FreyjaControl::EvViewportMaterial()
{
	Print("Material preview");
	SetSelectedView(DRAW_MATERIAL);
	if (mRender->GetFlags() & FreyjaRender::fViewports)
		mRender->mViewports[mSelectedViewport].plane = DRAW_MATERIAL;
	freyja_event_gl_refresh();
}


void FreyjaControl::EvMove()
{
	mToken = true;

	Transform(mObjectMode, fTranslate,
			  freyja_event_get_float(EvMoveXId),
			  freyja_event_get_float(EvMoveYId),
			  freyja_event_get_float(EvMoveZId));

	freyja_event_set_float(EvMoveXId, 0.0f);
	freyja_event_set_float(EvMoveYId, 0.0f);
	freyja_event_set_float(EvMoveZId, 0.0f);
	freyja_event_gl_refresh();
}


void FreyjaControl::EvRotate()
{
	mToken = true;

	hel::Vec3 rot(freyja_event_get_float(EvRotateXId),
				  freyja_event_get_float(EvRotateYId),
				  freyja_event_get_float(EvRotateZId));

	switch (mObjectMode)
	{
	case tBone:
#warning "FIXME box transform for rotate is broken."
		break;

	default:
		;
	}

	Transform(mObjectMode, fRotate, rot.mX, rot.mY, rot.mZ);
		
	freyja_event_set_float(EvRotateXId, 0.0f);
	freyja_event_set_float(EvRotateYId, 0.0f);
	freyja_event_set_float(EvRotateZId, 0.0f);
	freyja_event_gl_refresh();
}


void FreyjaControl::EvScale()
{
	mToken = true;
	Transform(mObjectMode, fScale,
			  freyja_event_get_float(EvScaleXId),
			  freyja_event_get_float(EvScaleYId),
			  freyja_event_get_float(EvScaleZId));
		
	freyja_event_set_float(EvScaleXId, 1.0f);
	freyja_event_set_float(EvScaleYId, 1.0f);
	freyja_event_set_float(EvScaleZId, 1.0f);
	freyja_event_gl_refresh();
}


void FreyjaControl::EvModeUV()
{
	mRender->SetViewMode(VIEWMODE_TEXTURE_EDIT);
	freyja_event_gl_refresh();
	Print("UV Editor Scheme");
	SetControlScheme(eScheme_UV);
}


void FreyjaControl::EvModeModel()
{
	// Radio button like func for multiple widgets on same event
	mgtk_toggle_value_set(EvModeAutoKeyframeId, 0);

	mRender->SetViewMode(VIEWMODE_MODEL_EDIT);
	freyja_event_gl_refresh();
	Print("Model Editor Scheme");
	SetControlScheme(eScheme_Model);
}


void FreyjaControl::EvModeMaterial()
{
	mRender->SetViewMode(VIEWMODE_MATERIAL_EDIT);
	freyja_event_gl_refresh();
	Print("Material Editor Scheme");
	SetControlScheme(eScheme_Material);
}

void FreyjaControl::EvMeshNew()
{
	mObjectMode = tMesh;
	CreateObject();
	Dirty();
}


void FreyjaControl::EvMeshDelete()
{
	mObjectMode = tMesh;
	DeleteSelectedObject();
	Dirty();
}


void FreyjaControl::EvMeshSelect()
{
	mObjectMode = tMesh;
	mEventMode = aSelect;
	Print("Select object mesh...");
}


void FreyjaControl::EvRecentMetadataXML(uint32 value) 
{
	Metadata *m = new Metadata();
	m->AddToPool();
	m->Unserialize( mRecentMetadata.GetFilename(value) ); 
}


//////////////////////////////////////////////////////////////////////////
// Non-class methods, callbacks, and wrappers
//////////////////////////////////////////////////////////////////////////

void freyjaQueryCallbackHandler(unsigned int size, freyja_query_t *array)
{
	QueryDialog d;
	d.mName = "freyjaQueryCallbackHandler"; 
	d.mDialogIcon = "gtk-question"; 
	d.mInformationMessage = "Query Dialog"; 
	d.mCancelIcon = "gtk-cancel"; 
	d.mCancelText = "Cancel"; 
	d.mAcceptIcon = "gtk-ok";
	d.mAcceptText = "Answer"; 

	bool any = false;

	for (uint32 i = 0; i < size; ++i)
	{
		const char *type = array[i].type;
		const char *symbol = array[i].symbol;
		void *data = array[i].ptr;

		String s = type;

		if (s == "int")
		{
			s += " ";
			s += symbol;
			QueryDialogValue<int> v(symbol, s.c_str(), *(int *)data);
			d.mInts.push_back(v);
			any = true;
		}
		else if (s == "float")
		{
			s += " ";
			s += symbol;
			QueryDialogValue<float> v(symbol, s.c_str(), *(float *)data);
			d.mFloats.push_back(v);
			any = true;
		}
		else
		{
			FREYJA_INFOMSG(false, "FIXME: Unhandled query '%s' '%s' %p\n", 
						   type, symbol, data);
		}
	}


	// If the user chooses to set values, set them.
	if ( any && d.Execute() )
	{
		for (uint32 i = 0; i < size; ++i)
		{
			String s = array[i].type;

			if (s == "int")
			{
				(*(int *)array[i].ptr) = d.GetInt( array[i].symbol );
			}
			else if (s == "float")
			{
				(*(float *)array[i].ptr) = d.GetFloat( array[i].symbol );
			}
			else
			{
				// Not handled
			}
		}	
	}
}


int load_texture(const char *filename)
{
	int id = 0;
	FreyjaControl::GetInstance()->LoadTexture(filename, id);
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


void ePluginImport(ResourceEvent *e)
{
	if (!e || !e->getName())
		return;

	mstl::String s = e->getName();

	s.Replace('|', 0);

	int idx = atoi(s.c_str());

	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(idx);
			
	if (plugin)
	{
		QueryDialog d;
		d.mName = e->getName(); 
		d.mDialogIcon = "gtk-question"; 
		d.mInformationMessage.Set("Plugin Import: %s", 
								  plugin->mName.c_str()); 
		d.mCancelIcon = "gtk-cancel"; 
		d.mCancelText = "Cancel"; 
		d.mAcceptIcon = "gtk-ok";
		d.mAcceptText = "Import"; 

		uint32 i;
		foreach (plugin->mArgs, i)
		{
			if (plugin->mArgs[i].GetStringType() == "float")
			{
				mstl::String symbol = plugin->mArgs[i].GetName();
				mstl::String question = plugin->mArgs[i].GetName();
				float value = 0.0f;
				value = plugin->GetFloatArg(plugin->mArgs[i].GetName());

				QueryDialogValue<float> v(symbol.c_str(), question.c_str(), value);
				d.mFloats.push_back(v);	
			}
			else if (plugin->mArgs[i].GetStringType() == "int")
			{
				mstl::String symbol = plugin->mArgs[i].GetName();
				mstl::String question = plugin->mArgs[i].GetName();
				int value = 0;
				value = plugin->GetIntArg(plugin->mArgs[i].GetName());

				QueryDialogValue<int> v(symbol.c_str(), question.c_str(), value);
				d.mInts.push_back(v);	
			}
		}

		if (!plugin->mArgs.size() || d.Execute())
		{
			// Update plugin settings from dialog input.
			foreach (plugin->mArgs, i)
			{
				if (plugin->mArgs[i].GetStringType() == "float")
				{
					float value = 
					d.GetFloat(plugin->mArgs[i].GetName());
					mstl::String s;
					s.Set("%f", value);
					plugin->mArgs[i].SetValue(s.c_str());
				}
				else if (plugin->mArgs[i].GetStringType() == "int")
				{
					int value = 
					d.GetInt(plugin->mArgs[i].GetName());
					mstl::String s;
					s.Set("%i", value);
					plugin->mArgs[i].SetValue(s.c_str());
				}
			}
				
			// File dialog here with preset for export type.
			mstl::String title;
			title.Set("Importing %s - Freyja", plugin->mName.c_str());
			char *filename =
			mgtk_filechooser_blocking(title.c_str(),  
									  freyja_rc_map_string("/").c_str(), 
									  0,
									  plugin->mDescription.c_str(),
									  plugin->mExtention.c_str());

			freyja_print("! Importing: '%s'\n", filename);

			if (!freyjaImportModelByModule(filename, 
										   plugin->mFilename.c_str()))
			{				   
				if (plugin->mImportFlags & FREYJA_PLUGIN_PAK_VFS)
				{
					// FIXME: Old school plugin callback
					uint32 id = ResourceEvent::GetResourceIdBySymbol("eDialogPakReader");
					ResourceEvent::listen(id);
					id = ResourceEvent::GetResourceIdBySymbol("ePakReaderMenuUpdate");
					ResourceEvent::listen(id);

					FREYJA_INFOMSG(0, "Click on the Plugins Tab and expand PakBrowser to browse the pak file.\nClick the Reset VFS button if the vfs doesn't automaticaly load for you.\nIf the file doesn't open automatically from the pak\nit will likely still be written to /tmp/utpak \nor ./utpak to load from another plugin.");
				}
				else
				{
					FreyjaControl::GetInstance()->RecordSavedModel(filename);
					freyja_print("! Imported: '%s'\n", filename);

					// Update skeletal UI
					if (freyjaGetCurrentSkeleton() == INDEX_INVALID &&
						freyjaGetSkeletonCount() > 0)
					{
						freyjaCurrentSkeleton(0);
					}

					FreyjaControl::GetInstance()->UpdateSkeletalUI();
				}
			}
			else
			{
				freyja_print("! Failed to import: '%s'\n", filename);
			}

			mgtk_filechooser_blocking_free(filename);
		}
	}
}


void ePluginExport(ResourceEvent *e)
{
	if (!e || !e->getName())
		return;

	mstl::String s = e->getName();

	s.Replace('|', 0);

	int idx = atoi(s.c_str());

	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(idx);
			
	if (plugin)
	{
		QueryDialog d;
		d.mName = e->getName(); 
		d.mDialogIcon = "gtk-question"; 
		d.mInformationMessage.Set("Plugin Export: %s", 
								  plugin->mName.c_str()); 
		d.mCancelIcon = "gtk-cancel"; 
		d.mCancelText = "Cancel"; 
		d.mAcceptIcon = "gtk-ok"; 
		d.mAcceptText = "Export"; 

		uint32 i;
		foreach (plugin->mArgs, i)
		{
			if (plugin->mArgs[i].GetStringType() == "float")
			{
				mstl::String symbol = plugin->mArgs[i].GetName();
				mstl::String question = plugin->mArgs[i].GetName();
				float value = 0.0f;
				value = plugin->GetFloatArg(plugin->mArgs[i].GetName());

				QueryDialogValue<float> v(symbol.c_str(), question.c_str(), value);
				d.mFloats.push_back(v);	
			}
			else if (plugin->mArgs[i].GetStringType() == "int")
			{
				mstl::String symbol = plugin->mArgs[i].GetName();
				mstl::String question = plugin->mArgs[i].GetName();
				int value = 0;
				value = plugin->GetIntArg(plugin->mArgs[i].GetName());

				QueryDialogValue<int> v(symbol.c_str(), question.c_str(), value);
				d.mInts.push_back(v);	
			}
		}

		if (!plugin->mArgs.size() || d.Execute())
		{
			// Update plugin settings from dialog input.
			foreach (plugin->mArgs, i)
			{
				if (plugin->mArgs[i].GetStringType() == "float")
				{
					float value = 
					d.GetFloat(plugin->mArgs[i].GetName());
					mstl::String s;
					s.Set("%f", value);
					plugin->mArgs[i].SetValue(s.c_str());
				}
				else if (plugin->mArgs[i].GetStringType() == "int")
				{
					int value = 
					d.GetInt(plugin->mArgs[i].GetName());
					mstl::String s;
					s.Set("%i", value);
					plugin->mArgs[i].SetValue(s.c_str());
				}
			}
				
			// File dialog here with preset for export type.
			mstl::String title;
			title.Set("Exporting %s - Freyja", plugin->mName.c_str());
			char *filename =
			mgtk_filechooser_blocking(title.c_str(),  
									  freyja_rc_map_string("/").c_str(), 
									  1,
									  plugin->mDescription.c_str(),
									  plugin->mExtention.c_str());

			{
				String ext = filename;

				int pos = ext.find_last_of('.');

				if (pos == String::npos)
				{
					
				}
			}

			freyja_print("! Exporting: '%s'\n", filename);
				
			if (!freyjaExportModelByModule(filename, 
										   plugin->mFilename.c_str()))
			{				   
				FreyjaControl::GetInstance()->RecordSavedModel(filename);
				freyja_print("! Exported: '%s'\n", filename);
			}
			else
			{
				freyja_print("! Failed to export: '%s'\n", filename);
			}

			mgtk_filechooser_blocking_free(filename);
		}
	}
}


// FIXME: Rewrite this to be Skeleton based!!  Allow for bones outside of root
//        and include Skeleton in tree
//
//        Also have tree root be skeleton name, so you can do multiple skeletons
//        in the widget if needed later ala scene graphs
mgtk_tree_t *freyja_generate_skeletal_ui(uint32 skelIndex, uint32 rootIndex, 
										 mgtk_tree_t *tree)
{
	if (!freyjaIsBoneAllocated(rootIndex))
	{
		freyja_print("!generateSkeletalUI(): No Skeleton root given.\n");
		return 0x0;
	}

	uint32 rootChildCount = freyjaGetBoneChildCount(rootIndex);
	const char *rootName = freyjaGetBoneNameString(rootIndex);
	uint32 rootSkelBID = rootIndex;//freyjaGetBoneSkeletalBoneIndex(rootIndex);

	if (rootChildCount > freyjaGetBoneCount())
	{
		FREYJA_ASSERTMSG(0, "root %i '%s'\nchildren %i > %i\nInvalid bone indices predicted.  Child count exceeds maximum bone count.",
						 rootIndex, rootName,
						 rootChildCount, freyjaGetBoneCount());
		return 0x0;
	}

	if (tree == 0x0)
	{
		tree = new mgtk_tree_t;
		snprintf(tree->label, 63, "root");	
		tree->label[63] = '\0';
		tree->parent = 0x0;
	}
	else
	{
		snprintf(tree->label, 63, "bone%03i", rootSkelBID);
		tree->label[63] = '\0';
	}

	if (rootName[0])
	{
		snprintf(tree->label, 63, "%s", rootName);
		tree->label[63] = '\0';
	}

	tree->id = rootIndex;
	tree->numChildren = rootChildCount;
	tree->children = 0x0;

#if DEBUG_BONE_LOAD
	printf("-- %s : %i/%i children\n",  
		   tree->label, tree->numChildren, rootChildCount);
#endif

	if (tree->numChildren == 0)
		return tree->parent;

	tree->children = new mgtk_tree_t[tree->numChildren+1];

	for (uint32 count = 0, i = 0; i < rootChildCount; ++i)
	{
		uint32 boneChild = freyjaGetBoneChild(rootIndex, i);

		if (freyjaIsBoneAllocated(boneChild))
		{
			tree->children[count].parent = tree;
			freyja_generate_skeletal_ui(skelIndex, boneChild, &tree->children[count++]);
		}
	}

	return (tree->parent) ? tree->parent : tree;
}





