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

#include <assert.h>
#include <stdlib.h> 
#include <stdio.h> 
#include <math.h> 
#include <stdarg.h> 
#ifndef WIN32
#   include <libgen.h> 
#endif
#include <string.h>
#include <mgtk/ResourceEvent.h>
#include <freyja/FreyjaFileReader.h>
#include <freyja/FreyjaFileWriter.h>
#include <freyja/FreyjaPluginABI.h>
#include <freyja/Mesh.h>
#include <freyja/MeshABI.h>
#include <hel/math.h>

#include "freyja_events.h"
#include "Freyja3dCursor.h"
#include "FreyjaControl.h"

using namespace mstl;

using namespace freyja;

#define DEBUG_VIEWPORT_MOUSE 0

void event_register_control(FreyjaControl *c);
void mgtk_event_dialog_visible_set(int dialog, int visible);
extern Freyja3dCursor gFreyjaCursor;

////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

FreyjaControl::FreyjaControl(Resource *r)
{
	unsigned int width = 740;
	unsigned int height = 560;

	mResource = r;

	/* Hook up the backend and user interface */
	mModel = new FreyjaModel();
	mRender = new FreyjaRender();
	mRender->setSceneData(mModel);

	if (!mModel || !mRender)
	{
		freyja_print("FreyjaControl::FreyjaControl> Assertion failure");
	
		if (!mRender)
			freyja_print("FreyjaControl::FreyjaControl> FreyjaRender = 0x0");

		if (!mModel)
			freyja_print("FreyjaControl::FreyjaControl> FreyjaModel = 0x0");

		printf("Please read ~/.freyja/Freyja.log for errors.\n");
		exit(-1);
	}
		
	event_register_control(this);

	/* Build the user interface from lisp, and load user preferences */
	loadResource();

	/* Set some basic defaults */
	mEditorMode = MODEL_EDIT_MODE;
	mEventMode = modeNone;
	mTransformMode = FreyjaModel::TransformScene;
	mLastEvent = EVENT_MISC;
	mLastCommand = eSelect;
	setZoom(1.0f);
	mFullScreen = false;
	mFileDialogMode = FREYJA_MODE_LOAD_MODEL;

	/* Mongoose 2002.02.23, Tell renderer to start up */
	mRender->initContext(width, height, true);
	mRender->resizeContext(width, height);

	mModel->initTexture();

	// handle loaded from system call
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

	if (mModel)
		delete mModel;
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

float FreyjaControl::getZoom()
{
	return mRender->getZoom();
}


void FreyjaControl::takeScreenshot(const char *filename, 
								   uint32 width, uint32 height)
{
	mModel->mTexture.glScreenShot(filename, width, height);
}



////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

bool FreyjaControl::handleRecentFile(unsigned int value)
{
	if (value < mRecentFiles.size())
	{
		mFileDialogMode = FREYJA_MODE_LOAD_MODEL;
		handleFilename(mRecentFiles[value]);
		return true;
	}

	return false;
}


void FreyjaControl::updateDisplay()
{
	if (mRender)
	{
		mRender->display();
	}
	else
	{
		freyja_print("!updateDisplay> Call to NULL Renderer\n");
	}
}


void FreyjaControl::resizeDisplay(unsigned int width, unsigned int height)
{
	if (mRender)
	{
		mRender->resizeContext(width, height);
	}
	else
	{
		freyja_print("!resizeDisplay> Call to NULL Renderer\n");
	}
}

unsigned int gRecentFileLimit = 7; // This is becoming a plugin soon

void FreyjaControl::addRecentFilename(const char *filename)
{
	unsigned int i, l, n;
	char *insert, *swap, *old;
	bool found = false;


	if (!filename || !filename[0] || !FreyjaFileReader::doesFileExist(filename))
	{
		return;
	}

	for (i = mRecentFiles.begin(); i < mRecentFiles.end(); ++i)
	{
		if (strcmp(filename, mRecentFiles[i]) == 0)
		{
			found = true;
			break;
		}
	}


	/* 'Boost' this file to top slot, push others down one */
	if (found)
	{
		swap = mRecentFiles[0];
		n = mRecentFiles.end();
		l = i;

		// Already at top, no change to menu
		if (l == 0)
			return;

		mRecentFiles.assign(0, mRecentFiles[i]);

		for (i = 1; i < n; ++i)
		{
			if (i > l)
			{
				break;
			}

			old = mRecentFiles[i];
			mRecentFiles.assign(i, swap);
			swap = old;
		}
	}
	else  /* Add new file to top slot, push others down one */
	{
		l = strlen(filename);
		insert = new char[l+1];
		strncpy(insert, filename, l);
		insert[l] = 0;

		// Bubble up hack
		if (mRecentFiles.end() >= gRecentFileLimit)
		{
			swap = insert;
			n = mRecentFiles.end();
			for (i = mRecentFiles.begin(); i < n; ++i)
			{				
				old = mRecentFiles[i];
				mRecentFiles.assign(i, swap);
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

	n = mRecentFiles.end();

	for (i = mRecentFiles.begin(); i < n; ++i)
	{
		mgtk_append_item_to_menu2i(menuId, mRecentFiles[i], menuId, i);
	}


	/* Save recent_files to disk */
	FreyjaFileWriter w;
	char *filename2 = freyja_rc_map("recent_files");

	if (w.openFile(filename2))
	{
		for (i = mRecentFiles.begin(); i < mRecentFiles.end(); ++i)
		{
			swap = mRecentFiles[i];

			if (swap && swap[0])
			{
				w.print("%s\n", swap);
			}
		}

		w.closeFile();
	}

	if (filename2)
	{
		delete [] filename2;
	}
}


void FreyjaControl::setZoom(float zoom)
{
	assert(zoom > 0.0f);

	freyja_event_notify_observer1f(eZoom, zoom);

	mRender->setZoom(zoom);
	freyja_print("Zoom set to %f", mRender->getZoom());
}


bool FreyjaControl::event(int event, unsigned int value)
{
	vec_t x, y, z;


	if (ResourceEvent::listen(event - 10000 /*ePluginEventBase*/, value))
		return true;

	switch (event)
	{
	case 503:
		if (value != mModel->getCurrentTextureIndex())
		{
			mModel->setCurrentTextureIndex(value);
			freyja_event_gl_refresh();
		}
		break;

	case ePolygonSize:
		mModel->setCurrentPolygonEdgeCount(value);
		freyja_print("Polygons creation using %i sides",
					 mModel->getCurrentPolygonEdgeCount());
		break;


	case eAnimationSlider: // FIXME: Wrapping and bounds 
		if (value != mModel->getCurrentAnimationFrame())
		{
			freyja_event_set_range(event, value, 0, mModel->getAnimationFrameCount(mModel->getCurrentAnimation()));
			mModel->SetCurrentKeyFrame(value);	
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

			mModel->setCurrentBone(value);

			if (value == mModel->getCurrentBone() &&
				mModel->isCurrentBoneAllocated())
			{
				/* Mongoose 2002.08.31, Update spin buttons dependent 
				 * on this one */
				mModel->getBoneRotation(&x, &y, &z);
				freyja_event_set_float(520, x);
				freyja_event_set_float(521, y);
				freyja_event_set_float(522, z);
				
				mModel->getBoneTranslation(&x, &y, &z);
				freyja_event_set_float(510, x);
				freyja_event_set_float(511, y);
				freyja_event_set_float(512, z);
				freyja_event_gl_refresh();
				freyja_print("Selecting bone[%i] ...", value);

				/* Update any bone name listeners, 
				 * 1. Dup string to avoid evil widgets that want to mutate it
				 * 2. Diable event hook up in case of event loop */
				extern void mgtk_textentry_value_set(int event, const char *s);
				strncpy(dupname, mModel->getNameBone(value), 64);
				mModel->setFlags(FreyjaModel::fDontUpdateBoneName, 1);
				mgtk_textentry_value_set(eSetCurrentBoneName, dupname);
				mModel->setFlags(FreyjaModel::fDontUpdateBoneName, 0);
			}
		}
		break;


	case ePolygonIterator:
		if (!freyja_event_set_range(event, value, 0, freyjaGetCount(FREYJA_POLYGON)))
		{
			mModel->setCurrentPolygon(value);			
			freyja_event_gl_refresh();
			freyja_print("Selecting polygon[%i] ...", value);
		}
		break;


	case eMeshIterator:
		if (!freyja_event_set_range(event, value, 0, freyjaGetCount(FREYJA_MESH)))
		{
			mModel->setCurrentMesh(value);			
			freyja_event_gl_refresh();
			freyja_print("Selecting mesh[%i] ...", value);
		}
		break;


	case eGroupIterator:
		if (!freyja_event_set_range(event, value, 0, freyjaGetCount(FREYJA_VERTEX_GROUP)))
		{
			mModel->setCurrentGroup(value);
			freyja_event_gl_refresh();
			freyja_print("Selecting vertexgroup[%i] ...", value);
		}
		break;


	case eSelectLight:
		freyja_print("Light editing not backported");
		break;


	case eSetMaterialTexture:
		mModel->setCurrentTextureIndex(value);
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

	default:
		freyja_print("!Unhandled { event = %d, value = %u }", event, value);
		return false;
	}

	return true;
}


bool FreyjaControl::event(int event, vec_t value)
{
	vec4_t color, pos;
	vec_t x, y, z;


	if (ResourceEvent::listen(event - 10000 /*ePluginEventBase*/, value))
		return true;

	switch (event)
	{
	case 510:
	case 511:
	case 512:
		mModel->getBoneTranslation(&x, &y, &z);
		
		switch (event - 510)
		{
		case 0:
			mModel->setBoneTranslation(value, y, z);
			break;
		case 1:
			mModel->setBoneTranslation(x, value, z);
			break;
		case 2:
			mModel->setBoneTranslation(x, y, value);
			break;
		}
		
		freyja_event_gl_refresh();
		break;


	case 520:
	case 521:
	case 522:
		mModel->getBoneRotation(&x, &y, &z);

		switch (event - 520)
		{
		case 0:
			mModel->setBoneRotation(value, y, z);
			break;
		case 1:
			mModel->setBoneRotation(x, value, z);
			break;
		case 2:
			mModel->setBoneRotation(x, y, value);
			break;
		}
		
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


	case 800:
	case 801:
	case 802:
		freyjaGetLightPosition4v(0, pos);
		pos[event - 800] = value;
		freyjaLightPosition4v(0, pos);
		freyja_event_gl_refresh();
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
		setZoom(value);//freyja_event_get_float(eZoom));
		freyja_print("Zoom %f", mRender->getZoom());
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
		freyjaPolygonSplit(mModel->getCurrentMesh(), mModel->getCurrentPolygon());
		freyja_print("Splitting polygon[%i]", mModel->getCurrentPolygon());
		break;

	case eMeshFlipNormals:
		freyjaMeshNormalFlip(mModel->getCurrentMesh());
		freyja_print("Flipping normals for mesh[%i]", mModel->getCurrentMesh());
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
		mModel->printInfo();
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
		switch (mEditorMode)
		{
		case TEXTURE_EDIT_MODE:
			break;
		case ANIMATION_EDIT_MODE:
			break;
		case MAP_EDIT_MODE:
			break;
		case MODEL_EDIT_MODE:
			if (freyja_create_confirm_dialog("gtk-dialog-question",
							  "You must close the currently open model to create a new model.",
							  "Would you like to close the model and lose unsaved changes?",
							  "gtk-cancel", "_Cancel", "gtk-close", "C_lose"))
			{
				mModel->clear();
				freyja_print("Closing Model...");
				freyja_set_main_window_title(BUILD_NAME);
				mCleared = true;
			}
			break;
		case MATERIAL_EDIT_MODE:
			//freyjaMaterialDelete(freyjaCurrentMaterial());
			//freyja_print("New material [%i] deleted.", i);
			break;
		}
		break;


	case eNewFile:
		switch (mEditorMode)
		{
		case TEXTURE_EDIT_MODE:
			break;
		case ANIMATION_EDIT_MODE:
			break;
		case MAP_EDIT_MODE:
			break;
		case MODEL_EDIT_MODE:
			if (freyja_create_confirm_dialog("gtk-dialog-question",
							  "You must close the currently open model to create a new model.",
							  "Would you like to close the model and lose unsaved changes?",
							  "gtk-cancel", "_Cancel", "gtk-close", "C_lose"))
			{
				// Add check to see if the model was modified here
				//if (confirmDialog("gtk-dialog-question",
				//				  "You must save the currently open model to retain modifications.",
				//				  "Would you like to save the currently open model?",
				//				  "gtk-cancel", "_Cancel", "gtk-save", "_Save"))
				//{
				//	mFileDialogMode = FREYJA_MODE_SAVE_MODEL;
				//	freyja_event_file_dialog("Save model as...");
				//}

				mModel->clear();
				freyja_print("Closing Model...");
				freyja_set_main_window_title(BUILD_NAME);
				mCleared = true;
			}
			break;
		case MATERIAL_EDIT_MODE:
			i = freyjaMaterialCreate();
			freyja_print("New material [%i] created.", i);
			break;
		}
		break;


	case eOpenFileTexture:
		mFileDialogMode = FREYJA_MODE_LOAD_TEXTURE;
		freyja_event_file_dialog("Open texture...");
		break;

	case eOpenFile:
		switch (mEditorMode)
		{
		case TEXTURE_EDIT_MODE:
			mFileDialogMode = FREYJA_MODE_LOAD_TEXTURE;
			freyja_event_file_dialog("Open texture...");
			break;
		case ANIMATION_EDIT_MODE:
			mFileDialogMode = FREYJA_MODE_LOAD_ANIMATION;
			freyja_event_file_dialog("Open animation...");
			break;
		case MAP_EDIT_MODE:
			mFileDialogMode = FREYJA_MODE_LOAD_MAP;
			freyja_event_file_dialog("Open map...");
			break;
		case MODEL_EDIT_MODE:
			if (!mCleared)
			{
				if (freyja_create_confirm_dialog("gtk-dialog-question",
												 "You must close the current model to open a new one.",
												 "Open the new model and lose unsaved changes?",
												 "gtk-cancel", "_Cancel", "gtk-open", "_Open"))
				{
					mModel->clear();
					freyja_print("Closing Model...");
					freyja_set_main_window_title(BUILD_NAME);

					mFileDialogMode = FREYJA_MODE_LOAD_MODEL;
					freyja_event_file_dialog("Open model...");
				}
			}
			else
			{
				mFileDialogMode = FREYJA_MODE_LOAD_MODEL;
				freyja_event_file_dialog("Open model...");
			}
			break;
		case MATERIAL_EDIT_MODE:
			mFileDialogMode = FREYJA_MODE_LOAD_MATERIAL;
			freyja_event_file_dialog("Open material...");
			break;
		}
		break;


	case eSaveAsFile:
		switch (mEditorMode)
		{
		case TEXTURE_EDIT_MODE:
			//mFileDialogMode = FREYJA_MODE_SAVE_TEXTURE;
			//freyja_event_file_dialog("Save texture as...");
			break;
		case MODEL_EDIT_MODE:
			mFileDialogMode = FREYJA_MODE_SAVE_MODEL;
			freyja_event_file_dialog("Save model as...");
			break;
		case MAP_EDIT_MODE:
			//mFileDialogMode = FREYJA_MODE_SAVE_MAP;
			//freyja_event_file_dialog("Save map as...");
			break;
		case ANIMATION_EDIT_MODE:
			mFileDialogMode = FREYJA_MODE_SAVE_ANIMATION;
			freyja_event_file_dialog("Save animation as...");
			break;
		case MATERIAL_EDIT_MODE:
			mFileDialogMode = FREYJA_MODE_SAVE_MATERIAL;
			freyja_event_file_dialog("Save material as...");
			break;
		}
		break;

	case eSaveFile:
		switch (mEditorMode)
		{
		case TEXTURE_EDIT_MODE:
			//mFileDialogMode = FREYJA_MODE_SAVE_TEXTURE;
			//freyja_event_file_dialog("Save texture as...");
			break;
		case MODEL_EDIT_MODE:
			if (mCleared) // safety
			{
				freyja_print("We don't save empty files anymore");
				break;
			}

			if (mRecentFiles.empty())
			{
				mFileDialogMode = FREYJA_MODE_SAVE_MODEL;
				freyja_event_file_dialog("Save model as...");				
			}
			else
			{
				if (!mModel->saveModel(mRecentFiles[mRecentFiles.end()-1]))
					freyja_print("Model '%s' Saved",
								 mRecentFiles[mRecentFiles.end()-1]);
				else
					freyja_print("Model '%s' failed to save",
								 mRecentFiles[mRecentFiles.end()-1]);
			}
			break;
		case MAP_EDIT_MODE:
			//mFileDialogMode = FREYJA_MODE_SAVE_MAP;
			//freyja_event_file_dialog("Save map as...");
			break;
		case ANIMATION_EDIT_MODE:
			mFileDialogMode = FREYJA_MODE_SAVE_ANIMATION;
			freyja_event_file_dialog("Save animation as...");
			break;
		case MATERIAL_EDIT_MODE:
			mFileDialogMode = FREYJA_MODE_SAVE_MATERIAL;
			freyja_event_file_dialog("Save material as...");
			break;
		}
		break;

	case eAppendFile:
		mFileDialogMode = FREYJA_MODE_LOAD_MODEL;
		freyja_event_file_dialog("Append to model...");
		freyja_print("Append mode is default Open mode in this build...");
		break;

	case eSaveAsFileModel:
		mFileDialogMode = FREYJA_MODE_SAVE_MODEL;
		freyja_event_file_dialog("Save model as...");
		break;

	case eSaveFileModel:
		if (mCleared) // safety
		{
			freyja_print("No changes to be saved.");
			break;
		}

		if (mRecentFiles.empty())
		{
			mFileDialogMode = FREYJA_MODE_SAVE_MODEL;
			freyja_event_file_dialog("Save model as...");				
		}
		else
		{
			if (!mModel->saveModel(mRecentFiles[mRecentFiles.end()-1]))
				freyja_print("Model '%s' Saved",
							 mRecentFiles[mRecentFiles.end()-1]);
			else
				freyja_print("Model '%s' failed to save",
							 mRecentFiles[mRecentFiles.end()-1]);
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
				mModel->clear();
				freyja_print("Closing Model...");
				freyja_set_main_window_title(BUILD_NAME);

				
				mFileDialogMode = FREYJA_MODE_LOAD_MODEL;
				freyja_event_file_dialog("Open model...");
				//freyja_event_file_dialog2("Open model...", eLoadModelText);
			}
		}
		else
		{
			mFileDialogMode = FREYJA_MODE_LOAD_MODEL;
			freyja_event_file_dialog("Open model...");
			//freyja_event_file_dialog2("Open model...", eLoadModelText);
		}
		break;

	case eRevertFile:
		if (mRecentFiles.empty())
		{
			freyja_print("Revert requires a model being previously loaded");
			break;
		}

		if (freyja_create_confirm_dialog("gtk-dialog-question",
										 "You will lose all changes you made by reverting.",
										 "Would you like to revert the model and lose unsaved changes?",
										 "gtk-cancel", "_Cancel", "gtk-revert", "_Revert"))
		{
			mModel->clear();
			freyja_print("Reverting Model...");
			mCleared = true;
			freyja_set_main_window_title(BUILD_NAME);

			if (!mModel->loadModel(mRecentFiles[mRecentFiles.end()-1]))
				mCleared = false;
		}
		break;

	case eExportFile:
		mFileDialogMode = FREYJA_MODE_SAVE_MODEL;
		freyja_event_file_dialog("Export model...");
		freyja_print("Exporting is handled from Save As using file extentions...");
		break;

	case eImportFile:
		mFileDialogMode = FREYJA_MODE_LOAD_MODEL;
		freyja_event_file_dialog("Import model...");
		freyja_print("Importing is handled automatically from Open...");
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


	case eCamera:
		mRender->toggleFlag(FreyjaRender::RENDER_CAMERA);
		freyja_print("Camera rendering [%s]", 
					(mRender->getFlags() & FreyjaRender::RENDER_CAMERA) ? 
					"ON" : "OFF");
		break;

	case eViewports:
		mRender->toggleFlag(FreyjaRender::fViewports);
		freyja_print("Viewport rendering [%s]", 
					(mRender->getFlags() & FreyjaRender::fViewports) ? 
					"ON" : "OFF");
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
		mModel->mTexture.glScreenShot("Freyja", 
									  mRender->getWindowWidth(),
									  mRender->getWindowHeight());
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





	case eRenderBbox:
		mRender->toggleFlag(FreyjaRender::RENDER_BBOX);
		freyja_print("BoundingBox rendering [%s]", 
					(mRender->getFlags() & FreyjaRender::RENDER_BBOX) ? 
					"ON" : "OFF");
		break;


	case eMirrorUV_X:
		freyjaModelMirrorTexCoord(0, mModel->getCurrentTexCoord(), mModel->mUVMap, true, false);
		freyja_event_gl_refresh();
		break;

	case eMirrorUV_Y:
		freyjaModelMirrorTexCoord(0, mModel->getCurrentTexCoord(), mModel->mUVMap, false, true);
		freyja_event_gl_refresh();
		break;

	case eTmpUVMapOn:
		mModel->createPolyMappedUVMap(mModel->getCurrentPolygon());
		freyja_event_gl_refresh();
		break;

	case eTmpUVMapOff:
		mModel->createPolyMappedUVMap(-1);
		freyja_event_gl_refresh();
		break;

	case eTranslateUV:
		break;

	case eRotateUV:
		mModel->createPolyMappedUVMap(mModel->getCurrentPolygon());
		//mModel->transformTexCoord(mModel->getCurrentTexCoord(),
		//						  fRotateAboutPoint, 45, 0); 
		freyja_event_gl_refresh();
		break;

	case ePolyMapTexturePolygon:
		freyjaMeshPolygonSplitTexCoords(mModel->getCurrentMesh(),
									mModel->getCurrentPolygon());
		break;

	case eScaleUV:
		freyjaModelTransformTexCoord(0, mModel->getCurrentTexCoord(),
									fScale, 0.5, 0.5);
		break;

	case eSetMeshTexture:
		freyja_print("Switching all of mesh[%i]'s ploygons to material %i",
					 mModel->getCurrentMesh(),
					 mModel->getCurrentTextureIndex());
		freyjaMeshMaterial(mModel->getCurrentMesh(),
							mModel->getCurrentTextureIndex());
		freyja_event_gl_refresh();
		break;

	case eSetPolygonTexture:
		freyja_print("Switching all ploygon to material %i",
					 mModel->getCurrentTextureIndex());
		mModel->setPolygonMaterial(mModel->getCurrentPolygon(),
								   mModel->getCurrentTextureIndex());
		freyja_event_gl_refresh();
		break;

	case eTransformSelectedVertices:
		mTransformMode = FreyjaModel::TransformSelectedVertices;
		break;

	case eTransformGroup:
		mTransformMode = FreyjaModel::TransformVertexFrame;
		break;

	case eTransformScene:
		mTransformMode = FreyjaModel::TransformScene;
		break;

	case eTransformPoint:
		mTransformMode = FreyjaModel::TransformPoint;
		break;

	case eTransformMesh:
		mTransformMode = FreyjaModel::TransformMesh;
		break;

	case eTransformBone:
		mTransformMode = FreyjaModel::TransformBone;
		break;



	case eDelete:
		deleteSelectedObject();
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
		{
			FreyjaState *state = gFreyjaCursor.Pop();

			if (state)
			{
				//state->Undo();
				//freyja_print( "! Undo e=%i, m=%i, i=%i",
				//			  state->GetEvent(), state->GetMode(), state->GetIndex());
				//gFreyjaCursor.mPos = state->GetPos();
				delete state;
				freyja_event_gl_refresh();
			}
			else
			{
				freyja_print("Undo is out of stack frames.");
			}
		}
		break;

	case eRedo:
		freyja_print("Redo is not avalible in this build");
		break;

	case eCut:
		if (copySelectedObject())
			deleteSelectedObject();
		freyja_event_gl_refresh();
		break;
	case eCopy:
		copySelectedObject();
		freyja_event_gl_refresh();
		break;
	case ePaste:
		pasteSelectedObject();
		freyja_event_gl_refresh();
		break;


	case eMoveObject:
		mEventMode = modeMove;
		gFreyjaCursor.SetMode(Freyja3dCursor::Translation);
		freyja_print("Move object...");
		freyja_event_gl_refresh();
		break;

	case eScaleObject:
		mEventMode = modeScale;
		gFreyjaCursor.SetMode(Freyja3dCursor::Scale);
		freyja_print("Scale object...");
		freyja_event_gl_refresh();
		break;

	case eRotateObject:
		mEventMode = modeRotate;
		gFreyjaCursor.SetMode(Freyja3dCursor::Rotation);
		freyja_print("Rotate object...");
		freyja_event_gl_refresh();
		break;

	case eSelect:
		mEventMode = modeSelect;
		gFreyjaCursor.SetMode(Freyja3dCursor::Invisible);
		freyja_print("Select object...");
		freyja_event_gl_refresh();
		break;

	case eUnselect:
		mEventMode = modeUnselect;
		gFreyjaCursor.SetMode(Freyja3dCursor::Invisible);
		freyja_print("Unselect object...");
		freyja_event_gl_refresh();
		break;


	case eMove:
		mModel->transform(mTransformMode, fTranslate,
						  freyja_event_get_float(eMove_X),
						  freyja_event_get_float(eMove_Y),
						  freyja_event_get_float(eMove_Z));
		freyja_event_gl_refresh();
		
		freyja_event_set_float(eMove_X, 0.0f);
		freyja_event_set_float(eMove_Y, 0.0f);
		freyja_event_set_float(eMove_Z, 0.0f);
		break;

	case eRotate:
		mModel->transform(mTransformMode, fRotate,
						  freyja_event_get_float(eRotate_X),
						  freyja_event_get_float(eRotate_Y),
						  freyja_event_get_float(eRotate_Z));
		freyja_event_gl_refresh();
		
		freyja_event_set_float(eRotate_X, 0.0f);
		freyja_event_set_float(eRotate_Y, 0.0f);
		freyja_event_set_float(eRotate_Z, 0.0f);
		break;

	case eScale:
		mModel->transform(mTransformMode, fScale,
						  freyja_event_get_float(eScale_X),
						  freyja_event_get_float(eScale_Y),
						  freyja_event_get_float(eScale_Z));
		freyja_event_gl_refresh();
		
		freyja_event_set_float(eScale_X, 1.0f);
		freyja_event_set_float(eScale_Y, 1.0f);
		freyja_event_set_float(eScale_Z, 1.0f);
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
		mTransformMode = FreyjaModel::TransformBone;
		freyja_print("Select bone...");
		mEventMode = modeSelect;
		break;
	case CMD_BONE_NEW:
		mTransformMode = FreyjaModel::TransformBone;
		addObject();
		mCleared = false;
		//freyja_print("Select new child bone placement directly...");
		//mEventMode = BONE_ADD_MODE;
		break;
	case CMD_BONE_CONNECT:
		mTransformMode = FreyjaModel::TransformBone;
		freyja_print("Select a bone to connect to current bone...");
		mEventMode = BONE_CONNECT_MODE;
		break;

	case CMD_BONE_DISCONNECT:
		mTransformMode = FreyjaModel::TransformBone;
		freyja_print("Select bone to break from current");
		mEventMode = BONE_DISCONNECT_MODE;
		break;

	case CMD_BONE_NEXT:
		mTransformMode = FreyjaModel::TransformBone;
		mModel->setCurrentBone(mModel->getCurrentBone() + 1);
		break;

	case CMD_BONE_PREV:
		mTransformMode = FreyjaModel::TransformBone;
		if (mModel->getCurrentBone())
			mModel->setCurrentBone(mModel->getCurrentBone() - 1);
		break;

	case CMD_BONE_ADD_MESH:
		mTransformMode = FreyjaModel::TransformBone;
		mModel->addMeshToBone(mModel->getCurrentBone(), mModel->getCurrentMesh());
		freyja_print("New Bone[%i] now contains mesh %i",
					 mModel->getCurrentBone(), mModel->getCurrentMesh());
		break;
	
	case CMD_BONE_DELETE_MESH:
		mTransformMode = FreyjaModel::TransformBone;
		mModel->removeMeshFromBone(mModel->getCurrentBone(), mModel->getCurrentMesh());
		freyja_print("New Bone[%i] no longer contains mesh %i",
					 mModel->getCurrentBone(), mModel->getCurrentMesh());
		break;


	case eExtrude:
		freyjaPolygonExtrudeQuad1f(mModel->getCurrentPolygon(), 8.0f);

		if (freyjaGetPolygonVertexCount(mModel->getCurrentPolygon()))
		{
			long polygonIndex = mModel->getCurrentPolygon();
			long i, v, count = freyjaGetPolygonVertexCount(polygonIndex);
			Vector<unsigned int> list = mModel->getVertexSelectionList();

			list.clear();

			for (i = 0; i < count; ++i)
			{
				v = freyjaGetPolygonVertexIndex(polygonIndex, i);
				list.pushBack(v);
			}
		}
		freyja_event_gl_refresh();
		break;


		
	/* MESHES */
	case eMeshTesselate:
		freyjaMeshTesselateTriangles(mModel->getCurrentMesh());
		break;
	case eMeshTexcoordSpherical:
		freyjaMeshUVMapSpherical(mModel->getCurrentMesh());

		if (freyja_create_confirm_dialog("gtk-dialog-question",
										 "You generated vertex UV mapping, but it is not polymapped.",
										 "Would you like to promote it to ploymapped texcoords?",
										 "gtk-cancel", "_Cancel", "gtk-ok", "_Promote"))
		{
			freyjaMeshPromoteTexcoordsToPloymapping(mModel->getCurrentMesh());
		}
		break;
	case eMeshTexcoordCylindrical:
		freyjaMeshUVMapCylindrical(mModel->getCurrentMesh());

		if (freyja_create_confirm_dialog("gtk-dialog-question",
										 "You generated vertex UV mapping, but it is not polymapped.",
										 "Would you like to promote it to ploymapped texcoords?",
										 "gtk-cancel", "_Cancel", "gtk-ok", "_Promote"))
		{
			freyjaMeshPromoteTexcoordsToPloymapping(mModel->getCurrentMesh());
		}
		break;
	case eMeshGenerateNormals:
		freyjaMeshGenerateVertexNormals(mModel->getCurrentMesh());
		freyja_event_gl_refresh();
		break;
	case eMeshNew:
		mTransformMode = FreyjaModel::TransformMesh;
		addObject();
		mCleared = false;
		break;
	case eMeshDelete:
		mTransformMode = FreyjaModel::TransformMesh;
		deleteSelectedObject();
		mCleared = false;
		break;
	case eMeshSelect:
		mTransformMode = FreyjaModel::TransformMesh;
		mEventMode = modeSelect;
		freyja_print("Select object mesh...");
		break;
	case eMeshMirrorX:
		mModel->mirrorUsingVertexBuffer(true, false, false);
		break;
	case eMeshMirrorY:
		mModel->mirrorUsingVertexBuffer(false, true, false);
		break;
	case eMeshMirrorZ:
		mModel->mirrorUsingVertexBuffer(false, false, true);
		break;
	case eTransformMeshPivot:
		mTransformMode = FreyjaModel::TransformMesh;
		mEventMode = MESH_MOVE_CENTER;
		freyja_print("Reposition mesh center point");
		break;


	/* ANIMATIONS */
	case eAnimationNext:
		mModel->SetCurrentAnimation(mModel->getCurrentAnimation() + 1);
		freyja_print("Animation[%i].", mModel->getCurrentAnimation());
		break;

	case eAnimationPrev:
		if (mModel->getCurrentAnimation())
			mModel->SetCurrentAnimation(mModel->getCurrentAnimation() - 1);
		else
			mModel->SetCurrentAnimation(0);

		freyja_print("Animation[%i].", mModel->getCurrentAnimation());
		break;






	case eZoom:
		setZoom(freyja_event_get_float(eZoom));
		freyja_event_gl_refresh();
		break;


	case CMD_MISC_BBOX_SELECT:
		if (mEventMode == VERTEX_BBOX_SELECT_MODE)
		{
			mEventMode = modeNone;
			mRender->clearFlag(FreyjaRender::RENDER_BBOX_SEL);
			mModel->boxSelectionListBuild();
			freyja_print("Vertex box select: Vertex list built");
		}
		else
		{
			mEventMode = VERTEX_BBOX_SELECT_MODE;
			mRender->setFlag(FreyjaRender::RENDER_BBOX_SEL);
			freyja_print("Vertex box select: Press agian to end selection");
		}
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
		freyjaMeshUVMapPlanar(mModel->getCurrentMesh());
		break;


	case CMD_MISC_SIDES_M:
		mModel->setCurrentPolygonEdgeCount(mModel->getCurrentPolygonEdgeCount()-1);
		freyja_print("Making %i sided polygons", 
					 mModel->getCurrentPolygonEdgeCount());
		break;
	case CMD_MISC_SIDES_P:
		mModel->setCurrentPolygonEdgeCount(mModel->getCurrentPolygonEdgeCount()+1);
		freyja_print("Making %i sided polygons", 
					 mModel->getCurrentPolygonEdgeCount());
		break;

	case CMD_MISC_ZOOM_IN:
		if (mRender->getZoom() <= 0.02)
		{
			mRender->setZoom(mRender->getZoom() + 0.0001);
		}
		else
		{
			mRender->setZoom(mRender->getZoom() + 0.01);
		}

		freyja_event_gl_refresh();
		break;

	case CMD_MISC_ZOOM_OUT:
		if (mRender->getZoom() <= 0.02)
		{
			mRender->setZoom(mRender->getZoom() - 0.0001);
		}
		else
		{
			mRender->setZoom(mRender->getZoom() - 0.01);
		}

		freyja_event_gl_refresh();
		break;


	case eGeneratePatchMesh:
		if (mModel->pasteSelectedPatch())
			mCleared = false;
		break;


	case eRenderPatch:
		if (FreyjaRender::mPatchDisplayList == -1)
		{
			//test_patch();
			FreyjaRender::mPatchDisplayList = 0;
		}

		FreyjaRender::mPatchDisplayList = !FreyjaRender::mPatchDisplayList;
		
		if (FreyjaRender::mPatchDisplayList)
		{
			//test_patch(); // hack for update
		}

		freyja_print("Patch Rendering [%s], Use Vertex:Move to edit...", 
					 (FreyjaRender::mPatchDisplayList) ? "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_RENDER_BONETAG:
		mRender->toggleFlag(FreyjaRender::RENDER_BONES);
		freyja_print("Bone Rendering [%s]", 
					 (mRender->getFlags() & FreyjaRender::RENDER_BONES) ? 
					 "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_RENDER_POINTS:
		mRender->toggleFlag(FreyjaRender::RENDER_POINTS);
		freyja_print("Point Rendering [%s]", 
					 (mRender->getFlags() & FreyjaRender::RENDER_POINTS) ? 
					 "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_RENDER_WIREFRAME:
		mRender->toggleFlag(FreyjaRender::RENDER_WIREFRAME);
		freyja_print("Wireframe Rendering [%s]", 
					 (mRender->getFlags() & FreyjaRender::RENDER_WIREFRAME) ? 
					 "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_RENDER_FACE:
		mRender->toggleFlag(FreyjaRender::RENDER_FACE);
		freyja_print("Face Rendering [%s]", 
					 (mRender->getFlags() & FreyjaRender::RENDER_FACE) ? 
					 "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_RENDER_NORMALS:
		mRender->toggleFlag(FreyjaRender::RENDER_NORMALS);
		freyja_print("Normal Rendering [%s]", 
					 (mRender->getFlags() & FreyjaRender::RENDER_NORMALS) ? 
					 "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_RENDER_LIGHTING:
		mRender->toggleFlag(FreyjaRender::RENDER_NORMAL);
		mRender->toggleFlag(FreyjaRender::RENDER_LIGHTING);
		freyja_print("GL Lighting is [%s]", 
					 (mRender->getFlags() & FreyjaRender::RENDER_LIGHTING) ? 
					 "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_RENDER_TEXTURE:
		mRender->toggleFlag(FreyjaRender::RENDER_TEXTURE);
		freyja_print("Texture rendering is [%s]", 
					 (mRender->getFlags() & FreyjaRender::RENDER_TEXTURE) ? 
					 "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_RENDER_MATERIAL:
		mRender->toggleFlag(FreyjaRender::RENDER_MATERIAL);
		freyja_print("Material rendering is [%s]", 
					 (mRender->getFlags() & FreyjaRender::RENDER_MATERIAL) ? 
					 "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_RENDER_GRID:
		mRender->toggleFlag(FreyjaRender::RENDER_EDIT_GRID);
		freyja_print("Edit Grid rendering [%s]",
					 (mRender->getFlags() & FreyjaRender::RENDER_EDIT_GRID) ? 
					 "ON" : "OFF");
		freyja_event_gl_refresh();
		break;


	case FREYJA_MODE_TEXTURE_EDIT:
		mRender->setViewMode(VIEWMODE_TEXTURE_EDIT);
		freyja_event_gl_refresh();
		freyja_print("Texture Edit");
		mEditorMode = TEXTURE_EDIT_MODE;
		break;

	case FREYJA_MODE_MODEL_EDIT:
		mRender->setViewMode(VIEWMODE_MODEL_EDIT);
		freyja_event_gl_refresh();
		freyja_print("Model Edit");
		mEditorMode = MODEL_EDIT_MODE;
		break;

	case FREYJA_MODE_MODEL_VIEW:
		mRender->setViewMode(VIEWMODE_MODEL_VIEW);
		freyja_event_gl_refresh();
		freyja_print("Animation View");
		mEditorMode = ANIMATION_EDIT_MODE;
		break;

	case FREYJA_MODE_MATERIAL_EDIT:
		mRender->setViewMode(VIEWMODE_MATERIAL_EDIT);
		freyja_event_gl_refresh();
		freyja_print("Material Edit");
		mEditorMode = MATERIAL_EDIT_MODE;
		break;

	case FREYJA_MODE_PLANE_XY:
		freyja_print("Plane XY");      
		mModel->setCurrentPlane(PLANE_XY);
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_PLANE_XZ:
		freyja_print("Plane XZ");      
		mModel->setCurrentPlane(PLANE_XZ);
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_PLANE_YZ:
		freyja_print("Plane ZY");      
		mModel->setCurrentPlane(PLANE_ZY);
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_AXIS_X:
		freyja_print("X locked"); 
		gFreyjaCursor.mAxis = 0;
		freyja_event_gl_refresh();
		break;
	case FREYJA_MODE_AXIS_Y:
		freyja_print("Y locked"); 
		gFreyjaCursor.mAxis = 1;
		freyja_event_gl_refresh();
		break;	
	case FREYJA_MODE_AXIS_Z:
		freyja_print("Z locked"); 
		gFreyjaCursor.mAxis = 2;
		freyja_event_gl_refresh();
		break;

	default:
		//if (command > eRecentFiles &&
		//	(command - eRecentFiles - 1) < (int)mRecentFiles.size())
		//{
		//	mFileDialogMode = FREYJA_MODE_LOAD_MODEL;
		//	handleFilename(mRecentFiles[(command - eRecentFiles - 1)]);
		//}

		freyja_print("!Unhandled event(%d)", command);
		return false;
	}   

	return true;
}


bool FreyjaControl::handleEvent(int mode, int cmd)
{
	freyja_print("Call to handleEvent is deprecated...\n");

	if (event(cmd))
		return true;

	return false;
}


void FreyjaControl::handleFilename(const char *filename)
{
	int failed = 1;
	int type = -1, type2 = -1;


	switch (mFileDialogMode)
	{
	case FREYJA_MODE_LOAD_MATERIAL:
		failed = mModel->loadMaterial(filename);
		type = 0;
		type2 = 1;

		if (!failed)
			freyja_refresh_material_interface();
		break;

	case FREYJA_MODE_SAVE_MATERIAL:
		failed = mModel->saveMaterial(filename);
		type = 0;
		type2 = 0;
		break;

	case FREYJA_MODE_LOAD_MODEL:
		failed = mModel->loadModel(filename);

		if (!failed)
		{
			char title[1024];

			snprintf(title, 1024, "%s - Freyja", filename);
			freyja_set_main_window_title(title);

			addRecentFilename(filename);

			mCleared = false;
		}

		type = 2;
		type2 = 1;
		break;

	case FREYJA_MODE_LOAD_TEXTURE:
		failed = mModel->loadTexture(filename);
		type = 1;
		type2 = 1;
		freyja_event_gl_refresh();

		if (failed == 0) // success
		{
			uint32 e = resourceGetEventId1s("eSetTextureNameA");
			uint32 texture = mModel->getTexureId() - 1;
			mgtk_textentry_value_set(e, filename);
			freyjaMaterialSetFlag(freyjaGetCurrentMaterial(),
			                      fFreyjaMaterial_Texture);
			mgtk_spinbutton_value_set(eSetMaterialTexture, texture);
			freyjaMaterialTexture(freyjaGetCurrentMaterial(), texture);
		}
		break;

	case FREYJA_MODE_SAVE_MODEL:
		if (FreyjaFileReader::doesFileExist(filename))
		{
			if (freyja_create_confirm_dialog("gtk-dialog-question",
											 "You are about to overwrite an existing file.",
											 "Would you like to replace the existing model with your current model?",
											 "gtk-cancel", "_Cancel", "gtk-replace", "_Replace"))
			{
				failed = mModel->saveModel(filename);
				type = 2;
				type2 = 0;
			}
		}
		else
		{
			failed = mModel->saveModel(filename);
			type = 2;
			type2 = 0;
		}

		if (!failed)
		{
			char title[1024];

			snprintf(title, 1024, "%s - Freyja", filename);
			freyja_set_main_window_title(title);
			addRecentFilename(filename);
			mCleared = true;
		}
		break;


	case FREYJA_MODE_SAVE_ANIMATION:
		failed = 0;

		if (mModel->saveAnimation(filename) < 0)
			failed = 1;

		type = 3;
		type2 = 0;
		break;

		/*
	case FREYJA_MODE_LOAD_PALETTE:
		failed = 0;
		mMaterial->loadTexturePalette(filename);
		type = 4;
		type2 = 0;
		break;
		*/
	}

	// Mongoose 2002.02.02, Reduce text segment size some  =)
	freyja_print("%s '%s' %s%s", 
					(type == 0) ? "Material" : 
					(type == 1) ? "Texture" : 
					(type == 2) ? "Model" : 
					(type == 3) ? "Animation" :
					(type == 4) ?"Palette" :
					"!ERROR: No event for ",
#ifdef WIN32
					filename,
#else
					basename((char*)filename),
#endif
					(type2 == 0) ? "save " : "load ",
					(failed == 0) ? "[sucess]" : "[failed]");
}


void FreyjaControl::handleTextEvent(int event, const char *text)
{
	if (ResourceEvent::listen(event - 10000 /*ePluginEventBase*/, (char*)text))
		return;

	switch (event)
	{
	case eSetMaterialName:
		freyjaMaterialName(freyjaGetCurrentMaterial(), text);
		break;

	case eSetTextureNameA:
		freyjaMaterialTextureName(freyjaGetCurrentMaterial(), text);
		break;

	case eSetTextureNameB:
		/* Text here is assumed to be a filename */
		//mMaterial->loadDetailTexture(text);
		break;

	case eSetCurrentBoneName:
		mModel->setNameBone(mModel->getCurrentBone(), text);
		break;

	default:
		freyja_print("Unhandled event(%i) '%s'", event, text);
	}
}


void FreyjaControl::getFreeWorldFromScreen(int xx, int yy, vec3_t xyz)
{
	vec_t x = xx, y = yy, width, height, invz, fs;
	vec3_t scroll, rotate, xy, yz, xz;


	width = mRender->getWindowWidth();
	height = mRender->getWindowHeight();
	mModel->getSceneTranslation(scroll);
	mRender->getRotation(rotate);

	invz = (1.0 / mRender->getZoom());
	fs = (40.0 * invz) / height;  // fov 40?

	x = (x - width / 2.0) * fs;
	y = -(y - height / 2.0) * fs;

	xy[0] = x - scroll[0] * invz;
	xy[1] = y - scroll[1] * invz;
	xy[2] = xyz[2]; //0.0f;

	xz[0] = x - scroll[0] * invz;
	xz[2] = y - scroll[2] * invz;
	xz[1] = xyz[1]; //0.0f;

	yz[2] = y - scroll[2] * invz;
	yz[1] = x - scroll[1] * invz;
	yz[0] = xyz[0]; //0.0f;


	// FIXME only considers up Y atm
	if (rotate[1] < 90) // ~0
	{
		xyz[0] = xy[0];
		xyz[1] = xy[1];
		xyz[2] = xy[2];
	}
	else if (rotate[1] >= 260) // ~270
	{
		xyz[0] = yz[0];
		xyz[1] = yz[2] + 18;
		xyz[2] = -(yz[1] - 18);
	}
	else if (rotate[1] >= 170) // ~180
	{
		xyz[0] = -xy[0];
		xyz[1] = xy[1];
		xyz[2] = xy[2];
	}
	else if (rotate[1] >= 60) // ~90
	{
		xyz[0] = yz[0];
		xyz[1] = yz[2] + 18;
		xyz[2] = yz[1] - 18;
	}
	else if (rotate[1] >= 40) // ~45
	{
		xyz[0] = xy[0]/2 + yz[0]/2;
		xyz[1] = xy[1]/2 + (yz[2] + 18)/2;
		xyz[2] = xy[2]/2 + (yz[1] - 18)/2;
	}
	else // FIXME
	{
		xyz[0] = xy[0];
		xyz[1] = xy[1];
		xyz[2] = xy[2];
	}

	//printf("r %f\n", rotate[1]);
}


bool FreyjaControl::motionEvent(int x, int y)
{
	static int old_y = 0, old_x = 0;

	
	switch (mEditorMode)
	{
	case ANIMATION_EDIT_MODE:
		switch (mMouseButton)
		{
		case MOUSE_BTN_RIGHT:
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
			break;

		case MOUSE_BTN_MIDDLE:
			{
				vec4_t pos;
				freyjaGetLightPosition4v(0, pos);
				Vector3d xyz = Vector3d(pos);

#if 0
				getFreeWorldFromScreen(x, y, xyz.mVec);
#else
				// We can't use actual values due to event system spam
				// method used now, so we do constant deltas for now
				float xf, yf;
				const float d = 1.0f;
				xf = ((x < old_x-2.0f) ? -d : ((x > old_x+2.0f) ? d : 0));
				yf = ((y < old_y-2.0f) ? -d : ((y > old_y+2.0f) ? d : 0));

				switch ( mEventMode )
				{
				case modeMove:
					gFreyjaCursor.SetMode(Freyja3dCursor::Translation);
					gFreyjaCursor.mPos.zero();
					mModel->CursorMove(xf, yf);
					xyz += gFreyjaCursor.mPos;
					gFreyjaCursor.mPos = xyz;
					break;

				case modeRotate:
					gFreyjaCursor.SetMode(Freyja3dCursor::Rotation);
					freyja_print("3d cursor rotate not implemented");
					break;

				default:
					;
				}

				old_x = x;
				old_y = y;
#endif
				pos[0] = xyz.mVec[0];
				pos[1] = xyz.mVec[1];
				pos[2] = xyz.mVec[2];
				freyjaLightPosition4v(0, pos);

				freyja_event_set_float(800, xyz.mVec[0]);
				freyja_event_set_float(801, xyz.mVec[1]);
				freyja_event_set_float(802, xyz.mVec[2]);
				freyja_event_gl_refresh();
			}
			break;


		case MOUSE_BTN_LEFT:
			{
				Vector3d xyz;

#if 0
				getFreeWorldFromScreen(x, y, xyz.mVec);
#else
				// We can't use actual values due to event system spam
				// method used now, so we do constant deltas for now
				float xf, yf;
				const float d = 0.5f, t = 1.0f;
				xf = ((x < old_x-t) ? -d : ((x > old_x+t) ? d : 0));
				yf = ((y < old_y-t) ? -d : ((y > old_y+t) ? d : 0));

				if (mTransformMode == FreyjaModel::TransformBone)
				{
					freyjaGetBoneTranslation3fv(mModel->getCurrentBone(),
												xyz.mVec);
				}

				gFreyjaCursor.mPos.zero();
				mModel->CursorMove(xf, -yf);
				xyz += gFreyjaCursor.mPos;
				gFreyjaCursor.mPos = xyz;

				old_x = x;
				old_y = y;
#endif
				if (FreyjaRender::mPatchDisplayList)
					mModel->movePatchControlPoint(xyz);

				if (mTransformMode == FreyjaModel::TransformBone)
				{
					FreyjaStateTransform *state = new
					FreyjaStateTransform(fTransformBone, fTranslate,
										 mModel->getCurrentBone(), xyz.mVec);
					gFreyjaCursor.ChangeState(state, Freyja3dCursor::Translation);
					mModel->moveObject(FreyjaModel::TransformBone, xyz);
				}
				
			}
			break;
		}
		
		break;


	case TEXTURE_EDIT_MODE:
		switch (mMouseButton)
		{
		case MOUSE_BTN_RIGHT:
			{
				float s;
				float t;
				
				s = (float)x / (float)mRender->getWindowWidth();
				t = (float)y / (float)mRender->getWindowHeight();

				if (s > 1.0) s = 1.0;
				if (s < 0.0) s = 0.0;
				if (t > 1.0) t = 1.0;
				if (t < 0.0) t = 0.0;
				
				mModel->TexelMove(s, t);
				return true;
			}
			break;


		case MOUSE_BTN_MIDDLE:
			{
				float s;
				float t;
				
				s = (float)x / (float)mRender->getWindowWidth();
				t = (float)y / (float)mRender->getWindowHeight();

				if (s > 1.0) s = 1.0;
				if (s < 0.0) s = 0.0;
				if (t > 1.0) t = 1.0;
				if (t < 0.0) t = 0.0;
				
				mModel->UVMapMotion(s, t);
				return true;
			}
			break;


		case MOUSE_BTN_LEFT:
			if (mUVMouseState)
			{
				float s;
				float t;
				
				s = (float)x / (float)mRender->getWindowWidth();
				t = (float)y / (float)mRender->getWindowHeight();
				
				if (s > 1.0) s = 1.0;
				if (s < 0.0) s = 0.0;
				if (t > 1.0) t = 1.0;
				if (t < 0.0) t = 0.0;
				
				mModel->TexelMove(s, t);
				return true;
			}
			break;
		}
		break;


	case MODEL_EDIT_MODE:
		MotionEdit(x, y, mModel->getCurrentPlane());
		break;
	default:
		;
	}
	
	//	freyja_print("motion -> %i, %i", x, y);

	return true;
}


bool FreyjaControl::mouseEvent(int btn, int state, int mod, int x, int y)
{
	EventMode mode = mEventMode;


	mMouseButton = btn;
	mModKey = mod;

	vec_t vx = x, vy = y;
	AdjustMouseXYForViewports(vx, vy);


	switch (mEditorMode)
	{
#if 0
	case ANIMATION_EDIT_MODE:
		if (btn == MOUSE_BTN_UP && state == MOUSE_BTN_STATE_PRESSED)
		{
			freyja_event2i(EVENT_MISC, CMD_MISC_ZOOM_IN);
			freyja_print("Zoom in");
		}
		
		if (btn == MOUSE_BTN_DOWN && state == MOUSE_BTN_STATE_PRESSED)
		{
			freyja_event2i(EVENT_MISC, CMD_MISC_ZOOM_OUT);
			freyja_print("Zoom out");
		}

		switch (btn)
		{
		case MOUSE_BTN_MIDDLE:
			{
				vec3_t pos;

				getFreeWorldFromScreen(x, y, pos);

				mXYZMouseState = 1;
			}
			break;
		case MOUSE_BTN_LEFT:
			{
				vec3_t pos;

				getFreeWorldFromScreen(x, y, pos);
		
				if (FreyjaRender::mPatchDisplayList)
					mModel->selectPatchControlPoint(pos);

				mXYZMouseState = 1;
			}
			break;
		}

		break;
#endif

	case TEXTURE_EDIT_MODE:
		if (mUVMouseState)
		{
			mUVMouseState = false;
			return true;
		}
		else if (btn == MOUSE_BTN_LEFT && state == MOUSE_BTN_STATE_PRESSED)
		{
			float s, t;
			
			
			s = (float)x / (float)mRender->getWindowWidth();
			t = (float)y / (float)mRender->getWindowHeight();
			
			// Mongoose: Clamp texels to be bound by min and max
			if (s > 1.0) s = 1.0;
			if (s < 0.0) s = 0.0;
			if (t > 1.0) t = 1.0;
			if (t < 0.0) t = 0.0;
			
			if (mEventMode == TEXEL_COMBINE)
			{
				mModel->TexelCombine(s, t);
			}
			else
			{
				mModel->TexelSelect(s, t);
			}
			
			mUVMouseState = true;
			return true;
		}
		break;

	case ANIMATION_EDIT_MODE:
	case MODEL_EDIT_MODE:
		// Mongoose 2002.01.12, Allow temp mode override
		if (mod & KEY_LCTRL)
		{
			handleEvent(EVENT_MISC, eSelect);
		}
		else if (mod & KEY_LSHIFT)
		{
			handleEvent(EVENT_MISC, eRotate);
		}

		MouseEdit(btn, state, mod, x, y, mModel->getCurrentPlane());
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

void FreyjaControl::getScreenToWorldOBSOLETE(float *x, float *y)
{
	static bool warn = false;
	float z;


	if (!warn)
	{
		warn = true;
		freyja_print("Call to getScreenToWorldOBSOLETE is deprecated...");
	}

	getWorldFromScreen(x, y, &z);

	switch (mModel->getCurrentPlane())
	{
	case PLANE_XY:
		/* Nothing to do for XY */
		break;
		
	case PLANE_XZ:
		*y = -z;
		break;
		
	case PLANE_ZY: // side
		*x = z;
		break;
	}
}


// Mongoose - 2006.07.31 
// More crap for old system to be backported then rewritten properly  =/
void FreyjaControl::AdjustMouseXYForViewports(vec_t &x, vec_t &y)
{
#if DEBUG_VIEWPORT_MOUSE
	freyja_print("> Mouse x = %f y = %f", x, y);
#endif

	// Trap the junk 0,0 states that are often tossed around on
	// just mouse button updates
	if ( x == 0 && y == 0 || 
		 mEditorMode == MATERIAL_EDIT_MODE ||
		 mEditorMode == TEXTURE_EDIT_MODE)
	{
	}
	else if (mRender->GetMode() & FreyjaRender::fViewports)
	{
		// Translate this to it's correct 'plane editing mode'
		// for the viewport and adjust the x, y values here 
		// to reuse the old mouse style handling
		// In other words - just make this work for now
		vec_t h = mRender->getWindowHeight();
		vec_t w = mRender->getWindowWidth();
		vec_t halfW = w * 0.5f;
		vec_t halfH = h * 0.5f;
		
		// Handle Front XY viewport ( not using viewport class yet )
		if ( x < halfW && y > halfH )
		{
			mEditorMode = MODEL_EDIT_MODE;
			mModel->setCurrentPlane(PLANE_XY);

			// Adjust actual window space mouse x, y to fit to viewport
			// This makes the x, y fill the 'window' for the viewport
			x = x*2.0f;
			y = y*2.0f - h;
		}
		// Handle Top XZ viewport ( not using viewport class yet )
		else if ( x > halfW && y < halfH )
		{
			mEditorMode = MODEL_EDIT_MODE;
			mModel->setCurrentPlane(PLANE_XZ);

			// Adjust actual window space mouse x, y to fit to viewport
			// This makes the x, y fill the 'window' for the viewport
			x = x*2.0f - w;
			y = y*2.0f;
		}
		// Handle Side ZY viewport ( not using viewport class yet )
		else if ( x > halfW && y > halfH )
		{
			mEditorMode = MODEL_EDIT_MODE;
			mModel->setCurrentPlane(PLANE_ZY);

			// Adjust actual window space mouse x, y to fit to viewport
			// This makes the x, y fill the 'window' for the viewport
			x = x*2.0f - w;
			y = y*2.0f - h;
		}
		// Handle 'Free' viewport ( not using viewport class yet )
		else if ( x < halfW && y < halfH )
		{
			mEditorMode = ANIMATION_EDIT_MODE;

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


#include <hel/Ray.h>
#define DEBUG_PICK_RAY 1
#define DEBUG_SCREEN_TO_WORLD 0
void FreyjaControl::CastPickRay(vec_t x, vec_t y)
{
	Ray &r = mRender->mTestRay;

	mRender->setFlag(FreyjaRender::fDrawPickRay);

	if (mEditorMode == ANIMATION_EDIT_MODE)
	{
		double rayOrigin[4];
		double rayVector[4];
		getPickRay(x, y, rayOrigin, rayVector);

		HEL_VEC3_COPY(rayOrigin, r.mOrigin.mVec);
		HEL_VEC3_COPY(rayVector, r.mDir.mVec);
	}
	else
	{
		vec_t z;

		getWorldFromScreen(&x, &y, &z);

		switch (mModel->getCurrentPlane())
		{
		case PLANE_XY: // Front
			r.mOrigin = Vec3(x, y, z + 100);
			r.mDir = Vec3(0,0,-10000);
			break;

		case PLANE_XZ: // Top
			r.mOrigin = Vec3(x, y-100, -z);
			r.mDir = Vec3(0,10000,0);
			break;

		case PLANE_ZY: // Side FIXME
			r.mOrigin = Vec3(x - 100, y, z);
			r.mDir = Vec3(10000,0,0);
			break;
		}	
	}


#if 0
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


void FreyjaControl::getPickRay(vec_t mouseX, vec_t mouseY, 
								double *rayOrigin, double *rayVector)
{
	/* This does not use viewport: quadrant checks, widths, and heights */
	extern double gMatrix[16]; // This is actual cached render transform
	double inverse[16];
	double rayPnt[4] = {0.0f, 0.0f, 0.0f, 1.0f };
	double rayVec[4];
	vec_t winH = mRender->getWindowHeight();
	vec_t winY = (winH - mouseY) - winH * 0.5f;
	vec_t normY = winY / ( winH * 0.5f );
	vec_t winX = mouseX - winH * 0.5f;
	vec_t normX = winX / ( winH * 0.5f );
	vec_t zNear = -400.0f; // mRender->getZNear();
	vec_t nearH = 20.0f;//mRender->getNearHeight();
	vec_t aspect = mRender->getWindowAspectRatio();

	// This is now ray in eye coordinates
	rayVec[0] = nearH * normY;
	rayVec[1] = nearH * aspect * normX;
	rayVec[2] = -zNear;
	rayVec[3] = 0.0f;

	// Now transform by inverse of modelview matrix to object coordinates
	matrixInvert16dv(gMatrix, inverse);
	matrix4dvx16dv(inverse, rayVec, rayVector); // src, dest can be same
	matrix4dvx16dv(inverse, rayPnt, rayOrigin); // src, dest can be same

#if DEBUG_PICK_RAY
	freyja_print("!%f, %f => <%f %f %f> @ %f, %f, %f\n",
				mouseX, mouseY,
				rayVector[0], rayVector[1], rayVector[2],
				 rayOrigin[0], rayOrigin[1], rayOrigin[2]);
#endif
}


void FreyjaControl::getWorldFromScreen(float *x, float *y, float *z)
{
	float nearHeight = mRender->getNearHeight() * 2.0f;
	float width, height, invz, fs;
	float scroll[3];
 

	width = mRender->getWindowWidth();
	height = mRender->getWindowHeight();
	mModel->getSceneTranslation(scroll);

#if DEBUG_SCREEN_TO_WORLD
	printf("Screen (%.3f, %.3f); aspect_ratio = %.3f; zoom = %.3f\n", 
			 *x, *y, width/height, mRender->getZoom());
	printf("Scroll (%.3f, %.3f, %.3f)\n", 
			 scroll[0], scroll[1], scroll[2]);
#endif

	invz = (1.0 / mRender->getZoom());
	fs = (nearHeight * invz) / height;  // fov 40?

	*x = (*x - width / 2.0) * fs;
	*y = -(*y - height / 2.0) * fs;
	
	switch (mModel->getCurrentPlane())
	{
	case PLANE_XY:
		*x -= scroll[0] * invz;
		*y -= scroll[1] * invz;
		*z = 0.0f;
		break;

	case PLANE_XZ:
		*x -= scroll[0] * invz;
		*z = *y - scroll[2] * invz;
		*y = 0.0f;
		break;

	case PLANE_ZY: // side ZY! change
		*z = *x - scroll[2] * invz;
		*y -= scroll[1] * invz;
		*x = 0.0f;
		break;
	}

#if DEBUG_SCREEN_TO_WORLD
	printf("World (%.3f, %.3f, %.3f)\n", *x, *y, *z);
#endif
}


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////

bool FreyjaControl::copySelectedObject()
{
	switch (mTransformMode)
	{
	case FreyjaModel::TransformSelectedVertices:
		return mModel->copyVertexBuffer();
		break;

	case FreyjaModel::TransformMesh:
		return mModel->copySelectedMesh();
		break;

	default:
		freyja_print("This object type is not supported by Generic Copy.");
		return false;
	}

	return true;
}


bool FreyjaControl::pasteSelectedObject()
{
	switch (mTransformMode)
	{
	case FreyjaModel::TransformSelectedVertices:
		return mModel->pasteSelectedMesh(); // vertexbuffercopy shares buf
		break;

	case FreyjaModel::TransformMesh:
		return mModel->pasteSelectedMesh();
		break;

	default:
		freyja_print("This object type is not supported by Generic Paste.");
		return false;
	}

	return true;
}


void FreyjaControl::deleteSelectedObject()
{
	switch (mTransformMode)
	{
	case FreyjaModel::TransformPoint:
		mEventMode = POINT_DEL_MODE;
		break;

	case FreyjaModel::TransformSelectedVertices:
		if (freyja_create_confirm_dialog("gtk-dialog-question",
										 "You are about to delete the selected vertices.",
										 "Are you sure you want to delete these vertices?",
										 "gtk-cancel", "_Cancel", "gtk-ok", "_Delete"))
		{
			mModel->cullUsingVertexBuffer();
		}
		break;

	case FreyjaModel::TransformBone:
		freyja_print("NOT IMPLEMENTED: Deleting Bone Tag %d", 
					mModel->getCurrentBone());
		break;

	case FreyjaModel::TransformMesh:
		mModel->MeshDel();
		break;
	}
}


void FreyjaControl::addObject()
{
	unsigned int index;


	switch (mTransformMode)
	{
	case FreyjaModel::TransformPoint:
		mEventMode = POINT_ADD_MODE;
		break;

	case FreyjaModel::TransformBone:
		index = mModel->newBone(0.0, 0.0, 0.0, 0x0);

		if (index > 0)
		{
			mModel->connectBone(mModel->getCurrentBone(), index);
		}
		
		freyja_event_gl_refresh();
		freyja_print("New Bone[%u], Bone[%u].parent = %i",
					 index, index, mModel->getCurrentBone());
		break;
	case FreyjaModel::TransformMesh:
		mModel->MeshNew();
		break;
	}
}


void FreyjaControl::UnselectObject(vec_t mouseX, vec_t mouseY)
{
	// Look for objects with depth ( was using planar projection )
	mRender->setFlag(FreyjaRender::fDrawPickRay); // debugging
	CastPickRay(mouseX, mouseY);

	// New backend ray test ( picks faces and marks them unselected )
	Mesh *m = freyjaModelGetMeshClass(0, mModel->getCurrentMesh());

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


void FreyjaControl::SelectObject(vec_t mouseX, vec_t mouseY)
{
	// Look for objects with depth ( was using planar projection )
	mRender->setFlag(FreyjaRender::fDrawPickRay); // debugging
	CastPickRay(mouseX, mouseY);

	// New backend ray test ( picks faces and marks them selected )
	Mesh *m = freyjaModelGetMeshClass(0, mModel->getCurrentMesh());

	if ( m )
	{
		int selected = -1;
		m->IntersectFaces(FreyjaRender::mTestRay, selected, false);

		if (selected > -1)
		{
			freyja_print("Face[%i] selected by pick ray.", selected);
			m->SetFaceFlags(selected, Face::fSelected);
		}
	}
}


#if 0
void FreyjaControl::SelectObject(vec_t x, vec_t y, freyja_plane_t plane)
{
	float xx = x, yy = y, zz;


	/* Mongoose: Convert screen to world coordinate system */
	getWorldFromScreen(&xx, &yy, &zz);

	switch (mTransformMode)
	{
	case FreyjaModel::TransformPoint:
		xx = x; 
		yy = y;
		getScreenToWorldOBSOLETE(&xx, &yy);

		if (FreyjaRender::mPatchDisplayList > 0)
			mModel->selectPatchControlPoint(xx, yy);
		else
			mModel->VertexSelect(xx, yy);
		// mModel->selectVertex(plane, xx, yy, zz);
		break;

	case FreyjaModel::TransformMesh:
		xx = x; 
		yy = y;
		getScreenToWorldOBSOLETE(&xx, &yy);
		mModel->MeshSelect(xx, yy);
		// mModel->selectMesh(plane, xx, yy, zz);
		freyja_print("Selected Mesh[%i]", 
					mModel->getCurrentMesh());
		break;

	case FreyjaModel::TransformBone:
		xx = x; 
		yy = y;
		getScreenToWorldOBSOLETE(&xx, &yy);
		mModel->selectBone(xx, yy);
		// mModel->selectBone(plane, xx, yy, zz);
		freyja_print("Selected Bone[%i] ( Still buggy? )", 
					mModel->getCurrentBone());
		break;

	default:
		freyja_print("WARNING: Selection undefined for this mode");
	}
}
#endif


void FreyjaControl::moveObject(int x, int y, freyja_plane_t plane)
{
	static int old_y = 0, old_x = 0;
	const float t = 2.0f, m = 0.5f;
	vec3_t center;
	float xx = x, yy = y, xf, yf, zf;


	/* Mongoose: Convert screen to world coordinate system */
	//getWorldFromScreen(&xx, &yy, &zz);

#if 0
				float xf, yf;
				const float d = 0.5f, t = 1.0f;
				xf = ((x < old_x-t) ? -d : ((x > old_x+t) ? d : 0));
				yf = ((y < old_y-t) ? -d : ((y > old_y+t) ? d : 0));

				if (mTransformMode == FreyjaModel::TransformBone)
				{
					freyjaGetBoneTranslation3fv(mModel->getCurrentBone(),
												xyz.mVec);
				}

				gFreyjaCursor.mPos.zero();
				mModel->CursorMove(xf, -yf);
				xyz += gFreyjaCursor.mPos;
				gFreyjaCursor.mPos = xyz;

				old_x = x;
				old_y = y;
#endif

	/* Mongoose: Compute a relative movement value too here */
	y = -y;

	switch (plane)
	{
	case PLANE_XY: // front
		xf = ((x < old_x-t) ? -m : ((x > old_x+t) ? m : 0));
		yf = ((y < old_y-t) ? -m : ((y > old_y+t) ? m : 0));
		zf = 0;
		break;

	case PLANE_XZ: // top
		xf = ((x < old_x-t) ? -m : ((x > old_x+t) ? m : 0));
		yf = 0;
		zf = ((y < old_y-t) ? -m : ((y > old_y+t) ? m : 0));
		break;

	case PLANE_ZY: // side
		xf = 0;
		zf = ((x < old_x-t) ? -m : ((x > old_x+t) ? m : 0));
		yf = ((y < old_y-t) ? -m : ((y > old_y+t) ? m : 0));
		break;
	}

	switch (mTransformMode)
	{
	case FreyjaModel::TransformPoint:
		xx = x; 
		yy = -y;
		getScreenToWorldOBSOLETE(&xx, &yy);
		if (FreyjaRender::mPatchDisplayList > 0)
		{
			mModel->movePatchControlPoint(xx, yy);
		}
		else
		{
			{
				freyjaGetVertexXYZ3fv(mModel->getCurrentVertexIndex(),
									  gFreyjaCursor.mPos.mVec);
				FreyjaStateTransform *state = new
				FreyjaStateTransform(fTransformVertex, fTranslate,
									 mModel->getCurrentVertexIndex(),
									 gFreyjaCursor.mPos.mVec);
				gFreyjaCursor.ChangeState(state, Freyja3dCursor::Translation);
			}
			
			mModel->VertexMove(xx, yy);
		}
		break;

	case FreyjaModel::TransformBone:
		xx = x; 
		yy = -y;
		getScreenToWorldOBSOLETE(&xx, &yy);
		mModel->moveBone(xx, yy);
		{
			freyjaGetBoneTranslation3fv(mModel->getCurrentBone(),
										gFreyjaCursor.mPos.mVec);
			FreyjaStateTransform *state = new
			FreyjaStateTransform(fTransformBone, fTranslate,
								 mModel->getCurrentBone(),
								 gFreyjaCursor.mPos.mVec);
			gFreyjaCursor.ChangeState(state, Freyja3dCursor::Translation);
		}
		break;			
		
	case FreyjaModel::TransformMesh:
		xx = x; 
		yy = -y;
		getScreenToWorldOBSOLETE(&xx, &yy);
		mModel->getCurrentMeshCenter(center);

		/* Exact movement based on center of object */
		switch (plane)
		{
		case PLANE_XY:
			xf = xx - center[0];
			yf = yy - center[1];
			zf = 0;
			break;
		case PLANE_XZ:
			xf = xx - center[0];
			yf = 0;
			zf = yy - center[2];
			break;
		case PLANE_ZY: // side
			xf = 0;
			zf = xx - center[2];
			yf = yy - center[1];
			break;
		}

		{
			mModel->getCurrentMeshCenter(gFreyjaCursor.mPos.mVec);
			FreyjaStateTransform *state = new
			FreyjaStateTransform(fTransformMesh, fTranslate,
								 mModel->getCurrentMesh(),
								 gFreyjaCursor.mPos.mVec);
			gFreyjaCursor.ChangeState(state, Freyja3dCursor::Translation);
		}

		mModel->transform(mTransformMode, fTranslate, xf, yf, zf);
		break;

	default:  
		/* Relative movement based on mouse tracking */
		if (plane == PLANE_XZ) zf = -zf;

		mModel->transform(mTransformMode, fTranslate, xf, yf, zf);
		break;
	}

	old_x = x;
	old_y = y;
}


void FreyjaControl::rotateObject(int x, int y, freyja_plane_t plane)
{
	static int old_y = 0, old_x = 0;
	const float t = 1.0f, m = 1.0f;
	//float xr, yr, zr;
	float xf, yf, zf;
	int swap;
	freyja_transform_action_t rotate;


	/* Mongoose: Compute a relative movement value too here */
	swap = x;
	x = -y;
	y = -swap;
	
	switch (plane)
	{
	case PLANE_XY:
		xf = ((x < old_x-t) ? -m : ((x > old_x+t) ? m : 0));
		yf = ((y < old_y-t) ? -m : ((y > old_y+t) ? m : 0));
		zf = 0.0f;
		break;

	case PLANE_XZ:
		xf = ((x < old_x-t) ? -m : ((x > old_x+t) ? m : 0));
		yf = 0.0f;
		zf = ((y < old_y-t) ? m : ((y > old_y+t) ? -m : 0));
		break;

	case PLANE_ZY: //side
		xf = 0.0f;
		zf = ((x < old_x-t) ? -m : ((x > old_x+t) ? m : 0));
		yf = ((y < old_y-t) ? -m : ((y > old_y+t) ? m : 0));
		break;
	}


	switch (mTransformMode)
	{
	case FreyjaModel::TransformBone:
//#define EDIT_LIKE_BONE_0_9_3
#ifdef EDIT_LIKE_BONE_0_9_3
		mModel->getBoneRotation(&xr, &yr, &zr);
		mModel->setBoneRotation(xr + xf, yr + yf, zr + zf);
		mModel->getBoneRotation(&xr, &yr, &zr);

		if (xr > 180.0f)
			mModel->setBoneRotation(-180.0f, yr, zr);

		if (xr > 180.0f)
			mModel->setBoneRotation(xr, -180.0f, zr);

		if (xr > 180.0f)
			mModel->setBoneRotation(xr, yr, -180.0f);
#else
		{
			const vec_t scale = 2.0f;
			vec3_t xyz;

			freyjaGetBoneRotationEuler3fv(mModel->getCurrentBone(), xyz);
			xyz[0] += HEL_DEG_TO_RAD(xf*scale);
			xyz[1] += HEL_DEG_TO_RAD(yf*scale);
			xyz[2] += HEL_DEG_TO_RAD(zf*scale);
		
			// FIXME: Limit range -pi/2 to pi/2

			freyjaBoneRotateEuler3fv(mModel->getCurrentBone(), xyz);
			//freyjaPrintMessage("bone[%i].setEuler(%f, %f, %f)", 
			//                  mModel->getCurrentBone(), xyz[0], xyz[1], xyz[2]);
		}

		rotate = fRotate;

		old_x = x;
		old_y = y;
		return;
#endif
		break;


	case FreyjaModel::TransformMesh:
		/* Mongoose: Scaled rotation for better response */
		xf *= 5.0f;
		yf *= 5.0f;
		zf *= 5.0f;

		rotate = fRotateAboutPoint;
		break;


	default:
		rotate = fRotate;
	}

	mModel->transform(mTransformMode, rotate, xf, yf, zf);
	
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
		case PLANE_XY:
			mModel->transform(mTransformMode, fScale, 1.0, 0.99, 1.0);
			break;
		case PLANE_XZ:
			mModel->transform(mTransformMode, fScale, 1.0, 1.0, 1.01);
			break;
		case PLANE_ZY: // side
			mModel->transform(mTransformMode, fScale, 1.0, 0.99, 1.0);
			break;
		}
	}
	else if (y < old_y)
	{
		switch (plane)
		{
		case PLANE_XY:
			mModel->transform(mTransformMode, fScale, 1.0, 1.01, 1.0);
			break;
		case PLANE_XZ:
			mModel->transform(mTransformMode, fScale, 1.0, 1.0, 0.99);
			break;
		case PLANE_ZY: // side
			mModel->transform(mTransformMode, fScale, 1.0, 1.01, 1.0);
		}
	}
	
	if (x < old_x)
	{
		switch (plane)
		{
		case PLANE_XY:
			mModel->transform(mTransformMode, fScale, 0.99, 1.0, 1.0);
			break;
		case PLANE_XZ:
			mModel->transform(mTransformMode, fScale, 0.99, 1.0, 1.0);
			break;
		case PLANE_ZY: // side
			mModel->transform(mTransformMode, fScale, 1.0, 1.0, 0.99);
			break;
		}
	}
	else if (x > old_x)
	{
		switch (plane)
		{
		case PLANE_XY:
			mModel->transform(mTransformMode, fScale, 1.01, 1.0, 1.0);
			break;
		case PLANE_XZ:
			mModel->transform(mTransformMode, fScale, 1.01, 1.0, 1.0);
			break;
		case PLANE_ZY: // side
			mModel->transform(mTransformMode, fScale, 1.0, 1.0, 1.01);
		}
	}
	
	old_x = x;
	old_y = y;
}


void FreyjaControl::MotionEdit(int x, int y, freyja_plane_t plane)
{
	static int old_y = 0, old_x = 0;
	const float treshold = 2.0f;
	float xyz[3];
	float xx = x, yy = y;
  

	switch (mMouseButton)
	{
	case MOUSE_BTN_MIDDLE:
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
		

		switch (mModel->getCurrentPlane())
		{
		case PLANE_XY: // front
			mModel->adjustSceneTranslation(xyz[0], xyz[1], xyz[2]);
			break;

		case PLANE_XZ: // top
			mModel->adjustSceneTranslation(xyz[0], xyz[2], xyz[1]);
			break;

		case PLANE_ZY: // side
			mModel->adjustSceneTranslation(xyz[2], xyz[1], xyz[0]);
			break;
		}

		old_x = x;
		old_y = y;
		break;


	case MOUSE_BTN_RIGHT:

		break;

	case MOUSE_BTN_LEFT:
		// Mongoose: Convert screen to world system
		getScreenToWorldOBSOLETE(&xx, &yy);

		switch (mEventMode)
		{
		case modeMove:
			moveObject(x, y, plane);
			break;

		case modeRotate: 
			rotateObject(x, y, plane);
			break;
			
		case modeScale:
			scaleObject(x, y, plane);
			break;

		case VERTEX_BBOX_SELECT_MODE:
			mModel->BBoxMove(xx, yy);
			break;
			
		case TAG_MOVE_CENTER:
			mModel->moveBoneCenter(xx, yy);
			break;

		case MESH_MOVE_CENTER: 
			mModel->MeshMoveCenter(xx, yy);
			break;

		default:
			break;
		}

		old_x = x;
		old_y = y;
		break;
	}
}


void FreyjaControl::MouseEdit(int btn, int state, int mod, int x, int y, 
							  freyja_plane_t plane) 
{ 
	vec3_t xyz;
	unsigned int master_tag, i;
	static float xxx, yyy;


	// Get the viewport adjusted mouse coordinates, and swap modes if needed
	vec_t vx = x, vy = y;
	AdjustMouseXYForViewports(vx, vy);

	bool nEvent = true;
	switch (mEventMode)
	{
	case modeSelect:
		SelectObject(vx, vy);
		break;

	case modeUnselect:
		UnselectObject(vx, vy);
		break;

	default:
		nEvent = false;
	}

	if (nEvent)
		return;
	
	vec_t xx = vx, yy = vy;
	getScreenToWorldOBSOLETE(&xx, &yy);
	
	switch (plane)
	{
	case PLANE_XY: // front
		xyz[0] = xx;
		xyz[1] = yy;
		xyz[2] = 0;
		break;
	case PLANE_XZ: // top
		xyz[0] = xx;
		xyz[1] = 0;
		xyz[2] = yy;
		break;
	case PLANE_ZY: // side
		xyz[0] = 0;
		xyz[1] = yy;
		xyz[2] = xx;
		break;
	}

	if (!(btn == MOUSE_BTN_LEFT && state == MOUSE_BTN_STATE_PRESSED))
	{	
		switch(mEventMode)
		{
		case modeMove:
			if ( mTransformMode == FreyjaModel::TransformMesh )
			{
				mModel->getCurrentMeshCenter(gFreyjaCursor.mPos.mVec);
				FreyjaStateTransform *state = new
				FreyjaStateTransform(fTransformMesh, fTranslate,
									 mModel->getCurrentMesh(),
									 gFreyjaCursor.mPos.mVec);
				gFreyjaCursor.ForceChangeState(state, Freyja3dCursor::Translation);
			}
			else if ( mTransformMode == FreyjaModel::TransformPoint )
			{
				unsigned int idx = mModel->getCurrentVertexIndex();
				freyjaGetVertexXYZ3fv(idx, gFreyjaCursor.mPos.mVec);
				FreyjaStateTransform *state = new
				FreyjaStateTransform(fTransformVertex, fTranslate,
									 idx, gFreyjaCursor.mPos.mVec);
				gFreyjaCursor.ForceChangeState(state, Freyja3dCursor::Translation);
			}
			freyja_print("! moved: %f, %f", xx-xxx, yy-yyy);
			break;
		default:
			;
		}

		return;
	}

	switch(mEventMode)
	{
	case modeMove:
		//if (mXYZMouseState == 0)
		//{
		xxx = xx;  yyy = yy;
		freyja_print("! store state: %f, %f", xxx, yyy);
		mModel->VertexSelect(xx, yy);

		if (FreyjaRender::mPatchDisplayList)
			mModel->selectPatchControlPoint(xx, yy);

		mXYZMouseState = 1;
		//}
		//else
		//{
		//	mXYZMouseState = 0;
		//}
		break;

	case modeSelect:
		SelectObject(vx, vy); //, plane);
		break;


	case VERTEX_COMBINE:
		if (mXYZMouseState == 0)
		{
			mModel->VertexCombine(xx, yy);
			 mXYZMouseState = 1;
		}
		else
		{
			mXYZMouseState = 0;
		}
		break;
	case VERTEX_BBOX_SELECT_MODE:
		if (mXYZMouseState == 0)
		{
			mModel->BBoxSelect(xx, yy);
			mXYZMouseState = 1;
		}
		else
			mXYZMouseState = 0;
      break;
	case POINT_DEL_MODE:
		mModel->VertexSelect(xx, yy);
		mModel->VertexDelete();
		break;
	case BONE_CONNECT_MODE:
		master_tag = mModel->getCurrentBone();
		mModel->selectBone(xx, yy);
		mModel->connectBone(master_tag, mModel->getCurrentBone());
		mModel->setCurrentBone(master_tag);
		break;
	case BONE_DISCONNECT_MODE:
		master_tag = mModel->getCurrentBone();
		mModel->selectBone(xx, yy);
		mModel->removeMeshFromBone(master_tag, mModel->getCurrentBone());
		mModel->setCurrentBone(master_tag);
		break;
	case MESH_MOVE_CENTER:
		if (mXYZMouseState == 0)
			mXYZMouseState = 1;
		else
			mXYZMouseState = 0;
		break;
	case POINT_ADD_MODE: 
		mModel->VertexNew(xx, yy);
		break;
	case BONE_ADD_MODE:
		i = mModel->newBone(xyz[0], xyz[1], xyz[2], 0x0);
		freyja_print("New bone[%i] created", i);
		break;
	case POLYGON_ADD_MODE:
		mModel->PolygonAddVertex(xx, yy);
		break;
	case POLYGON_DEL_MODE:
		mModel->PolygonDelVertex(xx, yy);
		break;
	case POLYGON_SELECT_MODE:
		mModel->PolygonSelectVertex(xx, yy);
		break;
	default:
		;
	}
}


void FreyjaControl::loadResource()
{
	int i, x, y;
	char *s;
	char *filename = "freyja-chimera.mlisp";
	bool failed = true;

	
	s = freyja_rc_map(filename);

	if (!mResource->Load(s))
	{
		failed = false;
	}

	delete [] s;

	if (failed)
	{
		printf("FreyjaControl::ReadRC> ERROR: Couldn't find '%s'\n", s);
		freyja_event_shutdown();
	}


	/* GUI stuff */
	if (mResource->Lookup("WINDOW_X", &x))
	{
		if (mResource->Lookup("WINDOW_Y", &y))
		{
			extern void application_window_move(int x, int y);
			freyja_application_window_move(x, y);
		}
	}

	if (mResource->Lookup("DUMP_SYMTAB", &i) && i)
	{
		mResource->Print();
	}

	if (mResource->Lookup("GRID_ON", &i))
	{
		if (i)
		{
			mRender->setFlag(FreyjaRender::RENDER_EDIT_GRID);
		}
		else
		{
			mRender->clearFlag(FreyjaRender::RENDER_EDIT_GRID);
		}
	}

	if (mResource->Lookup("FLUSH_RESOURCE", &i) && i)
	{
		mResource->Flush();
	}


	/* Recent files persistance test */
	FreyjaFileReader r;
	uint32 j;
	s = freyja_rc_map("recent_files");

	if (r.openFile(s))
	{
		if (s) 
		{
			delete [] s;
		}

		for (j = 0; j < gRecentFileLimit && !r.endOfFile(); ++j)
		{
			s = r.parseSymbol();
			addRecentFilename(s);
		}

		r.closeFile();
	}
}




