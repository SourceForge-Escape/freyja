/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.wetga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : FreyjaControl
 * License : No use w/o permission (C) 2000 Mongoose
 * Comments: This is the controler class for the client
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
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
#include <string.h>

#include "FreyjaControl.h"

extern unsigned int gBoneRenderType;
extern unsigned char gJointRenderType;


void event_register_render(FreyjaRender *r);
void event_register_model(FreyjaModel *m);
void event_register_control(FreyjaControl *c);


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
	mRender->Register(mModel);

	if (EGG_LIB_VERSION != 0x32312E38)
	{
		freyja_print("FreyjaControl::FreyjaControl> Assertion failure");
		freyja_print("*** You must upgrade libfreyja_model to v8.12");
		printf("Please read the /tmp/Freyja.log for errors.\n");
		exit(-1);
	}

	if (!mModel || !mRender)
	{
		freyja_print("FreyjaControl::FreyjaControl> Assertion failure");
	
		if (!mRender)
			freyja_print("FreyjaControl::FreyjaControl> FreyjaRender = 0x0");

		if (!mModel)
			freyja_print("FreyjaControl::FreyjaControl> FreyjaModel = 0x0");

		printf("Please read the /tmp/Freyja.log for errors.\n");
		exit(-1);
	}
		
	event_register_render(mRender);
	event_register_model(mModel);
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
	mRender->Init(width, height, true);
	mRender->Reshape(width, height);
	mMaterial = MaterialManager::Instance();
}


