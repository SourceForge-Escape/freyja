/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
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

// Mongoose 2002.01.12, Interfaces for FreyjaResource
//   must be implmented for each interface ( gtk, win32, etc )
arg_list_t *freyja_rc_window(arg_list_t *args);
arg_list_t *freyja_rc_gl_widget(arg_list_t *args);
arg_list_t *freyja_rc_notebook(arg_list_t *args);
arg_list_t *freyja_rc_tab(arg_list_t *args);
arg_list_t *freyja_rc_textbox(arg_list_t *args);
arg_list_t *freyja_rc_hsep(arg_list_t *args);
arg_list_t *freyja_rc_vsep(arg_list_t *args);
arg_list_t *freyja_rc_hbox(arg_list_t *args);
arg_list_t *freyja_rc_vbox(arg_list_t *args);
arg_list_t *freyja_rc_handlebox(arg_list_t *args);
arg_list_t *freyja_rc_label(arg_list_t *args);
arg_list_t *freyja_rc_button(arg_list_t *args);
arg_list_t *freyja_rc_colorbutton(arg_list_t *args);
arg_list_t *freyja_rc_togglebutton(arg_list_t *args);
arg_list_t *freyja_rc_spinbutton(arg_list_t *args);
arg_list_t *freyja_rc_spinbutton2(arg_list_t *args);
arg_list_t *freyja_rc_submenu(arg_list_t *args);
arg_list_t *freyja_rc_menu_seperator(arg_list_t *args);
arg_list_t *freyja_rc_menu_item(arg_list_t *args);
arg_list_t *freyja_rc_optionmenu(arg_list_t *args);
arg_list_t *freyja_rc_popup_menu(arg_list_t *args);
arg_list_t *freyja_rc_menubar(arg_list_t *args);
arg_list_t *freyja_rc_animation_tab_hack(arg_list_t *args);
arg_list_t *freyja_rc_toolbar(arg_list_t *args);
arg_list_t *freyja_rc_toolbar_togglebutton(arg_list_t *args);
arg_list_t *freyja_rc_toolbar_button(arg_list_t *args);
arg_list_t *freyja_rc_toolbar_box(arg_list_t *args);
arg_list_t *freyja_rc_color(arg_list_t *args);

