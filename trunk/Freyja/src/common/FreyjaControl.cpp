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

#include <stdlib.h> 
#include <stdio.h> 
#include <math.h> 
#include <stdarg.h> 
#include <string.h>

#include "FreyjaControl.h"


void event_register_render(FreyjaRender *r);
void event_register_model(FreyjaModel *m);
void event_register_control(FreyjaControl *c);



FreyjaEvent::FreyjaEvent()
{
	_func_event = NULL;

	_pair_active = false;
	_pair_event[0] = 0;
	_pair_event[1] = 0;
}


FreyjaEvent::~FreyjaEvent()
{
}


void FreyjaEvent::AddEvent(int e, int c)
{
	_pair_event[0] = e;
	_pair_event[1] = c;
	_pair_active = true;
}

	
void FreyjaEvent::Emit()
{
	if (_func_event)
	{
		//(*_func_event)();
	}

	if (_pair_active)
	{
		freyja_event2i(_pair_event[0], _pair_event[1]);
	}
}


////////////////////////////////////////////////////////////////


FreyjaControl::FreyjaControl()
{
	_model = new FreyjaModel();
	_render = new FreyjaRender();
	_render->Register(_model);

	if (EGG_LIB_VERSION != 0x32312E38)
	{
		printf("FreyjaControl::FreyjaControl> Assertion failure\n");
		printf("*** You must upgrade libfreyja_egg to v8.12\n");
		exit(-1);
	}

	if (!_model || !_render)
	{
		printf("FreyjaControl::FreyjaControl> Assertion failure\n");
		exit(-1);
	}
	
	_minor_mode = NO_MODE;
	_major_mode = MODEL_EDIT_MODE;
	
	event_register_render(_render);
	event_register_model(_model);
	event_register_control(this);

	mFullScreen = false;

	ReadRC();
}


FreyjaControl::~FreyjaControl()
{
}


void FreyjaControl::eventMain(int command)
{
	switch (command)
	{
	case CMD_MAIN_DEBUG_EGG:
		_model->Debug(!_model->Debug());
		event_print("Egg debug [%s]", _model->Debug() ? "ON" : "OFF");
		break;


	case CMD_MAIN_NEW_MODEL:
		_model->Clear();
		event_print("Erasing Model...");
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
		event_print("Unhandled mainEvent(%d)", command);
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
		_model->setCurrentBone(_model->getCurrentBone() + 1);
		break;
	case CMD_BONE_PREV:
		if (_model->getCurrentBone())
			_model->setCurrentBone(_model->getCurrentBone() - 1);
		break;
	case CMD_BONE_NEW:
		_model->TagNew(0.0, 0.0, 0.0, 0x0);
		event_print("New Bone Tag");
		break;
	case CMD_BONE_ADD_MESH:
		_model->TagAddMesh(_model->getCurrentBone(), _model->getCurrentMesh());
		event_print("New Bone[%i] now contains mesh %i",
						_model->getCurrentBone(), _model->getCurrentMesh());
		break;
	case CMD_BONE_DELETE_MESH:
		_model->TagDelMesh(_model->getCurrentBone(), _model->getCurrentMesh());
		event_print("New Bone[%i] no longer contains mesh %i",
						_model->getCurrentBone(), _model->getCurrentMesh());
		break;
	case CMD_BONE_DELETE:
		_model->TagDel();
		event_print("Deleting Bone Tag %d", _model->getCurrentBone());
		break;
	default:
		event_print("Unhandled boneEvent(%d)", command);
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


	case CMD_MESH_GENERATE_CAN:
		eggGenerateCylinder(4, 16, 64.0, 32.0);
		event_refresh();
		break;
	case CMD_MESH_GENERATE_SPHERE:
		eggGenerateSphere(16, 16, 64.0);
		event_refresh();
		break;
	case CMD_MESH_GENERATE_CUBE:
		eggGenerateCube(64.0);
		event_refresh();
		break;
	case CMD_MESH_GENERATE_TRIS:
		eggGenerateTriangleStrip(10, 64.0);
		event_refresh();
		break;
// 	case CMD_MESH_CLONE:
// 		if (1)
// 		{
// 			egg_mesh_t *mesh = _model->CachedMesh();
// 			egg_group_t *grp;
			
			
// 			if (mesh && (grp = _egg->getGroup(mesh->group[0])))
// 			{
// 				mList.copy(grp->vertex);
				
// 				if (!mList.empty())
// 				{
// 					event_print("Mesh cloned");
// 					_egg->addMesh(_egg->MeshCopy(mesh, &mList));
// 				}
// 			}
// 		}
// 		break;
//	case CMD_MESH_ROTATE:
//		_edit_mode = FreyjaModel::TransformMesh;
//		break;
//	case CMD_MESH_COPY2:
//		MeshCopy();
//		break;
//	case CMD_MESH_CUT:
//		event_print("Mesh partially cut (split)");
//		MeshCut();
//		break;
//	case CMD_MESH_PASTE:
//		_egg->AddMesh(_egg->MeshMerge(_egg->MeshNew(), _list, CachedMesh()));
//		_list.Clear();
//		break;
	case CMD_MESH_NEXT:
		_model->setCurrentMesh(_model->getCurrentMesh() + 1);
		break;
	case CMD_MESH_PREV:
		if (_model->getCurrentMesh())
			_model->setCurrentMesh(_model->getCurrentMesh() - 1);
		break;
	case CMD_MESH_MIRROR:
		_model->MeshMirror(_model->getCurrentMesh());
		break;
	case CMD_MESH_DUP_FRAME:
		_model->MeshFrameClone(_model->getCurrentMesh(), _model->getCurrentGroup());
		break;
	case CMD_MESH_DUP_SCENE:
		_model->GroupClone(_model->getCurrentGroup());
		break;
	case CMD_MESH_ADD:
		_model->MeshNew();
		break;
	case CMD_MESH_DELETE:
		_model->MeshDel();
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
		_model->setCurrentAnimation(_model->getCurrentAnimation() + 1);
		break;


	case CMD_ANIMATION_PREV:
		if (_model->getCurrentAnimation())
			_model->setCurrentAnimation(_model->getCurrentAnimation() - 1);
		else
			_model->setCurrentAnimation(0);
		break;


	default:
		event_print("Unhandled animationEvent(%d)", command);
	}

}