FreyjaControl::~FreyjaControl()
{
	unsigned int i;


	for (i = mRecentFiles.begin(); i <  mRecentFiles.end(); ++i)
	{
		// tmp fun
		freyja_print("(add_recent_file \"%s\")\n", mRecentFiles[i]);
		delete [] mRecentFiles[i];
	}

	/* Mongoose 2004.03.26, 
	 * Entry for MaterialManager test pattern */
	MaterialManager::DestroyInstance();
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

float FreyjaControl::getZoom()
{
	return mRender->getZoom();
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void FreyjaControl::addRecentFilename(const char *filename)
{
	unsigned int l;
	char *dupe;

	if (!filename || !filename[0])
	{
		return;
	}

	l = strlen(filename);
	dupe = new char[l+1];
	strncpy(dupe, filename, l);
	dupe[l] = 0;

	mRecentFiles.pushBack(dupe);
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


	switch (event)
	{
	case 503:
		if (value != mModel->getCurrentTextureIndex())
		{
			mModel->setCurrentTextureIndex(value);
			freyja_event_gl_refresh();
		}
		break;


	case eAnimationSlider:
		if (value != mModel->getCurrentAnimationFrame())
		{
			mModel->setCurrentAnimationFrame(value);	
			freyja_event_gl_refresh();
		}
		break;


	case eBoneIterator:
		if (!freyja_event_set_range(event, value, 0, eggGetNum(FREYJA_BONE)))
		{
			mModel->setCurrentBone(value);
			//value = gFreyjaModel->getCurrentBone();
			//spinbutton_value_set(event, value);

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
		}
		break;


	case eMeshIterator:
		if (!freyja_event_set_range(event, value, 0, eggGetNum(FREYJA_MESH)))
		{
			mModel->setCurrentMesh(value);			
			freyja_event_gl_refresh();
			freyja_print("Selecting mesh[%i] ...", value);
		}
		break;


	case eGroupIterator:
		if (!freyja_event_set_range(event, value, 0, eggGetNum(FREYJA_GROUP)))
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
		mMaterial->setTexture(0, value);
		freyja_print("Material[%i].texture[%d] = %i",
					mMaterial->getCurrent(), 0,
					mMaterial->getTexture(0));
		freyja_event_gl_refresh();
		break;


	default:
		freyja_print("!Unhandled { event = %d, value = %u }", event, value);
		return false;
	}

	return true;
}


bool FreyjaControl::event(int event, vec_t value)
{
	vec4_t color;
	vec_t x, y, z;
	vec_t *ptr;


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
		gMaterialManager->getColor(MaterialManager::eAmbient, color);
		color[event - 700] = value;
		gMaterialManager->setColor(MaterialManager::eAmbient, color);
		freyja_event_gl_refresh();
		break;


	case 704:
	case 705:
	case 706:
	case 707:
		gMaterialManager->getColor(MaterialManager::eDiffuse, color);
		color[event - 704] = value;
		gMaterialManager->setColor(MaterialManager::eDiffuse, color);
		freyja_event_gl_refresh();
		break;


	case 708:
	case 709:
	case 710:
	case 711:
		gMaterialManager->getColor(MaterialManager::eSpecular, color);
		color[event - 708] = value;
		gMaterialManager->setColor(MaterialManager::eSpecular, color);
		freyja_event_gl_refresh();
		break;


	case 712:
	case 713:
	case 714:
	case 715:
		gMaterialManager->getColor(MaterialManager::eEmissive, color);
		color[event - 712] = value;
		gMaterialManager->setColor(MaterialManager::eEmissive, color);
		freyja_event_gl_refresh();
		break;


	case 716:
		gMaterialManager->setShininess(value);
		freyja_event_gl_refresh();
		break;


	case 800:
	case 801:
	case 802:
		ptr = mModel->GetLight0Pos();
		ptr[event - 800] = value;
		freyja_event_gl_refresh();
		break;


	case eZoom:
		setZoom(value);//freyja_event_get_float(eZoom));
		freyja_print("Zoom %f", mRender->getZoom());
		freyja_event_gl_refresh();
		break;


	default:
		freyja_print("!Unhandled { event = %d, value = %f }", event, value);
		return false;
	}

	return true;
}



bool FreyjaControl::event(int command)
{
	unsigned int i, flags;


	switch (command)
	{
	case 1000:
		gMaterialManager->setBlendSource(GL_ZERO);
		break;
	case 1001:
		gMaterialManager->setBlendSource(GL_ONE);
		break;
	case 1002:
		gMaterialManager->setBlendSource(GL_SRC_COLOR);
		break;
	case 1003:
		gMaterialManager->setBlendSource(GL_ONE_MINUS_SRC_COLOR);
		break;
	case 1004:
		gMaterialManager->setBlendSource(GL_DST_COLOR);
		break;
	case 1005:
		gMaterialManager->setBlendSource(GL_ONE_MINUS_DST_COLOR);
		break;
	case 1006:
		gMaterialManager->setBlendSource(GL_SRC_ALPHA);
		break;
	case 1007:
		gMaterialManager->setBlendSource(GL_ONE_MINUS_SRC_ALPHA);
		break;
	case 1008:
		gMaterialManager->setBlendSource(GL_DST_ALPHA);
		break;
	case 1009:
		gMaterialManager->setBlendSource(GL_ONE_MINUS_DST_ALPHA);
		break;
	case 1010:
		gMaterialManager->setBlendSource(GL_SRC_ALPHA_SATURATE);
		break;
	case 1011:
		gMaterialManager->setBlendSource(GL_CONSTANT_COLOR);
		break;
	case 1012:
		gMaterialManager->setBlendSource(GL_ONE_MINUS_CONSTANT_COLOR);
		break;
	case 1013:
		gMaterialManager->setBlendSource(GL_CONSTANT_ALPHA);
		break;
	case 1014:
		gMaterialManager->setBlendSource(GL_ONE_MINUS_CONSTANT_ALPHA);
		break;


	case 2000:
		gMaterialManager->setBlendDest(GL_ZERO);
		break;
	case 2001:
		gMaterialManager->setBlendDest(GL_ONE);
		break;
	case 2002:
		gMaterialManager->setBlendDest(GL_SRC_COLOR);
		break;
	case 2003:
		gMaterialManager->setBlendDest(GL_ONE_MINUS_SRC_COLOR);
		break;
	case 2004:
		gMaterialManager->setBlendDest(GL_DST_COLOR);
		break;
	case 2005:
		gMaterialManager->setBlendDest(GL_ONE_MINUS_DST_COLOR);
		break;
	case 2006:
		gMaterialManager->setBlendDest(GL_SRC_ALPHA);
		break;
	case 2007:
		gMaterialManager->setBlendDest(GL_ONE_MINUS_SRC_ALPHA);
		break;
	case 2008:
		gMaterialManager->setBlendDest(GL_DST_ALPHA);
		break;
	case 2009:
		gMaterialManager->setBlendDest(GL_ONE_MINUS_DST_ALPHA);
		break;
	case 2010:
		gMaterialManager->setBlendDest(GL_SRC_ALPHA_SATURATE);
		break;
	case 2011:
		gMaterialManager->setBlendDest(GL_CONSTANT_COLOR);
		break;
	case 2012:
		gMaterialManager->setBlendDest(GL_ONE_MINUS_CONSTANT_COLOR);
		break;
	case 2013:
		gMaterialManager->setBlendDest(GL_CONSTANT_ALPHA);
		break;
	case 2014:
		gMaterialManager->setBlendDest(GL_ONE_MINUS_CONSTANT_ALPHA);
		break;

	case eMaterialTex:
		flags = gMaterialManager->getFlags();
		
		if (flags & Material::fEnable_Texture)
			gMaterialManager->clearFlag(Material::fEnable_Texture);
		else
			gMaterialManager->setFlag(Material::fEnable_Texture);
		
		freyja_print("OpenGL texturing is [%s]", 
					 !(flags & Material::fEnable_Texture) ? "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case eMaterialMultiTex:
		flags = gMaterialManager->getFlags();

		if (flags & Material::fEnable_DetailTexture)
			gMaterialManager->clearFlag(Material::fEnable_DetailTexture);
		else
			gMaterialManager->setFlag(Material::fEnable_DetailTexture);

		freyja_print("OpenGL detail texturing is [%s]", 
					 !(flags & Material::fEnable_DetailTexture) ? "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case eOpenGLNormalize:
		flags = gMaterialManager->getFlags();

		if (flags & Material::fEnable_Normalize)
			gMaterialManager->clearFlag(Material::fEnable_Normalize);
		else
			gMaterialManager->setFlag(Material::fEnable_Normalize);

		freyja_print("OpenGL normalization of normals is [%s]", 
					 !(flags & Material::fEnable_Normalize) ? "ON" : "OFF");
		freyja_event_gl_refresh();
		break;


	case eOpenGLBlend:
		flags = gMaterialManager->getFlags();
		
		if (flags & Material::fEnable_Blending)
			gMaterialManager->clearFlag(Material::fEnable_Blending);
		else
			gMaterialManager->setFlag(Material::fEnable_Blending);
		
		freyja_print("OpenGL blending [%s]", 
					 !(flags & Material::fEnable_Blending) ? "ON" : "OFF");
		freyja_event_gl_refresh();
		break;


	case eInfo:
		mModel->printInfo();
		break;

	case eHelpDialog:
		extern void mgtk_event_dialog_visible_set(int dialog, int visible);
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
				mModel->Clear();
				freyja_print("Closing Model...");
				freyja_set_main_window_title(BUILD_NAME);
			}
			break;
		case MATERIAL_EDIT_MODE:
			i = mMaterial->createNewMaterial();
			freyja_print("New material [%i] created.", i);
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

				mModel->Clear();
				freyja_print("Closing Model...");
				freyja_set_main_window_title(BUILD_NAME);
			}
			break;
		case MATERIAL_EDIT_MODE:
			i = mMaterial->createNewMaterial();
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
			mFileDialogMode = FREYJA_MODE_LOAD_MODEL;
			freyja_event_file_dialog("Open model...");
			break;
		case MATERIAL_EDIT_MODE:
			mFileDialogMode = FREYJA_MODE_LOAD_MATERIAL;
			freyja_event_file_dialog("Open material...");
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
			mFileDialogMode = FREYJA_MODE_SAVE_MODEL;
			freyja_event_file_dialog("Save model as...");
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


	case eShutdown:
		if (freyja_create_confirm_dialog("gtk-dialog-question",
						  "If you exit you will lose any unsaved data.",
						  "Would you like to exit?", 
						  "gtk-cancel", "_Cancel", "gtk-quit", "_Exit"))
		{
			freyja_event_exit();
		}
		break;


	case eCamera:
		mRender->Flags(FreyjaRender::RENDER_CAMERA, 
					   !(mRender->Flags() & FreyjaRender::RENDER_CAMERA));
		freyja_print("Camera rendering [%s]", 
					(mRender->Flags() & FreyjaRender::RENDER_CAMERA) ? 
					"ON" : "OFF");
		break;


	case eDebugEgg:
		mModel->Debug(!mModel->Debug());
		freyja_print("Egg debug [%s]", mModel->Debug() ? "ON" : "OFF");
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
		mRender->ScreenShot();
		break;


	case eGenerateNormals:
		eggGenerateVertexNormals();
		break;


	case eGenerateCone:
		freyja_print("eGenerateCone not backported");
		break;


	case eGenerateCylinder:
		eggGenerateCylinder(4, 16, 8.0, 4.0);
		freyja_event_gl_refresh();
		break;


	case eGenerateSphere:
		eggGenerateSphere(16, 16, 8.0);
		freyja_event_gl_refresh();
		break;


	case eGenerateCube:
		eggGenerateCube(8.0);
		freyja_event_gl_refresh();
		break;


	case eGenerateTriStrip:
		eggGenerateTriangleStrip(10, 8.0);
		freyja_event_gl_refresh();
		break;

	case ePointJoint:
		gJointRenderType = 1;
		break;

	case eSphereJoint:
		gJointRenderType = 2;
		break;

	case eAxisJoint:
		gJointRenderType = 3;
		break;

	case eLineBone:
		gBoneRenderType = 1;
		break;

	case ePolyMeshBone:
		gBoneRenderType = 2;
		break;


	case eRenderBbox:
		mRender->Flags(FreyjaRender::RENDER_BBOX, 
					   !(mRender->Flags() & FreyjaRender::RENDER_BBOX));
		freyja_print("BoundingBox rendering [%s]", 
					(mRender->Flags() & FreyjaRender::RENDER_BBOX) ? 
					"ON" : "OFF");
		break;


	case eSetMeshTexture:
		freyja_print("Switching all of mesh %i ploygons to texture %i",
					mModel->getCurrentMesh(), mModel->getCurrentTextureIndex());
		mModel->TextureShift();
		break;


	case eTextureSlotLoad:
		if (!mMaterial->getGeneralFlags() & 
			MaterialManager::fLoadTextureInSlot)
		{
			mMaterial->setGeneralFlag(MaterialManager::fLoadTextureInSlot);
		}
		else
		{
			mMaterial->clearGeneralFlag(MaterialManager::fLoadTextureInSlot);
		}	

		freyja_print("Texture loading to current slot [%s]",
					mMaterial->getGeneralFlags() & 
					MaterialManager::fLoadTextureInSlot ? "on" : "off");
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
		break;

	case eAddObject:
		addObject();
		break;

	case eCopyAppendMode:
		mModel->appendMode = !mModel->appendMode;
		break;

	case eCut:
		if (copySelectedObject())
			deleteSelectedObject();
		break;
	case eCopy:
		copySelectedObject();
		break;
	case ePaste:
		pasteSelectedObject();
		break;


	case eMoveObject:
		mEventMode = modeMove;
		freyja_print("Move object...");
		break;

	case eScaleObject:
		mEventMode = modeScale;
		freyja_print("Scale object...");
		break;

	case eRotateObject:
		mEventMode = modeRotate;
		freyja_print("Rotate object...");
		break;

	case eSelect:
		mEventMode = modeSelect;
		freyja_print("Select object...");
		break;


	case eMove:
		mModel->transform(mTransformMode, Egg::TRANSLATE,
						  freyja_event_get_float(eMove_X),
						  freyja_event_get_float(eMove_Y),
						  freyja_event_get_float(eMove_Z));
		freyja_event_gl_refresh();
		
		freyja_event_set_float(eMove_X, 0.0f);
		freyja_event_set_float(eMove_Y, 0.0f);
		freyja_event_set_float(eMove_Z, 0.0f);
		break;

	case eRotate:
		mModel->transform(mTransformMode, Egg::ROTATE,
						  freyja_event_get_float(eRotate_X),
						  freyja_event_get_float(eRotate_Y),
						  freyja_event_get_float(eRotate_Z));
		freyja_event_gl_refresh();
		
		freyja_event_set_float(eRotate_X, 0.0f);
		freyja_event_set_float(eRotate_Y, 0.0f);
		freyja_event_set_float(eRotate_Z, 0.0f);
		break;

	case eScale:
		mModel->transform(mTransformMode, Egg::SCALE,
						  freyja_event_get_float(eScale_X),
						  freyja_event_get_float(eScale_Y),
						  freyja_event_get_float(eScale_Z));
		freyja_event_gl_refresh();
		
		freyja_event_set_float(eScale_X, 1.0f);
		freyja_event_set_float(eScale_Y, 1.0f);
		freyja_event_set_float(eScale_Z, 1.0f);
		break;


		/* BONES */
	case CMD_BONE_CONNECT:
		mTransformMode = FreyjaModel::TransformBone;
		freyja_print("Select bone to connect to current");
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


		/* MESHES */
	case eTransformMeshPivot:
		mTransformMode = FreyjaModel::TransformMesh;
		mEventMode = MESH_MOVE_CENTER;
		freyja_print("Reposition mesh center point");
		break;


		/* ANIMATIONS */
	case eAnimationPlay:
		freyja_print("eAnimationPlay disabled / no longer implemented");
		break;
	case eAnimationStop:
		freyja_print("eAnimationStop disabled / no longer implemented");
		break;
	case eAnimationNext:
		mModel->setCurrentAnimation(mModel->getCurrentAnimation() + 1);
		freyja_print("Animation[%i].", mModel->getCurrentAnimation());
		break;
	case eAnimationPrev:
		if (mModel->getCurrentAnimation())
			mModel->setCurrentAnimation(mModel->getCurrentAnimation() - 1);
		else
			mModel->setCurrentAnimation(0);

		freyja_print("Animation[%i].", mModel->getCurrentAnimation());
		break;



	case eSelectMaterial:
		i = (int)freyja_event_get_float(eSelectMaterial);

		if (i != mMaterial->getCurrent())
		{
			mMaterial->setCurrent(i);
			freyja_event_set_float(eSelectMaterial, 
								   mMaterial->getCurrent());
			freyja_print("Selected material[%i].", i);

			freyja_refresh_material_interface();
			freyja_event_gl_refresh();
		}
		break;


	case eZoom:
		setZoom(freyja_event_get_float(eZoom));
		freyja_event_gl_refresh();
		break;


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


	case CMD_MISC_BBOX_SELECT:
		if (mEventMode == VERTEX_BBOX_SELECT_MODE)
		{
			mEventMode = modeNone;
			mRender->Flags(FreyjaRender::RENDER_BBOX_SEL, 0);
			mModel->BBoxListBuild();
			freyja_print("Vertex box select: Vertex list built");
		}
		else
		{
			mEventMode = VERTEX_BBOX_SELECT_MODE;
			mRender->Flags(FreyjaRender::RENDER_BBOX_SEL, 1);
			freyja_print("Vertex box select: Press agian to end selection");
		}
		break;


	case CMD_MISC_TEXEL_COMBINE:
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
		mModel->generateUVMap();
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


	case FREYJA_MODE_RENDER_BONETAG:
		mRender->Flags(FreyjaRender::RENDER_BONES, 
					   !(mRender->Flags() & FreyjaRender::RENDER_BONES));
		freyja_print("Bone Rendering [%s]", 
					 (mRender->Flags() & FreyjaRender::RENDER_BONES) ? 
					 "ON" : "OFF");
		freyja_event_gl_refresh();
		break;
	case FREYJA_MODE_RENDER_POINTS:
		mRender->Flags(FreyjaRender::RENDER_POINTS, 
					   !(mRender->Flags() & FreyjaRender::RENDER_POINTS));
		freyja_print("Point Rendering [%s]", 
					 (mRender->Flags() & FreyjaRender::RENDER_POINTS) ? 
					 "ON" : "OFF");
		freyja_event_gl_refresh();
		break;
	case FREYJA_MODE_RENDER_WIREFRAME:
		mRender->Flags(FreyjaRender::RENDER_WIREFRAME, 
					   !(mRender->Flags() & FreyjaRender::RENDER_WIREFRAME));
		freyja_print("Wireframe Rendering [%s]", 
					 (mRender->Flags() & FreyjaRender::RENDER_WIREFRAME) ? 
					 "ON" : "OFF");
		freyja_event_gl_refresh();
		break;
	case FREYJA_MODE_RENDER_FACE:
		mRender->Flags(FreyjaRender::RENDER_FACE, 
					   !(mRender->Flags() & FreyjaRender::RENDER_FACE));
		freyja_print("Face Rendering [%s]", 
					 (mRender->Flags() & FreyjaRender::RENDER_FACE) ? 
					 "ON" : "OFF");
		freyja_event_gl_refresh();
		break;
	case FREYJA_MODE_RENDER_NORMALS:
		mRender->Flags(FreyjaRender::RENDER_NORMALS, 
					   !(mRender->Flags() & FreyjaRender::RENDER_NORMALS));
		freyja_print("Normal Rendering [%s]", 
					 (mRender->Flags() & FreyjaRender::RENDER_NORMALS) ? 
					 "ON" : "OFF");
		freyja_event_gl_refresh();
		break;
	case FREYJA_MODE_RENDER_LIGHTING:
		mRender->Flags(FreyjaRender::RENDER_NORMAL, 
					   !(mRender->Flags() & FreyjaRender::RENDER_NORMAL));
		mRender->Flags(FreyjaRender::RENDER_LIGHTING, 
					   !(mRender->Flags() & FreyjaRender::RENDER_LIGHTING));
		freyja_print("GL Lighting is [%s]", 
					 (mRender->Flags() & FreyjaRender::RENDER_LIGHTING) ? 
					 "ON" : "OFF");
		freyja_event_gl_refresh();
		break;
	case FREYJA_MODE_RENDER_TEXTURE:
		mRender->Flags(FreyjaRender::RENDER_TEXTURE, 
					   !(mRender->Flags() & FreyjaRender::RENDER_TEXTURE));
		freyja_print("Texture rendering is [%s]", 
					 (mRender->Flags() & FreyjaRender::RENDER_TEXTURE) ? 
					 "ON" : "OFF");
		freyja_event_gl_refresh();
		break;
	case FREYJA_MODE_RENDER_MATERIAL:
		mRender->Flags(FreyjaRender::RENDER_MATERIAL, 
					   !(mRender->Flags() & FreyjaRender::RENDER_MATERIAL));
		freyja_print("Material rendering is [%s]", 
					 (mRender->Flags() & FreyjaRender::RENDER_MATERIAL) ? 
					 "ON" : "OFF");
		freyja_event_gl_refresh();
		break;
	case FREYJA_MODE_RENDER_GRID:
		mRender->Flags(FreyjaRender::RENDER_EDIT_GRID, 
					   !(mRender->Flags() & FreyjaRender::RENDER_EDIT_GRID));
		freyja_print("Edit Grid rendering [%s]",
					 (mRender->Flags() & FreyjaRender::RENDER_EDIT_GRID) ? 
					 "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_TEXTURE_EDIT:
		mRender->ViewMode(VIEWMODE_TEXTURE_EDIT);
		freyja_event_gl_refresh();
		freyja_print("Texture Edit");
		mEditorMode = TEXTURE_EDIT_MODE;
		break;
	case FREYJA_MODE_MODEL_EDIT:
		mRender->ViewMode(VIEWMODE_MODEL_EDIT);
		freyja_event_gl_refresh();
		freyja_print("Model Edit");
		mEditorMode = MODEL_EDIT_MODE;
		break;
	case FREYJA_MODE_MODEL_VIEW:
		mRender->ViewMode(VIEWMODE_MODEL_VIEW);
		freyja_event_gl_refresh();
		freyja_print("Animation View");
		mEditorMode = ANIMATION_EDIT_MODE;
		break;
	case FREYJA_MODE_MATERIAL_EDIT:
		mRender->ViewMode(VIEWMODE_MATERIAL_EDIT);
		freyja_event_gl_refresh();
		freyja_print("Material Edit");
		mEditorMode = MATERIAL_EDIT_MODE;
		break;
	case FREYJA_MODE_PLANE_XY:
		freyja_print("Plane XY");      
		mModel->CurrentPlane(Egg::PLANE_XY);
		freyja_event_gl_refresh();
		break;
	case FREYJA_MODE_PLANE_XZ:
		freyja_print("Plane XZ");      
		mModel->CurrentPlane(Egg::PLANE_XZ);
		freyja_event_gl_refresh();
		break;
	case FREYJA_MODE_PLANE_YZ:
		freyja_print("Plane YZ");      
		mModel->CurrentPlane(Egg::PLANE_YZ);
		freyja_event_gl_refresh();
		break;



	case CMD_POLYGON_DELETE:
		mEventMode = POLYGON_DEL_MODE;
		freyja_print("Select polygon by vertices to delete");
		break;
	case CMD_POLYGON_ADD:
		mEventMode = POLYGON_ADD_MODE;
		freyja_print("Select vertices to create a polygon");
		break;
	case CMD_POLYGON_SELECT:
		mEventMode = POLYGON_SELECT_MODE;
		freyja_print("Select polygon by vertices to highlight it");
		break;



	case CMD_POINT_DELETE:
		mEventMode = POINT_DEL_MODE;
		freyja_print("Select vertex to delete");
		break;
	case CMD_POINT_ADD:
		mEventMode = POINT_ADD_MODE;
		freyja_print("Select point in space to create a vertex");
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

		freyja_print("Vertex combine [%s]", 
					 (mEventMode == VERTEX_COMBINE) ? "on" : "off");
		break;

	default:
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
		failed = mMaterial->load(filename);
		type = 0;
		type2 = 1;
		break;
	case FREYJA_MODE_SAVE_MATERIAL:
		failed = mMaterial->save(filename);
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
		}

		type = 2;
		type2 = 1;
		break;
	case FREYJA_MODE_LOAD_TEXTURE:
		failed = mMaterial->loadTexture(filename);
		type = 1;
		type2 = 1;
		freyja_event_gl_refresh();
		break;
	case FREYJA_MODE_SAVE_MODEL:
		failed = mModel->saveModel(filename);
		type = 2;
		type2 = 0;
		break;


	case FREYJA_MODE_SAVE_ANIMATION:
		failed = 0;
		mModel->saveAnimation(filename);
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
					"ERROR: No event for ",
					basename(filename),
					(type2 == 0) ? "save " : "load ",
					(failed == 0) ? "[sucess]" : "[failed]");
}


void FreyjaControl::handleTextEvent(int event, const char *text)
{
	switch (event)
	{
	case eSetMaterialName:
		mMaterial->setName(text);
		break;

	case eSetTextureNameA:
		/* Text here is assumed to be a filename */
		//mMaterial->loadTexture(text);
		break;

	case eSetTextureNameB:
		/* Text here is assumed to be a filename */
		//mMaterial->loadDetailTexture(text);
		break;

	case eSetCurrentBoneName:
		mModel->nameBone(mModel->getCurrentBone(), text);
		break;

	default:
		freyja_print("Unhandled event(%i) '%s'", event, text);
	}
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
				mRender->Rotate(Y_F, -mRender->RotateAmount());
			else if (x < old_x)
				mRender->Rotate(Y_F, mRender->RotateAmount());
			
			if (y > old_y)
				mRender->Rotate(X_F, -mRender->RotateAmount());
			else if (y < old_y)
				mRender->Rotate(X_F, mRender->RotateAmount());

			old_x = x;
			old_y = y;
			break;

		case MOUSE_BTN_MIDDLE:
			break;
			
		case MOUSE_BTN_LEFT:
			break;
		}
		
		break;


	case TEXTURE_EDIT_MODE:
		if (_tex_state)
		{
			float s;
			float t;
			
			s = (float)x / (float)mRender->Width();
			t = (float)y / (float)mRender->Height();
			
			if (s > 1.0) s = 1.0;
			if (s < 0.0) s = 0.0;
			if (t > 1.0) t = 1.0;
			if (t < 0.0) t = 0.0;
			
			mModel->TexelMove(s, t);
			return true;
	  }
		break;


	case MODEL_EDIT_MODE:
		MotionEdit(x, y, mModel->CurrentPlane());
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

	switch (mEditorMode)
	{
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

		break;
	case TEXTURE_EDIT_MODE:
		if (_tex_state)
		{
			_tex_state = false;
			return true;
		}
		else if (btn == MOUSE_BTN_LEFT && state == MOUSE_BTN_STATE_PRESSED)
		{
			float s, t;
			
			
			s = (float)x / (float)mRender->Width();
			t = (float)y / (float)mRender->Height();
			
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
			
			_tex_state = true;
			return true;
		}
		break;
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

		MouseEdit(btn, state, mod, x, y, mModel->CurrentPlane());
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
		freyja_print("WARNING: Using FreyjaControl::getScreenToWorldOBSOLETE");
	}

	getWorldFromScreen(x, y, &z);

	switch (mModel->CurrentPlane())
	{
	case Egg::PLANE_XY:
		/* Nothing to do for XY */
		break;
		
	case Egg::PLANE_XZ:
		*y = z;
		break;
		
	case Egg::PLANE_YZ:
		*x = *y;
		*y = z;
		break;
	}
}


