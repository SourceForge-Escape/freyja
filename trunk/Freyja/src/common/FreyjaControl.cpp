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

#define ABOUT_MESSAGE "\t\t\t<big>Freyja 0.3.5</big>\nFreyja is an Open Source 3d modeling system.\n<small>Freyja Copyright (c) 2002-2004 by Terry 'Mongoose' Hendrix II</small>"

#define HELP_MESSAGE "There is no online help as yet.\n\nIf you'd like to work on documentation email me.\n\nSend bug reports and feature requests:\n<small>  * http://icculus.org/freyja\n  * irc://irc.freenode.net/#freyja\n  * mongoose@icculus.org</small>\n"

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
	//printf("\nFreyjaResource> Bind Resource and C++ function names\n");

	// Mongoose 2002.01.12, Bind script functions to C/C++ functions
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

	//printf("FreyjaResource> Done\n");

	//printf("FreyjaResource> Bind Resource and C++ symbols\n");
	// Mongoose 2002.01.21, Bind some script vars to matching name in C/C++

	r.RegisterInt("eScale", eScale);
	r.RegisterInt("eScale_X", eScale_X);
	r.RegisterInt("eScale_Y", eScale_Y);
	r.RegisterInt("eScale_Z", eScale_Z);

	r.RegisterInt("eZOOM", eZOOM);
	r.RegisterInt("eNONE", eNONE);

	r.RegisterInt("EVENT_NEW_FILE",      EVENT_NEW_FILE);
	r.RegisterInt("EVENT_OPEN_FILE",     EVENT_OPEN_FILE);
	r.RegisterInt("EVENT_SAVE_FILE",     EVENT_SAVE_FILE);
	r.RegisterInt("EVENT_HELP",          EVENT_HELP);



	r.RegisterInt("EVENT_MAIN", EVENT_MAIN);
	r.RegisterInt("CMD_MAIN_SHUTDOWN",      CMD_MAIN_SHUTDOWN);
	r.RegisterInt("CMD_TOGGLE_FULLSCREEN",  CMD_TOGGLE_FULLSCREEN);

	r.RegisterInt("EVENT_POINT", EVENT_POINT);
	r.RegisterInt("CMD_POINT_ADD", CMD_POINT_ADD);
	r.RegisterInt("CMD_POINT_MOVE", CMD_POINT_MOVE);
	r.RegisterInt("CMD_POINT_DELETE", CMD_POINT_DELETE);
	r.RegisterInt("CMD_POINT_COMBINE", CMD_POINT_COMBINE);

	r.RegisterInt("EVENT_POLYGON", EVENT_POLYGON);
	r.RegisterInt("CMD_POLYGON_ADD", CMD_POLYGON_ADD);
	r.RegisterInt("CMD_POLYGON_SELECT", CMD_POLYGON_SELECT);
	r.RegisterInt("CMD_POLYGON_DELETE", CMD_POLYGON_DELETE);	

	r.RegisterInt("EVENT_MESH", EVENT_MESH);
	r.RegisterInt("CMD_MESH_ADD", CMD_MESH_ADD);
	r.RegisterInt("CMD_MESH_DELETE", CMD_MESH_DELETE);
	r.RegisterInt("CMD_MESH_MOVE", CMD_MESH_MOVE);
	r.RegisterInt("CMD_MESH_SELECT", CMD_MESH_SELECT);
	r.RegisterInt("CMD_MESH_ROTATE", CMD_MESH_ROTATE);
	r.RegisterInt("CMD_MESH_SCALE", CMD_MESH_SCALE);
	r.RegisterInt("CMD_MESH_MIRROR", CMD_MESH_MIRROR);
	r.RegisterInt("CMD_MESH_MOVE_CENTER", CMD_MESH_MOVE_CENTER);
	r.RegisterInt("CMD_MESH_GENERATE_CUBE", CMD_MESH_GENERATE_CUBE);
	r.RegisterInt("CMD_MESH_GENERATE_TRIS", CMD_MESH_GENERATE_TRIS);
	r.RegisterInt("CMD_MESH_GENERATE_SPHERE", CMD_MESH_GENERATE_SPHERE);
	r.RegisterInt("CMD_MESH_GENERATE_CYLINDER", CMD_MESH_GENERATE_CYLINDER);

	r.RegisterInt("EVENT_BONE", EVENT_BONE);
	r.RegisterInt("CMD_BONE_NEW", CMD_BONE_NEW);
	r.RegisterInt("CMD_BONE_ADD_MESH", CMD_BONE_ADD_MESH);
	r.RegisterInt("CMD_BONE_SELECT", CMD_BONE_SELECT);
	r.RegisterInt("CMD_BONE_MOVE_PIVOT", CMD_BONE_MOVE_PIVOT);

	r.RegisterInt("EVENT_MISC", EVENT_MISC);
	r.RegisterInt("CMD_MISC_GEN_NORMALS", CMD_MISC_GEN_NORMALS);
	r.RegisterInt("CMD_MISC_ZOOM_IN", CMD_MISC_ZOOM_IN);
	r.RegisterInt("CMD_MISC_ZOOM_OUT", CMD_MISC_ZOOM_OUT);
	r.RegisterInt("CMD_MISC_SCREENSHOT", CMD_MISC_SCREENSHOT);
	r.RegisterInt("CMD_MISC_SELECT", CMD_MISC_SELECT);
	r.RegisterInt("CMD_MISC_BBOX_SELECT", CMD_MISC_BBOX_SELECT);
	r.RegisterInt("CMD_MISC_GEN_TEXMAP_XY", CMD_MISC_GEN_TEXMAP_XY);
	r.RegisterInt("CMD_MISC_VERTEX_UV", CMD_MISC_VERTEX_UV);
	r.RegisterInt("CMD_MISC_DISPLAY_INFO", CMD_MISC_DISPLAY_INFO);
	r.RegisterInt("CMD_MISC_ABOUT", CMD_MISC_ABOUT);
	r.RegisterInt("VIEW_JOINT1", VIEW_JOINT1);
	r.RegisterInt("VIEW_JOINT2", VIEW_JOINT2);
	r.RegisterInt("VIEW_JOINT3", VIEW_JOINT3);
	r.RegisterInt("VIEW_BONE1", VIEW_BONE1);
	r.RegisterInt("VIEW_BONE2", VIEW_BONE2);


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


	//printf("FreyjaResource> Done\n");
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


	mLastEvent = EVENT_MESH;
	mLastCommand = CMD_MESH_SELECT;
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

	freyja_event_notify_observer1f(eZOOM, zoom);

	mRender->setZoom(zoom);
	event_print("Zoom set to %f", mRender->getZoom());
}


