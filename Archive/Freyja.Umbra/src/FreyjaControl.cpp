/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : FreyjaControl
 * License : No use w/o permission (c) 2000-2004 Mongoose
 * Comments: This is the controler class for the client
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2000.09.10:
 * Mongoose - Created
 ==========================================================================*/

#include <assert.h>
#include <stdlib.h> 
#include <stdio.h> 
#include <math.h> 
#include <stdarg.h> 
#include <string.h>

#include <mstl/String.h>

#include "FreyjaControl.h"
#include "freyja_events.h"

extern unsigned int gJointRenderType;
extern unsigned int gBoneRenderType;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

FreyjaControl::FreyjaControl(Resource *rc)
{
	char *pluginDir = freyja_rc_map("plugins/");


	/* Hook up the rc, backend, renderer, and user interface */
	mResource = rc;
	mScene = new OpenGLFreyjaScene();
	mRender = new FreyjaRender(mScene);
	mModel = new FreyjaPlugin(mScene, pluginDir);

	delete [] pluginDir;

	mModel->setPrinter(new FreyjaAppPrinter());

	if (strncmp(FREYJA_PLUGIN_VERSION, "Freyja 0.9.1", 12))
	{
		freyja_print("FreyjaControl::FreyjaControl> Assertion failure");
		freyja_print("*** You must upgrade libfreyja to v0.9.1");
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

	/* Set some basic defaults */
	mEditorMode = MODEL_EDIT_MODE;
	mEventMode = modeNone;
	mTransformMode = fTransformScene;
	mLastEvent = EVENT_MAIN;
	mLastCommand = eSelect;
	mFullScreen = false;
	mFileDialogMode = FREYJA_MODE_LOAD_MODEL;
	setZoom(1.0f);
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

	/* Free the backend and user interface */
	if (mScene)
		delete mScene;

	if (mRender)
		delete mRender;

	if (mModel)
		delete mModel;

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

void FreyjaControl::initOpenGL()
{
	unsigned int width = 740;
	unsigned int height = 560;


	/* Mongoose 2002.02.23, Tell renderer to start up */
	mRender->Init(width, height, true);
	mRender->Reshape(width, height);
	mMaterial = MaterialManager::Instance();
}


void FreyjaControl::addRecentFilename(const char *filename)
{
	char *filenameCopy = String::strdup(filename);


	if (filenameCopy)
	{
		mRecentFiles.pushBack(filenameCopy);
	}
}


void FreyjaControl::display()
{
	mRender->Display();
}


bool FreyjaControl::eventMain(int event)
{
	unsigned int i;


	switch (event)
	{
	case eInfo:
		/* FIXME: Should be per model... */
		freyjaIterator(FREYJA_MODEL, freyjaGetCurrent(FREYJA_MODEL));

		//freyja_print("%d bones, %d meshes, %d polygons, %d vertices",
		//			mScene->getBoneCount(), 
		//			mScene->getMeshCount(), 
		//			mScene->getPolygonCount(), 
		//			mScene->getVertexCount());
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
				mModel->clear();
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

				mModel->clear();
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
		//FIXME
		freyja_print("eCamera event unhandled: %s:%i ", __FILE__, __LINE__);
		break;


	case eDebug:
		//FIXME
		freyja_print("eDebug event unhandled: %s:%i ", __FILE__, __LINE__);
		//mModel->Debug(!mModel->Debug());
		//freyja_print("Egg debug [%s]", mModel->Debug() ? "ON" : "OFF");
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
		//FIXME eggGenerateVertexNormals();
		freyja_print("eGenerateNormals disabled in tracer build");
		break;


	case eGenerateCylinder:
		//FIXME eggGenerateCylinder(4, 16, 8.0, 4.0);
		//event_refresh();
		freyja_print("eGenerateCylinder disabled in tracer build");
		break;


	case eGenerateSphere:
		//FIXME eggGenerateSphere(16, 16, 8.0);
		//event_refresh();
		freyja_print("eGenerateSphere disabled in tracer build");
		break;


	case eGenerateCone:
		//FIXME eggGenerateCone(8.0);
		//event_refresh();
		freyja_print("eGenerateCone disabled in tracer build");
		break;


	case eGenerateCube:
		if (freyjaGetCount(FREYJA_MODEL) == 0)
		{
			freyjaBegin(FREYJA_MODEL);
			freyjaEnd();
		}

		{
			vec3_t pos = {0.0f, 0.0f, 0.0f};
			freyjaGenerateQuadCubeMesh(pos, 8.0f);
		}
		freyja_event_gl_refresh();
		break;


	case eGenerateTriStrip:
		//FIXME eggGenerateTriangleStrip(10, 8.0);
		//event_refresh();
		freyja_print("eGenerateTriStrip disabled in tracer build");
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
		freyja_print("eRenderBbox event unhandled: %s:%i ", __FILE__, __LINE__);
		break;


	case eSetMeshTexture:
		freyja_print("Switching all of mesh %i ploygons to texture %i",
					mModel->getCurrentMesh(), 
					mRender->getCurrentTextureIndex());

		mModel->setMeshPolygonsMaterial(mModel->getCurrentMesh(), 
										mRender->getCurrentTextureIndex());
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
		mTransformMode = fTransformVertexFrame;
		freyja_print("eTransformVertexFrame not enabled in tracer build");
		break;
	case eTransformScene:
		mTransformMode = fTransformScene;
		freyja_print("eTransformScene not enabled in tracer build");
		break;
	case eTransformPoint:
		mTransformMode = fTransformVertex;
		freyja_print("eTransformVertex not enabled in tracer build");
		break;
	case eTransformMesh:
		mTransformMode = fTransformMesh;
		freyja_print("Mesh transform mode");
		break;
	case eTransformMeshPivot:
		mTransformMode = fTransformMesh;
		mEventMode = MESH_MOVE_CENTER;
		//freyja_print("Reposition mesh center point");
		freyja_print("eTransformMeshPivot not enabled in tracer build");
		break;
	case eTransformBone:
		mTransformMode = fTransformBone;
		freyja_print("eTransformBone not enabled in tracer build");
		break;


	case eDelete:
		deleteSelectedObject();
		break;

	case ePaste:
		pasteBufferedObject();
		break;

	case eCut:
		copySelectedObject();
		deleteSelectedObject();
		break;

	case eCopy:
		copySelectedObject();
		break;

	case eAddObject:
		addObject();
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

	default:
		//freyja_print("!Unhandled Event(%d)", event);
		return false;
	}

	return true;
}


bool FreyjaControl::event(int event, unsigned int value)
{
	switch (event)
	{
	case 503:
		if ((int)value != mRender->getCurrentTextureIndex())
		{
			mRender->setCurrentTextureIndex(value);
			//value = mModel->getCurrentTextureIndex();
			//spinbutton_value_set(event, value);
			freyja_event_gl_refresh();
			freyja_print("Selecting textureIndex[%i] ...", value);
		}
		break;


	case eModelIterator:
		if (!freyja_event_set_range(event, value, 
									0, freyjaGetCount(FREYJA_MODEL)))
		{
			freyjaIterator(FREYJA_MODEL, value);
			freyja_print("Selecting model[%i] ...", value);
		}
		break;


	case eMeshIterator:
		if (!freyja_event_set_range(event, value, 
									0, freyjaGetCount(FREYJA_MESH)))
		{
			freyjaIterator(FREYJA_MESH, value);
			freyja_event_gl_refresh();
			freyja_print("Selecting mesh[%i] ...", value);
		}
		break;


	case eSetMaterialTexture:
		gMaterialManager->setTexture(0, value);
		freyja_print("Set material texture to %d", value);
		break;


	case eAnimationSlider:
		// FIXME
		//if (adj->value != gFreyjaModel->getCurrentAnimationFrame())
		//{
		//	gFreyjaModel->setCurrentAnimationFrame((unsigned int)adj->value);
		//	adj->value = gFreyjaModel->getCurrentAnimationFrame();
		//	freyja_event_gl_refresh();
		//}
		freyja_print("eAnimationSlider:%u not implemented\n", value);
		break;

	default:
		freyja_print("!Unhandled event %d, value = %u", event, value);
		return false;
	}

	return true;	
}


bool FreyjaControl::event(int command, vec_t value)
{
	vec4_t color;
	vec3_t xyz;


	switch (command)
	{
	case eZoom:
		setZoom(value);//freyja_event_get_float(eZoom));
		freyja_print("Zoom %f", mRender->getZoom());
		freyja_event_gl_refresh();
		break;



	case 510:
	case 511:
	case 512:
		//FIXME
		freyjaGetBoneTranslation3fv(freyjaGetCurrent(FREYJA_BONE), xyz);
		xyz[command-510] = value;
		//freyjaSetBonePos3fv(xyz);
		//freyja_event_gl_refresh();
		break;

	case 520:
	case 521:
	case 522:
		// FIXME
		//freyjaGetBoneRotation3fv(freyjaGetCurrent(FREYJA_BONE), xyz);
		//xyz[command-520] = value;
		//freyjaBoneRotate3fv(xyz);
		//freyja_event_gl_refresh();
		break;

	case 700:
	case 701:
	case 702:
	case 703:
		gMaterialManager->getColor(MaterialManager::eAmbient, color);
		color[command-700] = value;
		gMaterialManager->setColor(MaterialManager::eAmbient, color);
		freyja_event_gl_refresh();
		break;
	case 704:
	case 705:
	case 706:
	case 707:
		gMaterialManager->getColor(MaterialManager::eDiffuse, color);
		color[command-704] = value;
		gMaterialManager->setColor(MaterialManager::eDiffuse, color);
		freyja_event_gl_refresh();
		break;
	case 708:
	case 709:
	case 710:
	case 711:
		gMaterialManager->getColor(MaterialManager::eSpecular, color);
		color[command-708] = value;
		gMaterialManager->setColor(MaterialManager::eSpecular, color);
		freyja_event_gl_refresh();
		break;

	case 712:
	case 713:
	case 714:
	case 715:
		gMaterialManager->getColor(MaterialManager::eEmissive, color);
		color[command-712] = value;
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
		// FIXME
		//freyjaGetLightPosition3fv(xyz);
		//xyz[command-800] = value;
		//freyjaSetLightPosition3fv(xyz);
		//freyja_event_gl_refresh();
		break;


		//case eMove:
	case eMove_X:
	case eMove_Y:
	case eMove_Z:
		/* Noop here, value used in another event */
		break;


		//case eRotate:
	case eRotate_X:
	case eRotate_Y:
	case eRotate_Z:
		/* Noop here, value used in another event */
		break;


		//case eScale:
	case eScale_X:
	case eScale_Y:
	case eScale_Z:
		/* Noop here, value used in another event */
		break;

	case CMD_BONE_CONNECT:
		mTransformMode = fTransformBone;
		freyja_print("Select bone to connect to current");
		mEventMode = BONE_CONNECT_MODE;
		break;


	case CMD_BONE_DISCONNECT:
		mTransformMode = fTransformBone;
		freyja_print("Select bone to break from current");
		mEventMode = BONE_DISCONNECT_MODE;
		break;


	case CMD_BONE_NEXT:
		mTransformMode = fTransformBone;
		freyjaIterator(FREYJA_BONE, freyjaGetCurrent(FREYJA_BONE) + 1);
		//mModel->setCurrentBone(mModel->getCurrentBone() + 1);
		break;


	case CMD_BONE_PREV:
		mTransformMode = fTransformBone;
		if (freyjaGetCurrent(FREYJA_BONE))
			freyjaIterator(FREYJA_BONE, freyjaGetCurrent(FREYJA_BONE) - 1);
		break;


	case CMD_BONE_ADD_MESH:
		mTransformMode = fTransformBone;
		// OBSOLETE
		//mModel->addMeshToBone(mModel->getCurrentBone(), mModel->getCurrentMesh());
		//freyjaIterator(FREYJA_BONE, freyjaGetCurrent(FREYJA_BONE));
		//freyjaBoneAddMesh1u(freyjaGetCurrent(FREYJA_MESH));
		
		//freyja_print("New Bone[%i] now contains mesh %i",
		//			 freyjaGetCurrent(FREYJA_BONE), 
		//			 freyjaGetCurrent(FREYJA_MESH));
		break;


	case CMD_BONE_DELETE_MESH:
		mTransformMode = fTransformBone;
		//FIXME
		//mModel->removeMeshFromBone(mModel->getCurrentBone(), mModel->getCurrentMesh());
		//freyja_print("New Bone[%i] no longer contains mesh %i",
		//				mModel->getCurrentBone(), mModel->getCurrentMesh());
		break;

	default:
		freyja_print("event> Unhandled event : %i, %f", command, value);
		return false;
	}

	return true;
}


bool FreyjaControl::eventMisc(int command)
{
	//vec4_t color;
	//vec3_t xyz;
	vec_t x, y, z;
	unsigned int i;
	unsigned int flags;


	// HACK: temp fix for main routing
	if (eventMain(command))
		return true;

	switch (command)
	{
	case 504:
		if (1)//!spinbutton_uint_set_range(spin, value, 0, freyjaGetCount(FREYJA_BONE)))
		{
			i = (int)freyja_event_get_float(command);
			freyjaIterator(FREYJA_BONE, i);

			//mModel->setCurrentBone(i);
			//value = gFreyjaModel->getCurrentBone();
			//spinbutton_value_set(event, value);

			/* Mongoose 2002.08.31, Update spin buttons dependent 
			 * on this one */
			// FIXME
			//mModel->getBoneRotation(&x, &y, &z);
			freyja_event_set_float(520, x);
			freyja_event_set_float(521, y);
			freyja_event_set_float(522, z);
			
			//mModel->getBoneTranslation(&x, &y, &z);
			freyja_event_set_float(510, x);
			freyja_event_set_float(511, y);
			freyja_event_set_float(512, z);
			freyja_event_gl_refresh();
		}
		break;



	/* "Well behaved" events */
	case FREYJA_MODE_RENDER_BONETAG:
		OpenGLFreyjaScene::mRenderMode ^= OpenGLFreyjaScene::fBones;
		freyja_print("Bone Rendering [%s]", 
					 (OpenGLFreyjaScene::mRenderMode & OpenGLFreyjaScene::fBones) ? 
					 "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_RENDER_POINTS:
		OpenGLFreyjaScene::mRenderMode ^= OpenGLFreyjaScene::fVertices;
		freyja_print("Point Rendering [%s]", 
					 (OpenGLFreyjaScene::mRenderMode & OpenGLFreyjaScene::fVertices) ? 
					 "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_RENDER_WIREFRAME:
		OpenGLFreyjaScene::mRenderMode ^= OpenGLFreyjaScene::fWireframe;
		freyja_print("Wireframe Rendering [%s]", 
					 (OpenGLFreyjaScene::mRenderMode & OpenGLFreyjaScene::fWireframe) ? 
					 "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_RENDER_FACE:
		OpenGLFreyjaScene::mRenderMode ^= OpenGLFreyjaScene::fFace;
		freyja_print("Face Rendering [%s]", 
					 (OpenGLFreyjaScene::mRenderMode & OpenGLFreyjaScene::fFace) ? 
					 "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_RENDER_NORMALS:
		OpenGLFreyjaScene::mRenderMode ^= OpenGLFreyjaScene::fNormals;
		freyja_print("Normal Rendering [%s]", 
					 (OpenGLFreyjaScene::mRenderMode & OpenGLFreyjaScene::fNormals) ? 
					 "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_RENDER_LIGHTING:
		OpenGLFreyjaScene::mRenderMode ^= OpenGLFreyjaScene::fLighting;

		freyja_print("GL Lighting is [%s]", 
					 (OpenGLFreyjaScene::mRenderMode & OpenGLFreyjaScene::fLighting) ? 
					 "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_RENDER_TEXTURE:
		OpenGLFreyjaScene::mRenderMode ^= OpenGLFreyjaScene::fTexture;

		freyja_print("Texture rendering is [%s]", 
					 (OpenGLFreyjaScene::mRenderMode & OpenGLFreyjaScene::fTexture) ? 
					 "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_RENDER_MATERIAL:
		OpenGLFreyjaScene::mRenderMode ^= OpenGLFreyjaScene::fMaterial;

		freyja_print("Material rendering is [%s]", 
					 (OpenGLFreyjaScene::mRenderMode & OpenGLFreyjaScene::fMaterial) ? 
					 "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_RENDER_GRID:
		OpenGLFreyjaScene::mRenderMode ^= OpenGLFreyjaScene::fGrid;
		freyja_print("Edit Grid rendering [%s]",
					 (OpenGLFreyjaScene::mRenderMode & OpenGLFreyjaScene::fGrid) ? 
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
		mRender->setCurrentPlane(FreyjaRender::PLANE_XY);
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_PLANE_XZ:
		freyja_print("Plane XZ");      
		mRender->setCurrentPlane(FreyjaRender::PLANE_XZ);
		freyja_event_gl_refresh();
		break;

	case FREYJA_MODE_PLANE_YZ:
		freyja_print("Plane YZ");      
		mRender->setCurrentPlane(FreyjaRender::PLANE_YZ);
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



	case eAnimationNext:
		freyja_print("eAnimationNext action not implemented");
		//freyja_event_gl_refresh();
		break;
	case eAnimationPrev:
		freyja_print("eAnimationPrev action not implemented");
		//freyja_event_gl_refresh();
		break;
	case eAnimationStop:
		freyja_print("eAnimationStop action not implemented");
		//freyja_event_gl_refresh();
		break;
	case eAnimationPlay:
		freyja_print("eAnimationPlay action not implemented");
		//freyja_event_gl_refresh();
		break;


	case eMeshNew:
		mTransformMode = fTransformMesh;
		freyja_print("eMeshNew action not implemented %s:%i",
					__FILE__, __LINE__);
		break;
	case eMeshDelete:
		mTransformMode = fTransformMesh;
		freyja_print("eMeshDelete action not implemented %s:%i",
					__FILE__, __LINE__);
		break;
	case eMeshMove:
		mTransformMode = fTransformMesh;
		freyja_print("eMeshMove action not implemented %s:%i",
					__FILE__, __LINE__);
		break;
	case eMeshSelect:
		mTransformMode = fTransformMesh;
		freyja_print("eMeshSelect action not implemented %s:%i",
					__FILE__, __LINE__);
		break;
	case eMeshRotate:
		mTransformMode = fTransformMesh;
		freyja_print("eMeshRotate action not implemented %s:%i",
					__FILE__, __LINE__);
		break;
	case eMeshScale:
		mTransformMode = fTransformMesh;
		freyja_print("eMeshScale action not implemented %s:%i",
					__FILE__, __LINE__);
		break;
	case eMeshMirror:
		mTransformMode = fTransformMesh;
		freyja_print("eMeshMirror action not implemented %s:%i",
					__FILE__, __LINE__);
		break;
	case eMeshCenter:
		mTransformMode = fTransformMesh;
		freyja_print("eMeshCenter action not implemented %s:%i",
					__FILE__, __LINE__);
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

	case eMoveBone_X:
	case eMoveBone_Y:
	case eMoveBone_Z:
		break;

	case CMD_MISC_BBOX_SELECT:
		freyja_print("CMD_MISC_BBOX_SELECT event unhandled: %s:%i ", __FILE__, __LINE__);
		if (mEventMode == VERTEX_BBOX_SELECT_MODE)
		{
			mEventMode = modeNone;
			//mRender->Flags(FreyjaRender::RENDER_BBOX_SEL, 0);
			//FIXME mModel->BBoxListBuild();
			//freyja_print("Vertex box select: Vertex list built");
		}
		else
		{
			//mEventMode = VERTEX_BBOX_SELECT_MODE;
			//mRender->Flags(FreyjaRender::RENDER_BBOX_SEL, 1);
			//freyja_print("Vertex box select: Press agian to end selection");
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
		//FIXME mModel->generateUVMap();
		break;
		
		
	case CMD_MISC_SIDES_M:
		//FIXME mModel->setCurrentPolygonEdgeCount(mModel->getCurrentPolygonEdgeCount()-1);
		//freyja_print("Making %i sided polygons", 
		//			mModel->getCurrentPolygonEdgeCount());
		break;
	case CMD_MISC_SIDES_P:
		//FIXME mModel->setCurrentPolygonEdgeCount(mModel->getCurrentPolygonEdgeCount()+1);
		//freyja_print("Making %i sided polygons", 
		//			mModel->getCurrentPolygonEdgeCount());
		break;
		
	case eZoomIn:
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
		
	case eZoomOut:
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


	case eMaterialTex:
		flags = gMaterialManager->getFlags();
		
		if (flags & Material::fEnable_Texture)
			gMaterialManager->clearFlag(Material::fEnable_Texture);
		else
			gMaterialManager->setFlag(Material::fEnable_Texture);
		
		freyja_print("OpenGL texturing is [%s]", 
						(flags & Material::fEnable_Texture) ? "ON" : "OFF");
		freyja_event_gl_refresh();

	case eMaterialMultiTex:
		flags = gMaterialManager->getFlags();

		if (flags & Material::fEnable_DetailTexture)
			gMaterialManager->clearFlag(Material::fEnable_DetailTexture);
		else
			gMaterialManager->setFlag(Material::fEnable_DetailTexture);

		freyja_print("OpenGL detail texturing is [%s]", 
						(flags & Material::fEnable_DetailTexture) ? "ON" : "OFF");
		freyja_event_gl_refresh();

	case eOpenGLNormalize:
		flags = gMaterialManager->getFlags();

		if (flags & Material::fEnable_Normalize)
			gMaterialManager->clearFlag(Material::fEnable_Normalize);
		else
			gMaterialManager->setFlag(Material::fEnable_Normalize);

		freyja_print("OpenGL normalization of normals is [%s]", 
						(flags & Material::fEnable_Normalize) ? "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

	case eOpenGLBlend:
		flags = gMaterialManager->getFlags();
		
		if (flags & Material::fEnable_Blending)
			gMaterialManager->clearFlag(Material::fEnable_Blending);
		else
			gMaterialManager->setFlag(Material::fEnable_Blending);
		
		freyja_print("OpenGL blending [%s]", 
						(flags & Material::fEnable_Blending) ? "ON" : "OFF");
		freyja_event_gl_refresh();
		break;

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

	default:
		freyja_print("!Unhandled eventMisc(%d)", command);
		return false;
	}   

	return true;
}


bool FreyjaControl::handleEvent(int mode, int cmd)
{
	/*
	if (mode == EVENT_REDO_LAST)
	{
		mode = mLastEvent;
		cmd = mLastCommand;
	}
	*/

	switch (mode)
	{
	default:
		if (!eventMisc(cmd))
		{
			freyja_print("!UnHandled mode %d and command %d", mode, cmd);
			return false;
		}
	}

	mLastEvent = mode;
	mLastCommand = cmd;

	return true;
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
		failed = mModel->importModel((char *)filename);

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
		//FIXME failed = mModel->saveModel(filename);
		type = 2;
		type2 = 0;
		break;


	case FREYJA_MODE_SAVE_ANIMATION:
		failed = 0;
		//FIXME mModel->saveAnimation(filename);
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
		//FIXME mModel->nameBone(mModel->getCurrentBone(), text);
		break;

	default:
		freyja_print("Unhandled event(%i) '%s'", event, text);
	}
}


bool FreyjaControl::motionEvent(int x, int y)
{
	static int old_y = 0, old_x = 0;
	static int e_old_y = 0, e_old_x = 0;
	const float treshold = 2.0f;
	float xyz[3];
	float xx = x, yy = y;
	FreyjaRender::plane_t plane = (FreyjaRender::plane_t)mRender->getCurrentPlane();

	switch (mEditorMode)
	{
	case ANIMATION_EDIT_MODE:
		switch (mMouseButton)
		{
		case MOUSE_BTN_RIGHT:
			/* FIXME: I need to use an arcball here for fuck's sake
			 *         this code is older than some people I know */
			if (x > old_x)
				mRender->Rotate(1, -mRender->RotateAmount());
			else if (x < old_x)
				mRender->Rotate(1, mRender->RotateAmount());
			
			if (y > old_y)
				mRender->Rotate(0, -mRender->RotateAmount());
			else if (y < old_y)
				mRender->Rotate(0, mRender->RotateAmount());

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
			
			//mModel->TexelMove(s, t);
			//FIXME: freyjaSetTexCoord2f(freyjaGetCurrent(FREYJA_TEXCOORD), s, t);
			return true;
		}
		break;


	case MODEL_EDIT_MODE:
		old_x = e_old_x;
		old_y = e_old_y;

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
		
			switch (mRender->getCurrentPlane())
			{
			case FreyjaRender::PLANE_XY:
				//FIXME 	mModel->adjustSceneTranslation(xyz[0], xyz[1], xyz[2]);
				break;
				
			case FreyjaRender::PLANE_XZ:
				//FIXME 	mModel->adjustSceneTranslation(xyz[0], xyz[2], xyz[1]);
				break;
				
			case FreyjaRender::PLANE_YZ:
				//FIXME 	mModel->adjustSceneTranslation(xyz[2], xyz[0], xyz[1]);
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
				//FIXME mModel->BBoxMove(xx, yy);
				break;
			
			case TAG_MOVE_CENTER:
				//FIXME mModel->moveBoneCenter(xx, yy);
				break;

			case MESH_MOVE_CENTER: 
				//FIXME mModel->MeshMoveCenter(xx, yy);
				break;

			default:
				break;
			}

			old_x = x;
			old_y = y;
			break;
		}

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
			eventMisc(eZoomIn);
			freyja_print("Zoom in");
		}
		
		if (btn == MOUSE_BTN_DOWN && state == MOUSE_BTN_STATE_PRESSED)
		{
			eventMisc(eZoomOut);
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
				//FIXME mModel->TexelCombine(s, t);
			}
			else
			{
				//FIXME mModel->TexelSelect(s, t);
			}
			
			_tex_state = true;
			return true;
		}
		break;
	case MODEL_EDIT_MODE:
		// Mongoose 2002.01.12, Allow temp mode override
		if (mod & KEY_LCTRL)
		{
			handleEvent(EVENT_MAIN, eSelect);
		}
		else if (mod & KEY_LSHIFT)
		{
			handleEvent(EVENT_MAIN, eRotate);
		}

		//FIXME MouseEdit(btn, state, mod, x, y, mModel->CurrentPlane());
		break;
	default:
		;
	}

	// Mongoose 2002.01.12, Reset mode
	mEventMode = mode;

	return true;
}


void FreyjaControl::resizeDisplay(unsigned int width, unsigned int height)
{
	mRender->Reshape(width, height);
}


void FreyjaControl::setZoom(float zoom)
{
	assert(zoom > 0.0f);
	//if (zoom > 0.0f)
	//	return;

	freyja_event_notify_observer1f(eZoom, zoom);

	mRender->setZoom(zoom);
	freyja_print("Zoom set to %f", mRender->getZoom());
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

	switch (mRender->getCurrentPlane())
	{
	case FreyjaRender::PLANE_XY:
		/* Nothing to do for XY */
		break;
		
	case FreyjaRender::PLANE_XZ:
		*y = z;
		break;
		
	case FreyjaRender::PLANE_YZ:
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
	//FIXME mModel->getSceneTranslation(scroll);

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
	
	switch (mRender->getCurrentPlane())
	{
	case FreyjaRender::PLANE_XY:
		*x -= scroll[0] * invz;
		*y -= scroll[1] * invz;
		*z = 0.0f;
		break;

	case FreyjaRender::PLANE_XZ:
		*x -= scroll[0] * invz;
		*z = *y - scroll[2] * invz;
		*y = 0.0f;
		break;

	case FreyjaRender::PLANE_YZ:
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

void FreyjaControl::copySelectedObject()
{
	switch (mTransformMode)
	{
	case fTransformVertex:
		freyja_print("NOT IMPLEMENTED: You can't copy vertices");
		break;

	case fTransformBone:
		freyja_print("NOT IMPLEMENTED: You can't copy bones"); 
		break;

	case fTransformMesh:
		//FIXME freyja_print("Copying currently selected mesh to buffer."); 
		//FIXME mModel->copyMesh();
		break;
	default:
		;
	}
}


void FreyjaControl::pasteBufferedObject()
{
	switch (mTransformMode)
	{
	case fTransformVertex:
		freyja_print("NOT IMPLEMENTED: You can't paste vertices");
		break;

	case fTransformBone:
		freyja_print("NOT IMPLEMENTED: You can't paste bones"); 
		break;

	case fTransformMesh:
		//FIXME freyja_print("Pasting currently buffered mesh into model."); 
		//FIXME mModel->pasteMesh();
		break;
	default:
		;
	}
}

// FIXME: (int x, int y, plane_t plane) for vertex / texel / polygon
void FreyjaControl::deleteSelectedObject()
{
	switch (mTransformMode)
	{
	case fTransformVertex:
		mEventMode = POINT_DEL_MODE;
		break;

	case fTransformBone:
		//FIXME freyja_print("NOT IMPLEMENTED: Deleting Bone Tag %d", 
		//			mModel->getCurrentBone());
		break;

	case fTransformMesh:
		//FIXME mModel->MeshDel();
		break;
	default:
		;
	}
}


// FIXME: (int x, int y, plane_t plane) for vertex / texel / polygon
void FreyjaControl::addObject()
{
	unsigned int index;


	switch (mTransformMode)
	{
	case fTransformVertex:
		mEventMode = POINT_ADD_MODE;
		break;

	case fTransformBone:
		//FIXME index = mModel->newBone(0.0, 0.0, 0.0, 0x0);

		if (index > 0)
		{
		//FIXME 	mModel->connectBone(mModel->getCurrentBone(), index);
		}
		
		freyja_print("New Bone[%u]", index);
		break;
	case fTransformMesh:
		//FIXME mModel->MeshNew();
		break;
	default:
		;
	}
}


void FreyjaControl::selectObject(int x, int y, FreyjaRender::plane_t plane)
{
	float xx = x, yy = y, zz;


	/* Mongoose: Convert screen to world coordinate system */
	getWorldFromScreen(&xx, &yy, &zz);

	switch (mTransformMode)
	{
	case fTransformVertex:
		xx = x; 
		yy = y;
		getScreenToWorldOBSOLETE(&xx, &yy);
		//FIXME mModel->VertexSelect(xx, yy);
		// mModel->selectVertex(plane, xx, yy, zz);
		break;

	case fTransformMesh:
		xx = x; 
		yy = y;
		getScreenToWorldOBSOLETE(&xx, &yy);
		//FIXME mModel->MeshSelect(xx, yy);
		// mModel->selectMesh(plane, xx, yy, zz);
		//FIXME freyja_print("Selected Mesh[%i]", 
		//FIXME 			mModel->getCurrentMesh());
		break;

	case fTransformBone:
		xx = x; 
		yy = y;
		getScreenToWorldOBSOLETE(&xx, &yy);
		//FIXME mModel->selectBone(xx, yy);
		// mModel->selectTag(plane, xx, yy, zz);
		//FIXME freyja_print("Selected Bone[%i] ( Still buggy? )", 
		//FIXME 			mModel->getCurrentBone());
		break;

	default:
		freyja_print("WARNING: Selection undefined for this mode");
	}
}


void FreyjaControl::moveObject(int x, int y, FreyjaRender::plane_t plane)
{
	static int old_y = 0, old_x = 0;
	const float t = 2.0f, m = 0.5f;
	//vec3_t center = {0, 0, 0};
	float xx = x, yy = y, xf, yf, zf;


	/* Mongoose: Convert screen to world coordinate system */
	//getWorldFromScreen(&xx, &yy, &zz);

	/* Mongoose: Compute a relative movement value too here */
	y = -y;
	
	switch (plane)
	{
	case FreyjaRender::PLANE_XY:
		xf = ((x < old_x-t) ? -m : ((x > old_x+t) ? m : 0));
		yf = ((y < old_y-t) ? -m : ((y > old_y+t) ? m : 0));
		zf = 0;
		break;
	case FreyjaRender::PLANE_XZ:
		xf = ((x < old_x-t) ? -m : ((x > old_x+t) ? m : 0));
		yf = 0;
		zf = ((y < old_y-t) ? -m : ((y > old_y+t) ? m : 0));
	case FreyjaRender::PLANE_YZ:
		xf = 0;
		yf = ((x < old_x-t) ? -m : ((x > old_x+t) ? m : 0));
		zf = ((y < old_y-t) ? -m : ((y > old_y+t) ? m : 0));
		break;
	}


	switch (mTransformMode)
	{
	case fTransformVertex:
		xx = x; 
		yy = -y;
		getScreenToWorldOBSOLETE(&xx, &yy);
		//FIXME mModel->VertexMove(xx, yy);
		break;

	case fTransformBone:
		xx = x; 
		yy = -y;
		getScreenToWorldOBSOLETE(&xx, &yy);
		//FIXME mModel->moveBone(xx, yy);
		break;			
		
	case fTransformMesh:
#ifdef FIXME
		xx = x; 
		yy = -y;
		getScreenToWorldOBSOLETE(&xx, &yy);
		//FIXME mModel->getCurrentMeshCenter(center);

		/* Exact movement based on center of object */
		switch (plane)
		{
		case FreyjaRender::PLANE_XY:
			xf = xx - center[0];
			yf = yy - center[1];
			zf = 0;
			break;
		case FreyjaRender::PLANE_XZ:
			xf = xx - center[0];
			yf = 0;
			zf = yy - center[2];
			break;
		case FreyjaRender::PLANE_YZ:
			xf = 0;
			yf = xx - center[1];
			zf = yy - center[2];
			break;
		}
		
		mModel->transform(mTransformMode, fTranslate, xf, yf, zf);
		break;
#endif

	default:  
		/* Relative movement based on mouse tracking */
		mModel->transform(mTransformMode, fTranslate, xf, yf, zf);
		break;
	}

	old_x = x;
	old_y = y;
}


void FreyjaControl::rotateObject(int x, int y, FreyjaRender::plane_t plane)
{
	static int old_y = 0, old_x = 0;
	const float t = 1.0f, m = 1.0f;
	//FIXME float xr, yr, zr;
	float xf, yf, zf;
	int swap;
	freyja_transform_action_t rotate;
	vec3_t center;


	/* Mongoose: Compute a relative movement value too here */
	swap = x;
	x = -y;
	y = -swap;
	
	switch (plane)
	{
	case FreyjaRender::PLANE_XY:
		xf = ((x < old_x-t) ? -m : ((x > old_x+t) ? m : 0));
		yf = ((y < old_y-t) ? -m : ((y > old_y+t) ? m : 0));
		zf = 0;
		break;
	case FreyjaRender::PLANE_XZ:
		xf = ((x < old_x-t) ? -m : ((x > old_x+t) ? m : 0));
		yf = 0;
		zf = ((y < old_y-t) ? -m : ((y > old_y+t) ? m : 0));
	case FreyjaRender::PLANE_YZ:
		xf = 0;
		yf = ((x < old_x-t) ? -m : ((x > old_x+t) ? m : 0));
		zf = ((y < old_y-t) ? -m : ((y > old_y+t) ? m : 0));
		break;
	}


	switch (mTransformMode)
	{
	case fTransformBone:
		//FIXME mModel->getBoneRotation(&xr, &yr, &zr);
		//FIXME mModel->setBoneRotation(xr + xf, yr + yf, zr + zf);
		//FIXME mModel->getBoneRotation(&xr, &yr, &zr);

		//FIXME if (xr > 180.0f)
		//FIXME 	mModel->setBoneRotation(-180.0f, yr, zr);

		//FIXME if (xr > 180.0f)
		//FIXME 	mModel->setBoneRotation(xr, -180.0f, zr);

		//FIXME if (xr > 180.0f)
		//FIXME 	mModel->setBoneRotation(xr, yr, -180.0f);			
		break;


	case fTransformMesh:
		// FIXME: Get center
		mModel->setTransformPoint(center);
		rotate = fRotate; // FIXME: fRotateAboutPoint;
		break;


	default:
		rotate = fRotate;
	}

	mModel->transform(mTransformMode, rotate, xf, yf, zf);
	
	old_x = x;
	old_y = y;
}


void FreyjaControl::scaleObject(int x, int y, FreyjaRender::plane_t plane)
{
	static int old_y = 0, old_x = 0;

	if (y > old_y)
	{
		switch (plane)
		{
		case FreyjaRender::PLANE_XY:
			mModel->transform(mTransformMode, fScale, 1.0, 0.99, 1.0);
			break;
		case FreyjaRender::PLANE_XZ:
			mModel->transform(mTransformMode, fScale, 1.0, 1.0, 0.99);
			break;
		case FreyjaRender::PLANE_YZ:
			mModel->transform(mTransformMode, fScale, 1.0, 1.0, 0.99);
			break;
		}
	}
	else if (y < old_y)
	{
		switch (plane)
		{
		case FreyjaRender::PLANE_XY:
			mModel->transform(mTransformMode, fScale, 1.0, 1.01, 1.0);
			break;
		case FreyjaRender::PLANE_XZ:
			mModel->transform(mTransformMode, fScale, 1.0, 1.0, 1.01);
			break;
		case FreyjaRender::PLANE_YZ:
			mModel->transform(mTransformMode, fScale, 1.0, 1.0, 1.01);
		}
	}
	
	if (x < old_x)
	{
		switch (plane)
		{
		case FreyjaRender::PLANE_XY:
			mModel->transform(mTransformMode, fScale, 0.99, 1.0, 1.0);
			break;
		case FreyjaRender::PLANE_XZ:
			mModel->transform(mTransformMode, fScale, 0.99, 1.0, 1.0);
			break;
		case FreyjaRender::PLANE_YZ:
			mModel->transform(mTransformMode, fScale, 1.0, 0.99, 1.0);
			break;
		}
	}
	else if (x > old_x)
	{
		switch (plane)
		{
		case FreyjaRender::PLANE_XY:
			mModel->transform(mTransformMode, fScale, 1.01, 1.0, 1.0);
			break;
		case FreyjaRender::PLANE_XZ:
			mModel->transform(mTransformMode, fScale, 1.01, 1.0, 1.0);
			break;
		case FreyjaRender::PLANE_YZ:
			mModel->transform(mTransformMode, fScale, 1.0, 1.01, 1.0);
		}
	}
	
	old_x = x;
	old_y = y;
}


void FreyjaControl::MouseEdit(int btn, int state, int mod, int x, int y, 
							  FreyjaRender::plane_t plane) 
{ 
	float xx = x, yy = y;
	//FIXME unsigned int master_tag;
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
		//FIXME mModel->VertexSelect(xx, yy);
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
			//FIXME mModel->VertexCombine(xx, yy);
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
			//FIXME mModel->BBoxSelect(xx, yy);
			_mouse_state = 1;
		}
		else
			_mouse_state = 0;
      break;
	case POINT_DEL_MODE:
		//FIXME mModel->VertexSelect(xx, yy);
		//FIXME mModel->VertexDelete();
		break;
	case BONE_CONNECT_MODE:
		//FIXME master_tag = mModel->getCurrentBone();
		//FIXME mModel->selectBone(xx, yy);
		//FIXME mModel->connectBone(master_tag, mModel->getCurrentBone());
		//FIXME mModel->setCurrentBone(master_tag);
		break;
	case BONE_DISCONNECT_MODE:
		//FIXME master_tag = mModel->getCurrentBone();
		//FIXME mModel->selectBone(xx, yy);
		//FIXME mModel->removeMeshFromBone(master_tag, mModel->getCurrentBone());
		//FIXME mModel->setCurrentBone(master_tag);
		break;
	case MESH_MOVE_CENTER:
		if (_mouse_state == 0)
			_mouse_state = 1;
		else
			_mouse_state = 0;
		break;
	case POINT_ADD_MODE: 
		//FIXME mModel->VertexNew(xx, yy);
		break;
	case POLYGON_ADD_MODE:
		//FIXME mModel->PolygonAddVertex(xx, yy);
		break;
	case POLYGON_DEL_MODE:
		//FIXME mModel->PolygonDelVertex(xx, yy);
		break;
	case POLYGON_SELECT_MODE:
		//FIXME mModel->PolygonSelectVertex(xx, yy);
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
	char *filename = "freyja.mlisp";
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
			freyja_application_window_move(x, y);
		}
	}

	if (mResource->Lookup("DUMP_SYMTAB", &i) && i)
	{
		mResource->Print();
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