void FreyjaControl::getWorldFromScreen(float *x, float *y, float *z)
{
	float width, height, invz, fs;
	float scroll[3];
 

	width = mRender->Width();
	height = mRender->Height();
	mModel->getSceneTranslation(scroll);

#ifdef DEBUG_SCREEN_TO_WORLD
	printf("Screen (%.3f, %.3f); aspect_ratio = %.3f; zoom = %.3f\n", 
			 *x, *y, width/height, mModel->getZoom());
	printf("Scroll (%.3f, %.3f, %.3f)\n", 
			 scroll[0], scroll[1], scroll[2]);
#endif

	invz = (1.0 / mRender->getZoom());
	fs = (40.0 * invz) / height;  // fov 40?

	*x = (*x - width / 2.0) * fs;
	*y = -(*y - height / 2.0) * fs;
	
	switch (mModel->CurrentPlane())
	{
	case Egg::PLANE_XY:
		*x -= scroll[0] * invz;
		*y -= scroll[1] * invz;
		*z = 0.0f;
		break;

	case Egg::PLANE_XZ:
		*x -= scroll[0] * invz;
		*z = *y - scroll[2] * invz;
		*y = 0.0f;
		break;

	case Egg::PLANE_YZ:
		*z = *y - scroll[2] * invz;
		*y = *x - scroll[1] * invz;
		*x = 0.0f;
		break;
	}

#ifdef DEBUG_SCREEN_TO_WORLD
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
	case FreyjaModel::TransformMesh:
		return mModel->pasteSelectedMesh();
		break;

	default:
		freyja_print("This object type is not supported by Generic Paste.");
		return false;
	}

	return true;
}