float FreyjaControl::getZoom()
{
	return mRender->getZoom();
}


void FreyjaControl::eventMain(int command)
{
	switch (command)
	{
	case CMD_MISC_DISPLAY_INFO:
		mModel->printInfo();
		break;

	case EVENT_HELP:
		freyja_event_info_dialog(HELP_MESSAGE);
		// system("gedit /home/mongoose/Projects/freyja/Freyja/README");
		break; 

	case CMD_MISC_ABOUT:
		freyja_event_info_dialog(ABOUT_MESSAGE);
		break;   


	case CMD_MISC_SCREENSHOT:
		mRender->ScreenShot();
		break;


	case EVENT_NEW_FILE:
		mModel->Clear();
		event_print("Erasing Model...");
		break;


	case EVENT_OPEN_FILE:
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


	case EVENT_SAVE_FILE:
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
			//mFileDialogMode = FREYJA_MODE_SAVE_ANIMATION;
			//freyja_event_file_dialog("Save animation as...");
			break;
		case MATERIAL_EDIT_MODE:
			mFileDialogMode = FREYJA_MODE_SAVE_MATERIAL;
			freyja_event_file_dialog("Save material as...");
			break;
		}
		break;


	case CMD_MAIN_SHUTDOWN:
		freyja_event_exit();
		break;


	case CMD_MAIN_DEBUG_EGG:
		mModel->Debug(!mModel->Debug());
		event_print("Egg debug [%s]", mModel->Debug() ? "ON" : "OFF");
		break;


	case CMD_TOGGLE_FULLSCREEN:
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


	default:
		event_print("!Unhandled mainEvent(%d)", command);
	}	
}