void setupResource(Resource &r)
{
	/* Mongoose 2002.01.12, 
	 * Bind script functions to C/C++ functions */

	r.RegisterFunction("window", freyja_rc_window);
	r.RegisterFunction("gl_widget", freyja_rc_gl_widget);
	r.RegisterFunction("notebook", freyja_rc_notebook);
	r.RegisterFunction("tab", freyja_rc_tab);
	r.RegisterFunction("hsep", freyja_rc_hsep);
	r.RegisterFunction("vsep", freyja_rc_vsep);
	r.RegisterFunction("vbox", freyja_rc_vbox);
	r.RegisterFunction("hbox", freyja_rc_hbox);
	r.RegisterFunction("handlebox", freyja_rc_handlebox);
	r.RegisterFunction("textbox", freyja_rc_textbox);
	r.RegisterFunction("label", freyja_rc_label);
	r.RegisterFunction("colorbutton", freyja_rc_colorbutton);
	r.RegisterFunction("button", freyja_rc_button);
	r.RegisterFunction("togglebutton", freyja_rc_togglebutton);
	r.RegisterFunction("spinbutton", freyja_rc_spinbutton);
	r.RegisterFunction("spinbutton2", freyja_rc_spinbutton2);
	r.RegisterFunction("color", freyja_rc_color);
	r.RegisterFunction("menu_item", freyja_rc_menu_item);
	r.RegisterFunction("submenu", freyja_rc_submenu);
	r.RegisterFunction("menu_seperator", freyja_rc_menu_seperator);
	r.RegisterFunction("optionmenu", freyja_rc_optionmenu);
	r.RegisterFunction("popup_menu", freyja_rc_popup_menu);
	r.RegisterFunction("menubar", freyja_rc_menubar);
	r.RegisterFunction("animation_tab_hack", freyja_rc_animation_tab_hack);
	r.RegisterFunction("toolbar", freyja_rc_toolbar);
	r.RegisterFunction("toolbar_box", freyja_rc_toolbar_box);
	r.RegisterFunction("toolbar_togglebutton", freyja_rc_toolbar_togglebutton);
	r.RegisterFunction("toolbar_button", freyja_rc_toolbar_button);


	/* Mongoose 2002.01.21, 
	 * Bind some script vars to matching name in C/C++ */

	r.RegisterInt("eNone", eNone);
	r.RegisterInt("eCut", eNone);
	r.RegisterInt("eCopy", eNone);
	r.RegisterInt("ePaste", eNone);
	r.RegisterInt("eConfig", eNone);
	r.RegisterInt("eSelectAll", eNone);
	r.RegisterInt("eAppendFile", eNone);
	r.RegisterInt("eRevertFile", eNone);
	r.RegisterInt("eExportFile", eNone);
	r.RegisterInt("eImportFile", eNone);
	r.RegisterInt("eRedo", eNone);
	r.RegisterInt("eUndo", eNone);

	r.RegisterInt("eCloseFile", eCloseFile);
	r.RegisterInt("eDelete", eDelete);
	r.RegisterInt("eSelect", eSelect);

	r.RegisterInt("eScreenShot", eScreenShot);
	r.RegisterInt("eShutdown", eShutdown);
	r.RegisterInt("eNewFile", eNewFile);
	r.RegisterInt("eOpenFile", eOpenFile);
	r.RegisterInt("eSaveFile", eSaveFile);
	r.RegisterInt("eHelp", eHelp);
	r.RegisterInt("eInfo", eInfo);
	r.RegisterInt("eAbout", eAbout);
	r.RegisterInt("eDebugEgg", eDebugEgg);
	r.RegisterInt("eFullscreen", eFullscreen);

	r.RegisterInt("eGenerateNormals", eGenerateNormals);
	r.RegisterInt("eGenerateCube", eGenerateCube);
	r.RegisterInt("eGenerateTriStrip", eGenerateTriStrip);
	r.RegisterInt("eGenerateCylinder", eGenerateCylinder);
	r.RegisterInt("eGenerateSphere", eGenerateSphere);

	r.RegisterInt("ePointJoint", ePointJoint);
	r.RegisterInt("eSphereJoint", eSphereJoint);
	r.RegisterInt("eAxisJoint", eAxisJoint);
	r.RegisterInt("eLineBone", eLineBone);
	r.RegisterInt("ePolyMeshBone", ePolyMeshBone);
	r.RegisterInt("eRenderBbox", eRenderBbox);

	r.RegisterInt("eSetMeshTexture", eSetMeshTexture);
	r.RegisterInt("eTextureSlotLoad", eTextureSlotLoad);

	r.RegisterInt("eAddObject", eAddObject);
	r.RegisterInt("eMoveObject", eMoveObject);
	r.RegisterInt("eRotateObject", eRotateObject);
	r.RegisterInt("eScaleObject", eScaleObject);

	r.RegisterInt("eTransformGroup", eTransformGroup);
	r.RegisterInt("eTransformScene", eTransformScene);
	r.RegisterInt("eTransformMesh", eTransformMesh);
	r.RegisterInt("eTransformPoint", eTransformPoint);


	/* Widget events ( widgets hold data like spinbuttons, etc ) */
	r.RegisterInt("eScale", eScale);
	r.RegisterInt("eScale_X", eScale_X);
	r.RegisterInt("eScale_Y", eScale_Y);
	r.RegisterInt("eScale_Z", eScale_Z);
	r.RegisterInt("eMove", eMove);
	r.RegisterInt("eMove_X", eMove_X);
	r.RegisterInt("eMove_Y", eMove_Y);
	r.RegisterInt("eMove_Z", eMove_Z);
	r.RegisterInt("eRotate", eRotate);
	r.RegisterInt("eRotate_X", eRotate_X);
	r.RegisterInt("eRotate_Y", eRotate_Y);
	r.RegisterInt("eRotate_Z", eRotate_Z);
	r.RegisterInt("eZoom", eZoom);
	r.RegisterInt("eSelectMaterial", eSelectMaterial);


	/* TODO:
	 * 1. Remove events below here and replace with new events
	 * 2. Then replace all the numbered gnome_callback events
	 * 3. Then refactor the new event groupings in plugins  */
	r.RegisterInt("EVENT_MAIN", EVENT_MAIN);

	r.RegisterInt("EVENT_POINT", EVENT_POINT);
	r.RegisterInt("CMD_POINT_ADD", CMD_POINT_ADD);
	r.RegisterInt("CMD_POINT_COMBINE", CMD_POINT_COMBINE);

	r.RegisterInt("EVENT_POLYGON", EVENT_POLYGON);
	r.RegisterInt("CMD_POLYGON_ADD", CMD_POLYGON_ADD);
	r.RegisterInt("CMD_POLYGON_DELETE", CMD_POLYGON_DELETE);	

	r.RegisterInt("EVENT_MESH", EVENT_MESH);
	r.RegisterInt("CMD_MESH_MOVE_CENTER", CMD_MESH_MOVE_CENTER);
	
	r.RegisterInt("EVENT_BONE", EVENT_BONE);
	r.RegisterInt("CMD_BONE_ADD_MESH", CMD_BONE_ADD_MESH);
	r.RegisterInt("CMD_BONE_MOVE_PIVOT", CMD_BONE_MOVE_PIVOT);

	r.RegisterInt("EVENT_MISC", EVENT_MISC);
	r.RegisterInt("CMD_MISC_ZOOM_IN", CMD_MISC_ZOOM_IN);
	r.RegisterInt("CMD_MISC_ZOOM_OUT", CMD_MISC_ZOOM_OUT);
	r.RegisterInt("CMD_MISC_BBOX_SELECT", CMD_MISC_BBOX_SELECT);
	r.RegisterInt("CMD_MISC_GEN_TEXMAP_XY", CMD_MISC_GEN_TEXMAP_XY);
	r.RegisterInt("CMD_MISC_VERTEX_UV", CMD_MISC_VERTEX_UV);

	r.RegisterInt("EVENT_MATERIAL", EVENT_MATERIAL);

	r.RegisterInt("EVENT_FREYJA_MODE", EVENT_FREYJA_MODE);
	r.RegisterInt("FREYJA_MODE_MODEL_EDIT", FREYJA_MODE_MODEL_EDIT);
	r.RegisterInt("FREYJA_MODE_MATERIAL_EDIT", FREYJA_MODE_MATERIAL_EDIT);
	r.RegisterInt("FREYJA_MODE_TEXTURE_EDIT", FREYJA_MODE_TEXTURE_EDIT);
	r.RegisterInt("FREYJA_MODE_ANIMATION", FREYJA_MODE_ANIMATION);
	r.RegisterInt("FREYJA_MODE_RENDER_WIREFRAME",FREYJA_MODE_RENDER_WIREFRAME);
	r.RegisterInt("FREYJA_MODE_RENDER_FACE", FREYJA_MODE_RENDER_FACE);
	r.RegisterInt("FREYJA_MODE_RENDER_POINTS", FREYJA_MODE_RENDER_POINTS);
	r.RegisterInt("FREYJA_MODE_RENDER_NORMALS", FREYJA_MODE_RENDER_NORMALS);
	r.RegisterInt("FREYJA_MODE_RENDER_TEXTURE", FREYJA_MODE_RENDER_TEXTURE);
	r.RegisterInt("FREYJA_MODE_RENDER_LIGHTING", FREYJA_MODE_RENDER_LIGHTING);
	r.RegisterInt("FREYJA_MODE_RENDER_MATERIAL", FREYJA_MODE_RENDER_MATERIAL);
	r.RegisterInt("FREYJA_MODE_RENDER_BONETAG",FREYJA_MODE_RENDER_BONETAG);
	r.RegisterInt("FREYJA_MODE_RENDER_GRID", FREYJA_MODE_RENDER_GRID);
	r.RegisterInt("FREYJA_MODE_PLANE_XY", FREYJA_MODE_PLANE_XY);
	r.RegisterInt("FREYJA_MODE_PLANE_YZ", FREYJA_MODE_PLANE_YZ);
	r.RegisterInt("FREYJA_MODE_PLANE_XZ", FREYJA_MODE_PLANE_XZ);
}