// FIXME: (int x, int y, Egg::egg_plane plane) for vertex / texel / polygon
void FreyjaControl::deleteSelectedObject()
{
	switch (mTransformMode)
	{
	case FreyjaModel::TransformPoint:
		mEventMode = POINT_DEL_MODE;
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


// FIXME: (int x, int y, Egg::egg_plane plane) for vertex / texel / polygon
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
		
		freyja_print("New Bone[%u]", index);
		break;
	case FreyjaModel::TransformMesh:
		mModel->MeshNew();
		break;
	}
}


void FreyjaControl::selectObject(int x, int y, Egg::egg_plane plane)
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
		// mModel->selectTag(plane, xx, yy, zz);
		freyja_print("Selected Bone[%i] ( Still buggy? )", 
					mModel->getCurrentBone());
		break;

	default:
		freyja_print("WARNING: Selection undefined for this mode");
	}
}


void FreyjaControl::moveObject(int x, int y, Egg::egg_plane plane)
{
	static int old_y = 0, old_x = 0;
	const float t = 2.0f, m = 0.5f;
	vec3_t center;
	float xx = x, yy = y, xf, yf, zf;


	/* Mongoose: Convert screen to world coordinate system */
	//getWorldFromScreen(&xx, &yy, &zz);

	/* Mongoose: Compute a relative movement value too here */
	y = -y;
	
	switch (plane)
	{
	case Egg::PLANE_XY:
		xf = ((x < old_x-t) ? -m : ((x > old_x+t) ? m : 0));
		yf = ((y < old_y-t) ? -m : ((y > old_y+t) ? m : 0));
		zf = 0;
		break;
	case Egg::PLANE_XZ:
		xf = ((x < old_x-t) ? -m : ((x > old_x+t) ? m : 0));
		yf = 0;
		zf = ((y < old_y-t) ? -m : ((y > old_y+t) ? m : 0));
	case Egg::PLANE_YZ:
		xf = 0;
		yf = ((x < old_x-t) ? -m : ((x > old_x+t) ? m : 0));
		zf = ((y < old_y-t) ? -m : ((y > old_y+t) ? m : 0));
		break;
	}


	switch (mTransformMode)
	{
	case FreyjaModel::TransformPoint:
		xx = x; 
		yy = -y;
		getScreenToWorldOBSOLETE(&xx, &yy);
		mModel->VertexMove(xx, yy);
		break;

	case FreyjaModel::TransformBone:
		xx = x; 
		yy = -y;
		getScreenToWorldOBSOLETE(&xx, &yy);
		mModel->moveBone(xx, yy);
		break;			
		
	case FreyjaModel::TransformMesh:
		xx = x; 
		yy = -y;
		getScreenToWorldOBSOLETE(&xx, &yy);
		mModel->getCurrentMeshCenter(center);

		/* Exact movement based on center of object */
		switch (plane)
		{
		case Egg::PLANE_XY:
			xf = xx - center[0];
			yf = yy - center[1];
			zf = 0;
			break;
		case Egg::PLANE_XZ:
			xf = xx - center[0];
			yf = 0;
			zf = yy - center[2];
			break;
		case Egg::PLANE_YZ:
			xf = 0;
			yf = xx - center[1];
			zf = yy - center[2];
			break;
		}
		
		mModel->transform(mTransformMode, Egg::TRANSLATE, xf, yf, zf); 
		break;


	default:  
		/* Relative movement based on mouse tracking */
		mModel->transform(mTransformMode, Egg::TRANSLATE, xf, yf, zf);
		break;
	}

	old_x = x;
	old_y = y;
}