void FreyjaControl::eventBone(int command)
{
	switch (command)
	{
	case CMD_BONE_SELECT:
		event_print("Select bone by pivot");
		_minor_mode = BONE_SELECT_MODE;
		break;
	case CMD_BONE_CONNECT:
		event_print("Select bone to connect to current");
		_minor_mode = BONE_CONNECT_MODE;
		break;
	case CMD_BONE_DISCONNECT:
		event_print("Select bone to break from current");
		_minor_mode = BONE_DISCONNECT_MODE;
		break;
	case CMD_BONE_MOVE_PIVOT:
		event_print("Moving bone pivot mode");
		_minor_mode = TAG_MOVE;
		break;
		
	case CMD_BONE_NEXT:
		mModel->setCurrentBone(mModel->getCurrentBone() + 1);
		break;
	case CMD_BONE_PREV:
		if (mModel->getCurrentBone())
			mModel->setCurrentBone(mModel->getCurrentBone() - 1);
		break;
	case CMD_BONE_NEW:
		mModel->TagNew(0.0, 0.0, 0.0, 0x0);
		event_print("New Bone Tag");
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
	case CMD_BONE_DELETE:
		mModel->TagDel();
		event_print("Deleting Bone Tag %d", mModel->getCurrentBone());
		break;
	default:
		event_print("!Unhandled boneEvent(%d)", command);
	}
}