FreyjaControl::FreyjaControl()
{
	unsigned int width = 740;
	unsigned int height = 560;


	setupResource(mResource);

	mModel = new FreyjaModel();
	mRender = new FreyjaRender();
	mRender->Register(mModel);

	if (EGG_LIB_VERSION != 0x32312E38)
	{
		printf("FreyjaControl::FreyjaControl> Assertion failure\n");
		printf("*** You must upgrade libfreyja_model to v8.12\n");
		exit(-1);
	}

	if (!mModel || !mRender)
	{
		printf("FreyjaControl::FreyjaControl> Assertion failure\n");
		exit(-1);
	}
	
	_minor_mode = NO_MODE;
	_major_mode = MODEL_EDIT_MODE;
	
	event_register_render(mRender);
	event_register_model(mModel);
	event_register_control(this);

	ReadRC();

	mTransformMode = FreyjaModel::TransformMesh;
	mLastEvent = EVENT_MAIN;
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
	/* Mongoose 2004.03.26, 
	 * Entry for MaterialManager test pattern */
	MaterialManager::DestroyInstance();
}


void FreyjaControl::setZoom(float zoom)
{
	assert(zoom > 0.0f);

	freyja_event_notify_observer1f(eZoom, zoom);

	mRender->setZoom(zoom);
	event_print("Zoom set to %f", mRender->getZoom());
}


float FreyjaControl::getZoom()
{
	return mRender->getZoom();
}