void FreyjaControl::eventMisc(int command)
{
	switch (command)
	{    
	case CMD_MISC_GEN_NORMALS:
		eggGenerateVertexNormals();
		break;
	case CMD_MISC_RENDER_BBOX:
		_render->Flags(FreyjaRender::RENDER_BBOX, 
					   !(_render->Flags() & FreyjaRender::RENDER_BBOX));
		event_print("BoundingBox rendering [%s]", 
					(_render->Flags() & FreyjaRender::RENDER_BBOX) ? 
					"ON" : "OFF");
		break;
	case CMD_MISC_RENDER_ROT_Z_M:
		_render->Rotate(Z_F, -_render->RotateAmount());
		break;
	case CMD_MISC_RENDER_ROT_Z_P:
		_render->Rotate(Z_F, _render->RotateAmount());
		break;
	case CMD_MISC_RENDER_ROT_Y_M:
		_render->Rotate(Y_F, -_render->RotateAmount());
		break;
	case CMD_MISC_RENDER_ROT_Y_P:
		_render->Rotate(Y_F, _render->RotateAmount());
		break;
	case CMD_MISC_RENDER_ROT_X_M:
		_render->Rotate(X_F, -_render->RotateAmount());
		break;
	case CMD_MISC_RENDER_ROT_X_P:
		_render->Rotate(X_F, _render->RotateAmount());
		break;
	case CMD_MISC_CAMMODE:
		_render->Flags(FreyjaRender::RENDER_CAMERA, 
					   !(_render->Flags() & FreyjaRender::RENDER_CAMERA));
		break;  
	case CMD_MISC_SCREENSHOT:
		_render->ScreenShot();
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
		event_print("eventMisc> Pure soft ABI CMD_MISC_SELECT (generic) disabled");
// 		switch (last_event)
// 		{
// 		case EVENT_BONE:
// 			event_print("Select tag by center point");
// 			_minor_mode = BONE_SELECT_MODE;
// 			break;
// 		case EVENT_MESH:
// 			event_print("Select mesh by center point");
// 			_minor_mode = MESH_SELECT_MODE;
// 			break;
// 		}
		break;
	case CMD_MISC_BBOX_SELECT:
		if (_minor_mode == VERTEX_BBOX_SELECT_MODE)
		{
			_minor_mode = NO_MODE;
			_render->Flags(FreyjaRender::RENDER_BBOX_SEL, 0);
			_model->BBoxListBuild();
			event_print("Vertex box select: Vertex list built");
		}
		else
		{
			_minor_mode = VERTEX_BBOX_SELECT_MODE;
			_render->Flags(FreyjaRender::RENDER_BBOX_SEL, 1);
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





		case CMD_MISC_DISPLAY_INFO:
			_model->printInfo();
			break;
// 		case CMD_MISC_LOAD_MAP:
// 			_model->setFlags(FL_LOAD_MAP, !(_model->getFlags() & FL_LOAD_MAP));
// 			break
			;
// 		case CMD_MISC_EDITMODE:
// 			_edit_mode++;
     
// 			if (_edit_mode == 3)
// 				_edit_mode = 0;
     
// 			switch (_edit_mode)
// 			{
// 			case FreyjaModel::TransformMesh:
// 				event_print("Edit mode set to MESH");
// 				break;
// 			case FreyjaModel::TransformVertexFrame:
// 				event_print("Edit mode set to FRAME");
// 				break;
// 			case FreyjaModel::TransformScene:
// 				event_print("Edit mode set to SCENE");
// 				break;	   
// 			}
// 			break;
// 		case CMD_MISC_SCENE_ROTATE:
// 			_edit_mode = FreyjaModel::TransformScene;
// 			break;
		case CMD_MISC_TEXTURE_SET:
			event_print("Switching all of mesh %i ploygons to texture %i",
						_model->getCurrentMesh(), _model->getCurrentTextureIndex());
			_model->TextureShift();
			break;
		case CMD_MISC_GEN_TEXMAP_XY:
			_model->generateUVMap();
			break;
// 		case CMD_MISC_VERTEX_UV:
// 			_model->setFlags(FL_VERTEX_UV, !(_model->getFlags() & FL_VERTEX_UV));
// 			event_print("Vertex with UV texels [%s]", 
// 						(_model->getFlags() & FL_VERTEX_UV) ? "on" : "off");
// 			break;
		case CMD_MISC_TEXTURE_NEXT:
			_model->setCurrentTextureIndex(_model->getCurrentTextureIndex() + 1);
			event_print("Texture[%i] in edit mode", _model->getCurrentTextureIndex());
			break;
		case CMD_MISC_TEXTURE_PREV:
			if (_model->getCurrentTextureIndex())
			{
				_model->setCurrentTextureIndex(_model->getCurrentTextureIndex() - 1);
			}

			event_print("Texture[%i] in edit mode", _model->getCurrentTextureIndex());
			break;
		case CMD_MISC_SIDES_M:
			_model->setCurrentPolygonEdgeCount(_model->getCurrentPolygonEdgeCount()-1);
			event_print("Making %i sided polygons", 
						_model->getCurrentPolygonEdgeCount());
			break;
		case CMD_MISC_SIDES_P:
			_model->setCurrentPolygonEdgeCount(_model->getCurrentPolygonEdgeCount()+1);
			event_print("Making %i sided polygons", 
						_model->getCurrentPolygonEdgeCount());
			break;
// 		case CMD_MISC_DUMPTEXTURE:
// 			_model->setFlags(FL_DUMP_TEXTURE, !(_model->getFlags() & FL_DUMP_TEXTURE));
// 			break;
		case CMD_MISC_ZOOM_IN:
			if (_model->getZoom() <= 0.02)
			{
				_model->setZoom(_model->getZoom() + 0.0001);
			}
			else
			{
				_model->setZoom(_model->getZoom() + 0.01);
			}

			event_refresh();
			break;
		case CMD_MISC_ZOOM_OUT:
			if (_model->getZoom() <= 0.02)
			{
				_model->setZoom(_model->getZoom() - 0.0001);
			}
			else
			{
				_model->setZoom(_model->getZoom() - 0.01);
			}

			event_refresh();
			break;
		case CMD_MISC_SCROLL_UP_X:
			_model->adjustSceneTranslation(1.0, 0.0, 0.0);
			break;
		case CMD_MISC_SCROLL_DOWN_X:
			_model->adjustSceneTranslation(-1.0, 0.0, 0.0);
			break;
		case CMD_MISC_SCROLL_UP_Y:
			_model->adjustSceneTranslation(0.0, 1.0, 0.0);
			break;
		case CMD_MISC_SCROLL_DOWN_Y:
			_model->adjustSceneTranslation(0.0, -1.0, 0.0);
			break;
		case CMD_MISC_SCROLL_UP_Z:
			_model->adjustSceneTranslation(0.0, 0.0, 1.0);
			break;
		case CMD_MISC_SCROLL_DOWN_Z:
			_model->adjustSceneTranslation(0.0, 0.0, -1.0);
			break;
		case CMD_MISC_SCROLL_UP:
			switch (_model->CurrentPlane())
			{
			case Egg::PLANE_XY:
				eventMisc(CMD_MISC_SCROLL_UP_Y);
				break;
			case Egg::PLANE_XZ:
				eventMisc(CMD_MISC_SCROLL_UP_Z);
				break;
			case Egg::PLANE_YZ:
				eventMisc(CMD_MISC_SCROLL_UP_Z);
				break;
			}
			break;
		case CMD_MISC_SCROLL_DOWN:
			switch (_model->CurrentPlane())
			{
			case Egg::PLANE_XY:
				eventMisc(CMD_MISC_SCROLL_DOWN_Y);
				break;
			case Egg::PLANE_XZ:
				eventMisc(CMD_MISC_SCROLL_DOWN_Z);
				break;
			case Egg::PLANE_YZ:
				eventMisc(CMD_MISC_SCROLL_DOWN_Z);
				break;
			}
			break;
		case CMD_MISC_SCROLL_RIGHT:
			switch (_model->CurrentPlane())
			{
			case Egg::PLANE_XY:
				eventMisc(CMD_MISC_SCROLL_DOWN_X);
				break;
			case Egg::PLANE_XZ:
				eventMisc(CMD_MISC_SCROLL_DOWN_X);
				break;
			case Egg::PLANE_YZ:
				eventMisc(CMD_MISC_SCROLL_DOWN_Y);
				break;
			}
			break;
		case CMD_MISC_SCROLL_LEFT:
			switch (_model->CurrentPlane())
			{
			case Egg::PLANE_XY:
				eventMisc(CMD_MISC_SCROLL_UP_X);
				break;
			case Egg::PLANE_XZ:
				eventMisc(CMD_MISC_SCROLL_UP_X);
				break;
			case Egg::PLANE_YZ:
				eventMisc(CMD_MISC_SCROLL_UP_Y);
				break;
			}
			break;
	default:
		event_print("Unhandled eventMisc(%d)", command);
	}   
}


void FreyjaControl::Event(int mode, int cmd)
{
	static int last_event = EVENT_MESH;
	static int last_cmd = CMD_MESH_SELECT;
	
	
	if (mode == EVENT_REDO_LAST)
	{
		mode = last_event;
		cmd = last_cmd;
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
			_render->Flags(FreyjaRender::RENDER_BONES, 
								!(_render->Flags() & FreyjaRender::RENDER_BONES));
			event_print("Bone Rendering [%s]", 
							(_render->Flags() & FreyjaRender::RENDER_BONES) ? 
							"ON" : "OFF");
			event_refresh();
			break;
		case FREYJA_MODE_RENDER_POINTS:
			_render->Flags(FreyjaRender::RENDER_POINTS, 
								!(_render->Flags() & FreyjaRender::RENDER_POINTS));
			event_print("Point Rendering [%s]", 
							(_render->Flags() & FreyjaRender::RENDER_POINTS) ? 
							"ON" : "OFF");
			event_refresh();
			break;
		case FREYJA_MODE_RENDER_WIREFRAME:
			_render->Flags(FreyjaRender::RENDER_WIREFRAME, 
								!(_render->Flags() & FreyjaRender::RENDER_WIREFRAME));
			event_print("Wireframe Rendering [%s]", 
							(_render->Flags() & FreyjaRender::RENDER_WIREFRAME) ? 
							"ON" : "OFF");
			event_refresh();
			break;
		case FREYJA_MODE_RENDER_FACE:
			_render->Flags(FreyjaRender::RENDER_FACE, 
								!(_render->Flags() & FreyjaRender::RENDER_FACE));
			event_print("Face Rendering [%s]", 
							(_render->Flags() & FreyjaRender::RENDER_FACE) ? 
							"ON" : "OFF");
			event_refresh();
			break;
		case FREYJA_MODE_RENDER_NORMALS:
			_render->Flags(FreyjaRender::RENDER_NORMALS, 
								!(_render->Flags() & FreyjaRender::RENDER_NORMALS));
			event_print("Normal Rendering [%s]", 
							(_render->Flags() & FreyjaRender::RENDER_NORMALS) ? 
							"ON" : "OFF");
			event_refresh();
			break;
		case FREYJA_MODE_RENDER_LIGHTING:
			_render->Flags(FreyjaRender::RENDER_NORMAL, 
								!(_render->Flags() & FreyjaRender::RENDER_NORMAL));
			_render->Flags(FreyjaRender::RENDER_LIGHTING, 
								!(_render->Flags() & FreyjaRender::RENDER_LIGHTING));
			event_print("GL Lighting is [%s]", 
							(_render->Flags() & FreyjaRender::RENDER_LIGHTING) ? 
							"ON" : "OFF");
			event_refresh();
			break;
		case FREYJA_MODE_RENDER_TEXTURE:
			_render->Flags(FreyjaRender::RENDER_TEXTURE, 
								!(_render->Flags() & FreyjaRender::RENDER_TEXTURE));
			event_print("Texture rendering is [%s]", 
							(_render->Flags() & FreyjaRender::RENDER_TEXTURE) ? 
							"ON" : "OFF");
			event_refresh();
			break;
		case FREYJA_MODE_RENDER_MATERIAL:
			_render->Flags(FreyjaRender::RENDER_MATERIAL, 
								!(_render->Flags() & FreyjaRender::RENDER_MATERIAL));
			event_print("Material rendering is [%s]", 
							(_render->Flags() & FreyjaRender::RENDER_MATERIAL) ? 
							"ON" : "OFF");
			event_refresh();
			break;
		case FREYJA_MODE_RENDER_GRID:
			_render->Flags(FreyjaRender::RENDER_EDIT_GRID, 
								!(_render->Flags() & FreyjaRender::RENDER_EDIT_GRID));
			event_print("Edit Grid rendering [%s]",
						(_render->Flags() & FreyjaRender::RENDER_EDIT_GRID) ? 
							"ON" : "OFF");
			event_refresh();
			break;

		case FREYJA_MODE_TEXTURE_EDIT:
			_render->ViewMode(VIEWMODE_TEXTURE_EDIT);
			event_refresh();
			event_print("Texture Edit");
			_major_mode = TEXTURE_EDIT_MODE;
			event_set_mouse_active(true);
			break;
		case FREYJA_MODE_MODEL_EDIT:
			_render->ViewMode(VIEWMODE_MODEL_EDIT);
			event_refresh();
			event_print("Model Edit");
			_major_mode = MODEL_EDIT_MODE;
			event_set_mouse_active(true);
			break;
		case FREYJA_MODE_MODEL_VIEW:
			_render->ViewMode(VIEWMODE_MODEL_VIEW);
			event_refresh();
			event_print("Animation View");
			_major_mode = MODEL_VIEW_MODE;
			event_set_mouse_active(true);
			break;
		case FREYJA_MODE_MATERIAL_EDIT:
			_render->ViewMode(VIEWMODE_MATERIAL_EDIT);
			event_refresh();
			event_print("Material Edit");
			_major_mode = MATERIAL_EDIT_MODE;
			event_set_mouse_active(false);
			break;
		case FREYJA_MODE_PLANE_XY:
			event_print("Plane XY");      
			_model->CurrentPlane(Egg::PLANE_XY);
			event_refresh();
			break;
		case FREYJA_MODE_PLANE_XZ:
			event_print("Plane XZ");      
			_model->CurrentPlane(Egg::PLANE_XZ);
			event_refresh();
			break;
		case FREYJA_MODE_PLANE_YZ:
			event_print("Plane YZ");      
			_model->CurrentPlane(Egg::PLANE_YZ);
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

	}

	last_event = mode;
	last_cmd = cmd;
}


void FreyjaControl::ReadRC()
{
	int i;
	float f;
	char *s;
	char *rc[3];
	char *rc0 = "freyja.mlisp";
	char *rc1 = "freyja.el";
	char *rc2 = "freyja.lisp";
	bool failed = true;


	rc[0] = rc0;
	rc[1] = rc1;
	rc[2] = rc2;

	for (i = 0; i < 3 && failed; ++i)
	{
		s = freyja_rc_map(rc[i]);

		if (!s)
		{
			continue;
		}

		if (_resource.Load(s))
		{
			continue;
		}
		else
		{
			failed = false;
		}

		delete [] s;
	}

	if (failed)
	{
		printf("FreyjaControl::ReadRC> ERROR: Couldn't find '%s'\n", s);
		event_shutdown();
	}

	int x, y;
	if (_resource.Lookup("WINDOW_X", &x))
	{
		if (_resource.Lookup("WINDOW_Y", &y))
		{
			extern void application_window_move(int x, int y);
			application_window_move(x, y);
		}
	}

	if (_resource.Lookup("DUMP_SYMTAB", &i) && i)
	{
		_resource.Print();
	}

	if (_resource.Lookup("GRID_ON", &i) && i)
	{
		_render->Flags(FreyjaRender::RENDER_EDIT_GRID, 
							!(_render->Flags() & FreyjaRender::RENDER_EDIT_GRID));
	}

	if (_resource.Lookup("ROTATE_AMOUNT", &f))
	{
		_render->RotateAmount(f);
	}

	if (_resource.Lookup("FLUSH_RESOURCE", &i) && i)
	{
		_resource.Flush();
	}
}


void FreyjaControl::ScreenToWorld(float *x, float *y)
{
	float width, height, invz, fs;
	float scroll[3];
 

	width = _render->Width();
	height = _render->Height();
	_model->getSceneTranslation(scroll);

#ifdef DEBUG_SCREEN_TO_WORLD
	printf("Screen (%.3f, %.3f); aspect_ratio = %.3f; zoom = %.3f\n", 
			 *x, *y, width/height, _model->getZoom());
	printf("Scroll (%.3f, %.3f, %.3f)\n", 
			 scroll[0], scroll[1], scroll[2]);
#endif

	invz = (1.0 / _model->getZoom());
	fs = (40.0 * invz) / height;  // fov 40?

	*x = (*x - width / 2.0) * fs;
	*y = -(*y - height / 2.0) * fs;
	
	switch (_model->CurrentPlane())
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

int BTN_HACK;
bool FreyjaControl::Motion(int x, int y)
{
	static int old_y = 0, old_x = 0;

	
	switch (_major_mode)
	{
	case MODEL_VIEW_MODE:
		if (BTN_HACK & MOUSE_BTN_RIGHT)
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
		else if (0) // don't edit mesh in animation view 
		{
			MotionEdit(x, y, _model->CurrentPlane());
		}
    break;
  case TEXTURE_EDIT_MODE:
	  if (_tex_state)
	  {
		  float s;
		  float t;

		  s = (float)x / (float)_render->Width();
		  t = (float)y / (float)_render->Height();
		  
		  if (s > 1.0) s = 1.0;
		  if (s < 0.0) s = 0.0;
		  if (t > 1.0) t = 1.0;
		  if (t < 0.0) t = 0.0;
		  
		  _model->TexelMove(s, t);
		  return true;
	  }
    break;
  case MODEL_EDIT_MODE:
    MotionEdit(x, y, _model->CurrentPlane());
    break;
  default:
    ;
  }

  return true;
}


bool FreyjaControl::Mouse(int btn, int state, int mod,  int x, int y)
{
	freyja_control_mode_t mode = _minor_mode;

	BTN_HACK = btn;
	// Mongoose 2002.01.12, Allow temp mode override
	if (mod & KEY_RCTRL)
	{
		Event(EVENT_MISC, CMD_MISC_SELECT);
	}

	switch (_major_mode)
	{
	case MODEL_VIEW_MODE:
		if (btn == MOUSE_BTN_4 && state == MOUSE_BTN_STATE_PRESSED)
		{
			freyja_event2i(EVENT_MISC, CMD_MISC_ZOOM_IN);
			event_print("Zoom in");
		}
		
		if (btn == MOUSE_BTN_5 && state == MOUSE_BTN_STATE_PRESSED)
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
			
			
			s = (float)x / (float)_render->Width();
			t = (float)y / (float)_render->Height();
			
			// Mongoose: Clamp texels to be bound by min and max
			if (s > 1.0) s = 1.0;
			if (s < 0.0) s = 0.0;
			if (t > 1.0) t = 1.0;
			if (t < 0.0) t = 0.0;
			
			if (_minor_mode == TEXEL_COMBINE)
			{
				_model->TexelCombine(s, t);
			}
			else
			{
				_model->TexelSelect(s, t);
			}
			
			_tex_state = true;
			return true;
		}
		break;
	case MODEL_EDIT_MODE:
		MouseEdit(btn, state, mod, x, y, _model->CurrentPlane());
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
  
	
	// Mongoose: Convert screen to world system
	ScreenToWorld(&xx, &yy);

	// Mongoose: No click and release
	if (_mouse_state != 1)
	{
		return;
	}

	switch (_minor_mode)
	{
	case MODEL_ROTATE: 

		if (x > old_x)
		{
			switch (plane)
			{
			case Egg::PLANE_XY:
				_model->Transform(FreyjaModel::TransformMesh, 
								  Egg::ROTATE, 0.0, -0.5, 0.0);
				break;
			case Egg::PLANE_XZ:
				_model->Transform(FreyjaModel::TransformMesh, 
								  Egg::ROTATE, 0.0, 0.0, -0.5);
				break;
			case Egg::PLANE_YZ:
				_model->Transform(FreyjaModel::TransformMesh, 
								  Egg::ROTATE, 0.0, 0.0, -0.5);
				break;
			}
		}
		else if (x < old_x)
		{
			switch (plane)
			{
			case Egg::PLANE_XY:
				_model->Transform(FreyjaModel::TransformMesh, 
								  Egg::ROTATE, 0.0, 0.5, 0.0);
				break;
			case Egg::PLANE_XZ:
				_model->Transform(FreyjaModel::TransformMesh, 
								  Egg::ROTATE, 0.0, 0.0, 0.5);
				break;
			case Egg::PLANE_YZ:
				_model->Transform(FreyjaModel::TransformMesh, 
								  Egg::ROTATE, 0.0, 0.0, 0.5);
			}
		}


		if (y > old_y)
		{
			switch (plane)
			{
			case Egg::PLANE_XY:
				_model->Transform(FreyjaModel::TransformMesh, 
								  Egg::ROTATE, -0.5, 0.0, 0.0);
				break;
			case Egg::PLANE_XZ:
				_model->Transform(FreyjaModel::TransformMesh, 
								  Egg::ROTATE, -0.5, 0.0, 0.0);
				break;
			case Egg::PLANE_YZ:
				_model->Transform(FreyjaModel::TransformMesh, 
								  Egg::ROTATE, 0.0, -0.5, 0.0);
				break;
			}
		}
		else if (y < old_y)
		{
			switch (plane)
			{
			case Egg::PLANE_XY:
				_model->Transform(FreyjaModel::TransformMesh, 
								  Egg::ROTATE, 0.5, 0.0, 0.0);
				break;
			case Egg::PLANE_XZ:
				_model->Transform(FreyjaModel::TransformMesh, 
								  Egg::ROTATE, 0.5, 0.0, 0.0);
				break;
			case Egg::PLANE_YZ:
				_model->Transform(FreyjaModel::TransformMesh, 
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
					_model->Transform(FreyjaModel::TransformMesh, 
									  Egg::SCALE, 1.0, 0.99, 1.0);
					break;
				case Egg::PLANE_XZ:
					_model->Transform(FreyjaModel::TransformMesh, 
									  Egg::SCALE, 1.0, 1.0, 0.99);
					break;
				case Egg::PLANE_YZ:
					_model->Transform(FreyjaModel::TransformMesh, 
									  Egg::SCALE, 1.0, 1.0, 0.99);
				break;
				}
			}
			else if (y < old_y)
			{
				switch (plane)
				{
				case Egg::PLANE_XY:
					_model->Transform(FreyjaModel::TransformMesh, 
									  Egg::SCALE, 1.0, 1.01, 1.0);
					break;
				case Egg::PLANE_XZ:
					_model->Transform(FreyjaModel::TransformMesh, 
									  Egg::SCALE, 1.0, 1.0, 1.01);
					break;
				case Egg::PLANE_YZ:
					_model->Transform(FreyjaModel::TransformMesh, 
									  Egg::SCALE, 1.0, 1.0, 1.01);
				}
			}
			
			if (x < old_x)
			{
				switch (plane)
				{
				case Egg::PLANE_XY:
					_model->Transform(FreyjaModel::TransformMesh, 
									  Egg::SCALE, 0.99, 1.0, 1.0);
					break;
				case Egg::PLANE_XZ:
					_model->Transform(FreyjaModel::TransformMesh, 
									  Egg::SCALE, 0.99, 1.0, 1.0);
					break;
				case Egg::PLANE_YZ:
					_model->Transform(FreyjaModel::TransformMesh, 
									  Egg::SCALE, 1.0, 0.99, 1.0);
					break;
				}
			}
			else if (x > old_x)
			{
				switch (plane)
				{
				case Egg::PLANE_XY:
					_model->Transform(FreyjaModel::TransformMesh, 
									  Egg::SCALE, 1.01, 1.0, 1.0);
					break;
				case Egg::PLANE_XZ:
					_model->Transform(FreyjaModel::TransformMesh, 
									  Egg::SCALE, 1.01, 1.0, 1.0);
					break;
				case Egg::PLANE_YZ:
					_model->Transform(FreyjaModel::TransformMesh, 
									  Egg::SCALE, 1.0, 1.01, 1.0);
				}
			}
				
			old_x = x;
			old_y = y;
			break;

  case VERTEX_BBOX_SELECT_MODE:
    _model->BBoxMove(xx, yy);
    break;
  case TAG_MOVE:
    _model->TagMove(xx, yy);
    break;
  case TAG_MOVE_CENTER:
    _model->TagMoveCenter(xx, yy);
    break;
  case MESH_MOVE:
    _model->MeshMove(xx, yy);
    break;
  case MESH_MOVE_CENTER: 
    _model->MeshMoveCenter(xx, yy);
    break;
  case VERTEX_MOVE:
    _model->VertexMove(xx, yy);
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
			_model->VertexCombine(xx, yy);
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
			_model->VertexSelect(xx, yy);
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
			_model->BBoxSelect(xx, yy);
			_mouse_state = 1;
		}
		else
			_mouse_state = 0;
      break;
	case MESH_SELECT_MODE:
		_model->MeshSelect(xx, yy);
		break;
	case POINT_DEL_MODE:
		_model->VertexSelect(xx, yy);
		_model->VertexDelete();
		break;
	case BONE_SELECT_MODE:
		_model->TagSelect(xx, yy);
		break;
	case BONE_CONNECT_MODE:
		master_tag = _model->getCurrentBone();
		_model->TagSelect(xx, yy);
		_model->TagConnect(master_tag, _model->getCurrentBone());
		_model->setCurrentBone(master_tag);
		break;
	case BONE_DISCONNECT_MODE:
		master_tag = _model->getCurrentBone();
		_model->TagSelect(xx, yy);
		_model->TagDisconnect(master_tag, _model->getCurrentBone());
		_model->setCurrentBone(master_tag);
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
		_model->VertexNew(xx, yy);
		break;
	case POLYGON_ADD_MODE:
		_model->PolygonAddVertex(xx, yy);
		break;
	case POLYGON_DEL_MODE:
		_model->PolygonDelVertex(xx, yy);
		break;
	case POLYGON_SELECT_MODE:
		_model->PolygonSelectVertex(xx, yy);
		break;
	case MESH_WHOLE_COPY_MODE:
		break;
	case MESH_PASTE_MODE:
		event_print("Mesh paste not impelmented\n");
		break;
	case MESH_COPY_MODE:
	case MESH_CUT_MODE:
		_model->VertexBuffer(xx, yy);
	default:
		;
	}
}