void FreyjaControl::eventMesh(int command)
{
	switch (command)
	{
	case CMD_MESH_SCALE:
		_minor_mode = MODEL_SCALE;
		event_print("Mesh scale mode");
		break;
	case CMD_MESH_ROTATE:
		_minor_mode = MODEL_ROTATE;
		event_print("Mesh rotate mode");
		break;
	case CMD_MESH_SELECT:
		_minor_mode = MESH_SELECT_MODE;
		event_print("Select mesh by center point");
		break;
	case CMD_MESH_MOVE:
		_minor_mode = MESH_MOVE;
		event_print("Mesh move mode");
		break;
	case CMD_MESH_MOVE_CENTER:
		_minor_mode = MESH_MOVE_CENTER;
		event_print("Reposition mesh center point");
		break;
	case CMD_MESH_GEN_FROM_FRAME:
		event_print("FreyjaControl::Event> %s, %s:%i",
					"CMD_MESH_GEN_FROM_FRAME no longer implemented", 
					__FILE__, __LINE__);
		break;
	case CMD_MESH_FULLCOPY:
		_minor_mode = MESH_WHOLE_COPY_MODE;      
		event_print("FreyjaControl::Event> %s, %s:%i",
					"CMD_MESH_FULLCOPY no longer implemented", 
					__FILE__, __LINE__);
		break;
	case CMD_MESH_COPY:
		_minor_mode = MESH_COPY_MODE;      
		event_print("FreyjaControl::Event> %s, %s:%i",
					"CMD_MESH_COPY no longer implemented", 
					__FILE__, __LINE__);
		break;
	case CMD_MESH_SPLIT:
		_minor_mode = MESH_CUT_MODE;      
		event_print("FreyjaControl::Event> %s, %s:%i",
					"CMD_MESH_SPLIT no longer implemented", 
					__FILE__, __LINE__);
		break;
	case CMD_MESH_MERGE:
		_minor_mode = MESH_PASTE_MODE;      
		event_print("FreyjaControl::Event> %s, %s:%i",
					"CMD_MESH_MERGE no longer implemented", 
					__FILE__, __LINE__);
		break;


	case CMD_MESH_GENERATE_CYLINDER:
		eggGenerateCylinder(4, 16, 8.0, 4.0);
		event_refresh();
		break;
	case CMD_MESH_GENERATE_SPHERE:
		eggGenerateSphere(16, 16, 8.0);
		event_refresh();
		break;
	case CMD_MESH_GENERATE_CUBE:
		eggGenerateCube(8.0);
		event_refresh();
		break;
	case CMD_MESH_GENERATE_TRIS:
		eggGenerateTriangleStrip(10, 8.0);
		event_refresh();
		break;

	case CMD_MESH_NEXT:
		mModel->setCurrentMesh(mModel->getCurrentMesh() + 1);
		break;
	case CMD_MESH_PREV:
		if (mModel->getCurrentMesh())
			mModel->setCurrentMesh(mModel->getCurrentMesh() - 1);
		break;
	case CMD_MESH_MIRROR:
		mModel->MeshMirror(mModel->getCurrentMesh());
		break;
	case CMD_MESH_DUP_FRAME:
		mModel->MeshFrameClone(mModel->getCurrentMesh(), mModel->getCurrentGroup());
		break;
	case CMD_MESH_DUP_SCENE:
		mModel->GroupClone(mModel->getCurrentGroup());
		break;
	case CMD_MESH_ADD:
		mModel->MeshNew();
		break;
	case CMD_MESH_DELETE:
		mModel->MeshDel();
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


extern unsigned int gBoneRenderType;
extern unsigned char gJointRenderType;
void FreyjaControl::eventMisc(int command)
{
	switch (command)
	{
	case VIEW_JOINT1:
		gJointRenderType = 1;
		break;
	case VIEW_JOINT2:
		gJointRenderType = 2;
		break;
	case VIEW_JOINT3:
		gJointRenderType = 3;
		break;

	case VIEW_BONE1:
		gBoneRenderType = 1;
		break;
	case VIEW_BONE2:
		gBoneRenderType = 2;
		break;

	case 1000:
		mModel->Transform(FreyjaModel::TransformMesh, Egg::SCALE,
						  freyja_event_get_float(1001),
						  freyja_event_get_float(1002),
						  freyja_event_get_float(1003));
		event_refresh();
		
		freyja_event_set_float(1001, 1.0f);
		freyja_event_set_float(1002, 1.0f);
		freyja_event_set_float(1003, 1.0f);
		break;

	case CMD_MISC_ABOUT:
		freyja_event_info_dialog(ABOUT_MESSAGE);
		break;   

	case CMD_MISC_SCREENSHOT:
		mRender->ScreenShot();
		break;

	case CMD_MISC_GEN_NORMALS:
		eggGenerateVertexNormals();
		break;
	case CMD_MISC_RENDER_BBOX:
		mRender->Flags(FreyjaRender::RENDER_BBOX, 
					   !(mRender->Flags() & FreyjaRender::RENDER_BBOX));
		event_print("BoundingBox rendering [%s]", 
					(mRender->Flags() & FreyjaRender::RENDER_BBOX) ? 
					"ON" : "OFF");
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

	case CMD_MISC_TEX_SLOT_LOAD:
		event_set_load_texture_to_slot(!query_load_texture_to_slot());
		event_print("Texture loading to current slot [%s]",
					query_load_texture_to_slot() ? "on" : "off");
		break;
	case CMD_MISC_SCENE_ROTATE:
		_minor_mode = MODEL_ROTATE;
		event_print("Scene rotate mode");
		break;
	case CMD_MISC_SELECT:

 		switch (_major_mode)
 		{
 		case ANIMATION_EDIT_MODE:
 			event_print("Select tag by center point");
 			_minor_mode = BONE_SELECT_MODE;
 			break;
 		case MODEL_EDIT_MODE:
 			event_print("Select mesh by center point");
 			_minor_mode = MESH_SELECT_MODE;
 			break;
		default:
			event_print("WARNING: Selection undefined for this mode");
 		}

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



		case CMD_MISC_TEXTURE_SET:
			event_print("Switching all of mesh %i ploygons to texture %i",
						mModel->getCurrentMesh(), mModel->getCurrentTextureIndex());
			mModel->TextureShift();
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
		case CMD_POINT_MOVE:
			_minor_mode = VERTEX_MOVE;
			event_print("Select vertex to move");
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
					(type == 3) ? "Emitter" :
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
		if (mMouseButton & MOUSE_BTN_RIGHT)
		{
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
		if (mod & KEY_RCTRL)
		{
			handleEvent(EVENT_MISC, CMD_MISC_SELECT);
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


void FreyjaControl::MotionEdit(int x, int y, Egg::egg_plane plane)
{
	static int old_y = 0, old_x = 0;
	float xx = x, yy = y;
  

	if (mMouseButton == MOUSE_BTN_MIDDLE)
	{
		float xyz[3];

		if (x > old_x)
			xyz[0] = 1.0f;
		else if (x < old_x)
			xyz[0] = -1.0f;

		if (y < old_y)
			xyz[1] = 1.0f;
		else if (y > old_y)
			xyz[1] = -1.0f;

		xyz[2] = 0.0f;	

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
		return;
	}

	// Mongoose: Convert screen to world system
	ScreenToWorld(&xx, &yy);


	// Mongoose: No click and release
	if (mMouseButton != MOUSE_BTN_LEFT)
	{
		return;
	}

	//event_print("* motion -> %i, %i : %i", x, y, mMouseButton);

	switch (_minor_mode)
	{
	case MODEL_ROTATE: 

		event_print("DEBUG MotionEdit, MODEL_ROTATE: %f %f", x, y);

		if (x > old_x)
		{
			switch (plane)
			{
			case Egg::PLANE_XY:
				mModel->Transform(FreyjaModel::TransformMesh, 
								  Egg::ROTATE, 0.0, -0.5, 0.0);
				break;
			case Egg::PLANE_XZ:
				mModel->Transform(FreyjaModel::TransformMesh, 
								  Egg::ROTATE, 0.0, 0.0, -0.5);
				break;
			case Egg::PLANE_YZ:
				mModel->Transform(FreyjaModel::TransformMesh, 
								  Egg::ROTATE, 0.0, 0.0, -0.5);
				break;
			}
		}
		else if (x < old_x)
		{
			switch (plane)
			{
			case Egg::PLANE_XY:
				mModel->Transform(FreyjaModel::TransformMesh, 
								  Egg::ROTATE, 0.0, 0.5, 0.0);
				break;
			case Egg::PLANE_XZ:
				mModel->Transform(FreyjaModel::TransformMesh, 
								  Egg::ROTATE, 0.0, 0.0, 0.5);
				break;
			case Egg::PLANE_YZ:
				mModel->Transform(FreyjaModel::TransformMesh, 
								  Egg::ROTATE, 0.0, 0.0, 0.5);
			}
		}


		if (y > old_y)
		{
			switch (plane)
			{
			case Egg::PLANE_XY:
				mModel->Transform(FreyjaModel::TransformMesh, 
								  Egg::ROTATE, -0.5, 0.0, 0.0);
				break;
			case Egg::PLANE_XZ:
				mModel->Transform(FreyjaModel::TransformMesh, 
								  Egg::ROTATE, -0.5, 0.0, 0.0);
				break;
			case Egg::PLANE_YZ:
				mModel->Transform(FreyjaModel::TransformMesh, 
								  Egg::ROTATE, 0.0, -0.5, 0.0);
				break;
			}
		}
		else if (y < old_y)
		{
			switch (plane)
			{
			case Egg::PLANE_XY:
				mModel->Transform(FreyjaModel::TransformMesh, 
								  Egg::ROTATE, 0.5, 0.0, 0.0);
				break;
			case Egg::PLANE_XZ:
				mModel->Transform(FreyjaModel::TransformMesh, 
								  Egg::ROTATE, 0.5, 0.0, 0.0);
				break;
			case Egg::PLANE_YZ:
				mModel->Transform(FreyjaModel::TransformMesh, 
								  Egg::ROTATE, 0.0, 0.5, 0.0);
			}

		}

		old_x = x;
		old_y = y;
		break;

		case MODEL_SCALE:
			if (y > old_y)
			{
				switch (plane)
				{
				case Egg::PLANE_XY:
					mModel->Transform(FreyjaModel::TransformMesh, 
									  Egg::SCALE, 1.0, 0.99, 1.0);
					break;
				case Egg::PLANE_XZ:
					mModel->Transform(FreyjaModel::TransformMesh, 
									  Egg::SCALE, 1.0, 1.0, 0.99);
					break;
				case Egg::PLANE_YZ:
					mModel->Transform(FreyjaModel::TransformMesh, 
									  Egg::SCALE, 1.0, 1.0, 0.99);
				break;
				}
			}
			else if (y < old_y)
			{
				switch (plane)
				{
				case Egg::PLANE_XY:
					mModel->Transform(FreyjaModel::TransformMesh, 
									  Egg::SCALE, 1.0, 1.01, 1.0);
					break;
				case Egg::PLANE_XZ:
					mModel->Transform(FreyjaModel::TransformMesh, 
									  Egg::SCALE, 1.0, 1.0, 1.01);
					break;
				case Egg::PLANE_YZ:
					mModel->Transform(FreyjaModel::TransformMesh, 
									  Egg::SCALE, 1.0, 1.0, 1.01);
				}
			}
			
			if (x < old_x)
			{
				switch (plane)
				{
				case Egg::PLANE_XY:
					mModel->Transform(FreyjaModel::TransformMesh, 
									  Egg::SCALE, 0.99, 1.0, 1.0);
					break;
				case Egg::PLANE_XZ:
					mModel->Transform(FreyjaModel::TransformMesh, 
									  Egg::SCALE, 0.99, 1.0, 1.0);
					break;
				case Egg::PLANE_YZ:
					mModel->Transform(FreyjaModel::TransformMesh, 
									  Egg::SCALE, 1.0, 0.99, 1.0);
					break;
				}
			}
			else if (x > old_x)
			{
				switch (plane)
				{
				case Egg::PLANE_XY:
					mModel->Transform(FreyjaModel::TransformMesh, 
									  Egg::SCALE, 1.01, 1.0, 1.0);
					break;
				case Egg::PLANE_XZ:
					mModel->Transform(FreyjaModel::TransformMesh, 
									  Egg::SCALE, 1.01, 1.0, 1.0);
					break;
				case Egg::PLANE_YZ:
					mModel->Transform(FreyjaModel::TransformMesh, 
									  Egg::SCALE, 1.0, 1.01, 1.0);
				}
			}
				
			old_x = x;
			old_y = y;
			break;

  case VERTEX_BBOX_SELECT_MODE:
    mModel->BBoxMove(xx, yy);
    break;
  case TAG_MOVE:
    mModel->TagMove(xx, yy);
    break;
  case TAG_MOVE_CENTER:
    mModel->TagMoveCenter(xx, yy);
    break;
  case MESH_MOVE:
    mModel->MeshMove(xx, yy);
    break;
  case MESH_MOVE_CENTER: 
    mModel->MeshMoveCenter(xx, yy);
    break;
  case VERTEX_MOVE:
    mModel->VertexMove(xx, yy);
    break;
  default:
    break;
  }
}


void FreyjaControl::MouseEdit(int btn, int state, int mod, int x, int y, 
										Egg::egg_plane plane) 
{ 
	float xx = x, yy = y;
	unsigned int master_tag;
	
	
	ScreenToWorld(&xx, &yy);
	
	if (!(btn == MOUSE_BTN_LEFT && state == MOUSE_BTN_STATE_PRESSED))
	{
		return;
	}

	switch(_minor_mode)
	{
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
	case VERTEX_MOVE:
	case MESH_MOVE:
		if (_mouse_state == 0)
		{
			mModel->VertexSelect(xx, yy);
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
	case MESH_SELECT_MODE:
		mModel->MeshSelect(xx, yy);
		break;
	case POINT_DEL_MODE:
		mModel->VertexSelect(xx, yy);
		mModel->VertexDelete();
		break;
	case BONE_SELECT_MODE:
		mModel->TagSelect(xx, yy);
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
	case TAG_MOVE:
		if (_mouse_state == 0)
			_mouse_state = 1;
		else
			_mouse_state = 0;
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
	case MESH_WHOLE_COPY_MODE:
		break;
	case MESH_PASTE_MODE:
		event_print("Mesh paste not impelmented\n");
		break;
	case MESH_COPY_MODE:
	case MESH_CUT_MODE:
		mModel->VertexBuffer(xx, yy);
	default:
		;
	}
}