void FreyjaControl::rotateObject(int x, int y, Egg::egg_plane plane)
{
	static int old_y = 0, old_x = 0;
	const float t = 1.0f, m = 1.0f;
	float xr, yr, zr;
	float xf, yf, zf;
	int swap;
	Egg::egg_transform rotate;


	/* Mongoose: Compute a relative movement value too here */
	swap = x;
	x = -y;
	y = -swap;
	
	switch (plane)
	{
	case Egg::PLANE_XY:
		xf = ((x < old_x-t) ? -m : ((x > old_x+t) ? m : 0));
		yf = ((y < old_y-t) ? -m : ((y > old_y+t) ? m : 0));
		zf = 0;
		break;
	case Egg::PLANE_XZ:
		xf = ((x < old_x-t) ? -m : ((x > old_x+t) ? m : 0));
		yf = 0;
		zf = ((y < old_y-t) ? -m : ((y > old_y+t) ? m : 0));
	case Egg::PLANE_YZ:
		xf = 0;
		yf = ((x < old_x-t) ? -m : ((x > old_x+t) ? m : 0));
		zf = ((y < old_y-t) ? -m : ((y > old_y+t) ? m : 0));
		break;
	}


	switch (mTransformMode)
	{
	case FreyjaModel::TransformBone:
		mModel->getBoneRotation(&xr, &yr, &zr);
		mModel->setBoneRotation(xr + xf, yr + yf, zr + zf);
		mModel->getBoneRotation(&xr, &yr, &zr);

		if (xr > 180.0f)
			mModel->setBoneRotation(-180.0f, yr, zr);

		if (xr > 180.0f)
			mModel->setBoneRotation(xr, -180.0f, zr);

		if (xr > 180.0f)
			mModel->setBoneRotation(xr, yr, -180.0f);			
		break;


	case FreyjaModel::TransformMesh:
		/* Mongoose: Scaled rotation for better response */
		xf *= 5.0f;
		yf *= 5.0f;
		zf *= 5.0f;

		rotate = Egg::ROTATE_ABOUT_CENTER;
		break;


	default:
		rotate = Egg::ROTATE;
	}

	mModel->transform(mTransformMode, rotate, xf, yf, zf);
	
	old_x = x;
	old_y = y;
}