void FreyjaControl::eventMain(int event)
{
	unsigned int i;
	char *s;


	switch (event)
	{
	case eInfo:
		mModel->printInfo();
		break;

	case eHelp:
		mResource.Lookup("HELP_MESSAGE", &s);

		for (i = 1; i < strlen(s); ++i)
		{
			if (((unsigned char *)s)[i-1] == 0xC2 && 
				((unsigned char *)s)[i] == 0xB6)
			{
				s[i-1] = ' ';
				s[i] = '\n';
			}
		}

		freyja_event_info_dialog("gtk-dialog-info", s);
		// system("gedit /home/mongoose/Projects/freyja/Freyja/README");
		break; 

	case eAbout:
		mResource.Lookup("ABOUT_MESSAGE", &s);

		for (i = 1; i < strlen(s); ++i)
		{
			if (((unsigned char *)s)[i-1] == 0xC2 && 
				((unsigned char *)s)[i] == 0xB6)
			{
				s[i-1] = ' ';
				s[i] = '\n';
			}
		}

		freyja_get_pixmap_filename(mScratchTextBuffer, 1024, "freyja.png");
		freyja_event_info_dialog(mScratchTextBuffer, s);
		break;   


	case eCloseFile:
		switch (_major_mode)
		{
		case TEXTURE_EDIT_MODE:
			break;
		case ANIMATION_EDIT_MODE:
			break;
		case MAP_EDIT_MODE:
			break;
		case MODEL_EDIT_MODE:
			if (confirmDialog("gtk-dialog-question",
							  "You must close the currently open model to create a new model.",
							  "Would you like to close the model and lose unsaved changes?",
							  "gtk-cancel", "_Cancel", "gtk-close", "C_lose"))
			{
				mModel->Clear();
				event_print("Closing Model...");
			}
			break;
		case MATERIAL_EDIT_MODE:
			i = gMaterialManager->createNewMaterial();
			event_print("New material [%i] created.", i);
			break;
		}
		break;


	case eNewFile:
		switch (_major_mode)
		{
		case TEXTURE_EDIT_MODE:
			break;
		case ANIMATION_EDIT_MODE:
			break;
		case MAP_EDIT_MODE:
			break;
		case MODEL_EDIT_MODE:
			if (confirmDialog("gtk-dialog-question",
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
				event_print("Closing Model...");
			}
			break;
		case MATERIAL_EDIT_MODE:
			i = gMaterialManager->createNewMaterial();
			event_print("New material [%i] created.", i);
			break;
		}
		break;


	case eOpenFile:
		switch (_major_mode)
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
		switch (_major_mode)
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
		if (confirmDialog("gtk-dialog-question",
						  "If you exit you will lose any unsaved data.",
						  "Would you like to exit?", 
						  "gtk-cancel", "_Cancel", "gtk-quit", "_Exit"))
		{
			freyja_event_exit();
		}
		break;


	case eDebugEgg:
		mModel->Debug(!mModel->Debug());
		event_print("Egg debug [%s]", mModel->Debug() ? "ON" : "OFF");
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


	case eGenerateCylinder:
		eggGenerateCylinder(4, 16, 8.0, 4.0);
		event_refresh();
		break;


	case eGenerateSphere:
		eggGenerateSphere(16, 16, 8.0);
		event_refresh();
		break;


	case eGenerateCube:
		eggGenerateCube(8.0);
		event_refresh();
		break;


	case eGenerateTriStrip:
		eggGenerateTriangleStrip(10, 8.0);
		event_refresh();
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
		event_print("BoundingBox rendering [%s]", 
					(mRender->Flags() & FreyjaRender::RENDER_BBOX) ? 
					"ON" : "OFF");
		break;


	case eSetMeshTexture:
		event_print("Switching all of mesh %i ploygons to texture %i",
					mModel->getCurrentMesh(), mModel->getCurrentTextureIndex());
		mModel->TextureShift();
		break;


	case eTextureSlotLoad:
		if (!gMaterialManager->getGeneralFlags() & 
			MaterialManager::fLoadTextureInSlot)
		{
			gMaterialManager->setGeneralFlag(MaterialManager::fLoadTextureInSlot);
		}
		else
		{
			gMaterialManager->clearGeneralFlag(MaterialManager::fLoadTextureInSlot);
		}	

		event_print("Texture loading to current slot [%s]",
					gMaterialManager->getGeneralFlags() & 
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


	case eDelete:
		deleteSelectedObject();
		break;

	case eAddObject:
		addObject();
		break;

	case eMoveObject:
		_minor_mode = modeMove;
		event_print("Move object...");
		break;

	case eScaleObject:
		_minor_mode = modeScale;
		event_print("Scale object...");
		break;

	case eRotateObject:
		_minor_mode = modeRotate;
		event_print("Rotate object...");
		break;

	case eSelect:
		_minor_mode = modeSelect;
		event_print("Select object...");
		break;


	case eMove:
		mModel->Transform(mTransformMode, Egg::TRANSLATE,
						  freyja_event_get_float(eMove_X),
						  freyja_event_get_float(eMove_Y),
						  freyja_event_get_float(eMove_Z));
		event_refresh();
		
		freyja_event_set_float(eMove_X, 0.0f);
		freyja_event_set_float(eMove_Y, 0.0f);
		freyja_event_set_float(eMove_Z, 0.0f);
		break;

	case eRotate:
		mModel->Transform(mTransformMode, Egg::ROTATE,
						  freyja_event_get_float(eRotate_X),
						  freyja_event_get_float(eRotate_Y),
						  freyja_event_get_float(eRotate_Z));
		event_refresh();
		
		freyja_event_set_float(eRotate_X, 0.0f);
		freyja_event_set_float(eRotate_Y, 0.0f);
		freyja_event_set_float(eRotate_Z, 0.0f);
		break;

	case eScale:
		mModel->Transform(mTransformMode, Egg::SCALE,
						  freyja_event_get_float(eScale_X),
						  freyja_event_get_float(eScale_Y),
						  freyja_event_get_float(eScale_Z));
		event_refresh();
		
		freyja_event_set_float(eScale_X, 1.0f);
		freyja_event_set_float(eScale_Y, 1.0f);
		freyja_event_set_float(eScale_Z, 1.0f);
		break;

	default:
		event_print("!Unhandled Event(%d)", event);
	}	
}


void FreyjaControl::eventBone(int command)
{
	mTransformMode = FreyjaModel::TransformBone;
	
	switch (command)
	{
	case CMD_BONE_CONNECT:
		event_print("Select bone to connect to current");
		_minor_mode = BONE_CONNECT_MODE;
		break;
	case CMD_BONE_DISCONNECT:
		event_print("Select bone to break from current");
		_minor_mode = BONE_DISCONNECT_MODE;
		break;
	case CMD_BONE_NEXT:
		mModel->setCurrentBone(mModel->getCurrentBone() + 1);
		break;
	case CMD_BONE_PREV:
		if (mModel->getCurrentBone())
			mModel->setCurrentBone(mModel->getCurrentBone() - 1);
		break;
	case CMD_BONE_ADD_MESH:
		mModel->TagAddMesh(mModel->getCurrentBone(), mModel->getCurrentMesh());
		event_print("New Bone[%i] now contains mesh %i",
						mModel->getCurrentBone(), mModel->getCurrentMesh());
		break;
	case CMD_BONE_DELETE_MESH:
		mModel->TagDelMesh(mModel->getCurrentBone(), mModel->getCurrentMesh());
		event_print("New Bone[%i] no longer contains mesh %i",
						mModel->getCurrentBone(), mModel->getCurrentMesh());
		break;
	default:
		event_print("!Unhandled boneEvent(%d)", command);
	}
}


void FreyjaControl::eventMesh(int command)
{
	mTransformMode = FreyjaModel::TransformMesh;

	switch (command)
	{
	case CMD_MESH_MOVE_CENTER:
		_minor_mode = MESH_MOVE_CENTER;
		event_print("Reposition mesh center point");
		break;
	case CMD_MESH_NEXT:
		mModel->setCurrentMesh(mModel->getCurrentMesh() + 1);
		break;
	case CMD_MESH_PREV:
		if (mModel->getCurrentMesh())
			mModel->setCurrentMesh(mModel->getCurrentMesh() - 1);
		break;
	default:
		event_print("Unhandled meshEvent(%d)", command);
	}
}


void FreyjaControl::eventAnimation(int command)
{
	switch (command)
	{
	case CMD_ANIMATION_NEXT:
		mModel->setCurrentAnimation(mModel->getCurrentAnimation() + 1);
		break;


	case CMD_ANIMATION_PREV:
		if (mModel->getCurrentAnimation())
			mModel->setCurrentAnimation(mModel->getCurrentAnimation() - 1);
		else
			mModel->setCurrentAnimation(0);
		break;


	default:
		event_print("!Unhandled animationEvent(%d)", command);
	}

}


void FreyjaControl::eventMisc(int command)
{
	unsigned int i;
	float x, y, z;


	switch (command)
	{
	case 500:
		i = (int)freyja_event_get_float(command);

		if (!freyja_event_set_range(command, i, 0, eggGetNum(FREYJA_MESH)))
		{
			mModel->setCurrentMesh(i);			
			event_refresh();
		}
		break;


	case 501:
		i = (int)freyja_event_get_float(command);

		if (!freyja_event_set_range(command, i, 0, eggGetNum(FREYJA_GROUP)))
		{
			i = (int)freyja_event_get_float(command);
			mModel->setCurrentGroup(i);
			event_refresh();
		}
		break;


	case 503:
		i = (int)freyja_event_get_float(command);

		if (i != mModel->getCurrentTextureIndex())
		{
			mModel->setCurrentTextureIndex(i);
			//value = mModel->getCurrentTextureIndex();
			//spinbutton_value_set(event, value);
			event_refresh();
		}
		break;
	case 504:
		if (1)//!spinbutton_uint_set_range(spin, value, 0, eggGetNum(FREYJA_BONE)))
		{
			i = (int)freyja_event_get_float(command);
			mModel->setCurrentBone(i);
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
			event_refresh();
		}
		break;
	case 717:
	case 718:
		i = (int)freyja_event_get_float(command);

		gMaterialManager->setTexture(command - 717, i);
		event_print("Material[%i].texture[%d] = %i",
					gMaterialManager->getCurrent(), 
					command - 717,
					gMaterialManager->getTexture(command - 717));
		event_refresh();
		break;


	/* "Well behaved" events */
	case eSelectMaterial:
		i = (int)freyja_event_get_float(eSelectMaterial);

		if (i != gMaterialManager->getCurrent())
		{
			gMaterialManager->setCurrent(i);
			freyja_event_set_float(eSelectMaterial, 
								   gMaterialManager->getCurrent());
			event_print("Selected material[%i].", i);

			refresh_material_interface();
			event_refresh();
		}
		break;


	case eZoom:
		setZoom(freyja_event_get_float(eZoom));
		event_refresh();
		break;


	case eMoveBone_X:
	case eMoveBone_Y:
	case eMoveBone_Z:
		break;


	case eMove:
	case eMove_X:
	case eMove_Y:
	case eMove_Z:
		break;


	case eRotate:
	case eRotate_X:
	case eRotate_Y:
	case eRotate_Z:
		break;


	case eScale:
	case eScale_X:
	case eScale_Y:
	case eScale_Z:
		break;


	case CMD_MISC_RENDER_ROT_Z_M:
		mRender->Rotate(Z_F, -mRender->RotateAmount());
		break;
	case CMD_MISC_RENDER_ROT_Z_P:
		mRender->Rotate(Z_F, mRender->RotateAmount());
		break;
	case CMD_MISC_RENDER_ROT_Y_M:
		mRender->Rotate(Y_F, -mRender->RotateAmount());
		break;
	case CMD_MISC_RENDER_ROT_Y_P:
		mRender->Rotate(Y_F, mRender->RotateAmount());
		break;
	case CMD_MISC_RENDER_ROT_X_M:
		mRender->Rotate(X_F, -mRender->RotateAmount());
		break;
	case CMD_MISC_RENDER_ROT_X_P:
		mRender->Rotate(X_F, mRender->RotateAmount());
		break;
	case CMD_MISC_BBOX_SELECT:
		if (_minor_mode == VERTEX_BBOX_SELECT_MODE)
		{
			_minor_mode = NO_MODE;
			mRender->Flags(FreyjaRender::RENDER_BBOX_SEL, 0);
			mModel->BBoxListBuild();
			event_print("Vertex box select: Vertex list built");
		}
		else
		{
			_minor_mode = VERTEX_BBOX_SELECT_MODE;
			mRender->Flags(FreyjaRender::RENDER_BBOX_SEL, 1);
			event_print("Vertex box select: Press agian to end selection");
		}
		break;
	case CMD_MISC_TEXEL_COMBINE:
		if (_minor_mode != TEXEL_COMBINE)
		{
			_minor_mode = TEXEL_COMBINE;
		}
		else
		{	
			_minor_mode = NO_MODE;
		}

		event_print("Texel combine [%s]", 
					(_minor_mode == TEXEL_COMBINE) ? "on" : "off");
		break;



		case CMD_MISC_GEN_TEXMAP_XY:
			mModel->generateUVMap();
			break;


		case CMD_MISC_SIDES_M:
			mModel->setCurrentPolygonEdgeCount(mModel->getCurrentPolygonEdgeCount()-1);
			event_print("Making %i sided polygons", 
						mModel->getCurrentPolygonEdgeCount());
			break;
		case CMD_MISC_SIDES_P:
			mModel->setCurrentPolygonEdgeCount(mModel->getCurrentPolygonEdgeCount()+1);
			event_print("Making %i sided polygons", 
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

			event_refresh();
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

			event_refresh();
			break;

	default:
		event_print("!Unhandled eventMisc(%d)", command);
	}   
}


void FreyjaControl::handleEvent(int mode, int cmd)
{
	if (mode == EVENT_REDO_LAST)
	{
		mode = mLastEvent;
		cmd = mLastCommand;
	}
	
	switch (mode)
	{
	case EVENT_ANIMATION:
		eventAnimation(cmd);
		break;

	case EVENT_BONE:
		eventBone(cmd);
		break;

 	case EVENT_MAIN:
		eventMain(cmd);
 		break;

	case EVENT_MESH:
		eventMesh(cmd);
		break;

	case EVENT_MISC:
		eventMisc(cmd);
		break;

	case EVENT_FREYJA_MODE:
		switch (cmd)
		{
		case FREYJA_MODE_RENDER_BONETAG:
			mRender->Flags(FreyjaRender::RENDER_BONES, 
								!(mRender->Flags() & FreyjaRender::RENDER_BONES));
			event_print("Bone Rendering [%s]", 
							(mRender->Flags() & FreyjaRender::RENDER_BONES) ? 
							"ON" : "OFF");
			event_refresh();
			break;
		case FREYJA_MODE_RENDER_POINTS:
			mRender->Flags(FreyjaRender::RENDER_POINTS, 
								!(mRender->Flags() & FreyjaRender::RENDER_POINTS));
			event_print("Point Rendering [%s]", 
							(mRender->Flags() & FreyjaRender::RENDER_POINTS) ? 
							"ON" : "OFF");
			event_refresh();
			break;
		case FREYJA_MODE_RENDER_WIREFRAME:
			mRender->Flags(FreyjaRender::RENDER_WIREFRAME, 
								!(mRender->Flags() & FreyjaRender::RENDER_WIREFRAME));
			event_print("Wireframe Rendering [%s]", 
							(mRender->Flags() & FreyjaRender::RENDER_WIREFRAME) ? 
							"ON" : "OFF");
			event_refresh();
			break;
		case FREYJA_MODE_RENDER_FACE:
			mRender->Flags(FreyjaRender::RENDER_FACE, 
								!(mRender->Flags() & FreyjaRender::RENDER_FACE));
			event_print("Face Rendering [%s]", 
							(mRender->Flags() & FreyjaRender::RENDER_FACE) ? 
							"ON" : "OFF");
			event_refresh();
			break;
		case FREYJA_MODE_RENDER_NORMALS:
			mRender->Flags(FreyjaRender::RENDER_NORMALS, 
								!(mRender->Flags() & FreyjaRender::RENDER_NORMALS));
			event_print("Normal Rendering [%s]", 
							(mRender->Flags() & FreyjaRender::RENDER_NORMALS) ? 
							"ON" : "OFF");
			event_refresh();
			break;
		case FREYJA_MODE_RENDER_LIGHTING:
			mRender->Flags(FreyjaRender::RENDER_NORMAL, 
								!(mRender->Flags() & FreyjaRender::RENDER_NORMAL));
			mRender->Flags(FreyjaRender::RENDER_LIGHTING, 
								!(mRender->Flags() & FreyjaRender::RENDER_LIGHTING));
			event_print("GL Lighting is [%s]", 
							(mRender->Flags() & FreyjaRender::RENDER_LIGHTING) ? 
							"ON" : "OFF");
			event_refresh();
			break;
		case FREYJA_MODE_RENDER_TEXTURE:
			mRender->Flags(FreyjaRender::RENDER_TEXTURE, 
								!(mRender->Flags() & FreyjaRender::RENDER_TEXTURE));
			event_print("Texture rendering is [%s]", 
							(mRender->Flags() & FreyjaRender::RENDER_TEXTURE) ? 
							"ON" : "OFF");
			event_refresh();
			break;
		case FREYJA_MODE_RENDER_MATERIAL:
			mRender->Flags(FreyjaRender::RENDER_MATERIAL, 
								!(mRender->Flags() & FreyjaRender::RENDER_MATERIAL));
			event_print("Material rendering is [%s]", 
							(mRender->Flags() & FreyjaRender::RENDER_MATERIAL) ? 
							"ON" : "OFF");
			event_refresh();
			break;
		case FREYJA_MODE_RENDER_GRID:
			mRender->Flags(FreyjaRender::RENDER_EDIT_GRID, 
								!(mRender->Flags() & FreyjaRender::RENDER_EDIT_GRID));
			event_print("Edit Grid rendering [%s]",
						(mRender->Flags() & FreyjaRender::RENDER_EDIT_GRID) ? 
							"ON" : "OFF");
			event_refresh();
			break;

		case FREYJA_MODE_TEXTURE_EDIT:
			mRender->ViewMode(VIEWMODE_TEXTURE_EDIT);
			event_refresh();
			event_print("Texture Edit");
			_major_mode = TEXTURE_EDIT_MODE;
			break;
		case FREYJA_MODE_MODEL_EDIT:
			mRender->ViewMode(VIEWMODE_MODEL_EDIT);
			event_refresh();
			event_print("Model Edit");
			_major_mode = MODEL_EDIT_MODE;
			break;
		case FREYJA_MODE_MODEL_VIEW:
			mRender->ViewMode(VIEWMODE_MODEL_VIEW);
			event_refresh();
			event_print("Animation View");
			_major_mode = ANIMATION_EDIT_MODE;
			break;
		case FREYJA_MODE_MATERIAL_EDIT:
			mRender->ViewMode(VIEWMODE_MATERIAL_EDIT);
			event_refresh();
			event_print("Material Edit");
			_major_mode = MATERIAL_EDIT_MODE;
			break;
		case FREYJA_MODE_PLANE_XY:
			event_print("Plane XY");      
			mModel->CurrentPlane(Egg::PLANE_XY);
			event_refresh();
			break;
		case FREYJA_MODE_PLANE_XZ:
			event_print("Plane XZ");      
			mModel->CurrentPlane(Egg::PLANE_XZ);
			event_refresh();
			break;
		case FREYJA_MODE_PLANE_YZ:
			event_print("Plane YZ");      
			mModel->CurrentPlane(Egg::PLANE_YZ);
			event_refresh();
			break;
		}
		break;


	case EVENT_POLYGON:
		switch (cmd)
		{
		case CMD_POLYGON_DELETE:
			_minor_mode = POLYGON_DEL_MODE;
			event_print("Select polygon by vertices to delete");
			break;
		case CMD_POLYGON_ADD:
			_minor_mode = POLYGON_ADD_MODE;
			event_print("Select vertices to create a polygon");
			break;
		case CMD_POLYGON_SELECT:
			_minor_mode = POLYGON_SELECT_MODE;
			event_print("Select polygon by vertices to highlight it");
			break;
		}
		break;


	case EVENT_POINT:
		switch (cmd)
		{
		case CMD_POINT_DELETE:
			_minor_mode = POINT_DEL_MODE;
			event_print("Select vertex to delete");
			break;
		case CMD_POINT_ADD:
			_minor_mode = POINT_ADD_MODE;
			event_print("Select point in space to create a vertex");
			break;    
		case CMD_POINT_COMBINE:
			if (_minor_mode != VERTEX_COMBINE)
			{
				_minor_mode = VERTEX_COMBINE;
			}
			else
			{	
				_minor_mode = NO_MODE;
			}

			event_print("Vertex combine [%s]", 
							(_minor_mode == VERTEX_COMBINE) ? "on" : "off");
			break;
		}
		break;

	default:
		event_print("!UnHandled mode %d and command %d", mode, cmd);
	}

	mLastEvent = mode;
	mLastCommand = cmd;
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
		type = 2;
		type2 = 1;
		break;
	case FREYJA_MODE_LOAD_TEXTURE:
		failed = mMaterial->loadTexture(filename);
		type = 1;
		type2 = 1;
		event_refresh();
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
	event_print("%s '%s' %s%s", 
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


void FreyjaControl::ReadRC()
{
	int i, x, y;
	float f;
	char *s;
	char *filename = "freyja.mlisp";
	bool failed = true;

	
	s = freyja_rc_map(filename);

	if (!mResource.Load(s))
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
	if (mResource.Lookup("WINDOW_X", &x))
	{
		if (mResource.Lookup("WINDOW_Y", &y))
		{
			extern void application_window_move(int x, int y);
			application_window_move(x, y);
		}
	}

	if (mResource.Lookup("DUMP_SYMTAB", &i) && i)
	{
		mResource.Print();
	}

	if (mResource.Lookup("GRID_ON", &i) && i)
	{
		mRender->Flags(FreyjaRender::RENDER_EDIT_GRID, 
							!(mRender->Flags() & FreyjaRender::RENDER_EDIT_GRID));
	}

	if (mResource.Lookup("ROTATE_AMOUNT", &f))
	{
		mRender->RotateAmount(f);
	}

	if (mResource.Lookup("FLUSH_RESOURCE", &i) && i)
	{
		mResource.Flush();
	}
}


void FreyjaControl::ScreenToWorld(float *x, float *y)
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

#ifdef DEBUG_SCREEN_TO_WORLD
		printf("World (%.3f, %.3f, 0.000)\n", *x, *y);
#endif
		break;
	case Egg::PLANE_XZ:
		*x -= scroll[0] * invz;
		*y -= scroll[2] * invz;

#ifdef DEBUG_SCREEN_TO_WORLD
		printf("World (%.3f, 0.000, %.3f)\n", *x, *y);
#endif
		break;
	case Egg::PLANE_YZ:
		*x -= scroll[1] * invz;
		*y -= scroll[2] * invz;

#ifdef DEBUG_SCREEN_TO_WORLD
		printf("World (0.000, %.3f, %.3f)\n", *x, *y);
#endif
		break;
	}
}


bool FreyjaControl::Motion(int x, int y)
{
	static int old_y = 0, old_x = 0;

	
	switch (_major_mode)
	{
	case ANIMATION_EDIT_MODE:
		switch (mMouseButton)
		{
		case MOUSE_BTN_RIGHT:
			if (x > old_x)
				freyja_event2i(EVENT_MISC, CMD_MISC_RENDER_ROT_Y_M);
			else if (x < old_x)
				freyja_event2i(EVENT_MISC, CMD_MISC_RENDER_ROT_Y_P);
			
			if (y > old_y)
				freyja_event2i(EVENT_MISC, CMD_MISC_RENDER_ROT_X_M);
			else if (y < old_y)
				freyja_event2i(EVENT_MISC, CMD_MISC_RENDER_ROT_X_P);

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
	
	//	event_print("motion -> %i, %i", x, y);

	return true;
}


bool FreyjaControl::Mouse(int btn, int state, int mod, int x, int y)
{
	freyja_control_mode_t mode = _minor_mode;


	mMouseButton = btn;
	mModKey = mod;

	switch (_major_mode)
	{
	case ANIMATION_EDIT_MODE:
		if (btn == MOUSE_BTN_UP && state == MOUSE_BTN_STATE_PRESSED)
		{
			freyja_event2i(EVENT_MISC, CMD_MISC_ZOOM_IN);
			event_print("Zoom in");
		}
		
		if (btn == MOUSE_BTN_DOWN && state == MOUSE_BTN_STATE_PRESSED)
		{
			freyja_event2i(EVENT_MISC, CMD_MISC_ZOOM_OUT);
			event_print("Zoom out");
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
			
			if (_minor_mode == TEXEL_COMBINE)
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
			handleEvent(EVENT_MAIN, eSelect);
		}
		else if (mod & KEY_LSHIFT)
		{
			handleEvent(EVENT_MAIN, eRotate);
		}

		MouseEdit(btn, state, mod, x, y, mModel->CurrentPlane());
		break;
	default:
		;
	}

	// Mongoose 2002.01.12, Reset mode
	_minor_mode = mode;

	return true;
}


//////////////////////////////////////////////////////////////
// Private methods
//////////////////////////////////////////////////////////////

// FIXME: (int x, int y, Egg::egg_plane plane) for vertex / texel / polygon
void FreyjaControl::deleteSelectedObject()
{
	switch (mTransformMode)
	{
	case FreyjaModel::TransformBone:
		event_print("NOT IMPLEMENTED: Deleting Bone Tag %d", 
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
	switch (mTransformMode)
	{
	case FreyjaModel::TransformBone:
		mModel->TagNew(0.0, 0.0, 0.0, 0x0);
		event_print("New Bone Tag");
		break;
	case FreyjaModel::TransformMesh:
		mModel->MeshNew();
		break;
	}
}


void FreyjaControl::selectObject(int x, int y, Egg::egg_plane plane)
{
	float xx = x, yy = y;


	/* Mongoose: Convert screen to world coordinate system */
	ScreenToWorld(&xx, &yy);

	switch (mTransformMode)
	{
	case FreyjaModel::TransformPoint:
		mModel->VertexSelect(xx, yy);
		break;
	case FreyjaModel::TransformMesh:
		mModel->MeshSelect(xx, yy);
		break;
	case FreyjaModel::TransformBone:
		mModel->TagSelect(xx, yy);
	default:
		event_print("WARNING: Selection undefined for this mode");
	}
}


void FreyjaControl::moveObject(int x, int y, Egg::egg_plane plane)
{
	static int old_y = 0, old_x = 0;
	const float t = 2.0f, m = 0.5f;
	vec3_t center;
	float xx = x, yy = y, xf, yf, zf;


	/* Mongoose: Convert screen to world coordinate system */
	ScreenToWorld(&xx, &yy);


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
		mModel->VertexMove(xx, yy);
		break;

	case FreyjaModel::TransformBone:
		mModel->TagMove(xx, yy);
		break;			
		
	case FreyjaModel::TransformMesh:
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
		
		mModel->Transform(mTransformMode, Egg::TRANSLATE, xf, yf, zf); 
		break;


	default:  
		/* Relative movement based on mouse tracking */
		mModel->Transform(mTransformMode, Egg::TRANSLATE, xf, yf, zf);
		break;
	}

	old_x = x;
	old_y = y;
}


void FreyjaControl::rotateObject(int x, int y, Egg::egg_plane plane)
{
	static int old_y = 0, old_x = 0;
	const float t = 1.0f, m = 1.0f;
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

	mModel->Transform(mTransformMode, rotate, xf, yf, zf);
	
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
			mModel->Transform(mTransformMode, Egg::SCALE, 1.0, 0.99, 1.0);
			break;
		case Egg::PLANE_XZ:
			mModel->Transform(mTransformMode, Egg::SCALE, 1.0, 1.0, 0.99);
			break;
		case Egg::PLANE_YZ:
			mModel->Transform(mTransformMode, Egg::SCALE, 1.0, 1.0, 0.99);
			break;
		}
	}
	else if (y < old_y)
	{
		switch (plane)
		{
		case Egg::PLANE_XY:
			mModel->Transform(mTransformMode, Egg::SCALE, 1.0, 1.01, 1.0);
			break;
		case Egg::PLANE_XZ:
			mModel->Transform(mTransformMode, Egg::SCALE, 1.0, 1.0, 1.01);
			break;
		case Egg::PLANE_YZ:
			mModel->Transform(mTransformMode, Egg::SCALE, 1.0, 1.0, 1.01);
		}
	}
	
	if (x < old_x)
	{
		switch (plane)
		{
		case Egg::PLANE_XY:
			mModel->Transform(mTransformMode, Egg::SCALE, 0.99, 1.0, 1.0);
			break;
		case Egg::PLANE_XZ:
			mModel->Transform(mTransformMode, Egg::SCALE, 0.99, 1.0, 1.0);
			break;
		case Egg::PLANE_YZ:
			mModel->Transform(mTransformMode, Egg::SCALE, 1.0, 0.99, 1.0);
			break;
		}
	}
	else if (x > old_x)
	{
		switch (plane)
		{
		case Egg::PLANE_XY:
			mModel->Transform(mTransformMode, Egg::SCALE, 1.01, 1.0, 1.0);
			break;
		case Egg::PLANE_XZ:
			mModel->Transform(mTransformMode, Egg::SCALE, 1.01, 1.0, 1.0);
			break;
		case Egg::PLANE_YZ:
			mModel->Transform(mTransformMode, Egg::SCALE, 1.0, 1.01, 1.0);
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
		ScreenToWorld(&xx, &yy);

		switch (_minor_mode)
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
			mModel->TagMoveCenter(xx, yy);
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
	
	ScreenToWorld(&xx, &yy);
	
	if (!(btn == MOUSE_BTN_LEFT && state == MOUSE_BTN_STATE_PRESSED))
	{	
		switch(_minor_mode)
		{
		case modeMove:
			event_print("moved: %f, %f", xx-xxx, yy-yyy);
			break;
		default:
			;
		}

		return;
	}

	switch(_minor_mode)
	{
	case modeMove:
		//	if (_mouse_state == 0)
		//{
		xxx = xx;  yyy = yy;
		event_print("store state: %f, %f", xxx, yyy);
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
		mModel->TagSelect(xx, yy);
		mModel->TagConnect(master_tag, mModel->getCurrentBone());
		mModel->setCurrentBone(master_tag);
		break;
	case BONE_DISCONNECT_MODE:
		master_tag = mModel->getCurrentBone();
		mModel->TagSelect(xx, yy);
		mModel->TagDisconnect(master_tag, mModel->getCurrentBone());
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