void FreyjaControl::scaleObject(int x, int y, Egg::egg_plane plane)
{
	static int old_y = 0, old_x = 0;

	if (y > old_y)
	{
		switch (plane)
		{
		case Egg::PLANE_XY:
			mModel->transform(mTransformMode, Egg::SCALE, 1.0, 0.99, 1.0);
			break;
		case Egg::PLANE_XZ:
			mModel->transform(mTransformMode, Egg::SCALE, 1.0, 1.0, 0.99);
			break;
		case Egg::PLANE_YZ:
			mModel->transform(mTransformMode, Egg::SCALE, 1.0, 1.0, 0.99);
			break;
		}
	}
	else if (y < old_y)
	{
		switch (plane)
		{
		case Egg::PLANE_XY:
			mModel->transform(mTransformMode, Egg::SCALE, 1.0, 1.01, 1.0);
			break;
		case Egg::PLANE_XZ:
			mModel->transform(mTransformMode, Egg::SCALE, 1.0, 1.0, 1.01);
			break;
		case Egg::PLANE_YZ:
			mModel->transform(mTransformMode, Egg::SCALE, 1.0, 1.0, 1.01);
		}
	}
	
	if (x < old_x)
	{
		switch (plane)
		{
		case Egg::PLANE_XY:
			mModel->transform(mTransformMode, Egg::SCALE, 0.99, 1.0, 1.0);
			break;
		case Egg::PLANE_XZ:
			mModel->transform(mTransformMode, Egg::SCALE, 0.99, 1.0, 1.0);
			break;
		case Egg::PLANE_YZ:
			mModel->transform(mTransformMode, Egg::SCALE, 1.0, 0.99, 1.0);
			break;
		}
	}
	else if (x > old_x)
	{
		switch (plane)
		{
		case Egg::PLANE_XY:
			mModel->transform(mTransformMode, Egg::SCALE, 1.01, 1.0, 1.0);
			break;
		case Egg::PLANE_XZ:
			mModel->transform(mTransformMode, Egg::SCALE, 1.01, 1.0, 1.0);
			break;
		case Egg::PLANE_YZ:
			mModel->transform(mTransformMode, Egg::SCALE, 1.0, 1.01, 1.0);
		}
	}
	
	old_x = x;
	old_y = y;
}


void FreyjaControl::MotionEdit(int x, int y, Egg::egg_plane plane)
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
		

		switch (mModel->CurrentPlane())
		{
		case Egg::PLANE_XY:
			mModel->adjustSceneTranslation(xyz[0], xyz[1], xyz[2]);
			break;

		case Egg::PLANE_XZ:
			mModel->adjustSceneTranslation(xyz[0], xyz[2], xyz[1]);
			break;

		case Egg::PLANE_YZ:
			mModel->adjustSceneTranslation(xyz[2], xyz[0], xyz[1]);
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
							  Egg::egg_plane plane) 
{ 
	float xx = x, yy = y;
	unsigned int master_tag;
	static float xxx, yyy;
	
	getScreenToWorldOBSOLETE(&xx, &yy);
	
	if (!(btn == MOUSE_BTN_LEFT && state == MOUSE_BTN_STATE_PRESSED))
	{	
		switch(mEventMode)
		{
		case modeMove:
			freyja_print("moved: %f, %f", xx-xxx, yy-yyy);
			break;
		default:
			;
		}

		return;
	}

	switch(mEventMode)
	{
	case modeMove:
		//if (_mouse_state == 0)
		//{
		xxx = xx;  yyy = yy;
		freyja_print("store state: %f, %f", xxx, yyy);
		mModel->VertexSelect(xx, yy);
		_mouse_state = 1;
		//}
		//else
		//{
		//	_mouse_state = 0;
		//}
		break;

	case modeSelect:
		selectObject(x, y, plane);
		break;


	case VERTEX_COMBINE:
		if (_mouse_state == 0)
		{
			mModel->VertexCombine(xx, yy);
			 _mouse_state = 1;
		}
		else
		{
			_mouse_state = 0;
		}
		break;
	case VERTEX_BBOX_SELECT_MODE:
		if (_mouse_state == 0)
		{
			mModel->BBoxSelect(xx, yy);
			_mouse_state = 1;
		}
		else
			_mouse_state = 0;
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
		if (_mouse_state == 0)
			_mouse_state = 1;
		else
			_mouse_state = 0;
		break;
	case POINT_ADD_MODE: 
		mModel->VertexNew(xx, yy);
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
	float f;
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

	if (mResource->Lookup("GRID_ON", &i) && i)
	{
		mRender->Flags(FreyjaRender::RENDER_EDIT_GRID, 
							!(mRender->Flags() & FreyjaRender::RENDER_EDIT_GRID));
	}

	if (mResource->Lookup("ROTATE_AMOUNT", &f))
	{
		mRender->RotateAmount(f);
	}

	if (mResource->Lookup("FLUSH_RESOURCE", &i) && i)
	{
		mResource->Flush();
	}
}




